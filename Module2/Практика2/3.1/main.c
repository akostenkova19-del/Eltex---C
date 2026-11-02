#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "perm.h"

#define SIZE 64

int main(void)
{
    setlocale(LC_ALL,".UTF-8");
    mode_t mode = 0;
    int initialized = 0;
    int choice;

    char input[SIZE];

    while (1)
    {
        printf("1. Ввести права доступа\n");
        printf("2. Получить права доступа файла\n");
        printf("3. Изменить права доступа \n");
        printf("0. Выход\n");
        printf("Выберите действие: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Ошибка ввода\n");
            continue;
        }

        if (choice == 0)
        {
            printf("Завершение программы.\n");
            break;
        }

        switch (choice)
        {
            case 1:
                printf("Введите права: ");

                if (fgets(input, SIZE, stdin) == NULL)
                    break;

                input[strcspn(input, "\n")] = '\0';

                if (parse_permissions(input, &mode))
                {
                    initialized = 1;
                    print_permissions(mode);
                }
                else
                {
                    printf("Некорректный формат прав доступа\n");
                }

                break;

            case 2:
                printf("Введите имя файла: ");

                if (fgets(input, SIZE, stdin) == NULL)
                    break;

                input[strcspn(input, "\n")] = '\0';

                if (get_file_permissions(input, &mode))
                {
                    initialized = 1;

                    printf("\nПрава доступа файла:\n");
                    print_permissions(mode);
                }
                break;

            case 3:
                if (!initialized)
                {
                    printf("Сначала введите права или выберите файл\n");
                    break;
                }

                printf("Текущие права:\n");
                print_permissions(mode);

                printf("\nВведите права или команду chmod:\n");
                printf("Примеры: 754, 644, u+x, g-w, o=r\n");
                printf("Ваш ввод: ");

                if (fgets(input, SIZE, stdin) == NULL)
                    break;

                input[strcspn(input, "\n")] = '\0';

                if (modify_permissions(&mode, input))
                {
                    printf("\nРезультат изменения прав:\n");
                    print_permissions(mode);
                }
                else
                {
                    printf("Некорректная команда или формат прав!\n");
                }

                break;

            default:
                printf("Выберите из списка\n");
        }
    }

    return 0;
}
