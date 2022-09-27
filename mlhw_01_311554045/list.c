#include<stdio.h>
#include<stdlib.h>
#include"list.h"

List* init_list(){
    List* ptr=malloc(sizeof(List));
    ptr->length=0;
    ptr->capacity=2;
    ptr->list=malloc(2*sizeof(List));
    return ptr;
}

void free_list(List* ptr){
    free(ptr->list);
    free(ptr);
    return ;
}

void print_list(List* ptr){
    printf("[");
    for(int i=0; i<ptr->length; i++){
        printf("%.2f, ", ptr->list[i]);
    }
    printf("]\n");
}

void leng_list(List *ptr){
    float* new_list=malloc(2*ptr->capacity*sizeof(float));
    for(int i=0; i<ptr->length; i++){
        new_list[i]=ptr->list[i];
    }
    ptr->capacity*=2;
    free(ptr->list);
    ptr->list=new_list;
    return;
}

void list_push(List* ptr, float item){
    if(ptr->length==ptr->capacity){
        leng_list(ptr);
    }
    ptr->list[ptr->length]=item;
    ptr->length+=1;
    return ;
}