#include <stdio.h>
#include <stdlib.h>
#include "../../include/graph.h"
#include "../../include/stack.h"


//note to self: when you make the edgeset you can't store the edges as a list the way you normally would.
//              you will have to either copy the vertices or create a container.


//add null pointer catches

//add debugprints

//make sure that the free() functions are correct.

//change vertex degree when merging
//change when removing edges.

//set root needs a case completed

//remove edge has error cases that are not caught

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
   vs->parent = 0;
   vs->degree = 0;

   vs->edge = NULL;
}

//also frees any edges and their twins.
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
   g->root = 0;
   g->vert = malloc(sizeof(vertex*)*(v+1)); //vertex zero is reserved.

   for(int i = 0; i<=v; i++){
      vertex* newVertex = vertex_create(i);
      g->vert[i] = newVertex;
   }
  return g;
}

void graph_free(graph* g) {
   for(int i = 0; i<=g->original_vertex_count; i++){
      free(g->vert[i]);
   }
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
      fflush(stdout);
   }
   vertex* curVertex = origVertex;
   while(curVertex != finalVertex){ 
      curVertex->mergeValue = finalVertex->value; //update all values on the chain
      curVertex = g->vert[curVertex->mergeValue]; //not sure if this is correct.
   }
   //printf("value: %i -> %i", v, finalVertex->value);
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
      if(value(g, e->otherVertex) != v2) //edge wasnt found
         return 0;
   }
   else{
      e = ePrev->next;
   }

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

   e->next = g->vert[0]->edge;
   g->vert[0]->edge = e->next;

   ee->next = g->vert[0]->edge;
   g->vert[0]->edge = ee->next;
   
   return 1;
}



///NOTE:: CAN GO FROM O(E) -> O(1) IF WE KEEP TRACK OF LAST EDGE IN THE LIST.
//how should we handle vertex->parent? 
//V1 SUBSUMES V2
//if you are doing this, you may want to do it on another graph too.
void merge_vertices(graph* g, int v1, int v2){
   v1 = value(g, v1);
   v2 = value(g, v2);

   int big = (g->vert[v1]->degree > g->vert[v1]->degree) ? v1 : v2; //reduce how much list searching we have to do.
   int small = v1 ^ v2 ^ big;

   edge* begSmall = g->vert[small]->edge;
   edge* endSmall = begSmall; //find end of the list
   while(endSmall && endSmall->next){
      endSmall = endSmall->next;
   }

   endSmall->next = g->vert[big]->edge; //combine the lists
   g->vert[big]->edge = begSmall;

   g->vert[small]->edge=NULL; //remove the edges from the smaller vertex.

   g->vert[small]->mergeValue = big;
   
   g->vertex_count--;
}





///////////TREE FUNCTIONS/////////////

//you should probably only do this if the graph is actually a tree.
void set_root(graph* tree, int v){
   v = value(tree, v);
   if(tree->root){ //changing the root. need to reset old parents

   }
   else{
      tree->root = v;
      tree->vert[v]->parent = v; //PARENT OF ROOT IS SELF
      generate_parents(tree,v);
   }
}

void generate_parents(graph* tree, int v){
   v = value(tree, v);
   vertex* parentVertex = tree->vert[v];
   edge* e = parentVertex->edge;
   while(e){
      //we need to set parent and recurse
      int childVertex = tree->vert[e->otherVertex]->mergeValue;

      if(!tree->vert[childVertex]->parent){
         tree->vert[childVertex]->parent = v;
         generate_parents(tree, childVertex);
      }
      e = e->next;
   }
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