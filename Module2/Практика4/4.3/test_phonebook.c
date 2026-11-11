#include "unity.h"
#include "phoneBook.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

void setUp(void) {}
void tearDown(void) {}

Record CreateRecord(const char* firstName, const char* lastName) {
    Record rec = {0};

    strcpy(rec.firstName, firstName);
    strcpy(rec.lastName, lastName);

    return rec;
}

int CheckAVL(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = CheckAVL(root->left);
    int rightHeight = CheckAVL(root->right);

    if (leftHeight == -1 || rightHeight == -1) {
        return -1;
    }

    if (leftHeight - rightHeight > 1 || rightHeight - leftHeight > 1) {
        return -1;
    }

    int expectedHeight = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;

    if (root->height != expectedHeight) {
        return -1;
    }

    return expectedHeight;
}

void test_AddContact(void) {
    Node* root = NULL;
    int count = 0;

    Record r1 = CreateRecord("Александр", "Александров");
    Record r2 = CreateRecord("Борис", "Борисов");
    Record r3 = CreateRecord("Виктор", "Воронов");

    int result = AddContact(&root, &count, r1);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, count);

    printf("\nПосле добавления Александрова:\n");
    PrintTree(root);

    AddContact(&root, &count, r3);
    printf("\nПосле добавления Воронова:\n");
    PrintTree(root);

    AddContact(&root, &count, r2);
    printf("\nПосле добавления Борисова:\n");
    PrintTree(root);

    printf("\nОбычный упорядоченный список контактов:\n");
    PrintList(root);

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_TRUE(CheckAVL(root) >= 0);

    DeleteTree(&root);
}

void test_FindContactByLastName(void) {
    Node* root = NULL;
    int count = 0;

    Record r1 = CreateRecord("Александр", "Александров");
    Record r2 = CreateRecord("Борис", "Борисов");
    Record r3 = CreateRecord("Виктор", "Воронов");

    AddContact(&root, &count, r1);
    AddContact(&root, &count, r2);
    AddContact(&root, &count, r3);

    Node* contact = FindContactByLastName(root, "Борисов");

    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Борис", contact->data.firstName);

    Node* notFound = FindContactByLastName(root, "Кузнецов");

    TEST_ASSERT_NULL(notFound);

    DeleteTree(&root);
}

void test_EditContactField_LastName(void) {
    Node* root = NULL;
    int count = 0;

    Record r1 = CreateRecord("Александр", "Александров");
    Record r2 = CreateRecord("Борис", "Борисов");
    Record r3 = CreateRecord("Виктор", "Воронов");

    AddContact(&root, &count, r1);
    AddContact(&root, &count, r2);
    AddContact(&root, &count, r3);

    printf("\nДерево до изменения фамилии:\n");
    PrintTree(root);

    Node* contact = FindContactByLastName(root, "Александров");

    TEST_ASSERT_NOT_NULL(contact);

    int result = EditContactField(&root, contact, 2, "Яковлев");

    TEST_ASSERT_EQUAL_INT(0, result);

    printf("\nДерево после изменения фамилии Александрова на Яковлев:\n");
    PrintTree(root);

    TEST_ASSERT_NULL(FindContactByLastName(root, "Александров"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Яковлев"));

    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Борисов"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Воронов"));

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_TRUE(CheckAVL(root) >= 0);

    DeleteTree(&root);
}

void test_DeleteContact(void) {
    Node* root = NULL;
    int count = 0;

    Record r1 = CreateRecord("Александр", "Александров");
    Record r2 = CreateRecord("Андрей", "Баранов");
    Record r3 = CreateRecord("Алексей", "Белов");
    Record r4 = CreateRecord("Борис", "Борисов");
    Record r5 = CreateRecord("Сергей", "Васильев");
    Record r6 = CreateRecord("Виктор", "Воронов");
    Record r7 = CreateRecord("Григорий", "Борисенко");

    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r1));
    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r2));
    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r3));
    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r4));
    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r5));
    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r6));
    TEST_ASSERT_EQUAL_INT(0, AddContact(&root, &count, r7));

    TEST_ASSERT_EQUAL_INT(7, count);
    TEST_ASSERT_EQUAL_STRING("Борисов", root->data.lastName);

    printf("\nДерево до удаления Борисова:\n");
    PrintTree(root);

    printf("\nУдаляем Борисова, у которого два потомка\n");
    printf("Ищем максимальный элемент в левом поддереве - Борисенко\n");

    TEST_ASSERT_NOT_NULL(root->left);
    TEST_ASSERT_NOT_NULL(root->left->right);
    TEST_ASSERT_NOT_NULL(root->left->right->right);
    TEST_ASSERT_NOT_NULL(root->right);

    TEST_ASSERT_EQUAL_STRING("Борисенко", root->left->right->right->data.lastName);

    Node* contact = FindContactByLastName(root, "Борисов");

    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_INT(0, DeleteContact(&root, &count, contact));

    TEST_ASSERT_EQUAL_INT(6, count);

    printf("\nДерево после удаления Борисова:\n");
    PrintTree(root);

    TEST_ASSERT_EQUAL_STRING("Борисенко", root->data.lastName);
    TEST_ASSERT_EQUAL_STRING("Баранов", root->left->data.lastName);
    TEST_ASSERT_EQUAL_STRING("Белов", root->left->right->data.lastName);
    TEST_ASSERT_EQUAL_STRING("Васильев", root->right->data.lastName);

    TEST_ASSERT_NULL(FindContactByLastName(root, "Борисов"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Борисенко"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Александров"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Баранов"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Белов"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Васильев"));
    TEST_ASSERT_NOT_NULL(FindContactByLastName(root, "Воронов"));

    TEST_ASSERT_TRUE(CheckAVL(root) >= 0);

    DeleteTree(&root);
}

void test_AVL_Balancing(void) {
    Node* root = NULL;
    int count = 0;

    Record r1 = CreateRecord("Александр", "Александров");
    Record r2 = CreateRecord("Борис", "Борисов");
    Record r3 = CreateRecord("Виктор", "Воронов");

    AddContact(&root, &count, r1);
    printf("\nПосле добавления Александрова:\n");
    PrintTree(root);

    AddContact(&root, &count, r2);
    printf("\nПосле добавления Борисова:\n");
    PrintTree(root);

    AddContact(&root, &count, r3);
    printf("\nПосле добавления Воронова и балансировки:\n");
    PrintTree(root);

    TEST_ASSERT_NOT_NULL(root);
    TEST_ASSERT_EQUAL_STRING("Борисов", root->data.lastName);
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(2, CheckAVL(root));

    DeleteTree(&root);
}

int main(void) {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");
    UNITY_BEGIN();

    RUN_TEST(test_AddContact);
    RUN_TEST(test_FindContactByLastName);
    RUN_TEST(test_EditContactField_LastName);
    RUN_TEST(test_DeleteContact);
    RUN_TEST(test_AVL_Balancing);

    return UNITY_END();
}
