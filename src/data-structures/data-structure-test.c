#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../../include/stack.h"
#include "../../include/graph.h"
#include "../../include/fibonacci-heap.h"
#include "../../include/queue.h"
#include "../../include/list.h"
#include "../../include/int-list.h"

int main() {
   printf("\n\n");
   printf("_____LIST_____\n");
   printf("Creating int list 1\n");
   int_list* int_ls1 = int_list_create(1);
   int_ls1 = list_add(int_ls1, int_list_create(2));
   int_ls1 = list_add(int_ls1, int_list_create(3));
   list_print(int_ls1, int_list_print);

   printf("Creating int list 2\n");
   int_list* int_ls2 = int_list_create(4);
   int_ls2 = list_add(int_ls2, int_list_create(5));
   int_ls2 = list_add(int_ls2, int_list_create(6));
   int_ls2 = list_add(int_ls2, int_list_create(7));
   list_print(int_ls2, int_list_print);

   printf("Size of list 1: %i\n", list_size(int_ls1));
   printf("Size of list 2: %i\n\n", list_size(int_ls2));

   printf("Merging list 1 and list 2\n");
   int_ls1 = list_merge(int_ls1, int_ls2);
   printf("List 1: ");
   list_print(int_ls1, int_list_print);
   printf("List 2: ");
   list_print(list_first(int_ls2), int_list_print);

   printf("Removing from combined list elements 1, 4, and 3\n");
   int_ls2 = int_list_create(1);
   int_ls1 = list_remove_elem(int_ls1, int_list_match, free, int_ls2);
   int_ls2 = list_remove(int_ls2, free);
   int_ls2 = int_list_create(4);
   int_ls1 = list_remove_elem(int_ls1, int_list_match, free, int_ls2);
   int_ls2 = list_remove(int_ls2, free);
   int_ls2 = int_list_create(3);
   int_ls1 = list_remove_elem(int_ls1, int_list_match, free, int_ls2);
   int_ls2 = list_remove(int_ls2, free);
   list_print(int_ls1, int_list_print);

   printf("Adding 1, 4, and 7 back to list\n");
   int_ls1 = list_add(int_ls1, int_list_create(1));
   int_ls1 = list_add(int_ls1, int_list_create(4));
   int_ls1 = list_add(int_ls1, int_list_create(7));
   list_print(int_ls1, int_list_print);

   printf("Removing from combined list elements [2, 5, 7]\n");
   int_ls2 = int_list_create(2);
   int_ls2 = list_add(int_ls2, int_list_create(5));
   int_ls2 = list_add(int_ls2, int_list_create(7));
   int_ls1 = list_remove_list(int_ls1, int_list_match, free, int_ls2);
   list_print(int_ls1, int_list_print);

   printf("Adding 2, 6, and 7 back to list\n");
   int_ls1 = list_add(int_ls1, int_list_create(2));
   int_ls1 = list_add(int_ls1, int_list_create(6));
   int_ls1 = list_add(int_ls1, int_list_create(7));
   list_print(int_ls1, int_list_print);

   printf("Freeing list 1 and list 2\n");
   list_free(int_ls1, free);
   list_free(int_ls2, free);





  printf("\n\n");
  printf("_____STACK_____\n");
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
  printf("_____DIRECTED GRAPH_____\n");
  printf("Create Empty Directed Graph\n");
  directedgraph* dg = directedgraph_create(0);
  directedgraph_print(dg);
  printf("Adding vertices 1, 2, 3 to empty graph\n");
  directedgraph_add_vertex(dg, 1);
  directedgraph_add_vertex(dg, 2);
  directedgraph_add_vertex(dg, 3);
  directedgraph_print(dg);
  printf("Freeing directed graph\n");
  directedgraph_free(dg);
  printf("Creating Directed Graph with 5 vertices\n");
  dg = directedgraph_create(5);

  printf("Print empty graph:\n");
  directedgraph_print(dg);

  printf("Adding edges (1,2), (1,3), (1,4), (2,4), (3,1), (3,5), (4,5), (5,2)\n");
  directedgraph_add_edge(dg, 1, 2);
  directedgraph_add_edge(dg, 1, 2);
  directedgraph_add_edge(dg, 1, 3);
  directedgraph_add_edge(dg, 1, 4);
  directedgraph_add_edge(dg, 2, 4);
  directedgraph_add_edge(dg, 3, 5);
  directedgraph_add_edge(dg, 3, 1);
  directedgraph_add_edge(dg, 5, 2);
  directedgraph_add_edge(dg, 4, 5);
  directedgraph_add_edge(dg, 6, 3);
  directedgraph_add_edge(dg, 4, 6);

  printf("Print graph:\n");
  directedgraph_print(dg);

  printf("Create copy of graph:\n");
  directedgraph* dg_copy = directedgraph_create_copy(dg);
  directedgraph_print_weights(dg_copy);

  printf("Update cost of existing edge (1,3):\n");
  directedgraph_add_weighted_edge(dg_copy, 1, 3, 0);
  directedgraph_print_weights(dg_copy);

  printf("Removing vertices 0, 2, 4:\n");
  directedgraph_remove_vertex(dg, 0);
  directedgraph_remove_vertex(dg, 2);
  directedgraph_remove_vertex(dg, 4);
  directedgraph_print(dg);

  printf("Adding vertices 1, 2, 6\n");
  directedgraph_add_vertex(dg, 1);
  directedgraph_add_vertex(dg, 2);
  directedgraph_add_vertex(dg, 6);

  printf("Adding edges (0,1) (1,2) (2,3) (1,3) (5,3) (3,5)\n");
  directedgraph_add_edge(dg, 0, 1);
  directedgraph_add_edge(dg, 0, 1);
  directedgraph_add_edge(dg, 1, 2);
  directedgraph_add_edge(dg, 2, 3);
  directedgraph_add_edge(dg, 1, 3);
  directedgraph_add_edge(dg, 2, 4);
  directedgraph_add_edge(dg, 3, 4);
  directedgraph_add_edge(dg, 5, 3);
  directedgraph_add_edge(dg, 3, 5);

  printf("Check if 2 is a successor of 1: %i\n", directedgraph_is_successor(dg, 1, 2));
  printf("Check if 1 is a predecessor of 2: %i\n", directedgraph_is_predecessor(dg, 1, 2));

  printf("Print graph:\n");
  directedgraph_print(dg);
  directedgraph_print_predecessors(dg);

  printf("Finding a cycle in the directed graph:\n");
  adjlist* cycle = adjlist_find_cycle_in_directedgraph(dg);
  adjlist_print(cycle);
  adjlist_free(cycle);

  printf("Removing all predecessors of vertex 3\n");
  directedgraph_remove_predecessors(dg, 3);

  printf("Removing edges (1,2) (3,5) (5,3)\n");
  directedgraph_remove_edge(dg, 1, 2);
  directedgraph_remove_edge(dg, 3, 5);
  directedgraph_remove_edge(dg, 5, 3);

  printf("Check if 2 is a successor of 1: %i\n", directedgraph_is_successor(dg, 1, 2));
  printf("Check if 1 is a predecessor of 2: %i\n", directedgraph_is_predecessor(dg, 1, 2));

  printf("Print directed graph:\n");
  directedgraph_print(dg);

  printf("Free directed graph\n");
  directedgraph_free(dg);
  directedgraph_free(dg_copy);

  printf("\n\n");
  printf("_____GRAPH_____\n");
  printf("Creating Graph of size 5\n");
  graph* g = graph_create(5);

  printf("Print graph:\n");
  graph_print(g);

  printf("Adding edges (1,2) (1,3) (1,6) (1,5) (2,3) (2,4) (3,3) (3,4) (5,3) (3,5)\n");
  graph_add_edge(g, 1, 2);
  graph_add_edge(g, 1, 3);
  graph_add_edge(g, 1, 6);
  graph_add_edge(g, 1, 5);
  graph_add_edge(g, 2, 3);
  graph_add_edge(g, 3, 3);
  graph_add_edge(g, 2, 4);
  graph_add_edge(g, 3, 4);
  graph_add_edge(g, 5, 3);
  graph_add_edge(g, 3, 5);

  printf("Print graph:\n");
  graph_print(g);

  printf("Making a copy of the current graph\n");
  graph* gcopy = graph_copy(g);
  graph_print(gcopy);

  printf("Removing edges (1,2) (1,2) (2,3) (2,4) (1,4) (3,3) (6,7) (6,3) (3,6)\n");
  graph_remove_edge(g, 1, 2);
  graph_remove_edge(g, 1, 2);
  graph_remove_edge(g, 2, 3);
  graph_remove_edge(g, 2, 4);
  graph_remove_edge(g, 1, 4);
  graph_remove_edge(g, 3, 3);
  graph_remove_edge(g, 6, 7);
  graph_remove_edge(g, 6, 3);
  graph_remove_edge(g, 3, 6);

  printf("Print graph:\n");
  graph_print(g);

  printf("Remove vertex 3 and 6\n");
  graph_remove_vertex(g, 3);
  graph_remove_vertex(g, 6);
  graph_print(g);

  printf("Making sure the copy hasn't changed\n");
  graph_print(gcopy);

  printf("Freeing graph\n");
  graph_free(g);
  graph_free(gcopy);

  /* Queue */
  printf("\n\n");
  printf("_____QUEUE_____\n");
  printf("Creating queue\n");
  queue* q = queue_create();

  printf("Check if queue is empty: %i\n", queue_isEmpty(q));

  printf("Enqueue 1, 2, 3\n");
  int_ls1 = int_list_create(1);
  int_ls2 = int_list_create(2);
  int_list* int_ls3 = int_list_create(3);
  queue_enqueue(q, int_ls1);
  queue_enqueue(q, int_ls2);
  queue_enqueue(q, int_ls3);
  queue_print(q, int_list_print);

  printf("Check if queue is empty: %i\n", queue_isEmpty(q));

  printf("Dequeue 1\n");
  queue_dequeue(q);
  queue_print(q, int_list_print);

  printf("Dequeue 2\n");
  queue_dequeue(q);
  queue_print(q, int_list_print);

  printf("Peek at front of queue: %i\n", int_list_value(queue_peek(q)));

  printf("Dequeue 3\n");
  queue_dequeue(q);
  queue_print(q, int_list_print);

  printf("Dequeue empty\n");
  queue_dequeue(q);
  queue_print(q, int_list_print);

  printf("Enqueue 1, 2, 3\n");
  queue_enqueue(q, int_ls1);
  queue_enqueue(q, int_ls2);
  queue_enqueue(q, int_ls3);
  queue_print(q, int_list_print);

  int_list_free(int_ls1);
  int_list_free(int_ls2);
  int_list_free(int_ls3);

  printf("Free queue\n");
  queue_free(q);



  /* Fibonacci Heap */
  // printf("\n\n");
  // printf("Creating Fibonacci Heap\n");
  // fib_heap* h = fib_heap_create();
  //
  // printf("Print empty fibonacci heap\n");
  // fib_heap_print(h);
  // printf("Heap size (should be 0): %i\n", fib_heap_size(h));
  //
  // printf("Insert new keys (1, 2, 3, 4, 5)\n");
  // fib_heap_insertion(h, 5);
  // fib_heap_print(h);
  // printf("Heap size (should be 1): %i\n", fib_heap_size(h));
  // printf("\n");
  // fib_heap_insertion(h, 2);
  // fib_heap_print(h);
  // printf("Heap size (should be 2): %i\n", fib_heap_size(h));
  // printf("\n");
  // fib_heap_insertion(h, 3);
  // fib_heap_print(h);
  // printf("Heap size (should be 3): %i\n", fib_heap_size(h));
  // printf("\n");
  // fib_heap_insertion(h, 4);
  // fib_heap_print(h);
  // printf("Heap size (should be 4): %i\n", fib_heap_size(h));
  // printf("\n");
  // fib_heap_insertion(h, 1);
  // fib_heap_print(h);
  // printf("Heap size (should be 5): %i\n", fib_heap_size(h));
  // printf("\n");
  //
  // printf("Create another Fibonacci heap\n");
  // fib_heap* hh = fib_heap_create();
  //
  // printf("Insert new keys (6, 7, 8, 9, 10)\n");
  // fib_heap_insertion(hh, 6);
  // fib_heap_insertion(hh, 7);
  // fib_heap_insertion(hh, 8);
  // fib_heap_insertion(hh, 9);
  // fib_heap_insertion(hh, 10);
  //
  // printf("Union the two heaps\n");
  // h = fib_heap_union(h, hh);
  //
  // printf("Heap size (should be 10): %i\n", fib_heap_size(h));
  //
  // printf("Print the unioned heap\n");
  // fib_heap_print(h);
  //
  // printf("Extract Minimum Value\n");
  // printf("Minimum value: %i\n", fib_heap_extract_min(h));
  // printf("Heap Size (should be 9): %i\n", fib_heap_size(h));
  //
  // printf("Extract Minimum Value:\n");
  // printf("Minimum value: %i\n", fib_heap_extract_min(h));
  // printf("Minimum value: %i\n", fib_heap_extract_min(h));
  //
  // printf("\nUnion two empty fibonacci heaps\n");
  // h = fib_heap_create();
  // hh = fib_heap_create();
  // h = fib_heap_union(h, hh);
  // fib_heap_print(h);
  //
  // printf("\nUnion two fibonacci heaps of size 1\n");
  // h = fib_heap_create();
  // hh = fib_heap_create();
  // fib_heap_insertion(h, 2);
  // fib_heap_insertion(hh, 1);
  // h = fib_heap_union(h, hh);
  // fib_heap_print(h);
}
