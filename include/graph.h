#ifndef GRAPH_H
#define GRAPH_H

/* the struct Edge:
   int v: the label of the destination vertex
   edge* next: a pointer to the next edge */

/* the struct EdgeSet:
   int degree: the degree of the vertex
   edge* next: a pointer to the first edge */

/* the struct DirectedGraph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   edgeset* successors: edges in the graph from source to destination (can be indexed)
   edgeset* predecessors: edges in the graph from destination to source (can be indexed) */
typedef struct DirectedGraph directedgraph;

/* the struct Graph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   edgeset* edgeSet: edges in the graph (can be indexed) */
typedef struct Graph graph;

/* Allocates memory for and initializes a DirectedGraph struct
   int v: the number of vertices in the directed graph
   RETURNS: a directedgraph* with no edges */
directedgraph* directedgraph_create(int v);

/* Checks if an edge exists from source to destination in a directed graph
   directedgraph* g: the directed graph to check for edge
   int source: the source vertex
   int destination: the destination vertex
   RETURNS: returns 1 if the edge exists, 0 if not */
int directedgraph_is_successor(directedgraph* g, int source, int destination);

/* Checks if an edge exists to destination from source in a directed graph
   directedgraph* g: the directed graph to check for edge
   int destination: the destination vertex
   int source: the source vertex
   RETURNS: 1 if the edge exists, 0 if not */
int directedgraph_is_predecessor(directedgraph* g, int destination, int source);

/* Returns the cost of an edge if it exists
   directedgraph* g: the directed graph to get edge cost
   int source: the source vertex
   int destination: the destination vertex
   RETURNS: cost of the edge if it exists, -1 if not
   NOTE: returning -1 isn't optimal, since sometimes we use negative edge costs */
int directedgraph_get_edge_cost(directedgraph* g, int source, int destination);

/* Finds the specified edge and sets its edge cost
   directedgraph* g: the directed graph to get edge cost
   int source: the source vertex
   int destination: the destination vertex
   int cost: the cost to set the edge
   MODIFIES: edge in g to new cost if the edge exists */
void directedgraph_set_edge_cost(directedgraph* g, int source, int destination, int cost);

/* Adds an edge to a directed graph with unit cost
   directedgraph* g: the directed graph to add the edge to
   int source: the source vertex
   int destination: the destination vertex
   MODIFES: g by adding a new edge */
void directedgraph_add_edge(directedgraph* g, int source, int destination);

/* Adds an edge to a directed graph with cost
   directedgraph* g: the directed graph to add the edge to
   int source: the source vertex
   int destination: the destination vertex
   int cost: the cost of the edge
   MODIFIES: g by adding a new edge with associated cost */
void directedgraph_add_weighted_edge(directedgraph* g, int source, int destination, int cost);

/* Removes an edge from a directed graph
   directedgraph* g: the directed graph to remove the edge from
   int source: the source vertex
   int destination: the destination vertex
   MODIFES: g by removing an edge */
void directedgraph_remove_edge(directedgraph* g, int source, int destination);

/* Prints a directed graph to console
   directedgraph* g: the directed graph to be printed
   NOTE: probably don't want to do this for large graphs */
void directedgraph_print(directedgraph* g);

/* Prints a directed graph to console including edge costs
   directedgraph* g: the directed graph to be printed
   NOTE: probably don't want to do this for large graphs */
void directedgraph_print_weights(directedgraph* g);

/* Frees the memory of a directed graph
   directedgraph* g: the directed graph to be freed */
void directedgraph_free(directedgraph* g);

/* Allocates memory for and initializes a Graph struct
   int v: the number of vertices in the graph
   RETURNS: a graph* with no edges */
graph* graph_create(int v);

/* Checks if an edge exists from vertex u to vertex v in a graph
   graph* g: a graph to add the edge to
   int u: a vertex
   int v: another vertex
   RETURNS: 1 if the edge exists, 0 if not */
int graph_is_edge(graph* g, int u, int v);

/* Adds an edge to a graph with unit cost from vertex u to vertex v
   graph* g: the graph to add the edge to
   int u: a vertex
   int v: another vertex
   MODIFIES: g by adding a new edge */
void graph_add_edge(graph* g, int u, int v);

/* Removes an edge from a graph from vertex u to vertex v
   graph* g: the graph to remove the edge from
   int u: a vertex
   int v: a vertex
   MODIFIES: g by removing an edge */
void graph_remove_edge(graph* g, int u, int v);

/* Prints a graph to console
   graph* g: the graph to be printed
   NOTE: probably don't want to do this for large graphs */
void graph_print(graph* g);

/* Frees the memory of a graph
   graph* g: the directed graph to be freed */
void graph_free(graph* g);

#endif
