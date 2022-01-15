#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"

#define INF 8

/* Finds the first vertex that is a leaf (1-edge connected) */
int findLeaf(int n, graph* tree) {
   // we need a variable to count the number of edges in each vertex
   int edges;
   for (int i = 0; i < n; i++) {
      // at each vertex, reset our edges incident to zero
      edges = 0;
      // count the number of edges from vertex i
      for (int j = 0; j < n; j++) {
         if (graph_is_edge(tree, i, j)) {
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
void directTree(int n, graph* tree, directedgraph* directedTree, int node, int previousNode) {
   // check to see if this is the first time the function was called
   // this allows us to know we are at the 'root node'
   // which does not necessarily need to be the 0 node
   if (previousNode == -1) {
      // check to see what nodes are connected to the root node
      // since this is the root, there are no outgoing edges
      for (int j = 0; j < n; j++) {
         if (graph_is_edge(tree, node, j)) {
            directedgraph_add_edge(directedTree, j, node);
            // recursively call directTree with the root node as the previous node
            directTree(n, tree, directedTree, j, node);
         }
      }
   // this is not the root case, meaning there was an outgoing edge towards the root
   } else {
      // check to see what incoming edges are connected to this node
      for (int j = 0; j < n; j++) {
         // ignore the edge to the previous node, as
         // it is the outgoing edge towards the root
         if (graph_is_edge(tree, node, j) && j != previousNode) {
            directedgraph_add_edge(directedTree, j, node);
            // recurse on the next node that is an incoming
            // edge with this node as the previous node
            directTree(n, tree, directedTree, j, node);
         }
      }
   }
}

/* Sets the weights of the graph that will be used when finding a minimum arborescence */
void setWeights(int n, directedgraph* directedTree, directedgraph* weightedTree, int root) {
   for (int source = 0; source < n; source++) {
      for (int destination = 0; destination < n; destination++) {
         /* Don't add an edge from source to destination if they are the same */
         if (source == destination) {
         /* If the destination is the root, the edge cost should be INF */
         } else if (destination == root) {
            directedgraph_add_weighted_edge(weightedTree, source, destination, INF);
         /* If the edge is in the directedTree, the edge cost should be 0 */
         } else if (directedgraph_is_successor(directedTree, source, destination)) {
            directedgraph_add_weighted_edge(weightedTree, source, destination, 0);
         /* Otherwise, the edge cost should be 1 */
         } else {
            directedgraph_add_weighted_edge(weightedTree, source, destination, 1);
         }
      }
   }
}

void strongConnect(int n, int i, directedgraph* arborescence, directedgraph* cycles, int stack[n], int onStack[n], int vIndex[n], int vLink[n], int* stackIndex, int* index, int* cycle) {
   vIndex[i] = *index;
   vLink[i] = *index;
   *index = *index + 1;
   stack[*stackIndex] = i;
   *stackIndex = *stackIndex + 1;
   onStack[i] = 1;
   int w;

   for (int j = 0; j < n; j++) {
      if (directedgraph_is_successor(arborescence, i, j)) {
         if (vIndex[j] == -1) {
            strongConnect(n, j, arborescence, cycles, stack, onStack, vIndex, vLink, stackIndex, index, cycle);
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
         directedgraph_add_edge(cycles, *cycle, w);
      } while (w != i);
      *cycle = *cycle + 1;
   }
}

void findCycle(int n, directedgraph* arborescence, directedgraph* cycles) {
   int stack[n];
   int onStack[n];
   int vIndex[n];
   int vLink[n];
   for (int i = 0; i < n; i++) {
      stack[i] = 0;
      onStack[i] = 0;
      vIndex[i] = -1;
      vLink[i] = 0;
   }
   int stackIndex = 0;
   int index = 0;
   int cycle = 0;

   for (int i = 0; i < n; i++) {
      if (vIndex[i] == -1) {
         strongConnect(n, i, arborescence, cycles, stack, onStack, vIndex, vLink, &stackIndex, &index, &cycle);
      }
   }
}

/* Algorithm to find the minimum arborescence */
void edmondsAlgorithm(int n, directedgraph* weightedTree, directedgraph* arborescence, int root) {
   // we need to keep track of node that the incoming edge originates from
   int edgeSource[n];
   for (int i = 0; i < n; i++) {
      edgeSource[i] = -1;
   }
   // for each node j other than the root, find the edge incoming to j of minimum weight
   for (int j = 0; j < n; j++) {
      if (j != root) {
         int minimumIncomingEdgeWeight = INF;
         int minimumIncomingEdge = -1;
         for (int i = 0; i < n; i++) {
            if (i != j && directedgraph_get_edge_cost(weightedTree, i, j) < minimumIncomingEdgeWeight) {
               minimumIncomingEdgeWeight = directedgraph_get_edge_cost(weightedTree, i, j);
               minimumIncomingEdge = i;
            }
         }
         directedgraph_add_edge(arborescence, minimumIncomingEdge, j);
         edgeSource[j] = minimumIncomingEdge;
      }
   }

   // check if there are cycles present in the current solution
   directedgraph* cycles = directedgraph_create(n);
   findCycle(n, arborescence, cycles);

   // find cycle by finding first strongly connected component of n greater than one
   int numscc = 0;
   int cycle = -1;
   int cycleNodes[n];
   for (int i = 0; i < n; i++) {
      cycleNodes[i] = 0;
   }
   for (int i = 0; i < n; i++) {
      numscc = 0;
      for (int j = 0; j < n; j++) {
         if (directedgraph_is_successor(cycles, i, j)) {
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
      int conSize = n - numscc + 1;
      int count = 0;
      for (int j = 0; j < n; j++) {
         if (directedgraph_is_successor(cycles, cycle, j)) {
            cycleNodes[j] = conSize - 1;
         } else {
            cycleNodes[j] = count;
            count++;
         }
      }
      root = cycleNodes[root];
      directedgraph* contractedWeightedGraph = directedgraph_create(conSize);
      directedgraph* contractTrace = directedgraph_create(conSize);
      for (int i = 0; i < conSize; i++) {
         for (int j = 0; j < conSize; j++) {
            directedgraph_add_weighted_edge(contractedWeightedGraph, i, j, INF);
            directedgraph_add_weighted_edge(contractTrace, i, j, -1);
         }
      }
      for (int i = 0; i < n; i++) {
         for (int j = 0; j < n; j++) {
            if(i != j && directedgraph_get_edge_cost(weightedTree, i, j) < INF) {
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
                  int newEdgeWeight = directedgraph_get_edge_cost(weightedTree, i, j) - directedgraph_get_edge_cost(weightedTree, edgeSource[j], j);
                  if (newEdgeWeight < directedgraph_get_edge_cost(contractedWeightedGraph, cycleNodes[i], cycleNodes[j])) {
                     directedgraph_set_edge_cost(contractedWeightedGraph, cycleNodes[i], cycleNodes[j], newEdgeWeight);
                     directedgraph_set_edge_cost(contractTrace, cycleNodes[i], cycleNodes[j], i);
                     directedgraph_set_edge_cost(contractTrace, cycleNodes[j], cycleNodes[i], j);
                  }
               // check if (i,j) is an edge going out of the cycle
               } else if (icycle == 1 && jcycle == 0) {
                  if (directedgraph_get_edge_cost(weightedTree, i, j) < directedgraph_get_edge_cost(contractedWeightedGraph, cycleNodes[i], cycleNodes[j])) {
                     directedgraph_set_edge_cost(contractedWeightedGraph, cycleNodes[i], cycleNodes[j], directedgraph_get_edge_cost(weightedTree, i, j));
                     directedgraph_set_edge_cost(contractTrace, cycleNodes[i], cycleNodes[j], i);
                     directedgraph_set_edge_cost(contractTrace, cycleNodes[j], cycleNodes[i], j);
                  }
               // check if (i,j) is an edge unrelated to the cycle
               } else if (icycle == 0 && jcycle == 0) {
                  directedgraph_set_edge_cost(contractedWeightedGraph, cycleNodes[i], cycleNodes[j], directedgraph_get_edge_cost(weightedTree, i, j));
                  directedgraph_set_edge_cost(contractTrace, cycleNodes[i], cycleNodes[j], i);
                  directedgraph_set_edge_cost(contractTrace, cycleNodes[j], cycleNodes[i], j);
               }
            }
         }
      }

      directedgraph* contractedArborescence = directedgraph_create(conSize);

      edmondsAlgorithm(conSize, contractedWeightedGraph, contractedArborescence, root);

      // remove edges from the original arborescence that are not in the cycle
      for (int j = 0; j < n; j++) {
         if (cycleNodes[j] != conSize - 1) {
            for (int i = 0; i < n; i++) {
               directedgraph_remove_edge(arborescence, i, j);
            }
         }
      }
      int originVertex;
      int destinationVertex;
      for (int i = 0; i < conSize; i++) {
         for (int j = 0; j < conSize; j++) {
            // find the new incoming edge to our contracted vertex
            if (j == conSize - 1 && directedgraph_is_successor(contractedArborescence, i, j)) {
               originVertex = directedgraph_get_edge_cost(contractTrace, i, j);
               destinationVertex = directedgraph_get_edge_cost(contractTrace, j, i);
               // add that edge to the original arborescence and remove the previous edge
               directedgraph_add_edge(arborescence, originVertex, destinationVertex);
               directedgraph_remove_edge(arborescence, edgeSource[destinationVertex], destinationVertex);
            // add the edges from the contracted arborescence to the original arborescence
         } else if (directedgraph_is_successor(contractedArborescence, i, j)) {
               directedgraph_add_edge(arborescence, directedgraph_get_edge_cost(contractTrace, i, j), directedgraph_get_edge_cost(contractTrace, j, i));
            }
         }
      }

      directedgraph_free(contractedWeightedGraph);
      directedgraph_free(contractTrace);
      directedgraph_free(contractedArborescence);
   }
   directedgraph_free(cycles);
}

int twoEdgeConnect(int n, graph* tree, directedgraph* arborescence) {
   int e = 0;
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         if (directedgraph_is_successor(arborescence, i, j) && !graph_is_edge(tree, i, j)) {
            e++;
         }
      }
   }
   return e;
}

int fredrickson(int n, graph* tree) {
   // create a directed tree from leaf node
   int root = findLeaf(n, tree);
   directedgraph* directedTree = directedgraph_create(n);
   directTree(n, tree, directedTree, root, -1);

   printf("Directed Tree:\n");
   directedgraph_print(directedTree);

   directedgraph* weightedTree = directedgraph_create(n);
   setWeights(n, directedTree, weightedTree, root);

   printf("Weighted Graph:\n");
   directedgraph_print_weights(weightedTree);

   directedgraph* arborescence = directedgraph_create(n);
   edmondsAlgorithm(n, weightedTree, arborescence, root);

   int e = twoEdgeConnect(n, tree, arborescence);

   directedgraph_free(directedTree);
   directedgraph_free(weightedTree);
   directedgraph_free(arborescence);
   return e;
}
