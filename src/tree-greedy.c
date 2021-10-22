#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/tree-check-connected.h"
#include "../include/tree-helper.h"

int greedyHeuristic(int n, int tree[][n], int edgeSet[][n]){
   /* Check if a 2-edge connected graph is possible */
   if (checkConnected(n, tree, edgeSet) == 0) {
      return 0;
   }
   /* Begin Greedy Algorithm */
   srand(time(NULL));
   int e = 0;
   int (*edges)[n] = malloc(sizeof(int[n][n]));
   memset(edges, 0, sizeof(int)*n*n);
  
   for (int i = 0; i < n; i++) {
      int degree = 0;
      /* Check if the degree of node i is less than 2 */
      for (int j = 0; j < n; j++) {
         if (tree[i][j] == 1 || edges[i][j] == 1) {
            degree++;
            if (degree >= 2) {
               break;
            }
         }
      }
      /* Arbitrarily add an edge if the degree is less than 2 */
      if (degree < 2) {
         for (int j = 0; j < n; j++) {
            if (tree[i][j] == 0 && edges[i][j] == 0 && edgeSet[i][j] == 1) {
               edges[i][j] = 1;
               edges[j][i] = 1;
               e++;
               break;
            }
         }
      }
   }
   /* Check if the graph is 2-edge connected */
   while (checkConnected(n, tree, edges) == 0) {
      /* Arbitrarily add edges until the graph is 2-edge connected */
      int i = rand() % n;
      for (int j = 0; j < n; j++) {
         if  (tree[i][j] == 0 && edges[i][j] == 0 && edgeSet[i][j] ==1) {
            edges[i][j] = 1;
            edges[j][i] = 1;
            e++;
            break;
         }
      }
   }
   
   /* combine tree and edges into a single graph */
/*   int (*graph)[n] = malloc(sizeof(int[n][n]));*/
/*   memset(graph, 0, sizeof(int)*n*n);*/
/*   for (int i = 0; i < n; i++) {*/
/*      for (int j = 0; j < n; j++) {*/
/*         if (tree[i][j] == 1 || edges[i][j] == 1) {*/
/*            graph[i][j] = 1;*/
/*         }*/
/*      }*/
/*   }*/
/*   printf("GREEDY 2-EDGE CONNECTED GRAPH\n");*/
/*   printTreeAdjMat(n, graph);*/
/*   printf("\n");*/

/*   free(graph);*/
   free(edges);
   return e;
}

