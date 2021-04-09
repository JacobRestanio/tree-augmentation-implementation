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
      //unweighted min cover using linear tree algorithm
      int vertexWeights[treesize];
      for(int i=0; i<treesize; i++){ //sets all to 1 for unweighted
      	vertexWeights[i]=1;
      }
      clock_t t1; //begin clock time
      t1 = clock();
      printMinCover(treesize, tree,vertexWeights);
      t1 = clock() - t1; // end clock time
      double time_taken1 = ((double)t1)/CLOCKS_PER_SEC;
      printf("It took %f seconds to execute \n", time_taken1);
      //unweighted min cover using brute force
      clock_t t2; //begin clock time
      t2 = clock();
      printVertexCover(treesize, tree, vertexWeights);
      t2 = clock() - t2; // end clock time
      double time_taken2 = ((double)t2)/CLOCKS_PER_SEC;
      printf("It took %f seconds to execute \n", time_taken2);
      //weighted min cover using linear tree algorithm
      genVertexWeights(treesize, vertexWeights, 10); //generate random vertex weights from 1-10
      /*print vertex weights (leaving here in case we need it)
      printf("{ ");
      for(int i=0; i<treesize; i++){
      	printf(",%d ", vertexWeights[i]);
      }
      printf("}\n");
      */
      clock_t t3; //begin clock time
      t3 = clock();
      printMinCover(treesize, tree, vertexWeights);
      t3 = clock() - t3; // end clock time
      double time_taken3 = ((double)t3)/CLOCKS_PER_SEC;
      printf("It took %f seconds to execute \n", time_taken3);
      //weighted min cover using brute force
      clock_t t4; //begin clock time
      t4 = clock();
      printVertexCover(treesize, tree, vertexWeights);
      t4 = clock() - t4; // end clock time
      double time_taken4 = ((double)t4)/CLOCKS_PER_SEC;
      printf("It took %f seconds to execute \n", time_taken4);
      printf("Continue? [y/n]: ");
      scanf(" %c", &exit);
      printf("\n");
   }
}
