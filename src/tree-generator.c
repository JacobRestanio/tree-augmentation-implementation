#include <stdlib.h>
#include <stdio.h>
#include "../include/graph.h"

/* generate a linear tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
graph* linearTree(int n) {
  graph* g = graph_create(n);
  for (int i = 0; i < n - 1; i++) {
    graph_add_edge(g, i, i + 1);
  }
  return g;
}

/* generate a star tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
graph* starTree(int n) {
  graph* g = graph_create(n);
  for (int i = 1; i < n; i++) {
    graph_add_edge(g, 0, i);
  }
  return g;
}

/* generates a star-like tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
graph* starlikeTree(int n) {
  int bfactor = rand() % 101;
  graph* g = graph_create(n);
  for (int i = 1; i < n; i++) {
    if (rand() % 100 < bfactor) {
      graph_add_edge(g, 0, i);
    } else {
      graph_add_edge(g, i - 1, i);
    }
  }
  return g;
}

/* generates a caterpillar tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
graph* caterpillarTree(int n) {
   graph* g = graph_create(n);
   int l = rand() % (n - 2) + 1;
   for (int i = 1; i < n - l; i++) {
      graph_add_edge(g, i - 1, i);
   }
   for (int i = n - l; i < n; i++) {
      graph_add_edge(g, rand() % (n - l), i);
   }
   return g;
}

/* generates a lobster tree
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
graph* lobsterTree(int n) {
   graph* g = graph_create(n);
   int l1 = rand() % (n - 3) + 1;
   int l2 = rand() % ((n - 2) - l1) + 1;
   for (int i = 1; i < n - (l1 + l2); i++) {
      graph_add_edge(g, i - 1, i);
   }
   for (int i = n - (l1 + l2); i < n - l2; i++) {
      graph_add_edge(g, rand() % (n - (l1 + l2)), i);
   }
   for (int i = n - l2; i < n; i++) {
      graph_add_edge(g, rand() % l1 + (n - (l1 + l2)), i);
   }
   return g;
}

/* generate a random forest
   n is the number of nodes
   tree is the adjacency matrix of size n x n */
graph* randomForestTree(int n) {
  graph* g = graph_create(n);
  int prufer[n - 2];
  int degree[n];

  for (int i = 0; i < n - 2; i++) {
    prufer[i] = rand() % n;
  }
  for (int i = 0; i < n; i++) {
    degree[i] = 1;
  }
  for (int i = 0; i < n - 2; i++) {
    degree[prufer[i]]++;
  }

  for (int i = 0; i < n - 2; i++) {
    for (int j = 0; j < n; j++) {
      if (degree[j] == 1) {
        graph_add_edge(g, prufer[i], j);
        degree[prufer[i]]--;
        degree[j]--;
        break;
      }
    }
  }

  int u = -1;
  int v = -1;
  for (int i = 0; i < n; i++) {
    if (degree[i] == 1) {
      if (u == -1) {
        u = i;
      } else {
        v = i;
        break;
      }
    }
  }
  graph_add_edge(g, u, v);
  return g;
}
