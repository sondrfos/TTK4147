#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{
    list_t list = malloc(sizeof(struct list));
    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_delete(list_t list)
{
    struct node* iter = list->head;
    while(iter != NULL){
        struct node* temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(list);
}


void list_insert(list_t list, int index, int data)
{
    struct node* insert = malloc(sizeof(struct node));
    insert->data = data;
    if(list->length == 0){
        list->head = insert;
        list->tail = insert;
        insert->prev = NULL;
        insert->next = NULL;
    }
    else if(index >=list->length){
        insert->next = NULL;
        insert->prev = list->tail;
        list->tail->next = insert;
        list->tail = insert;
    }
    else{
        struct node* iter = list->head;
        for(int i = 0; i<index; i++){
            iter = iter->next;
        }
        iter->prev->next = insert;
        insert->prev = iter->prev;
        insert->next = iter;
        iter->prev = insert;
    }
    list->length++;
}

void list_append(list_t list, int data){
    list_insert(list, list->length, data);
}

void list_print(list_t list)
{
    struct node* iter = list->head;
    while(iter != NULL){
        printf("%i ",iter->data);
        iter = iter->next;
    }
    printf("\n");
}

long list_sum(list_t list)
{
    struct node* iter = list->head;
    long sum = 0;
    while(iter != NULL){
        sum += iter->data;
        iter = iter->next;
    }
    return sum;
}

int list_get(list_t list, int index)
{
    struct node* iter = list->head;
    for(int i = 0; i<index; i++){
        iter = iter->next;
    }
    return iter->data;

}

int list_extract(list_t list, int index)
{
    struct node* iter = list->head;
    for(int i = 0; i<index; i++){
        iter = iter->next;
    }

    if(index == 0){
        list->head = iter->next;
        if(iter->next){
            iter->next->prev = NULL;
        }
    }
    else if(index == list->length){
        list->tail = iter->prev;
        iter->prev->next = NULL;
    }
    else{
        iter->prev->next = iter->next;
        iter->next->prev = iter->prev;
    }
    int element = iter->data;
    free(iter);
    list->length--;
    return element;
}
