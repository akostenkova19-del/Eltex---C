#include <stdio.h>
#include "calc.h"
#include <locale.h>

int main(void) {
    setlocale(LC_ALL, ".UTF-8");
    float a, b, result;
    char action;
    int success;

    printf("Введите первый операнд (разделитель для дробного числа - ','): ");
    scanf("%f", &a);

    printf("Введите второй операнд разделитель для дробного числа - ','): ");
    scanf("%f", &b);


    while (action != 'q') {
        printf("Введите действие (+, -, *, /) или q для выхода: ");
        scanf(" %c", &action);


        if (action != '+' && action != '-' &&
            action != '*' && action != '/') {
            printf("Неизвестное действие, попробуйте снова\n");
            continue;
        }


        result = Execute(a, b, action, &success);

        if (success) {
            printf("Результат: %g\n", result);
        } else {
            printf("Деление на ноль\n");
        }
    }

    printf("Программа завершена\n");
    return 0;
}