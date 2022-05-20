#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"

int greedyHeuristic(graph* tree, graph* edgeSet){
   /* Check if a 2-edge connected graph is possible */
   int n = graph_get_number_of_vertices(tree);
   if (checkConnected(tree, edgeSet) == 0) {
      printf("impossible to connect tree and edge set\n");
      return 0;
   }

   /* Begin Greedy Algorithm */
   int e = 0;
   graph* edges = graph_create(n);

   for (int i = 0; i < n; i++) {
      int degree = 0;
      /* Check if the degree of node i is less than 2 */
      for (int j = 0; j < n; j++) {
         if (graph_is_edge(tree, i, j) || graph_is_edge(edges, i, j)) {
            degree++;
            if (degree >= 2) {
               break;
            }
         }
      }

      /* Arbitrarily add an edge if the degree is less than 2 */
      if (degree < 2) {
         for (int j = 0; j < n; j++) {
            if (!graph_is_edge(tree, i, j) && !graph_is_edge(edges, i, j) && graph_is_edge(edgeSet, i, j)) {
               graph_add_edge(edges, i, j);
               e++;
               break;
            }
         }
      }
   }

   /* Check if the graph is 2-edge connected */
   while (checkConnected(tree, edges) == 0) {
      /* Arbitrarily add edges until the graph is 2-edge connected */
      int i = rand() % n;
      for (int j = 0; j < n; j++) {
         if (!graph_is_edge(tree, i, j) && !graph_is_edge(edges, i, j) && graph_is_edge(edgeSet, i, j)) {
             graph_add_edge(edges, i, j);
             e++;
             break;
         }
      }
   }

   // printf("Edges to Add:\n");
   // graph_print(edges);
   graph_free(edges);
   return e;
}
