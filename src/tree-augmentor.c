#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include "../include/fredrickson.h"
#include "../include/tree-generator.h"
#include "../include/tree-helper.h"
#include "../include/tree-greedy.h"
#include "../include/graph.h"

/* Temporary Function to some debugging info */
/* Some change but in a different branch */
void printInfo(int n, int tree[][n], int edgeSet[][n], int edgeWeights[][n]) {
   printf("TREE\n");
   printTreeAdjMat(n, tree);
   printf("\nEDGE SET\n");
   printTreeAdjMat(n, edgeSet);
   printf("\nEDGE WEIGHTS\n");
   printTreeAdjMatWithPadding(n, edgeWeights, 1);
   printf("___________________________________\n\n");
}

int main(int argc, char *argv[]) {
   printf("begin.\n");
   srand(time(0));

   char* t = "0\n1\n2\n3\n0 1\n1 2\n2 3";
   
   const int treesize = 25;

   graph* tree = graph_construct_csacademy(t, 4);
   graph_add_edge(tree, 0, 1);

   graph_print_csacademy(tree);

   graph* edgeSet = graph_create(treesize);
   createEdgeSet(treesize, tree, edgeSet);
   // printf("EDGESET:\n");
   // graph_print(edgeSet);


   /* ______________________GREEDY ALGORITHM_______________________ */
   int num_edges = greedyHeuristic(treesize, tree, edgeSet);
   //printf("Greedy Algorithm: %i\n", num_edges);


}
