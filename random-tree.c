#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void printTreeEdges(int prufer[], int m) {
   int vertices = m + 2;
   int vertex_set[vertices];
   
   for (int i = 0; i < vertices; i ++) {
      vertex_set[i] = 0;
   }
   
   for (int i = 0; i < m; i++) {
      vertex_set[prufer[i] - 1] += 1;
   }
   
   printf("\nThe edge set E(G) is:\n{");
   
   for (int i = 0; i < m; i++) {
      for (int j = 0; j < vertices; j++) {
         if (vertex_set[j] == 0) {
            vertex_set[j] = -1;
            printf("(%i, %i), ", j + 1, prufer[i]);
            vertex_set[prufer[i] - 1] -= 1;
            break;
         }
      }
   }
   
   int j = 0;
   for (int i = 0; i < vertices; i++) {
      if (vertex_set[i] == 0 && j == 0) {
         printf("(%i, ", i + 1);
         j++;
      }
      else if (vertex_set[i] == 0 && j == 1) {
         printf("%i)}\n", i + 1);
      }
   }
}

int ran(int l, int r) {
   return l + (rand() % (r - 1 + 1));
}
   

void generateRandomTree(int n) {
   int length = n - 2;
   int arr[length];
   srand(time(0));
   
   for (int i = 0; i < length; i++) {
      arr[i] = ran(1, n);
   }
   
   printTreeEdges(arr, length);
}

int main() {
   int n;
   printf("Enter N number of nodes in tree: ");
   scanf("%i", &n);
   generateRandomTree(n);
   
   return 0;
}
