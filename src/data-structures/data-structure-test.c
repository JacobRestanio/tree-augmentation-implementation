#include <stdio.h>
#include <stdlib.h>
#include "../../include/stack.h"
#include "../../include/graph.h"
#include "../../include/fibonacci-heap.h"

int main() {
  printf("\n\n");
  printf("Creating stack\n");
  stack* s = stack_create(10);

  printf("Pushing 1, 2, 3, 4, 5 onto stack\n");
  for (int i = 1; i <= 5; i++) {
    stack_push(s, i);
  }

  printf("Peeking at stack: %i\n", stack_peek(s));

  printf("Checking stack size: %i\n", stack_size(s));

  printf("Print stack:\n");
  stack_print(s);

  printf("Popping 5 and 4 off the stack\n");
  printf("Popped %i\n", stack_pop(s));
  printf("Popped %i\n", stack_pop(s));

  printf("Peeking at stack: %i\n", stack_peek(s));

  printf("Checking stack size: %i\n", stack_size(s));

  printf("Print stack:\n");
  stack_print(s);

  printf("Freeing stack\n");
  stack_free(s);

  printf("\n\n");
  printf("Creating Directed Graph\n");
  directedgraph* dg = directedgraph_create(5);

  printf("Print empty graph:\n");
  directedgraph_print(dg);

  printf("Adding edges (0,1) (0,2) (0,3) (1,3) (2,4) (3,4)\n");
  directedgraph_add_edge(dg, 0, 1);
  directedgraph_add_edge(dg, 0, 1);
  directedgraph_add_edge(dg, 0, 2);
  directedgraph_add_edge(dg, 0, 3);
  directedgraph_add_edge(dg, 1, 3);
  directedgraph_add_edge(dg, 2, 4);
  directedgraph_add_edge(dg, 3, 4);
  directedgraph_add_edge(dg, 5, 3);
  directedgraph_add_edge(dg, 3, 5);

  printf("Check if 2 is a successor of 0: %i\n", directedgraph_is_successor(dg, 0, 2));
  printf("Check if 0 is a predecessor of 2: %i\n", directedgraph_is_predecessor(dg, 2, 0));

  printf("Print graph:\n");
  directedgraph_print(dg);

  printf("Removing edges (0,2) (2,4)\n");
  directedgraph_remove_edge(dg, 0, 2);
  directedgraph_remove_edge(dg, 0, 2);
  directedgraph_remove_edge(dg, 2, 4);
  directedgraph_remove_edge(dg, 1, 4);
  directedgraph_remove_edge(dg, 3, 5);
  directedgraph_remove_edge(dg, 5, 3);

  printf("Check if 2 is a successor of 0: %i\n", directedgraph_is_successor(dg, 0, 2));
  printf("Check if 0 is a predecessor of 2: %i\n", directedgraph_is_predecessor(dg, 2, 0));

  printf("Print graph:\n");
  directedgraph_print(dg);

  printf("Freeing graph\n");
  directedgraph_free(dg);

  printf("\n\n");
  printf("Creating Graph\n");
  graph* g = graph_create(10);

  printf("Print empty graph:\n");
  graph_print(g);

  printf("Adding edges (0,3) (0,6) (0,5) (1,3) (2,4) (3,4)\n");
  graph_add_edge(g, 0, 3);
  graph_add_edge(g, 0, 6);
  graph_add_edge(g, 0, 5);
  graph_add_edge(g, 1, 3);
  graph_add_edge(g, 2, 4);
  graph_add_edge(g, 3, 4);
  graph_add_edge(g, 5, 3);
  graph_add_edge(g, 3, 5);

  printf("Check if edge (0,2) is in the graph: %i\n", graph_is_edge(g, 0, 2));
  printf("Check if edge (2,0) is in the graph: %i\n", graph_is_edge(g, 0, 2));

  printf("Print graph:\n");
  graph_print(g);

  printf("Removing edges (0,2) (2,4)\n");
  graph_remove_edge(g, 0, 2);
  graph_remove_edge(g, 0, 2);
  graph_remove_edge(g, 2, 4);
  graph_remove_edge(g, 1, 4);
  graph_remove_edge(g, 3, 5);
  graph_remove_edge(g, 5, 3);

  printf("Check if (2,0) is in the graph: %i\n", graph_is_edge(g, 0, 2));
  printf("Check if (0,2) is in the graph: %i\n", graph_is_edge(g, 2, 0));

  printf("Print graph:\n");
  graph_print(g);

  printf("Freeing graph\n");
  graph_free(g);

  printf("\n\n");
  printf("Creating Fibonacci Heap\n");
  fib_heap* h = fib_heap_create();

  printf("Print empty fibonacci heap\n");
  fib_heap_print(h);
  printf("Heap size (should be 0): %i\n", fib_heap_size(h));

  printf("Insert new keys (1, 2, 3, 4, 5)\n");
  fib_heap_insertion(h, 5);
  fib_heap_print(h);
  printf("Heap size (should be 1): %i\n", fib_heap_size(h));
  printf("\n");
  fib_heap_insertion(h, 2);
  fib_heap_print(h);
  printf("Heap size (should be 2): %i\n", fib_heap_size(h));
  printf("\n");
  fib_heap_insertion(h, 3);
  fib_heap_print(h);
  printf("Heap size (should be 3): %i\n", fib_heap_size(h));
  printf("\n");
  fib_heap_insertion(h, 4);
  fib_heap_print(h);
  printf("Heap size (should be 4): %i\n", fib_heap_size(h));
  printf("\n");
  fib_heap_insertion(h, 1);
  fib_heap_print(h);
  printf("Heap size (should be 5): %i\n", fib_heap_size(h));
  printf("\n");

  printf("Create another Fibonacci heap\n");
  fib_heap* hh = fib_heap_create();

  printf("Insert new keys (6, 7, 8, 9, 10)\n");
  fib_heap_insertion(hh, 6);
  fib_heap_insertion(hh, 7);
  fib_heap_insertion(hh, 8);
  fib_heap_insertion(hh, 9);
  fib_heap_insertion(hh, 10);

  printf("Union the two heaps\n");
  h = fib_heap_union(h, hh);

  printf("Heap size (should be 10): %i\n", fib_heap_size(h));

  printf("Print the unioned heap\n");
  fib_heap_print(h);

  printf("Extract Minimum Value\n");
  printf("Minimum value: %i\n", fib_heap_extract_min(h));
  printf("Heap Size (should be 9): %i\n", fib_heap_size(h));

  printf("Extract Minimum Value:\n");
  printf("Minimum value: %i\n", fib_heap_extract_min(h));
  printf("Minimum value: %i\n", fib_heap_extract_min(h));

  printf("\nUnion two empty fibonacci heaps\n");
  h = fib_heap_create();
  hh = fib_heap_create();
  h = fib_heap_union(h, hh);
  fib_heap_print(h);

  printf("\nUnion two fibonacci heaps of size 1\n");
  h = fib_heap_create();
  hh = fib_heap_create();
  fib_heap_insertion(h, 2);
  fib_heap_insertion(hh, 1);
  h = fib_heap_union(h, hh);
  fib_heap_print(h);
}
