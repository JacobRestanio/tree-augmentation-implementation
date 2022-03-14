
#ifndef INT_LIST
#define INT_LIST


typedef struct int_ls {
    int value; 

    struct int_ls* prev;
    struct int_ls* next;
} int_ls;


void ls_free();

int_ls* ls_create();

int_ls* ls_first(int_ls* is);

int_ls* ls_last(int_ls* is);

int ls_size(int_ls* is);

int_ls* ls_add(int_ls* is, int i);

int_ls* ls_merge(int_ls* is, int_ls* js);

int ls_remove(int_ls* is, int i);

int ls_remove_all(int_ls* is, int i);

void ls_remove_list(int_ls* is, int_ls* ns);

void ls_print(int_ls* is);

int_ls* ls_contains(int_ls* is, int n);

int ls_contains_2(int_ls* list, int a, int b);

int_ls* ls_contains_any(int_ls* list, int_ls* ns);

//void merge_set(int_ls* is, int_ls* js);

//void merge_set_fn(int_ls* is, int_ls* js);

#endif