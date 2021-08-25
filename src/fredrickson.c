#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"

/* Recursive function to direct a tree toward the root taken to be the node,
   the first call to this function should set previousNode to -1 */
void directTree(int size, int tree[][size], int node, int previousNode) {
   // check to see if this is the first time the function was called
   // this allows us to know we are at the 'root node'
   // which does not necessarily need to be the 0 node
   if (previousNode == -1) {
      // check to see what nodes are connected to the root node
      // since this is the root, there are no outgoing edges
      for (int j = 0; j < size; j++) {
         if (tree[node][j] == 1) {
            tree[node][j] = 0;
            // recursively call directTree with the root node as the previous node
            directTree(size, tree, j, node);
         }
      }
   // this is not the root case, meaning there was an outgoing edge towards the root
   } else {
      // check to see what incoming edges are connected to this node
      for (int j = 0; j < size; j++) {
         // ignore the edge to the previous node, as
         // it is the outgoing edge towards the root
         if (tree[node][j] == 1 && j != previousNode) {
            tree[node][j] = 0;
            // recurse on the next node that is an incoming
            // edge with this node as the previous node
            directTree(size, tree, j, node);
         }
      }
   }
}

int fredrickson(int size, int tree[][size]) {
   // create complete directed graph
   // set diagonal to 0 and nondiagonal to 1
   int complete_graph[size][size];
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         if (i == j) {
            complete_graph[i][j] = 0;
         } else {
            complete_graph[i][j] = 1;
         }
      }
   }
   
   // create a directed tree from node 0
   int directedTree[size][size];
   copyMatrix(size, tree, directedTree);
   directTree(size, directedTree, 0, -1);
   printf("DIRECTED TREE\n");
   printTreeAdjMat(size, directedTree);
   printf("\n");
   
   return 1;
}
