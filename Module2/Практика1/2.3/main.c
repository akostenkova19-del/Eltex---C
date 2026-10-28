#include <stdio.h>
#include <string.h>
#include "calc.h"
#include <locale.h>
#include <windows.h>

int main(void)
{
    setlocale(LC_ALL,".UTF-8");
    char expression[MAX];
    Token rpn[MAX];
    int count;
    float result;

    while (1) {
        printf("Введите выражение или q дял выхода: ");

        if (fgets(expression, sizeof(expression), stdin) == NULL)
            break;

        expression[strcspn(expression, "\n")] = '\0';

        if (strcmp(expression, "q") == 0)
            break;

        if (to_rpn(expression, rpn, &count) == 0 &&
            calculate_rpn(rpn, count, &result) == 0) {
            printf("Ответ:  %.2f\n", result);
        }
        else {
            printf("Ошибка\n");
        }
    }

    return 0;
}