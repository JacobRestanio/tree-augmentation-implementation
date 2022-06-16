#ifndef INT_LIST_H
#define INT_LIST_H

typedef struct Int_List int_list;

int_list* int_list_create(int v);

int int_list_match(void* int_lst1, void* in);

void int_list_print(void* int_lst);

int int_list_value(void* int_lst);

void int_list_free(void* int_lst);

#endif
