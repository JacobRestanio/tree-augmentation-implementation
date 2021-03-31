#include <stdlib.h>
#include <stdio.h>

/* generate a linear tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void linearTree(int n, int tree[][n]){
    for(int i=0; i<(n-1); i++){
        tree[i][i+1]=1;
        tree[i+1][i]=1;
    }
}

/* generate a star tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void starTree(int n, int tree[][n]){
    for(int i=1; i<n; i++){
        tree[0][i]=1;
        tree[i][0]=1;
    }
}

/* generates a star-like tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void starlikeTree(int n, int tree[][n]){
    //generates a starlike tree, vertex 0 is the middle node
    int last;
    int path;
    int branches= 1+(rand() % (n - 2 + 1));
    int branchArr[branches]; //array of last node added to branch for each branch
    for(int i= 1; i<=branches; i++){
        tree[0][i]=1; //creates first node in each branch
        tree[i][0]=1;
        branchArr[i-1]=i;
    }

    for(int i=branches+1; i<n; i++){ //starts at 1 more than the number of branches
        path= (rand() % (branches - 1 + 1)); //selects which branch to place the next node
        last=branchArr[path];//find last node added to this branch
        tree[last][i]=1;
        tree[i][last]=1;
        branchArr[path]=i; //sets this as new last node
    }
}

/* generates a caterpillar tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void caterpillarTree(int n, int tree[][n]) {
   int last;
   int node;
   int length = (n / 3 + (n % 3 != 0)) + (rand() % (n - ((n / 3 + (n % 3 != 0)) + 1)));
   int branches[length];
   
   for(int i = 0; i < length - 1; i++) {
      tree[i][i+1] = 1;
      tree[i+1][i] = 1;
      branches[i] = 1;
   }
   branches[length - 1] = 1;
   
   for(int i = length; i < n; i++) {
      while(branches[node = (rand() % (length))] > 2) {}
      tree[node][i] = 1;
      tree[i][node] = 1;
      branches[node]++;
   }
}

/* generates a lobster tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void lobsterTree(int n, int tree[][n]) {
   int last;
   int node;
   int length = (n / 5 + (n % 5 != 0)) + (rand() % (n - ((n / 5 + (n % 5 != 0)) + 1)));
   int branches[length];
   int branchesPath[n - length];
   
   for(int i = 0; i < length - 1; i++) {
      tree[i][i+1] = 1;
      tree[i+1][i] = 1;
      branches[i] = 1;
   }
   branches[length - 1] = 1;
   
   for(int i = 0; i < n - length; i++) {
      branchesPath[i] = 1;
   }
   
   for(int i = length; i < n; i++) {
      int goodNode = 0;
      while(goodNode == 0) {
         node = (rand() % (i));
         if (node < length) {
            goodNode = branches[node] < 3;
         }
         else if (node >= length) {
            goodNode = branchesPath[node - length] < 2;
         }
      }
      tree[node][i] = 1;
      tree[i][node] = 1;
      if (node < length) {
         branches[node]++;
      }
      else if (node >= length) {
         branchesPath[node - length]++;
      }
   }
}

/* generate a random forest
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void randomForestTree(int n, int tree[][n]) {
   int length = n - 2;
   int prufer[length];
   int nodes = n;
   int degree[nodes];
   
   for (int i = 0; i < length; i++) {
      prufer[i] = 1 + (rand() % n);
   }
   
   for (int i = 0; i < nodes; i ++) {
      degree[i] = 1;
   }
   
   for (int i = 0; i < length; i++) {
      degree[prufer[i] - 1] += 1;
   }
   
   for (int i = 0; i < length; i++) {
      for (int j = 0; j < nodes; j++) {
         if (degree[j] == 1) {
            tree[prufer[i]-1][j] = 1;
            tree[j][prufer[i]-1] = 1;
            degree[prufer[i]-1]--;
            degree[j]--;
            break;
         }
      }
   }
   
   int u = -1;
   int v = -1;
   for (int i = 0; i < nodes; i++) {
      if (degree[i] == 1) {
         if (u == -1) {
         u = i;
         }
         else {
            v = i;
            break;
         }
      }
   }
   tree[u][v] = 1;
   tree[v][u] = 1;
   degree[u]--;
   degree[v]--;
}







