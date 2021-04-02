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

int vertexCover(int min, int n, int tree[][n], int r, int cover[n])
{
   //if(cur>=min)
   //   return -1;
   if(r>n){
      return min;
   }
    for(int i=0; i<n; i++){ //checks if this set covers
      for(int j=i+1; j<n; j++){
         if(tree[i][j]==1 && (cover[i]==0&&cover[j]==0)){ //if it doesn't, return n+1
            return n+1;
         }
      }
    }
    int cur=0;
    for(int i=0; i<n; i++){
      if(cover[i]==1){
         cur++;
      }
    }
    if(cur<min){
      min=cur;
    }
   cover[r]=0;
   cur=vertexCover(min, n, tree, r+1, cover);
   if(cur<min){
      min=cur;
   }
   cover[r]=1;
   cur=vertexCover(min, n, tree, r+1, cover);
   if(cur<min){
      min=cur;
   }
   return cur;
}
//to find and print minimum vertex cover
void printVertexCover(int n, int tree[][n]) {
   int cover[n];
   for(int i=0; i<n; i++){
      cover[i]=1;
   }
   printf("\n Size of Vertex cover: %d \n", vertexCover(n, n, tree, 0, cover) );
}
