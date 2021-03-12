#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;               // podatak (u ovom primeru pravimo listu int-ova)
    struct node* next;      // pokazivac na sledeci cvor liste
} LLNode;

void initLinkedList(LLNode**);
void addElement(LLNode**, int);
void printLinkedList(LLNode*);
void deleteElement(LLNode**, int);
void deleteLinkedList(LLNode**);

int main() {
    LLNode* head;
    initLinkedList(&head);

    for(int i = 1; i <= 10; i++) {
        addElement(&head, i);
    }

    printLinkedList(head);

    deleteElement(&head, 3);

    printLinkedList(head);

    deleteElement(&head, 1);
    
    printLinkedList(head);

    deleteLinkedList(&head);

    return 0;
}

void initLinkedList(LLNode** head) {
    *head = NULL;
}

void addElement(LLNode** head, int data) {
    LLNode *new, *curr, *prev;
    new = (LLNode*)malloc(sizeof(LLNode));

    new->data = data;
    new->next = NULL;

    // provera da li je lista prazna
    if(*head == NULL) {
        *head = new;
    }else {
        curr = *head;
        prev = *head;

        // pozicioniranje na kraj liste (pokazivac prev ce pokazivati na poslednji element u listi)
        while(curr != NULL) {
            prev = curr;
            curr = curr->next;
        }

        prev->next = new;
    }
}

void printLinkedList(LLNode* head) {
    LLNode* curr;

    if(head == NULL) {
        printf("List is empty...\n");
    }else {
        curr = head;

        while(curr != NULL) {
            printf("%d ", curr->data);
            curr = curr->next;
        }

        printf("\n");
    }
}

void deleteElement(LLNode** head, int data) {
    LLNode *prev, *curr;

    prev = *head;
    curr = *head;

    // trazenje cvora koji treba obrisati
    // dok god ne dodjes do kraja liste I dok god je trenutni element razlicit od prosledjene vrednosti (one vrednosti koju hocemo da brisemo)
    while(curr != NULL && (curr->data != data)) {
        prev = curr;
        curr = curr->next;
    }

    // provera da li se brise prvi element liste
    // jeste prvi element liste -> prevezivanje head pokazivaca, tj. pocetka liste
    if(prev == curr) {
        *head = curr->next;
    // nije prvi element liste -> prevezivanje pokazivaca
    }else {
        prev->next = curr->next;
    }

    curr->next = NULL;
    free(curr);
}

void deleteLinkedList(LLNode** head) {
    LLNode* curr;

    while(*head != NULL) {
        curr = *head;
        *head = curr->next;
        free(curr);
    }
}