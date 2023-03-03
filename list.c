#include "list.h"
#include <stdlib.h>
#include <string.h>

void init_list(struct List *list){
    list->head = NULL;
    list->tail = NULL;
}

struct Node* create_node(char* key, char* value){
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

void append(struct List *list, struct Node *node){
    if(node == NULL) return;
    if(list->head == NULL){
        list->head = node;
        list->tail = node;
    }else{
        list->tail->next = node;
        list->tail = node;
    }
}

struct Node* get_node(struct List *header_list, char *key){
    struct Node *current = header_list->head;
    while(current != NULL){
        if(strcmp(current->key, key) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int clear_list(struct List *list){
    struct Node *current = list->head;
    struct Node *next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    return 0;
}

