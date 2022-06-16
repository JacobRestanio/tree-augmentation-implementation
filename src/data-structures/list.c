#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/list.h"

typedef struct List {
   list* next;
   list* prev;
} list;

void* list_next(void* lst) {
   list* ls = lst;
   return ls->next;
}

void* list_prev(void* lst) {
   list* ls = lst;
   return ls->prev;
}

void* list_first(void* lst) {
   list* ls = lst;
   if (!(ls)) {
      return NULL;
   }
   while (ls->prev) {
      ls = ls->prev;
   }
   return ls;
}

void* list_last(void* lst) {
   list* ls = lst;
   if (!(ls)) {
      return ls;
   }
   while (ls->next) {
      ls = ls->next;
   }
   return ls;
}

int list_size(void* lst) {
   list* ls = lst;
   ls = list_first(ls);
   int n = 0;
   while (ls) {
      n++;
      ls = ls->next;
   }
   return n;
}

void* list_add(void* lst, void* elem) {
   if (!(lst)) {
      return elem;
   }
   if (!(elem)) {
      return lst;
   }

   list* ls1 = lst;
   list* el = elem;

   if (el->prev || el->next) {
      return lst;
   }

   el->next = ls1;
   el->prev = ls1->prev;
   ls1->prev = el;
   return el;
}

void* list_merge(void* lst1, void* lst2) {
   if (!lst1) {
      return lst2;
   }
   if (!lst2) {
      return lst1;
   }

   list* ls1 = lst1;
   list* ls2 = lst2;

   list *lls1 = list_last(ls1);
   list *fls2 = list_first(ls2);

   lls1->next = fls2;
   fls2->prev = lls1;
   return lst1;
}

void* list_contains(void* lst, int (*match_fn)(void*, void*), void* elem) {
   if (!(lst) || !(elem)) {
      return NULL;
   }
   list* ls = lst;
   while (ls) {
      if (match_fn(ls, elem)) {
         return ls;
      }
      ls = ls->next;
   }
   return NULL;
}

void* list_contains_list(void* lst, int (*match_fn)(void* , void* ), void* elems) {
   if (!(lst) || !(elems)) {
      return NULL;
   }
   list* ls = lst;
   while (ls) {
      list* elms = elems;
      while(elms) {
         if (match_fn(ls, elms)) {
            return ls;
         }
         elms = elms->next;
      }
      ls = ls->next;
   }
   return NULL;
}

void* list_remove(void* lst, void (*free_fn)(void*)) {
   if (!(lst)) {
      return NULL;
   }
   list* ls = lst;
   list* pls = ls->prev;
   list* nls = ls->next;
   if (pls) {
      pls->next = nls;
   }
   if (nls) {
      nls->prev = pls;
   }
   free_fn(ls);
   return (nls) ? nls : pls;
}

void* list_remove_elem(void* lst, int (*match_fn)(void*, void*), void (*free_fn)(void*), void* elem) {
   list* ls = lst;
   while ((ls = list_contains(ls, match_fn, elem))) {
      lst = (lst == ls) ? ls->next : lst;
      ls = list_remove(ls, free_fn);
   }
   return lst;
}

void* list_remove_list(void* lst, int (*match_fn)(void*, void*), void (*free_fn)(void*), void* elems) {
   list* ls = lst;
   list* elms = elems;
   ls = list_first(ls);
   elms = list_first(elms);
   while ((ls = list_contains_list(ls, match_fn, elms))) {
      lst = (lst == ls) ? ls->next : lst;
      ls = list_remove(ls, free_fn);
   }
   return lst;
}

void list_print(void* lst, void (*print_fn)(void*)) {
    list* ls = lst;
    ls = list_first(ls);
    printf("{ ");
    while (ls) {
        print_fn(ls);
        printf(" ");
        ls = ls->next;
    }
    printf("}\n\n");
}

void list_free(void* lst, void (*free_fn)(void*)) {
   list* ls = lst;
   ls = list_first(ls);
   while(ls) {
      list* next = ls->next;
      free_fn(ls);
      ls = next;
   }
}
