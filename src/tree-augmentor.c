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

   char* t = "1 6\n1 7\n1 2\n2 1\n2 5\n3 11\n3 4\n3 5\n4 10\n4 8\n4 3\n5 3\n5 2\n6 1\n7 1\n8 4\n9 1\n10 4\n11 3\n0\n13 3\n12 11\n12 14\n15 12\n";
   int size = 15;


   graph* g = graph_create_text(t,size);

   graph_print(g);
   printf("\n\n");

   set_root(g,9);

   for(int i = 1; i<=size; i++){
      printf("%i -> %i parent: %i \t depth: %i\n", i , value(g,i), get_parent(g,i), get_depth(g,i));
      fflush(stdout);
   }

   printf("\n\n");

   merge_path(g, 1, 14);

   graph_print(g);

}
