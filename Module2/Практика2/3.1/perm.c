
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "perm.h"

static void mode_to_symbolic(mode_t mode, char result[10])
{
    const char symbols[] = "rwx";

    for (int i = 0; i < 9; i++)
    {
        if (mode & (1 << (8 - i)))
            result[i] = symbols[i % 3];
        else
            result[i] = '-';
    }
    result[9] = '\0';
}

int parse_permissions(const char *input, mode_t *mode)
{
    if (strlen(input) == 3)
    {
        int value = 0;

        for (int i = 0; i < 3; i++)
        {
            if (input[i] < '0' || input[i] > '7')
                return 0;

            value = value * 8 + (input[i] - '0');
        }

        *mode = value;
        return 1;
    }

    if (strlen(input) == 9)
    {
        const char expected[] = "rwxrwxrwx";
        mode_t result = 0;

        for (int i = 0; i < 9; i++)
        {
            if (input[i] == expected[i])
                result |= (1 << (8 - i));
            else if (input[i] != '-')
                return 0;
        }

        *mode = result;
        return 1;
    }

    return 0;
}

void print_permissions(mode_t mode)
{
    char symbolic[10];

    mode &= 0777;

    mode_to_symbolic(mode, symbolic);

    printf("\nСимвольное представление: %s\n", symbolic);
    printf("Числовое представление: %03o\n",
           (unsigned int)mode);

    printf("Бинарное представление:  ");

    for (int i = 8; i >= 0; i--)
    {
        printf("%d", (mode >> i) & 1);

        if (i == 6 || i == 3)
            printf(" ");
    }

    printf("\n");
}

int get_file_permissions(const char *filename, mode_t *mode)
{
    struct stat file_info;

    if (stat(filename, &file_info) == -1)
    {
        return 0;
    }

    *mode = file_info.st_mode & 0777;

    return 1;
}

int modify_permissions(mode_t *mode, const char *command)
{
    char who[10] = "";
    const char *p = command;
    const char *operation;

    mode_t who_mask = 0;
    mode_t permission_mask = 0;
    mode_t full_mask = 0;

    if (strlen(command) == 3)
    {
        int value = 0;
        int is_numeric = 1;

        for (int i = 0; i < 3; i++)
        {
            if (command[i] < '0' || command[i] > '7')
            {
                is_numeric = 0;
                break;
            }

            value = value * 8 + (command[i] - '0');
        }

        if (is_numeric)
        {
            *mode = value;
            return 1;
        }
    }

    while (*p == 'u' || *p == 'g' ||
           *p == 'o' || *p == 'a')
    {
        if (strlen(who) >= sizeof(who) - 1)
            return 0;

        who[strlen(who)] = *p;
        p++;
    }

    if (who[0] == '\0')
        strcpy(who, "a");

    if (*p != '+' && *p != '-' && *p != '=')
        return 0;

    operation = p;
    p++;

    while (*p != '\0')
    {
        switch (*p)
        {
            case 'r':
                permission_mask |= 4;
                break;

            case 'w':
                permission_mask |= 2;
                break;

            case 'x':
                permission_mask |= 1;
                break;

            default:
                return 0;
        }

        p++;
    }

    if (strchr(who, 'u') || strchr(who, 'a'))
        who_mask |= 0700;

    if (strchr(who, 'g') || strchr(who, 'a'))
        who_mask |= 0070;

    if (strchr(who, 'o') || strchr(who, 'a'))
        who_mask |= 0007;

    if (who_mask & 0700)
        full_mask |= permission_mask << 6;

    if (who_mask & 0070)
        full_mask |= permission_mask << 3;

    if (who_mask & 0007)
        full_mask |= permission_mask;

    switch (*operation)
    {
        case '+':
            *mode |= full_mask;
            break;

        case '-':
            *mode &= ~full_mask;
            break;

        case '=':
            *mode = (*mode & ~who_mask) | full_mask;
            break;
    }

    *mode &= 0777;

    return 1;
}