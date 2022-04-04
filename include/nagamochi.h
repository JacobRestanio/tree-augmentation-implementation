#ifndef NAGAMOCHI_H
#define NAGAMOCHI_H

#include "graph.h"

#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"

typedef struct edge_ls
{
    struct edge_ls *next;
    struct edge_ls *prev;

    edge *e;
} edge_ls;



edge *nagamochi(graph *g, graph *t, double approx);

void p1(graph *g, graph *t, int u);

int case1(graph *g, graph *t);
int case2(graph *g, graph *t);

int case3(graph *g, graph *t);

int case4(graph *g, graph *t);

// accepts edge_ls* and edge*
int edge_match(void *list, void *item);

// edge_ls* graph_adjacent_edges(graph* g, int v);

// computes maximum matching
edge_ls* blossom_algorithm(graph *g, int_ls *vs);

#endif