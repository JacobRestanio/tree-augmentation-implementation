#include <stdio.h>
#include <stdlib.h>
#include "../../include/graph.h"

typedef struct Edge {
  int v;
  int cost;
  struct Edge* next;
} edge;

typedef struct EdgeSet {
  int degree;
  edge* next;
} edgeset;

struct DirectedGraph {
  int vertices;
  int edges;
  edgeset* successors;
  edgeset* predecessors;
};

struct Graph {
  int vertices;
  int edges;
  edgeset* edgeSet;
};

void _add_edge(edgeset* es, edge* e) {
  edge* last = NULL;
  edge* curr = es->next;
  if (curr) {
    while (curr->next) {
      if (curr->v > e->v) {
        if (last) {
          last->next = e;
          e->next = curr;
        } else {
          es->next = e;
          e->next = curr;
        }
        break;
      }
      last = curr;
      curr = curr->next;
    }
    if (!curr->next && curr->v > e->v) {
      if (last) {
        last->next = e;
        e->next = curr;
      } else {
        es->next = e;
      }
    } else if (!curr->next) {
      curr->next = e;
    }
  } else {
    es->next = e;
  }
  es->degree++;
}

void _remove_edge(edgeset* es, int e) {
  edge* last = NULL;
  edge* curr = NULL;
  if (es->next) {
    curr = es->next;
    while (curr->next){
      if (curr->v == e) {
        if (last) {
          last->next = curr->next;
        } else {
          es->next = curr->next;
        }
        free(curr);
        es->degree--;
        curr = NULL;
        break;
      }
      last = curr;
      curr = curr->next;
    }
    if (curr && curr->v == e) {
      if (last) {
        last->next = curr->next;
      } else {
        es->next = curr->next;
      }
      free(curr);
      es->degree--;
    }
  }
}

directedgraph* directedgraph_create(int v) {
  directedgraph* g = malloc(sizeof(*g));
  g->successors = malloc(sizeof(*(g->successors)) * v);
  g->predecessors = malloc(sizeof(*(g->predecessors)) * v);
  g->vertices = v;
  g->edges = 0;

  for (int i = 0; i < v; i++) {
    g->successors[i].degree = 0;
    g->successors[i].next = NULL;
    g->predecessors[i].degree = 0;
    g->predecessors[i].next = NULL;
  }

  return g;
}

int directedgraph_is_successor(directedgraph* g, int source, int destination) {
  if (source >= g->vertices || destination >= g->vertices) {
    return 0;
  }
  edge* curr = g->successors[source].next;
  for (int i = 0; i < g->successors[source].degree; i++) {
    if (curr->v == destination) {
      return 1;
    }
    curr = curr->next;
  }
  return 0;
}

int directedgraph_is_predecessor(directedgraph* g, int destination, int source) {
  if (source >= g->vertices || destination >= g->vertices) {
    return 0;
  }
  edge* curr = g->predecessors[destination].next;
  for (int i = 0; i < g->predecessors[destination].degree; i++) {
    if (curr->v == source) {
      return 1;
    }
    curr = curr->next;
  }
  return 0;
}

int directedgraph_get_edge_cost(directedgraph* g, int source, int destination) {
   if (source >= g->vertices || destination >= g->vertices) {
      return -1;
   }
   edge* curr = g->successors[source].next;
   for (int i = 0; i < g->successors[source].degree; i++) {
      if (curr->v == destination) {
         return curr->cost;
      }
      curr = curr->next;
   }
   return -1;
}

void directedgraph_set_edge_cost(directedgraph* g, int source, int destination, int cost) {
   if (source >= g->vertices || destination >= g->vertices) {
      return;
   }
   edge* curr = g->successors[source].next;
   for (int i = 0; i < g->successors[source].degree; i++) {
      if (curr->v == destination) {
         curr->cost = cost;
         break;
      }
      curr = curr->next;
   }
   curr = g->predecessors[destination].next;
   for (int i = 0; i < g->predecessors[destination].degree; i++) {
      if (curr->v == source) {
         curr->cost = cost;
      }
      curr = curr->next;
      break;
   }
}

void directedgraph_add_edge(directedgraph* g, int source, int destination) {
  if (source >= g->vertices || destination >= g->vertices) {
    return;
  }
  if (directedgraph_is_successor(g, source, destination)) {
    return;
  }

  edge* dest = malloc(sizeof(*dest));
  dest->v = destination;
  dest->cost = 1;
  dest->next = NULL;
  _add_edge(&(g->successors[source]), dest);

  edge* srce = malloc(sizeof(*srce));
  srce->v = source;
  srce->cost = 1;
  srce->next = NULL;
  _add_edge(&(g->predecessors[destination]), srce);

  g->edges++;
}

void directedgraph_add_weighted_edge(directedgraph* g, int source, int destination, int cost) {
  if (source >= g->vertices || destination >= g->vertices) {
    return;
  }
  if (directedgraph_is_successor(g, source, destination)) {
    return;
  }

  edge* dest = malloc(sizeof(*dest));
  dest->v = destination;
  dest->cost = cost;
  dest->next = NULL;
  _add_edge(&(g->successors[source]), dest);

  edge* srce = malloc(sizeof(*srce));
  srce->v = source;
  srce->cost = cost;
  srce->next = NULL;
  _add_edge(&(g->predecessors[destination]), srce);

  g->edges++;
}

void directedgraph_remove_edge(directedgraph* g, int source, int destination) {
  if (source >= g->vertices || destination >= g->vertices) {
    return;
  }
  if (!directedgraph_is_successor(g, source, destination)) {
    return;
  }

  _remove_edge(&(g->successors[source]), destination);
  _remove_edge(&(g->predecessors[destination]), source);
  g->edges--;
}

void directedgraph_print(directedgraph* g) {
  printf("Printing Directed Graph:\n");
  for (int i = 0; i < g->vertices; i++) {
    printf("%i -> ", i);
    if (g->successors[i].next) {
      edge* curr = g->successors[i].next;
      printf("%i -> ", curr->v);
      while(curr->next) {
        curr = curr->next;
        printf("%i -> ", curr->v);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void directedgraph_print_weights(directedgraph* g) {
   printf("Printing Directed Graph:\n");
   for (int i = 0; i < g->vertices; i++) {
     printf("%i -> ", i);
     if (g->successors[i].next) {
       edge* curr = g->successors[i].next;
       printf("%i -> ", curr->v);
       while(curr->next) {
         curr = curr->next;
         printf("%i (%i)-> ", curr->v, curr->cost);
       }
     }
     printf("\n");
   }
   printf("\n");
}

void directedgraph_free(directedgraph* g) {
  for (int i = 0; i < g->vertices; i++) {
    edge* last = NULL;
    edge* curr = NULL;
    if (g->successors[i].next) {
      curr = g->successors[i].next;
      while (curr->next) {
        last = curr;
        curr = curr->next;
        free(last);
      }
      free(curr);
    }
    if (g->predecessors[i].next) {
      curr = g->predecessors[i].next;
      while (curr->next) {
        last = curr;
        curr = curr->next;
        free(last);
      }
      free(curr);
    }
  }
  free(g->successors);
  free(g->predecessors);
  free(g);
}

graph* graph_create(int v) {
  graph* g = malloc(sizeof(*g));
  g->edgeSet = malloc(sizeof(struct EdgeSet) * v);
  g->vertices = v;
  g->edges = 0;

  for (int i = 0; i < v; i++) {
    g->edgeSet[i].degree = 0;
    g->edgeSet[i].next = NULL;
  }

  return g;
}

int graph_is_edge(graph* g, int u, int v) {
  if (u >= g->vertices || v >= g->vertices) {
    return 0;
  }
  edge* curr = g->edgeSet[u].next;
  for (int i = 0; i < g->edgeSet[u].degree; i++) {
    if (curr->v == v) {
      return 1;
    }
    if (curr->next) {
      curr = curr->next;
    }
  }
  return 0;
}

void graph_add_edge(graph* g, int u, int v) {
  if (u >= g->vertices || v >= g->vertices) {
    return;
  }
  if (graph_is_edge(g, u, v)) {
    return;
  }

  edge* e1 = malloc(sizeof(*e1));
  e1->v = v;
  e1->cost = 1;
  e1->next = NULL;
  _add_edge(&(g->edgeSet[u]), e1);

  edge* e2 = malloc(sizeof(*e2));
  e2->v = u;
  e2->cost = 1;
  e2->next = NULL;
  _add_edge(&(g->edgeSet[v]), e2);

  g->edges++;
}

void graph_remove_edge(graph* g, int u, int v) {
  if (u >= g->vertices || v >= g->vertices) {
    return;
  }
  if (!graph_is_edge(g, u, v)) {
    return;
  }

  _remove_edge(&(g->edgeSet[u]), v);
  _remove_edge(&(g->edgeSet[v]), u);
  g->edges--;
}

void graph_print(graph* g) {
  printf("Printing Graph:\n");
  for (int i = 0; i < g->vertices; i++) {
    printf("%i -> ", i);
    if (g->edgeSet[i].next) {
      edge* curr = g->edgeSet[i].next;
      printf("%i -> ", curr->v);
      while(curr->next) {
        curr = curr->next;
        printf("%i -> ", curr->v);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void graph_free(graph* g) {
  for (int i = 0; i < g->vertices; i++) {
    edge* last = NULL;
    edge* curr = NULL;
    if (g->edgeSet[i].next) {
      curr = g->edgeSet[i].next;
      while (curr->next) {
        last = curr;
        curr = curr->next;
        free(last);
      }
      free(curr);
    }
  }
  free(g->edgeSet);
  free(g);
}
