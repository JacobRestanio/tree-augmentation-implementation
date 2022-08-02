#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"
#include <string.h>
#include "../include/list.h"
#include "../include/blossom.h"
#include "nagamochi.h"
#include "lemma7.h"



int lemma7_min_edges2 = __INT32_MAX__;
pair_ls* lemma_7_edges2 = NULL;
pair_ls* merge_order_g2 = NULL;
pair_ls* merge_order_t2 = NULL;

void lemma72(graph* g, graph* t, int r, double approx){
    double l = (4/approx) - 1;

    lemma7_min_edges2 = __INT32_MAX__; //reset max

    if(lemma_7_edges2){
        l_free(lemma_7_edges2);
        lemma_7_edges2 = NULL;
    }

    merge_order_g2 = NULL;
    merge_order_t2 = NULL;

    int_ls* des = descendants(t,r);
    int* newold;
    int* oldnew;

    int* a;
    int* b;

    graph* g_new = graph_copy(g, r, des,&oldnew,&newold);
    graph* t_new = graph_copy(t, r, des,&a,&b);

    g_new = normal_copy(g);
    t_new = normal_copy(t);

    lemma7_helper2(g_new, t_new, r, approx, 0, NULL);


    printf("\nlemma 7 output:\n");

    for(pair_ls* p = l_last(lemma_7_edges2); p; p = p->prev){
        int u1 = value(g, p->u);
        int u2 = value(g, p->v);

        edge* e = find_edge(g,u1,u2);
        if(!e){
            printf("lemma 7 err: edge not found\n");
        }else{
            printf("(%i, %i)  \t%i\n", e->thisVertex, e->otherVertex, p->blossom_number);
            retain_merge_trim(g,t,u1,u2);
        }
        fflush(stdout);
    }

    printf("\n\n");
    graph_print_all(g);

    graph_free(g_new);
    free(newold);
    free(oldnew);
    ls_free(des);
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
            z_incident = ls_add(z_incident, value(g,e->otherVertex));
        }

        for(int_ls* u = z_incident; u; u = u->next){
            int uu = u->value;


            int_ls* des =  descendants(t,uu);

            while(des = ls_contains(des, uu)){
                des = ls_remove(des);
            }

            des = ls_first(des);

            if(ls_contains_any(des, z_incident)){
                while(remove_edge(g, uu, z));
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

int times_ran = 0;

void lemma7_helper2(graph* g, graph* t, int r, double approx, int recur_depth, pair_ls* cur_edges){
    double l = (4/approx) - 1;
    r = value(g,r);
    int cur_size = l_size(cur_edges);

    printf("lemma7_helper: cur_size: %i    recur_depth: %i  times_ran %i\n", cur_size, recur_depth, times_ran++);

    int_ls* leafs = leaves(t,r);
    int n_leaves = l_size(leafs);

    
    //check if the original tree is covered.
    printf("r: %i,  lf->v: %i    n_lfs:%i   \n", r, leafs->value, n_leaves);
    if(leafs->value == r || g->vert[leafs->value]->edge == NULL){
        if(cur_size < lemma7_min_edges2){ //if best so far
            lemma7_min_edges2 = cur_size;

            if(lemma_7_edges2){
                l_free(lemma_7_edges2); //free old
            }
            lemma_7_edges2 = cur_edges;
            return;
        }
        l_free(cur_edges);
        l_free(leafs);
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
        c_lf = c_lf->next;
        printf("%i  cur_lf: %i   %X\n",i, ll,e_k[i]);
    }

    //function to advance an edge.
    int combination_number = 1;

    // recurse on combination after merging edges.
    for(int i = 0; i < l*l; i++){
        // merge, recurse, then unmerge
        if(recur_depth == 0)
        printf("combination number: %i\n", combination_number++);
        graph* new_g = normal_copy(g);
        graph* new_t = normal_copy(t);
        pair_ls* new_edges = pair_ls_copy(cur_edges);

        for(int k = 0; k<n_leaves; k++){ //merge all paths
            edge* e = e_k[k];

            int u = value(new_g,e->thisVertex);
            int v = value(new_g,e->otherVertex);
            if(u==v || u==0 || v==0)
                continue;

            //put each edge on new pair_ls with recur depth and pass to new fnctn
            new_edges = l_add(new_edges, pair_create(e->thisVertex,e->otherVertex,recur_depth));
     
            retain_merge_trim(new_g, new_t, u, v);
        }
        
        lemma7_helper2(new_g, new_t, r, approx, recur_depth+1, new_edges);

        int j = n_leaves-1;
        int overflow = 0;
        do{
            
            overflow = 0;
            e_k[j] = e_k[j]->next;
            
            /*
            while(e_k[j]&&(value(new_g, e_k[j]->thisVertex) == value(new_g, e_k[j]->otherVertex))){
                e_k[j] = e_k[j]->next;
                //printf("advanced e_k to %X  \n", e_k[j]);
            } 
            */            

           

            if(e_k[j] == NULL){
                e_k[j] = g->vert[lf_arr[j]]->edge;
                overflow = 1;
            }
            if(recur_depth == 0)
            printf("lf_arr[j]: %i  \tj: %i  \t of: %i\n", lf_arr[j], j, overflow);
            j--;
        }while(overflow && j >=0);
        if(j<0){
            if(recur_depth == 0)
            printf("combination overflow!\n");
            break;
        }

        
    }
    l_free(leafs);
    free(lf_arr);
    free(e_k);
}

