#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#define MAX_CONTACTS 100
#define MAX_FIELD_LEN 200
#define PHONE_NUMBER_LEN 30
#define MAX_PROFILES 5

typedef struct Record {
    char firstName[MAX_FIELD_LEN];
    char lastName[MAX_FIELD_LEN];
    char patronymic[MAX_FIELD_LEN];
    char jobPlace[MAX_FIELD_LEN];
    char post[MAX_FIELD_LEN];
    char phoneNumber[PHONE_NUMBER_LEN];
    char mailAddress[MAX_FIELD_LEN];
    char profileAddresses[MAX_PROFILES][MAX_FIELD_LEN];
} Record;

typedef struct Node {
    Record data;
    struct Node* next;
    struct Node* prev;
} Node;

int AddContact(Node** head, int* count, Record newRecord);
Node* FindContactByLastName(Node* head, const char* lastName);
int EditContactField(Node** head, Node* contact, int fieldChoice, const char* newValue);
int DeleteContact(Node** head, int* count, Node* contact);
void PrintList(Node* head);

#endif