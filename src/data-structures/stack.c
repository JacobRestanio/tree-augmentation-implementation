#include <stdio.h>
#include <stdlib.h>
#include "../../include/stack.h"

struct Stack {
  int* stack;
  int top;
  int size;
};

stack* stack_create(int size) {
  stack* s = malloc(sizeof(*s));
  s->stack = malloc(sizeof(int[size]));
  s->top = 0;
  s->size = size;
  return s;
}

int stack_is_full(stack* s) {
  if (s->top == s->size) {
    return 1;
  } else {
    return 0;
  }
}

int stack_is_empty(stack* s) {
  if (s->top == 0) {
    return 1;
  } else {
    return 0;
  }
}

void stack_push(stack* s, int element) {
  if (stack_is_full(s)) {
    printf("Stack is full\n");
  } else {
    s->stack[s->top] = element;
    s->top++;
  }
}

int stack_pop(stack* s) {
  if (stack_is_empty(s)) {
    printf("Stack is empty\n");
    return -1;
  } else {
    s->top--;
    return s->stack[s->top];
  }
}

int stack_size(stack* s) {
  return s->top;
}

int stack_peek(stack* s) {
  if (stack_is_empty(s)) {
    printf("Stack is empty\n");
    return -1;
  }
  return s->stack[s->top - 1];
}

void stack_print(stack* s) {
  printf("Printing Stack:\n");
  for (int i = s->top - 1; i >= 0; i--) {
    printf("%i\n", s->stack[i]);
  }
  printf("\n");
}

void stack_free(stack* s) {
  free(s->stack);
  free(s);
}
