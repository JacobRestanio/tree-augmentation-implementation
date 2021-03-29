//A lobster tree is a tree in which all vertices are within distance 2 of a central path subgraph.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void lobsterTree(int n) {
   int last;
   int node;
   int tree[n][n];
   memset(tree, 0, sizeof tree);
   srand(time(0));
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
   lobsterTree(n);
   
   return 0;
}
