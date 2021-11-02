#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../../include/fibonacci-heap.h"

typedef struct Node {
  int key;
  int degree;
  struct Node* left_sibling;
  struct Node* right_sibling;
  struct Node* parent;
  struct Node* child;
  bool mark;
  bool visited;
} node;

struct FibonacciHeap {
  int size;
  node* min;
  int phi;
  int degree;
};

fib_heap* fib_heap_create() {
  fib_heap* h = malloc(sizeof(*h));
  h->size = 0;
  h->min = NULL;
  h->phi = 0;
  h->degree = 0;
  return h;
}

void _heap_print(node* n) {
  node* child = n->child;
  while(child) {
    printf("node %i child of node %i", child->key, n->key);
    _heap_print(child);
    if (child->right_sibling != n->child) {
      child = child->right_sibling;
    } else {
      child = NULL;
    }
  }
}

void fib_heap_print(fib_heap* h) {
  node* curr = h->min;
  while(curr) {
    printf("node %i root of heap", h->min->key);
    _heap_print(curr);
    if (curr->right_sibling != h->min) {
      curr = curr->right_sibling;
    } else {
      curr = NULL;
    }
  }
}
