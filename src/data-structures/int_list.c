#include "../../include/int_list.h"
#include <stdio.h>
#include <stdlib.h>


#ifndef NULL
#define NULL ((void*)0);
#endif




void int_ls_free(int_ls* is){
    while(is){
        int_ls* next = is->next;
        free(is);
        is = next;
    }
}

int_ls* int_ls_create(){
    int_ls* ghost = malloc(sizeof(int_ls));

    ghost->value = 0; 

    ghost->prev = NULL;
    ghost->next = NULL;
    return ghost;
}

void last(int_ls* is);

int_ls* add(int_ls* is, int i){
    int_ls* n = int_ls_create();
    n->value = i;
    n->next = is;
    if(is){
        is->prev = n;
    }
    return n;
}

void remove_int(int_ls* is, int i){

}

int_ls* merge(int_ls* is, int_ls* js){

    if(!is)
        return js;
    if(!js)
        return is;
    
    int_ls* is_last = is;
    while(is_last->next)
        is_last = is_last->next;
    is_last ->next = js;
    
    js->prev = is_last;
    return is;
}

void print_ls(int_ls* is){
    printf("{ ");
    while(is){
        printf("%i ", is->value);
    }
    printf("}");
}

//void merge_set(int_ls* is, int_ls* js);

//void merge_set_fn(int_ls* is, int_ls* js);
