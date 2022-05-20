#ifndef QUEUE_H
#define QUEUE_H

/* The struct Queue:
   element* head: a pointer to the head of the queue
   element* tail: a pointer to the tail of the queue */
typedef struct Queue queue;

queue* queue_create();

int queue_isEmpty(queue* q);

void queue_enqueue(queue* q, int e);

int queue_dequeue(queue* q);

int queue_peek(queue* q);

void queue_print(queue* q);

void queue_free(queue* q);

#endif
