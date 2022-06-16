#ifndef LIST_H
#define LIST_H

typedef struct List list;

void* list_create();

void* list_next(void* lst);

void* list_prev(void* lst);

void* list_first(void* lst);

void* list_last(void* lst);

int list_size(void* lst);

void* list_add(void* lst1, void* elem);

void* list_merge(void* lst1, void* lst2);

void* list_contains(void* lst, int (*compare_fn)(void*, void*), void* elem);

void* list_contains_ls(void* lst, int (*compare_fn)(void*, void*), void* elems);

void* list_remove(void* lst, void (*free_fn)(void*));

void* list_remove_elem(void* lst, int (*match_fn)(void*, void*), void (*free)(void*), void* elem);

void* list_remove_list(void* lst, int (*match_fn)(void*, void*), void (*free_fn)(void*), void* elems);

void list_print(void* lst, void (*print_fn)(void*));

void list_free(void* lst, void (*free_fn)(void*));

#endif
