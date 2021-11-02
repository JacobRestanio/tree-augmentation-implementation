#ifndef GRAPH_H
#define GRAPH_H

/* the struct edge:
   int v: the label of the destination vertex
   edge* next: a pointer to the next edge */
typedef struct DirectedGraph directedgraph;
typedef struct Graph graph;

directedgraph* directedgraph_create(int v);

int directedgraph_is_successor(directedgraph* g, int source, int destination);

int directedgraph_is_predecessor(directedgraph* g, int destination, int source);

void directedgraph_add_edge(directedgraph* g, int source, int destination);

void directedgraph_remove_edge(directedgraph* g, int source, int destination);

void directedgraph_print(directedgraph* g);

void directedgraph_free(directedgraph* g);

graph* graph_create(int v);

int graph_is_edge(graph* g, int u, int v);

void graph_add_edge(graph* g, int u, int v);

void graph_remove_edge(graph* g, int u, int v);

void graph_print(graph* g);

void graph_free(graph* g);

#endif
