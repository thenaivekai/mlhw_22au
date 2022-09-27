#ifndef LIST_H
#define LIST_H

typedef struct list{
    int length;
    int capacity;
    float* list;
}List;

List* init_list();
void free_list(List* ptr);
void print_list(List* ptr);

//void leng_list(List *ptr);/* lengthen the list */
void list_push(List* ptr, float item);

#endif