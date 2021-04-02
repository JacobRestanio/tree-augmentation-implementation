#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/tree-generator.h"
#include "../include/tree-helper.h"

int main() {
   srand(time(0));
   int treetype;
   int treesize;
   char exit = 'y';
   while(exit == 'y') {
      printf("[1] Random Forest\n");
      printf("[2] Linear Tree\n");
      printf("[3] Star Tree\n");
      printf("[4] Star-like Tree\n");
      printf("[5] Caterpillar Tree\n");
      printf("[6] Lobster Tree\n");
      printf("Choose a tree to generate: ");
      scanf("%d", &treetype);

      printf("Enter size of tree: ");
      scanf("%d", &treesize);
      int tree[treesize][treesize];
      memset(tree, 0, sizeof tree);

      switch(treetype) {
         case 1:
            randomForestTree(treesize, tree);
            break;
         case 2:
            linearTree(treesize, tree);
            break;
         case 3:
            starTree(treesize, tree);
            break;
         case 4:
            starlikeTree(treesize, tree);
            break;
         case 5:
            caterpillarTree(treesize, tree);
            break;
         case 6:
            lobsterTree(treesize, tree);
            break;
         default:
            printf("invalid selection\n");
            break;
      }

      printf("\n");
      printTreeAdjMat(treesize, tree);
      printf("\n");
      clock_t t; //begin clock time
      t = clock();
      printVertexCover(treesize, tree);
      t = clock() - t; // end clock time
      double time_taken = ((double)t)/CLOCKS_PER_SEC;
      printf("It took %f seconds to execute \n", time_taken);
      printf("Continue? [y/n]: ");
      scanf(" %c", &exit);
      printf("\n");
   }
}
