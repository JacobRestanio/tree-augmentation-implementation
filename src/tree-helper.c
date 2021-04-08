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
int treeCover(int n, int tree[][n], int v,int stor[n][2], int covered, int parent[n]){
   int leaf=1;
   for(int i=0; i<n; i++){
      if(tree[v][i]==1 && i!=parent[v]){
         leaf=0;
      }
   }
   if(leaf==1){                    //node doesn't have any new edge
       return covered;
   }
   else if(stor[v][covered]!=-1){      //already calculated
       return stor[v][covered];
   }
   int sum = 0;
   for(int i=0; i<n; i++){
       int u= tree[v][i];
       if(i!=parent[v]&& i!=v && u==1){             //not a parent
           parent[i]=v;
           if(covered==0){                 //not guarded, must set a watchman
               sum = sum + treeCover(n,tree,i,stor,1,parent);
           }
           else{
               int f1=treeCover(n,tree,i,stor,1,parent);//guarded, check both
               int f2=treeCover(n,tree,i,stor,0,parent);
               if(f1<f2){
                  sum=sum+f1;
               }
               else{
                  sum=sum+f2;
               }
         }
      }
   }
   stor[v][covered] = sum + covered;
    //printf("\n Stor: %d, v: %d, cover: %d \n", stor[v][covered],v,covered);
   return stor[v][covered];
}
void printMinCover(int n, int tree[][n]){
      int parent[n];
      int stor[n][2];
      for(int i=0; i<n; i++){
         stor[i][0]=-1;
         stor[i][1]=-1;
         parent[i]=-1;
      }
      int minCover1=treeCover(n, tree, 0, stor, 1,parent);
      int minCover0=treeCover(n, tree, 0, stor, 0,parent);
      int cover=0;
      if(minCover1<minCover0){
         cover=minCover1;
      }
      else{
         cover=minCover0;
      }
      printf("\n Size of Vertex cover: %d \n", cover);
}

//to find and print minimum vertex cover
void printVertexCover(int n, int tree[][n]) {
   int cover[n];
   for(int i=0; i<n; i++){
      cover[i]=1;
   }
   printf("\n Size of Vertex cover: %d \n", vertexCover(n, n, tree, 0, cover) );
}


