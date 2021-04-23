#include <stdlib.h>
#include <stdio.h>

/* returns 1 if the graph is 2-edge connected, 0 otherwise */


int checkConnected(int n, int tree[][n], int edgeSet[][n]){
	int count=0; //count of edges for vertex i
	for(int i=0; i<n; i++){ //check each vertex, if every vertex has 2 edges the graph is 2-edge connected, otherwise it is not
		count=0;
		for(int j=0; j<n; j++){
			if(tree[i][j]==1 || edgeSet[i][j]==1){
				count++;
				if(count>1){ 
					break;
				}
			}
		}
		if(count<=1){
			return 0;
		}
	}
	return 1;
}
