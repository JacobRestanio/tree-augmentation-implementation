//A path graph (or linear graph) consists of n vertices arranged in a line, so that vertices i and i+1 are connected by an edge for i=1,...,n−1.
int main() {
   int n;
   printf("Enter N number of nodes in tree: ");
   scanf("%i", &n);
   int path[n][n];
   memset(path, 0, sizeof path); //fills matrix with 0's
   pathGraph(n, path);
   for(int i=0; i<n; i++){ //loop for printing
      for(int j=0; j<n; j++){
        printf("%d ",path[i][j]);
      }
      printf("\n");
   }
   return 0;
}


void pathGraph(int n, int tree[][n]){
    //generates path graph
    for(int i=0; i<(n-1); i++){
        tree[i][i+1]=1;
        tree[i+1][i]=1;
    }
}
