
#ifndef PERM_H
#define PERM_H

#include <sys/types.h>

int parse_permissions(const char*, mode_t*);

void print_permissions(mode_t);

int get_file_permissions(const char*, mode_t*);

int modify_permissions(mode_t*, const char*);

#endif
