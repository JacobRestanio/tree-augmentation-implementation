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


//TODO
//hashmap in children()

int main(int argc, char *argv[])
{
  srand(time(0));

  // need to test merging's effect on parents

  char *t_text = "10 11\n3 13\n1 10\n10 12\n3 16\n1 4\n2 3\n2 4\n3 8\n3 14\n3 5\n3 6\n3 7\n3 9\n3 15\n";
  char *g_text = "16 9\n15 9\n13 5\n10 11\n6 9\n5 6\n14 8\n7 13\n15 9\n1 4\n14\n15\n16\n2 3\n2 4\n1 10\n10 12\n7 9\n8 6\n";
  char *match_text = "28 2\n28 1\n1 3\n3 4\n4 5\n5 6\n2 6\n3 8\n8 7\n9 10\n10 11\n9 8\n9 11\n14 15\n16 17\n14 13\n16 13\n13 12\n12 11\n17 18\n15 19\n18 19\n6 25\n25 24\n24 23\n23 20\n24 22\n22 21\n21 20\n20 19\n5 26\n26 27";
  int size = 16;

  graph *match_test = graph_create_text(match_text, 28);
  int_ls* verts = NULL;
  for(int i = 1; i<=28; i++)
    verts = ls_add(verts,i);
  edge_ls* matchting = blossom_algorithm(match_test, verts);
  printf("matching: "); print_edge_ls(matchting);
 
  printf("\n\n");

  graph *t = graph_create_text(t_text, size);
  graph *g = graph_create_text(g_text, size);

  int rt = 4;
  set_root(t, rt);

  printf("des 3 ");ls_print(descendants(t,3));printf("\n");

  case1(g,t);

  

  graph_print(g);

  printf("\n\n");print_edge(g->retain,1);

  //graph_print_all(g);

/*
  int_ls* sg = children(t,3);
  printf("children of 3: ");
  ls_print(sg);
  printf("\n");
  fflush(stdout);


  edge_ls* x = blossom_algorithm(g, sg);
  printf("a2\n");
  fflush(stdout);
*/

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