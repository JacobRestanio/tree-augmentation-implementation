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
#include "../include/blossom.h"
#include "../include/lemma7.h"


//TODO
//hashmap in children()
// merge_vertices breaks parent array. can only merge_path.
//case 3 and case 4 may leave f' edges uncovered. store these edges for later and see if they are covered at the end


void run_matching_test(){
  char *match_text = "28 2\n28 1\n1 3\n3 4\n4 5\n5 6\n2 6\n3 8\n8 7\n9 10\n10 11\n9 8\n9 11\n14 15\n16 17\n14 13\n16 13\n13 12\n12 11\n17 18\n15 19\n18 19\n6 25\n25 24\n24 23\n23 20\n24 22\n22 21\n21 20\n20 19\n5 26\n26 27";

  graph *match_test = graph_create_text(match_text, 28);
  int_ls* verts = NULL;
  for(int i = 1; i<=28; i++)
    verts = ls_add(verts,i);
  edge_ls* matchting = blossom_algorithm(match_test, verts);
  printf("matching: "); print_edge_ls(matchting);
  if(l_size(matchting) == 13){
    printf("\nthis is a maximum matching\n");
  }
  else{
    printf("\nthis is not a maximum matching\n");
  }
}


int main(int argc, char *argv[])
{
  srand(time(0));

  char *t_text = "1 2\n2 5\n5 8\n1 3\n1 4\n3 15\n15 11\n15 12\n3 6\n6 9\n6 13\n6 16\n4 7\n7 10\n7 14\n";
  char *g_text = "1 2\n2 5\n5 8\n1 3\n1 4\n3 15\n15 11\n15 12\n3 6\n6 9\n6 13\n6 16\n4 7\n7 10\n7 14\n14 10\n9 16\n16 13\n13 11\n11 6\n15 13\n16 12\n12 13\n12 9\n9 15\n8 2\n8 1\n5 1";
  int size = 17;
  
  printf("\n\n");

  graph *t = graph_create_text(t_text, size);
  graph *g = graph_create_text(g_text, size);

  int rt = 1;
  set_root(t, rt);
  set_gm(g);
    

  //lemma7(g,t,1,.0001);

  
  nagamochi(g,t,.1);

  return 0;


  int_ls *fringe = fringes(t, rt);
  int_ls *p_fringes = pseudo_fringes(g, t, rt);
  int_ls *desc = descendants(t, rt);
  int_ls *child = children(t, rt);
  int_ls *f = all_fringes(g, t, rt);
  int_ls *th = thorns(t,rt);
  int_ls *br = NULL;
  edge_ls* x1 = leaf_edges(g,t,rt);
  edge_ls* x2 = leaf_to_leaf_edges(g,t,rt);
  edge_ls* prime1 = prime_edges_type1(g,t,rt);
  edge_ls* prime2 = prime_edges_type2(g,t,rt);
  edge_ls* primes = prime_edges(g,t,rt);
  chain_ls* chains = find_chains(t,rt);
  process_chains(g,t,chains);
  
  /*for(int_ls* cur_des = desc; cur_des; cur_des = cur_des->next){
    if(is_branch(t,cur_des->value))
      br = ls_add(br,cur_des->value);
  }*/

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

  printf("all fringe: ");
  ls_print(f);
  printf("\n");

  printf("thorns: ");
  ls_print(th);
  printf("\n");

  printf("branches ");
  ls_print(br);
  printf("\n");

  printf("leaf_edges:\t"); print_edge_ls(x1); printf("\n");

  printf("lf2lf_edges:\t"); print_edge_ls(x2); printf("\n");

  printf("prime type 1:\t"); print_edge_ls(prime1); printf("\n");

  printf("prime type 2:\t"); print_edge_ls(prime2); printf("\n");

  printf("all prime:\t"); print_edge_ls(primes); printf("\n");

  int u = 1; int v = 11;
  printf("path %i %i :\t",u,v); ls_print(tree_path(t,u,v)); printf("\n");

 print_chain_and_swings(chains); printf("\n");

  int_ls* des = descendants(t,rt);
  int_ls* lf = NULL;
  /*while(des){
    if(lf_closed(g,t,des->value)){
      lf = ls_add(lf,des->value);
    }
    des = des->next;
  }*/

  printf("lf-closed: "); ls_print(lf); printf("\n\n");
  lf = minimally_lf_closed(g,t,1);
  printf("min lf-closed: "); ls_print(lf); printf("\n\n");

  nagamochi(g,t,.25);


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