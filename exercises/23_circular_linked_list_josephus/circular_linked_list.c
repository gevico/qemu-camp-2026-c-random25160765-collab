#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) {
        return NULL;
    }
    
    Node* head = NULL;
    Node* tail = NULL;
    
    for (int i = 1; i <= n; i++) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        if (new_node == NULL) {
            return NULL;
        }
        new_node->id = i;
        new_node->next = NULL;
        
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    tail->next = head;
    return head;
}

void free_list(Node* head) {
    if (head == NULL) {
        return;
    }
    
    Node* current = head;
    Node* next;
    
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != head);
}
