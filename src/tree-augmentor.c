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

int main(int argc, char *argv[])
{
  srand(time(0));

  // need to test merging's effect on parents

  char *t_text = "1 4\n4 5\n6 12\n2 6\n2 5\n1 8\n8 3\n9 10\n10 7\n10 11\n15 8\n15 9\n2 14\n6 13";
  char *g_text = "1 4\n4 5\n6 12\n2 6\n2 5\n1 8\n8 3\n9 10\n10 7\n10 11\n15 8\n15 9\n2 14\n6 13\n13 12\n12 14\n12 3\n13 3\n";
  int size = 15;

  graph *t = graph_create_text(t_text, size);
  graph *g = graph_create_text(g_text, size);

  int rt = 4;
  set_root(t, rt);

  // isolated and non_redudant test

  graph_print(g);

  //while (case1(g, t) ^ 2 * case2(g, t) ^ 4 * case3(g, t) ^ 8 * case4(g, t));

  
  while(case4(g,t));

  printf("\n\n\n");

  graph_print(t);
  printf("\n\n");
  graph_print(g);
  printf("\n\n");
  ls_print(descendants(t,4));
  printf("\n\n");

  /*
  int_ls *pth = tree_path(t, 5, 2);

  merge_list(t, pth);
  merge_list(g, pth);

  unmerge_vertices(g, 4);
  unmerge_vertices(t, 4);

  int_ls *fringe = fringes(t, rt);
  int_ls *p_fringes = pseudo_fringes(g, t, rt);
  int_ls *desc = descendants(t, rt);
  int_ls *child = children(t, rt);

  printf("desc: ");
  ls_print(desc);
  printf("\n");

  printf("children: ");
  ls_print(child);
  printf("\n");

  printf("fringes: ");
  ls_print(fringe);
  printf("\n");

  printf("p_fringes: ");
  ls_print(p_fringes);
  printf("\n");

  while (fringe)
  {
    int_ls *isol = isolated(g, t, fringe->value);
    int_ls *non_redun = non_redundant(g, t, fringe->value);
    printf("p: %i \tisol: ", fringe->value);
    ls_print(isol);

    int_ls *childs = children(t, fringe->value);
    while (childs)
    {
      printf("\t non-red(%i): ", childs->value);
      ls_print(non_redundant(g, t, childs->value));
      printf("  triv(%i){%i}", childs->value, trivial(g, t, childs->value));
      childs = childs->next;
    }

    printf("\n");

    fringe = fringe->next;
  }
  */
}