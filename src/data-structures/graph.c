#include <stdio.h>
#include <stdlib.h>
#include "../../include/graph.h"
#include "../../include/stack.h"

typedef struct Edge {
  int value;
  int cost;
  struct Edge* next;
} edge;

typedef struct Vertex {
  int value;
  int degree;
  edge* nextEdge;
  struct Vertex* nextVertex;
} vertex;


struct Graph {
  int vertices;
  int edges;
  vertex* root;
  vertex* vertexSet;
};

typedef struct AdjList {
   int value;
   struct AdjList* next;
} adjlist;

// This was done in a pretty convoluted fashion, could be rewritten to be simpler
void _add_edge(vertex* vs, edge* e) {
  edge* last = NULL;
  edge* curr = vs->nextEdge;
  if (curr) {
    while (curr->next) {
      if (curr->value > e->value) {
        if (last) {
          last->next = e;
          e->next = curr;
        } else {
          vs->nextEdge = e;
          e->next = curr;
        }
        break;
      }
      last = curr;
      curr = curr->next;
    }
    if (!curr->next && curr->value > e->value) {
      if (last) {
        last->next = e;
        e->next = curr;
      } else {
        vs->nextEdge = e;
        e->next = curr;
      }
    } else if (!curr->next) {
      curr->next = e;
    }
  } else {
    vs->nextEdge = e;
  }
  vs->degree++;
}

void _remove_edge(vertex* vs, int e) {
   edge* last = NULL;
   edge* curr = vs->nextEdge;

   while (curr) {
      if (curr->value == e) {
         if (last) {
            last->next = curr->next;
         } else {
            vs->nextEdge = curr->next;
         }
         free(curr);
         vs->degree--;
         break;
      }
      last = curr;
      curr = curr->next;
   }
}

void _add_vertex(vertex* vs, vertex* nvs) {
   vertex* curr = vs;
   vertex* last = NULL;
   while (curr) {
      if (curr->value < nvs->value) {
         last = curr;
         curr = curr->nextVertex;
      } else {
         if (last) {
            last->nextVertex = nvs;
         }
         nvs->nextVertex = curr;
         break;
      }
   }
   if (!curr) {
      last->nextVertex = nvs;
   }
}

void _remove_vertex(vertex* vs, int value) {
   vertex* currVertex = vs;
   vertex* prevVertex = NULL;
   while (currVertex) {
      if (currVertex->value == value) {
         if (prevVertex) {
            prevVertex->nextVertex = currVertex->nextVertex;
         }
         free(currVertex);
         break;
      }
      prevVertex = currVertex;
      currVertex = currVertex->nextVertex;
   }
}

edge* edge_get_next(edge* e) {
   return e->next;
}

int edge_get_value(edge* e) {
   return e->value;
}

int edge_get_cost(edge* e) {
   return e->cost;
}

void edge_set_cost(edge* e, int cost) {
   e->cost = cost;
}

edge* vertex_get_edges(vertex* vs) {
   return vs->nextEdge;
}

vertex* vertex_get_next(vertex* vs) {
   return vs->nextVertex;
}

int vertex_get_value(vertex* vs) {
   return vs->value;
}

int vertex_get_degree(vertex* vs) {
   return vs->degree;
}

graph* graph_create(int v) {
  graph* g = malloc(sizeof(*g));
  g->vertexSet = NULL;
  g->vertices = v;
  g->edges = 0;

  vertex* prevVertex = NULL;
  if (v > 0) {
     vertex* vs = malloc(sizeof(*vs));
     vs->value = 0;
     vs->degree = 0;
     vs->nextEdge = NULL;
     vs->nextVertex = NULL;
     g->vertexSet = vs;
     prevVertex = vs;
 }
  for (int i = 1; i < v; i++) {
     vertex* vs = malloc(sizeof(*vs));
     vs->value = i;
     vs->degree = 0;
     vs->nextEdge = NULL;
     vs->nextVertex = NULL;
     prevVertex->nextVertex = vs;
     prevVertex = vs;
  }

  return g;
}

int graph_is_edge(graph* g, int u, int v) {
   vertex* currVertex = g->vertexSet;
   while (currVertex) {
      if (currVertex->value == u) {
         break;
      }
      currVertex = currVertex->nextVertex;
   }
   edge* currEdge = currVertex->nextEdge;
   while(currEdge) {
      if (currEdge->value == v) {
         return 1;
      }
      currEdge = currEdge->next;
   }
   return 0;
}

void graph_add_edge(graph* g, int u, int v) {
  if (graph_is_edge(g, u, v)) {
    return;
  }

  vertex* currSourceVertex = g->vertexSet;
  while (currSourceVertex) {
     if (currSourceVertex->value == u) {
        break;
     }
     currSourceVertex = currSourceVertex->nextVertex;
  }

  edge* e1 = malloc(sizeof(*e1));
  e1->value = v;
  e1->cost = 1;
  e1->next = NULL;
  _add_edge(currSourceVertex, e1);

  vertex* currDestinationVertex = g->vertexSet;
  while (currDestinationVertex) {
     if (currDestinationVertex->value == v) {
        break;
     }
     currDestinationVertex = currDestinationVertex->nextVertex;
  }

  edge* e2 = malloc(sizeof(*e2));
  e2->value = u;
  e2->cost = 1;
  e2->next = NULL;
  _add_edge(currDestinationVertex, e2);

  g->edges++;
}

void graph_remove_edge(graph* g, int u, int v) {
  if (!graph_is_edge(g, u, v)) {
    return;
  }
  vertex* currSourceVertex = g->vertexSet;
  while (currSourceVertex) {
     if (currSourceVertex->value == u) {
        break;
     }
     currSourceVertex = currSourceVertex->nextVertex;
  }
  vertex* currDestinationVertex = g->vertexSet;
  while (currDestinationVertex) {
     if (currDestinationVertex->value == v) {
        break;
     }
     currDestinationVertex = currDestinationVertex->nextVertex;
  }

  _remove_edge(currSourceVertex, v);
  _remove_edge(currDestinationVertex, u);
  g->edges--;
}


void graph_print(graph* g) {
   printf("Printing Graph:\n");
   vertex* currVertex = g->vertexSet;
   while (currVertex) {
      printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->nextEdge;
      while(currEdge) {
         printf("%i -> ", currEdge->value);
         currEdge = currEdge->next;
      }
      printf("\n");
      currVertex = currVertex->nextVertex;
   }
   printf("\n");
}

//prints the format for CSAcademy's graph visualizer.
void graph_print_csacademy(graph* g){
   vertex* currVertex = g->vertexSet;
   while (currVertex) {
      //printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->nextEdge;
      while(currEdge) {
         printf("%i %i\n", currVertex->value, currEdge->value);
         currEdge = currEdge->next;
      }
      currVertex = currVertex->nextVertex;
   }
   printf("\n");
}

 const char* t = "0\n1\n2\n3\n0 1\n1 2\n2 3";

/* Creates a graph from csacademy's output format
   char* text: output text with linebreaks as '\n' */
graph* graph_construct_csacademy(char* text, int vertices){
   if(!text)
      return NULL;

   graph* out = graph_create(vertices);

   int e1 = -1;
   int e2 = -1;
   char space = 0;
   int i;

   for(i = 0; text[i]; i++){
      char c = text[i];
      if(c == '\n'){ //new line. add any complete edge and reset.
         if(e1 >= 0 && e2 >= 0){
            graph_add_edge(out, e1, e2);
         }
         e2 = e1 = -1;
         space = 0;
         continue;
      }

      if(c == ' '){
         space++;
         continue;
      }

      int* e = space ? &e2 : &e1; //select the edge that we are reading

      if(c >= '0' && c<='9'){  //adjust value of the selected edge
         if(*e > 0){
            *e *= 10;
            *e += c - '0';
         }
         else
            *e = c - '0';
      }
      else{
         printf("graph_construct_csacademy: invalid text");
         return out;
      }
   }

   if(!text[i]){
      if(e1 >= 0 && e2 >= 0){
            graph_add_edge(out, e1, e2);
         }
   }

   return out;
}

void graph_free(graph* g) {
   vertex* currVertex = g->vertexSet;
   vertex* prevVertex = NULL;
   while (currVertex) {
      edge* currEdge = currVertex->nextEdge;
      edge* lastEdge = NULL;
      while(currEdge) {
         lastEdge = currEdge;
         currEdge = currEdge->next;
         free(lastEdge);
      }
      prevVertex = currVertex;
      currVertex = currVertex->nextVertex;
      free(prevVertex);
   }
  free(g);
}

adjlist* adjlist_create(int v) {
   adjlist* adj = malloc(sizeof(*adj));
   adj->value = v;
   adj->next = NULL;

   return adj;
}

adjlist* adjlist_get_next(adjlist* adj) {
   return adj->next;
}

int adjlist_get_value(adjlist* adj) {
   return adj->value;
}

void adjlist_set_value(adjlist* adj, int v) {
   adj->value = v;
}

void adjlist_add_element(adjlist* adj, int v) {
   adjlist* a = adjlist_create(v);
   adjlist* curr = adj;
   while(curr->next) {
      curr = curr->next;
   }
   curr->next = a;
}

void adjlist_print(adjlist* adj) {
   adjlist* curr = adj;
   while(curr) {
      printf("%i -> ", curr->value);
      curr = curr->next;
   }
   printf("\n\n");
}

void adjlist_free(adjlist* adj) {
   adjlist* curr = adj;
   adjlist* last = NULL;
   while(curr) {
      last = curr;
      curr = curr->next;
      free(last);
   }
}
