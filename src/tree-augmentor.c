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

   /* parameters */
   int exactpoly = 1; // 1: enable exact polynomial algorithm, 0: disable
   int random = 1; // 1: enable random algorithm, 0: disable
   int frederickson = 1; // 1: enable frederickson algorithm, 0: disable
   int numAlgorithms = 2; // number of algorithms we have implemented
   int numTreeTypes = 6; // number of tree types we are testing on
   int density[3] = {100, 10, 1}; // edge set densities to test

   /* these should updated automatically when parameters are changed */
   int densityLen = sizeof(density)/sizeof(density[0]);
   int treeSizeLen = sizeof(treesize)/sizeof(treesize[0]);
   float exact[treeSizeLen][numTreeTypes];

   /* Create a 4-dimensional array to keep average times */
   float ****averagedTimes = malloc(sizeof(float ***) * numAlgorithms);
   for (int a = 0; a < numAlgorithms; a++) {
      averagedTimes[a] = malloc(sizeof(float **) * densityLen);
      for (int d = 0; d < densityLen; d++) {
         averagedTimes[a][d] = malloc(sizeof(float *) * treeSizeLen);
         for (int t = 0; t < treeSizeLen; t++) {
            averagedTimes[a][d][t] = malloc(sizeof(float[6]));
            memset(averagedTimes[a][d][t], 0, sizeof(float[6]));
         }
      }
   }

   /* Create a 4-dimensional array to keep average solutions */
   float ****averagedSolutions = malloc(sizeof(float ***) * numAlgorithms);
   for (int a = 0; a < numAlgorithms; a++) {
      averagedSolutions[a] = malloc(sizeof(float **) * densityLen);
      for (int d = 0; d < densityLen; d++) {
         averagedSolutions[a][d] = malloc(sizeof(float *) * treeSizeLen);
         for (int t = 0; t < treeSizeLen; t++) {
            averagedSolutions[a][d][t] = malloc(sizeof(float) * 6);
            memset(averagedSolutions[a][d][t], 0, sizeof(float) * 6);
         }
      }
   }

    for(int n = 0; n < treeSizeLen; n++){
      for(int treetype = 0; treetype < 6; treetype++){
         for(int d = 0; d < densityLen; d++) {
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
               int maxAttempts = 100;
               int attempts = 0;
               do {
                  printf("\tAttempting to generate valid edge set...\n");
                  createEdgeSet(density[d], tree, edgeSet);
                  attempts++;
               } while (!checkConnected(tree, edgeSet) && attempts < maxAttempts);
               if (attempts == maxAttempts) {
                  printf("\tCould not generate a valid edge set\n\n");
               } else {
                  printf("\tFinished\n\n");
               }
               /* Writes generated edge set to file */
               // graph_print_to_file(edgeSet, edgeSetFileName);
               /* Reads generated edge set from file */
               // edgeSet = graph_read_from_file(edgeSetFileName);

               if (checkConnected(tree, edgeSet)) {
                  /* ____________________EXACT POLY ALGORITHM_____________________ */
                  if (density[d] == 100 && exactpoly) {
                     int num_edges_exact = exactPolynomial(tree);
                     exact[n][treetype] = i > 0 ? (i * exact[n][treetype] + num_edges_exact) / (i + 1) : num_edges_exact;
                  }

                  /* ______________________GREEDY ALGORITHM_______________________ */
                  if (random) {
                     int num_edges_greedy;
                     clock_t time_greedy;
                     printf("Running Greedy Algorithm...\n");
                     time_greedy = clock();
                     num_edges_greedy = greedyHeuristic(tree, edgeSet);
                     time_greedy = clock() - time_greedy;
                     printf("\tFinished\n");
                     printf("Greedy Algorithm: %i\n\n", num_edges_greedy);

                     averagedTimes[0][d][n][treetype] = i > 0 ? (i * averagedTimes[0][d][n][treetype] + time_greedy) / (i + 1) : time_greedy;
                     averagedSolutions[0][d][n][treetype] = i > 0 ? (i * averagedSolutions[0][d][n][treetype] + num_edges_greedy) / (i + 1) : num_edges_greedy;
                  }

                  /* ____________________FREDRICKSON ALGORITHM____________________ */
                  if (frederickson) {
                     int num_edges_fredrickson;
                     clock_t time_fredrickson;
                     printf("Running Fredrickson Algorithm...\n");
                     time_fredrickson = clock();
                     num_edges_fredrickson = fredrickson(tree, edgeSet);
                     time_fredrickson = clock() - time_fredrickson;
                     printf("\tFinished\n");
                     printf("Fredrickson Algorithm: %i\n\n", num_edges_fredrickson);

                     averagedTimes[1][d][n][treetype] = i > 0 ? (i * averagedTimes[1][d][n][treetype] + time_fredrickson) / (i + 1) : time_fredrickson;
                     averagedSolutions[1][d][n][treetype] = i > 0 ? (i * averagedSolutions[1][d][n][treetype] + num_edges_fredrickson) / (i + 1) : num_edges_fredrickson;
                  }
               }

               graph_free(edgeSet);
               graph_free(tree);
   		   }
         }
      }
   }
   /* print the average times for the algorithms */
   printf("\n\n");
   printf("\t\t    RANDOM\t    LINEAR\t      STAR\t  STARLIKE\tCATERPILLAR\t   LOBSTER\n");
   printf("EXACT:\n");
   for (int t = 0; t < treeSizeLen; t++) {
      printf("  SIZE = %i\n", treesize[t]);
      printf("    num edges:\t");
      for (int k = 0; k < numTreeTypes; k++) {
         printf("%10.3lf\t", exact[t][k]);
      }
      printf("\n\n");
   }
   printf("\n");
   for (int a = 0; a < numAlgorithms; a++) {
      switch(a) {
         case 0:
            printf("RANDOM:\n");
            break;
         case 1:
            printf("FREDRICKSON:\n");
            break;
      }
      for (int t = 0; t < treeSizeLen; t++) {
         printf("  SIZE = %i\t\n", treesize[t]);
         for (int d = 0; d < densityLen; d++) {
            printf("  density = %.2f\n", density[d] / 100.0);
            if (treesize[t] <= 100) {
               printf("    time (ms):\t");
            } else {
               printf("    time (s):\t");
            }
            for (int k = 0; k < 6; k++) {
               if (treesize[t] <= 100) {
                  printf("%10.3lf\t", 1000 * averagedTimes[a][d][t][k] / CLOCKS_PER_SEC);
               } else {
                  printf("%10.3lf\t", averagedTimes[a][d][t][k] / CLOCKS_PER_SEC);
               }
            }
            printf("\n");
            printf("    num edges:\t");
            for (int k = 0; k < 6; k++) {
               printf("%10.3lf\t", averagedSolutions[a][d][t][k]);
            }
            printf("\n");
         }
         printf("\n");
      }
      printf("\n");
   }

   /* free the memory used by averaged times and averaged solutions */
   for (int a = 0; a < numAlgorithms; a++) {
      for (int d = 0; d < densityLen; d++) {
         for (int t = 0; t < treeSizeLen; t++) {
            free(averagedTimes[a][d][t]);
            free(averagedSolutions[a][d][t]);
         }
         free(averagedTimes[a][d]);
         free(averagedSolutions[a][d]);
      }
      free(averagedTimes[a]);
      free(averagedSolutions[a]);
   }
   free(averagedTimes);
   free(averagedSolutions);

/*   commented out for future use */
/*   genPlot(numAlgorithms,treeSizeLen,timeSize, times, treesize); //generate Latex plots*/
}
