#ifndef TREE_CHECK_CONNECTED_H
#define TREE_CHECK_CONNECTED_H
#include "graph.h"

/* checks whether there exists a bridge connected component in the graph */
int checkBridgeConnected(int n, int v, graph* tree, graph* edges, int arrival[], int visited[], int parent, int* dep);

/* returns 1 if the graph is 2-edge connected, 0 otherwise */
int checkConnected(int n, graph* tree, graph* edges);

#endif
