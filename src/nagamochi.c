#include <stdlib.h>
#include <stdio.h>
#include "../include/tree-helper.h"
#include "../include/graph.h"
#include <string.h>
#include "../include/list.h"


typedef struct edge_ls
{
    struct edge_ls *next;
    struct edge_ls *prev;

    edge *e;
} edge_ls;









graph *gm = NULL;

// accepts edge_ls* and edge*. match by deref
int edge_match(void *list, void *item)
{
    if (!gm)
        return 0;
    
    edge* list_e = ((edge_ls*)list)->e;
    edge* e = (edge*)item;

    int u1 = value(gm,list_e->thisVertex);
    int v1 = value(gm, list_e->otherVertex);

    int u2 = value(gm,e->thisVertex);
    int v2 = value(gm,e->otherVertex);
    
    return ((u1 == u2) && (v1 == v2)) || ((u1 == v2)  && (u2 == v1));
}

int edge_match_one(void *list, void *item)
{
    if (!gm)
        return 0;
    
    edge* list_e = ((edge_ls*)list)->e;
    edge* e = (edge*)item;

    int u1 = value(gm,list_e->thisVertex);
    int v1 = value(gm, list_e->otherVertex);

    int u2 = value(gm,e->thisVertex);
    int v2 = value(gm,e->otherVertex);
    
    return ((u1 == u2) || (v1 == v2)) || ((u1 == v2)  || (u2 == v1));
}

edge_ls *graph_adjacent_edges(graph *g, int v)
{
    v = value(g, v);
    char *added = malloc(sizeof(char) * (1 + g->original_vertex_count));
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

edge_ls* edge_ls_contains(edge_ls* el, edge* e){
    return l_contains(el, edge_match, e);
}

edge* exposed_verts(graph*g, edge* e, char* matched){
    int u = value(g,e->thisVertex);
    int v = value(g,e->otherVertex);

    if(u == v){
        return 0;
    }

    int u_matched = matched[u];
    int v_matched = matched[v];

    return (matched[u] || matched[v]) ? 0 : e;
}

int is_in_subgraph(graph* g, edge* e, char* in_subgraph){
    int u = value(g,e->thisVertex);
    int v = value(g,e->otherVertex);

    return in_subgraph[u] && in_subgraph[v];
}

//sets all used values of queue to 0
void reset_queue(int* queue, int_ls *vs){
    while(vs){
        queue[vs->value] = 0; //may need to deref value
        vs = vs->next;
    }
}

void print_edge_ls_fn(void* el){
    edge* e = ((edge_ls*)el)->e;
    printf("(%i,%i)",e->thisVertex,e->otherVertex);
}

void print_edge_ls(edge_ls* el){
    l_print(el,print_edge_ls_fn);
}

edge_ls* edge_ls_create(edge* e){
    edge_ls* nn = malloc(sizeof(edge_ls));
    nn->e = e;
    nn->next = NULL;
    nn->prev = NULL;

    return nn;
}


typedef struct pair_ls
{
    struct pair_ls *next;
    struct pair_ls *prev;

    int u;
    int v;

    int blossom_number;
} pair_ls;

pair_ls* pair_create(int u, int v, int blossom_number){
    pair_ls* ret = malloc(sizeof(pair_ls));

    ret->next = NULL;
    ret->prev = NULL;
    
    ret->u = u;
    ret->v = v;

    ret->blossom_number = blossom_number;

    return ret;
}


pair_ls* blossom_merge(graph* g, int u, int v, int blossom_number, pair_ls* merge_order){
    u = value(g,u);
    v = value(g,v);

    merge_vertices(g,u,v);

    pair_ls* new_pair = pair_create(u,v,blossom_number);

    return l_add(merge_order,new_pair);
}


pair_ls* blossom_unmerge(graph* g, pair_ls* merge_order){
    if(!merge_order)
        return NULL;

    //printf("unmerge: \n");

    int blossom_number = merge_order->blossom_number;

    while(merge_order && (merge_order->blossom_number == blossom_number)){
        //printf("bm: %i\n",merge_order->blossom_number);
        int u = value(g,merge_order->u);
        int v = merge_order->v;

        int_ls* u_alias = g->vert[u]->aliases;

        int_ls* v_aliases = g->vert[v]->aliases;

        edge* e_prev = NULL;
        edge* e = g->vert[u]->edge;

        while(e && ls_contains(v_aliases,e->thisVertex)){
            e_prev = e;
            e = e->next;
        }

        if(e_prev){
            e_prev->next = NULL;
            g->vert[v]->lastedge = e_prev;
        }

        g->vert[v]->edge = g->vert[u]->edge;
        g->vert[v]->lastedge = e_prev;

        g->vert[u]->edge = e;

        g->vert[v]->mergeValue = v;


        while(!ls_contains(v_aliases,u_alias->value)){
            u_alias = u_alias->next;
        }
        ls_free_some(u_alias);

        merge_order = l_remove(merge_order);
        }
    
    return merge_order;
}

int_ls* last_blossom_verts(pair_ls* merge_order){
    if(!merge_order)
        return NULL;
    
    int blossom_number = merge_order->blossom_number;
    int_ls* ret = ls_add(NULL,merge_order->u);

    while(merge_order && (merge_order->blossom_number == blossom_number)){
        ret = ls_add(ret,merge_order->v);

        merge_order = merge_order->next;
    }
    return ret;
}

//should lift all blossoms once the path is found
pair_ls* lift_blossom(graph* g, edge_ls* matching, int* queued_by, int* queues, char* not_exposed, pair_ls* merge_order){
    if(!merge_order || !matching || !g)
        return NULL;
    
    //printf("lifting blossom. \n");

    int old_u = value(g,merge_order->u);
    int cur_v = queues[old_u];
    int exit_node = queued_by[old_u];

    int_ls* blossom_verts = last_blossom_verts(merge_order);
    merge_order = blossom_unmerge(g, merge_order);

    int p_u = queued_by[old_u];

    edge* e = g->vert[cur_v]->edge;

    while(e && !not_exposed[value(g,e->otherVertex)] && !ls_contains(blossom_verts, value(g,e->otherVertex))){
        e = e->next;
    }
    // now e should be in the path

    old_u = value(g,e->otherVertex);
    queued_by[cur_v] = old_u;
    queues[old_u] = cur_v;

    //terminate lifting current blossom node connects to p[old_u];
    edge* temp = edge_create(old_u, exit_node);
    int chain_completed = !!l_contains(matching,edge_match,e);

    while(!chain_completed){

        int v = old_u;

        edge* e = g->vert[v]->edge;
        edge* ex = NULL;
        while(e){
            ex = l_contains(matching,edge_match,e);
            if(ex)
                break;

            e = e->next;
        }
        
        int ux = value(g,ex->thisVertex);
        int vx = value(g,ex->otherVertex);

        int u = ux == v ? vx : ux;

        queued_by[v] = u;
        queues[u] = v;

        e = g->vert[u]->edge;

        while(!ls_contains(blossom_verts,value(g,e->otherVertex))){
            e = e->next;
        }

        old_u = value(g,e->otherVertex);

        queued_by[u] = old_u;
        queues[old_u] = u;

        temp->thisVertex = old_u;
        chain_completed = !!l_contains(matching,edge_match,e);
    }

    free(temp);
    ls_free(blossom_verts);
    return merge_order;
}


edge_ls *blossom_algorithm(graph *g, int_ls *vs)
{
    gm = g;

    edge_ls* matching = NULL;

    pair_ls* merge_order = NULL;

    int vertices = ls_size(vs);

    int g_vertices = g->original_vertex_count + 1;
    int c_bytes = sizeof(char)*g_vertices;
    int i_bytes = sizeof(int)*g_vertices;

    //these are really bad and should be converted to hash-maps

    int* queued = malloc(i_bytes);
    memset(queued,0,i_bytes);

    int* queued_by = malloc(i_bytes);
    memset(queued,0,i_bytes);

    int* queues = malloc(i_bytes);
    memset(queued,0,i_bytes);

    char* in_subgraph = malloc(c_bytes);
    memset(in_subgraph,0,c_bytes);

    char* not_exposed = malloc(c_bytes);
    memset(in_subgraph,0,c_bytes);

    int* in_blossom = malloc(i_bytes);
    memset(queued,0,i_bytes);

    //set all verts to be in subgraph.
    int_ls* cur_v = vs;
    while(cur_v){
        int v = value(g,cur_v->value);
        in_subgraph[v] = 1;
        cur_v = cur_v->next;
    }

    int blossom_count = 0;
    int non_aug_checks = 0; //counts how many vertices have been checked without augmenting the graph.
    cur_v = vs;
    
/*
    matching = l_add(matching, edge_ls_create(edge_create(6,8)));
     matching = l_add(matching, edge_ls_create(edge_create(5,13)));
      matching = l_add(matching, edge_ls_create(edge_create(7,9)));
        cur_v = ls_contains(vs,14);
      not_exposed[6] = 1;
    not_exposed[8] = 1;
    not_exposed[5] = 1;
    not_exposed[13] = 1;
    not_exposed[7] = 1;
    not_exposed[9] = 1;
*/



    while(non_aug_checks < vertices){
        reset_queue(queued,vs); //slow, resets queued verts.
        reset_queue(queued_by,vs);
        reset_queue(queues,vs);

        int root = value(g,cur_v->value);


        if(!not_exposed[root]){ //start BFS from unmatched verticies
            int break_flag = 0;

            /*printf("\n\nmatching: ");
            fflush(stdout);
            print_edge_ls(matching);
            printf("\n");
            printf("starting from: %i\n", root);
            printf("non_aug: %i\n", non_aug_checks);
            fflush(stdout);*/

            int_ls* queue = ls_add(NULL, root); //start queue
            int_ls* cur_queue = queue;
            int_ls* last_queue = queue;
            queued[root] = 1;
            
            int prev_u = 0;

            while(cur_queue){
                int q_u = value(g,cur_queue->value); //current v
                edge* e = g->vert[q_u]->edge;

                int current_depth = queued[q_u]+1;

                int looking_for_matched = current_depth%2;

                //printf("u:%i \tcurrent depth: %i\n", q_u, current_depth);fflush(stdout);
                while(e){ //go through each edge in current vertex
                        int u = value(g,e->thisVertex); // should be the same as q_u
                        int v = value(g,e->otherVertex);

                    if(is_in_subgraph(g,e,in_subgraph) && u!=v){

                        int e_in_matching = !!l_contains(matching,edge_match,e);
                        //printf("%i,%i :\t looking: %i\tin_m:%i\tq[v]mod2 = %i\n",u,v,looking_for_matched,e_in_matching,queued[v]%2);fflush(stdout);

                        
                        
                        if(exposed_verts(g,e,not_exposed)){  //add to matching
                            edge_ls* new_edge_ls = edge_ls_create(e);
                            matching = l_add(matching,new_edge_ls);
                            not_exposed[u] = 1;
                            not_exposed[v] = 1;

                            non_aug_checks = -1; //reset counter
                            break_flag = 1;
                            break;
                        }
                        

                        if(looking_for_matched && e_in_matching){
                            if(!queued[v]){                   //add to queue
                                //printf("2. queueing %i ", v);fflush(stdout);
                                //ls_print(cur_queue);

                                int_ls* new_node = ls_add(NULL,v);
                                ls_merge(last_queue,new_node);
                                last_queue = new_node;
                                queued[v] = current_depth;
                                queued_by[v] = q_u;
                                queues[q_u] = v;

                                //printf("  ->  ");fflush(stdout);
                                //ls_print(cur_queue);
                                //printf("\n");fflush(stdout);
                            }
                            break;//can break because this should be the only one.
                        }
                        else if(!looking_for_matched && !e_in_matching && !queued[v]%2){
                            //printf("!looking & !in mathcing\n");fflush(stdout);
                            
                            if(!not_exposed[v]){ //flip edges
                              //  printf("exposed!\n");fflush(stdout);
                                int cur_v = v;
                                int old_u = u;
                                int p_u = queued_by[old_u];

                                //lift all blossoms
                                while(merge_order = lift_blossom(g, matching,queued_by,queues,not_exposed,merge_order));
                                    
                                do{
                                    //printf("\t flipping:\t cur_v: %i \t old_u: %i\t p_u: %i\n",cur_v,old_u,p_u); fflush(stdout);
                                    edge* temp_e = edge_create(old_u,cur_v); //edge struct for matching list

                                    edge* ee = find_edge(g,old_u,cur_v);
                                    edge_ls* new_node = edge_ls_create(ee);

                                    matching = l_add(matching,new_node);
                                    not_exposed[old_u] = 1;
                                    not_exposed[cur_v] = 1;

                                    free(temp_e);

                                    if(p_u){
                                        edge* temp_e2 = edge_create(old_u, p_u);

                                        edge_ls* rm = l_contains(matching,edge_match,temp_e2); //remove edge
                                        l_remove(rm);
                                    }

                                    cur_v = p_u;
                                    old_u = queued_by[p_u];
                                    p_u = queued_by[old_u];
                                }while(old_u);
                                non_aug_checks = -1; //reset counter
                                break_flag = 1;
                                break;
                            }
                            if(!queued[v]){                   //add to queue
                                //printf("3. queueing %i ", v);fflush(stdout);
                                //ls_print(cur_queue);

                                int_ls* new_node = ls_add(NULL,v);
                                ls_merge(last_queue,new_node);
                                last_queue = new_node;
                                queued[v] = current_depth;
                                queued_by[v] = q_u;
                                queues[q_u] = v;
                                //printf("  ->  ");fflush(stdout);
                                //ls_print(cur_queue);
                                //printf("\n");fflush(stdout);
                            }
                        }else if(!looking_for_matched && queued[v]%2){ //blossom found
                                blossom_count++;

                                //backtrack and merge until du and dv meet
                                //printf("!!!!CONTRACTING BLOSSOM\n");
                                in_blossom[u] = blossom_count;
                                in_blossom[v] = blossom_count;

                                while(u != v){
                                    int du = queued[u]; // one end of blossom
                                    int dv = queued[v]; // other end of blossom

                                    if(du > dv){
                                        
                                        int u_prev = queued_by[u];
                                        blossom_merge(g,u,u_prev,blossom_count,merge_order);
                                        u = u_prev;
                                        in_blossom[u_prev] = blossom_count;
                                    }
                                    else{
                                        int v_prev = queued_by[v];
                                        blossom_merge(g,v,v_prev,blossom_count,merge_order);
                                        v = v_prev;
                                        in_blossom[v_prev] = blossom_count;
                                    }
                                }
                                
                                //restart algo from same vert.
                                cur_v = cur_v->prev ? cur_v->prev : ls_last(cur_v);
                                break_flag = 1;
                                break;
                                
                        }
                        else{ // may need to add a flag to see if the vert has changed
                            while(merge_order = blossom_unmerge(g,merge_order)); //unmerge all blossoms
                            reset_queue(in_blossom,vs);
                            blossom_count = 0;
                            non_aug_checks--;
                        }
                    }
                    e = e->next;
                } // end while(e)
                if(break_flag)
                    break;
                prev_u = q_u;
                cur_queue = cur_queue->next;
            } // end while(cur_queue){
            ls_free(queue);
        }// end if(!matched[root])
        non_aug_checks++;
        cur_v = cur_v->next ? cur_v->next : vs;
    } // end while (non_aug_checks < vertices)

    free(queued);
    free(queued_by);
    free(queues);
    free(in_subgraph);
    free(not_exposed);
    free(in_blossom);
    return matching;
}

int case1(graph *g, graph *t)
{

    int ret = 0;
    int_ls *fringe = fringes(t, t->root);
    int_ls *cur_fringe = fringe;
    while (cur_fringe)
    {
        int v = value(t, cur_fringe->value);
        if (l_closed(g, t, v))
        {
            int_ls* des = descendants(t,v);

            edge_ls* e_retain = blossom_algorithm(g,des);

            edge_ls* cur_e = e_retain;

            while(cur_e){
                retain_merge_trim(g,t,cur_e->e->thisVertex, cur_e->e->otherVertex);

                cur_e = cur_e->next;
            }

            graph_print_all(g);

            int_ls* remaining_des = descendants(t,v);
            int_ls* cur_des = remaining_des;
            printf("remaining des %i ",v); ls_print(remaining_des); printf("\n");
            printf("-----\n");
            while(cur_des){
                int u = value(g,cur_des->value);
                if(u == v){
                    cur_des = cur_des->next;
                    continue;
                }
                printf("(((u: %i\t",u);
                fflush(stdout);
                int u2 = g->vert[u]->edge->otherVertex;
                printf("u2: %i\t))))\n",u2);
                retain_merge_trim(g,t,u,u2);

                printf("v4: %i\n",g->vert[4]->edge->otherVertex);
                
                graph_print_all(g);

                cur_des = cur_des->next;
            }

            ls_free(des);
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


// computes maximum matching


edge *nagamochi(graph *g, graph *t, double approx)
{

    // at the very very end, we may have to check that each edge is covered,
    // p4 doesnt seem to guarantee covering an edge when contracting f'
    // apparently we can select any edge that covers it??/???

    // this may not work. test for short circuit evaluation.
    while (case1(g, t) ^ 2 * case2(g, t) ^ 4 * case3(g, t) ^ 8 * case4(g, t));

    return NULL;
}


//should lift all blossoms once the path is found
pair_ls* lift_blossom2(graph* g, edge_ls* matching, int* queued_by, int* queues, char* not_exposed, edge_ls* es, pair_ls* merge_order){
    if(!merge_order || !matching || !g)
        return NULL;
    
    //printf("lifting blossom. \n");

    int old_u = value(g,merge_order->u);
    int cur_v = queues[old_u];
    int exit_node = queued_by[old_u];

    int_ls* blossom_verts = last_blossom_verts(merge_order);
    merge_order = blossom_unmerge(g, merge_order);

    int p_u = queued_by[old_u];

    edge* e = g->vert[cur_v]->edge;

    while(e && edge_ls_contains(es,e) && !not_exposed[value(g,e->otherVertex)] && !ls_contains(blossom_verts, value(g,e->otherVertex))){
        e = e->next;
    }
    // now e should be in the path

    old_u = value(g,e->otherVertex);
    queued_by[cur_v] = old_u;
    queues[old_u] = cur_v;

    //terminate lifting current blossom node connects to p[old_u];
    edge* temp = edge_create(old_u, exit_node);
    int chain_completed = !!l_contains(matching,edge_match,e);

    while(!chain_completed){

        int v = old_u;

        edge* e = g->vert[v]->edge;
        edge* ex = NULL;
        while(e){
            ex = l_contains(matching,edge_match,e);
            if(ex)
                break;

            e = e->next;
        }
        
        int ux = value(g,ex->thisVertex);
        int vx = value(g,ex->otherVertex);

        int u = ux == v ? vx : ux;

        queued_by[v] = u;
        queues[u] = v;

        e = g->vert[u]->edge;

        while(!ls_contains(blossom_verts,value(g,e->otherVertex)) && !edge_ls_contains(es,e)){
            e = e->next;
        }

        old_u = value(g,e->otherVertex);

        queued_by[u] = old_u;
        queues[old_u] = u;

        temp->thisVertex = old_u;
        chain_completed = !!l_contains(matching,edge_match,e);
    }

    free(temp);
    ls_free(blossom_verts);
    return merge_order;
}


edge_ls *blossom_algorithm2(graph *g, int_ls *vs, edge_ls* es)
{
    gm = g;

    edge_ls* matching = NULL;

    pair_ls* merge_order = NULL;

    int vertices = ls_size(vs);

    int g_vertices = g->original_vertex_count + 1;
    int c_bytes = sizeof(char)*g_vertices;
    int i_bytes = sizeof(int)*g_vertices;

    //these are really bad and should be converted to hash-maps

    int* queued = malloc(i_bytes);
    memset(queued,0,i_bytes);

    int* queued_by = malloc(i_bytes);
    memset(queued,0,i_bytes);

    int* queues = malloc(i_bytes);
    memset(queued,0,i_bytes);

    char* in_subgraph = malloc(c_bytes);
    memset(in_subgraph,0,c_bytes);

    char* not_exposed = malloc(c_bytes);
    memset(in_subgraph,0,c_bytes);

    int* in_blossom = malloc(i_bytes);
    memset(queued,0,i_bytes);

    //set all verts to be in subgraph.
    int_ls* cur_v = vs;
    while(cur_v){
        int v = value(g,cur_v->value);
        in_subgraph[v] = 1;
        cur_v = cur_v->next;
    }

    int blossom_count = 0;
    int non_aug_checks = 0; //counts how many vertices have been checked without augmenting the graph.
    cur_v = vs;
    
/*
    matching = l_add(matching, edge_ls_create(edge_create(6,8)));
     matching = l_add(matching, edge_ls_create(edge_create(5,13)));
      matching = l_add(matching, edge_ls_create(edge_create(7,9)));
        cur_v = ls_contains(vs,14);
      not_exposed[6] = 1;
    not_exposed[8] = 1;
    not_exposed[5] = 1;
    not_exposed[13] = 1;
    not_exposed[7] = 1;
    not_exposed[9] = 1;
*/

    while(non_aug_checks < vertices){
        reset_queue(queued,vs); //slow, resets queued verts.
        reset_queue(queued_by,vs);
        reset_queue(queues,vs);

        int root = value(g,cur_v->value);


        if(!not_exposed[root]){ //start BFS from unmatched verticies
            int break_flag = 0;

            /*printf("\n\nmatching: ");
            fflush(stdout);
            print_edge_ls(matching);
            printf("\n");
            printf("starting from: %i\n", root);
            printf("non_aug: %i\n", non_aug_checks);
            fflush(stdout);*/

            int_ls* queue = ls_add(NULL, root); //start queue
            int_ls* cur_queue = queue;
            int_ls* last_queue = queue;
            queued[root] = 1;
            
            int prev_u = 0;

            while(cur_queue){
                int q_u = value(g,cur_queue->value); //current v
                edge* e = g->vert[q_u]->edge;

                int current_depth = queued[q_u]+1;

                int looking_for_matched = current_depth%2;

                //printf("u:%i \tcurrent depth: %i\n", q_u, current_depth);fflush(stdout);
                while(e){ //go through each edge in current vertex
                        int u = value(g,e->thisVertex); // should be the same as q_u
                        int v = value(g,e->otherVertex);

                    if(is_in_subgraph(g,e,in_subgraph) && u!=v && edge_ls_contains(es,e)){

                        int e_in_matching = !!l_contains(matching,edge_match,e);
                        //printf("%i,%i :\t looking: %i\tin_m:%i\tq[v]mod2 = %i\n",u,v,looking_for_matched,e_in_matching,queued[v]%2);fflush(stdout);

                        
                        
                        if(exposed_verts(g,e,not_exposed)){  //add to matching
                            edge_ls* new_edge_ls = edge_ls_create(e);
                            matching = l_add(matching,new_edge_ls);
                            not_exposed[u] = 1;
                            not_exposed[v] = 1;

                            non_aug_checks = -1; //reset counter
                            break_flag = 1;
                            break;
                        }
                        

                        if(looking_for_matched && e_in_matching){
                            if(!queued[v]){                   //add to queue
                                //printf("2. queueing %i ", v);fflush(stdout);
                                //ls_print(cur_queue);

                                int_ls* new_node = ls_add(NULL,v);
                                ls_merge(last_queue,new_node);
                                last_queue = new_node;
                                queued[v] = current_depth;
                                queued_by[v] = q_u;
                                queues[q_u] = v;

                                //printf("  ->  ");fflush(stdout);
                                //ls_print(cur_queue);
                                //printf("\n");fflush(stdout);
                            }
                            break;//can break because this should be the only one.
                        }
                        else if(!looking_for_matched && !e_in_matching && !queued[v]%2){
                            //printf("!looking & !in mathcing\n");fflush(stdout);
                            
                            if(!not_exposed[v]){ //flip edges
                              //  printf("exposed!\n");fflush(stdout);
                                int cur_v = v;
                                int old_u = u;
                                int p_u = queued_by[old_u];

                                //lift all blossoms
                                while(merge_order = lift_blossom2(g, matching,queued_by,queues,not_exposed,merge_order));
                                    
                                do{
                                    //printf("\t flipping:\t cur_v: %i \t old_u: %i\t p_u: %i\n",cur_v,old_u,p_u); fflush(stdout);
                                    edge* temp_e = edge_create(old_u,cur_v); //edge struct for matching list

                                    edge* ee = find_edge(g,old_u,cur_v);
                                    edge_ls* new_node = edge_ls_create(ee);

                                    matching = l_add(matching,new_node);
                                    not_exposed[old_u] = 1;
                                    not_exposed[cur_v] = 1;

                                    free(temp_e);

                                    if(p_u){
                                        edge* temp_e2 = edge_create(old_u, p_u);

                                        edge_ls* rm = l_contains(matching,edge_match,temp_e2); //remove edge
                                        l_remove(rm);
                                    }

                                    cur_v = p_u;
                                    old_u = queued_by[p_u];
                                    p_u = queued_by[old_u];
                                }while(old_u);
                                non_aug_checks = -1; //reset counter
                                break_flag = 1;
                                break;
                            }
                            if(!queued[v]){                   //add to queue
                                //printf("3. queueing %i ", v);fflush(stdout);
                                //ls_print(cur_queue);

                                int_ls* new_node = ls_add(NULL,v);
                                ls_merge(last_queue,new_node);
                                last_queue = new_node;
                                queued[v] = current_depth;
                                queued_by[v] = q_u;
                                queues[q_u] = v;
                                //printf("  ->  ");fflush(stdout);
                                //ls_print(cur_queue);
                                //printf("\n");fflush(stdout);
                            }
                        }else if(!looking_for_matched && queued[v]%2){ //blossom found
                                blossom_count++;

                                //backtrack and merge until du and dv meet
                                //printf("!!!!CONTRACTING BLOSSOM\n");
                                in_blossom[u] = blossom_count;
                                in_blossom[v] = blossom_count;

                                while(u != v){
                                    int du = queued[u]; // one end of blossom
                                    int dv = queued[v]; // other end of blossom

                                    if(du > dv){
                                        
                                        int u_prev = queued_by[u];
                                        blossom_merge(g,u,u_prev,blossom_count,merge_order);
                                        u = u_prev;
                                        in_blossom[u_prev] = blossom_count;
                                    }
                                    else{
                                        int v_prev = queued_by[v];
                                        blossom_merge(g,v,v_prev,blossom_count,merge_order);
                                        v = v_prev;
                                        in_blossom[v_prev] = blossom_count;
                                    }
                                }
                                
                                //restart algo from same vert.
                                cur_v = cur_v->prev ? cur_v->prev : ls_last(cur_v);
                                break_flag = 1;
                                break;
                                
                        }
                        else{ // may need to add a flag to see if the vert has changed
                            while(merge_order = blossom_unmerge(g,merge_order)); //unmerge all blossoms
                            reset_queue(in_blossom,vs);
                            blossom_count = 0;
                            non_aug_checks--;
                        }
                    }
                    e = e->next;
                } // end while(e)
                if(break_flag)
                    break;
                prev_u = q_u;
                cur_queue = cur_queue->next;
            } // end while(cur_queue){
            ls_free(queue);
        }// end if(!matched[root])
        non_aug_checks++;
        cur_v = cur_v->next ? cur_v->next : vs;
    } // end while (non_aug_checks < vertices)

    free(queued);
    free(queued_by);
    free(queues);
    free(in_subgraph);
    free(not_exposed);
    free(in_blossom);
    return matching;
}

typedef struct edge_ls_ls
{
    void* x;
} edge_ls_ls;



// v must be minimally lf-closed s
void COVER(graph* g, graph* t, int v){

    edge_ls* F_leaf = leaf_edges(g,t,v); //leaf edges in T[v] - edges that connect a leaf to an ancestor

    edge_ls* E_leaf = leaf_to_leaf_edges(g,t,v); //edges that connect two leaves

    edge_ls* E_prime1 = prime_edge_type1(g,t,v);

    edge_ls* E_prime2 = prime_edges_type2(g,t,v);

    edge_ls* E_prime = ls_merge(ls_copy(E_prime1), ls_copy(E_prime2)); //prime edges

    //for each swing edge g in a given chain of T[v]
        edge_ls* gg; // each swing edge in the lower part of the chain
        edge_ls* e_g; //binding edges corresponding to 'g' (possible also for each chain. ambiguous)
        //for each chain P
            edge* e_p; //upper edge of P
    edge_ls_ls* E_bind; //set of all e_g's
    edge_ls_ls* E_upper; //set of all e_p's

    //for each branch vertex in T[v]
        int vi; //current branch vertex
/*     E_upper(vi) set of upper edges e_p of -chains of vi-  */


    /* PHASE 1 - PHASE 1 - PHASE 1 - PHASE 1 - PHASE 1 - PHASE 1*/
        int_ls* vs = leaves(t,v);
        edge_ls* e_mat = l_remove_ls(leaves,edge_match,E_prime);

        edge_ls* m_star = blossom_algorithm2(g,vs,e_mat); //this list will be retained

        edge_ls* matched_v = NULL;

        for(edge_ls* m = m_star; m; m=m->next){
            int u1 = value(g,m->e->thisVertex);
            int u2 = value(g,m->e->otherVertex);
            
            ls_add(matched_v,u1);
            ls_add(matched_v,u2);
        }

        int_ls* unmatched_v = ls_copy(vs); ls_remove_list(unmatched_v,matched_v);

        edge_ls* M1_s = NULL;
        edge_ls* M2_s = NULL;

        int_ls* newly_matched_v = NULL;

        for(edge_ls* e1 = E_prime1; e1; e1 = e1->next){
            int u1 = value(g,e1->e->thisVertex);
            int u2 = value(g,e1->e->otherVertex);

            if(ls_contains_2(unmatched_v,u1,u2)){
                M1_s = l_add(M1_s, edge_ls_create(e1->e)); //this list will be retained
                newly_matched_v = ls_add(newly_matched_v,u1);
                newly_matched_v = ls_add(newly_matched_v,u2);
            }
        }

        for(edge_ls* e2 = E_prime2; e2; e2 = e2->next){
            int u1 = value(g,e2->e->thisVertex);
            int u2 = value(g,e2->e->otherVertex);

            if(ls_contains_2(unmatched_v,u1,u2))
                M2_s = l_add(M1_s, edge_ls_create(e2->e));
        }

        edge_ls* m2_d = NULL; //m2_d should be size m1_d/2
        for(edge_ls* m = M1_s; m; m = m->next){
            m2_d = ls_add(m2_d, l_contains(M2_s,edge_match_one, m->e)); //this list will be retained

            int u1 = value(g,m->e->thisVertex);
            int u2 = value(g,m->e->otherVertex);
            newly_matched_v = ls_add(newly_matched_v,u1);
            newly_matched_v = ls_add(newly_matched_v,u2);
        }

        ls_remove_list(unmatched_v,matched_v); //remove the newly matched verts from unmatched.

        //for each unmatched_v, w,
            //if has binding edge where w is the higher vert, and w has no other binding edges
                //'ew' = bind_edge

           //else if w is incident to a swing
                //'ew' = swing
            //else
                //'ew' = high(w)

        //retain all 'ew'




    /* PHASE 2 - PHASE 2 - PHASE 2 - PHASE 2 - PHASE 2 - PHASE 2*/


    /* PHASE 3 - PHASE 3 - PHASE 3 - PHASE 3 - PHASE 3 - PHASE 3*/

}

typedef struct chain_ls{
    chain_ls* next;
    chain_ls* prev;

    int u ;
    int uk;

    int ua;

    edge* e_p; //upper
}chain_ls;

chain_ls* create_chain(int u,int uk){
    chain_ls* chain = malloc(sizeof(chain_ls));
    chain->next = NULL;
    chain->prev = NULL;

    int u;

    edge* e_p = NULL; 
}


edge_ls* prime_edges(graph* g, graph* t, int v){
   int u = value(g, v);


    edge_ls* prime_edges = NULL;

   int_ls *fringe = fringes(t, u);
   int_ls *cur_fri = fringes(t, u);

   while (cur_fri)
   {
      int f = value(g, cur_fri->value);
      int_ls *kids = children(t, f);

      if (kids->next && !kids->next->next)
      { // number of kids is two
         //printf("%i: number of kids is two\n", f);
         int u1 = kids->value;
         int u2 = kids->next->value;
         edge* p1 = NULL;
         if (p1 = find_edge(g,u1,u2))
         { // kids are connected, prime edge type 1
            prime_edges = l_add(prime_edges, edge_ls_create(p1));
           //printf("%i: u1(%i) and u2(%i)  are connected\n", f, u1, u2);
            int cur_parent = get_parent(t, f);
            int n;
            while (cur_parent)
            {
             //  printf("%i (%i): climbing parents\n", f, cur_parent);
               int_ls *cur_kids = children(t, cur_parent);
               if (cur_kids->next && !cur_kids->next->next)
               { // two children
               //   printf("%i (%i): <-- potential p_fringe\n", f, cur_parent);
                  int p_fringe_candidate = cur_parent;
                  cur_parent = 0; // stop while loop

                  // make u3 the leaf hanging from p_fringe
                  int u3 = is_leaf(t, cur_kids->value);
                  u3 = u3 ? u3 : is_leaf(t, cur_kids->next->value);
                 // printf("%i : u3 is %i\n", f, u3);
                  if (u3)
                  {
                    edge* p21 = NULL;
                    edge* p22 = NULL;
                     if ((p21 = find_edge(g, u1, u3)) && (p22 = find_edge(g, u2, u3)))
                     { // prime edge type 2
                   //     printf("%i : prime edges of type 2 exist\n", f);
                        int_ls *desc_v = descendants(t, p_fringe_candidate);

                        edge *e = g->vert[u1]->edge;

                        int u1_or_u2_bad_edge = 0;

                        while (e)
                        {
                           int other_v = value(g, e->otherVertex);
                           void *c = ls_contains(desc_v, other_v);
                           if (!c)
                           {
                              u1_or_u2_bad_edge = 1;
                              break;
                           }
                           e = e->next;
                        }

                        e = g->vert[u2]->edge;

                        while (e)
                        {
                           int other_v = value(g, e->otherVertex);
                           void *c = ls_contains(desc_v, other_v);
                           if (!c)
                           {
                              u1_or_u2_bad_edge = 1;
                              break;
                           }
                           e = e->next;
                        }
                        if (!u1_or_u2_bad_edge)
                        {
                            prime_edges = l_add(prime_edges, edge_ls_create(p21));
                            prime_edges = l_add(prime_edges, edge_ls_create(p22));
                        }
                        ls_free(desc_v);
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
   return prime_edges;
}

edge_ls* prime_edges_type1(graph* g, graph* t, int v){
       int u = value(g, v);


    edge_ls* prime_edges = NULL;

   int_ls *fringe = fringes(t, u);
   int_ls *cur_fri = fringes(t, u);

   while (cur_fri)
   {
      int f = value(g, cur_fri->value);
      int_ls *kids = children(t, f);

      if (kids->next && !kids->next->next)
      { // number of kids is two
         //printf("%i: number of kids is two\n", f);
         int u1 = kids->value;
         int u2 = kids->next->value;
         edge* p1 = NULL;
         if (p1 = find_edge(g,u1,u2))
         { // kids are connected, prime edge type 1
            prime_edges = l_add(prime_edges, edge_ls_create(p1));
         }
      }
      ls_free(kids);
      cur_fri = cur_fri->next;
   }
   ls_free(fringe);
   return prime_edges;
}

edge_ls* prime_edges_type2(graph* g, graph* t, int v){
       int u = value(g, v);


    edge_ls* prime_edges = NULL;

   int_ls *fringe = fringes(t, u);
   int_ls *cur_fri = fringes(t, u);

   while (cur_fri)
   {
      int f = value(g, cur_fri->value);
      int_ls *kids = children(t, f);

      if (kids->next && !kids->next->next)
      { // number of kids is two
         //printf("%i: number of kids is two\n", f);
         int u1 = kids->value;
         int u2 = kids->next->value;
         edge* p1 = NULL;
         if (p1 = find_edge(g,u1,u2))
         { // kids are connected, prime edge type 1
            //prime_edges = l_add(prime_edges, edge_ls_create(p1));
           //printf("%i: u1(%i) and u2(%i)  are connected\n", f, u1, u2);
            int cur_parent = get_parent(t, f);
            int n;
            while (cur_parent)
            {
             //  printf("%i (%i): climbing parents\n", f, cur_parent);
               int_ls *cur_kids = children(t, cur_parent);
               if (cur_kids->next && !cur_kids->next->next)
               { // two children
               //   printf("%i (%i): <-- potential p_fringe\n", f, cur_parent);
                  int p_fringe_candidate = cur_parent;
                  cur_parent = 0; // stop while loop

                  // make u3 the leaf hanging from p_fringe
                  int u3 = is_leaf(t, cur_kids->value);
                  u3 = u3 ? u3 : is_leaf(t, cur_kids->next->value);
                 // printf("%i : u3 is %i\n", f, u3);
                  if (u3)
                  {
                    edge* p21 = NULL;
                    edge* p22 = NULL;
                     if ((p21 = find_edge(g, u1, u3)) && (p22 = find_edge(g, u2, u3)))
                     { // prime edge type 2
                   //     printf("%i : prime edges of type 2 exist\n", f);
                        int_ls *desc_v = descendants(t, p_fringe_candidate);

                        edge *e = g->vert[u1]->edge;

                        int u1_or_u2_bad_edge = 0;

                        while (e)
                        {
                           int other_v = value(g, e->otherVertex);
                           void *c = ls_contains(desc_v, other_v);
                           if (!c)
                           {
                              u1_or_u2_bad_edge = 1;
                              break;
                           }
                           e = e->next;
                        }

                        e = g->vert[u2]->edge;

                        while (e)
                        {
                           int other_v = value(g, e->otherVertex);
                           void *c = ls_contains(desc_v, other_v);
                           if (!c)
                           {
                              u1_or_u2_bad_edge = 1;
                              break;
                           }
                           e = e->next;
                        }
                        if (!u1_or_u2_bad_edge)
                        {
                            prime_edges = l_add(prime_edges, edge_ls_create(p21));
                            prime_edges = l_add(prime_edges, edge_ls_create(p22));
                        }
                        ls_free(desc_v);
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
   return prime_edges;
}

edge_ls* leaf_edges(graph* g, graph* t, int v){
    v = value(t,v);

    edge_ls* leaf_edges = NULL;

    int_ls* leafs = leaves(t,v);

    for(int_ls* leaf = leafs; leaf; leaf = leaf->next){
        int cur_lf = value(g,leaf->value);

        int_ls* parent_ls = NULL;
        
        for(int cur_v = get_parent(t,cur_lf); cur_v != get_parent(t,v); cur_v = get_parent(t,cur_v)){
            parent_ls = ls_add(parent_ls,cur_v);
        }

        printf("parent list (%i): ",cur_lf); ls_print(parent_ls); printf("\n");

        for(edge* e = g->vert[cur_lf]->edge; e ; e=e->next){
            int other_v = value(g,e->otherVertex);

            if(ls_contains(parent_ls,other_v)){
                leaf_edges = l_add(leaf_edges, edge_ls_create(e));
            }
        }
        ls_free(parent_ls);
    }

    ls_free(leafs);
    return leaf_edges;
}

edge_ls* leaf_to_leaf_edges(graph* g, graph*t, int v){
    v = value(t,v);

    edge_ls* leaf_edges = NULL;

    int_ls* leafs = leaves(t,v);

    for(int_ls* leaf = leafs; leaf; leaf = leaf->next){
        int cur_lf = value(g,leaf->value);

        for(edge* e = g->vert[cur_lf]->edge; e ; e=e->next){
            int other_v = value(g,e->otherVertex);

            if(is_leaf(t,other_v) && !edge_ls_contains(leaf_edges,e)){ //to assert lf-closed change this to ls_contains(leafs, other_v);

                leaf_edges = l_add(leaf_edges, edge_ls_create(e));
            }
        }
    }
    ls_free(leafs);

    return leaf_edges;
}