#ifndef GRAPH_H
#define GRAPH_H

/* the struct Edge:
   int v: the label of the destination vertex
   edge* next: a pointer to the next edge */
typedef struct Edge edge;

/* the struct Vertex:
   int degree: the degree of the vertex
   edge* next: a pointer to the first edge */
typedef struct Vertex vertex;

/* the struct DirectedGraph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   vertex* successors: edges in the graph from source to destination (can be indexed)
   vertex* predecessors: edges in the graph from destination to source (can be indexed) */
typedef struct DirectedGraph directedgraph;

/* the struct Graph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   vertex* edgeSet: edges in the graph (can be indexed) */
typedef struct Graph graph;

/* the struct AdjList:
   int v: the label of the element in the AdjList
   adjlist* next: a pointer to the next element in the list */
typedef struct AdjList adjlist;

edge* edge_get_next(edge* e);

int edge_get_value(edge* e);

int edge_get_cost(edge* e);

void edge_set_cost(edge* e, int cost);

edge* vertex_get_edges(vertex* vs);

vertex* vertex_get_next(vertex* vs);

int vertex_get_value(vertex* vs);

int vertex_get_degree(vertex* vs);

int vertex_get_visited(vertex* vs);

int vertex_get_inpath(vertex* vs);

int vertex_get_minimum_incoming_edge(vertex* vs);

void vertex_set_visited(vertex* vs, int visited);

void vertex_set_inpath(vertex* vs, int inpath);

void vertex_set_minimum_incoming_edge(vertex* vs, int e);

vertex* directedgraph_get_successors(directedgraph* dg);

vertex* directedgraph_get_predecessors(directedgraph* dg);

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

void directedgraph_add_vertex(directedgraph* g, int v);

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

void directedgraph_remove_vertex(directedgraph* g, int v);

void directedgraph_remove_predecessors(directedgraph* g, int destination);

/* Prints a directed graph to console
   directedgraph* g: the directed graph to be printed
   NOTE: probably don't want to do this for large graphs */
void directedgraph_print(directedgraph* g);

void directedgraph_print_predecessors(directedgraph* g);

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

/* Prints a graph to console so that it may be visualized with csacademy's graph visualizer
   graph* g: the graph to be printed
   NOTE: probably don't want to do this for large graphs */
void graph_print_csacademy(graph* g);

/* Creates a graph from csacademy's output format
   char* text: output text with linebreaks */
graph* graph_construct_csacademy(char* text, int vertices);

/* Frees the memory of a graph
   graph* g: the directed graph to be freed */
void graph_free(graph* g);

adjlist* adjlist_create(int v);

adjlist* adjlist_get_next(adjlist* adj);

int adjlist_get_value(adjlist* adj);

void adjlist_set_value(adjlist* adj, int v);

void adjlist_add_element(adjlist* adj, int v);

void adjlist_print(adjlist* adj);

void adjlist_free(adjlist* adj);

adjlist* adjlist_find_cycle_in_directedgraph(directedgraph* dg);

#endif
