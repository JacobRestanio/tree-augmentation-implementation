#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/graph.h"
#include "../../include/stack.h"
#include "../../include/list.h"

struct Edge {
   edge* next;
   edge* prev;
   vertex* source;
   vertex* destination;
   int cost;
   edge* twin;
};

struct Vertex {
   vertex* next;
   vertex* prev;
   int value;
   int degree;
   int visited;
   int inpath;
   edge* edges;
   graph* graph;
};

struct Graph {
   graph* next;
   graph* prev;
   vertex* vertices;
   int size;
   int edges;
};

struct DirectedGraph {
   directedgraph* next;
   directedgraph* prev;
   vertex* successors;
   vertex* predecessors;
   int size;
   int edges;
};

typedef struct AdjList {
   int value;
   struct AdjList* next;
} adjlist;

/*-----------------EDGES-----------------*/

edge* edge_create(vertex* u, vertex* v) {
   edge* e = malloc(sizeof(*e));
   e->next = NULL;
   e->prev = NULL;
   e->source = u;
   e->destination = v;
   e->cost = 1;
   e->twin = NULL;
   return e;
}

int edge_match(void* es1, void* es2) {
   edge* e1 = es1;
   edge* e2 = es2;
   return (e1->source->value == e2->source->value && e1->destination->value == e2->destination->value && e1->cost == e2->cost) ? 1 : 0;
}

edge* edge_get_next(edge* e) {
   return e->next;
}

edge* edge_next(edge* e) {
   return e->next;
}

void edge_set_next(edge* e, edge* next) {
   e->next = next;
}

edge* edge_get_prev(edge* e) {
   return e->prev;
}

edge* edge_prev(edge* e) {
   return e->prev;
}

void edge_set_prev(edge* e, edge* prev) {
   e->prev = prev;
}

vertex* edge_get_source(edge* e) {
   return e->source;
}

void edge_set_source(edge* e, vertex* u) {
   e->source = u;
   if (e->twin) {
      e->twin->destination = u;
   }
}

vertex* edge_get_destination(edge* e) {
   return e->destination;
}

int edge_destination_value(edge* e) {
   return e->destination->value;
}

void edge_set_destination(edge* e, vertex* v) {
   e->destination = v;
   if (e->twin) {
      e->twin->source = v;
   }
}

int edge_get_cost(edge* e) {
   return e->cost;
}

void edge_set_cost(edge* e, int c) {
   e->cost = c;
   if (e->twin) {
      e->twin->cost = c;
   }
}

edge* edge_get_twin(edge* e) {
   return e->twin;
}

void edge_set_twin(edge* e, edge* twin) {
   e->twin = twin;
}

void edge_free(void* es) {
   edge* e = es;
   free(e);
}

// This was done in a pretty convoluted fashion, could be rewritten to be simpler
void _add_edge(vertex* vs, edge* e) {
  edge* last = NULL;
  edge* curr = vs->edges;
  if (curr) {
    while (curr->next) {
      if (curr->destination > e->destination) {
        if (last) {
          last->next = e;
          e->next = curr;
        } else {
          vs->edges = e;
          e->next = curr;
        }
        break;
      }
      last = curr;
      curr = curr->next;
    }
    if (!curr->next && curr->destination > e->destination) {
      if (last) {
        last->next = e;
        e->next = curr;
      } else {
        vs->edges = e;
        e->next = curr;
      }
    } else if (!curr->next) {
      curr->next = e;
    }
  } else {
    vs->edges = e;
  }
  vs->degree++;
}

void _remove_edge(vertex* vs, int e) {
   edge* last = NULL;
   edge* curr = vs->edges;

   while (curr) {
      if (curr->destination->value == e) {
         if (last) {
            last->next = curr->next;
         } else {
            vs->edges = curr->next;
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
         curr = curr->next;
      } else {
         if (last) {
            last->next = nvs;
         }
         nvs->next = curr;
         break;
      }
   }
   if (!curr) {
      last->next = nvs;
   }
}

void _remove_vertex(vertex* vs, int value) {
   vertex* currVertex = vs;
   vertex* prevVertex = NULL;
   while (currVertex) {
      if (currVertex->value == value) {
         if (prevVertex) {
            prevVertex->next = currVertex->next;
         }
         free(currVertex);
         break;
      }
      prevVertex = currVertex;
      currVertex = currVertex->next;
   }
}

/*-----------------VERTEX-----------------*/

vertex* vertex_create(int v) {
   vertex* vert = malloc(sizeof(*vert));
   vert->next = NULL;
   vert->prev = NULL;
   vert->value = v;
   vert->degree = 0;
   vert->visited = 0;
   vert->inpath = 0;
   vert->edges = NULL;
   vert->graph = NULL;
   return vert;
}

int vertex_match(void* vs1, void* vs2) {
   vertex* v1 = vs1;
   vertex* v2 = vs2;
   return (v1->value == v2->value) ? 1 : 0;
}

void _vertex_add_edge(vertex* vs, edge* e) {
   vs->edges = list_add(vs->edges, e);
   vs->degree++;
}

vertex* vertex_get_next(vertex* vs) {
   return vs->next;
}

vertex* vertex_next(vertex* vs) {
   return vs->next;
}

void vertex_set_next(vertex* vs, vertex* next) {
   vs->next = next;
}

vertex* vertex_get_prev(vertex* vs) {
   return vs->prev;
}

vertex* vertex_prev(vertex* vs) {
   return vs->prev;
}

void vertex_set_prev(vertex* vs, vertex* prev) {
   vs->prev = prev;
}

int vertex_get_value(vertex* vs) {
   return vs->value;
}

void vertex_set_value(vertex* vs, int value) {
   vs->value = value;
}

int vertex_get_degree(vertex* vs) {
   return vs->degree;
}

void vertex_set_degree(vertex* vs, int degree) {
   vs->degree = degree;
}

int vertex_get_visited(vertex* vs) {
   return vs->visited;
}

void vertex_set_visited(vertex* vs, int visited) {
   vs->visited = visited;
}

int vertex_get_inpath(vertex* vs) {
   return vs->inpath;
}

void vertex_set_inpath(vertex* vs, int inpath) {
   vs->inpath = inpath;
}

edge* vertex_get_edges(vertex* vs) {
   return vs->edges;
}

void vertex_set_edges(vertex* vs, edge* es) {
   vs->edges = es;
}

void vertex_free(void* vs) {
   vertex* v = vs;
   list_free(v->edges, edge_free);
   free(v);
}

void vertex_print(void* vs) {
   vertex* v = vs;
   printf("%i", v->value);
}

/*-----------------GRAPH-----------------*/

graph* graph_create(int v) {
   graph* g = malloc(sizeof(*g));
   g->next = NULL;
   g->prev = NULL;
   g->vertices = NULL;
   g->size = v;
   g->edges = 0;

   for (int i = 1; i <= v; i++) {
      g->vertices = list_add(g->vertices, vertex_create(i));
   }

   return g;
}

vertex* graph_find_vertex(graph* g, int v) {
   vertex* vs = NULL;
   vertex* vvs = vertex_create(v);
   vs = list_contains(g->vertices, vertex_match, vvs);
   vertex_free(vvs);
   return vs;
}

edge* graph_find_edge(graph* g, int u, int v) {
   vertex* vs = NULL;
   edge* e = NULL;
   vertex* uvs = vertex_create(u);
   vertex* vvs = vertex_create(v);
   edge* es = edge_create(uvs, vvs);
   if ((vs = list_contains(g->vertices, vertex_match, uvs))) {
      e = list_contains(vs->edges, edge_match, es);
   }
   vertex_free(uvs);
   vertex_free(vvs);
   edge_free(es);
   return e;
}

int graph_is_edge(graph* g, edge* e) {
   vertex* vs = NULL;
   vertex* u = edge_get_source(e);
   if ((vs = graph_find_vertex(g, u->value))) {
      return (list_contains(vs->edges, edge_match, e)) ? 1 : 0;
   }
   return 0;
}

void graph_add_vertex(graph* g, int vs) {
   if (graph_find_vertex(g, vs)) {
      return;
   }
   vertex* v = vertex_create(vs);
   g->vertices = list_add(g->vertices, v);
   v->graph = g;
   g->size++;
}

void graph_insert_vertex(graph* g, vertex* vs) {
   graph_add_vertex(g, vs->value);
}

void graph_add_edge(graph* g, int u, int v) {
   if (graph_find_edge(g, u, v)) {
      return;
   }
   vertex* uvs = graph_find_vertex(g, u);
   vertex* vvs = graph_find_vertex(g, v);
   if (uvs && vvs) {
      if (vertex_match(uvs, vvs)) {
         edge* e = edge_create(uvs, vvs);
         _vertex_add_edge(uvs, e);
      } else {
         edge* e1 = edge_create(uvs, vvs);
         edge* e2 = edge_create(vvs, uvs);
         _vertex_add_edge(uvs, e1);
         _vertex_add_edge(vvs, e2);
         e1->twin = e2;
         e2->twin = e1;
      }
      g->edges++;
   }
}

void graph_insert_edge(graph* g, edge* e) {
   graph_add_edge(g, e->source->value, e->destination->value);
}

graph* graph_copy(graph* grph) {
   graph* g = graph_create(0);

   for (vertex* vtx = grph->vertices; vtx; vtx = vtx->next) {
      graph_add_vertex(g, vtx->value);
   }

   for (vertex* vtx = grph->vertices; vtx; vtx = vtx->next) {
      for (edge* es = vtx->edges; es; es = es->next) {
         graph_add_edge(g, es->source->value, es->destination->value);
      }
   }

   return g;
}

void graph_add_vertices(graph* g, vertex* vs) {
   for (; vs; vs = vs->next) {
      graph_add_vertex(g, vs->value);
   }
}

void graph_remove_edge(graph* g, int u, int v) {
   edge* e = graph_find_edge(g, u, v);
   if (!(e)) {
      return;
   }
   if (e->source->edges == e) {
      e->source->edges = e->next;
   }
   if (e->twin) {
      if (e->destination->edges == e->twin) {
         e->destination->edges = e->twin->next;
      }
      list_remove(e->twin, free);
      e->destination->degree--;
   }
   e->source->degree--;
   list_remove(e, free);
}

void graph_delete_edge(edge* e) {
   if (!(e)) {
      return;
   }
   if (e->source->edges == e) {
      e->source->edges = e->next;
   }
   if (e->twin) {
      if (e->destination->edges == e->twin) {
         e->destination->edges = e->twin->next;
      }
      e->destination->degree--;
      list_remove(e->twin, free);
   }
   e->source->degree--;
   list_remove(e, free);
}

void graph_remove_vertex(graph* g, int vs) {
   vertex* v = graph_find_vertex(g, vs);
   if (!(v)) {
      return;
   }
   edge* es = v->edges;
   while (es) {
      graph_delete_edge(es);
      es = v->edges;
   }
   if (g->vertices == v) {
      g->vertices = v->next;
   }
   g->size--;
   list_remove(v, free);
}

void graph_delete_vertex(vertex* v) {
   if (!(v)) {
      return;
   }
   edge* es = v->edges;
   while (es) {
      graph_delete_edge(es);
      es = v->edges;
   }
   if (v->graph->vertices == v) {
      v->graph->vertices = v->next;
   }
   v->graph->size--;
   list_remove(v, free);
}

int graph_find_largest_vertex(graph* g) {
   int m = 0;
   for (vertex* v = g->vertices; v; v = v->next) {
      m = (v->value > m) ? v->value : m;
   }
   return m;
}

void graph_print(graph* g) {
   printf("Printing Graph:\n");
   vertex* currVertex = g->vertices;
   while (currVertex) {
      printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->edges;
      while(currEdge) {
         printf("%i -> ", currEdge->destination->value);
         currEdge = currEdge->next;
      }
      printf("\n");
      currVertex = currVertex->next;
   }
   printf("\n");
}

void graph_print_to_file(graph* g, char* filename) {
   char filepath[255] = "data/";
   strcat(filepath, filename);
   strcat(filepath, ".txt");
   FILE* out = fopen(filepath, "w+");

   printf("Writing graph to file %s...\n", filepath);
   vertex* currVertex = g->vertices;
   while (currVertex) {
      fprintf(out, "%i ", currVertex->value);
      edge* currEdge = currVertex->edges;
      while(currEdge) {
         fprintf(out, "%i ", currEdge->destination->value);
         currEdge = currEdge->next;
      }
      fprintf(out, "\n");
      currVertex = currVertex->next;
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

vertex* graph_get_vertices(graph* g) {
   return g->vertices;
}

void graph_set_vertices(graph* g, vertex* vs) {
   g->vertices = vs;
}

int graph_get_size(graph* g) {
   return g->size;
}

void graph_set_size(graph* g, int size) {
   g->size = size;
}

int graph_get_edges(graph* g) {
   return g->edges;
}

void graph_set_edges(graph* g, int edges) {
   g->edges = edges;
}

void graph_free(void* grph) {
   graph* g = grph;
   list_free(g->vertices, vertex_free);
   free(g);
}

/*-----------------DIRECTED GRAPH-----------------*/

vertex* directedgraph_find_successor_vertex(directedgraph* dg, int v) {
   vertex* currSuccessorVertex = dg->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == v) {
         return currSuccessorVertex;
      }
      currSuccessorVertex = currSuccessorVertex->next;
   }
   return NULL;
}

vertex* directedgraph_find_predecessor_vertex(directedgraph* dg, int v) {
   vertex* currPredecessorVertex = dg->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == v) {
         return currPredecessorVertex;
      }
      currPredecessorVertex = currPredecessorVertex->next;
   }
   return NULL;
}

vertex* directedgraph_get_successors(directedgraph* dg) {
   return dg->successors;
}

vertex* directedgraph_get_predecessors(directedgraph* dg) {
   return dg->predecessors;
}

directedgraph* directedgraph_create(int v) {
   directedgraph* g = malloc(sizeof(*g));
   g->successors = NULL;
   g->predecessors = NULL;
   g->size = v;
   g->edges = 0;

   vertex* prevSuccessor = NULL;
   vertex* prevPredecessor = NULL;
   for (int i = 1; i <= v; i++) {
      vertex* vs = vertex_create(i);
      vertex* vp = vertex_create(i);
      if (i == 1) {
         prevSuccessor = vs;
         prevPredecessor = vp;
         g->successors = vs;
         g->predecessors = vp;
      } else {
         prevSuccessor->next = vs;
         prevSuccessor = vs;
         prevPredecessor->next = vp;
         prevPredecessor = vp;
      }
   }

   return g;
}

void directedgraph_add_vertices(directedgraph* dg, vertex* vs) {
   for (; vs; vs = vs->next) {
      directedgraph_add_vertex(dg, vs->value);
   }
}

int directedgraph_is_edge(directedgraph* dg, int source, int destination) {
  vertex* currVertex = dg->successors;
  while (currVertex) {
     if (currVertex->value == source) {
        break;
     }
      currVertex = currVertex->next;
 }
 if (!currVertex) {
    return 0;
}
  edge* currEdge = currVertex->edges;
  while (currEdge) {
    if (currEdge->destination->value == destination) {
      return 1;
    }
    currEdge = currEdge->next;
  }
  return 0;
}

int directedgraph_is_successor(directedgraph* dg, int source, int destination) {
  vertex* currVertex = dg->successors;
  while (currVertex) {
     if (currVertex->value == source) {
        break;
     }
      currVertex = currVertex->next;
 }
 if (!currVertex) {
    return 0;
}
  edge* currEdge = currVertex->edges;
  while (currEdge) {
    if (currEdge->destination->value == destination) {
      return 1;
    }
    currEdge = currEdge->next;
  }
  return 0;
}

int directedgraph_is_predecessor(directedgraph* dg, int source, int destination) {
   vertex* currVertex = dg->predecessors;
   while (currVertex) {
      if (currVertex->value == destination) {
         break;
      }
      currVertex = currVertex->next;
   }
   if (!currVertex) {
      return 0;
   }
   edge* currEdge = currVertex->edges;
   while (currEdge) {
      if (currEdge->destination->value == source) {
         return 1;
      }
      currEdge = currEdge->next;
   }
   return 0;
}

int directedgraph_is_vertex(directedgraph* dg, int v) {
   vertex* currVertex = dg->successors;
   while (currVertex) {
      if (currVertex->value == v) {
         return 1;
      }
      currVertex = currVertex->next;
   }
   return 0;
}

edge* directedgraph_get_edge(directedgraph* dg, int source, int destination) {
   vertex* currVertex = dg->successors;
   while (currVertex) {
     if (currVertex->value == source) {
         break;
     }
      currVertex = currVertex->next;
 }
 if (!currVertex) {
     return NULL;
}
   edge* currEdge = currVertex->edges;
   while (currEdge) {
     if (currEdge->destination->value == destination) {
      return currEdge;
     }
     currEdge = currEdge->next;
   }
   return NULL;
}

int directedgraph_get_edge_cost(directedgraph* dg, int source, int destination) {
   vertex* currVertex = dg->successors;
   while (currVertex) {
      if (currVertex->value == source) {
         break;
      }
      currVertex = currVertex->next;
   }
   if (!currVertex) {
      return -1;
   }
   edge* curr = currVertex->edges;
   for (int i = 0; i < currVertex->degree; i++) {
      if (curr->destination->value == destination) {
         return curr->cost;
      }
      curr = curr->next;
   }
   return -1;
}

int directedgraph_get_number_of_vertices(directedgraph* dg) {
   return dg->size;
}

int directedgraph_get_number_of_edges(directedgraph* dg) {
   return dg->edges;
}

int directedgraph_get_greatest_vertex(directedgraph* dg) {
   vertex* currVertex = dg->successors;
   while (currVertex->next) {
      currVertex = currVertex->next;
   }
   return currVertex->value;
}

void directedgraph_set_edge_cost(directedgraph* dg, int source, int destination, int cost) {
   // Set edge cost in list of successors
   vertex* currSuccessorVertex = dg->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->next;
   }
   if (!currSuccessorVertex) {
      return;
   }
   edge* currSuccessorEdge = currSuccessorVertex->edges;
   while(currSuccessorEdge) {
      if (currSuccessorEdge->destination->value == destination) {
         currSuccessorEdge->cost = cost;
         break;
      }
      currSuccessorEdge = currSuccessorEdge->next;
   }
   // Set edge cost in list of predecessors
   vertex* currPredecessorVertex = dg->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->next;
   }
   if (!currPredecessorVertex) {
      return;
   }
   edge* currPredecessorEdge = currPredecessorVertex->edges;
   while(currPredecessorEdge) {
      if (currPredecessorEdge->destination->value == destination) {
         currPredecessorEdge->cost = cost;
         break;
      }
      currPredecessorEdge = currPredecessorEdge->next;
   }
}

void directedgraph_add_edge(directedgraph* dg, int source, int destination) {
   if (directedgraph_is_edge(dg, source, destination)) {
      return;
   }

   vertex* ss = directedgraph_find_successor_vertex(dg, source);
   vertex* sd = directedgraph_find_successor_vertex(dg, destination);
   vertex* ps = directedgraph_find_predecessor_vertex(dg, source);
   vertex* pd = directedgraph_find_predecessor_vertex(dg, destination);

   if (ss && sd && ps && pd) {
      edge* successor = edge_create(ss, sd);
      _add_edge(ss, successor);

      edge* predecessor = edge_create(pd, ps);
      _add_edge(pd, predecessor);

      successor->twin = predecessor;
      predecessor->twin = successor;

      dg->edges++;
   }
}

void directedgraph_add_vertex(directedgraph* dg, int v) {
   if (directedgraph_is_vertex(dg, v)) {
      return;
   }

   vertex* vs = vertex_create(v);

   vertex* vp = vertex_create(v);

   if (dg->size) {
      _add_vertex(dg->successors, vs);
      _add_vertex(dg->predecessors, vp);

      if (v < dg->successors->value) {
         dg->successors = vs;
         dg->predecessors = vp;
      }
   } else {
      dg->successors = vs;
      dg->predecessors = vp;
   }
   dg->size++;
}

void directedgraph_add_weighted_edge(directedgraph* dg, int source, int destination, int cost) {
   vertex* currSuccessorVertex = dg->successors;
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == source) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->next;
   }
   if (!currSuccessorVertex) {
      return;
   }
   vertex* currPredecessorVertex = dg->predecessors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == destination) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->next;
   }
   if (!currPredecessorVertex) {
      return;
   }

   if (directedgraph_is_successor(dg, source, destination)) {
      edge* currSuccessorEdge = currSuccessorVertex->edges;
      edge* currPredecessorEdge = currPredecessorVertex->edges;
      while (currSuccessorEdge) {
         if (currSuccessorEdge->destination->value == destination) {
            if (cost < currSuccessorEdge->cost) {
               currSuccessorEdge->cost = cost;
            }
         }
         currSuccessorEdge = currSuccessorEdge->next;
      }
      while (currPredecessorEdge) {
         if (currPredecessorEdge->destination->value == source) {
            if (cost < currPredecessorEdge->cost) {
               currPredecessorEdge->cost = cost;
            }
         }
         currPredecessorEdge = currPredecessorEdge->next;
      }
      return;
   }

   edge* successor = edge_create(currSuccessorVertex, currPredecessorVertex);
   _add_edge(currSuccessorVertex, successor);

   edge* predecessor = edge_create(currPredecessorVertex, currSuccessorVertex);
   _add_edge(currPredecessorVertex, predecessor);

   successor->twin = predecessor;
   predecessor->twin = successor;

   dg->edges++;
}

void directedgraph_remove_edge(directedgraph* dg, int source, int destination) {
   edge* e = directedgraph_get_edge(dg, source, destination);
   if (!(e)) {
      return;
   }
   edge* e1 = e;
   edge* e2 = e->twin;
   _remove_edge(e1->source, destination);
   _remove_edge(e2->source, destination);

   dg->edges--;
}

void directedgraph_remove_edges_from_vertex(directedgraph* g, int v) {
   vertex* currPredecessorVertex = g->predecessors;
   vertex* currSuccessorVertex = g->successors;
   while (currPredecessorVertex) {
      if (currPredecessorVertex->value == v) {
         break;
      }
      currPredecessorVertex = currPredecessorVertex->next;
   }
   while (currSuccessorVertex) {
      if (currSuccessorVertex->value == v) {
         break;
      }
      currSuccessorVertex = currSuccessorVertex->next;
   }
   if (!currPredecessorVertex || !currSuccessorVertex) {
      return;
   }

   while (currSuccessorVertex->edges) {
      edge* e = currSuccessorVertex->edges;
      vertex* currPredecessorVertex = g->predecessors;
      while (currPredecessorVertex) {
         if (currPredecessorVertex->value == e->destination->value) {
            break;
         }
         currPredecessorVertex = currPredecessorVertex->next;
      }
      _remove_edge(currPredecessorVertex, v);
      _remove_edge(currSuccessorVertex, e->destination->value);

      g->edges--;
   }

   while (currPredecessorVertex->edges) {
      edge* e = currPredecessorVertex->edges;
      vertex* currSuccessorVertex = g->successors;
      while (currSuccessorVertex) {
         if (currSuccessorVertex->value == e->destination->value) {
            break;
         }
         currSuccessorVertex = currSuccessorVertex->next;
      }
      _remove_edge(currSuccessorVertex, v);
      _remove_edge(currPredecessorVertex, e->destination->value);

      g->edges--;
   }
}

void directedgraph_remove_vertex(directedgraph* g, int v) {
   if (!directedgraph_is_vertex(g, v)) {
      return;
   }

   vertex* nextSuccessor = g->successors->next;
   vertex* nextPredecessor = g->predecessors->next;
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
      currPredecessorVertex = currPredecessorVertex->next;
   }
   if (!currPredecessorVertex) {
      return;
   }

   while (currPredecessorVertex->edges) {
      edge* e = currPredecessorVertex->edges;
      vertex* currSuccessorVertex = g->successors;
      while (currSuccessorVertex) {
         if (currSuccessorVertex->value == e->destination->value) {
            break;
         }
         currSuccessorVertex = currSuccessorVertex->next;
      }
      _remove_edge(currSuccessorVertex, destination);
      _remove_edge(currPredecessorVertex, e->destination->value);

      g->edges--;
   }
}

directedgraph* directedgraph_create_copy(directedgraph* dg) {
   directedgraph* ndg = directedgraph_create(0);
   vertex* currVertex = dg->successors;
   edge* currEdge;
   while (currVertex) {
      directedgraph_add_vertex(ndg, currVertex->value);
      currVertex = currVertex->next;
   }
   currVertex = dg->successors;
   while (currVertex) {
      currEdge = currVertex->edges;
      while (currEdge) {
         directedgraph_add_weighted_edge(ndg, currVertex->value, currEdge->destination->value, currEdge->cost);
         currEdge = currEdge->next;
      }
      currVertex = currVertex->next;
   }
   return ndg;
}

void directedgraph_print(directedgraph* g) {
   printf("Printing Directed Graph:\n");
   vertex* currVertex = g->successors;
   while (currVertex) {
      printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->edges;
      while(currEdge) {
         printf("%i -> ", currEdge->destination->value);
         currEdge = currEdge->next;
      }
      printf("\n");
      currVertex = currVertex->next;
   }
   printf("\n");
}

void directedgraph_print_predecessors(directedgraph* g) {
   printf("Printing Directed Graph Predecessors:\n");
   vertex* currVertex = g->predecessors;
   while (currVertex) {
      printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->edges;
      while(currEdge) {
         printf("%i -> ", currEdge->destination->value);
         currEdge = currEdge->next;
      }
      printf("\n");
      currVertex = currVertex->next;
   }
   printf("\n");
}

void directedgraph_print_weights(directedgraph* g) {
   printf("Printing Directed Graph with Weights:\n");
   vertex* currVertex = g->successors;
   while (currVertex) {
      printf("%i -> ", currVertex->value);
      edge* currEdge = currVertex->edges;
      while(currEdge) {
         printf("%i (%i) -> ", currEdge->destination->value, currEdge->cost);
         currEdge = currEdge->next;
      }
      printf("\n");
      currVertex = currVertex->next;
   }
   printf("\n");
}

void directedgraph_free(directedgraph* g) {
   vertex* currSuccessorVertex = g->successors;
   vertex* lastSuccessorVertex = NULL;
   vertex* currPredecessorVertex = g->predecessors;
   vertex* lastPredecessorVertex = NULL;
   while (currSuccessorVertex) {
      edge* currSuccessorEdge = currSuccessorVertex->edges;
      edge* lastSuccessorEdge = NULL;
      while(currSuccessorEdge) {
         lastSuccessorEdge = currSuccessorEdge;
         currSuccessorEdge = currSuccessorEdge->next;
         free(lastSuccessorEdge);
      }
      lastSuccessorVertex = currSuccessorVertex;
      currSuccessorVertex = currSuccessorVertex->next;
      free(lastSuccessorVertex);
   }
   while (currPredecessorVertex) {
      edge* currPredecessorEdge = currPredecessorVertex->edges;
      edge* lastPredecessorEdge = NULL;
      while(currPredecessorEdge) {
         lastPredecessorEdge = currPredecessorEdge;
         currPredecessorEdge = currPredecessorEdge->next;
         free(lastPredecessorEdge);
      }
      lastPredecessorVertex = currPredecessorVertex;
      currPredecessorVertex = currPredecessorVertex->next;
      free(lastPredecessorVertex);
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
   edge* currEdge = vs->edges;
   vertex* nextVertex = dg->successors;
   while (currEdge) {
      while (nextVertex) {
         if (nextVertex->value == currEdge->destination->value) {
            break;
         }
         nextVertex = nextVertex->next;
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
   stack* path = stack_create(dg->size);
   int pathStart;

   vertex* currVertex = dg->successors;
   while (currVertex) {
      if ((pathStart = _find_cycle(dg, path, currVertex)) > -1) {
         break;
      }
      currVertex = currVertex->next;
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
