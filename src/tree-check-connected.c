#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/graph.h"

/* returns 0 if there exists a bridge in the graph, otherwise returns 1 */
int checkBridgeConnected(int n, int v, graph* tree, graph* edges, int arrival[], int visited[], int parent, int* depPtr) {
  /* set the arrival of the current node */
  arrival[v] = ++(*depPtr);
  /* mark the node as visited */
  visited[v] = 1;
  /* keep track of the current arrival of this node to see if it back connects */
  int d = arrival[v];
  int bfs;
  /* we need to check all the edges coming out from our current node */
  for (int j = 0; j < n; j++) {
    if (graph_is_edge(tree, v, j) || graph_is_edge(edges, v, j)) {
      /* if we find an edge to a node that has not been visited, recurse on the node */
      if (visited[j] == 0) {
        bfs = checkBridgeConnected(n, j, tree, edges, arrival, visited, v, depPtr);
        /* if we find a bridge we can return */
        if (bfs == 0) {
          return 0;
        }
        d = (d < bfs) ? d : bfs;
      /* if we find a node that has been visited and it is not the parent
         see if the edge is back connected */
      } else if (j != parent) {
        d = (d < arrival[j]) ? d : arrival[j];
      }
    }
  }
  /* if our current subtree at v is not back connected, there exists a bridge */
  if (d == arrival[v] && parent != -1) {
    return 0;
  }
  return d;
}

/* returns 1 if the graph is 2-edge connected, 0 otherwise */
int checkConnected(int n, graph* tree, graph* edges) {
  /* perform DFS on graph rooted at node 0
	   we need to make sure no bridges exist in our graph */
	int r = 0;
	int dep = 0;
	int* depPtr = &dep;
	int parent =-1;
	int arrival[n];
	int visited[n];
	for (int i = 0; i < n; i++) {
	  arrival[i] = 0;
	  visited[i] = 0;
	}
	int check = checkBridgeConnected(n, r, tree, edges, arrival, visited, parent, depPtr);

	return (check > 0) ? check : 0;
}
