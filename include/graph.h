#ifndef GRAPH_H
#define GRAPH_H

//note to self: when you make the edgeset you can't store the edges as a list the way you normally would.
//              you will have to either copy the vertices or create a container.


//add null pointer catches

//EDGE
typedef struct Edge {
   int thisVertex;
   int otherVertex;

   struct Edge* next;
   struct Edge* twin;
} edge;

edge* edge_create(int thisVertex, int otherVertex);


//frees the whole list
void edge_free(edge* e);

//VERTEX
typedef struct Vertex {
  int value;
  int mergeValue;
  int degree;

  edge* edge;
  edge* lastedge;
} vertex;

vertex* vertex_create(int v);


//also frees any edges, but not their twins.
void vertex_free(vertex* v);


//GRAPH
typedef struct Graph {
  int vertex_count;
  int original_vertex_count;
  int edges;

  vertex** vert;
  
  //used only if the graph is a tree
  int root;
  int* depths;
  int* parents;
} graph;

graph* graph_create(int v);

void graph_free(graph* g);

//needs renamed
int value(graph* g, int v); //not sure if chains merge correctly

void add_new_edge(graph* g, int v1, int v2);

void graph_add_edge(graph* g, int v1, int v2); 

//returns null if no match.
edge* find_edge(graph* g, int v1, int v2);

int graph_is_edge(graph* g, int v1, int v2);

int remove_edge(graph* g, int v1, int v2); //degree. could be slightly improved by doubly linked edges

///NOTE:: CAN GO FROM O(E) -> O(1) IF WE KEEP TRACK OF LAST EDGE IN THE LIST.
///       IF DOUBLY LINKED, REMOVALS WOULD ALSO GO FROM O(E)->O(1).
///       COST OF 25% EXTRA STORAGE ON EDGES
//how should we handle vertex->parent in rooted trees?
//V1 SUBSUMES V2
//if you are doing this, you may want to do it on another graph too.
void merge_vertices(graph* g, int v1, int v2);


//TREE
void set_root(graph* tree, int v);

void generate_parents(graph* tree, int v);

int get_parent(graph* tree, int v);

int get_depth(graph* t, int v);

void merge_path(graph* t, int u, int v);


//PRINT

//prints the format for CSAcademy's graph visualizer.
void graph_print(graph* g);

/* Creates a graph from csacademy's output format
   char* text: output text with linebreaks as '\n' */
graph* graph_create_text(char* text, int vertex_count);

#endif