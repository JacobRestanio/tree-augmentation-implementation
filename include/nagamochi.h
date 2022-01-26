#ifndef NAGAMOCHI_H
#define NAGAMOCHI_H

#include "graph.h"

/*
APPROX Input: A graph G = (V; E) and a tree T = (V; F) rooted at r with E ∩F = ∅
such that T + F = (V; F ∪ E) is 2-edge-connected, and a constant  e > 0.

E := ∅;
while T contains more than one vertex do

    while one of Cases 1, 2, 3 and 4 holds do
        Execute procedures (P1), (P2), (P3) and (P4) in Cases 1, 2, 3 and 4,
        respectively, and add to E the edges retained by the procedure
    end // while //

    // Conditions (A1) and (A2) hold. 

    Choose a minimally lf-closed subtree T[D(v)];

    if condition (A3) holds in T[D(v)] then
        Compute an edge set Eapx ⊆ E which covers edges in T[D(v)] by procedure COVER;

        E := E ∪ Eapx;
        For X = {the end vertices of edges f ∈ F covered by Eapx}, let
        T := T=X and G := G=X ;
    else // T[D(v)] has a solo edge.

    Choose a lowest solo edge g;
    Fg := {all edges in Tg* };

    Compute an edge set E+ ⊆ E which covers ~(Fg) by Lemma 9 with apx-fac > 0;

    E' := E' ∪ E+;
    For X = {the end vertices of edges f ∈ F covered by E+},
     let T := T=X and G := G=X ;
end // while //

Output E (after modifying E
, if necessary, so that the edges f
contracted in Cases 3 and 4 are also covered in the input T without
increasing the size of E
).

*/

int nagamochi(int size, graph* tree, double aproxFactor);


/* PAGE 88
Case 1: There is an l-closed leaf tree T[D(v)]: Let Fleaf be the set of all leaf edges
in T[D(v)], and Eleaf be the set of edges in E connecting two leaf vertices in T[D(v)].
Now ~(F)leaf =Fleaf. 
In this case, a smallest set Evopt ⊆ E that covers Fleaf can be found by the next procedure (P1).
*/
//case1()

/* PAGE 89
Case 2: There is a leaf tree T[D(v)] such that T[D(v)] is not l-closed and there is a
trivial or isolated leaf vertex u ∈ Ch(v) (this includes the case of |Ch(v)|= 1): There is
the parent v' = p(v) of v (since v is not the root by the non-l-closedness of T[D(v)]).
Let Iv denote the set of all isolated vertices in Ch(v).

For each trivial leaf vertex u' ∈Iv (such that Eg(u') is a set of multiple edges of
(u', v)), we retain an edge (u', v) as part of the solution to cover the original T and
contract u' and v into a vertex both in T and G. Now if there remains an isolated
vertex u'' ∈ Iv, then any non-redundant edge in EG(u'') which covers the leaf edge
f = (v, u'') also covers the fringe edge f' = (v', v) of v. Thus, B(F) = B(F − f). 
For this reason, we contract the end vertices of the fringe edge f' = (v', v) into a single
vertex both in T and G, deleting any resulting self-loops. The procedure in Case 2 is
described as follows.
*/
//case2()

/* PAGE 89 */
//case3()

/* PAGE 89*/
//case4()

/* PAGE 88*/
//proc1()

/* PAGE 89*/
//proc2()

/* PAGE 89*/
//proc3()

/* PAGE 90*/
//proc4()

/* PAGE 93*/
//A3()

/* PAGE 94 */
//cover()


////////////////////


/* PAGE 92*/
// bindingEdge
/* B1 B2 B3 B4*/

// isLClosed()

// leaf()

// fringe()

/* child */
// ch()

/* parent*/
// p()

/* descendants */
// D()

// imposed subgraph




#endif