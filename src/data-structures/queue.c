#include <stdlib.h>
#include <stdio.h>
#include "../../include/list.h"

typedef struct Element {
   struct Element* next;
   struct Element* prev;
   void* object;
} element;

typedef struct Queue {
   element* front;
   element* rear;
} queue;

queue* queue_create() {
   queue* que = malloc(sizeof(*que));
   que->front = NULL;
   que->rear = NULL;
   return que;
}

int queue_isEmpty(queue* q) {
   if (q->front) {
      return 0;
   } else {
      return 1;
   }
}

void queue_enqueue(queue* q, void* object) {
   element* elm = malloc(sizeof(struct Element));
   elm->next = NULL;
   elm->prev = NULL;
   elm->object = object;

   if (q->rear) {
      elm->prev = q->rear;
      q->rear->next = elm;
   }
   q->rear = elm;
   if (!q->front) {
      q->front = elm;
   }
}

void* queue_dequeue(queue* q) {
   if (queue_isEmpty(q)) {
      return NULL;
   }
   element* elm = q->front;
   if (q->front == q->rear) {
      q->front = NULL;
      q->rear = NULL;
   } else {
      q->front = q->front->next;
   }
   void* object = elm->object;
   free(elm);
   return object;
}

void* queue_peek(queue *q) {
   if (queue_isEmpty(q)) {
      return NULL;
   }
   element* elm = q->front;
   void* object = elm->object;
   return object;
}

void queue_print(queue* q, void (*print_fn)(void*)) {
   element* elm = q->front;
   while (elm) {
      print_fn(elm->object);
      printf(" <- ");
      elm = elm->next;
   }
   printf("\n");
}

void queue_free(queue* q) {
   while (queue_dequeue(q)) {}
   free(q);
}
