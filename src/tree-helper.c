#include <stdio.h>

/* prints an adjacency matrix
   n is the number of nodes in the tree
   tree is the adjacency matrix of size n x n */
void printTreeAdjMat(int n, int tree[][n]) {
   for(int i=0; i<n; i++) {
      for(int j=0; j<n; j++) {
        printf("%d ",tree[i][j]);
      }
      printf("\n");
   }
}
