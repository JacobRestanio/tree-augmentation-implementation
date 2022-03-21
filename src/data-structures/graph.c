#include <stdio.h>
#include <stdlib.h>
#include <string.h> //included only for memset()
#include "../../include/graph.h"
#include "../../include/int-list.h"
#include "../../include/list.h"


/*////////////items of future concern in graph.c

make sure that the free() functions are correct.

degree includes duplicate edges

combine duplicate edges when merging.

*/



///////////EDGE/////////////

edge* edge_create(int thisVertex, int otherVertex){
   edge* e = malloc(sizeof(*e));
   e->thisVertex = thisVertex;
   e->otherVertex = otherVertex;
   e->next = NULL;
   e->twin = NULL;
}

//frees the whole list
void edge_free(edge* e){
   if(e){
      edge* ee = e->next;
      free(e);
      edge_free(ee);
   }
}


///////////VERTEX/////////////

vertex* vertex_create(int v){
   vertex* vs = malloc(sizeof(*vs));
   vs->value = v;
   vs->mergeValue = v;
   vs->degree = 0;

   vs->aliases = ls_add(NULL,v);

   vs->edge = NULL;
   vs->lastedge = NULL;
}

//also frees any edges
void vertex_free(vertex* v){
   if(v){
   edge* e = v->edge;
   edge_free(e);
   if(v->mergeValue == v->value)
      ls_free(v->aliases);
   free(v);
   }
}



////////////GRAPH//////////////

graph* graph_create(int v) {
   graph* g = malloc(sizeof(*g));
   g->vertex_count = v;
   g->original_vertex_count = v;
   g->edges = 0;
   g->vert = malloc(sizeof(vertex*)*(v+1)); //vertex zero is reserved for retention.

   for(int i = 0; i<=v; i++){
      vertex* newVertex = vertex_create(i);
      g->vert[i] = newVertex;
   }

   g->retain = NULL;

   g->root = 0;
   g->parents = NULL;
   g->depths = NULL;

  return g;
}

void graph_free(graph* g) {
   for(int i = 0; i<=g->original_vertex_count; i++){
      free(g->vert[i]);
   }

   if(g->parents)
      free(g->parents);
   if(g->depths)
      free(g->depths);

   free(g);
}



/////////////GENERAL FUNCTIONS/////////////////

//integer references to vertices should be passed through this function.
//returns the correct vertex value if the input vertex was merged.
int value(graph* g, int v){ //condenses potential vertex list so that average call time is O(1)
   vertex* origVertex = g->vert[v];
   vertex* finalVertex = origVertex;
   while(finalVertex->mergeValue != finalVertex->value){ //go to the end of the chain
      finalVertex = g->vert[finalVertex->mergeValue];
   }
   vertex* curVertex = origVertex;
   while(curVertex != finalVertex){ 
      curVertex->mergeValue = finalVertex->value; //update all values on the chain
      curVertex = g->vert[curVertex->mergeValue]; //not sure if this is correct.
   }
   return finalVertex->value;
}



void add_new_edge(graph* g, int v1, int v2) {//Creates a new edge
   #ifdef DEBUGPRINT
   if(!g)
      printf("add_edge: graph is null");
   if(v1<0 || v2<0 || v1 > g->vertex_count || v2 > g-> vertex_count)
      printf("add_edge: vertex value out of bounds\n");
   #endif

   v1 = value(g, v1);
   v2 = value(g, v2);

   edge* e1 = edge_create(v1, v2);
   edge* e2=  edge_create(v2, v1);
   
   e1->next = g->vert[v1]->edge; //put the edge at the beginning of the list for each vertex
   g->vert[v1]->edge = e1;
   g->vert[v1]->degree++;

   e2->next = g->vert[v2]->edge;
   g->vert[v2]->edge = e2;
   g->vert[v2]-> degree++;

   if(g->vert[v1]->lastedge == NULL){ //update last edge
      g->vert[v1]->lastedge = e1;
   }
   if(g->vert[v2]->lastedge == NULL){
      g->vert[v2]->lastedge = e2;
   }

   e1->twin = e2; //point each node to its twin.
   e2->twin = e1;
}

void graph_add_edge(graph* g, int v1, int v2){
   add_new_edge(g, v1, v2);
}

int_ls* graph_adjacent_vertices(graph* g, int v){
   v = value(g,v);
   char* added = malloc(sizeof(char)*(1+g->vertex_count));
   memset(added,0,sizeof(char)*(1+g->vertex_count));
   added[v] = 1;
   int_ls* adj_verts = NULL;
   edge* e = g->vert[v]->edge;
   while(e){
      int cur_vert = value(g,e->otherVertex);
      if(!added[cur_vert]){
         adj_verts = ls_add(adj_verts,cur_vert);
         added[cur_vert] = 1;
      }
      e = e->next;
   }
   free(added);
   return adj_verts;
}

void retain(graph* g, edge* e){
   if(!e){
      printf("err: retain() null edge");
      return;
   }
    edge* ne = edge_create(e->thisVertex,e->otherVertex);
    ne->next = g->retain;
    g->retain = ne;
}

//twin nonsense? what if v2 and v1 are flipped? is this possible?
//returns null if no match.
edge* find_edge(graph* g, int v1, int v2){
   v1 = value(g,v1);
   v2 = value(g,v2);

   edge* e = g->vert[v1]->edge;
   while(e){
      if(v2 == value(g,e->otherVertex)){
         return e;
      }
      e = e->next;
   }
   return NULL;
}

int graph_is_edge(graph* g, int v1, int v2){
   return find_edge(g, v1, v2) != NULL;
}


//finds the edge that occurs before (v1,v2), if it exists
//returns null if not found
//returns null if the first edge in list is (v1,v2)
edge* find_prev_edge(graph* g, int v1, int v2){
   v1 = value(g,v1);
   v2 = value(g,v2);

   edge* ePrev = NULL;
   edge* e = g->vert[v1]->edge;
   while(e){
      if(v2 == value(g,e->otherVertex)){
         return ePrev;
      }
      ePrev = e;
      e = e->next;
   }
   return NULL;
}

//has a starting edge
edge* find_prev_edge_se(graph* g, edge* se, int v1, int v2){
   v1 = value(g,v1);
   v2 = value(g,v2);

   edge* ePrev = NULL;
   edge* e = se;
   while(e){
      if(v2 == value(g,e->otherVertex)){
         return ePrev;
      }
      ePrev = e;
      e = e->next;
   }
   return NULL;
}



unsigned int retain_merge_trim(graph* g, graph* t, int u, int v){

   edge* e = find_edge(g,u,v);
   if(!e)
      return 0;

   retain(g,e);
   retain(t,e);

   int_ls* path = tree_path(t,u,v);
   merge_list(t,path);
   merge_list(g,path);

   remove_self_edges(t,u);
   remove_self_edges(g,u);

   free(path);

   return e != 0;
}



//will remove ONE edge. not all edges between v1 and v2
//returns 1 if successful.
int remove_edge(graph* g, int v1, int v2) {
   //we should be careful to remove the twin of a given edge
   v1 = value(g,v1);
   v2 = value(g,v2);


   edge* ePrev = find_prev_edge(g,v1,v2);
   edge* e = NULL; //edge to remove


   //if edge is first in the list we need to change vertex->edge
   //ePrev will be null.

   if(ePrev == NULL){ //see if e is the first in the list.
      e = g->vert[v1]->edge;
      if(!e || value(g, e->otherVertex) != v2) //edge wasnt found
         return 0;
   }
   else{
      e = ePrev->next;
   }

   if(e == g->vert[v1]->lastedge)
      g->vert[v1]->lastedge = ePrev;
   

   //what if the twin is null??

   edge* eePrev = NULL;
   edge* ee = g->vert[v2]->edge; //other edge to remove

   while(ee && ee != e->twin){
      eePrev = ee;
      ee = ee->next;
   }
   //ee should now be the twin

   if(ePrev == NULL){ //is first in the list
      g->vert[v1]->edge = e->next; 
   }
   else{
      ePrev->next = e->next;
   }

   if(eePrev == NULL){
      g->vert[v2]->edge = ee->next;
   }
   else{
      eePrev->next = ee->next;
   }

   if(ee == g->vert[v2]->lastedge)
      g->vert[v2]->lastedge = eePrev;

   e->next = g->vert[0]->edge;
   g->vert[0]->edge = e->next;

   ee->next = g->vert[0]->edge;
   g->vert[0]->edge = ee->next;

   g->vert[v1]->degree--;

   g->vert[v2]->degree--;
   
   return 1;
}

int remove_self_edges(graph* g, int v){
   v = value(g,v);
   edge* le = g->vert[v]->edge;
   edge* se = find_prev_edge_se(g,le,v,v);

   while(1){
      if(se){
         edge* e_remove = se->next;
        se->next = se->next->next;
        e_remove -> next = NULL;
        edge_free(e_remove);
      }
      // if e is null check the first edge in the list
      else if (value(g,le->thisVertex)==value(g,le->otherVertex)){ 
         edge* e_next = le->next;
         if(le == g->vert[v]->edge){
            g->vert[v]->edge = e_next;
         }
         le->next = NULL;
         free(le);
         se = e_next;
      }
      else{ //no more self-loops
         return 1;
      }
      le = se;
      se = find_prev_edge_se(g,le,v,v);
   }
   return 0;
}




///NOTE:: CAN GO FROM O(E) -> O(1) IF WE KEEP TRACK OF LAST EDGE IN THE LIST.
//how should we handle vertex->parent? 
//larger degree vertex subsumes smaller degree, v1 subsumes v2 if same degree
//if you are doing this, you may want to do it on another graph too.
void merge_vertices(graph* g, int v1, int v2){

   //find all edges that are v2,v1
   //remove them
   //add them to an edgelist


   v1 = value(g, v1);
   v2 = value(g, v2);

   if(v1 == v2)
      return;

   g->vert[v2]->lastedge->next = g->vert[v1]->edge; //combine the lists
   g->vert[v1]->edge = g->vert[v2]->edge;

   g->vert[v2]->edge=NULL; //remove the edges from the smaller vertex.
   g->vert[v2]->lastedge=NULL;

   g->vert[v2]->mergeValue = v1;
   g->vert[v1]->aliases = ls_merge(g->vert[v2]->aliases, g->vert[v1]->aliases);

   g->vert[v1]->degree += g->vert[v2]->degree;
   
   g->vertex_count--;
}


void unmerge_vertices(graph* g, int v){
   v = value(g,v);
   ls_free(g->vert[v]->aliases);
   g->vert[v]->aliases = ls_add(NULL,v);
   edge* e = g->vert[v]->edge;
   edge* e_prev = NULL;
   while(e){
      edge* e_next = e->next;

      if(e->thisVertex != v){
         int u = e->thisVertex;
         g->vert[u]->mergeValue = u;

         if(!e_prev){
            g->vert[v]->edge = e_next;
         }
         e->next = g->vert[u]->edge;
         g->vert[u]->edge = e;

         g->vert[u]->degree++;
         g->vert[v]->degree--;
         e = NULL;
      }


      e_prev = (e) ? e_prev: e;
      e = e_next;
   }

}


///////////TREE FUNCTIONS/////////////


//you should probably only do this if the graph is actually a tree.
//has bug with merged graphs, unless you sanitize all vertex accesses. intended?
void set_root(graph* t, int v){
   v = value(t, v);
   if(t->root){ //changing the root. need to reset old parents
      //if there are bugs, just memset parents[] to 0;
      //update parents for edges on the path from old root to new root.
      int prev = v;
      int current = v;
      int currentParent = value(t,t->parents[current]);
      while(current!=currentParent){
         t->parents[current] = prev;

         prev = current;
         current = currentParent;
         currentParent = value(t,t->parents[current]);
      }
      t->parents[current] = prev;
      t->root = v;
   }
   else{
      if(! t->parents)
         t->parents = malloc(sizeof(int) * (t->original_vertex_count+1));
         //t->depths = malloc(sizeof(int) * (t->original_vertex_count+1));
      memset(t->parents, 0, sizeof t->parents);
      //memset(t->depths, 0, sizeof t->depths);
      t->root = v;
      t->parents[v]= v; //PARENT OF ROOT IS SELF
      //t->depths[v] = 0;
      generate_parents(t,v);
   }
}

void generate_parents(graph* t, int v){
   v = value(t, v);
   vertex* parentVertex = t->vert[v];
   edge* e = parentVertex->edge;
   while(e){
      //we need to set parent and recurse
      int childVertex = value(t,e->otherVertex);

      if(!t->parents[childVertex]){
         t->parents[childVertex] = v;
         //t->depths[childVertex] = t->depths[v]+1;
         generate_parents(t, childVertex);
      }
      e = e->next;
   }
}

int get_parent(graph* tree, int v){
  if (tree->parents == 0)
    return 0;
   v = value(tree, v);
  return value(tree,tree->parents[v]); //this originally was NOT corrected w/ value
}


//could maybe generate this when the tree is formed
int get_depth(graph* t, int v){
   v = value(t,v);
   int d = 0;
   while(get_parent(t,v) != v){
      d++;
      v = get_parent(t,v);
   }
   return d;
}

// 'u' will be the remaining vertex after merging the path.
void merge_path(graph* t, int u, int v){
   int_ls* path = tree_path(t, u, v);
   while(path->next){
      merge_vertices(t, path->value, path->next->value);
      path = path->next;
   }
   free(path);
}

void merge_list(graph* g, int_ls* vs){
   int_ls* path = vs;
   while(path->next){
      merge_vertices(g, path->value, path->next->value);
      path = path->next;
   }
}


int_ls* tree_path(graph* t, int u, int v){
   u = value(t,u);
   v = value(t,v);

   int_ls* vs = NULL;
   
   int u_depth = get_depth(t,u);
   int v_depth = get_depth(t,v);

   while (u!=v) {
      if(u_depth > v_depth){     //doing this with a pointer to the deeper vertex caused bugs for some reason
         int p = get_parent(t,u);
         vs = ls_add(vs, u);
         u = value(t,p);
         u_depth--;
      }
      else{
         int p = get_parent(t,v);
         vs = ls_add(vs, v);
         v = value(t,p);
         v_depth--;
      }
      
   }
    vs = ls_add(vs, u);
    return vs;
}

int_ls* children(graph* t, int u){
   u = value(t, u);
   edge* e = t->vert[u]->edge;
   
   int p = value(t, t->parents[u]);
   int_ls* ls = NULL;
   
   while(e){
      if(value(t, e->otherVertex) != p && (value(t, e->otherVertex) != value(t, e->thisVertex)))
         ls = ls_add(ls,e->otherVertex);
      e = e->next;
   }

   return ls;
}

// return children u + children(children u)


int_ls* d_helper(graph* t, int u){
   u = value(t, u);

   int_ls* kids = children(t, u);
   int_ls* kids_of_kids = NULL;

   int_ls* current_kid = kids;
   while(current_kid){
      int_ls* currents_kids = d_helper(t,current_kid->value);
      kids_of_kids = ls_merge(kids_of_kids,currents_kids);
      current_kid = current_kid->next;
   }
   return ls_merge(kids,kids_of_kids);
}

int_ls* descendants(graph* t, int u){
   return ls_add(d_helper(t,u), u);
}


int lca(graph* t, int u, int v){
   u = value(t,u);
   v = value(t,v);
   
   int u_depth = get_depth(t,u);
   int v_depth = get_depth(t,v);

   while (u!=v) {
      if(u_depth > v_depth){     //doing this with a pointer to the deeper vertex caused bugs for some reason
         int p = get_parent(t,u);
         u = value(t,p);
         u_depth--;
      }
      else{
         int p = get_parent(t,v);
         v = value(t,p);
         v_depth--;
      }
   }
    return u;
}




int_ls* add_leaves(graph* t, int_ls* leaves, int u){
   u = value(t,u);

   int_ls* kids = children(t, u);

   if(kids == NULL){ //is leaf
      leaves = ls_add(leaves, u);
      return leaves;
   }

   int_ls* kids_of_kids = NULL;

   int_ls* current_kid = kids;
   while(current_kid){
      leaves = add_leaves(t,leaves,current_kid->value);
      current_kid = current_kid->next;
   }
   ls_free(kids);
   return leaves;
}

//could be sped up by not depending on other function calls.
int_ls* leaves(graph* t, int u){
   int_ls* leaves = NULL;
   return add_leaves(t, leaves, u);
}


char is_leaf(graph* t, int u){
   u = value(t, u);

   int_ls* has_children = children(t,u);
   if(has_children != NULL){
      ls_free(has_children);
      return 0;
   }
   return 1;
}

int_ls* fringes(graph* t, int u){
   u = value(t,u);
   int_ls* d = descendants(t, u);
   int_ls* fringes = NULL;

   int_ls* cur_d = d;
   while(cur_d){
      if(is_fringe(t,cur_d->value)){
         fringes = ls_add(fringes,value(t,cur_d->value));
      }
      cur_d = cur_d->next;
   }
   ls_free(d);
   return fringes;
}

char is_fringe(graph* t, int u){
   int_ls* kids = children(t, u);
   int_ls* child = kids;
   if(child == NULL)
      return 0;
   while(child){
      if(!is_leaf(t,child->value)){
         ls_free(kids);
         return 0;
      }
      child = child->next;
   }
   ls_free(kids);
   return 1;
}


int_ls* pseudo_fringes(graph* g, graph* t, int  u){
   u = value(g,u);

   int_ls* pseudo_fringes = NULL;

   int_ls* fringe = fringes(t,u);
   int_ls* cur_fri = fringes(t,u);

   while(cur_fri){
      int f = value(g,cur_fri);
      int_ls* kids = children(t,f);

      //O(1) check for size 2
      if(kids->next && !kids->next->next){
         int cur_parent = get_parent(f);
         int n;
         while(cur_parent(children)){
            //get parent while parent != current
            // and while |children| = 1;
         }

      }
      
      ls_free(kids);
      cur_fri = cur_fri->next; 
   }

   ls_free(fringe);
}

// O(VE) solution. perhaps change to sort + binary search
char l_closed_nm(graph* g, graph* t, int r){
   int_ls* d = descendants(t,r);
   int_ls* l = leaves(t,r);

   int_ls* cur_leaf = l;

   while(cur_leaf){
      int v = value(t,cur_leaf->value);
      edge* cur_edge = g->vert[v]->edge;
      while(cur_edge){
         if(!ls_contains(d,value(g,cur_edge->otherVertex))){
            ls_free(d);
            ls_free(l);
            return 0;
         }
         cur_edge = cur_edge->next;
      }
      cur_leaf = cur_leaf->next;
   }
   ls_free(d);
   ls_free(l);
   return 1;
}

char l_closed_b(graph* g, graph* t, int r){
   return 1;
}


char l_closed(graph* g, graph* t, int r){
   return l_closed_nm(g,t,r);
}

//returns 1 if (u,v) covers (tu,tv);
char covers(graph* t, int u, int v, int tu, int tv){
   u = value(t,u);
   v = value(t,v);
   tv = value(t,tv);
   tu = value(t,tu);

   int_ls* path = tree_path(t, u, v);
   char covers = ls_contains_2(path, tu, tv);
   free(path);
   return covers;
}


   graph* gx = NULL;

int ls_match_edge(void* item1, void* item2){
   if(!gx)
      return 0;
   int_ls* ls1 = item1;
   edge* ls2 = item2;
   int v1 = value(gx,ls1->value);
   int v2 = value(gx,ls2->otherVertex);
   return (v1 == v2);
}


// v is a fringe vertex
int_ls* isolated(graph* g, graph* t, int parent){
   parent = value(t,parent);

   void* l_contains(void* l, int(*compare)(void*,void*),void* item);
   
   int_ls* isolated_vs = NULL;

   int_ls* desc = children(t, parent);
   int_ls* cur_desc = desc;
   while(cur_desc){
      int v = value(g,cur_desc->value);
      edge* e = g->vert[v]->edge;

      int isolated = 1;

      while(e){
         int v1 = value(g,e->thisVertex);
         int v2 = value(g,e->otherVertex);
         if(v1 != v2){
            gx = g;
            if(l_contains(desc, ls_match_edge, e)){
               isolated = 0;
               break;
            }
         }
         e = e->next;
      }
      
      if(isolated){
         isolated_vs = ls_add(isolated_vs, cur_desc->value);
      }

      cur_desc = cur_desc->next;
   }

   ls_free(desc);
   return isolated_vs;
}

//speed up with hash map
int_ls* non_redundant(graph* g, graph* t, int u){
   u = value(g,u);
   int parent = get_parent(t,u);

   char* v_added = malloc(sizeof(char)*(g->vertex_count+1));
   memset(v_added,0,sizeof(char)*(g->vertex_count+1));

   edge* e = g->vert[u]->edge;
   int_ls* non_redundant = NULL;
   while(e){
      int cur_v = value(g,e->otherVertex);
      if(cur_v != parent && cur_v != u && !v_added[cur_v]){
         non_redundant = ls_add(non_redundant,cur_v);
         v_added[cur_v] = 1;
      }
      e = e->next;
   }

   if(!non_redundant){
      non_redundant = ls_add(non_redundant,get_parent(t,u));
   }
   free(v_added);
   return non_redundant;
}

int trivial(graph* g, graph* t, int u){
   int_ls* nr = non_redundant(g,t,u);
   if(nr->next){
      free(nr);
      return 0;
   }
   int ret = nr->value;
   free(nr);
   return ret;
}



/////////////PRINT/////////////

//prints the format for CSAcademy's graph visualizer.
void graph_print(graph* g){

   for(int i = 1; i<= g->original_vertex_count; i++){
      //check if merged vertex
      vertex* currentVert = g->vert[i];
      if(currentVert->value != currentVert->mergeValue)
         continue;

      edge* e = currentVert->edge;
      while(e){
         int v1 = value(g, e->thisVertex);
         int v2 = value(g, e->otherVertex);

         printf("%i %i\n", v1, v2);
         e = e->next;
      }

   }
}

/* Creates a graph from csacademy's output format
   char* text: output text with linebreaks as '\n' */
graph* graph_create_text(char* text, int vertex_count){
   if(!text)
      return NULL;

   graph* out = graph_create(vertex_count);

   int v1 = -1;
   int v2 = -1;
   char space = 0;
   int i;

   for(i = 0; text[i]; i++){
      char c = text[i];
      if(c == '\n'){ //new line. add any complete edge and reset.
         if(v1 >= 0 && v2 >= 0){
            graph_add_edge(out, v1, v2);
         }
         v2 = v1 = -1;
         space = 0;
         continue;
      }

      if(c == ' '){
         space++;
         continue;
      }

      int* e = space ? &v2 : &v1; //select the edge that we are reading

      if(c >= '0' && c<='9'){  //adjust value of the selected edge
         if(*e > 0){
            *e *= 10;
            *e += c - '0';
         }
         else
            *e = c - '0';
      }
      else{
         printf("graph_construct_csacademy: invalid text");
         return out;
      }
   }

   if(!text[i]){
      if(v1 >= 0 && v2 >= 0){
            graph_add_edge(out, v1, v2);
         }
   }

   return out;
}