#include "unity.h"
#include "phoneBook.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_AddContact(void) {
    Record contacts[MAX_CONTACTS];
    int count = 0;

    Record newRec = {0};
    strcpy(newRec.firstName, "Иван");
    strcpy(newRec.lastName, "Иванов");
    strcpy(newRec.phoneNumber, "1234567890");

    int result = AddContact(contacts, &count, newRec);
    
    TEST_ASSERT_EQUAL_INT(0, result); 
    TEST_ASSERT_EQUAL_INT(1, count);  
    TEST_ASSERT_EQUAL_STRING("Иванов", contacts[0].lastName); 
}

void test_AddContact_Overflow(void) {
    Record contacts[MAX_CONTACTS];
    int count = MAX_CONTACTS; 

    Record newRec = {0};
    int result = AddContact(contacts, &count, newRec);
    
    TEST_ASSERT_EQUAL_INT(-1, result); 
    TEST_ASSERT_EQUAL_INT(MAX_CONTACTS, count);
}

void test_FindContactByLastName(void) {
    Record contacts[MAX_CONTACTS];
    int count = 0;

    Record r1 = {0};
    strcpy(r1.lastName, "Петров");
    AddContact(contacts, &count, r1);

    Record r2 = {0};
    strcpy(r2.lastName, "Сидоров");
    AddContact(contacts, &count, r2);

    int index = FindContactByLastName(contacts, count, "Сидоров");
    TEST_ASSERT_EQUAL_INT(1, index); 

    int notFound = FindContactByLastName(contacts, count, "Кузнецов");
    TEST_ASSERT_EQUAL_INT(-1, notFound); 
}

void test_EditContactField(void) {
    Record contact = {0};
    strcpy(contact.firstName, "Иван");
    strcpy(contact.phoneNumber, "1234567890");

    int res1 = EditContactField(&contact, 1, "Петр");
    TEST_ASSERT_EQUAL_INT(0, res1);
    TEST_ASSERT_EQUAL_STRING("Петр", contact.firstName);

    int res2 = EditContactField(&contact, 8, "t.me/petr");
    TEST_ASSERT_EQUAL_INT(0, res2);
    TEST_ASSERT_EQUAL_STRING("t.me/petr", contact.profileAddresses[0]);

    int res3 = EditContactField(&contact, 99, "ошибка");
    TEST_ASSERT_EQUAL_INT(-1, res3);
}

void test_DeleteContact(void) {
    Record contacts[MAX_CONTACTS];
    int count = 0;

    Record r1 = {0}, r2 = {0};
    strcpy(r1.lastName, "Первый");
    strcpy(r2.lastName, "Второй");
    
    AddContact(contacts, &count, r1);
    AddContact(contacts, &count, r2); 

    int result = DeleteContact(contacts, &count, 0);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, count); 
    TEST_ASSERT_EQUAL_STRING("Второй", contacts[0].lastName);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_AddContact);
    RUN_TEST(test_AddContact_Overflow);
    RUN_TEST(test_FindContactByLastName);
    RUN_TEST(test_EditContactField);
    RUN_TEST(test_DeleteContact);
    
    return UNITY_END();
}