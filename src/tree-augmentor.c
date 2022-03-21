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
#include "../include/nagamochi.h"


int main(int argc, char *argv[]) {
   srand(time(0));


  //need to test merging's effect on parents

   char* t_text = "4 1\n2 4\n5 1\n3 2\n2 6\n5 10\n5 7\n5 11\n2 12\n8 2\n9 4\n";
   char* g_text = "1 10\n1 5\n2 3\n2 8\n2 12\n2 6\n2 4\n4 12\n4 9\n5 11\n5 7\n5 10\n5 1\n6 8\n6 2\n7 5\n8 6\n8 2\n9 12\n10 1\n10 11\n10 5\n11 10\n11 5\n12 9\n12 4\n12 2\n7 11\n7 10\n";
   int size = 12;

   graph* t = graph_create_text(t_text,size);
   graph* g = graph_create_text(g_text,size);

   int rt = 4;
   set_root(t,rt);
  /*
  case2(g,t);
  case2(g,t);
  case2(g,t);
  case2(g,t);
  case2(g,t);
  */
  case3(g,t);

  graph_print(g);
  //isolated and non_redudant test
  /*

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
      printf("\t non-red(%i): ", childs->value);
      ls_print(non_redundant(g,t, childs->value));
      printf("  triv(%i){%i}", childs->value, trivial(g,t,childs->value));
      childs = childs->next;
    }


    printf("\n");

    fringe = fringe->next;
  }
  */

}