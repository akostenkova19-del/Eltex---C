
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

static int Height(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

static int Max(int a, int b) {
    return (a > b) ? a : b;
}

static void UpdateHeight(Node* node) {
    if (node != NULL) {
        node->height = 1 + Max(Height(node->left), Height(node->right));
    }
}

static int BalanceFactor(Node* node) {
    if (node == NULL) return 0;
    return Height(node->left) - Height(node->right);
}

static Node* RotateRight(Node* y) {
    Node* x = y->left;
    Node* t = x->right;

    x->right = y;
    y->left = t;

    UpdateHeight(y);
    UpdateHeight(x);

    return x;
}

static Node* RotateLeft(Node* x) {
    Node* y = x->right;
    Node* t = y->left;

    y->left = x;
    x->right = t;

    UpdateHeight(x);
    UpdateHeight(y);

    return y;
}

static Node* Balance(Node* node) {
    if (node == NULL) return NULL;

    UpdateHeight(node);

    int bf = BalanceFactor(node);

    if (bf > 1) {
        if (BalanceFactor(node->left) < 0) {
            node->left = RotateLeft(node->left);
        }
        return RotateRight(node);
    }

    if (bf < -1) {
        if (BalanceFactor(node->right) > 0) {
            node->right = RotateRight(node->right);
        }
        return RotateLeft(node);
    }

    return node;
}

static Node* InsertNode(Node* root, Record record, int* result) {
    if (root == NULL) {
        Node* newNode = malloc(sizeof(Node));

        if (newNode == NULL) {
            *result = -1;
            return NULL;
        }

        newNode->data = record;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;

        *result = 0;
        return newNode;
    }

    int comparison = CompareContacts(&record, &root->data);

    if (comparison < 0) {
        root->left = InsertNode(root->left, record, result);
    }
    else if (comparison > 0) {
        root->right = InsertNode(root->right, record, result);
    }
    else {
        *result = -1;
        return root;
    }

    if (*result != 0) return root;

    return Balance(root);
}

int AddContact(Node** root, int* count, Record newRecord) {
    int result = 0;

    *root = InsertNode(*root, newRecord, &result);

    if (result == 0) {
        (*count)++;
    }

    return result;
}

Node* FindContactByLastName(Node* root, const char* lastName) {
    if (root == NULL) return NULL;

    if (strcmp(root->data.lastName, lastName) == 0) {
        return root;
    }

    Node* found = FindContactByLastName(root->left, lastName);

    if (found != NULL) return found;

    return FindContactByLastName(root->right, lastName);
}

static Node* ExtractMax(Node* root, Record* maxRecord) {
    if (root->right == NULL) {
        *maxRecord = root->data;
        Node* left = root->left;
        free(root);

        return left;
    }

    root->right = ExtractMax(root->right, maxRecord);
    return Balance(root);
}

static Node* RemoveNode(Node* root, const Record* record, int* deleted) {
    if (root == NULL) return NULL;

    int comparison = CompareContacts(record, &root->data);

    if (comparison < 0) {
        root->left = RemoveNode(root->left, record, deleted);
    }
    else if (comparison > 0) {
        root->right = RemoveNode(root->right, record, deleted);
    }
    else {
        *deleted = 1;

        Node* left = root->left;
        Node* right = root->right;

        if (left == NULL) {
            free(root);
            return right;
        }

        if (right == NULL) {
            free(root);
            return left;
        }

        Record maxRecord;
        root->left = ExtractMax(left, &maxRecord);
        root->data = maxRecord;
    }
    return Balance(root);
}

int DeleteContact(Node** root, int* count, Node* contact) {
    if (*root == NULL || count == NULL || contact == NULL) {
        return -1;
    }

    Record record = contact->data;

    int deleted = 0;

    *root = RemoveNode(*root, &record, &deleted);

    if (deleted) {
        (*count)--;
        return 0;
    }

    return -1;
}

int EditContactField(Node** root, Node* contact, int fieldChoice, const char* newValue) {
    if (*root == NULL || contact == NULL || newValue == NULL) {
        return -1;
    }

    Record oldRecord = contact->data;
    Record newRecord = oldRecord;

    switch (fieldChoice) {
        case 1:
            if (strlen(newValue) >= MAX_FIELD_LEN) return -1;
            strcpy(newRecord.firstName, newValue);
            break;

        case 2:
            if (strlen(newValue) >= MAX_FIELD_LEN) return -1;
            strcpy(newRecord.lastName, newValue);
            break;

        case 3:
            if (strlen(newValue) >= MAX_FIELD_LEN) return -1;
            strcpy(newRecord.patronymic, newValue);
            break;

        case 4:
            if (strlen(newValue) >= MAX_FIELD_LEN) return -1;
            strcpy(newRecord.jobPlace, newValue);
            break;

        case 5:
            if (strlen(newValue) >= MAX_FIELD_LEN) return -1;
            strcpy(newRecord.post, newValue);
            break;

        case 6:
            if (strlen(newValue) >= PHONE_NUMBER_LEN) return -1;
            strcpy(newRecord.phoneNumber, newValue);
            break;

        case 7:
            if (strlen(newValue) >= MAX_FIELD_LEN) return -1;
            strcpy(newRecord.mailAddress, newValue);
            break;

        default:
            if (fieldChoice >= 8 &&
                fieldChoice < 8 + MAX_PROFILES) {

                int profileIndex = fieldChoice - 8;

                if (strlen(newValue) >= MAX_FIELD_LEN) return -1;

                strcpy(newRecord.profileAddresses[profileIndex],
                       newValue);
            }
            else {
                return -1;
            }
    }

    int deleted = 0;
    *root = RemoveNode(*root, &oldRecord, &deleted);

    if (deleted == 0) return -1;

    int result = 0;
    *root = InsertNode(*root, newRecord, &result);

    if (result == 0) {
        return 0;
    }

    result = 0;
    *root = InsertNode(*root, oldRecord, &result);

    return -1;
}

void PrintList(Node* root) {
    if (root == NULL) {
        return;
    }

    PrintList(root->left);

    printf("Фамилия: [%s]\n", root->data.lastName);
    printf("Имя: [%s]\n", root->data.firstName);
    printf("Телефон: [%s]\n", root->data.phoneNumber);
    printf("----------------------\n");

    PrintList(root->right);
}

static void PrintTreeRecursive(Node* root, int level, char branch) {
    if (root == NULL) {
        return;
    }

    PrintTreeRecursive(root->right, level + 1, '/');

    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    if (branch != ' ') {
        printf("%c-- ", branch);
    }

    printf("%s %s\n",root->data.lastName, root->data.firstName);
    PrintTreeRecursive(root->left, level + 1, '\\');
}

void PrintTree(Node* root) {
    if (root == NULL) {
        printf("Дерево пусто.\n");
        return;
    }

    printf("\nДерево контактов:\n\n");

    PrintTreeRecursive(root, 0, ' ');
}


void DeleteTree(Node** root) {
    if (root == NULL || *root == NULL) return;

    DeleteTree(&(*root)->left);
    DeleteTree(&(*root)->right);

    free(*root);
    *root = NULL;
}
