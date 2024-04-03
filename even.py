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
    I = nx.Graph()
    I.add_nodes_from(T)


    # ##### PREPROCESSING #####

    

    # print(contract_map)

    ##########################

    # designate an arbitrary root node
    root_node = random.choice(list(T.nodes()))
    print("ROOT", root_node)
    for node in T.nodes():
        T.nodes[node]['coupons']=0
        
    T.nodes[root_node]['coupons']=1

    # we want to obtain a matching among nontwin leaf-to-leaf links

    leaf_to_leaf = L.copy()

    twins = []
    locking_links = []

    # filter out edges which are not leaf-to-leaf, twin, or locking

    for link in L.edges():
        
        # if the link is not leaf-to-leaf, remove it
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

                # we're looking at a twin link. check if there is a locking link spawning from it

                # first determine the path to the root to follow to look for T'
                path_to_root = nx.shortest_path(T, common_parent, root_node)
                for curr_node in path_to_root:
                    avoid_node = path_to_root[path_to_root.index(curr_node) + 1] if curr_node != root_node else None
                    subtree_leaves = find_descendant_leaves(T, curr_node, avoid_node)
                    if len(subtree_leaves) == 3:
                        subtree = subtree_finder(T, curr_node, avoid_node)
                        subtree_nodes = set(subtree.nodes())
                        bprime = None
                        for leaf in subtree_leaves:
                            if leaf != link[0] and leaf != link[1]:
                                bprime = leaf
                        if (link[0], bprime) in L.edges() or (bprime, link[0]) in L.edges():
                            incident_links = [(u, v) for u, v in L.edges() if u == link[0] or v == link[0]]
                            for ilink in incident_links:
                                if ilink[0] not in subtree_nodes or ilink[1] not in subtree_nodes:
                                    break
                            else:
                                twins.append(link)
                                if (link[0], bprime) in L.edges():
                                    locking_links.append((link[0], bprime))
                                    leaf_to_leaf.remove_edge(link[0], bprime)
                                else:
                                    locking_links.append((bprime, link[0]))
                                    leaf_to_leaf.remove_edge(bprime, link[0])

                        if (link[1], bprime) in L.edges() or (bprime, link[1]) in L.edges():
                            incident_links = [(u, v) for u, v in L.edges() if u == link[1] or v == link[1]]
                            for ilink in incident_links:
                                if ilink[0] not in subtree_nodes or ilink[1] not in subtree_nodes:
                                    break
                            else:
                                twins.append(link) if len(twins) == len(locking_links) else None
                                if (link[1], bprime) in L.edges():
                                    locking_links.append((link[1], bprime)) if len(locking_links) < len(twins) else None
                                    leaf_to_leaf.remove_edge(link[1], bprime)
                                else:
                                    locking_links.append((bprime, link[1])) if len(locking_links) < len(twins) else None
                                    leaf_to_leaf.remove_edge(bprime, link[1])
                                break

                    elif len(subtree_leaves) > 3:
                        break         

    # print("leaf-to-leaf",leaf_to_leaf.edges())

    # obtain a maximal matching on this link-set - leaf-to-leaf links which are nontwin + nonlocking
    matching = nx.maximal_matching(leaf_to_leaf)
                    # matching = set(leaf_to_leaf.edges())
                    # matching.pop()
    # print(matching)

    greedy_locking(T, L, I, matching, links, twins, locking_links)

    exhausted = False

    while(not exhausted):
        twins = []
        locking_links = []
        for link in L.edges():
            if T.degree(link[0]) != 1 or T.degree(link[1]) != 1:
                continue

            # check for a common parent
            common_parent = next(T.neighbors(link[0]))
            if common_parent == next(T.neighbors(link[1])):
                # if parent is root node, the parent isnt a stem and the children aren't twins
                if(common_parent == root_node):
                    continue
                # throw away the edge if the link is connecting twins
                if len([n for n in T.neighbors(common_parent) if n != link[0] and n!= link[1] and T.degree(n) == 1]) == 0:

                    # we're looking at a twin link. check if there is a locking link spawning from it

                    # first determine the path to the root to follow to look for T'
                    path_to_root = nx.shortest_path(T, common_parent, root_node)
                    for curr_node in path_to_root:
                        avoid_node = path_to_root[path_to_root.index(curr_node) + 1] if curr_node != root_node else None
                        subtree_leaves = find_descendant_leaves(T, curr_node, avoid_node)
                        if len(subtree_leaves) == 3:
                            subtree = subtree_finder(T, curr_node, avoid_node)
                            subtree_nodes = set(subtree.nodes())
                            bprime = None
                            for leaf in subtree_leaves:
                                if leaf != link[0] and leaf != link[1]:
                                    bprime = leaf
                            if (link[0], bprime) in L.edges() or (bprime, link[0]) in L.edges():
                                incident_links = [(u, v) for u, v in L.edges() if u == link[0] or v == link[0]]
                                for ilink in incident_links:
                                    if ilink[0] not in subtree_nodes or ilink[1] not in subtree_nodes:
                                        break
                                else:
                                    twins.append(link)
                                    if (link[0], bprime) in L.edges():
                                        locking_links.append((link[0], bprime))
                                    else:
                                        locking_links.append((bprime, link[0]))

                            if (link[1], bprime) in L.edges() or (bprime, link[1]) in L.edges():
                                incident_links = [(u, v) for u, v in L.edges() if u == link[1] or v == link[1]]
                                for ilink in incident_links:
                                    if ilink[0] not in subtree_nodes or ilink[1] not in subtree_nodes:
                                        break
                                else:
                                    twins.append(link) if len(twins) == len(locking_links) else None
                                    if (link[1], bprime) in L.edges():
                                        locking_links.append((link[1], bprime)) if len(locking_links) < len(twins) else None
                                    else:
                                        locking_links.append((bprime, link[1])) if len(locking_links) < len(twins) else None
                                    break

                        elif len(subtree_leaves) > 3:
                            break 

        print("CALLING GREEDY LOCKING")
        greedy_locking(T, L, I, matching, links, twins, locking_links)  
        if len(twins) == 0:
            exhausted = True

    # while len(T.nodes()) > 1:
    print(matching)
    greedy2(T,L,I,matching, links)


    # Now, we contract all links in the matching with the intent to decontract them

    subtree = T.copy()
    find_minimally_semiclosed(subtree, L, matching, links, I)
    print("T'")
    print(subtree.edges())


def find_minimally_semiclosed(T, L, M, original_linkset, I):
    dangerous_subtree_list = []

    # Find all leaf nodes
    leaves = [node for node in T.nodes() if T.degree(node) == 1 and node != root_node]

    # Initialize a queue for BFS traversal
    queue = leaves

    # Initialize a set to keep track of visited nodes
    visited = set(leaves)
    print("edges", T.edges())
    # Perform BFS traversal
    while queue:
        current_node = queue.pop(0)
        path = nx.shortest_path(T, current_node, root_node)
        toAvoid = path[path.index(current_node) + 1] if current_node != root_node else None
        curr_rooted_subtree = subtree_finder(T, current_node, toAvoid)
        print("testing SUBTREE", curr_rooted_subtree.nodes(), "from", current_node, "to", toAvoid)
        print("queue", queue)
        if M_compatible(set(curr_rooted_subtree.nodes()), M):
            if unmatched_leaf_closed(set(curr_rooted_subtree.nodes()), T, L, M):
                if not dangerous(curr_rooted_subtree, T, L.copy(), M):
                    print("found not dangerous")
                    toContract = set()
                    for link in M:
                        if link[0] in curr_rooted_subtree.nodes() and link[1] in curr_rooted_subtree.nodes():
                            toContract.add(link)
                    unmatched_leaves = [node for node in curr_rooted_subtree if T.degree(node) == 1 and all(node not in match for match in M)]
                    for leaf in unmatched_leaves:
                        toContract.add(uplink(T, L, leaf))
                    while len(toContract) > 0:
                        contract(T, toContract.pop(), I, L, original_linkset, toContract, True)
                    return 
                else:
                    print("DANGEROUS")
                    dangerous_subtree_list.append(curr_rooted_subtree)
                    for path_node in nx.shortest_path(T, current_node, root_node):
                        visited.add(path_node)

            # if so, check if dangerous
            # if not, return the tree and its exact cover
    
        parent_node = [neighbor for neighbor in T.neighbors(current_node) if neighbor not in visited and neighbor != root_node and neighbor in nx.shortest_path(T, current_node, root_node)]
        print("for", current_node, "parent node", parent_node)
        queue.extend(parent_node)
        visited.update(parent_node)

    if len(dangerous_subtree_list) == 0:
        print("no dangerous :(")
        toContract = set()
        for link in M:
            if link[0] in curr_rooted_subtree.nodes() and link[1] in curr_rooted_subtree.nodes():
                toContract.add(link)
        unmatched_leaves = [node for node in curr_rooted_subtree if T.degree(node) == 1 and all(node not in match for match in M)]
        for leaf in unmatched_leaves:
            toContract.add(uplink(T, L, leaf))
        while len(toContract) > 0:
            print("contracting", toContract)
            contract(T, toContract.pop(), I, L, original_linkset, toContract, True)
        return
    else:
        dangerous_scheme(T, L, M, dangerous_subtree_list)



# finds a non-dangerous semiclosed tree T' and its exact cover I' when all minimally semiclosed trees are dangerous
def dangerous_scheme(T, L, M):
    return

def dangerous(subtree, T, L, M):
    if len([node for node in subtree.nodes() if T.degree(node) > 1 and T.nodes[node]['coupons'] == 1]) == 0:
        if len([(u, v) for u, v in M if u in subtree.nodes() and v in subtree.nodes()]) == 1:
            matched_leaves = [(u, v) for u, v in M if u in subtree.nodes() and v in subtree.nodes()]
            leaf_list = [node for node in subtree.nodes() if T.degree(node) == 1]
            if len(leaf_list) == 3:
                if (leaf_list[0], leaf_list[1]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[1])):
                        return False
                if (leaf_list[0], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[2])):
                        return False
                if (leaf_list[1], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[2])):
                        return False
                # no stems
                    
                a = leaf_list[0] if leaf_list[0] not in matched_leaves[0] else leaf_list[1] if leaf_list[1] not in matched_leaves[0] else leaf_list[2]
                if (a,matched_leaves[0][0]) in L.edges() or (matched_leaves[0][0],a) in L.edges():
                    link = (a,matched_leaves[0][0]) if (a,matched_leaves[0][0]) in L.edges() else (matched_leaves[0][0],a)
                    copy = subtree.copy()
                    contract(copy, link, None, L.copy(), None, M.copy(), False)
                    if len([node for node in copy.nodes() if T.degree(node) == 1]) == 1:
                        if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                            return True
                elif (a,matched_leaves[0][1]) in L.edges() or (matched_leaves[0][1],a) in L.edges():
                    link = (a,matched_leaves[0][1]) if (a,matched_leaves[0][1]) in L.edges() else (matched_leaves[0][1],a)
                    copy = subtree.copy()
                    contract(copy, link, None, L.copy(), None, M.copy(), False)
                    if len([node for node in copy.nodes() if copy.degree(node) == 1]) == 1:
                        if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                            return True
                
            elif len(leaf_list) == 4:
                stem_count = 0
                stem = [None, []]
                if (leaf_list[0], leaf_list[1]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[1])):
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[0])), [leaf_list[0], leaf_list[1]]]
                if (leaf_list[0], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[2])):
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[0])), [leaf_list[0], leaf_list[2]]]
                if (leaf_list[0], leaf_list[3]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[3])):
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[0])), [leaf_list[0], leaf_list[3]]]
                if (leaf_list[1], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[2])):
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[1])), [leaf_list[1], leaf_list[2]]]
                if (leaf_list[1], leaf_list[3]) in L.edges():
                    if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[3])):
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[1])), [leaf_list[1], leaf_list[3]]]
                if (leaf_list[2], leaf_list[3]) in L.edges():
                    if next(T.neighbors(leaf_list[2])) == next(T.neighbors(leaf_list[3])):
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[2])), [leaf_list[2], leaf_list[3]]]
                if stem_count == 1:
                    if stem[1][0] in matched_leaves[0] and stem[1][1] not in matched_leaves[0] or stem[1][1] in matched_leaves[0] and stem[1][0] not in matched_leaves[0]:
                        subtree = nx.Graph(subtree)
                        newM = M.copy()
                        contract(subtree, (stem[1][0], stem[1][1]), None, L, None, newM, False)
                        matched_leaves = [(u, v) for u, v in newM if u in subtree.nodes() and v in subtree.nodes()]
                        leaf_list = [node for node in subtree.nodes() if T.degree(node) == 1]
                        if len(leaf_list) == 3:
                            if (leaf_list[0], leaf_list[1]) in L.edges():
                                if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[1])):
                                    return False
                            if (leaf_list[0], leaf_list[2]) in L.edges():
                                if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[2])):
                                    return False
                            if (leaf_list[1], leaf_list[2]) in L.edges():
                                if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[2])):
                                    return False
                            # no stems
                            a = leaf_list[0] if leaf_list[0] not in matched_leaves[0] else leaf_list[1] if leaf_list[1] not in matched_leaves[0] else leaf_list[2]
                            if (a,matched_leaves[0][0]) in L.edges() or (matched_leaves[0][0],a) in L.edges():
                                link = (a,matched_leaves[0][0]) if (a,matched_leaves[0][0]) in L.edges() else (matched_leaves[0][0],a)
                                copy = subtree.copy()
                                contract(copy, link, None, L.copy(), None, M.copy(), False)
                                if len([node for node in copy.nodes() if T.degree(node) == 1]) == 1:
                                    if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                                        return True
                            elif (a,matched_leaves[0][1]) in L.edges() or (matched_leaves[0][1],a) in L.edges():
                                link = (a,matched_leaves[0][1]) if (a,matched_leaves[0][1]) in L.edges() else (matched_leaves[0][1],a)
                                copy = subtree.copy()
                                contract(copy, link, None, L.copy(), None, M.copy(), False)
                                if len([node for node in copy.nodes() if copy.degree(node) == 1]) == 1:
                                    if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                                        return True

    return False
                



def unmatched_leaf_closed(subtree, T, L, M):
    unmatched_leaves = [node for node in subtree if T.degree(node) == 1 and all(node not in match for match in M)]

    for leaf in unmatched_leaves:
        incident_links = [(u, v) for u, v in L.edges() if u == leaf or v == leaf]
        print(leaf, "indicent links", incident_links)
        for link in incident_links:
            if link[0] not in subtree or link[1] not in subtree:
                return False
    return True

def M_compatible(subtree, M):
    for link in M:
        if link[0] not in subtree and link[1] in subtree or link[1] not in subtree and link[0] in subtree:
            return False
    return True
    

def uplink(tree, linkset, node):
    closest = [None, len(tree.nodes())]
    incident_links = [(u, v) for u, v in linkset.edges() if u == node or v == node]
    for link in incident_links:
        # save the tuple element which is not the node
        other_node = link[0] if link[1] == node else link[1]
        path = nx.shortest_path(tree, other_node, root_node)
        print("Link", link, "Path", path)
        if len(path) < closest[1]:
            closest = [other_node, len(path)]
    return (node, closest[0]) if (node, closest[0]) in linkset.edges() else (closest[0], node)


def subtree_finder(tree, toKeep, throwAway):

    if (toKeep == root_node):
        return tree
    
    # Make a copy of the tree
    tree_copy = tree.copy()
    
    # Remove the specified edge
    tree_copy.remove_edge(toKeep, throwAway)
    
    # Get the connected components
    connected_components = list(nx.connected_components(tree_copy))
    
    # Find the connected component containing the first node of the removed edge
    for component in connected_components:
        if toKeep in component:
            return tree_copy.subgraph(component)
    
    return nx.Graph()

def find_descendant_leaves(graph, node, toAvoid):
    descendant_leaves = set()

    # Perform BFS from the specified node
    queue = [node]
    visited = set()
    while queue:
        current_node = queue.pop(0)
        visited.add(current_node)
        neighbors = graph.neighbors(current_node)
        for neighbor in neighbors:
            if neighbor not in visited and neighbor != toAvoid:
                queue.append(neighbor)
                if graph.degree(neighbor) == 1:
                    descendant_leaves.add(neighbor)

    return descendant_leaves


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
        for link in L.edges():
            # contract along a link connecting two unmatched leaves
            if T.degree(link[0]) == 1 and T.degree(link[1]) == 1:
                for matched in matching:
                    if link[0] in matched or link[1] in matched:
                        break
                else:
                    contract(T, link, I, L, original_linkset, matching, False)
                    break
        else:
            break

    

# greedy locking contraction
def greedy_locking(tree, links, I, matching, original_linkset, twins, locking_links):
    print("twins", twins)
    print("locking links", locking_links)
    for i in range(len(twins)):
        a = twins[i][0] if twins[i][1] in locking_links[i] else twins[i][1]
        print("contracting", locking_links[i])
        contract(tree, locking_links[i], I, links, original_linkset, matching, True)
        print("contracting", uplink(tree, links, a))
        contract(tree, uplink(tree, links, a), I, links, original_linkset, matching, True)
        plot(tree, links)
            

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

def plot(graph1, graph2):
    plt.figure(figsize=(10, 5))

    plt.subplot(121)
    nx.draw(graph1, with_labels=True, font_weight='bold')
    plt.title('arg 1')

    plt.subplot(122)
    nx.draw(graph2, with_labels=True, font_weight='bold', node_color='lightblue', edge_color='gray')
    plt.title('arg 2')

    plt.tight_layout()
    plt.show()

def main():
    T = nx.Graph()
    T.add_edges_from([(0,1),(1,2),(2,3),(3,4),(3,5),(2,6),(6,7),(6,8)])
    L = nx.Graph()
    L.add_edges_from([(0,8),(3,7), (4,5),(5,8)])
    even(T, L)
    return

if __name__ == '__main__':
    main()