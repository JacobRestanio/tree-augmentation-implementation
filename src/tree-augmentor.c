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

  char *t_text = "1 4\n2 3\n2 4\n3 5\n3 6\n3 7\n3 9\n3 8\n1 10\n10 12\n10 11\n3 13";
  char *g_text = "1 4\n2 3\n2 4\n1 10\n10 12\n8 5\n7 9\n7 13\n13 5\n6 8\n10 11\n9 6";
  int size = 13;

  graph *t = graph_create_text(t_text, size);
  graph *g = graph_create_text(g_text, size);

  int rt = 4;
  set_root(t, rt);

  int_ls* sg = children(t,3);
  printf("children of 3: ");
  ls_print(sg);
  printf("\n");
  fflush(stdout);


  edge_ls* x = blossom_algorithm(g, sg);
  printf("a2\n");
  fflush(stdout);


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