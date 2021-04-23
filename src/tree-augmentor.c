#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include "../include/tree-generator.h"
#include "../include/tree-helper.h"
#include "../include/tree-greedy.h"

/* Temporary Function to some debugging info */
void printInfo(int n, int tree[][n], int edgeSet[][n], int edgeWeights[][n]) {
   printf("TREE\n");
   printTreeAdjMat(n, tree);
   printf("\nEDGE SET\n");
   printTreeAdjMat(n, edgeSet);
   printf("\nEDGE WEIGHTS\n");
   printTreeAdjMatWithPadding(n, edgeWeights, 1);
   printf("___________________________________\n\n");
}

/* Still inconsistent, might look into Valgrind, which is 3rd party 
   The compiler might still be doing some efficiency things */
long getMemUse() {
   struct rusage usage;
   getrusage(RUSAGE_SELF, &usage);
   return usage.ru_maxrss;
}

int main() {
   srand(time(0));
   int treesize[]={10}; //the different sizes of trees tested for. should be able to change this without having to change any other code
   
   int numAlgorithms=1; //number of algorithms we have implemented
   int treeSizeLen=sizeof(treesize)/sizeof(treesize[0]);
   int timeSize=treeSizeLen*numAlgorithms;
   //double times[6][timeSize]; //array of times for each tree type(6 as of now) and each tree size * number of algorithms
	double (*times)[timeSize] =malloc(sizeof(double[6][timeSize]));
   for(int n=0; n<treeSizeLen; n++){
   	for(int treetype=1; treetype<=6; treetype++){
   	   long mem1 = getMemUse();
   	   
   	   /* allocate, initialize, then create a tree of a certain type */
		   int (*tree)[treesize[n]] =malloc(sizeof(int[treesize[n]][treesize[n]]));
		   memset(tree, 0, sizeof(tree[0][0])*treesize[n]*treesize[n]);
		   switch(treetype) {
		      case 1:
		         randomForestTree(treesize[n], tree);
		         break;
		      case 2:
		         linearTree(treesize[n], tree);
		         break;
		      case 3:
		         starTree(treesize[n], tree);
		         break;
		      case 4:
		         starlikeTree(treesize[n], tree);
		         break;
		      case 5:
		         caterpillarTree(treesize[n], tree);
		         break;
		      case 6:
		         lobsterTree(treesize[n], tree);
		         break;
		      default:
		         printf("invalid selection\n");
		         break;
		   }
/*		   commented out for repurposing later    */

/*		   //unweighted min cover using linear tree algorithm*/
/*		   int (*vertexWeights)=malloc(sizeof(int[treesize[n]]));*/
/*		   for(int i=0; i<treesize[n]; i++){ //sets all to 1 for unweighted*/
/*		   	vertexWeights[i]=1;*/
/*		   }*/
/*		   clock_t t1; //begin clock time*/
/*		   t1 = clock();*/
/*		   printMinCover(treesize[n], tree,vertexWeights);*/
/*		   t1 = clock() - t1; // end clock time*/
/*		   double time_taken1 = ((double)t1)/CLOCKS_PER_SEC;*/
/*		   times[treetype-1][numAlgorithms*n]=time_taken1; //sets time taken for treetype, treesize, algorithm 1*/
/*		   printf("It took %f seconds to execute \n", time_taken1);*/
/*		   //weighted min cover using linear tree algorithm*/
/*		   genVertexWeights(treesize[n], vertexWeights, 10); //generate random vertex weights from 1-10*/
/*		   clock_t t2; //begin clock time*/
/*		   t2 = clock();*/
/*		   printMinCover(treesize[n], tree, vertexWeights);*/
/*		   t2 = clock() - t2; // end clock time*/
/*		   double time_taken2 = ((double)t2)/CLOCKS_PER_SEC;*/
/*		   times[treetype-1][numAlgorithms*n+1]=time_taken2; //sets time taken for treetype, treesize, algorithm 2*/
/*		   printf("It took %f seconds to execute \n", time_taken2);*/

         /* allocate, initialize, and create edge set */
         int (*edgeSet)[treesize[n]] = malloc(sizeof(int[treesize[n]][treesize[n]]));
         memset(edgeSet, 0, sizeof(edgeSet[0][0])*treesize[n]*treesize[n]);
         createEdgeSet(treesize[n], tree, edgeSet);
         
         /* allocate, initialize, and generate edge weights */
         int (*edgeWeights)[treesize[n]] = malloc(sizeof(int[treesize[n]][treesize[n]]));
         memset(edgeWeights, 0, sizeof(edgeWeights[0][0])*treesize[n]*treesize[n]);
         genEdgeWeights(treesize[n], edgeSet, edgeWeights, 9);
         
         //call greedy algorithm
         int augment=greedyHeuristic(treesize[n], tree, edgeSet);
         printf("Greedy Algorithm returned %d \n", augment);
         
         long mem2 = getMemUse();
         printf("Memory Usage: %ld\n", mem2 - mem1);
         printInfo(treesize[n], tree, edgeSet, edgeWeights);
         
         /* every call to malloc needs a corresponding call to free */
         free(tree);
         free(edgeSet);
         free(edgeWeights);
		}     
   }
/*   commented out for future use */ 
/*   genPlot(numAlgorithms,treeSizeLen,timeSize, times, treesize); //generate Latex plots*/
}
