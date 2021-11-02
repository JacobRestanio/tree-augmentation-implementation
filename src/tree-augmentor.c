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
   srand(time(0));
   int iterations = atoi(argv[argc-1]);
   int treesize[argc-2];
   for (int i = 1; i < argc - 1; i++) {
      treesize[i-1] = atoi(argv[i]);
   }

   int numAlgorithms=2; //number of algorithms we have implemented
   int treeSizeLen=sizeof(treesize)/sizeof(treesize[0]);

   /* Create a 3-dimensional array to keep average times */
   float ***averagedTimes = malloc(sizeof(float **)*numAlgorithms);
   for (int i = 0; i < numAlgorithms; i++) {
      averagedTimes[i] = malloc(sizeof(float *)*treeSizeLen);
      for (int j = 0; j < treeSizeLen; j++) {
         averagedTimes[i][j] = malloc(sizeof(float[6]));
         memset(averagedTimes[i][j], 0, sizeof(float[6]));
      }
   }

    for(int n=0; n < treeSizeLen; n++){
      for(int treetype = 0; treetype < 6; treetype++){
        for(int i = 0; i < iterations; i++) {

      	   /* allocate, initialize, then create a tree of a certain type */
		      graph* g = NULL;
		      switch(treetype) {
		         case 0:
		            printf("RANDOM FOREST: Size = %i\n\n", treesize[n]);
		            g = randomForestTree(treesize[n]);
		            break;
		         case 1:
		            printf("LINEAR TREE: Size = %i\n\n", treesize[n]);
		            g = linearTree(treesize[n]);
		            break;
		         case 2:
		            printf("STAR TREE: Size = %i\n\n", treesize[n]);
		            g = starTree(treesize[n]);
		            break;
		         case 3:
		            printf("STARLIKE TREE: Size = %i\n\n", treesize[n]);
		            g = starlikeTree(treesize[n]);
		            break;
		         case 4:
		            printf("CATERPILLAR TREE: Size = %i\n\n", treesize[n]);
		            g = caterpillarTree(treesize[n]);
		            break;
		         case 5:
		            printf("LOBSTER TREE: Size = %i\n\n", treesize[n]);
		            g = lobsterTree(treesize[n]);
		            break;
		         default:
		            printf("invalid selection\n");
		            break;
		      }

          graph_free(g);

            // /* allocate, initialize, and create edge set */
            // int (*edgeSet)[treesize[n]] = malloc(sizeof(int[treesize[n]][treesize[n]]));
            // memset(edgeSet, 0, sizeof(edgeSet[0][0])*treesize[n]*treesize[n]);
            // createEdgeSet(treesize[n], tree, edgeSet);
            //
            // /* ______________________GREEDY ALGORITHM_______________________*/
            //
            // /* begin clock */
            // clock_t time = clock();
            //
            // /* call greedy algorithm */
            // int greedy = greedyHeuristic(treesize[n], tree, edgeSet);
            // printf("Greedy Algorithm returned %d \n", greedy);
            //
            // /* end clock */
            // time = clock() - time;
            //
            // /* average time for algorithm on specific tree */
            // /* FIX */
            // if (i > 0) {
            //    averagedTimes[0][n][treetype] = (i * averagedTimes[0][n][treetype] + time) / (i + 1);
            // } else {
            //    averagedTimes[0][n][treetype] = time;
            // }
            //
            // /* ____________________FREDRICKSON ALGORITHM____________________ */
            //
            // /* begin clock */
            // time = clock();
            //
            // /* fredrickson algorithm */
            // int fredrick = fredrickson(treesize[n], tree);
            // printf("Fredrickson Algorithm returned %i \n", fredrick);
            //
            // /* end clock */
            // time = clock() - time;
            //
            // /* average time for algorithm on specific tree */
            // if (i > 0) {
            //    averagedTimes[1][n][treetype] = (i * averagedTimes[1][n][treetype] + time) / (i + 1);
            // } else {
            //    averagedTimes[1][n][treetype] = time;
            // }
            //
            // printf("_______________________\n\n");
            //
            // /* every call to malloc needs a corresponding call to free */
            // free(tree);
            // free(edgeSet);
		   }
     }
   }
   /* print the average times for the algorithms */
   // for (int i = 0; i < numAlgorithms; i++) {
   //    for (int j = 0; j < treeSizeLen; j++) {
   //       for (int k = 0; k < 6; k++) {
   //          printf("%.3lf\t\t", 1000 * averagedTimes[i][j][k] / CLOCKS_PER_SEC);
   //       }
   //       printf("\n");
   //    }
   // }

   /* free the memory used by averaged times */
   for (int i = 0; i < numAlgorithms; i++) {
      for (int j = 0; j < treeSizeLen; j++) {
         free(averagedTimes[i][j]);
      }
      free(averagedTimes[i]);
   }
   free(averagedTimes);

/*   commented out for future use */
/*   genPlot(numAlgorithms,treeSizeLen,timeSize, times, treesize); //generate Latex plots*/
}
