#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/graph.h"
#include "../../include/stack.h"

typedef struct Edge {
  int value;
  int cost;
  struct Edge* correspondingEdge;
  struct Edge* next;
} edge;

typedef struct Vertex {
  int value;
  int degree;
  int visited;
  int inpath;
  edge* nextEdge;
  struct Vertex* nextVertex;
} vertex;

struct DirectedGraph {
  int vertices;
  int edges;
  vertex* successors;
  vertex* predecessors;
};

struct Graph {
  int vertices;
  int edges;
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

edge* edge_get_corresponding_edge(edge* e) {
   return e->correspondingEdge;
}

void edge_set_cost(edge* e, int cost) {
   e->cost = cost;
}

void edge_set_corresponding_edge(edge* e, edge* ce) {
   e->correspondingEdge = ce;
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

int vertex_get_visited(vertex* vs) {
   return vs->visited;
}

int vertex_get_inpath(vertex* vs) {
   return vs->inpath;
}

void vertex_set_visited(vertex* vs, int visited) {
   vs->visited = visited;
}

void vertex_set_inpath(vertex* vs, int inpath) {
   vs->inpath = inpath;
}

vertex* directedgraph_get_vertex_successors(directedgraph* dg, int v) {
   vertex* currSuccessorVertex = dg->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == v) {
         return currSuccessorVertex;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   return NULL;
}

vertex* directedgraph_get_vertex_predecessors(directedgraph* dg, int v) {
   vertex* currPredecessorVertex = dg->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == v) {
         return currPredecessorVertex;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   return NULL;
}

vertex* directedgraph_get_vertex_successors_list(directedgraph* dg) {
   return dg->successors;
}

vertex* directedgraph_get_vertex_predecessors_list(directedgraph* dg) {
   return dg->predecessors;
}

directedgraph* directedgraph_create(int v) {
   directedgraph* g = malloc(sizeof(*g));
   g->successors = NULL;
   g->predecessors = NULL;
   g->vertices = v;
   g->edges = 0;

   vertex* lastSuccessor = NULL;
   vertex* lastPredecessor = NULL;
   if (v > 0) {
      vertex* ess = malloc(sizeof(*ess));
      ess->value = 0;
      ess->degree = 0;
      ess->visited = 0;
      ess->inpath = 0;
      ess->nextEdge = NULL;
      ess->nextVertex = NULL;
      g->successors = ess;
      lastSuccessor = ess;

      vertex* esp = malloc(sizeof(*esp));
      esp->value = 0;
      esp->degree = 0;
      esp->visited = 0;
      esp->inpath = 0;
      esp->nextEdge = NULL;
      esp->nextVertex = NULL;
      g->predecessors = esp;
      lastPredecessor = esp;
   }
   for (int i = 1; i < v; i++) {
      vertex* ess = malloc(sizeof(*ess));
      ess->value = i;
      ess->degree = 0;
      ess->visited = 0;
      ess->inpath = 0;
      ess->nextEdge = NULL;
      ess->nextVertex = NULL;
      lastSuccessor->nextVertex = ess;
      lastSuccessor = ess;

      vertex* esp = malloc(sizeof(*esp));
      esp->value = i;
      esp->degree = 0;
      esp->visited = 0;
      esp->inpath = 0;
      esp->nextEdge = NULL;
      esp->nextVertex = NULL;
      lastPredecessor->nextVertex = esp;
      lastPredecessor = esp;
   }

   return g;
}

int directedgraph_is_successor(directedgraph* g, int source, int destination) {
  vertex* currVertex = g->successors;
  while (currVertex) {
     if (currVertex->value == source) {
        break;
     }
      currVertex = currVertex->nextVertex;
 }
 if (!currVertex) {
    return 0;
}
  edge* currEdge = currVertex->nextEdge;
  while (currEdge) {
    if (currEdge->value == destination) {
      return 1;
    }
    currEdge = currEdge->next;
  }
  return 0;
}

int directedgraph_is_predecessor(directedgraph* g, int destination, int source) {
   vertex* currVertex = g->predecessors;
   while (currVertex) {
      if (currVertex->value == destination) {
         break;
      }
      currVertex = currVertex->nextVertex;
   }
   if (!currVertex) {
      return 0;
   }
   edge* currEdge = currVertex->nextEdge;
   while (currEdge) {
      if (currEdge->value == source) {
         return 1;
      }
      currEdge = currEdge->next;
   }
   return 0;
}

int directedgraph_is_vertex(directedgraph* g, int v) {
   vertex* currVertex = g->successors;
   while (currVertex) {
      if (currVertex->value == v) {
         return 1;
      }
      currVertex = currVertex->nextVertex;
   }
   return 0;
}

int directedgraph_get_edge_cost(directedgraph* g, int source, int destination) {
   vertex* currVertex = g->successors;
   while (currVertex) {
      if (currVertex->value == source) {
         break;
      }
      currVertex = currVertex->nextVertex;
   }
   if (!currVertex) {
      return -1;
   }
   edge* curr = currVertex->nextEdge;
   for (int i = 0; i < currVertex->degree; i++) {
      if (curr->value == destination) {
         return curr->cost;
      }
      curr = curr->next;
   }
   return -1;
}

int directedgraph_get_number_of_vertices(directedgraph* dg) {
   return dg->vertices;
}

int directedgraph_get_number_of_edges(directedgraph* dg) {
   return dg->edges;
}

int directedgraph_get_greatest_vertex(directedgraph* dg) {
   vertex* currVertex = dg->successors;
   while (currVertex->nextVertex) {
      currVertex = currVertex->nextVertex;
   }
   return currVertex->value;
}

void directedgraph_set_edge_cost(directedgraph* g, int source, int destination, int cost) {
   // Set edge cost in list of successors
   vertex* currSuccessorVertex = g->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   if (!currSuccessorVertex) {
      return;
   }
   edge* currSuccessorEdge = currSuccessorVertex->nextEdge;
   while(currSuccessorEdge) {
      if (currSuccessorEdge->value == destination) {
         currSuccessorEdge->cost = cost;
         break;
      }
      currSuccessorEdge = currSuccessorEdge->next;
   }
   // Set edge cost in list of predecessors
   vertex* currPredecessorVertex = g->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   if (!currPredecessorVertex) {
      return;
   }
   edge* currPredecessorEdge = currPredecessorVertex->nextEdge;
   while(currPredecessorEdge) {
      if (currPredecessorEdge->value == destination) {
         currPredecessorEdge->cost = cost;
         break;
      }
      currPredecessorEdge = currPredecessorEdge->next;
   }
}

void directedgraph_add_edge(directedgraph* g, int source, int destination) {
   if (directedgraph_is_successor(g, source, destination)) {
      return;
   }

   vertex* currSuccessorVertex = g->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   if (!currSuccessorVertex) {
      return;
   }
   vertex* currPredecessorVertex = g->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   if (!currPredecessorVertex) {
      return;
   }

   edge* dest = malloc(sizeof(*dest));
   dest->value = destination;
   dest->cost = 1;
   dest->correspondingEdge = NULL;
   dest->next = NULL;
   _add_edge(currSuccessorVertex, dest);

   edge* srce = malloc(sizeof(*srce));
   srce->value = source;
   srce->cost = 1;
   srce->correspondingEdge = NULL;
   srce->next = NULL;
   _add_edge(currPredecessorVertex, srce);

   g->edges++;
}

void directedgraph_add_vertex(directedgraph* g, int v) {
   if (directedgraph_is_vertex(g, v)) {
      return;
   }

   vertex* ness = malloc(sizeof(*ness));
   ness->value = v;
   ness->degree = 0;
   ness->visited = 0;
   ness->inpath = 0;
   ness->nextEdge = NULL;
   ness->nextVertex = NULL;

   vertex* nesp = malloc(sizeof(*nesp));
   nesp->value = v;
   nesp->degree = 0;
   nesp->visited = 0;
   nesp->inpath = 0;
   nesp->nextEdge = NULL;
   nesp->nextVertex = NULL;

   if (g->vertices) {
      _add_vertex(g->successors, ness);
      _add_vertex(g->predecessors, nesp);

      if (v < g->successors->value) {
         g->successors = ness;
         g->predecessors = nesp;
      }
   } else {
      g->successors = ness;
      g->predecessors = nesp;
   }
   g->vertices++;
}

void directedgraph_add_weighted_edge(directedgraph* g, int source, int destination, int cost) {
   vertex* currSuccessorVertex = g->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   if (!currSuccessorVertex) {
      return;
   }
   vertex* currPredecessorVertex = g->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   if (!currPredecessorVertex) {
      return;
   }

   if (directedgraph_is_successor(g, source, destination)) {
      edge* currSuccessorEdge = currSuccessorVertex->nextEdge;
      edge* currPredecessorEdge = currPredecessorVertex->nextEdge;
      while (currSuccessorEdge) {
         if (currSuccessorEdge->value == destination) {
            if (cost < currSuccessorEdge->cost) {
               currSuccessorEdge->cost = cost;
            }
         }
         currSuccessorEdge = currSuccessorEdge->next;
      }
      while (currPredecessorEdge) {
         if (currPredecessorEdge->value == source) {
            if (cost < currPredecessorEdge->cost) {
               currPredecessorEdge->cost = cost;
            }
         }
         currPredecessorEdge = currPredecessorEdge->next;
      }
      return;
   }

   edge* dest = malloc(sizeof(*dest));
   dest->value = destination;
   dest->cost = cost;
   dest->next = NULL;
   _add_edge(currSuccessorVertex, dest);

   edge* srce = malloc(sizeof(*srce));
   srce->value = source;
   srce->cost = cost;
   srce->next = NULL;
   _add_edge(currPredecessorVertex, srce);

   g->edges++;
}

void directedgraph_add_corresponding_weighted_edge(directedgraph* dg, int source, int destination, edge* correspondingEdge, int cost) {
   vertex* currSuccessorVertex = dg->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   if (!currSuccessorVertex) {
      return;
   }
   vertex* currPredecessorVertex = dg->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   if (!currPredecessorVertex) {
      return;
   }

   if (directedgraph_is_successor(dg, source, destination)) {
      edge* currSuccessorEdge = currSuccessorVertex->nextEdge;
      edge* currPredecessorEdge = currPredecessorVertex->nextEdge;
      while (currSuccessorEdge) {
         if (currSuccessorEdge->value == destination) {
            if (cost < currSuccessorEdge->cost) {
               currSuccessorEdge->cost = cost;
            }
         }
         currSuccessorEdge = currSuccessorEdge->next;
      }
      while (currPredecessorEdge) {
         if (currPredecessorEdge->value == source) {
            if (cost < currPredecessorEdge->cost) {
               currPredecessorEdge->cost = cost;
            }
         }
         currPredecessorEdge = currPredecessorEdge->next;
      }
      return;
   }

   edge* dest = malloc(sizeof(*dest));
   dest->value = destination;
   dest->cost = cost;
   dest->correspondingEdge = correspondingEdge;
   dest->next = NULL;
   _add_edge(currSuccessorVertex, dest);

   edge* srce = malloc(sizeof(*srce));
   srce->value = source;
   srce->cost = cost;
   srce->correspondingEdge = correspondingEdge;
   srce->next = NULL;
   _add_edge(currPredecessorVertex, srce);

   dg->edges++;
}

void directedgraph_remove_edge(directedgraph* g, int source, int destination) {
   if (!directedgraph_is_successor(g, source, destination)) {
      return;
   }
   vertex* currSuccessorVertex = g->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   if (!currSuccessorVertex) {
      return;
   }
   vertex* currPredecessorVertex = g->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   if (!currPredecessorVertex) {
      return;
   }

   _remove_edge(currSuccessorVertex, destination);
   _remove_edge(currPredecessorVertex, source);

   g->edges--;
}

void directedgraph_remove_edges_from_vertex(directedgraph* g, int v) {
   vertex* currPredecessorVertex = g->predecessors;
   vertex* currSuccessorVertex = g->successors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == v) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == v) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->nextVertex;
   }
   if (!currPredecessorVertex || !currSuccessorVertex) {
      return;
   }

   while (currSuccessorVertex->nextEdge) {
      edge* e = currSuccessorVertex->nextEdge;
      vertex* currPredecessorVertex = g->predecessors;
      while (currPredecessorVertex) {
         if (currPredecessorVertex->value == e->value) {
            break;
         }
         currPredecessorVertex = currPredecessorVertex->nextVertex;
      }
      _remove_edge(currPredecessorVertex, v);
      _remove_edge(currSuccessorVertex, e->value);

      g->edges--;
   }

   while (currPredecessorVertex->nextEdge) {
      edge* e = currPredecessorVertex->nextEdge;
      vertex* currSuccessorVertex = g->successors;
      while (currSuccessorVertex) {
         if (currSuccessorVertex->value == e->value) {
            break;
         }
         currSuccessorVertex = currSuccessorVertex->nextVertex;
      }
      _remove_edge(currSuccessorVertex, v);
      _remove_edge(currPredecessorVertex, e->value);

      g->edges--;
   }
}

void directedgraph_remove_vertex(directedgraph* g, int v) {
   if (!directedgraph_is_vertex(g, v)) {
      return;
   }

   vertex* nextSuccessor = g->successors->nextVertex;
   vertex* nextPredecessor = g->predecessors->nextVertex;
   int initialVertex = g->successors->value;

   directedgraph_remove_edges_from_vertex(g, v);

   _remove_vertex(g->successors, v);
   _remove_vertex(g->predecessors, v);

   if (v == initialVertex && nextSuccessor) {
      g->successors = nextSuccessor;
      g->predecessors = nextPredecessor;
   }

}

void directedgraph_remove_predecessors(directedgraph* g, int destination) {
   vertex* currPredecessorVertex = g->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->nextVertex;
   }
   if (!currPredecessorVertex) {
      return;
   }

   while (currPredecessorVertex->nextEdge) {
      edge* e = currPredecessorVertex->nextEdge;
      vertex* currSuccessorVertex = g->successors;
      while (currSuccessorVertex) {
         if (currSuccessorVertex->value == e->value) {
            break;
         }
         currSuccessorVertex = currSuccessorVertex->nextVertex;
      }
      _remove_edge(currSuccessorVertex, destination);
      _remove_edge(currPredecessorVertex, e->value);

      g->edges--;
   }
}

directedgraph* directedgraph_create_copy(directedgraph* dg) {
   directedgraph* ndg = directedgraph_create(0);
   vertex* currVertex = dg->successors;
   edge* currEdge;
   while (currVertex) {
      directedgraph_add_vertex(ndg, currVertex->value);
      currVertex = currVertex->nextVertex;
   }
   currVertex = dg->successors;
   while (currVertex) {
      currEdge = currVertex->nextEdge;
      while (currEdge) {
         directedgraph_add_weighted_edge(ndg, currVertex->value, currEdge->value, currEdge->cost);
         currEdge = currEdge->next;
      }
      currVertex = currVertex->nextVertex;
   }
   return ndg;
}

void directedgraph_print(directedgraph* g) {
   printf("Printing Directed Graph:\n");
   vertex* currVertex = g->successors;
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

void directedgraph_print_predecessors(directedgraph* g) {
   printf("Printing Directed Graph Predecessors:\n");
   vertex* currVertex = g->predecessors;
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

void directedgraph_print_weights(directedgraph* g) {
   printf("Printing Directed Graph with Weights:\n");
   vertex* currVertex = g->successors;
   while (currVertex) {
      printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->nextEdge;
      while(currEdge) {
         printf("%i (%i) -> ", currEdge->value, currEdge->cost);
         currEdge = currEdge->next;
      }
      printf("\n");
      currVertex = currVertex->nextVertex;
   }
   printf("\n");
}

void directedgraph_free(directedgraph* g) {
   vertex* currSuccessorVertex = g->successors;
   vertex* lastSuccessorVertex = NULL;
   vertex* currPredecessorVertex = g->predecessors;
   vertex* lastPredecessorVertex = NULL;
   while (currSuccessorVertex) {
      edge* currSuccessorEdge = currSuccessorVertex->nextEdge;
      edge* lastSuccessorEdge = NULL;
      while(currSuccessorEdge) {
         lastSuccessorEdge = currSuccessorEdge;
         currSuccessorEdge = currSuccessorEdge->next;
         free(lastSuccessorEdge);
      }
      lastSuccessorVertex = currSuccessorVertex;
      currSuccessorVertex = currSuccessorVertex->nextVertex;
      free(lastSuccessorVertex);
   }
   while (currPredecessorVertex) {
      edge* currPredecessorEdge = currPredecessorVertex->nextEdge;
      edge* lastPredecessorEdge = NULL;
      while(currPredecessorEdge) {
         lastPredecessorEdge = currPredecessorEdge;
         currPredecessorEdge = currPredecessorEdge->next;
         free(lastPredecessorEdge);
      }
      lastPredecessorVertex = currPredecessorVertex;
      currPredecessorVertex = currPredecessorVertex->nextVertex;
      free(lastPredecessorVertex);
   }
   free(g);
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
     vs->visited = 0;
     vs->inpath = 0;
     vs->nextEdge = NULL;
     vs->nextVertex = NULL;
     g->vertexSet = vs;
     prevVertex = vs;
 }
  for (int i = 1; i < v; i++) {
     vertex* vs = malloc(sizeof(*vs));
     vs->value = i;
     vs->degree = 0;
     vs->visited = 0;
     vs->inpath = 0;
     vs->nextEdge = NULL;
     vs->nextVertex = NULL;
     prevVertex->nextVertex = vs;
     prevVertex = vs;
  }

  return g;
}

vertex* graph_get_vertex_list(graph* g) {
   return g->vertexSet;
}

vertex* graph_get_vertex(graph* g, int v) {
   vertex* currVertex = g->vertexSet;
   while (currVertex) {
      if (currVertex->value == v) {
         return currVertex;
      }
      currVertex = currVertex->nextVertex;
   }
   return NULL;
}

int graph_get_number_of_vertices(graph* g) {
   return g->vertices;
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

int graph_is_vertex(graph* g, int v) {
   vertex* currVertex = g->vertexSet;
   while (currVertex) {
      if (currVertex->value == v) {
         return 1;
      }
      currVertex = currVertex->nextVertex;
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
  e1->correspondingEdge = NULL;
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
  e2->correspondingEdge = NULL;
  e2->next = NULL;
  _add_edge(currDestinationVertex, e2);

  g->edges++;
}

void graph_add_vertex(graph* g, int v) {
   if (graph_is_vertex(g, v)) {
      return;
   }

   vertex* vertex = malloc(sizeof(*vertex));
   vertex->value = v;
   vertex->degree = 0;
   vertex->visited = 0;
   vertex->inpath = 0;
   vertex->nextEdge = NULL;
   vertex->nextVertex = NULL;

   if (g->vertices) {
      _add_vertex(g->vertexSet, vertex);

      if (v < g->vertexSet->value) {
         g->vertexSet = vertex;
      }
   } else {
      g->vertexSet = vertex;
   }
   g->vertices++;
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

void graph_print_to_file(graph* g, char* filename) {
   char filepath[255] = "data/";
   strcat(filepath, filename);
   strcat(filepath, ".txt");
   FILE* out = fopen(filepath, "w+");

   printf("Writing graph to file %s...\n", filepath);
   vertex* currVertex = g->vertexSet;
   while (currVertex) {
      fprintf(out, "%i ", currVertex->value);
      edge* currEdge = currVertex->nextEdge;
      while(currEdge) {
         fprintf(out, "%i ", currEdge->value);
         currEdge = currEdge->next;
      }
      fprintf(out, "\n");
      currVertex = currVertex->nextVertex;
   }
   fclose(out);
   printf("Finished\n\n");
}

graph* graph_read_from_file(char* filename) {
   char filepath[255] = "data/";
   strcat(filepath, filename);
   strcat(filepath, ".txt");
   FILE* in = fopen(filepath, "r");
   graph* g = graph_create(0);

   printf("Reading graph from file %s...\n", filepath);
   char buffer[1024];
   char delimiter[3] = " \n";
   char* token;
   int vertex;
   int edge;

   while(fgets(buffer, sizeof(buffer), in)) {
     token = strtok(buffer, delimiter);
     vertex = atoi(token);
     graph_add_vertex(g, vertex);
     token = strtok(NULL, delimiter);
     while(token) {
        edge = atoi(token);
        graph_add_vertex(g, edge);
        graph_add_edge(g, vertex, edge);
        token = strtok(NULL, delimiter);
     }
   }
   printf("Finished\n\n");
   return g;
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

int adjlist_is_element(adjlist* adj, int v) {
   adjlist* curr = adj;
   while(curr) {
      if (curr->value == v) {
         return 1;
      }
      curr = curr->next;
   }
   return 0;
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

int _find_cycle(directedgraph* dg, stack* path, vertex* vs) {
   int pathStart;
   vs->visited = 1;
   vs->inpath = 1;
   stack_push(path, vs->value);
   edge* currEdge = vs->nextEdge;
   vertex* nextVertex = dg->successors;
   while (currEdge) {
      while (nextVertex) {
         if (nextVertex->value == currEdge->value) {
            break;
         }
         nextVertex = nextVertex->nextVertex;
      }

      if (nextVertex) {
         if (nextVertex->inpath) {
            return nextVertex->value;
         } else if (!nextVertex->visited) {
            if ((pathStart = _find_cycle(dg, path, nextVertex)) > -1) {
               return pathStart;
            }
         }
      }
      currEdge = currEdge->next;
   }
   vs->inpath = 0;
   stack_pop(path);
   return -1;
}

adjlist* adjlist_find_cycle_in_directedgraph(directedgraph* dg) {
   stack* path = stack_create(dg->vertices);
   int pathStart;

   vertex* currVertex = dg->successors;
   while (currVertex) {
      if ((pathStart = _find_cycle(dg, path, currVertex)) > -1) {
         break;
      }
      currVertex = currVertex->nextVertex;
   }

   if (pathStart == -1) {
      stack_free(path);
      return NULL;
   }
   adjlist* cycle = adjlist_create(stack_pop(path));
   while (stack_peek(path) != pathStart) {
      adjlist_add_element(cycle, stack_pop(path));
   }
   adjlist_add_element(cycle, pathStart);
   stack_free(path);
   return cycle;
}
