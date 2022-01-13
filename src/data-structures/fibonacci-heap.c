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
};

fib_heap* fib_heap_create() {
  fib_heap* h = malloc(sizeof(*h));
  h->size = 0;
  h->min = NULL;
  return h;
}

node* _node_create(int key) {
  node* n = malloc(sizeof(*n));
  n->key = key;
  n->degree = 0;
  n->left_sibling = NULL;
  n->right_sibling = NULL;
  n->parent = NULL;
  n->child = NULL;
  n->mark = false;
  n->visited = false;
  return n;
}

int fib_heap_is_empty(fib_heap* h) {
  return h->min ? 0 : 1;
}

void _heap_print(node* n) {
  node* child = n->child;
  while(child) {
    printf("node %i child of node %i with degree %i\n", child->key, n->key, n->degree);
    _heap_print(child);
    if (child->right_sibling != n->child) {
      child = child->right_sibling;
    } else {
      child = NULL;
    }
  }
}

void fib_heap_print(fib_heap* h) {
  if (fib_heap_is_empty(h)) {
    printf("fibonacci heap is empty\n");
    return;
  }
  node* curr = h->min;
  while(curr) {
    printf("node %i root of heap\n", curr->key);
    _heap_print(curr);
    if (curr->right_sibling != h->min) {
      curr = curr->right_sibling;
    } else {
      curr = NULL;
    }
  }
}

void _node_extract(node* n) {
  if (n->left_sibling != n) {
    if (n->parent) {
      n->parent->child = n->left_sibling;
      n->parent->degree--;
    }
    n->left_sibling->right_sibling = n->right_sibling;
    n->right_sibling->left_sibling = n->left_sibling;
    n->right_sibling = NULL;
    n->left_sibling = NULL;
  } else {
    if (n->parent) {
      n->parent->child = NULL;
      n->parent->degree--;
    }
  }
}

void _node_insert(node* p, node* n) {
  if (p->child) {
    n->right_sibling = p->child->right_sibling;
    p->child->right_sibling = n;
    n->left_sibling = p->child;
    n->parent = p;
  } else {
    p->child = n;
    n->right_sibling = n;
    n->left_sibling = n;
    n->parent = p;
  }
  p->degree++;
}

void _node_union(node* n1, node* n2) {
  if (n1->key < n2->key) {
    _node_extract(n2);
    _node_insert(n1, n2);
  } else {
    _node_extract(n1);
    _node_insert(n2, n1);
  }
}

void _fib_heap_update_min(fib_heap* h) {
  if (fib_heap_is_empty(h)) {
    return;
  }
  node* initn = h->min;
  node* currm = h->min;
  node* curr = h->min;
  while(curr->right_sibling != initn) {
    curr = curr->right_sibling;
    if (curr->key < currm->key) {
      h->min = curr;
      currm = curr;
    }
  }
}

void _fib_heap_node_extract(fib_heap* h, node* n) {
  if (n->left_sibling != n) {
    if (n->parent) {
      n->parent->child = n->left_sibling;
      n->parent->degree--;
    }
    n->left_sibling->right_sibling = n->right_sibling;
    n->right_sibling->left_sibling = n->left_sibling;
    if (h->min == n) {
      h->min = n->right_sibling;
      _fib_heap_update_min(h);
    }
    n->right_sibling = NULL;
    n->left_sibling = NULL;
    if (n->parent) {
      n->parent = NULL;
    } else {
      h->size--;
    }
  } else {
    if (n->parent) {
      n->parent->child = NULL;
      n->parent->degree--;
    } else {
      h->min = NULL;
      h->size--;
    }
  }
}

void _fib_heap_node_insert(fib_heap* h, node* n) {
  if (fib_heap_is_empty(h)) {
    n->right_sibling = n;
    n->left_sibling = n;
    h->min = n;
    h->size++;
    return;
  }
  n->right_sibling = h->min->right_sibling;
  h->min->right_sibling->left_sibling = n;
  n->left_sibling = h->min;
  h->min->right_sibling = n;
  if (n->key < h->min->key) {
    h->min = n;
  }
  h->size++;
}

void _fib_heap_consolidate(fib_heap* h) {
  if (fib_heap_is_empty(h)) {
    return;
  }
  int max_degree = (int) floor(log2((double) h->size));
  printf("Max degree: %i\n", max_degree);
  node* degrees[max_degree + 1];
  for (int i = 0; i < max_degree + 1; i++) {
    degrees[i] = NULL;
  }
  node* curr = h->min;
  while (curr->right_sibling != h->min) {
    int d = curr->degree;
    printf("Current degree: %i\n", d);
    while (d <= max_degree && degrees[d] != NULL) {
      printf("Degree match of %i and %i\n", degrees[d]->key, curr->key);
      _node_union(degrees[d], curr);
      fib_heap_print(h);
      if (degrees[d]->key < curr->key) {
        curr = degrees[d];
      }
      degrees[d] = NULL;
      d++;
    }
    degrees[curr->degree] = curr;
    printf("curr %i and curr->right_sibling %i\n", curr->key, curr->right_sibling->key);
    curr = curr->right_sibling;
  }
}

void fib_heap_insertion(fib_heap* h, int key) {
  node* n = _node_create(key);
  // check if heap is empty
  if (fib_heap_is_empty(h)) {
    n->right_sibling = n;
    n->left_sibling = n;
    h->min = n;
    h->size++;
    return;
  }
  // insert new node
  n->right_sibling = h->min->right_sibling;
  n->left_sibling = h->min;
  h->min->right_sibling = n;
  if (h->min->left_sibling == h->min) {
    h->min->left_sibling = n;
  }
  // update min key
  if (key < h->min->key) {
    h->min = n;
  }
  h->size++;
}

fib_heap* fib_heap_union(fib_heap* h1, fib_heap* h2) {
  fib_heap* h = fib_heap_create();
  if (fib_heap_is_empty(h1)) {
    h->min = h2->min;
    h->size = h2->size;
  } else if (fib_heap_is_empty(h2)) {
    h->min = h1->min;
    h->size = h1->size;
  } else {
    h->min = h1->min->key < h2->min->key ? h1->min : h2->min;
    h2->min->left_sibling->right_sibling = h1->min->right_sibling;
    h1->min->right_sibling->left_sibling = h2->min->left_sibling;
    h2->min->left_sibling = h1->min;
    h1->min->right_sibling = h2->min;
    h->size = h1->size + h2->size;
  }
  free(h1);
  free(h2);
  return h;
}

int fib_heap_extract_min(fib_heap* h) {
  if (fib_heap_is_empty(h)) {
    return -1;
  }
  node* min = h->min;
  if (h->min->right_sibling != min) {
    h->min->left_sibling->right_sibling = h->min->right_sibling;
    h->min->right_sibling->left_sibling = h->min->left_sibling;
    h->min = h->min->right_sibling;
    fib_heap_print(h);
  } else {
    h->min = NULL;
  }
  h->size--;

  if (min->child) {
    node* curr = min->child;
    node* next = curr->right_sibling;
    while (next != curr) {
      curr->right_sibling->left_sibling = curr->left_sibling;
      curr->left_sibling->right_sibling = curr->right_sibling;
      curr->left_sibling = NULL;
      curr->right_sibling = NULL;
      curr->parent = NULL;
      _fib_heap_node_insert(h, curr);
      curr = next;
      next = curr->right_sibling;
    }
    curr->right_sibling = NULL;
    curr->left_sibling = NULL;
    curr->parent = NULL;
    _fib_heap_node_insert(h, curr);
    fib_heap_print(h);
  }
  _fib_heap_update_min(h);
  _fib_heap_consolidate(h);
  return min->key;
}

int fib_heap_size(fib_heap* h) {
  return h->size;
}
