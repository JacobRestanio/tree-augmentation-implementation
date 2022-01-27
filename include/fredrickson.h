#ifndef FREDRICKSON_H
#define FREDRICKSON_H
#include "graph.h"

/* Finds the first vertex that is a leaf (1-edge connected)
   size is the number of nodes in the graph
   tree is a graph that the search is performed on
   RETURNS: an integer corresponding to the node that is a leaf
   NOTE: this can be replaced, the graph structure keeps track of the degree of each node */
int findLeaf(int size, graph* tree);

/* Recursive function to direct a tree toward the root taken to be the node
   size is the number of nodes in the graph
   tree is an undirected graph that contains edges
   directedTree is an directed graph with no edges
   node is the node that is being directed towards
   previous node is needed in recursion, -1 indicates no previous node
   MODIFIES: directedTree
   NOTE: this could be done differently with the new graph structure */
void directTree(int size, graph* tree, directedgraph* directedTree, int node, int previousNode);

/* Sets the weights of the graph that will be used when finding a minimum arborescence
   size is the number of nodes in the graph
   directedTree is a tree with all edges directed toward a root node (see directTree())
   weightedGraph is an size x size matrix
   root is the node the directed tree is rooted at
   MODIFIES: weightedTree */
void setEdgeWeights(int size, directedgraph* directedTree, directedgraph* edgeWeights, int root);

/* Edmonds Algorithm finds a minimum cost arborescence
   size is the number of nodes in the graph
   weightedTree is a directed graph with each edge having a cost
   arborescence is a directed graph with no edges
   root is the root of weightedTree
   MODIFIES: arborescence */
void edmondsAlgorithm(directedgraph* edgeWeights, directedgraph* arborescence, int root);

/* Frederickson algorithm finds a 2-factor approximate solution to the tree augmentation problem
   size is the number of nodes in the graph
   tree the graph to 2-edge connect
   RETURNS: the number of edges to add to make tree 2-edge connected */
int fredrickson(int size, graph* tree);

#endif
