#ifndef OPERATION_H
#define OPERATION_H

#include <stddef.h>

#define MAX_OPERATIONS 4

typedef float (*operation_function)(float, float);

typedef struct {
    const char *menu_name;
    const char *function_name;
    operation_function function;
} Operation;

#endif
