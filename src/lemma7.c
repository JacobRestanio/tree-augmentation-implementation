#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"
#include <string.h>
#include "../include/list.h"
#include "../include/blossom.h"
#include "nagamochi.h"



int lemma7_min_edges = __INT32_MAX__;
pair_ls* lemma_7_edges = NULL;
pair_ls* merge_order_g = NULL;
pair_ls* merge_order_t = NULL;

void lemma72(graph* g, graph* t, int r, double approx){
    double l = (4/approx) - 1;

    lemma7_min_edges = __INT32_MAX__; //reset max

    if(lemma_7_edges){
        l_free(lemma_7_edges);
        lemma_7_edges = NULL;
    }

    merge_order_g = NULL;
    merge_order_t = NULL;

    //lemma 7 helper


    lemma7_helper2(g, t, r, approx, 0, NULL);


    printf("lemma 7 output:\n");
    for(pair_ls* p = lemma_7_edges; p; p = p->next){
        printf("%i %i\n", p->u, p->v);
        fflush(stdout);
    }

}

//modified_case1();
//modified_case2();

void lowest_edges2(graph* g, graph* t, int r, int_ls* leaves){
    int original_root = t->root;

    for(int_ls* lf = leaves; lf; lf = lf->next){
        int z = lf->value;
        set_root(t, z);

        int_ls* z_incident = NULL;

        for(edge* e = g->vert[z]->edge;e;e = e->next){
            z_incident = l_add(z_incident, value(g,e->otherVertex));
        }

        for(int_ls* u = z_incident; u; u = u->next){
            int uu = u->value;
            int_ls* des =  descendants(t,uu);
            des = ls_remove_all(des, uu);
            des = ls_first(des);

            if(ls_contains_any(des, z_incident)){
                remove_all_edges(g, uu, z);
            }
            ls_free(des);
        }
        ls_free(z_incident);
    }
    set_root(t,t->root);
}


pair_ls* pair_ls_copy(pair_ls* ls){
    
    pair_ls* new_ls = NULL;

    for(ls = l_last(ls); ls; ls=ls->prev){
        int u = ls->u;
        int v = ls->v;
        int bl = ls->blossom_number;
        pair_ls* new_pair = pair_create(u,v,bl);
        new_ls = l_add(new_ls, new_pair);
    }
    return new_ls;
}

void lemma7_helper2(graph* g, graph* t, int r, double approx, int recur_depth, pair_ls* cur_edges){
    double l = (4/approx) - 1;
    //cur_edges = pair_ls_copy(cur_edges);
    int cur_size = l_size(cur_edges);

    printf("lemma7_helper: cur_size: %i    recur_depth: %i  \n", cur_size, recur_depth);

    int_ls* leafs = leaves(t,r);
    int n_leaves = l_size(leafs);

    
    //check if the original tree is covered.
    if(leafs->value == r){
        if(cur_size < lemma7_min_edges){ //if best so far
            lemma7_min_edges = cur_size;

            if(lemma_7_edges)
                l_free(lemma_7_edges); //free old

            lemma_7_edges = cur_edges;
            return;
        }
        l_free(cur_edges);
        return;
    }


    lowest_edges2(g, t, r, leafs);

    int lf_bytes = sizeof(int)*n_leaves;
    int* lf_arr = malloc(lf_bytes);
    memset(lf_arr,0,lf_bytes);

    int e_k_bytes = sizeof(edge*)*n_leaves;
    edge** e_k = malloc(e_k_bytes);
    memset(e_k,0,e_k_bytes);

    //initialize each edge
    int_ls* c_lf = leafs;
    for(int i = 0; i<n_leaves && c_lf; i++){
        int ll = c_lf->value;
        lf_arr[i] = ll;
        e_k[i] = g->vert[ll]->edge;
    }

    //function to advance an edge.
    int combination_number = 1;

    // recurse on combination after merging edges.
    for(int i = 0; i < l*l; i++){
        printf("combo_n %i / %i  depth = %i\n", combination_number, (int)(l*l), recur_depth);
        fflush(stdout);
        // merge, recurse, then unmerge
        graph* new_g = normal_copy(g);
        graph* new_t = normal_copy(t);
        pair_ls* new_edges = pair_ls_copy(cur_edges);

        for(int k = 0; k<n_leaves; k++){ //merge all paths
            edge* e = e_k[k];
            int u = value(g,e->thisVertex);
            int v = value(g,e->otherVertex);
            
            //put each edge on new pair_ls with recur depth and pass to new fnctn
            new_edges = l_add(new_edges, pair_create(u,v,recur_depth));

            retain_merge_trim(new_g, new_t, u, v);
        }
        
        lemma7_helper2(new_g, new_t, r, approx, recur_depth+1, new_edges);

        l_free(new_edges);
        graph_free(new_g);
        graph_free(new_t);

        int j = n_leaves-1;
        int overflow = 0;
        do{
            overflow = 0;
            e_k[j] = e_k[j]->next;
            if(e_k[j] == NULL){
                e_k[j] = g->vert[lf_arr[j]]->edge;
                overflow = 1;
            }
            j--;
        }while(overflow && j >=0);
        if(j<0){
            printf("combination overflow!\n");
            break;
        }
    }
    l_free(leafs);
    free(lf_arr);
    free(e_k);
}

