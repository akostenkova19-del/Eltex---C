#include <stdio.h>
#include <locale.h>

#include "operation.h"
#include "operation_loader.h"

#define LIBRARY_DIRECTORY "./libs"

int main(void)
{
    setlocale(LC_ALL, "UTF-8");

    Operation operations[MAX_OPERATIONS] = {
        {"Сложение", "add", NULL},
        {"Вычитание", "sub", NULL},
        {"Умножение", "mul", NULL},
        {"Деление", "divide", NULL}
    };

    size_t loaded_count = load_operations(LIBRARY_DIRECTORY, operations);

    if (loaded_count == 0) {
        printf("Не найдено библиотек с поддерживаемыми функциями в %s.\n", LIBRARY_DIRECTORY);
        unload_operations();
        return 1;
    }

    float a, b, result;

    printf("Введите первый операнд: ");
    if (scanf("%f", &a) != 1) {
        printf("Некорректное число.\n");
        unload_operations();
        return 1;
    }

    printf("Введите второй операнд: ");
    if (scanf("%f", &b) != 1) {
        printf("Некорректное число.\n");
        unload_operations();
        return 1;
    }

    char action = '\0';

    while (action != 'q') {
        
        printf("\nДоступные операции: ");

        for (size_t i = 0; i < MAX_OPERATIONS; ++i) {
            if (operations[i].function != NULL)
                printf("%s  ", operations[i].menu_name);
        }

        printf("\nВведите действие (+, -, *, /) или q для выхода: ");

        if (scanf(" %c", &action) != 1)
            break;

        if (action == 'q')
            break;

        switch (action) {
            case '+':
                if (operations[0].function == NULL) {
                    printf("Сложение не загружено.\n");
                    break;
                }
                result = operations[0].function(a, b);
                printf("Результат: %g\n", result);
                break;

            case '-':
                if (operations[1].function == NULL) {
                    printf("Вычитание не загружено.\n");
                    break;
                }
                result = operations[1].function(a, b);
                printf("Результат: %g\n", result);
                break;

            case '*':
                if (operations[2].function == NULL) {
                    printf("Умножение не загружено.\n");
                    break;
                }
                result = operations[2].function(a, b);
                printf("Результат: %g\n", result);
                break;

            case '/':
                if (operations[3].function == NULL) {
                    printf("Деление не загружено.\n");
                    break;
                }
                if (b == 0.0f) {
                    printf("Деление на ноль.\n");
                    break;
                }
                result = operations[3].function(a, b);
                printf("Результат: %g\n", result);
                break;

            default:
                printf("Неизвестное действие, попробуйте снова.\n");
                break;
        }
    }

    unload_operations();
    return 0;
}