#include <stdlib.h>
#include <stdio.h>
#include "../../include/int-list.h"

typedef struct Int_List {
   int_list* next;
   int_list* prev;
   int value;
} int_list;

int_list* int_list_create(int v) {
   int_list* int_ls = malloc(sizeof(*int_ls));
   int_ls->next = NULL;
   int_ls->prev = NULL;
   int_ls->value = v;
   return int_ls;
}

int int_list_match(void* int_lst1, void* int_lst2) {
   if (!(int_lst1)) {
      return 0;
   }
   if (!(int_lst2)) {
      return 0;
   }
   int_list* int_ls1 = int_lst1;
   int_list* int_ls2 = int_lst2;

   return (int_ls1->value == int_ls2->value) ? 1 : 0;
}

void int_list_print(void* int_lst) {
   int_list* int_ls = int_lst;
   printf("%i", int_ls->value);
}

int int_list_value(void* int_lst) {
   int_list* int_ls = int_lst;
   return int_ls->value;
}

void int_list_free(void* int_lst) {
   int_list* int_ls = int_lst;
   free(int_ls);
}
