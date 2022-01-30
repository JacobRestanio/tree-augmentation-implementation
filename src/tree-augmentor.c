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

   //test tree roots and create child thing.
   //test degree correctness.

   //degree changes when adding
   //degree changes when merging
   //degree changes when removing

   char* t = "5 2\n5 3\n3 4\n4 8\n4 10\n2 1\n1 7\n1 6\n1 9\n11 3";
   int size = 11;


   graph* g = graph_create_text(t,size);

   graph_print(g);

   set_root(g,4);

   for(int i = 1; i<=size; i++){
      printf("%i\t%i\n", i, g->parents[i]);
      fflush(stdout);
   }

   merge_vertices(g,3,5);


   printf("\n\n");

   set_root(g,9);

   for(int i = 1; i<=size; i++){
      printf("%i\t%i\n", value(g,i), value(g,g->parents[value(g,i)]));
      fflush(stdout);
   }

   //graph_print(g);
   

}
