#ifndef TREE_HELPER_H
#define TREE_HELPER_H

/* prints an adjacency matrix
   n is the number of nodes in the tree
   tree is the adjacency matrix of size n x n */
void printTreeAdjMat(int n, int tree[][n]);
void printVertexCover(int n, int tree[][n], int vertex[n]);
void printMinCover(int n, int tree[][n], int vertex[n]);
int treeCover(int n, int tree[][n], int v,int stor[n][2], int covered, int vertex[n]);
void genVertexWeights(int n, int vertex[n], int max);
#endif
