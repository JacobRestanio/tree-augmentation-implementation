#ifndef FREDRICKSON_H
#define FREDRICKSON_H
#include "graph.h"

int findLeaf(int size, graph* tree);

void directTree(int size, graph* tree, directedgraph* directedTree, int node, int previousNode);

void setWeights(int size, directedgraph* directedTree, directedgraph* weightedTree, int root);

void strongConnect(int size, int i, directedgraph* arborescence, directedgraph* cycles, int stack[size], int onStack[size], int vIndex[size], int vLink[size], int* stackIndex, int* index, int* cycle);

void findCycle(int size, directedgraph* arborescence, directedgraph* cycles);

void edmondsAlgorithm(int size, directedgraph* weightedTree, directedgraph* arborescence, int root);

int fredrickson(int size, graph* tree);

#endif
