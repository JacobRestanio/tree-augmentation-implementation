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

/* the struct Graph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   vertex* edgeSet: edges in the graph (can be indexed) */
typedef struct Graph graph;

/* the struct DirectedGraph:
   int vertices: the number of vertices in the graph
   int edges: the number of edges in the graph
   vertex* successors: edges in the graph from source to destination (can be indexed)
   vertex* predecessors: edges in the graph from destination to source (can be indexed) */
typedef struct DirectedGraph directedgraph;

/* the struct AdjList:
   int v: the label of the element in the AdjList
   adjlist* next: a pointer to the next element in the list */
typedef struct AdjList adjlist;

/*-----------------EDGE-----------------*/

edge* edge_create(vertex* u, vertex* v);

int edge_match(void* es1, void* es2);

edge* edge_get_next(edge* e);

edge* edge_next(edge* e);

void edge_set_next(edge* e, edge* next);

edge* edge_get_prev(edge* e);

edge* edge_prev(edge* e);

void edge_set_prev(edge* e, edge* prev);

vertex* edge_get_source(edge* e);

void edge_set_source(edge* e, vertex* v);

vertex* edge_get_destination(edge* e);

int edge_destination_value(edge* e);

void edge_set_destination(edge* e, vertex* u);

int edge_get_cost(edge* e);

void edge_set_cost(edge* e, int cost);

edge* edge_get_twin(edge* e);

void edge_set_twin(edge* e, edge* twin);

void edge_free(void* e);

/*-----------------VERTEX-----------------*/

vertex* vertex_create(int v);

vertex* vertex_get_next(vertex* vs);

vertex* vertex_next(vertex* vs);

void vertex_set_next(vertex* vs, vertex* next);

vertex* vertex_get_prev(vertex* vs);

vertex* vertex_prev(vertex* vs);

void vertex_set_prev(vertex* vs, vertex* prev);

int vertex_get_value(vertex* vs);

void vertex_set_value(vertex* vs, int value);

int vertex_get_degree(vertex* vs);

void vertex_set_degree(vertex* vs, int degree);

int vertex_get_visited(vertex* vs);

void vertex_set_visited(vertex* vs, int visited);

int vertex_get_inpath(vertex* vs);

void vertex_set_inpath(vertex* vs, int inpath);

edge* vertex_get_edges(vertex* vs);

void vertex_set_edges(vertex* vs, edge* es);

void vertex_free(void* vs);

/*-----------------GRAPH-----------------*/

/* Allocates memory for and initializes a graph
   int v: the number of vertices in the graph
      v can be < 1, which creates an empty graph
   RETURNS: a graph* of size v with no edges */
graph* graph_create(int v);

/* Returns the vertex specificied for a given graph
   graph* g: a graph
   int v: the integer label of the vertex
   RETURNS: a vertex* corresponding to v or NULL if none exists */
vertex* graph_find_vertex(graph* g, int v);

/* Returns the specified edge for a given graph
   graph* g: a graph
   int u: the source vertex
   int v: the destination vertex
   RETURNS: an edge* corresponding to uv of NULL if none exists */
edge* graph_find_edge(graph* g, int u, int v);

int graph_is_edge(graph* g, edge* e);

/* Adds a vertex to a graph of value vs if it does not already exist
   graph* g: the graph to add the vertex to
   int vs: the value of the vertex
   MODIFES: g by adding a new vertex */
void graph_add_vertex(graph* g, int vs);

/* Adds a vertex to a graph of value vs if it does not already exist
   graph* g: the graph to add the vertex to
   vertex* vs: the vertex whose value is used
   MODIFIES: g by adding a new vertex */
void graph_insert_vertex(graph* g, vertex* vs);

/* Adds an edge to a graph with unit cost from vertex u to vertex v
   graph* g: the graph to add the edge to
   int u: a vertex
   int v: another vertex
   MODIFIES: g by adding a new edge */
void graph_add_edge(graph* g, int u, int v);

/* Adds an edge to a graph with unit cost between the vertices of a given edge
   graph* g: the graph to add the edge to
   edge* e: the edge that contains the vertices to use
   MODIFIES: g by adding a new edge */
void graph_insert_edge(graph* g, edge* e);

/* Makes a copy of a given graph that contains the same vertices and edges
   graph* g: the graph to be copied
   RETURNS: a graph* that is identical to g */
graph* graph_copy(graph* g);

/* Adds a list of vertices to a graph
   graph* g: the graph to add the vertices to
   vertex* vs: the list of vertices to be added
   RETURNS: a graph with the vertices in the list of vertices vs */
void graph_add_vertices(graph* g, vertex* vs);

/* Removes a vertex from a graph
   graph* g: the graph to remove the vertex from
   int vs: the vertex to be removed
   MODIFIES: g by removing a vertex */
void graph_remove_vertex(graph* g, int vs);

/* Deletes the given vertex from its graph
   vertex* v: the vertex to be deleted
   MODIFIES: the graph containing the vertex by removing it */
void graph_delete_vertex(vertex* v);

/* Removes an edge (and its twin if it exists) from a graph
   graph* g: the graph to remove the edge from
   int u: the source vertex
   int v: the destination vertex
   MODIFIES: g by removing an edge and its twin (if it exists) */
void graph_remove_edge(graph* g, int u, int v);

/* Deletes the given edge from its graph, useful if you have the edge
      that way you don't need to search for it in the graph again
   edge* e: the edge to be deleted
   MODIFIES: the graph containing the edge by removing it and its twin (if it exists) */
void graph_delete_edge(edge* e);

/* Find the value of the largest vertex in the graph,
      useful if you want to use an array to index vertices
   graph* g: the graph to search
   RETURNS: and int that is the largest value vertex in the graph */
int graph_find_largest_vertex(graph* g);

/* Prints a graph to console
   graph* g: the graph to be printed
   NOTE: probably don't want to do this for large graphs */
void graph_print(graph* g);

/* Prints a graph to file
   graph* g: the graph to be printed
   char* filename: the name of the file without extension */
void graph_print_to_file(graph* g, char* filename);

/* Reads a graph from file
   char* filename: the name of the file without extension */
graph* graph_read_from_file(char* filename);

/* Returns the list of vertices in the graph
   graph* g: a graph
   RETURNS: a vertex* that is the first vertex in the list */
vertex* graph_get_vertices(graph* g);

void graph_set_vertices(graph* g, vertex* vs);

int graph_get_size(graph* g);

void graph_set_size(graph* g, int size);

int graph_get_edges(graph* g);

void graph_set_edges(graph* g, int edges);

/* Frees the memory of a graph
   void* g: the graph to be freed */
void graph_free(void* grph);

/*-----------------DIRECTED GRAPH-----------------*/

vertex* directedgraph_find_successor_vertex(directedgraph* dg, int v);

vertex* directedgraph_find_predecessor_vertex(directedgraph* dg, int v);

vertex* directedgraph_get_successors(directedgraph* dg);

vertex* directedgraph_get_predecessors(directedgraph* dg);

/* Allocates memory for and initializes a DirectedGraph struct
   int v: the number of vertices in the directed graph
   RETURNS: a directedgraph* with no edges */
directedgraph* directedgraph_create(int v);

void directedgraph_add_vertices(directedgraph* dg, vertex* vs);

int directedgraph_is_edge(directedgraph* dg, int source, int destination);

/* Checks if an edge exists from source to destination in a directed graph
   directedgraph* dg: the directed graph to check for edge
   int source: the source vertex
   int destination: the destination vertex
   RETURNS: returns 1 if the edge exists, 0 if not */
int directedgraph_is_successor(directedgraph* dg, int source, int destination);

/* Checks if an edge exists to destination from source in a directed graph
   directedgraph* dg: the directed graph to check for edge
   int source: the source vertex
   int destination: the destination vertex
   RETURNS: 1 if the edge exists, 0 if not */
int directedgraph_is_predecessor(directedgraph* dg, int source, int destination);

edge* directedgraph_get_edge(directedgraph* dg, int source, int destination);

/* Returns the cost of an edge if it exists
   directedgraph* dg: the directed graph to get edge cost
   int source: the source vertex
   int destination: the destination vertex
   RETURNS: cost of the edge if it exists, -1 if not
   NOTE: returning -1 isn't optimal, since sometimes we use negative edge costs */
int directedgraph_get_edge_cost(directedgraph* dg, int source, int destination);

int directedgraph_get_number_of_vertices(directedgraph* dg);

int directedgraph_get_number_of_edges(directedgraph* dg);

int directedgraph_get_greatest_vertex(directedgraph* dg);

/* Finds the specified edge and sets its edge cost
   directedgraph* dg: the directed graph to get edge cost
   int source: the source vertex
   int destination: the destination vertex
   int cost: the cost to set the edge
   MODIFIES: edge in g to new cost if the edge exists */
void directedgraph_set_edge_cost(directedgraph* dg, int source, int destination, int cost);

/* Adds an edge to a directed graph with unit cost
   directedgraph* dg: the directed graph to add the edge to
   int source: the source vertex
   int destination: the destination vertex
   MODIFES: g by adding a new edge */
void directedgraph_add_edge(directedgraph* dg, int source, int destination);

void directedgraph_add_vertex(directedgraph* dg, int v);

/* Adds an edge to a directed graph with cost
   directedgraph* dg: the directed graph to add the edge to
   int source: the source vertex
   int destination: the destination vertex
   int cost: the cost of the edge
   MODIFIES: g by adding a new edge with associated cost */
void directedgraph_add_weighted_edge(directedgraph* dg, int source, int destination, int cost);

/* Removes an edge from a directed graph
   directedgraph* dg: the directed graph to remove the edge from
   int source: the source vertex
   int destination: the destination vertex
   MODIFES: g by removing an edge */
void directedgraph_remove_edge(directedgraph* dg, int source, int destination);

void directedgraph_remove_edges_from_vertex(directedgraph* dg, int v);

void directedgraph_remove_vertex(directedgraph* dg, int v);

void directedgraph_remove_predecessors(directedgraph* dg, int destination);

directedgraph* directedgraph_create_copy(directedgraph* dg);

/* Prints a directed graph to console
   directedgraph* dg: the directed graph to be printed
   NOTE: probably don't want to do this for large graphs */
void directedgraph_print(directedgraph* dg);

void directedgraph_print_predecessors(directedgraph* dg);

/* Prints a directed graph to console including edge costs
   directedgraph* dg: the directed graph to be printed
   NOTE: probably don't want to do this for large graphs */
void directedgraph_print_weights(directedgraph* dg);

/* Frees the memory of a directed graph
   directedgraph* dg: the directed graph to be freed */
void directedgraph_free(directedgraph* dg);

adjlist* adjlist_create(int v);

adjlist* adjlist_get_next(adjlist* adj);

int adjlist_get_value(adjlist* adj);

void adjlist_set_value(adjlist* adj, int v);

int adjlist_is_element(adjlist* adj, int v);

void adjlist_add_element(adjlist* adj, int v);

void adjlist_print(adjlist* adj);

void adjlist_free(adjlist* adj);

adjlist* adjlist_find_cycle_in_directedgraph(directedgraph* dg);

#endif
