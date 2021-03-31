#ifndef TREE_GENERATOR_H
#define TREE_GENERATOR_H

/* generate a linear tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void linearTree(int n, int tree[][n]);

/* generate a star tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void starTree(int n, int tree[][n]);

/* generates a star-like tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void starlikeTree(int n, int tree[][n]);

/* generates a caterpillar tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void caterpillarTree(int n, int tree[][n]);

/* generates a lobster tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void lobsterTree(int n, int tree[][n]);

/* generate a random forest
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
void randomForestTree(int n, int tree[][n]);

#endif
