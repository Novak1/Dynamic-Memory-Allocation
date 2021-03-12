#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 20

typedef struct student_st {
    char index[MAX_LEN];
    char firstname[MAX_LEN];
    char lastname[MAX_LEN];
    unsigned points1;
    unsigned points2;
    char class[MAX_LEN];
} student;

typedef struct node {
    student data;
    struct node* next;
} LLnode;

void printStudent(student st);
void initLinkedList(LLnode**);
void addElement(LLnode**, student);
void printLinkedList(LLnode*);
void deleteElement(LLnode**, char*);
void deleteLinkedList(LLnode**);

int main() {
    student st1 = { .index = "RA 1/2020", .firstname = "Alan", .lastname = "Ford", .class = "C", .points1 = 19, .points2 = 20 };
    student st2 = { .index = "RA 2/2020", .firstname = "Bob", .lastname = "Rock", .class = "C", .points1 = 19, .points2 = 18 };

    LLnode* head;
    initLinkedList(&head);

    addElement(&head, st1);
    addElement(&head, st2);

    printLinkedList(head);

    student st3 = { .index = "RA 3/2020", .firstname = "Sir", .lastname = "Oliver", .class = "C", .points1 = 19, .points2 = 18 };
    addElement(&head, st3);

    printLinkedList(head);

    deleteElement(&head, "RA 3/2020");

    printLinkedList(head);

    deleteLinkedList(&head);
}

void printStudent(student st) {
    printf("Index: %s\n", st.index);
    printf("Firstname: %s\n", st.firstname);
    printf("Lastname: %s\n", st.lastname);
    printf("Class: %s\n", st.class);
    printf("Test 1 points: %u\n", st.points1);
    printf("Test 2 points: %u\n", st.points2);
    printf("---------------------------------\n");
}

void initLinkedList(LLnode** head) {
    *head = NULL;
}

void addElement(LLnode** head, student data) {
    LLnode *new, *prev, *curr;
    new = (LLnode*)malloc(sizeof(LLnode));

    // kopiranje podataka u new cvor
    strcpy(new->data.index, data.index);
    strcpy(new->data.firstname, data.firstname);
    strcpy(new->data.lastname, data.lastname);
    strcpy(new->data.class, data.class);
    new->data.points1 = data.points1;
    new->data.points2 = data.points2;

    if(*head == NULL) {
        *head = new;
    }else {
        prev = *head;
        curr = *head;

        while(curr != NULL) {
            prev = curr;
            curr = curr->next;
        }

        prev->next = new;
    }
}

void printLinkedList(LLnode* head) {
    LLnode* curr;

    if(head == NULL) {
        printf("List is empty...\n");
    }else {
        curr = head;

        while(curr != NULL) {
            printStudent(curr->data);
            curr = curr->next;
        }
    }
}

void deleteElement(LLnode** head, char* index) {
    LLnode *prev, *curr;

    prev = *head;
    curr = *head;

    while(curr != NULL && (strcmp(curr->data.index, index))) {
        prev = curr;
        curr = curr->next;
    }

    if(prev == curr) {
        *head = curr->next;
    }else {
        prev->next = curr->next;
    }

    curr->next = NULL;
    free(curr);
}

void deleteLinkedList(LLnode** head) {
    LLnode* curr;

    while(*head != NULL) {
        curr = *head;
        *head = curr->next;
        free(curr);
    }
}