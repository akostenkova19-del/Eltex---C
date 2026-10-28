#include "phoneBook.h"
#include <stdio.h>
#include <string.h>

int FindContactByLastName(const Record contacts[], int count, const char lastName[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(contacts[i].lastName, lastName) == 0) {
            return i; 
        }
    }
    return -1; 
}

int AddContact(Record contacts[], int *count, Record newRecord) {
    if (*count < MAX_CONTACTS) {
        contacts[*count] = newRecord;
        (*count)++;
        return 0;
    }
    return -1;   
}

int EditContactField(Record *contact, int fieldChoice, const char *newValue) {
    switch (fieldChoice) {
        case 1:
            strncpy(contact->firstName, newValue, MAX_FIELD_LEN - 1);
            break;
        case 2:
            strncpy(contact->lastName, newValue, MAX_FIELD_LEN - 1);
            break;
        case 3:
            strncpy(contact->patronymic, newValue, MAX_FIELD_LEN - 1);
            break;
        case 4:
            strncpy(contact->jobPlace, newValue, MAX_FIELD_LEN - 1);
            break;
        case 5:
            strncpy(contact->post, newValue, MAX_FIELD_LEN - 1);
            break;
        case 6:
            strncpy(contact->phoneNumber, newValue, PHONE_NUMBER_LEN - 1);
            break;
        case 7:
            strncpy(contact->mailAddress, newValue, MAX_FIELD_LEN - 1);
            break;
        default:
            if (fieldChoice >= 8 && fieldChoice < 8 + MAX_PROFILES) {
                int profileIndex = fieldChoice - 8;
                strncpy(contact->profileAddresses[profileIndex], newValue, MAX_FIELD_LEN - 1);
            } else {
                return -1; 
            }
            break;
    }
    return 0;
}

int DeleteContact(Record contacts[], int *count, int index) {
    if (index >= 0 && index < *count) {
        for (int i = index; i < *count - 1; i++) {
            contacts[i] = contacts[i + 1];
        }
        (*count)--;
        return 0;
    } 
    return -1;
}