#ifndef COPY_H
#define COPY_H

#include <unistd.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>


typedef struct {
    char **fileNames;
    int count;
    char *pipeName;
} Arguments;

typedef struct {
    char filename[PATH_MAX];
    uint64_t file_size;
} FileHeader;

int parse_args(int argc, char* args[], Arguments *arguments);
int copy_files(Arguments *arguments, pid_t pid, int fd1[2], int fd2[2]);

#endif