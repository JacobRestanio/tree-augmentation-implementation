#ifndef QUEUE_H
#define QUEUE_H

/* The struct Queue:
   element* head: a pointer to the head of the queue
   element* tail: a pointer to the tail of the queue */
typedef struct Queue queue;

queue* queue_create();

int queue_isEmpty(queue* q);

void queue_enqueue(queue* q, void* object);

void* queue_dequeue(queue* q);

void* queue_peek(queue* q);

void queue_print(queue* q, void (*print_fn)(void*));

void queue_free(queue* q);

#endif
