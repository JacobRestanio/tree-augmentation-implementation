#include <stdlib.h>
#include <stdio.h>

typedef struct Element {
   int value;
   struct Element* nextElement;
   struct Element* prevElement;
} element;

typedef struct Queue {
   element* front;
   element* rear;
} queue;

element* _element_create(int e) {
   element* elem = malloc(sizeof(*elem));
   elem->value = e;
   elem->nextElement = NULL;
   elem->prevElement = NULL;
   return elem;
}

void _element_free(element* e) {
   free(e);
}

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

void queue_enqueue(queue* q, int e) {
   element* elem = _element_create(e);
   if (q->rear) {
      elem->prevElement = q->rear;
      q->rear->nextElement = elem;
   }
   q->rear = elem;
   if (!q->front) {
      q->front = elem;
   }
}

int queue_dequeue(queue* q) {
   if (queue_isEmpty(q)) {
      return -1;
   }
   element* elem = q->front;
   if (q->front == q->rear) {
      q->front = NULL;
      q->rear = NULL;
   } else {
      q->front = q->front->nextElement;
   }
   int e = elem->value;
   _element_free(elem);
   return e;
}

int queue_peek(queue *q) {
   if (queue_isEmpty(q)) {
      return -1;
   }
   int elem = q->front->value;
   return elem;
}

void queue_print(queue* q) {
   element* currElem = q->front;
   while (currElem) {
      printf("%i <- ", currElem->value);
      currElem = currElem->nextElement;
   }
   printf("\n");
}

void queue_free(queue* q) {
   while(q->front) {
      queue_dequeue(q);
   }
   free(q);
}
