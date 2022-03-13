#include "../../include/ptr-list.h"
#include <stdio.h>
#include <stdlib.h>





void ps_free(ptr_ls* ps){
    while(ps){
        ptr_ls* next = ps->next;
        free(ps);
        ps = next;
    }
}


void ps_free_recursive(ptr_ls* ps, void (*free_fn)(void*)){
    while(ps){
        ptr_ls* next = ps->next;
        if(ps->ptr)
            free_fn(ps->ptr);
        free(ps);
        ps = next;
    }
}

ptr_ls* ps_create(){
    ptr_ls* new_ls = malloc(sizeof(ptr_ls));

    new_ls->ptr = NULL; 

    new_ls->prev = NULL;
    new_ls->next = NULL;
    return new_ls;
}

ptr_ls* ps_last(ptr_ls* ps){
    if(!ps)
        return ps;
    while(ps->next)
        ps = ps->next;
    return ps;
}


ptr_ls* ps_add(ptr_ls* ps, void* i){
    ptr_ls* new_ptr = ps_create();
    new_ptr->ptr = i;
    new_ptr->next = ps;
    if(ps){
        ps->prev = new_ptr;
    }
    return new_ptr;
}

void ps_remove(ptr_ls* ps, void* i){

}

ptr_ls* ps_merge(ptr_ls* ps, ptr_ls* js){

    if(!ps)
        return js;
    if(!js)
        return ps;
    
    ptr_ls* last = ps_last(ps);

    last->next = js;
    js->prev = last;

    return ps;
}

void ps_print(ptr_ls* ps){
    printf("{ ");
    while(ps){
        printf("%i ", ps->ptr);
        ps = ps->next;
    }
    printf("}");
}

//void merge_set(int_ls* ps, int_ls* js);

//void merge_set_fn(int_ls* ps, int_ls* js);
