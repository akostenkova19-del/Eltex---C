
#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#define MAX_FIELD_LEN 200
#define PHONE_NUMBER_LEN 30
#define MAX_PROFILES 5

typedef struct {
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
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int AddContact(Node** root, int* count, Record newRecord);

Node* FindContactByLastName(Node* root, const char* lastName);

int EditContactField(Node** root, Node* contact, int fieldChoice, const char* newValue);

int DeleteContact(Node** root, int* count, Node* contact);

void PrintList(Node* root);

void PrintTree(Node* root);

void DeleteTree(Node** root);

#endif
