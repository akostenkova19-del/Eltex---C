#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include "phoneBook.h"

int main() {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");

    Node* head = NULL;
    int count = 0;
    int choice = 0;

    do {
        printf("1. Добавить контакт\n");
        printf("2. Найти контакт по фамилии\n");
        printf("3. Редактировать контакт\n");
        printf("4. Удалить контакт\n");
        printf("5. Вывести все контакты\n");
        printf("6. Выход\n");

        printf("Выберите пункт меню: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка, введите число\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        switch (choice) {
            case 1: {
                Record newRec = {0};

                printf("Введите имя: ");
                scanf("%s", newRec.firstName);

                printf("Введите фамилию: ");
                scanf("%s", newRec.lastName);

                printf("Введите отчество: ");
                scanf("%s", newRec.patronymic);

                printf("Введите место работы: ");
                scanf("%s", newRec.jobPlace);

                printf("Введите должность: ");
                scanf("%s", newRec.post);

                printf("Введите номер телефона: ");
                scanf("%s", newRec.phoneNumber);

                printf("Введите email: ");
                scanf("%s", newRec.mailAddress);

                int profileCount = 0;

                printf("Сколько ссылок на соцсети/мессенджеры хотите добавить (от 0 до %d): ", MAX_PROFILES);

                if (scanf("%d", &profileCount) != 1 ||
                    profileCount < 0 || profileCount > MAX_PROFILES) {

                    printf("Некорректное количество ссылок\n");
                    break;
                }

                for (int i = 0; i < profileCount; i++) {
                    printf("Введите ссылку #%d: ", i + 1);
                    scanf("%199s", newRec.profileAddresses[i]);
                }

                if (AddContact(&head, &count, newRec) == 0) {
                    printf("Контакт успешно добавлен\n");
                } else {
                    printf("Не удалось добавить контакт\n");
                }
                break;
            }
            case 2: {
                char searchLastName[MAX_FIELD_LEN];
                printf("Введите фамилию для поиска: ");
                scanf("%s", searchLastName);

                Node* contact = FindContactByLastName(head, searchLastName);

                if (contact != NULL) {
                    printf("Найден контакт: %s %s %s\n", contact->data.lastName, contact->data.firstName, contact->data.patronymic);
                    printf("Телефон: %s\n", contact->data.phoneNumber);
                } else {
                    printf("Контакт не найден.\n");
                }
                break;
            }
            case 3: {
                char searchLastName[MAX_FIELD_LEN];
                printf("Введите фамилию контакта для редактирования: ");
                scanf("%s", searchLastName);
                Node* contact = FindContactByLastName(head, searchLastName);

                if (contact != NULL) {
                    int fieldChoice = -1;
                    do {

                        printf("\nРедактирование контакта: %s %s\n", contact->data.lastName, contact->data.firstName);
                        printf("1. Имя: %s\n", contact->data.firstName);
                        printf("2. Фамилия: %s\n", contact->data.lastName);
                        printf("3. Отчество: %s\n", contact->data.patronymic);
                        printf("4. Место работы: %s\n", contact->data.jobPlace);
                        printf("5. Должность: %s\n", contact->data.post);
                        printf("6. Телефон: %s\n", contact->data.phoneNumber);
                        printf("7. Email: %s\n",contact->data.mailAddress);
                        for (int i = 0; i < MAX_PROFILES; i++) {
                            printf("%d. Ссылка на профиль #%d: %s\n", 8 + i, i + 1,contact->data.profileAddresses[i]);
                        }

                        printf("0. Выйти из режима редактирования\n");
                        printf("Выберите поле для изменения: ");

                        if (scanf("%d", &fieldChoice) != 1) {
                            printf("Введите число\n");
                            continue;
                        }
                        if (fieldChoice >= 1 && fieldChoice <= 7 + MAX_PROFILES) {
                            char newValue[MAX_FIELD_LEN];
                            printf("Введите новое значение: ");
                            scanf("%199s", newValue);

                            if (EditContactField(&head, contact, fieldChoice, newValue) == 0) {
                                printf("Поле успешно изменено\n");
                            } else {
                                printf("Ошибка при изменении поля\n");
                            }
                        } else if (fieldChoice == 0) {
                            printf("Выход из режима редактирования\n");
                        } else {
                            printf("Неверный выбор. Попробуйте снова.\n");
                        }
                    } while (fieldChoice != 0);

                } else {
                    printf("Контакт для редактирования не найден.\n");
                }
                break;
            }
            case 4: {
                char searchLastName[MAX_FIELD_LEN];
                printf("Введите фамилию контакта для удаления: ");
                scanf("%s", searchLastName);

                Node* contact = FindContactByLastName(
                    head, searchLastName
                );
                if (contact != NULL) {
                    if (DeleteContact(&head, &count, contact) == 0) {
                        printf("Контакт успешно удален\n");
                    } else {
                        printf("Ошибка удаления контакта\n");
                    }
                } else {
                    printf("Контакт для удаления не найден.\n");
                }
                break;
            }
            case 5: {
                PrintList(head);
                break;
            }
            case 6:
                printf("Выход из программы\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 6);
    return 0;
}