#include <stdio.h>
#include <stdlib.h>
#include "../../include/graph.h"
#include "../../include/stack.h"


//note to self: when you make the edgeset you can't store the edges as a list the way you normally would.
//              you will have to either copy the vertices or create a container.


//add null pointer catches

////////////////////
//EDGE
////////////////////
typedef struct Edge {
   int thisVertex;
   int otherVertex;

   struct Edge* next;
   struct Edge* twin;
} edge;

edge* edge_create(int thisVertex, int otherVertex){
   edge* e = malloc(sizeof(*e));
   e->thisVertex = thisVertex;
   e->otherVertex = otherVertex;
   e->next = NULL;
   e->twin = NULL;
}

///////////////////////
//VERTEX
///////////////////////
typedef struct Vertex {
  int value;
  int mergeValue;
  int parent; //used only if the graph is a tree
  int degree;

  edge* edge;
} vertex;

vertex* vertex_create(int v){
   vertex* vs = malloc(sizeof(*vs));
   vs->value = v;
   vs->mergeValue = v;
   vs->parent = 0;
   vs->degree = 0;

   vs->edge = NULL;
}


/////////////////////
//GRAPH
/////////////////////
typedef struct Graph {
  int vertex_count;
  int edges;
  
  vertex* root; //used only if the graph is a tree
  vertex** vert;
} graph;


graph* graph_create(int v) {
   graph* g = malloc(sizeof(*g));
   g->vertex_count = v;
   g->edges = 0;
   g->root = NULL;
   g->vert = malloc(sizeof(vertex*)*(v+1)); //vertex zero is empty space.

   for(int i = 0; i<v; i++){
      vertex* newVertex = vertex_create(i);
      g->vert[i] = newVertex;
   }
  return g;
}


///////////////////////////
//TREE
///////////////////////////


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
      int childVertex = tree->vert[e->otherVertex];

      if(!tree->vert[childVertex]->parent){
         tree->vert[childVertex]->parent = v;
         generate_parents(tree, childVertex);
      }
      e = e->next;
   }
}


////////////////////

int value(graph* g, int v){ //condenses potential vertex list
   vertex* origVertex = g->vert[v];
   vertex* finalVertex = origVertex;
   while(finalVertex->mergeValue != finalVertex->value){ //go to the end of the chain
      finalVertex = g->vert[finalVertex->mergeValue];
   }
   vertex* curVertex = origVertex;
   while(curVertex != finalVertex){ 
      curVertex->mergeValue = finalVertex->value; //update all values on the chain
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

   edge* e1 = edge_create(v1, v2);
   edge* e2=  edge_create(v2, v1);
   
   e1->next = g->vert[v1]->edge; //put the edge at the beginning of the list for each vertex
   g->vert[v2]->edge = e1;
   g->vert[v1]->degree++;

   e2->next = g->vert[v2]->edge;
   g->vert[v2]->edge = e2;

   g->vert[v2]-> degree++;

   e1->twin = e2; //point each node to its twin.
   e2->twin = e1;
}


////////////////////////////////////////////////NOTE:: CAN GO FROM O(E) -> O(1) IF WE KEEP TRACK OF LAST EDGE IN THE LIST.
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

//true value is not correct.

//twin nonsense?
//returns null if no match.
edge* find_edge(graph* g, int v1, int v2){
   v1 = value(g,v1);
   v2 = value(g,v2);

   edge* e = g->vert[v1];
   while(e){
      if(v2 == value(g,e->otherVertex)){
         return e;
      }
   }
   return NULL;
}

int remove_edge(graph* g, int v1, int v2) {
   edge* e = find_edge(g, v1, v2);
}

//frees the whole list
void edge_free(edge* e){

}

//also frees any edges and their twins.
void vertex_free(vertex* v){
   typedef struct Vertex {
  int value;
  int parent; //used only if the graph is a tree
  int degree;

  edge* edge;
} vertex;
}


void graph_free(graph* g) {
   for(int i = 0; i <= g->vertex_count; i++){
      while(g->vert[i]->edge){
         edge* currEdge = g->vert[i]->edge;
         g->vert[i]->edge = currEdge->next;
         free(currEdge);
      }
      free(g->vert[i]);
   }
   free(g->vert);
  free(g);
}


//////////////////////////
//PRINT
//////////////////////////

//prints the format for CSAcademy's graph visualizer.
void graph_print(graph* g){

}

/* Creates a graph from csacademy's output format
   char* text: output text with linebreaks as '\n' */
graph* graph_create_text(char* text, int vertex_count){
   if(!text)
      return NULL;

   graph* out = graph_create(vertex_count);

   int e1 = -1;
   int e2 = -1;
   char space = 0;
   int i;

   for(i = 0; text[i]; i++){
      char c = text[i];
      if(c == '\n'){ //new line. add any complete edge and reset.
         if(e1 >= 0 && e2 >= 0){
            graph_add_edge(out, e1, e2);
         }
         e2 = e1 = -1;
         space = 0;
         continue;
      }

      if(c == ' '){
         space++;
         continue;
      }

      int* e = space ? &e2 : &e1; //select the edge that we are reading

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
      if(e1 >= 0 && e2 >= 0){
            graph_add_edge(out, e1, e2);
         }
   }

   return out;
}