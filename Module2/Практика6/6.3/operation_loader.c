#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

#include "operation_loader.h"

#define MAX_LOADED_LIBRARIES 4

static void *library_handles[MAX_LOADED_LIBRARIES];
static size_t library_count;

static int has_so_extension(const char *filename)
{
    size_t length = strlen(filename);
    return length >= 3 && strcmp(filename + length - 3, ".so") == 0;
}

static int find_operation_in_library(const char *path, Operation operations[],size_t operation_count)
{
    void *handle = dlopen(path, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "Не удалось загрузить %s: %s\n", path, dlerror());
        return 0;
    }

    for (size_t i = 0; i < operation_count; ++i) {
        if (operations[i].function != NULL)
            continue;

        dlerror();
        operation_function function = (operation_function)dlsym(handle, operations[i].function_name);
        const char *error = dlerror();
        if (error == NULL && function != NULL) {
            if (library_count >= MAX_LOADED_LIBRARIES) {
                fprintf(stderr, "Слишком много динамических библиотек.\n");
                dlclose(handle);
                return 0;
            }
            operations[i].function = function;
            library_handles[library_count++] = handle;
            return 1;
        }
    }

    dlclose(handle);
    return 0;
}

size_t load_operations(const char *directory, Operation operations[])
{
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Не удалось открыть каталог библиотек");
        return 0;
    }

    size_t loaded_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!has_so_extension(entry->d_name))
            continue;

        char path[1024];
        int length = snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        if (length < 0 || (size_t)length >= sizeof(path)) {
            fprintf(stderr, "Не удалось сформировать путь: %s\n", entry->d_name);
            continue;
        }

        if (find_operation_in_library(path, operations, MAX_OPERATIONS)) loaded_count++;
    }

    closedir(dir);
    return loaded_count;
}

void unload_operations(void)
{
    while (library_count > 0)
        dlclose(library_handles[--library_count]);
}
