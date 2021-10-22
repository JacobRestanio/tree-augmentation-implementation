#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"

#define INF 8

/* Finds the first vertex that is a leaf (1-edge connected) */
int findLeaf(int size, int tree[][size]) {
   // we need a variable to count the number of edges in each vertex
   int edges;
   for (int i = 0; i < size; i++) {
      // at each vertex, reset our edges incident to zero
      edges = 0;
      // count the number of edges from vertex i
      for (int j = 0; j < size; j++) {
         if (tree[i][j] > 0) {
            edges++;
         }
      }
      // check if vertex i only has one edge, if so return that vertex
      if (edges == 1) {
         return i;
      }
   }
   // if there are no leaf nodes, return -1
   // this shouldn't be possible since we are working with trees
   return -1;
}

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

/* Sets the weights of the graph that will be used when finding a minimum arborescence */
void setWeights(int size, int directedTree[][size], int weightedTree[][size], int root) {
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         // set the edge weight for edges common to directTree to 0
         if (directedTree[i][j] == 1) {
            weightedTree[i][j] = 0;
         }
         // set edge weight for edges that go to the root to infinity
         if (j == root && i != j) {
            weightedTree[i][j] = INF;
         }
      }
   }
}

void strongConnect(int size, int i, int arborescence[][size], int cycles[][size], int stack[size], int onStack[size], int vIndex[size], int vLink[size], int* stackIndex, int* index, int* cycle) {
   vIndex[i] = *index;
   vLink[i] = *index;
   *index = *index + 1;
   stack[*stackIndex] = i;
   *stackIndex = *stackIndex + 1;
   onStack[i] = 1;
   int w;
   
   for (int j = 0; j < size; j++) {
      if (arborescence[i][j] == 1) {
         if (vIndex[j] == -1) {
            strongConnect(size, j, arborescence, cycles, stack, onStack, vIndex, vLink, stackIndex, index, cycle);
            vLink[i] = (vLink[i] <= vLink[j]) ? vLink[i] : vLink[j];
         } else if (onStack[j] == 1) {
            vLink[i] = (vLink[i] <= vIndex[j]) ? vLink[i] : vIndex[j];
         }
      }
   }
   
   if (vLink[i] == vIndex[i]) {
      do {
         *stackIndex = *stackIndex - 1;
         w = stack[*stackIndex];
         stack[*stackIndex] = 0;
         onStack[w] = 0;
         cycles[*cycle][w] = 1;
      } while (w != i);
      *cycle = *cycle + 1;
   }
}

void findCycle(int size, int arborescence[][size], int cycles[][size]) {
   int stack[size];
   int onStack[size];
   int vIndex[size];
   int vLink[size];
   for (int i = 0; i < size; i++) {
      stack[i] = 0;
      onStack[i] = 0;
      vIndex[i] = -1;
      vLink[i] = 0;
   }
   int stackIndex = 0;
   int index = 0;
   int cycle = 0;
   
   for (int i = 0; i < size; i++) {
      if (vIndex[i] == -1) {
         strongConnect(size, i, arborescence, cycles, stack, onStack, vIndex, vLink, &stackIndex, &index, &cycle);
      }
   }
}

/* Algorithm to find the minimum arborescence */
void edmondsAlgorithm(int size, int weightedTree[][size], int arborescence[][size], int root) {
   // we need to keep track of node that the incoming edge originates from
   int edgeSource[size];
   for (int i = 0; i < size; i++) {
      edgeSource[i] = -1;
   }
   // for each node j other than the root, find the edge incoming to j of minimum weight
   for (int j = 0; j < size; j++) {
      if (j != root) {
         int minimumIncomingEdgeWeight = INF;
         int minimumIncomingEdge = -1;
         for (int i = 0; i < size; i++) {
            if (i != j && weightedTree[i][j] < minimumIncomingEdgeWeight) {
               minimumIncomingEdgeWeight = weightedTree[i][j];
               minimumIncomingEdge = i;
            }
         }
         arborescence[minimumIncomingEdge][j] = 1;
         edgeSource[j] = minimumIncomingEdge;
      }
   }
   printf("ARBORESCENCE\n");
   printTreeAdjMat(size, arborescence);
   printf("\n");
   
   // check if there are cycles present in the current solution
   int cycles[size][size];
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         cycles[i][j] = 0;
      }
   }
   findCycle(size, arborescence, cycles);
   printf("STRONGLY CONNECTED COMPONENTS\n");
   printTreeAdjMat(size, cycles);
   printf("\n");
   
   // find cycle by finding first strongly connected component of size greater than one
   int numscc = 0;
   int cycle = -1;
   int cycleNodes[size];
   for (int i = 0; i < size; i++) {
      cycleNodes[i] = 0;
   }
   for (int i = 0; i < size; i++) {
      numscc = 0;
      for (int j = 0; j < size; j++) {
         if (cycles[i][j] == 1) {
            numscc++;
         }
      }
      if (numscc > 1) {
         cycle = i;
         break;
      }
   }
   if (cycle >= 0) {
      // contract the cycle into one node
      int conSize = size - numscc + 1;
      int count = 0;
      for (int j = 0; j < size; j++) {
         if (cycles[cycle][j] == 1) {
            cycleNodes[j] = conSize - 1;
         } else {
            cycleNodes[j] = count;
            count++;
         }
      }
      root = cycleNodes[root];
      int contractedWeightedGraph[conSize][conSize];
      int contractTrace[conSize][conSize];
      for (int i = 0; i < conSize; i++) {
         for (int j = 0; j < conSize; j++) {
            contractedWeightedGraph[i][j] = INF;
            contractTrace[i][j] = -1;
         }
      }
      for (int i = 0; i < size; i++) {
         for (int j = 0; j < size; j++) {
            if(i != j && weightedTree[i][j] < INF) {
               int icycle = 0;
               int jcycle = 0;
               if (cycleNodes[i] == conSize - 1) {
                  icycle = 1;
               }
               if (cycleNodes[j] == conSize - 1) {
                  jcycle = 1;
               }
               // check if (i,j) is an edge coming into the cycle
               if (icycle == 0 && jcycle == 1) {
                  int newEdgeWeight = weightedTree[i][j] - weightedTree[edgeSource[j]][j];
                  if (newEdgeWeight < contractedWeightedGraph[cycleNodes[i]][cycleNodes[j]]) {
                     contractedWeightedGraph[cycleNodes[i]][cycleNodes[j]] = newEdgeWeight;
                     contractTrace[cycleNodes[i]][cycleNodes[j]] = i;
                     contractTrace[cycleNodes[j]][cycleNodes[i]] = j;
                  } 
               // check if (i,j) is an edge going out of the cycle
               } else if (icycle == 1 && jcycle == 0) {
                  if (weightedTree[i][j] < contractedWeightedGraph[cycleNodes[i]][cycleNodes[j]]) {
                     contractedWeightedGraph[cycleNodes[i]][cycleNodes[j]] = weightedTree[i][j];
                     contractTrace[cycleNodes[i]][cycleNodes[j]] = i;
                     contractTrace[cycleNodes[j]][cycleNodes[i]] = j;
                  }
               // check if (i,j) is an edge unrelated to the cycle
               } else if (icycle == 0 && jcycle == 0) {
                  contractedWeightedGraph[cycleNodes[i]][cycleNodes[j]] = weightedTree[i][j];
                  contractTrace[cycleNodes[i]][cycleNodes[j]] = i;
                  contractTrace[cycleNodes[j]][cycleNodes[i]] = j;
               }
            }
         }
      }
      int contractedArborescence[conSize][conSize];
      for (int i = 0; i < conSize; i++) {
         for (int j = 0; j < conSize; j++) {
            contractedArborescence[i][j] = 0;
         }
      }
      
      printf("CONTRACTED WEIGHTED GRAPH\n");
      printTreeAdjMat(conSize, contractedWeightedGraph);
      printf("\n");
      
      printf("CONTRACT TRACE\n");
      printTreeAdjMat(conSize, contractTrace);
      printf("\n");
      
      edmondsAlgorithm(conSize, contractedWeightedGraph, contractedArborescence, root);
      // remove edges from the original arborescence that are not in the cycle
      for (int j = 0; j < size; j++) {
         if (cycleNodes[j] != conSize - 1) {
            for (int i = 0; i < size; i++) {
               arborescence[i][j] = 0;
            }
         }
      }
      int originVertex;
      int destinationVertex;
      for (int i = 0; i < conSize; i++) {
         for (int j = 0; j < conSize; j++) {
            // find the new incoming edge to our contracted vertex
            if (j == conSize - 1 && contractedArborescence[i][j] == 1) {
               originVertex = contractTrace[i][j];
               destinationVertex = contractTrace[j][i];
               // add that edge to the original arborescence and remove the previous edge
               arborescence[originVertex][destinationVertex] = 1;
               arborescence[edgeSource[destinationVertex]][destinationVertex] = 0;
            // add the edges from the contracted arborescence to the original arborescence
            } else if (contractedArborescence[i][j] == 1) {
               arborescence[contractTrace[i][j]][contractTrace[j][i]] = 1;
            }
         }
      }
      
      printf("EXPANDED ARBORESCENCE\n");
      printTreeAdjMat(size, arborescence);
      printf("\n");
   }
}

void twoEdgeConnect(int size, int tree[][size], int arborescence[][size]) {
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         if (arborescence[i][j] == 1) {
            tree[i][j] = 1;
            tree[j][i] = 1;
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
   
   // create a directed tree from leaf node
   int root = findLeaf(size, tree);
   int directedTree[size][size];
   copyMatrix(size, tree, directedTree);
   directTree(size, directedTree, root, -1);
   printf("DIRECTED TREE\n");
   printTreeAdjMat(size, directedTree);
   printf("\n");
   
   int weightedTree[size][size];
   createCompleteDirectedTree(size, weightedTree);
   
   setWeights(size, directedTree, weightedTree, root);
   printf("WEIGHTED GRAPH\n");
   printTreeAdjMat(size, weightedTree);
   printf("\n");
   
   int arborescence[size][size];
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         arborescence[i][j] = 0;
      }
   }
   edmondsAlgorithm(size, weightedTree, arborescence, root);
   twoEdgeConnect(size, tree, arborescence);
   
   return 1;
}
