#ifndef GRAPH_H
#define GRAPH_H

//note to self: when you make the edgeset you can't store the edges as a list the way you normally would.
//              you will have to either copy the vertices or create a container.


//add null pointer catches

////////////////////
//EDGE
////////////////////
typedef struct Edge {
   int thisVertex;
   int otherVertex;

   struct Edge* next;
   struct Edge* twin;
} edge;

edge* edge_create(int thisVertex, int otherVertex){
   edge* e = malloc(sizeof(*e));
   e->thisVertex = thisVertex;
   e->otherVertex = otherVertex;
   e->next = NULL;
   e->twin = NULL;
}

///////////////////////
//VERTEX
///////////////////////
typedef struct Vertex {
  int value;
  int mergeValue;
  int parent; //used only if the graph is a tree
  int degree;

  edge* edge;
} vertex;

vertex* vertex_create(int v);


/////////////////////
//GRAPH
/////////////////////
typedef struct Graph;


graph* graph_create(int v);
}


///////////////////////////
//TREE
///////////////////////////


//you should probably only do this if the graph is actually a tree.
void set_root(graph* tree, int v);

void generate_parents(graph* tree, int v);


////////////////////

int value(graph* g, int v);


void add_new_edge(graph* g, int v1, int v2);

////////////////////////////////////////////////NOTE:: CAN GO FROM O(E) -> O(1) IF WE KEEP TRACK OF LAST EDGE IN THE LIST.
//how should we handle vertex->parent? 
//V1 SUBSUMES V2
//if you are doing this, you may want to do it on another graph too.
void merge_vertices(graph* g, int v1, int v2);

//true value is not correct.

//twin nonsense?
//returns null if no match.
edge* find_edge(graph* g, int v1, int v2);

int remove_edge(graph* g, int v1, int v2);

//frees the whole list
void edge_free(edge* e);

//also frees any edges and their twins.
void vertex_free(vertex* v);


void graph_free(graph* g);
}


//////////////////////////
//PRINT
//////////////////////////

//prints the format for CSAcademy's graph visualizer.
void graph_print(graph* g);

/* Creates a graph from csacademy's output format
   char* text: output text with linebreaks as '\n' */
graph* graph_create_text(char* text, int vertex_count);

#endif