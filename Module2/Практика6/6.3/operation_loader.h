#ifndef OPERATION_LOADER_H
#define OPERATION_LOADER_H

#include <stddef.h>
#include "operation.h"

size_t load_operations(const char *directory, Operation operations[]);
void unload_operations(void);

#endif
