#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include "../include/tree-generator.h"
#include "../include/tree-helper.h"
#include "../include/tree-greedy.h"
#include "../include/graph.h"
#include "../include/int-list.h"

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

   char* t_text = "20 1\n1 3\n7 1\n7 8\n9 7\n9 10\n10 11\n11 12\n20 2\n20 4\n4 5\n4 6\n6 13\n6 14\n6 15\n14 16\n14 17\n17 18\n17 19\n8 21";
   char* g_text = "20 1\n1 3\n7 1\n7 8\n9 7\n9 10\n10 11\n11 12\n20 2\n20 4\n4 5\n4 6\n6 13\n6 14\n6 15\n14 16\n14 17\n17 18\n17 19\n8 21\n17 15\n19 18\n19 16\n14 2\n13 3";
   int size = 21;

   graph* t = graph_create_text(t_text,size);
   graph* g = graph_create_text(g_text,size);

   set_root(t,20);

   int x[5] = {1, 4, 6, 14, 17};

   for(int i= 1; i<22; i++){
      char c = lf_closed(g,t,i);
      printf("   {n:%i lf:%i}   \n", i, c);
   }

   printf("\n\n");
}
