#include <copy.h>
#include <stdlib.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024


int parse_args(int argc, char* args[], Arguments *arguments){

    for (int i = 1; i < argc; i++){

        if (strcmp("-p", args[i]) == 0){

            if (i + 1 >= argc){
                write(STDERR_FILENO, "Имя канала не указано\n", sizeof("Имя канала не указано\n") - 1);
                return 1;
            }
            i++;
            arguments->pipeName = args[i];         
        }
        else {
            arguments->fileNames[arguments->count] = args[i];
            arguments->count++;
            }
        }
    return 0;
}

static int write_all(int fd, const void *buffer, size_t count)
{
    const char *p = buffer;

    while (count > 0) {
        ssize_t n = write(fd, p, count);

        if (n == -1 && errno == EINTR) continue;

        if (n <= 0) return -1;

        p += n;
        count -= (size_t)n;
    }
    return 0;
}

static int read_all(int fd, void *buffer, size_t count)
{
    char *p = buffer;

    while (count > 0) {
        ssize_t n = read(fd, p, count);

        if (n == -1 && errno == EINTR) continue;

        if (n <= 0) return -1;

        p += n;
        count -= (size_t)n;
    }
    return 0;
}

static int receive_files(int data_fd, int ready_fd)
{
    char buffer[BUFFER_SIZE];
    char ready = 'R';
    int result = 0;

    while (1) {

        if (write_all(ready_fd, &ready, sizeof(ready)) == -1) {
            perror("Ошибка отправки сообщения о готовности");
            return 1;
        }

        FileHeader header;

        if (read_all(data_fd, &header, sizeof(header)) == -1) {
            perror("Ошибка получения FileHeader");
            return 1;
        }

        if (header.filename[0] == '\0') {
            break;
        }

        size_t len = strlen(header.filename);

        char *copyname = malloc(len + sizeof(".copy"));

        if (copyname == NULL) {
            perror("malloc");
            return 1;
        }

        snprintf(copyname, len + sizeof(".copy"), "%s.copy", header.filename);

        int output = open(copyname, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        free(copyname);

        if (output == -1) {
            perror("Не удалось создать файл-копию");
            result = 1;
        }

        uint64_t remaining = header.file_size;

        while (remaining > 0) {

            size_t block_size = sizeof(buffer);

            if (remaining < block_size) {
                block_size = (size_t)remaining;
            }

            if (read_all(data_fd, buffer, block_size) == -1) {
                perror("Ошибка чтения из канала");

                if (output != -1) {
                    close(output);
                }

                return 1;
            }

            if (output != -1) {
                if (write_all(output, buffer, block_size) == -1) {
                    perror("Ошибка записи файла-копии");
                    close(output);
                    output = -1;
                    result = 1;
                }
            }

            remaining -= block_size;
        }

        if (output != -1) {
            if (close(output) == -1) {
                perror("Ошибка закрытия файла");
                result = 1;
            }
        }
    }
    return result;
}

static int send_files(const Arguments *arguments,int data_fd, int ready_fd){
    char buffer[BUFFER_SIZE];
    int result = 0;
    
    for(int i=0; i<arguments->count; i++) {
        const char *currentfile = arguments->fileNames[i];
        int input  = open(currentfile, O_RDONLY);
        if (input == -1){
            perror("open file");
            result = 1;
            continue;
        }

        struct stat st;

        if (fstat(input, &st) == -1) {
            perror("fstat");
            close(input);
            result = 1;
            continue;
        }

        FileHeader header = {0};
        memcpy(header.filename, currentfile, strlen(currentfile) + 1);
        header.file_size = (uint64_t)st.st_size;

        char ready;

        if (read_all(ready_fd, &ready, sizeof(ready)) == -1) {
            perror("Ошибка получения сообщения о готовности");
            close(input);
            return 1;
        }

        if (ready != 'R') {
            fprintf(stderr, "Некорректное сообщение о готовности\n");
            close(input);
            return 1;
        }

        if (write_all(data_fd, &header, sizeof(header)) == -1) {
            perror("Ошибка отправки FileHeader");
            close(input);
            return 1;
        }

        uint64_t remaining = header.file_size;

        while (remaining > 0) {

            size_t block_size = sizeof(buffer);

            if (remaining < block_size) {
                block_size = (size_t)remaining;
            }

            if (read_all(input, buffer, block_size) == -1) {
                perror("Ошибка чтения исходного файла");
                close(input);
                return 1;
            }

            if (write_all(data_fd, buffer, block_size) == -1) {
                perror("Ошибка передачи данных по каналу");
                close(input);
                return 1;
            }

            remaining -= block_size;
        }

        if (close(input) == -1) {
            perror("close");
            result = 1;
        }
    }

    char ready;

    if (read_all(ready_fd, &ready, sizeof(ready)) == -1) {
        perror("Ошибка получения последней готовности");
        return 1;
    }

    if (ready != 'R') {
        fprintf(stderr, "Некорректное сообщение о готовности\n");
        return 1;
    }

    FileHeader end = {0};

    if (write_all(data_fd, &end, sizeof(end)) == -1) {
        perror("Ошибка отправки сообщения о завершении");
        return 1;
    }

    return result;
}

int copy_files(Arguments *arguments, pid_t pid, int fd1[2], int fd2[2]){
    switch (pid) {
        case -1:
            perror("Ошибка fork");
            return 1;

        case 0: {
            int data_fd;
            int ready_fd;

            if (arguments->pipeName == NULL) {
                close(fd1[1]);
                close(fd2[0]);

                data_fd = fd1[0];
                ready_fd = fd2[1];
            }
            else {

                size_t len = strlen(arguments->pipeName);

                char *data_name = malloc(len + sizeof(".data"));
                char *ready_name = malloc(len + sizeof(".ready"));

                if (data_name == NULL || ready_name == NULL) {
                    perror("malloc");
                    free(data_name);
                    free(ready_name);
                    return 1;
                }

                snprintf(data_name, len + sizeof(".data"), "%s.data", arguments->pipeName);

                snprintf(ready_name, len + sizeof(".ready"), "%s.ready", arguments->pipeName);

                data_fd = open(data_name, O_RDONLY);

                if (data_fd == -1) {
                    perror("open data FIFO");
                    free(data_name);
                    free(ready_name);
                    return 1;
                }

                ready_fd = open(ready_name, O_WRONLY);

                free(data_name);
                free(ready_name);

                if (ready_fd == -1) {
                    perror("open ready FIFO");
                    close(data_fd);
                    return 1;
                }
            }

            int result = receive_files(data_fd, ready_fd);

            close(data_fd);
            close(ready_fd);

            return result;
        }
        
        default: {
            int data_fd;
            int ready_fd;

            char *data_name = NULL;
            char *ready_name = NULL;

            if (arguments->pipeName == NULL) {
                close(fd1[0]);
                close(fd2[1]);

                data_fd = fd1[1];
                ready_fd = fd2[0];
            }
            else {

                size_t len = strlen(arguments->pipeName);

                data_name = malloc(len + sizeof(".data"));
                ready_name = malloc(len + sizeof(".ready"));

                if (data_name == NULL || ready_name == NULL) {
                    perror("malloc");
                    free(data_name);
                    free(ready_name);
                    return 1;
                }

                snprintf(data_name, len + sizeof(".data"), "%s.data", arguments->pipeName);

                snprintf(ready_name, len + sizeof(".ready"),"%s.ready", arguments->pipeName);

                data_fd = open(data_name, O_WRONLY);

                if (data_fd == -1) {
                    perror("open data FIFO");
                    free(data_name);
                    free(ready_name);
                    return 1;
                }

                ready_fd = open(ready_name, O_RDONLY);

                if (ready_fd == -1) {
                    perror("open ready FIFO");
                    close(data_fd);
                    free(data_name);
                    free(ready_name);
                    return 1;
                }
            }

            int result = send_files(arguments, data_fd, ready_fd);

            close(data_fd);
            close(ready_fd);

            int status;

            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                result = 1;
            }
            else if (WEXITSTATUS(status) != 0) result = 1;

            if (data_name != NULL) {
                if (unlink(data_name) == -1) {
                    perror("unlink data FIFO");
                    result = 1;
                }
                free(data_name);
            }

            if (ready_name != NULL) {
                if (unlink(ready_name) == -1) {
                    perror("unlink ready FIFO");
                    result = 1;
                }
                free(ready_name);
            }
            return result;
        }

    }
}
