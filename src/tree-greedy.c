#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"

int greedyHeuristic(graph* tree, graph* edgeSet){
   /* Check if a 2-edge connected graph is possible */
   int treeSize = graph_get_size(tree);
   if (checkConnected(tree, edgeSet) == 0) {
      printf("impossible to connect tree and edge set\n");
      return 0;
   }

   /* Begin Greedy Algorithm */
   graph* edgeSetCopy = graph_copy(edgeSet);
   graph* selectedEdges = graph_create(treeSize);

   for (vertex* v = graph_get_vertices(tree); v; v = vertex_get_next(v)) {
      /* Randomly add an edge if the degree is less than 2 */
      vertex* selEdgVtx = graph_find_vertex(selectedEdges, vertex_get_value(v));
      if (vertex_get_degree(v) + vertex_get_degree(selEdgVtx) < 2) {
         vertex* escVtx = graph_find_vertex(edgeSetCopy, vertex_get_value(v));
         int chosenEdge = rand() % vertex_get_degree(escVtx);
         edge* escVtxEdges = vertex_get_edges(escVtx);
         for (int i = 0; i < chosenEdge; i++) {
            escVtxEdges = edge_get_next(escVtxEdges);
         }
         graph_insert_edge(selectedEdges, escVtxEdges);
         graph_delete_edge(escVtxEdges);
         if (vertex_get_degree(escVtx) == 0) {
            graph_delete_vertex(escVtx);
         }
      }
   }

   /* Check if the graph is 2-edge connected */
   while (checkConnected(tree, selectedEdges) == 0) {
      /* Randomly add edges until the graph is 2-edge connected */
      int chosenVertex = rand() % graph_get_size(edgeSetCopy);
      vertex* escVtx = graph_get_vertices(edgeSetCopy);
      for (int i = 0; i < chosenVertex; i++) {
         escVtx = vertex_get_next(escVtx);
      }
      if (vertex_get_degree(escVtx) > 0) {
         int chosenEdge = rand () % vertex_get_degree(escVtx);
         edge* escVtxEdges = vertex_get_edges(escVtx);
         for (int i = 0; i < chosenEdge; i++) {
            escVtxEdges = edge_get_next(escVtxEdges);
         }
         graph_insert_edge(selectedEdges, escVtxEdges);
         graph_delete_edge(escVtxEdges);
      } else {
         graph_delete_vertex(escVtx);
      }
   }

   int numEdgesAdded = graph_get_edges(selectedEdges);
   graph_free(selectedEdges);
   graph_free(edgeSetCopy);
   return numEdgesAdded;
}
