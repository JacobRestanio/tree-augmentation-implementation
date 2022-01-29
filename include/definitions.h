#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "graph.h"


/* the struct Edge:
   int v: the label of the destination vertex
   edge* next: a pointer to the next edge */
typedef struct Edge {
  int value;
  int cost;
  struct Edge* next;
} edge;

/* the struct Vertex:
   int degree: the degree of the vertex
   edge* next: a pointer to the first edge */
typedef struct Vertex {
  int value;
  int degree;
  edge* nextEdge;
  struct Vertex* nextVertex;
} vertex;

/* the struct Graph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   vertex* edgeSet: edges in the graph (can be indexed) */
typedef struct Graph {
  int vertices;
  int edges;
  vertex* root;
  vertex* vertexSet;
} graph;

/* the struct AdjList:
   int v: the label of the element in the AdjList
   adjlist* next: a pointer to the next element in the list */
typedef struct AdjList {
   int value;
   struct AdjList* next;
} adjlist;



int generatePath(graph* tree);

int generatePath(graph* tree, vertex* currentVertex, vertex* prevVertex);

int setRoot(graph* tree, vertex* v);

int isLeafClosed(graph* tree, vertex* v);

vertex* leaves(graph* tree, vertex* v);





#endif