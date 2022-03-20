#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include "../include/tree-generator.h"
#include "../include/tree-helper.h"
#include "../include/tree-greedy.h"
#include "../include/graph.h"
#include "../include/int-list.h"
#include "../include/list.h"

/* Temporary Function to some debugging info */
/* Some change but in a different branch */
void printInfo(int n, int tree[][n], int edgeSet[][n], int edgeWeights[][n]) {
   printf("TREE\n");
   printTreeAdjMat(n, tree);
   printf("\nEDGE SET\n");
   printTreeAdjMat(n, edgeSet);
   printf("\nEDGE WEIGHTS\n");
   printTreeAdjMatWithPadding(n, edgeWeights, 1);
   printf("___________________________________\n\n");
}

typedef struct testls {
  struct testls* next;
  struct testls* prev;

  int k;
} testls;

void testprint(void* testlist){
  testls* xx = testlist;
  printf("%i",xx->k);
}

int testmatch(void* testlist, void* testlist2){
  testls* xx = testlist;
  testls* t2 = testlist2;
  if(xx->k == t2->k)
    return 1;
  return 0;
}

testls* testmake(int x){
    testls* ret = malloc(sizeof(testls*));

    ret->k = x;
}


int main(int argc, char *argv[]) {
   srand(time(0));


  //need to test merging's effect on parents

   char* t_text = "4 1\n2 4\n5 1\n3 2\n2 6\n5 10\n5 7\n5 11\n2 12\n8 2\n9 4\n";
   char* g_text = "2 4\n5 1\n3 2\n2 6\n5 10\n5 7\n5 11\n2 12\n8 2\n9 4\n8 6\n10 11\n12 4\n3 2\n3 2\n12 9\n12 11\n12 5\n10 1\n";
   int size = 12;

   graph* t = graph_create_text(t_text,size);
   graph* g = graph_create_text(g_text,size);

   int rt = 4;
   set_root(t,rt);

   int_ls* pth = tree_path(t, 5,2);

   merge_list(t,pth);
   merge_list(g,pth);

  unmerge_vertices(g,4);
  unmerge_vertices(t,4);

   graph_print(g);
   printf("\n\n");
   graph_print(t);

   

  int_ls* fringe = fringes(t,rt);
  int_ls* desc = descendants(t,rt);
  int_ls* child = children(t,rt);
  printf("desc: ");
  ls_print(desc);
  printf("\n");

  printf("children: ");
  ls_print(child);
  printf("\n");

  printf("fringes: ");
  ls_print(fringe);
  printf("\n");

  while(fringe){
    int_ls* isol = isolated(g,t,fringe->value);
    int_ls* non_redun = non_redundant(g,t, fringe->value);
    printf("p: %i \tisol: ",fringe->value);
    ls_print(isol);

    int_ls* childs = children(t,fringe->value);
    while(childs){
      printf("\t non-redundant(%i): ", childs->value);
      ls_print(non_redundant(g,t, childs->value));

      childs = childs->next;
    }


    printf("\n");

    fringe = fringe->next;
  }

}