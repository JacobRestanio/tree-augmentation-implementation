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
void setEdgeWeights(int n, directedgraph* directedTree, directedgraph* edgeWeights, int root) {
   for (int source = 0; source < n; source++) {
      for (int destination = 0; destination < n; destination++) {
         /* If the source and destination are the same, the edge cost should be INF */
         if (source == destination) {}
         /* If the destination is the root, the edge cost should be INF */
         else if (destination == root) {}
         /* If the edge is in the directedTree, the edge cost should be 0 */
         else if (directedgraph_is_successor(directedTree, source, destination)) {
            directedgraph_add_weighted_edge(edgeWeights, source, destination, 0);
         /* Otherwise, the edge cost should be 1 */
         } else {
            directedgraph_add_weighted_edge(edgeWeights, source, destination, 1);
         }
      }
   }
}

/* Algorithm to find the minimum arborescence */
void edmondsAlgorithm(directedgraph* edgeWeights, directedgraph* arborescence, int root) {
   // we need to keep track of node that the incoming edge originates from
   // for each node j other than the root, find the edge incoming to j of minimum weight
   vertex* currVertex = directedgraph_get_vertex_predecessors_list(edgeWeights);
   edge* currEdge = NULL;
   while (currVertex) {
      currEdge = vertex_get_edges(currVertex);
      int minimumIncomingEdgeWeight = INF;
      while (currEdge) {
         // Remove edges to the root
         if (vertex_get_value(currVertex) == root) {
            directedgraph_remove_edge(edgeWeights, edge_get_value(currEdge), vertex_get_value(currVertex));
         }
         // Remove self edges
         else if (vertex_get_value(currVertex) == edge_get_value(currEdge)) {
            directedgraph_remove_edge(edgeWeights, edge_get_value(currEdge), vertex_get_value(currVertex));
         }
         // Check if current edge is the minimum incoming edge
         else if (edge_get_cost(currEdge) < minimumIncomingEdgeWeight) {
            minimumIncomingEdgeWeight = edge_get_cost(currEdge);
            vertex_set_minimum_incoming_edge(currVertex, currEdge);
         }
         currEdge = edge_get_next(currEdge);
      }
      edge* minimumIncomingEdge = vertex_get_minimum_incoming_edge(currVertex);
      if (minimumIncomingEdge) {
         directedgraph_add_edge(arborescence, edge_get_value(minimumIncomingEdge), vertex_get_value(currVertex));
      }
      currVertex = vertex_get_next(currVertex);
   }

   // check if there are cycles present in the current solution
   adjlist* cycle = adjlist_find_cycle_in_directedgraph(arborescence);
   // findCycle(n, arborescence, cycles);

   printf("Arborescence:\n");
   directedgraph_print(arborescence);

   printf("cycles:\n");
   adjlist_print(cycle);

   // If there is a cycle present, we create a new contracted vertex set
   if (cycle) {
      int contractedVertex = directedgraph_get_greatest_vertex(edgeWeights) + 1;
      directedgraph* contractedEdgeWeights = directedgraph_create_copy(edgeWeights);
      directedgraph_add_vertex(contractedEdgeWeights, contractedVertex);
      adjlist* currCycle = cycle;
      while(currCycle) {
         vertex* currCycleVertex = directedgraph_get_vertex_predecessors(edgeWeights, adjlist_get_value(currCycle));
         directedgraph_remove_vertex(contractedEdgeWeights, vertex_get_value(currCycleVertex));
         edge* currCycleEdge = vertex_get_edges(currCycleVertex);
         while (currCycleEdge) {
            if (adjlist_is_element(cycle, edge_get_value(currCycleEdge))) {}
            else {
               edge* minimumIncomingEdge = vertex_get_minimum_incoming_edge(currCycleVertex);
               if (minimumIncomingEdge) {
                  int cost = edge_get_cost(currCycleEdge) - edge_get_cost(minimumIncomingEdge);
                  directedgraph_add_corresponding_weighted_edge(contractedEdgeWeights, edge_get_value(currCycleEdge), contractedVertex, currCycleEdge, cost);
               }
            }
            currCycleEdge = edge_get_next(currCycleEdge);
         }
         currCycleVertex = directedgraph_get_vertex_successors(edgeWeights, adjlist_get_value(currCycle));
         currCycleEdge = vertex_get_edges(currCycleVertex);
         while (currCycleEdge) {
            if (adjlist_is_element(cycle, edge_get_value(currCycleEdge))) {}
            else {
               int cost = edge_get_cost(currCycleEdge);
               directedgraph_add_corresponding_weighted_edge(contractedEdgeWeights, contractedVertex, edge_get_value(currCycleEdge), currCycleEdge, cost);
            }
            currCycleEdge = edge_get_next(currCycleEdge);
         }
         currCycle = adjlist_get_next(currCycle);
      }
      printf("Contracted Edge Weights:\n");
      directedgraph_print_weights(contractedEdgeWeights);

      directedgraph* contractedArborescence = directedgraph_create(0);
      vertex* currContractedVertex = directedgraph_get_vertex_successors_list(contractedEdgeWeights);
      while (currContractedVertex) {
         directedgraph_add_vertex(contractedArborescence, vertex_get_value(currContractedVertex));
         currContractedVertex = vertex_get_next(currContractedVertex);
      }
      edmondsAlgorithm(contractedEdgeWeights, contractedArborescence, root);
   }
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
   printf("Tree:\n");
   graph_print(tree);

   int root = findLeaf(n, tree);
   directedgraph* directedTree = directedgraph_create(n);
   directTree(n, tree, directedTree, root, -1);

   printf("Directed Tree:\n");
   directedgraph_print(directedTree);

   directedgraph* edgeWeights = directedgraph_create(n);
   setEdgeWeights(n, directedTree, edgeWeights, root);

   printf("Edge Weights:\n");
   directedgraph_print_weights(edgeWeights);

   directedgraph* arborescence = directedgraph_create(n);
   edmondsAlgorithm(edgeWeights, arborescence, root);

   int e = twoEdgeConnect(n, tree, arborescence);

   free(edgeWeights);
   directedgraph_free(directedTree);
   directedgraph_free(arborescence);
   return e;
}
