#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

typedef struct FibonacciHeap fib_heap;

fib_heap* fib_heap_create();

int fib_heap_is_empty(fib_heap* h);

void fib_heap_print(fib_heap* h);

void fib_heap_insertion(fib_heap* h, int key);

fib_heap* fib_heap_union(fib_heap* h1, fib_heap* h2);

int fib_heap_extract_min(fib_heap* h);

int fib_heap_size(fib_heap* h);

#endif
