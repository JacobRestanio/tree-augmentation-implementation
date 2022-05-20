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

   /* Create a 3-dimensional array to keep average solutions */
   float ***averagedSolutions = malloc(sizeof(float **)*numAlgorithms);
   for (int i = 0; i < numAlgorithms; i++) {
      averagedSolutions[i] = malloc(sizeof(float *)*treeSizeLen);
      for (int j = 0; j < treeSizeLen; j++) {
         averagedSolutions[i][j] = malloc(sizeof(float[6]));
         memset(averagedSolutions[i][j], 0, sizeof(float[6]));
      }
   }

    for(int n=0; n < treeSizeLen; n++){
      for(int treetype = 0; treetype < 6; treetype++){
        for(int i = 0; i < iterations; i++) {

      	   /* allocate, initialize, then create a tree of a certain type */
		      graph* tree = NULL;
            char treeFileName[255];
            /* Determines the file name of the next tree (or generates a tree) */
		      switch(treetype) {
		         case 0:
		            printf("\nRANDOM FOREST: Size = %i\n\n", treesize[n]);
                  sprintf(treeFileName, "randomforest%i_%i", treesize[n], i+1);
		            tree = randomForestTree(treesize[n]);
		            break;
		         case 1:
		            printf("\nLINEAR TREE: Size = %i\n\n", treesize[n]);
                  sprintf(treeFileName, "lineartree%i_%i", treesize[n], i+1);
                  tree = linearTree(treesize[n]);
		            break;
		         case 2:
                  printf("\nSTAR TREE: Size = %i\n\n", treesize[n]);
                  sprintf(treeFileName, "startree%i_%i", treesize[n], i+1);
                  tree = starTree(treesize[n]);
		            break;
		         case 3:
		            printf("\nSTARLIKE TREE: Size = %i\n\n", treesize[n]);
                  sprintf(treeFileName, "starliketree%i_%i", treesize[n], i+1);
		            tree = starlikeTree(treesize[n]);
		            break;
		         case 4:
		            printf("\nCATERPILLAR TREE: Size = %i\n\n", treesize[n]);
                  sprintf(treeFileName, "caterpillartree%i_%i", treesize[n], i+1);
		            tree = caterpillarTree(treesize[n]);
		            break;
		         case 5:
		            printf("\nLOBSTER TREE: Size = %i\n\n", treesize[n]);
                  sprintf(treeFileName, "lobstertree%i_%i", treesize[n], i+1);
		            tree = lobsterTree(treesize[n]);
		            break;
		         default:
		            printf("\ninvalid selection\n\n");
		            break;
		      }
            /* ______________________READ INPUT______________________ */
            /* Writes generated tree to file */
            // graph_print_to_file(tree, filename);
            /* Reads generated tree from file */
            // tree = graph_read_from_file(treeFileName);

            /* Create or load edge set */
            char edgeSetFileName[263];
            sprintf(edgeSetFileName, "%s_edgeset", treeFileName);
            graph* edgeSet = graph_create(treesize[n]);
            /* Generates an edge set based off tree */
            printf("Generating Edge Set based off tree...\n");
            do {
               printf("\tAttempting to generate valid edge set...\n");
               createEdgeSet(1000, tree, edgeSet);
            } while (!checkConnected(tree, edgeSet));
            printf("\tFinished\n\n");
            /* Writes generated edge set to file */
            // graph_print_to_file(edgeSet, edgeSetFileName);
            /* Reads generated edge set from file */
            // edgeSet = graph_read_from_file(edgeSetFileName);


            /* ______________________GREEDY ALGORITHM_______________________ */
            int num_edges_greedy;
            clock_t time_greedy;
            printf("Running Greedy Algorithm...\n");
            time_greedy = clock();
            num_edges_greedy = greedyHeuristic(tree, edgeSet);
            time_greedy = clock() - time_greedy;
            printf("\tFinished\n");
            printf("Greedy Algorithm: %i\n\n", num_edges_greedy);

            averagedTimes[0][n][treetype] = i > 0 ? (i * averagedTimes[0][n][treetype] + time_greedy) / (i + 1) : time_greedy;
            averagedSolutions[0][n][treetype] = i > 0 ? (i * averagedSolutions[0][n][treetype] + num_edges_greedy) / (i + 1) : num_edges_greedy;

            /* ____________________FREDRICKSON ALGORITHM_______________________ */
            // num_edges = fredrickson(treesize[n], tree);
            // printf("Fredrickson Algorithm: %i\n", num_edges);
            int num_edges_fredrickson;
            clock_t time_fredrickson;
            printf("Running Fredrickson Algorithm...\n");
            time_fredrickson = clock();
            num_edges_fredrickson = fredrickson(tree, edgeSet);
            time_fredrickson = clock() - time_fredrickson;
            printf("\tFinished\n");
            printf("Fredrickson Algorithm: %i\n\n", num_edges_fredrickson);

            averagedTimes[1][n][treetype] = i > 0 ? (i * averagedTimes[1][n][treetype] + time_fredrickson) / (i + 1) : time_fredrickson;
            averagedSolutions[1][n][treetype] = i > 0 ? (i * averagedSolutions[1][n][treetype] + num_edges_fredrickson) / (i + 1) : num_edges_fredrickson;

            graph_free(edgeSet);
            graph_free(tree);

            /* PREVIOUS TREE ADJACENCY IMPLEMENTATION *//* PREVIOUS TREE ADJACENCY IMPLEMENTATION */
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
   printf("\n\n");
   printf("\t\t    RANDOM\t    LINEAR\t      STAR\t  STARLIKE\tCATERPILLAR\t   LOBSTER\n");
   for (int i = 0; i < numAlgorithms; i++) {
      switch(i) {
         case 0:
            printf("RANDOM:\n");
            break;
         case 1:
            printf("FREDRICKSON:\n");
            break;
      }
      for (int j = 0; j < treeSizeLen; j++) {
         printf("  SIZE = %i\t\n", treesize[j]);
         if (treesize[j] <= 100) {
            printf("    time (ms):\t");
         } else {
            printf("    time (s):\t");
         }
         for (int k = 0; k < 6; k++) {
            if (treesize[j] <= 100) {
               printf("%10.3lf\t", 1000 * averagedTimes[i][j][k] / CLOCKS_PER_SEC);
            } else {
               printf("%10.3lf\t", averagedTimes[i][j][k] / CLOCKS_PER_SEC);
            }
         }
         printf("\n");
         printf("    num edges:\t");
         for (int k = 0; k < 6; k++) {
            printf("%10.3lf\t", averagedSolutions[i][j][k]);
         }
         printf("\n");
      }
   }

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
