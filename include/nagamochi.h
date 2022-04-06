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



void print_edge_ls_fn(void* el);
void print_edge_ls(edge_ls* el);


typedef struct pair_ls
{
    struct pair_ls *next;
    struct pair_ls *prev;

    int u;
    int v;
} pair_ls;

pair_ls* pair_create(int u, int v);

pair_ls* blossom_merge(graph* g, int u, int v, int blossom_number, pair_ls* merge_order);

pair_ls* blossom_unmerge(graph* g, pair_ls* merge_order);

int_ls* last_blossom_verts(pair_ls* merge_order);


#endif