#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/tree-check-connected.h"

/* returns the size of number of edges */
int greedyHeuristic(int n, int tree[][n], int edgeSet[][n]){
  int edges=0; //number of edges
  int (*edgeCopy)[n] = malloc(sizeof(int[n][n]));
  edgeCopy=memcpy(edgeCopy,edgeSet, n*n*sizeof(int));
  for(int i=0; i<n; i++){
  	for(int j=i+1; j<n; j++){
  		//printf("loop started \n");
  		if(edgeCopy[i][j]==1){
  			//printf("edge found:%d %d \n", i, j);
  			edgeCopy[i][j]=0;
  			edgeCopy[j][i]=0;
  			if(checkConnected(n, tree, edgeCopy)==0){ //if removing this edge disconnects the graph, add it back in
  				//printf("bridge found %d %d \n", i, j);
  				edgeCopy[i][j]=1;
  				edgeCopy[j][i]=1;
  				edges++;
  			}
  		}
  	}
  }
  /*temporary printer
  for(int i=0; i<n; i++) {
      for(int j=0; j<n; j++) {
        printf("%d ",edgeCopy[i][j]);
      }
      printf("\n");
   }
   */
  free(edgeCopy);
  return edges;
}

