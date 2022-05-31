#ifndef STACK_H
#define STACK_H

typedef struct Stack stack;

stack* stack_create(int size);

int stack_is_full(stack* s);

int stack_is_empty(stack* s);

void stack_push(stack* s, int element);

int stack_pop(stack* s);

int stack_size(stack* s);

int stack_peek(stack* s);

void stack_print(stack* s);

void stack_free(stack* s);

#endif
