#ifndef NAGAMOCHI_H
#define NAGAMOCHI_H

#include "graph.h"

#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"

edge *nagamochi(graph *g, graph *t, double approx);

void p1(graph *g, graph *t, int u);

int case1(graph *g, graph *t);
int case2(graph *g, graph *t);

int case3(graph *g, graph *t);

int case4(graph *g, graph *t);

// maximum matching algorithm

// copied from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x);

// copied from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int unhash(unsigned int x);

typedef struct pair
{
    int key;
    int value;
} pair;

pair *map_get(pair **map, int len, int key);

pair *map_add(pair **map, int len, int key, int value);

void map_free(pair **map, int len);

// accepts edge_ls* and edge*
int edge_match(void *list, void *item);

// edge_ls* graph_adjacent_edges(graph* g, int v);

// computes maximum matching
edge *blossom_algorithm(graph *g, int_ls *vs);

#endif