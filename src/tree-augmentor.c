#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
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
   srand(time(0));

   char* t = "1\n2\n3\n4\n1 2\n2 3\n3 4";

   
   graph* g = graph_create_text(t, 4);

   graph_print(g);

   printf("\n\n");



   
   printf("merging:\n");
   merge_vertices(g, 1, 4);
   printf("merged.\n");
   graph_print(g);

   printf("\n\n");

   printf("removing\n");
   remove_edge(g,1,2);
   printf("removed\n");
   graph_print(g);

}
