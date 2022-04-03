#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"
#include <string.h>
#include "../include/list.h"

void p1(graph *g, graph *t, int u)
{

    // retain edges.
}

int case1(graph *g, graph *t)
{
    printf("\n\n__CASE 1__\n\n");
    printf("not debugged yet");
    fflush(stdout);
    return 0;

    int ret = 0;
    int_ls *fringe = fringes(t, t->root);
    int_ls *cur_fringe = fringe;
    while (cur_fringe)
    {
        int v = value(t, cur_fringe->value);
        if (l_closed(g, t, v))
        {
            p1(g, t, v);
            ret = 1;
        }
        cur_fringe = cur_fringe->next;
    }
    ls_free(fringe);
    printf("\t ret: %i",ret);
   fflush(stdout);
    return ret;
}

int case2(graph *g, graph *t)
{
    printf("\n\n__CASE 2__\n\n");
    fflush(stdout);

    int_ls *fringe = fringes(t, t->root);
    int_ls *cur_fringe = fringe;

    int ret = 0;
    while (cur_fringe)
    {   
       printf("cur_fringe: %i\n",cur_fringe->value);
        fflush(stdout);

        int parent = value(t, cur_fringe->value);
        if (!l_closed(g, t, parent))
        {
            printf("not l-closed\n");
            fflush(stdout);
            int_ls *kids = children(t, parent);
            int_ls *cur_kid = kids;
            while (cur_kid)
            {
                printf("cur_kid %i\n",cur_kid->value);
                fflush(stdout);
                int cur_v = value(g, cur_kid->value);
                int triv = trivial(g, t, cur_kid->value); // vertex connection that makes the cur_kid non-trivial;
                if (triv && (cur_v != value(g, parent)))
                {
                    printf("cur_v: %i\ttriv:%i\n", cur_v, triv);
                    graph_print(g);
                    fflush(stdout);


                    ret++;
                    retain_merge_trim(g, t, cur_v, triv);
                    printf("1.33..\n");
                    fflush(stdout);
                }
                cur_kid = cur_kid->next;
            }
            ls_free(kids);
        }
        cur_fringe = cur_fringe->next;
    }
    ls_free(fringe);
    printf("\t ret: %i",ret);
   fflush(stdout);
    return ret;
}

int case3(graph *g, graph *t)
{
    printf("\n\n__CASE 3__\n\n");
    fflush(stdout);

    int_ls *fringe = fringes(t, t->root);
    int_ls *cur_fringe = fringe;
    int ret = 0;
    while (cur_fringe)
    {
        int parent = value(t, cur_fringe->value);
        if (!l_closed(g, t, parent))
        {
            int_ls *kids = children(t, parent);
            if (ls_size(kids) == 3)
            {
                int_ls *iso = isolated(g, t, parent);
                if (iso == NULL)
                {
                    int_ls *cur_kid = kids;
                    while (cur_kid)
                    {
                        if (trivial(g, t, cur_kid->value))
                            break;
                        cur_kid = cur_kid->next;
                    }
                    if (!cur_kid)
                    {
                        ret++;
                        int par_of_par = get_parent(t, parent);
                        merge_vertices(g, parent, par_of_par);
                        merge_vertices(t, parent, par_of_par);
                        remove_self_edges(g, parent);
                        remove_self_edges(t, parent);
                    }
                }
                ls_free(iso);
            }
            ls_free(kids);
        }
        cur_fringe = cur_fringe->next;
    }
    ls_free(fringe);

    printf("\t ret: %i",ret);
    fflush(stdout);

    return ret;
}



int case4(graph *g, graph *t)
{   
    printf("\n\n__CASE 4__\n\n");
    fflush(stdout);

    int u = t->root;

    int ret = 0;//return value. true if contract
    int_ls *fringe = fringes(t, u);
    ls_print(fringe);
    int_ls *cur_fri = fringe;

    while (cur_fri)
    {
        printf("checking %i.. \n",cur_fri->value);
        int f = value(g, cur_fri->value);
        int_ls *kids = children(t, f);

        if (kids->next && !kids->next->next)
        { // number of kids is two
            printf("%i: number of kids is two\n", f);
            int u1 = kids->value;
            int u2 = kids->next->value;
            if (graph_is_edge(g, u1, u2))
            { // kids are connected, prime edge type 1
                printf("%i: u1(%i) and u2(%i)  are connected\n", f, u1, u2);
                int cur_parent = get_parent(t, f);
                int n;
                while (cur_parent)
                {
                    printf("%i (%i): climbing parents\n", f, cur_parent);
                    int_ls *cur_kids = children(t, cur_parent);
                    if (cur_kids->next && !cur_kids->next->next)
                    { // two children
                        printf("%i (%i): <-- potential p_fringe\n", f, cur_parent);
                        int v = cur_parent;
                        cur_parent = 0; // stop while loop

                        // make u3 the leaf hanging from p_fringe
                        int u3 = is_leaf(t, cur_kids->value);
                        u3 = u3 ? u3 : is_leaf(t, cur_kids->next->value);
                        printf("%i : u3 is %i\n", f, u3);
                        if (u3)
                        {
                            if (!l_closed(g, t, v))
                            { // the paper asserts that this is true, but I am not convinced
                                printf("%i - %i not lclosed\n", f, v);
                                int u1_u3 = graph_is_edge(g, u1, u3);
                                int u2_u3 = graph_is_edge(g, u2, u3);

                                int unconnected = (u1_u3? 0:u1) + (u2_u3?0:u2);
                                printf("%i - unconnected %i\n", f, unconnected);
                                int unconnected_connects_outside = 0;

                                
                                edge* e = g->vert[unconnected]->edge;
                                int_ls* tree_verts = descendants(t,v);
                                printf("non p-fringe descendants: "); ls_print(tree_verts);
                                printf("\n");
                                if(!(u1_u3 && u2_u3)){ //this check isnt necessary
                                    
                                    while(e){
                                        int cur_e = value(g,e->otherVertex);
                                        printf("e:%i  ", cur_e);
                                        if(!ls_contains(tree_verts,cur_e)){
                                            // mark for parent contraction
                                            unconnected_connects_outside = 1;
                                            printf("%i - connects outside = %i\n", f, unconnected_connects_outside);
                                            break;
                                        }
                                        e = e->next;
                                    }
                                     printf("%i - connects outside = %i\n", f, unconnected_connects_outside);
                                }

                                if ((u1_u3 && u2_u3) || unconnected_connects_outside)
                                { // prime edge type 2
                                    retain_merge_trim(g,t,v, get_parent(t,v));
                                    ret = 1;
                                }
                                else{
                                    retain_merge_trim(g,t,u1, u2);
                                    ret = 1;
                                }
                                ls_free(tree_verts);
                            }
                        }
                    }
                    ls_free(cur_kids);
                    if (cur_parent)
                    {
                        int np = get_parent(t, cur_parent);
                        cur_parent = np == cur_parent ? 0 : np;
                    }
                }
            }
        }
        ls_free(kids);
        cur_fri = cur_fri->next;
    }
    ls_free(fringe);
    printf("\t ret: %i",ret);
   fflush(stdout);
    return ret;
}

// maximum matching algorithm

// copied from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash(unsigned int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

// copied from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int unhash(unsigned int x)
{
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = (x >> 16) ^ x;
    return x;
}

typedef struct pair
{
    int key;
    int value;
} pair;

#define NUM_PRIMES 47
int primes[NUM_PRIMES] = {2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423, 9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243, 110503, 132049, 216091, 756839, 859433, 1257787, 1398269, 2976221, 3021377, 6972593, 13466917, 20996011, 24036583, 25964951, 30402457, 2147483647};

int prime_bigger_than(int size)
{
#define factor 2
    size = (size + 1) * factor;

    int i = 0;
    for (i = 0; i < NUM_PRIMES; i++)
    {
        if (primes[i] > size)
            break;
    }
    return primes[i];
}

pair *map_get(pair **map, int len, int key)
{
    unsigned int i = hash(key) % len;

    while (map[i])
    {
        pair *kv = map[i];
        if (kv->key == key)
            return kv;
        i = (i + 1) % len;
    }
    return NULL;
}

pair *map_add(pair **map, int len, int key, int value)
{
    unsigned int i = hash(key) % len;

    while (map[i])
    {
        pair *kv = map[i];
        if (kv->key == key)
        {
            kv->value = value;
            return kv;
        }
        i = (i + 1) % len;
    }
    map[i] = malloc(sizeof(pair *));
    map[i]->key = key;
    map[i]->value = value;
}

void map_free(pair **map, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (map[i])
            free(map[i]);
    }
    free(map);
}

typedef struct edge_ls
{
    struct edge_ls *next;
    struct edge_ls *prev;

    edge *e;
} edge_ls;

graph *gm = NULL;

// accepts edge_ls* and edge*
int edge_match(void *list, void *item)
{
    if (!gm)
        return 0;
    edge *cur_e = ((edge_ls *)list)->e;
    edge *e = ((edge_ls *)item)->e;
    return (value(gm, (cur_e->otherVertex)) ^ (value(gm, cur_e->thisVertex)) == (value(gm, (e->otherVertex) ^ value(gm, e->thisVertex))));
}

edge_ls *graph_adjacent_edges(graph *g, int v)
{
    v = value(g, v);
    char *added = malloc(sizeof(char) * (1 + g->vertex_count));
    memset(added, 0, sizeof(char) * (1 + g->vertex_count));
    added[v] = 1;
    edge_ls *adj_verts = NULL;
    edge *e = g->vert[v]->edge;
    while (e)
    {
        int cur_vert = value(g, e->otherVertex);
        if (!added[cur_vert])
        {
            edge_ls *eee = malloc(sizeof(edge_ls));
            eee->e = e;
            adj_verts = l_add(adj_verts, eee);
            added[cur_vert] = 1;
        }
        e = e->next;
    }
    free(added);
    return adj_verts;
}

// computes maximum matching
edge *blossom_algorithm(graph *g, int_ls *vs)
{
    int num_v = ls_size(vs);

    int len = prime_bigger_than(num_v);
    pair **map = malloc(sizeof(pair *) * len);

    int_ls *cur_v = vs;
    for (int i = 1; cur_v; i++)
    {
        int v = value(g, cur_v->value);
        map_add(map, len, v, i);
        cur_v = cur_v->next;
    }

    gm = graph_create(num_v);

    cur_v = vs;
    while (cur_v)
    {
        int v = value(g, cur_v->value);
        edge *e = g->vert[v]->edge;
        while (e)
        {
            int other_v = value(g, e->otherVertex);

            int this_v = map_get(map, len, v)->value;
            pair *this_other_v = map_get(map, len, other_v);
            if (this_other_v)
            {
                graph_add_edge(gm, this_v, this_other_v->value);
            }
        }
    }

    edge_ls *matching = NULL;

    int *matched = malloc((num_v + 1) * sizeof(int));
    memset(matched, 0, sizeof(char) * (num_v + 1));
    int *queued = malloc((num_v + 1) * sizeof(int));
    int nonaugmented = 0;

    for (int i = 1; i <= num_v && nonaugmented < num_v; i = i % (num_v + 1) + 1)
    { // for each vert

        if (matched[i])
        { // matched, the algorithm ignores.
            nonaugmented++;
            continue;
        }

        int v = map_get(map, len, i)->value;
        int_ls *queue = ls_add(NULL, v);
        int_ls *end_queue = queue;
        memset(queued, 0, sizeof(int) * (num_v + 1)); // set all verts to unprocessed
        int dfs_depth = 1;
        queued[v] = dfs_depth;
        while (queue)
        {
            int v = value(gm, queue->value);
            if (queued[v])
            {
                queue = queue->next;
                continue;
            }
            dfs_depth = queued[v] + 1;

            edge_ls *adj = graph_adjacent_edges(gm, v);
            edge_ls *cur_adj = adj;
            while (cur_adj)
            {
                int cur_edg = value(gm, cur_adj->e->otherVertex);

                edge *e = edge_create(v, cur_edg);
                e->next = NULL;
                e->twin = NULL;

                void *in_matching = l_contains(matching, edge_match, e);
                edge_free(e);
                e = NULL;

                int odd = dfs_depth % 2;

                if ((odd && in_matching) || (!odd && !in_matching))
                {
                    cur_adj = cur_adj->next;
                    continue;
                }

                if (!queued[cur_edg])
                {
                    if (!matched[v] && !matched[cur_edg])
                    { // free edge that can be matched
                        // add to matching (non-value)
                        edge_ls *cur_e = malloc(sizeof(edge_ls));
                        cur_e->e = cur_adj->e;
                        l_add(matching, cur_e);

                        int u = cur_e->e->otherVertex;
                        int v = cur_e->e->thisVertex;

                        matched[u] = v;
                        matched[v] = u;
                        // update
                    }
                    // if even and unmatched, invert the current path.

                    if (!odd && !matched[v])
                        ;

                    int_ls *new = ls_add(NULL, cur_edg);
                    queued[cur_edg] = dfs_depth;
                    end_queue = ls_merge(end_queue, new);
                    end_queue = ls_last(new);
                }
                else if (queued[cur_edg] == dfs_depth)
                {
                    // contract blossom
                    // reDFS
                    // mark for lifting
                }
                else
                {
                    // ignore
                }

                cur_adj = cur_adj->next;
            }

            // if queued, check depth
            // if match
            // backtrack and contract blossom
            // rerun dfs
            // if queued
            // skip

            // flip edges.
            // update matching nonaugmented = 0;
            //  un-merge all.

            // edge_ls
            // while(adj)
            // free(adj);
        }
        free(ls_first(queue));
    }

    graph_free(gm);
    gm = NULL;
    map_free(map, len);
}

edge *nagamochi(graph *g, graph *t, double approx)
{

    // at the very very end, we may have to check that each edge is covered,
    // p4 doesnt seem to guarantee covering an edge when contracting f'
    // apparently we can select any edge that covers it??/???

    // this may not work. test for short circuit evaluation.
    while (case1(g, t) ^ 2 * case2(g, t) ^ 4 * case3(g, t) ^ 8 * case4(g, t));

    return NULL;
}