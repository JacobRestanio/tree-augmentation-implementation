#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"

edge* nagamochi(graph* g, graph* t, double approx){

    //TEST FOR SHORT CIRCUIT EVALUATION
    while(case1(g,t) ^ 2*case2(g,t) ^ 4*case3(g,t) ^ 8*case4(g,t));

    return NULL;
}



void p1(graph* g, graph* t, int u){

    //retain edges.
}

int case1(graph* g, graph* t){
    int ret = 0;
    int_ls* fringe = fringes(t,t->root);
    int_ls* cur_fringe = fringes;
    while(cur_fringe){
        int v = value(t,cur_fringe->value);
        if(lf_closed(g,t,v)){
            p1(g, t, v);
            ret = 1;
        }
    }
    ls_free(fringe);
    return ret;
}

int p2(graph* g, graph* t, int );





//maximum matching algorithm


// copied from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

// copied from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int unhash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = (x >> 16) ^ x;
    return x;
}

typedef struct pair {
    int key;
    int value;
} pair;


#define NUM_PRIMES 47
int primes[NUM_PRIMES] = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423, 9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243, 110503, 132049, 216091, 756839, 859433, 1257787, 1398269, 2976221, 3021377, 6972593, 13466917, 20996011, 24036583, 25964951, 30402457, 2147483647};

int prime_bigger_than(int size){
    #define factor 2
     size = (size+1)*factor;

    int i = 0;
    for(i = 0; i<NUM_PRIMES; i++){
        if(primes[i]>size)
            break;
    }
    return primes[i];
}

pair* map_get(pair** map, int len, int key){
    unsigned int i = hash(key) % len;

    while(map[i]){
        pair* kv = map[i];
        if(kv->key == key)
            return kv;
        i = (i+1)%len;
    }
    return NULL;
}

pair* map_add(pair** map, int len, int key, int value){
    unsigned int i = hash(key) % len;

    while(map[i]){
        pair* kv = map[i];
        if(kv->key == key){
            kv->value = value;
            return kv;
        }
        i = (i+1)%len;
    }
    map[i] = malloc(sizeof(pair*));
    map[i]->key = key;
    map[i]->value = value;
}

void map_free(pair** map, int len){
    for(int i =0; i<len; i++){
        if(map[i])
            free(map[i]);
    }
    free(map);
}


//computes maximum matching
edge* blossom_algorithm(graph* g, int_ls* vs){
    int num_v = ls_size(vs);

    int len = prime_bigger_than(num_v);
    pair** map = malloc(sizeof(pair*)*len);

    int_ls* cur_v = vs;
    for(int i = 1; cur_v; i++){
        int v = value(g,cur_v->value);
        map_add(map,len,v,i);
        cur_v = cur_v->next;
    }

    graph* gm = graph_create(num_v);

    int_ls* cur_v = vs;
    while(cur_v){
        int v = value(g,cur_v->value);
        edge* e = g->vert[v]->edge;
        while(e){
            int other_v = value(g,e->otherVertex);

            int this_v = map_get(map,len,v)->value;
            pair* this_other_v = map_get(map,len,other_v);
            if(this_other_v){
                graph_add_edge(gm,this_v,this_other_v->value);
            }
        }
    }

    //matching list 
    //array to keep track of matched vertices

    //continually dfs from un-matched verts
        //add to matching 
    // if loop is found, add it to blossom

    //un-merge procedure.
    //


    graph_free(gm);
    map_free(map,len);
}



