#ifndef FREDRICKSON_H
#define FREDRICKSON_H

int findLeaf(int size, int tree[][size]);

void directTree(int size, int tree[][size], int node, int previousNode);

void setWeights(int size, int directedTree[][size], int weightedTree[][size], int root);

void strongConnect(int size, int i, int arborescence[][size], int cycles[][size], int stack[size], int onStack[size], int vIndex[size], int vLink[size], int* stackIndex, int* index, int* cycle);

void findCycle(int size, int arborescence[][size], int cycles[][size]);

void edmondsAlgorithm(int size, int weightedTree[][size], int arborescence[][size], int root);

int fredrickson(int size, int tree[][size]);

#endif
