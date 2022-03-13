
#ifndef PTR_LIST
#define PTR_LIST


typedef struct ptr_ls {
    void* ptr; 

    struct ptr_ls* prev;
    struct ptr_ls* next;
} ptr_ls;


void ps_free();

void ps_free_recursive();

ptr_ls* ps_create();

ptr_ls* ps_last(ptr_ls* ps);

ptr_ls* ps_add(ptr_ls* ps, void* i);

ptr_ls* ps_merge(ptr_ls* ps, ptr_ls* js);

void ps_print(ptr_ls* ps);

//void merge_set(int_ls* ps, int_ls* js);

//void merge_set_fn(int_ls* ps, int_ls* js);

#endif