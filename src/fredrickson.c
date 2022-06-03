#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"
#include "../include/queue.h"

#define INF 8

/* Finds the first vertex that is a leaf (1-edge connected) */
vertex* findLeaf(graph* tree) {
   // loop through the vertices of the tree and return the first leaf found
   vertex* currVertex = graph_get_vertex_list(tree);
   while (currVertex) {
      if (vertex_get_degree(currVertex) == 1) {
         return currVertex;
      } else {
         currVertex = vertex_get_next(currVertex);
      }
   }
   // this shouldn't be possible since we are working with trees
   return NULL;
}

/* Function that uses a queue to direct edges towards the root */
void directTree(graph* tree, directedgraph* directedTree, vertex* root) {
   queue* que = queue_create();
   queue_enqueue(que, vertex_get_value(root));
   int visited[graph_get_number_of_vertices(tree)];
   for (int i = 0; i < graph_get_number_of_vertices(tree); i++) {
      visited[i] = 0;
   }
   while (!queue_isEmpty(que)) {
      int currVertex = queue_dequeue(que);
      visited[currVertex] = 1;
      edge* currEdge = vertex_get_edges(graph_get_vertex(tree, currVertex));
      while (currEdge) {
         if (!visited[edge_get_value(currEdge)]) {
            directedgraph_add_edge(directedTree, edge_get_value(currEdge), currVertex);
            queue_enqueue(que, edge_get_value(currEdge));
         }
         currEdge = edge_get_next(currEdge);
      }
   }
   queue_free(que);
}

/* Sets the weights of the graph that will be used when finding a minimum arborescence */
void setEdgeWeights(directedgraph* directedTree, directedgraph* edgeWeights, vertex* root) {
   int n = directedgraph_get_number_of_vertices(directedTree);
   for (int source = 0; source < n; source++) {
      for (int destination = 0; destination < n; destination++) {
         /* If the source and destination are the same, the edge cost should be INF */
         if (source == destination) {}
         /* If the destination is the root, the edge cost should be INF */
         else if (destination == vertex_get_value(root)) {}
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
   int greatestVertex = directedgraph_get_greatest_vertex(edgeWeights);
   // define a lookup array that contains the source of each incoming edge in our arborescence
   int source[greatestVertex + 1];
   for (int i = 0; i <= greatestVertex; i++) {
      source[i] = -1;
   }
   while (currVertex) {
      currEdge = vertex_get_edges(currVertex);
      int currVertexValue = vertex_get_value(currVertex);
      int minimumIncomingEdgeWeight = INF;
      while (currEdge) {
         int currEdgeValue = edge_get_value(currEdge);
         int currEdgeCost = edge_get_cost(currEdge);
         // Remove edges to the root
         if (currVertexValue == root) {
            directedgraph_remove_edge(edgeWeights, currEdgeValue, currVertexValue);
         }
         // Remove self edges
         else if (currVertexValue == currEdgeValue) {
            directedgraph_remove_edge(edgeWeights, currEdgeValue, currVertexValue);
         }
         // Check if current edge is the minimum incoming edge
         else if (currEdgeCost < minimumIncomingEdgeWeight) {
            minimumIncomingEdgeWeight = currEdgeCost;
            source[currVertexValue] = currEdgeValue;
         }
         currEdge = edge_get_next(currEdge);
      }
      int minimumIncomingEdge = source[currVertexValue];
      if (minimumIncomingEdge > -1) {
         directedgraph_add_edge(arborescence, minimumIncomingEdge, currVertexValue);
      }
      currVertex = vertex_get_next(currVertex);
   }

   // check if there is a cycle present in the current solution
   adjlist* cycle = adjlist_find_cycle_in_directedgraph(arborescence);

   if (cycle) {
      int contractedVertex = greatestVertex + 1;
      directedgraph_add_vertex(edgeWeights, contractedVertex);
      int minimumIncomingEdgeWeights[greatestVertex + 1];
      int minimumOutgoingEdgeWeights[greatestVertex + 1];
      int incomingCorrespondingEdges[greatestVertex + 1];
      int outgoingCorrespondingEdges[greatestVertex + 1];
      for (int i = 0; i <= greatestVertex; i++) {
         minimumIncomingEdgeWeights[i] = INF;
         minimumOutgoingEdgeWeights[i] = INF;
         incomingCorrespondingEdges[i] = -1;
         outgoingCorrespondingEdges[i] = -1;
      }
      adjlist* currCycle = cycle;
      while(currCycle) {
         int currCycleValue = adjlist_get_value(currCycle);
         // (u,v) is an edge incoming to the cycle
         vertex* currPredecessorVertex = directedgraph_get_vertex_predecessors(edgeWeights, currCycleValue);
         edge* currIncomingEdge = vertex_get_edges(currPredecessorVertex);
         while (currIncomingEdge) {
            int currIncomingEdgeValue = edge_get_value(currIncomingEdge);
            if (!adjlist_is_element(cycle, currIncomingEdgeValue)) {
               int minimumIncomingEdgeValue = source[vertex_get_value(currPredecessorVertex)];
               int cost = edge_get_cost(currIncomingEdge) - minimumIncomingEdgeValue;
               if (cost < minimumIncomingEdgeWeights[currIncomingEdgeValue]) {
                  minimumIncomingEdgeWeights[currIncomingEdgeValue] = cost;
                  incomingCorrespondingEdges[currIncomingEdgeValue] = currCycleValue;
                  directedgraph_add_weighted_edge(edgeWeights, currIncomingEdgeValue, contractedVertex, cost);
               }
            }
            currIncomingEdge = edge_get_next(currIncomingEdge);
         }
         // (u,v) is an edge outgoing from the cycle
         vertex* currSuccessorVertex = directedgraph_get_vertex_successors(edgeWeights, currCycleValue);
         edge* currOutgoingEdge = vertex_get_edges(currSuccessorVertex);
         while (currOutgoingEdge) {
            int currOutgoingEdgeValue = edge_get_value(currOutgoingEdge);
            if (!adjlist_is_element(cycle, currOutgoingEdgeValue)) {
               int cost = edge_get_cost(currOutgoingEdge);
               if (cost < minimumOutgoingEdgeWeights[currOutgoingEdgeValue]) {
                  minimumOutgoingEdgeWeights[currOutgoingEdgeValue] = cost;
                  outgoingCorrespondingEdges[currOutgoingEdgeValue] = currCycleValue;
                  directedgraph_add_weighted_edge(edgeWeights, contractedVertex, currOutgoingEdgeValue, cost);
               }
            }
            currOutgoingEdge = edge_get_next(currOutgoingEdge);
         }
         currCycle = adjlist_get_next(currCycle);
      }

      // remove cycle vertices from our contracted graph
      currCycle = cycle;
      while (currCycle) {
         int currCycleValue = adjlist_get_value(currCycle);
         directedgraph_remove_vertex(edgeWeights, currCycleValue);
         currCycle = adjlist_get_next(currCycle);
      }

      for (int i = 0; i < contractedVertex; i++) {
         if (adjlist_is_element(cycle, i)) {
            directedgraph_remove_vertex(arborescence, i);
         } else {
            directedgraph_remove_edges_from_vertex(arborescence, i);
         }
      }
      directedgraph_add_vertex(arborescence, contractedVertex);

      vertex* currContractedVertex = directedgraph_get_vertex_successors_list(edgeWeights);
      while (currContractedVertex) {
         directedgraph_add_vertex(arborescence, vertex_get_value(currContractedVertex));
         currContractedVertex = vertex_get_next(currContractedVertex);
      }
      edmondsAlgorithm(edgeWeights, arborescence, root);

      // add the cycle edges back in
      currCycle = cycle;
      adjlist* prevCycle = NULL;
      while (currCycle) {
         directedgraph_add_vertex(arborescence, adjlist_get_value(currCycle));
         if (prevCycle) {
            directedgraph_add_edge(arborescence, adjlist_get_value(currCycle), adjlist_get_value(prevCycle));
         }
         prevCycle = currCycle;
         currCycle = adjlist_get_next(currCycle);
      }
      directedgraph_add_edge(arborescence, adjlist_get_value(cycle), adjlist_get_value(prevCycle));

      // remove the edge (pi(v), v) from the cycle and add edge (u, v)
      vertex* cyclePredecessorVertex = directedgraph_get_vertex_predecessors(arborescence, contractedVertex);
      edge* incomingEdgeCycle = vertex_get_edges(cyclePredecessorVertex);
      int incomingEdgeCycleValue = edge_get_value(incomingEdgeCycle);
      int correspondingCycleVertexValue = incomingCorrespondingEdges[incomingEdgeCycleValue];
      int prevIncomingEdgeValue = source[correspondingCycleVertexValue];
      directedgraph_remove_edge(arborescence, prevIncomingEdgeValue, correspondingCycleVertexValue);
      directedgraph_add_edge(arborescence, incomingEdgeCycleValue, correspondingCycleVertexValue);

      // add corresponding outgoing edges from the cycle
      vertex* cycleSuccessorVertex = directedgraph_get_vertex_successors(arborescence, contractedVertex);
      edge* outgoingEdgeCycle = vertex_get_edges(cycleSuccessorVertex);
      while (outgoingEdgeCycle) {
         int outgoingEdgeCycleValue = edge_get_value(outgoingEdgeCycle);
         int correspondingCycleVertexValue = outgoingCorrespondingEdges[outgoingEdgeCycleValue];
         directedgraph_add_edge(arborescence, correspondingCycleVertexValue, outgoingEdgeCycleValue);
         outgoingEdgeCycle = edge_get_next(outgoingEdgeCycle);
      }

      // remove contracted cycle vertex
      directedgraph_remove_vertex(arborescence, contractedVertex);
   }
   adjlist_free(cycle);
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

int fredrickson(graph* tree, graph* edgeSet) {
   int n = graph_get_number_of_vertices(tree);

   vertex* root = findLeaf(tree);
   directedgraph* directedTree = directedgraph_create(n);
   directTree(tree, directedTree, root);

   directedgraph* edgeWeights = directedgraph_create(n);
   setEdgeWeights(directedTree, edgeWeights, root);

   directedgraph* arborescence = directedgraph_create(n);
   edmondsAlgorithm(edgeWeights, arborescence, vertex_get_value(root));

   int e = twoEdgeConnect(n, tree, arborescence);

   directedgraph_free(edgeWeights);
   directedgraph_free(directedTree);
   directedgraph_free(arborescence);
   return e;
}
