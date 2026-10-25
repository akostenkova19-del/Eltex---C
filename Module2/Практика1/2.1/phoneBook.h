#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#define MAX_CONTACTS 100
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

int AddContact(Record[], int*, Record);
int EditContactField(Record*, int, const char*);
int DeleteContact(Record[], int*, int);
int FindContactByLastName(const Record[], int, const char[]);

#endif