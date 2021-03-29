//A caterpillar tree is a tree in which all vertices are within distance 1 of a central path subgraph.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void caterpillarTree(int n) {
   int last;
   int node;
   int tree[n][n];
   memset(tree, 0, sizeof tree);
   srand(time(0));
   int length = (n / 3 + (n % 3 != 0)) + (rand() % (n - ((n / 3 + (n % 3 != 0)) + 1)));
   printf("length: %i\n", length);
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
   /*
   for(int i=0; i<n; i++) {
      for(int j=0; j<n; j++) {
        printf("%d ",tree[i][j]);
      }
      printf("\n");
   } */
}

int main() {
   int n;
   printf("Enter N number of nodes in tree: ");
   scanf("%i", &n);
   caterpillarTree(n);
   
   return 0;
}
      
   
   
