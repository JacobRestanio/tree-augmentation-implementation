#ifndef TREE_CHECK_CONNECTED_H
#define TREE_CHECK_CONNECTED_H

/* checks whether there exists a bridge connected component in the graph */
int checkBridgeConnected(int n, int v, int tree[][n], int edges[][n], int arrival[], int visited[], int parent, int* dep);

/* returns 1 if the graph is 2-edge connected, 0 otherwise */
int checkConnected(int n, int tree[][n], int edges[][n]);

#endif
