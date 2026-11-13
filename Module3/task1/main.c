#include "copy.h"

int main(int argc, char *args[]) {

    if (argc == 1) {
        write(STDERR_FILENO, "Пустой ввод\n", sizeof("Пустой ввод\n") - 1);
        return 1;
    }

    Arguments ar = {0};
    ar.fileNames = malloc((argc - 1) * sizeof(char *));

    if (ar.fileNames == NULL) {
        perror("malloc");
        return 1;
    }

    if (parse_args(argc, args, &ar) != 0 || ar.count == 0) {
        if (ar.count == 0)
            write(STDERR_FILENO, "Не указаны файлы\n", sizeof("Не указаны файлы\n") - 1);
        free(ar.fileNames);
        return 1;
    }

    int fd1[2] = {-1, -1};
    int fd2[2] = {-1, -1};

    char data_name[PATH_MAX];
    char ready_name[PATH_MAX];

    if (ar.pipeName == NULL) {
        if (pipe(fd1) == -1) {
            perror("pipe fd1");
            free(ar.fileNames);
            return 1;
        }

        if (pipe(fd2) == -1) {
            perror("pipe fd2");
            close(fd1[0]);
            close(fd1[1]);
            free(ar.fileNames);
            return 1;
        }
    }
    else {
        if (strlen(ar.pipeName) + sizeof(".ready") > PATH_MAX) {
            write(STDERR_FILENO, "Слишком длинное имя канала\n",sizeof("Слишком длинное имя канала\n") - 1);
            free(ar.fileNames);
            return 1;
        }

        snprintf(data_name, sizeof(data_name), "%s.data", ar.pipeName);
        snprintf(ready_name, sizeof(ready_name),"%s.ready", ar.pipeName);

        if (mkfifo(data_name, 0600) == -1) {
            perror("mkfifo data");
            free(ar.fileNames);
            return 1;
        }

        if (mkfifo(ready_name, 0600) == -1) {
            perror("mkfifo ready");
            unlink(data_name);
            free(ar.fileNames);
            return 1;
        }
    }

    pid_t pid = fork();

    int result = copy_files(&ar, pid, fd1, fd2);

    if (pid == -1) {
        if (ar.pipeName == NULL) {
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
        }
        else {
            unlink(data_name);
            unlink(ready_name);
        }
    }

    free(ar.fileNames);
    return result;
}
