#include "phoneBook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int CompareContacts(const Record* a, const Record* b) {
    int result;
    result = strcmp(a->lastName, b->lastName);
    if (result != 0) return result;
    result = strcmp(a->firstName, b->firstName);
    if (result != 0) return result;
    result = strcmp(a->patronymic, b->patronymic);
    if (result != 0) return result;
    result = strcmp(a->phoneNumber, b->phoneNumber);
    if (result != 0) return result;
    result = strcmp(a->mailAddress, b->mailAddress);
    if (result != 0) return result;
    result = strcmp(a->jobPlace, b->jobPlace);
    if (result != 0) return result;
    result = strcmp(a->post, b->post);
    if (result != 0) return result;
    for (int i = 0; i < MAX_PROFILES; i++) {
        result = strcmp(a->profileAddresses[i],
                        b->profileAddresses[i]);
        if (result != 0) return result;
    }
    return 0;
}

static void InsertNode(Node** head, Node* newNode) {
    if (*head == NULL) {
        newNode->next = newNode;
        newNode->prev = newNode;
        *head = newNode;
        return;
    }

    Node* tmp = *head;

    do {
        if (CompareContacts(&newNode->data, &tmp->data) < 0) {
            newNode->next = tmp;
            newNode->prev = tmp->prev;

            tmp->prev->next = newNode;
            tmp->prev = newNode;

            if (tmp == *head) {
                *head = newNode;
            }
            return;
        }
        tmp = tmp->next;
    } while (tmp != *head);

    newNode->next = *head;
    newNode->prev = (*head)->prev;

    (*head)->prev->next = newNode;
    (*head)->prev = newNode;
}

static void RemoveNode(Node** head, Node* node) {

    if (node->next == node) {
        *head = NULL;
        return;
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;

    if (*head == node) {
        *head = node->next;
    }
}

int AddContact(Node** head, int* count, Record newRecord) {

    if (*count >= MAX_CONTACTS) {
        return -1;
    }
    Node* newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        return -1;
    }
    newNode->data = newRecord;
    InsertNode(head, newNode);
    (*count)++;
    return 0;
}

Node* FindContactByLastName(Node* head, const char* lastName) {
    if (head == NULL) {
        return NULL;
    }
    Node* tmp = head;

    do {
        if (strcmp(tmp->data.lastName, lastName) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    } while (tmp != head);
    return NULL;
}

int EditContactField(Node** head, Node* contact, int fieldChoice, const char* newValue) {
    if (contact == NULL) {
        return -1;
    }
    RemoveNode(head, contact);

    switch (fieldChoice) {
        case 1:
            strncpy(contact->data.firstName, newValue,
                    MAX_FIELD_LEN - 1);
            contact->data.firstName[MAX_FIELD_LEN - 1] = '\0';
            break;
        case 2:
            strncpy(contact->data.lastName, newValue,
                    MAX_FIELD_LEN - 1);
            contact->data.lastName[MAX_FIELD_LEN - 1] = '\0';
            break;
        case 3:
            strncpy(contact->data.patronymic, newValue,
                    MAX_FIELD_LEN - 1);
            contact->data.patronymic[MAX_FIELD_LEN - 1] = '\0';
            break;
        case 4:
            strncpy(contact->data.jobPlace, newValue,
                    MAX_FIELD_LEN - 1);
            contact->data.jobPlace[MAX_FIELD_LEN - 1] = '\0';
            break;
        case 5:
            strncpy(contact->data.post, newValue,
                    MAX_FIELD_LEN - 1);
            contact->data.post[MAX_FIELD_LEN - 1] = '\0';
            break;
        case 6:
            strncpy(contact->data.phoneNumber, newValue,
                    PHONE_NUMBER_LEN - 1);
            contact->data.phoneNumber[PHONE_NUMBER_LEN - 1] = '\0';
            break;
        case 7:
            strncpy(contact->data.mailAddress, newValue,
                    MAX_FIELD_LEN - 1);
            contact->data.mailAddress[MAX_FIELD_LEN - 1] = '\0';
            break;
        default:
            if (fieldChoice >= 8 && fieldChoice < 8 + MAX_PROFILES) {
                int profileIndex = fieldChoice - 8;
                strncpy(contact->data.profileAddresses[profileIndex], newValue, MAX_FIELD_LEN - 1);
                contact->data.profileAddresses[profileIndex]
                    [MAX_FIELD_LEN - 1] = '\0';
            } else {
                InsertNode(head, contact);
                return -1;
            }
            break;
    }
    InsertNode(head, contact);

    return 0;
}

int DeleteContact(Node** head, int* count, Node* contact) {
    if (*head == NULL || contact == NULL) {
        return -1;
    }
    RemoveNode(head, contact);
    free(contact);
    (*count)--;
    return 0;
}

void PrintList(Node* head) {
    if (head == NULL) {
        printf("Список пуст\n");
        return;
    }
    Node* tmp = head;
    do {
        printf("%s %s %s | Телефон: %s\n",
               tmp->data.lastName,
               tmp->data.firstName,
               tmp->data.patronymic,
               tmp->data.phoneNumber);
        tmp = tmp->next;
    } while (tmp != head);
}