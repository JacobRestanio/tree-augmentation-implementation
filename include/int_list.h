
#ifndef INT_LIST
#define INT_LIST


typedef struct int_ls {
    int value; 

    struct int_ls* prev;
    struct int_ls* next;
} int_ls;


void int_ls_free();

int_ls* int_ls_create();

void last(int_ls* is);

int_ls* add_ls(int_ls* is, int i);

int_ls* merge(int_ls* is, int_ls* js);

void print_ls(int_ls* is);

//void merge_set(int_ls* is, int_ls* js);

//void merge_set_fn(int_ls* is, int_ls* js);

#endif