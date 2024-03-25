import networkx as nx
import matplotlib.pyplot as plt
import treegenerator as tg
import random
import numpy as np
import time
from itertools import combinations

# In a contracted version of a tree or linkset, we need to add a link between compound nodes to the solution 
# set I. This map helps keep track of which tree nodes were contracted to which compound nodes to make solution
# appending easier

# KEYS: names of compound nodes
# VALUES: list of treenodes that the compound node contracted

contract_map = {}

def even(tree, links):
    global root_node
    """
    This function runs the Even et al algorithm on a given Tree and Link Set (preprocessing included)

    """
    contract_map.clear()

    T = tree.copy()
    L = links.copy()

    # print("tree", T.edges())
    # print("links", L.edges())

    # solution set!
    I = tree.copy()

    ##### PREPROCESSING #####

    # Assumption 2: There are no 'reducible' edges in T
    # Determine the maximal links of L. If there is a unique maximal link 
    # covering an edge e, put the link in I, and contract the whole cycle. Repeat exhaustively.
                    
    eliminate_reducible_edges(T,L,I)

    # print("reduced tree", T.edges(), "w reduced links ", L.edges(), "and new solution", I.edges())

    # print(contract_map)

    if(len(list(T.nodes())) == 1):
        return I

    ##########################

    # designate an arbitrary root node
    root_node = list(T.nodes())[0]

    # we want to obtain a matching among nontwin leaf-to-leaf links

    leaf_to_leaf = L.copy()

    # filter out edges which are not leaf-to-leaf or twin
    for link in L.edges():

        # remove edge if not leaf-to-leaf
        if T.degree(link[0]) != 1 or T.degree(link[1]) != 1:
            leaf_to_leaf.remove_edge(*link)
            continue

        # check for a common parent
        common_parent = next(T.neighbors(link[0]))
        if common_parent == next(T.neighbors(link[1])):
             # if parent is root node, the parent isnt a stem and the children aren't twins
            if(common_parent == root_node):
                continue
            # throw away the edge if the link is connecting twins
            if len([n for n in T.neighbors(common_parent) if n != link[0] and n!= link[1] and T.degree(n) == 1]) == 0:
                leaf_to_leaf.remove_edge(link[0], link[1])


    # print("leaf-to-leaf",leaf_to_leaf.edges())
    # obtain a maximal matching on this link-set
    matching = nx.maximal_matching(leaf_to_leaf)
    # print(matching)

    # distribute coupons initially
    for node in T.nodes():
        T.nodes[node]['coupons'] = 0

        ## unmatched leaves get 1 coupon ##
        if(T.degree(node) == 1 and not any(node in edge for edge in matching)):
            T.nodes[node]['coupons']=1
   
    # so does the root
    T.nodes[root_node]['coupons']=1

    # initialize the link coupons
    for edge in L.edges():
        L[edge[0]][edge[1]]['coupons'] = 0

    # find links in the matching where one node is a leaf of a stem and the other child is unmatched
    for link in matching:
        # ensure one node in the link is a leaf
        for i in range(2):
            parent = next(T.neighbors(link[i]))
            if parent != root_node:
                sibling_leaf_count = 0
                sibling_leaf = None
                for neighbor in T.neighbors(parent):
                    if(neighbor != link[i] and T.degree(neighbor) == 1):
                        sibling_leaf = neighbor
                        sibling_leaf_count+=1
                if(sibling_leaf_count == 1 and not any(sibling_leaf in edge for edge in matching)):
                    L[link[0]][link[1]]['coupons'] = 2
                    break
        else:
            L[link[0]][link[1]]['coupons'] = 1.5

    # for node in T:
    #     print("node ", node, " has ", T.nodes[node]['coupons'])

    # for edge in L.edges():
    #     print("edge ", edge, " has ", L[edge[0]][edge[1]]['coupons'])

    #while(len(T.nodes()) > 1):
    # print("matching", matching)
    greedy1(T, L, I, matching, links)
    greedy2(T,L,I,matching, links)
    print("tree", T.edges())
    print("links", L.edges())
    print("solution", I.edges())
    print("matching", matching)


    # Now, we contract all links in the matching with the intent to decontract them


    list_of_matchings = list(matching)
    contractionLog = []
    original_incidence = []
    
    # we need to find a T' as specified by the proof of lemma 3.11
    i = 0
    while len(list_of_matchings) > 0:
        contractionLog.append(nx.shortest_path(T, list_of_matchings[0][0], list_of_matchings[0][1]))
        neighbors = []
        for iter in range(len(contractionLog[i])):
            neighbors.append(list(T.neighbors(contractionLog[i][iter])))
        original_incidence.append(neighbors)
        contract(T, list_of_matchings[0], I, L, links, [], False)
        for toFix in list_of_matchings[1:]:
            if toFix[0] in contractionLog[i][1:]:
                toFix[0] = contractionLog[i][0]
            elif toFix[1] in contractionLog[i][1:]:
                toFix[1] = contractionLog[i][0]
        i+=1
        list_of_matchings.pop(0)
    
    print("contraction log", contractionLog)
    print(original_incidence)

    subtree = T.copy()
    find_rooted_subtree(subtree, L.edges())
    print("T'")
    print(subtree.edges())

    #TODO: Determine why I am oversaturated with links. This step doesn't work unless there is some deficit among the vertices of T

        # make each edge contraction then work backwards as designed
    
    # TODO: Determine if this T' is a special deficient tree
    #   if not, cover T' with a quick 'basic cover' B(T') = up(L(T')) U (M ^ T')
    #   if so, there is a little more work involved in finding the cover (definition 4.11)

def find_rooted_subtree(tree, links):
    # Get all leaf nodes in the tree
    leaf_nodes = [node for node in tree.nodes() if tree.degree(node) == 1]
    descendants_dict = nx.dfs_successors(tree, root_node)
    print(root_node)
    print("tree", tree.edges())
    print("links", links)
    # Perform BFS traversal on the tree graph
    for edge in nx.bfs_edges(tree, root_node):
        # Get the descendants of the child node in the current edge
        descendants = list(descendants_dict.get(edge[1], []))
        descendants.append(edge[1])
        print("trying node ", edge[1])
        print("descendents", descendants)
        
        # Check if any leaf node has an edge outside of its descendants in the tree
        for link in links:
            if link[0] in leaf_nodes and link[0] in descendants and link[1] not in descendants or link[1] in leaf_nodes and link[1] in descendants and link[0] not in descendants:
                break
        else:
            # No leaf node has an external edge, root the subtree at edge[1]
            return tree.subgraph(descendants)

# GREEDY CONTRACTION 2: for any two links in M, contract both if they're disjoint
def greedy2(T,L,I,matching, original_linkset):
    while True:
        pairs = combinations(matching, 2)
        any_disjoint=False
        for pair in pairs:
            if not set(nx.shortest_path(T, pair[0][0], pair[0][1])).intersection(set(nx.shortest_path(T, pair[1][0], pair[1][1]))):
                contract(T, pair[0], I, L, original_linkset, matching, True)
                contract(T, pair[1], I, L, original_linkset, matching, True)
                any_disjoint = True
                break
        if not any_disjoint:
            break
    if(len(matching) == 1):
        contract(T, matching.pop(), I, L, original_linkset, matching, True)
    

# GREEDY CONTRACTION 1 : if link + treepath has >= 2 coupons, contract the cycle
def greedy1(tree, links, I, matching, original_linkset):
    while(True):
        for link in links.edges():
            if tree.nodes[link[0]]['coupons'] + tree.nodes[link[1]]['coupons'] >= 2:
                contract(tree, link, I, links, original_linkset, matching, True)
                break
            if link in matching:
                found = False
                for node in nx.shortest_path(tree, link[0], link[1]):
                    if(node != link[0] and node != link[1] and tree.nodes[node]['coupons']):
                        contract(tree, link, I, links, original_linkset, matching, True)
                        found = True
                        break
                if(found):
                    break
            if links[link[0]][link[1]]['coupons'] == 2:
                contract(tree, link, I, links, original_linkset, matching, True)
                break
        else:
            break

def contract(tree, link, I, links, original_linkset, matching, append_to_I):
    global root_node
    toContract = []
    shadow = nx.shortest_path(tree, link[0], link[1])
    for i in range(len(shadow)-1):
        toContract.append([shadow[i],shadow[i+1]]) if [shadow[i],shadow[i+1]] not in toContract and [shadow[i+1],shadow[i]] not in toContract else None

    while(not len(toContract) == 0):
        if toContract[0][1] == root_node:
            root_node = toContract[0][0]

        if toContract[0][0] not in contract_map:
            contract_map[toContract[0][0]] = set()
        contract_map[toContract[0][0]].add(toContract[0][1])

        # contract the first edge in the list
        nx.contracted_edge(tree,tuple(toContract[0]),self_loops=False, copy=False)

        if tuple(toContract[0]) not in links.edges() and tuple(toContract[0])[::-1] not in links.edges():
            links.add_edge(*tuple(toContract[0]))
        nx.contracted_edge(links,tuple(toContract[0]),self_loops=False, copy=False)

        # if any other edge in our list is adjacent to the "dest" node of contraction, re-index it
        for toFix in toContract[1:]:
            if(toFix[0] == toContract[0][1]):
                toFix[0] = toContract[0][0]
            elif(toFix[1] == toContract[0][1]):
                toFix[1] = toContract[0][0]

        for pair in matching.copy():
            if(pair[0] == toContract[0][1]):
                matching.remove(pair)
                if pair[1] != toContract[0][0]:
                    matching.add((toContract[0][0], pair[1]))
            elif(pair[1] == toContract[0][1]):
                matching.remove(pair)
                if pair[0] != toContract[0][0]:
                    matching.add((pair[0],toContract[0][0]))

        # remove the edge we contracted from our list
        tree.nodes[toContract[0][0]]['coupons'] = 1
        toContract.remove(toContract[0])

        if(append_to_I):
            add_to_solution(I, original_linkset, link)

def add_to_solution(I, linkset, link):
    """
    given a link between compound nodes to append to the solution I, this procedure determines which edge can be added 
    in the original linkset

    """
    if link[0] in contract_map and link[1] in contract_map:
        for v1 in contract_map[link[0]]:
            for v2 in contract_map[link[1]]:
                if linkset.has_edge(v1, v2):
                    return I.add_edge(v1, v2)
    elif link[0] in contract_map:
        for v1 in contract_map[link[0]]:
            if linkset.has_edge(v1, link[1]):
                return I.add_edge(v1, link[1])
    elif link[1] in contract_map:
        for v2 in contract_map[link[1]]:
            if linkset.has_edge(link[0], v2):
                return I.add_edge(link[0], v2)
    else:
        if linkset.has_edge(link[0], link[1]):
            return I.add_edge(link[0], link[1])

def eliminate_reducible_edges(T,L,I):
    """
    Given a tree T, link set L, and solution set I, this function contracts reducible edges and updated I accordingly.
    More specifically, the maximal links are determined. Edges covered only with 1 maximal link are reducible and contracted
    Note that networkx contracts edges, renaming the node with the "source" node of the contracted edge

    """
    # determine first only the max shadows
    shadows = []

    for link in L.edges():
        link_treepath = nx.shortest_path(T, link[0], link[1])

        # no max shadows? add one
        if(len(shadows) == 0):
            shadows.append(link_treepath)
            continue

        i = 0
        already_replaced= False
        while i < len(shadows):
            verified_path = shadows[i]
            # current is a subset of one we have?
            if(is_sublist(link_treepath, verified_path)):
                break
            # one we have is subset of current?
            elif (is_sublist(verified_path, link_treepath)):
                # be careful, maybe this link is a superset of more than one verified max shadow
                shadows.remove(verified_path)
                shadows.append(link_treepath) if not already_replaced else None
                already_replaced = True
                continue
            i+=1
        # new shadow doesn't nest sequences! add it to max links
        else:
            shadows.append(link_treepath)

    # now that we have the max links, see what edges are reducible

    # keep track of the 2-connected components with at least one edge covered by only one max shadow
    toContract = []

    # maximal links to add to I
    toadd = set()

    for e in T.edges():
        # count of number of times covered
        unique = 0
        # shadow which covers it (only used when the cover is unique)
        bp = []
        for basis_path in shadows:
            # is e contained in a max shadow?
            if is_sublist([e[0],e[1]],basis_path):
                unique += 1
                bp = basis_path
            # if we have more than one cover, quit. this edge isn't interesting
            if unique > 1:
                break

        # only one cover means the 2-connected component is reducible
        if(unique == 1):
            # add every edge in the shadow to toContract if it isn't already there
            for i in range(len(bp)-1):
                toContract.append([bp[i],bp[i+1]]) if [bp[i],bp[i+1]] not in toContract and [bp[i+1],bp[i]] not in toContract else None
            # add the max link we're using to make the 2-connected component
            toadd.add((bp[0],bp[-1]))

    # contracting workaround
    while(not len(toContract) == 0):
        if toContract[0][0] not in contract_map:
            contract_map[toContract[0][0]] = set()
        contract_map[toContract[0][0]].add(toContract[0][1])

        # contract the first edge in the list
        nx.contracted_edge(T,tuple(toContract[0]),self_loops=False, copy=False)

        # add the tree edge to L for the sake of contracting to see the resulting link configuration
        if tuple(toContract[0]) not in L.edges() and tuple(toContract[0])[::-1] not in L.edges():
            L.add_edge(*tuple(toContract[0]))
        nx.contracted_edge(L,tuple(toContract[0]),self_loops=False, copy=False)

        # if any other edge in our list is adjacent to the "dest" node of contraction, re-index it
        for toFix in toContract[1:]:
            if(toFix[0] == toContract[0][1]):
                toFix[0] = toContract[0][0]
            elif(toFix[1] == toContract[0][1]):
                toFix[1] = toContract[0][0]

        # remove the edge we contracted from our list
        toContract.remove(toContract[0])

    # lastly, add the maximal links which are unique covers to the solution set
    for link in toadd:
        I.add_edge(*link)
    
# routine for determining normal or reverse sublists of treepaths
def is_sublist(sub, lst):
    return sub[0] in lst and sub[-1] in lst

def main():
    return

if __name__ == '__main__':
    main()