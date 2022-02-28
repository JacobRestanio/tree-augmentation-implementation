#include <stdio.h>
#include <stdlib.h>
#include <string.h> //included only for memset()
#include "../../include/graph.h"
#include "../../include/stack.h"


//note to self: when you make the edgeset, you can't store the edges as a list the way you normally would.
//              you will have to either copy the edges or create a container.


/*////////////items of future concern in graph.c

update comments

add null pointer catches

add debugprints

make sure that the free() functions are correct.

change vertex degree when merging
change when removing edges.

remove edge has unlikely/impossible cases that are not caught

handling parents when merging trees. what if no longer a tree?

degree includes duplicate edges

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

   vs->edge = NULL;
   vs->lastedge = NULL;
}

//also frees any edges
void vertex_free(vertex* v){
   if(v){
   edge* e = v->edge;
   edge_free(e);
   free(v);
   }
}



////////////GRAPH//////////////

graph* graph_create(int v) {
   graph* g = malloc(sizeof(*g));
   g->vertex_count = v;
   g->original_vertex_count = v;
   g->edges = 0;
   g->vert = malloc(sizeof(vertex*)*(v+1)); //vertex zero is reserved.

   for(int i = 0; i<=v; i++){
      vertex* newVertex = vertex_create(i);
      g->vert[i] = newVertex;
   }

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



///NOTE:: CAN GO FROM O(E) -> O(1) IF WE KEEP TRACK OF LAST EDGE IN THE LIST.
//how should we handle vertex->parent? 
//larger degree vertex subsumes smaller degree, v1 subsumes v2 if same degree
//if you are doing this, you may want to do it on another graph too.
void merge_vertices(graph* g, int v1, int v2){
   v1 = value(g, v1);
   v2 = value(g, v2);

   if(v1 == v2)
      return;

   g->vert[v2]->lastedge->next = g->vert[v1]->edge; //combine the lists
   g->vert[v1]->edge = g->vert[v2]->edge;

   g->vert[v2]->edge=NULL; //remove the edges from the smaller vertex.
   g->vert[v2]->lastedge=NULL;

   g->vert[v2]->mergeValue = v1;

   g->vert[v1]->degree += g->vert[v2]->degree;
   
   g->vertex_count--;
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
  return tree->parents[v];
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

// u will be the remaining vertex after merging the path.
void merge_path(graph* t, int u, int v){
   u = value(t,u);
   v = value(t,v);
   
   int u_depth = get_depth(t,u);
   int v_depth = get_depth(t,v);

   while (u!=v) {
      if(u_depth > v_depth){     //doing this with a pointer to the deeper vertex caused bugs for some reason
         int p = get_parent(t,u);
         merge_vertices(t,p,u);
         u = value(t,u);
         u_depth--;
      }
      else{
         int p = get_parent(t,v);
         merge_vertices(t,p,v);
         v = value(t,v);
         v_depth--;
      }
      
   }
}

//returns 1 if (u,v) covers (tu,tv);
int covers(graph* t, int u, int v, int tu, int tv){

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