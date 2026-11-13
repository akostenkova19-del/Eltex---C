#include "unity.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static char initial_dir[PATH_MAX];
static char test_dir[PATH_MAX];
static char program[PATH_MAX];

void setUp(void)
{
    char pattern[] = "/tmp/copy_test_XXXXXX";
    char *path = mkdtemp(pattern);

    TEST_ASSERT_NOT_NULL(path);

    strcpy(test_dir, path);
    TEST_ASSERT_EQUAL_INT(0, chdir(test_dir));
}

void tearDown(void)
{
    DIR *dir = opendir(".");

    if (dir != NULL) {
        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                unlink(entry->d_name);
            }
        }

        closedir(dir);
    }

    chdir(initial_dir);
    rmdir(test_dir);
}

static void create_file(const char *name, const void *data, size_t size)
{
    FILE *file = fopen(name, "wb");

    TEST_ASSERT_NOT_NULL(file);

    if (size > 0) {
        TEST_ASSERT_TRUE(fwrite(data, 1, size, file) == size);
    }

    TEST_ASSERT_EQUAL_INT(0, fclose(file));
}

static void assert_same_files(const char *source, const char *copy)
{
    FILE *a = fopen(source, "rb");
    FILE *b = fopen(copy, "rb");

    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);

    int x, y;

    do {
        x = fgetc(a);
        y = fgetc(b);

        TEST_ASSERT_EQUAL_INT(x, y);
    } while (x != EOF);

    TEST_ASSERT_EQUAL_INT(0, ferror(a));
    TEST_ASSERT_EQUAL_INT(0, ferror(b));

    fclose(a);
    fclose(b);
}

static int run_program(char *const args[])
{
    pid_t child = fork();

    if (child == -1) {
        return -1;
    }

    if (child == 0) {
        int err = open("stderr.log", O_WRONLY | O_CREAT | O_TRUNC, 0600);

        if (err == -1 || dup2(err, STDERR_FILENO) == -1) {
            _exit(127);
        }

        close(err);
        alarm(5);

        execv(program, args);
        _exit(127);
    }

    int status;

    while (waitpid(child, &status, 0) == -1) {
        if (errno != EINTR) {
            return -1;
        }
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return 128;
}

void test_CopyText(void)
{
    const char text[] = "Hello, Linux!\n";

    create_file("a.txt", text, sizeof(text) - 1);

    char *args[] = {program, "a.txt", NULL};

    TEST_ASSERT_EQUAL_INT(0, run_program(args));
    assert_same_files("a.txt", "a.txt.copy");
}

void test_CopyTwoFiles(void)
{
    create_file("a.txt", "First", 5);
    create_file("b.txt", "Second", 6);

    char *args[] = {
        program, "a.txt", "b.txt", NULL
    };

    TEST_ASSERT_EQUAL_INT(0, run_program(args));

    assert_same_files("a.txt", "a.txt.copy");
    assert_same_files("b.txt", "b.txt.copy");
}

void test_CopyEmptyFile(void)
{
    create_file("empty.txt", NULL, 0);

    char *args[] = {program, "empty.txt", NULL};

    TEST_ASSERT_EQUAL_INT(0, run_program(args));
    assert_same_files("empty.txt", "empty.txt.copy");
}

void test_NamedPipe(void)
{
    create_file("a.txt", "FIFO", 4);

    char *args[] = {program, "-p", "channel", "a.txt", NULL};

    TEST_ASSERT_EQUAL_INT(0, run_program(args));

    assert_same_files("a.txt", "a.txt.copy");

    TEST_ASSERT_EQUAL_INT(-1, access("channel.data", F_OK));

    TEST_ASSERT_EQUAL_INT(-1, access("channel.ready", F_OK));
}

void test_MissingFile(void)
{
    create_file("valid.txt", "OK", 2);

    char *args[] = {
        program, "missing.txt", "valid.txt", NULL
    };

    TEST_ASSERT_TRUE(run_program(args) != 0);

    assert_same_files("valid.txt", "valid.txt.copy");

    FILE *error = fopen("stderr.log", "rb");

    TEST_ASSERT_NOT_NULL(error);
    TEST_ASSERT_TRUE(fgetc(error) != EOF);

    fclose(error);
}

void test_NoArguments(void)
{
    char *args[] = {program, NULL};

    TEST_ASSERT_TRUE(run_program(args) != 0);
}

int main(void)
{
    if (getcwd(initial_dir, sizeof(initial_dir)) == NULL) {
        return 1;
    }

    int n = snprintf(program, sizeof(program),
                     "%s/filecopy", initial_dir);

    if (n < 0 || (size_t)n >= sizeof(program)) {
        return 1;
    }

    UNITY_BEGIN();

    RUN_TEST(test_CopyText);
    RUN_TEST(test_CopyTwoFiles);
    RUN_TEST(test_CopyEmptyFile);
    RUN_TEST(test_NamedPipe);
    RUN_TEST(test_MissingFile);
    RUN_TEST(test_NoArguments);

    return UNITY_END();
}
