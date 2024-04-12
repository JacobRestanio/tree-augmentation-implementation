import random
import networkx as nx
import matplotlib.pyplot as plt
from itertools import combinations
import queue

def even(tree, links):
    global root_node
    """
    This function runs the Even et al algorithm on a given Tree and Link Set (preprocessing included)

    """

    T = tree.copy()
    L = links.copy()

    # solution set!
    I = nx.Graph()
    I.add_nodes_from(T)

    # designate a uniform random root node
    root_node = random.choice(list(T.nodes()))
    #root_node = 0

    # scheme to tell if a node is compound (1) or not (0)
    for node in T.nodes():
        T.nodes[node]['coupons']=0
        
    T.nodes[root_node]['coupons']=1

    # we want to obtain a matching among leaf-to-leaf links excluding twin links and locking links
    ### twin link - any link whose contraction results in a leaf
    ### locking link - given a twin link ab, find a minimal proper rooted subtree Tv such that L(Tv) = {a,b,b'}, bb' is a link, and Tv is a-closed
    leaf_to_leaf = L.copy()

    # book-keep the locking links and the twins they spawned from. note this is not all twins
    twins = []
    locking_links = []

    find_locking_twins(T, L, locking_links, twins, leaf_to_leaf)       

    # obtain a maximal matching on this link-set - leaf-to-leaf links which are nontwin + nonlocking
    matching = nx.maximal_matching(leaf_to_leaf)

    # matching = set([(9,10),(22,14),(16,24),(26,27)])

    # exhaust greedy locking contractions
    while(True):
        greedy_locking(T, L, I, matching, links, twins, locking_links) 
        twins = []
        locking_links = []
        find_locking_twins(T, L, locking_links, twins, None)
         
        if len(twins) == 0:
            break

    while len(T.nodes()) > 1:
        greedy_link(T,L,I,matching, links)
        if(len(T.nodes()) == 1):
            break

        T = find_minimally_semiclosed(T, L, matching, links, I)

    print(len(I.edges()))
    return len(I.edges())

def find_locking_twins(T, L, locking_links, twins, leaf_to_leaf):
    # filter out edges which are not leaf-to-leaf, twin, or locking
    for link in L.edges():

        # if the link is not leaf-to-leaf, remove it
        if T.degree(link[0]) != 1 or T.degree(link[1]) != 1:
            leaf_to_leaf.remove_edge(link[0], link[1]) if leaf_to_leaf is not None else None
            continue

        # if the link is a twin link, remove it
        if check_twin_link(T, link):
            leaf_to_leaf.remove_edge(link[0], link[1]) if leaf_to_leaf is not None else None

            # 'link' is a twin link. check if there is a locking link spawning from it

            # first determine the path to the root to follow to look for T'
            path_to_root = [x for x in nx.shortest_path(T, next(T.neighbors(link[1])), root_node) if x in nx.shortest_path(T, next(T.neighbors(link[0])), root_node)][1:]

            # check each possble rooted subtree in T
            for curr_node in path_to_root:

                # if the current rooted subtree is not proper, there is no locking link
                if curr_node == root_node:
                    break

                # find the parent of the current subtree root. This helps to cut it off from the rest of the tree to observe the subtree
                avoid_node = path_to_root[path_to_root.index(curr_node) + 1]

                # find the leaves of the subtree
                subtree_leaves = find_descendant_leaves(T, curr_node, avoid_node)

                # if the subtree has 3 leaves, it is a candidate for a locking link
                if len(subtree_leaves) == 3:

                    # using the avoid_node, cut the edge between it and the subtree we desire
                    subtree = subtree_finder(T, curr_node, avoid_node)
                    subtree_nodes = set(subtree.nodes())

                    # let b' be the leaf not in the twin link
                    bprime = next(iter(subtree_leaves - set(link)))

                    # check if ab' or bb' is an link in L

                    # Let b=link[0]. if bb' is a link in L:
                    if (link[0], bprime) in L.edges() or (bprime, link[0]) in L.edges():
                        incident_links = [(u, v) for u, v in L.edges() if u == link[1] or v == link[1]]
                        
                        # verify the subtree is a-closed
                        for ilink in incident_links:
                            if ilink[0] not in subtree_nodes or ilink[1] not in subtree_nodes:
                                break
                        else:
                            # a-closed -> locking link found: bb'
                            locking = (link[0], bprime) if (link[0], bprime) in L.edges() else (bprime, link[0])
                            locking_links.append(locking)
                            
                            # locking edge may have already been removed due to being a twin link
                            leaf_to_leaf.remove_edge(*locking) if leaf_to_leaf is not None and (locking) in leaf_to_leaf.edges() else None

                            twins.append(link)

                            break

                    # let b=link[1]. if ab' is a link in L:
                    if (link[1], bprime) in L.edges() or (bprime, link[1]) in L.edges():
                        incident_links = [(u, v) for u, v in L.edges() if u == link[0] or v == link[0]]
                        
                        # verify the subtree is a-closed
                        for ilink in incident_links:
                            if ilink[0] not in subtree_nodes or ilink[1] not in subtree_nodes:
                                break
                        else:
                            # a-closed -> locking link found: bb'
                            locking = (link[0], bprime) if (link[0], bprime) in L.edges() else (bprime, link[0])
                            locking_links.append(locking)
                            
                            # locking edge may have already been removed due to being a twin link
                            leaf_to_leaf.remove_edge(*locking) if leaf_to_leaf is not None and (locking) in leaf_to_leaf.edges() else None

                            twins.append(link)

                            break

                # if the subtree has more than 3 leaves, quit looking for locking, we've gone too close to the root
                elif len(subtree_leaves) > 3:
                    break             

def check_twin_link(tree, link):
    """ 
    Given a tree and a link, return if the link is a twin link.\n
    A twin link is a link that when contracted forms a leaf
    """
    # twin link must be leaf-to-leaf
    if tree.degree(link[0]) != 1 or tree.degree(link[1]) != 1:
        return False
    
    # see if contraction results in a leaf
    copy = tree.copy()
    contract(copy, False, link, None, None, None, None, False)
    
    # if a leaf is formed, instead of the contraction eliminating two leaves, only one will be eliminated
    if len([node for node in copy.nodes() if copy.degree(node) == 1]) == len([node for node in tree.nodes() if tree.degree(node) == 1]) - 2:
        return False
    return True

def find_minimally_semiclosed(T, L, M, original_linkset, I):

    # look for a minimally semiclosed tree which is not dangerous

    # however, keep track of those which are dangerous in case all minimally semiclosed T' are dangerous
    dangerous_subtree_list = [[],[]]

    # Find all leaf nodes
    leaves = [node for node in T.nodes() if T.degree(node) == 1 and node != root_node]

    # Initialize a queue for BFS traversal
    # look at rooted subtrees in order of distance from the root
    pq = queue.PriorityQueue()
    for leaf in leaves:
        pq.put((-len(nx.shortest_path(T, leaf, root_node)), leaf))

    # Initialize a set to keep track of visited nodes
    visited = set(leaves)

    # Perform BFS traversal
    while pq.qsize() > 0:
        current_node = pq.get()[1]
        path = nx.shortest_path(T, current_node, root_node)
        toAvoid = path[path.index(current_node) + 1] if current_node != root_node else None
        curr_rooted_subtree = subtree_finder(T, current_node, toAvoid)

        if M_compatible(set(curr_rooted_subtree.nodes()), M):
            if unmatched_leaf_closed(set(curr_rooted_subtree.nodes()), T, L, M):
                if not dangerous(curr_rooted_subtree, T, L, M):
                    if original_linkset is not None:    
                        #print("found not dangerous")
                        toContract = set()
                        for link in M:
                            if link[0] in curr_rooted_subtree.nodes() and link[1] in curr_rooted_subtree.nodes():
                                toContract.add(link)
                        unmatched_leaves = [node for node in curr_rooted_subtree if T.degree(node) == 1 and all(node not in match for match in M)]
                        for leaf in unmatched_leaves:
                            toContract.add(uplink(T, L, leaf))
                        while len(toContract) > 0:
                            contract(T, True, toContract.pop(), I, L, original_linkset, [toContract, M], True)
                        return T
                    else:
                        return curr_rooted_subtree
                else:
                    #print("DANGEROUS")
                    if sum(1 for n in curr_rooted_subtree.nodes() if curr_rooted_subtree.degree(n) == 1) == 3:
                        dangerous_subtree_list[0].append(curr_rooted_subtree)
                    else:
                        dangerous_subtree_list[1].append(curr_rooted_subtree)

                    for path_node in nx.shortest_path(T, current_node, root_node):
                        visited.add(path_node)

            # if so, check if dangerous
            # if not, return the tree and its exact cover
    
        parent_node = [neighbor for neighbor in T.neighbors(current_node) if neighbor not in visited and neighbor != root_node and neighbor in nx.shortest_path(T, current_node, root_node)]
        #print("for", current_node, "parent node", parent_node)
        pq.put((-len(nx.shortest_path(T, parent_node[0], root_node)), parent_node[0])) if parent_node else None
        visited.update(parent_node)

    if len(dangerous_subtree_list[0]) == 0 and len(dangerous_subtree_list[1]) == 0:
        #print("T'= T")
        toContract = set()
        for link in M:
            toContract.add(link)
        unmatched_leaves = [node for node in curr_rooted_subtree if T.degree(node) == 1 and all(node not in match for match in M)]
        for leaf in unmatched_leaves:
            toContract.add(uplink(T, L, leaf))
        while len(toContract) > 0:
            #print("contracting - last step", toContract)
            #print("TREE", T.edges())
            contract(T, True, toContract.pop(), I, L, original_linkset, [toContract, M], True)
        return T
    else:

        T_prime, cover = dangerous_scheme(T.copy(), L.copy(), M.copy(), dangerous_subtree_list)
        cover = set(cover.edges())
        while(len(cover) > 0):
            #print(cover)
            contract(T, True, cover.pop(), I, L, original_linkset, [cover, M], True)
        return T



# finds a non-dangerous semiclosed tree T' and its exact cover I' when all minimally semiclosed trees are dangerous
def dangerous_scheme(T, L, M, dangerous_list):
    #print("DANGEROUS SCHEME")
    W_tilde = nx.Graph()
    for subtree in dangerous_list[1]:
        leaves = [node for node in subtree.nodes() if T.degree(node) == 1]
        for pair in combinations(leaves, 2):
            # if twin link exists (two leaves have same parent, which has exactly two leaves, and the two leaves are connected by a link in L)
            if next(subtree.neighbors(pair[0])) == next(subtree.neighbors(pair[1])) and T.degree(next(T.neighbors(pair[0]))) == 3 and sum(1 for n in subtree.neighbors(next(subtree.neighbors(pair[0]))) if subtree.degree(n) == 1) == 2 and (pair in L.edges() or pair[::-1] in L.edges()):
                W_tilde.add_edge(*pair)
    #print("W_tilde", W_tilde.edges())
    twin_link_dict = {}
    modified_4_graphs = []
    for twin_link in W_tilde.edges():
        twin_link_dict[next(T.neighbors(twin_link[0]))] = twin_link
        #print("contracting along", twin_link)
        contract(T, True, twin_link, None, L, None, [M], False)
        for subgraph in dangerous_list[1]:
            if twin_link[0] in subgraph.nodes() and twin_link[1] in subgraph.nodes():
                subcopy = subgraph.copy()
                #print(M)
                contract(subcopy, False, twin_link, None, L, None, [M], False)
                #print(M)
                modified_4_graphs.append(subcopy)
                dangerous_list[1].remove(subgraph)
    
    #print(twin_link_dict)
    M_tilde = set()
    for match in M:
        for subtree in dangerous_list[0] + modified_4_graphs:
            if match[0] in subtree.nodes() and match[1] in subtree.nodes():
                a = [node for node in subtree.nodes() if T.degree(node) == 1 if node != match[0] and node != match[1]][0]
                if next(subtree.neighbors(a)) != next(subtree.neighbors(match[0])) and ((a, match[0]) in L.edges() or (match[0], a) in L.edges()):
                    M_tilde.add((a,match[0]))
                else:
                    M_tilde.add((a,match[1]))

    #print("M_tilde", M_tilde)

    T_tilde_prime = find_minimally_semiclosed(T, L, M_tilde, None, None)
    I_tilde_prime = nx.Graph()
    for link in M_tilde:
        if link[0] in T_tilde_prime.nodes() and link[1] in T_tilde_prime.nodes():
            I_tilde_prime.add_edge(*link)

    for unmatched_leaf in [node for node in T_tilde_prime.nodes() if T.degree(node) == 1 and not any(node in edge for edge in M_tilde)]:
        #print(*uplink(T, L, unmatched_leaf))
        I_tilde_prime.add_edge(*uplink(T, L, unmatched_leaf))

    #print(twin_link_dict)
    for parent in twin_link_dict:
        if twin_link_dict[parent][0] in [node for node in T_tilde_prime.nodes() if T_tilde_prime.degree(node) == 1]:
            T_tilde_prime = T_tilde_prime.copy()
            nx.relabel_nodes(T_tilde_prime, {twin_link_dict[parent][0] : parent}, copy=False)
            T_tilde_prime.add_edge(parent, twin_link_dict[parent][0])
            T_tilde_prime.add_edge(parent, twin_link_dict[parent][1])
            I_tilde_prime.add_edge(twin_link_dict[parent][0], twin_link_dict[parent][1])

    toRemove = []
    for (u, v) in I_tilde_prime.edges():
        for parent in twin_link_dict:
            if u == twin_link_dict[parent][0]:
                toRemove.append(((u,v), (parent, v)))
            if v == twin_link_dict[parent][0]:
                toRemove.append(((u,v), (parent, u)))

    while len(toRemove) > 0:
        pair = toRemove.pop()
        I_tilde_prime.remove_edge(*pair[0])
        I_tilde_prime.add_edge(*pair[1])

    #print("T_tilde_prime", T_tilde_prime.edges())
    #print("I_tilde_prime", I_tilde_prime.edges())
    return T_tilde_prime, I_tilde_prime

def dangerous(subtree, T, L, M):
    """
    check if a subtree is dangerous - if it is one of two configurations laid out in the paper
    """
    if len([node for node in subtree.nodes() if T.degree(node) > 1 and T.nodes[node]['coupons'] == 1]) == 0:
        if len([(u, v) for u, v in M if u in subtree.nodes() and v in subtree.nodes()]) == 1:
            matched_leaves = [(u, v) for u, v in M if u in subtree.nodes() and v in subtree.nodes()]
            leaf_list = [node for node in subtree.nodes() if T.degree(node) == 1]
            if len(leaf_list) == 3:
                if (leaf_list[0], leaf_list[1]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[1])) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                        return False
                if (leaf_list[0], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[2])) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                        return False
                if (leaf_list[1], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[2])) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                        return False
                # no stems               
                a = leaf_list[0] if leaf_list[0] not in matched_leaves[0] else leaf_list[1] if leaf_list[1] not in matched_leaves[0] else leaf_list[2]
                if (a,matched_leaves[0][0]) in L.edges() or (matched_leaves[0][0],a) in L.edges():
                    link = (a,matched_leaves[0][0]) if (a,matched_leaves[0][0]) in L.edges() else (matched_leaves[0][0],a)
                    Tcopy = T.copy()
                    subcopy = subtree.copy()
                    start_leaves = len([node for node in subtree.nodes() if T.degree(node) == 1])
                    #print("start number", start_leaves)
                    contract(Tcopy, False, link, None, L.copy(), None, [M.copy()], False)
                    contract(subcopy, False, link, None, L.copy(), None, [M.copy()], False)
                    if start_leaves == len([node for node in subcopy.nodes() if Tcopy.degree(node) == 1]) + 2:
                        if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                            return True
                elif (a,matched_leaves[0][1]) in L.edges() or (matched_leaves[0][1],a) in L.edges():
                    link = (a,matched_leaves[0][1]) if (a,matched_leaves[0][1]) in L.edges() else (matched_leaves[0][1],a)
                    Tcopy = T.copy()
                    subcopy = subtree.copy()
                    start_leaves = len([node for node in subtree.nodes() if T.degree(node) == 1])
                    contract(Tcopy, False, link, None, L.copy(), None, [M.copy()], False)
                    contract(subcopy, False, link, None, L.copy(), None, [M.copy()], False)
                    if start_leaves == len([node for node in subcopy.nodes() if Tcopy.degree(node) == 1]) + 2:
                        if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                            return True
                
            elif len(leaf_list) == 4:
                stem_count = 0
                stem = [None, []]
                if (leaf_list[0], leaf_list[1]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[1])) and T.degree(next(T.neighbors(leaf_list[0]))) == 3 and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[0])), [leaf_list[0], leaf_list[1]]]
                if (leaf_list[0], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[2])) and T.degree(next(T.neighbors(leaf_list[0]))) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[0])), [leaf_list[0], leaf_list[2]]]
                if (leaf_list[0], leaf_list[3]) in L.edges():
                    if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[3])) and T.degree(next(T.neighbors(leaf_list[0]))) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[0])), [leaf_list[0], leaf_list[3]]]
                if (leaf_list[1], leaf_list[2]) in L.edges():
                    if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[2])) and T.degree(next(T.neighbors(leaf_list[0]))) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[1]))) if subtree.degree(n) == 1) == 2:
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[1])), [leaf_list[1], leaf_list[2]]]
                if (leaf_list[1], leaf_list[3]) in L.edges():
                    if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[3])) and T.degree(next(T.neighbors(leaf_list[0]))) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[1]))) if subtree.degree(n) == 1) == 2:
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[1])), [leaf_list[1], leaf_list[3]]]
                if (leaf_list[2], leaf_list[3]) in L.edges():
                    if next(T.neighbors(leaf_list[2])) == next(T.neighbors(leaf_list[3])) and T.degree(next(T.neighbors(leaf_list[0]))) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[2]))) if subtree.degree(n) == 1) == 2:
                        stem_count += 1
                        stem = [next(T.neighbors(leaf_list[2])), [leaf_list[2], leaf_list[3]]]
                if stem_count == 1:
                    if stem[1][0] in matched_leaves[0] and stem[1][1] not in matched_leaves[0] or stem[1][1] in matched_leaves[0] and stem[1][0] not in matched_leaves[0]:
                        subtree = nx.Graph(subtree)
                        newM = M.copy()
                        contract(subtree, True, (stem[1][0], stem[1][1]), None, L, None, [newM], False)
                        matched_leaves = [(u, v) for u, v in newM if u in subtree.nodes() and v in subtree.nodes()]
                        leaf_list = [node for node in subtree.nodes() if T.degree(node) == 1]
                        if len(leaf_list) == 3:
                            if (leaf_list[0], leaf_list[1]) in L.edges():
                                if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[1])) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                                    return False
                            if (leaf_list[0], leaf_list[2]) in L.edges():
                                if next(T.neighbors(leaf_list[0])) == next(T.neighbors(leaf_list[2])) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                                    return False
                            if (leaf_list[1], leaf_list[2]) in L.edges():
                                if next(T.neighbors(leaf_list[1])) == next(T.neighbors(leaf_list[2])) and sum(1 for n in subtree.neighbors(next(subtree.neighbors(leaf_list[0]))) if subtree.degree(n) == 1) == 2:
                                    return False
                            # no stems
                            a = leaf_list[0] if leaf_list[0] not in matched_leaves[0] else leaf_list[1] if leaf_list[1] not in matched_leaves[0] else leaf_list[2]
                            if (a,matched_leaves[0][0]) in L.edges() or (matched_leaves[0][0],a) in L.edges():
                                link = (a,matched_leaves[0][0]) if (a,matched_leaves[0][0]) in L.edges() else (matched_leaves[0][0],a)
                                Tcopy = T.copy()
                                subcopy = subtree.copy()
                                start_leaves = len([node for node in subtree.nodes() if T.degree(node) == 1])
                                #print("start number", start_leaves)
                                contract(Tcopy, False, link, None, L.copy(), None, [M.copy()], False)
                                contract(subcopy, False, link, None, L.copy(), None, [M.copy()], False)
                                if start_leaves == len([node for node in subcopy.nodes() if Tcopy.degree(node) == 1]) + 2:
                                    if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                                        return True
                            elif (a,matched_leaves[0][1]) in L.edges() or (matched_leaves[0][1],a) in L.edges():
                                link = (a,matched_leaves[0][1]) if (a,matched_leaves[0][1]) in L.edges() else (matched_leaves[0][1],a)
                                Tcopy = T.copy()
                                subcopy = subtree.copy()
                                start_leaves = len([node for node in subtree.nodes() if T.degree(node) == 1])
                                contract(Tcopy, False, link, None, L.copy(), None, [M.copy()], False)
                                contract(subcopy, False, link, None, L.copy(), None, [M.copy()], False)
                                if start_leaves == len([node for node in subcopy.nodes() if Tcopy.degree(node) == 1]) + 2:
                                    if len([(u,v) for u, v in L.edges() if u == matched_leaves[0][1] and v not in subtree.nodes() or v == matched_leaves[0][1] and u not in subtree.nodes()]) > 0:
                                        return True

    return False
                



def unmatched_leaf_closed(subtree, T, L, M):
    """
    verify if a subtree is unmatched leaf closed. That is, unmatched leaves have no links leading outside the subtree
    """
    unmatched_leaves = [node for node in subtree if T.degree(node) == 1 and all(node not in match for match in M)]

    for leaf in unmatched_leaves:
        incident_links = [(u, v) for u, v in L.edges() if u == leaf or v == leaf]
        for link in incident_links:
            if link[0] not in subtree or link[1] not in subtree:
                return False
    return True

def M_compatible(subtree, M):
    """
    verify if a tree is M-compatible. That is, no link in M has an endpoint in the subtree and the other endpoint outside the subtree
    """
    for link in M:
        if link[0] not in subtree and link[1] in subtree or link[1] not in subtree and link[0] in subtree:
            return False
    return True
    

def uplink(tree, linkset, node):
    """
    finds the closest node in the tree to the root which links to the given node
    """

    # format of answer: (node, distance to root)
    closest = [None, len(tree.nodes())]

    # consider all links incident to the given node
    incident_links = [(u, v) for u, v in linkset.edges() if u == node or v == node]

    for link in incident_links:
        # save the tuple element which is not the node
        other_node = link[0] if link[1] == node else link[1]

        path = nx.shortest_path(tree, other_node, root_node)

        if len(path) < closest[1]:
            closest = [other_node, len(path)]

    return (node, closest[0]) if (node, closest[0]) in linkset.edges() else (closest[0], node)


def subtree_finder(tree, toKeep, throwAway):
    """
    Given a tree, a node to keep, and a node to throw away, return the subtree rooted at the node to keep
    """

    # improper subtree case
    if (toKeep == root_node):
        return tree
    
    # Make a copy of the tree
    tree_copy = tree.copy()
    
    # Remove the specified edge
    tree_copy.remove_edge(toKeep, throwAway)
    
    # Get the connected components
    connected_components = list(nx.connected_components(tree_copy))
    
    # Find and return the connected component containing the node to keep
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
    #print(root_node)
    #print("tree", tree.edges())
    #print("links", links)
    # Perform BFS traversal on the tree graph
    for edge in nx.bfs_edges(tree, root_node):
        # Get the descendants of the child node in the current edge
        descendants = list(descendants_dict.get(edge[1], []))
        descendants.append(edge[1])
        #print("trying node ", edge[1])
        #print("descendents", descendants)
        
        # Check if any leaf node has an edge outside of its descendants in the tree
        for link in links:
            if link[0] in leaf_nodes and link[0] in descendants and link[1] not in descendants or link[1] in leaf_nodes and link[1] in descendants and link[0] not in descendants:
                break
        else:
            # No leaf node has an external edge, root the subtree at edge[1]
            return tree.subgraph(descendants)

def greedy_link(T,L,I,matching, original_linkset):
    """
    Greedy contraction: Find and contract a leaf-to-leaf link with neither endpoint in the matching
    """

    while True:
        for link in L.edges():

            if T.degree(link[0]) == 1 and T.degree(link[1]) == 1:
                for matched in matching:
                    if link[0] in matched or link[1] in matched:
                        break
                else:
                    contract(T, True, link, I, L, original_linkset, [matching], True)
                    break
        else:
            break

def greedy_locking(tree, links, I, matching, original_linkset, twins, locking_links):
    """
    Given a tree, and the twins and locking tree found in a rooted proper subtree, contract them
    """
    for i in range(len(twins)):
        # twin = ab, locking = bb'
        a = twins[i][0] if twins[i][1] in locking_links[i] else twins[i][1]

        # contract bb'
        contract(tree, True, locking_links[i], I, links, original_linkset, [matching, locking_links, twins], True)

        # contract a,uplink(a)
        contract(tree, True, uplink(tree, links, a), I, links, original_linkset, [matching, twins], True)
            

def contract(tree, not_copy, link, I, links, original_linkset, lists_to_keep_consistent, append_to_I):
    """
    Contract the edges covered by a given link.\n
    Options:\n 
             not_copy - if True, the global root node value is updated if necessary\n
             I - the solution set to update. Can pass I: None, append_to_I: False\n
             links - the link set to update. Possible to pass links: None\n
             original_linkset - the original link set to reference when appending links to I. Possible to pass original_linkset: None\n
             lists_to_keep_consistent - a list of lists to keep consistent with the contraction. Ex: Matchings, list of links to contract, etc. May pass several\n
             append_to_I - if True, the link is appended to the solution set I\n
    """
    global root_node
    toContract = []

    # determine the set of edges covered by the link
    shadow = nx.shortest_path(tree, link[0], link[1])

    # perpare each edge for contraction
    for i in range(len(shadow)-1):
        toContract.append([shadow[i],shadow[i+1]]) if [shadow[i],shadow[i+1]] not in toContract and [shadow[i+1],shadow[i]] not in toContract else None

    # contract each edge covered by the link
    while(not len(toContract) == 0):

        # update the root node if necessary
        if toContract[0][1] == root_node and not_copy:
            root_node = toContract[0][0]

        # contract the first edge in the list
        nx.contracted_edge(tree,tuple(toContract[0]),self_loops=False, copy=False)

        # add the tree edge to L for the sake of contracting to see the resulting link configuration
        if links is not None and tuple(toContract[0]) not in links.edges() and tuple(toContract[0])[::-1] not in links.edges():
            links.add_edge(*tuple(toContract[0]))
        nx.contracted_edge(links,tuple(toContract[0]),self_loops=False, copy=False) if links is not None else None

        # if any other edge in our list is adjacent to the "dest" node of contraction, re-index it
        for toFix in toContract[1:]:
            if(toFix[0] == toContract[0][1]):
                toFix[0] = toContract[0][0]
            elif(toFix[1] == toContract[0][1]):
                toFix[1] = toContract[0][0]

        # keep the lists consistent
        if lists_to_keep_consistent is not None:
            for list in lists_to_keep_consistent:
                for pair in list.copy():
                    if(pair[0] == toContract[0][1]):
                        list.remove(pair)
                        if pair[1] != toContract[0][0]:
                            list.add((toContract[0][0], pair[1]))
                    elif(pair[1] == toContract[0][1]):
                        list.remove(pair)
                        if pair[0] != toContract[0][0]:
                            list.add((pair[0],toContract[0][0]))

        # give the compound node a coupon
        tree.nodes[toContract[0][0]]['coupons'] = 1

        # remove the edge we contracted from our list, 
        toContract.remove(toContract[0])

    # append to the solution if necessary
    if(append_to_I):
        print("adding", link, "to solution")
        add_to_solution(I, original_linkset, link) if I is not None and link not in I.edges() and link[::-1] not in I.edges() else None

def add_to_solution(I, linkset, link):
    """
    given a link between compound nodes to append to the solution I, this procedure determines which edge can be added 
    in the original linkset

    """
    I.add_edge(*link)


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
    #T.add_edges_from([(0,1), (0,2), (0,3), (2,4), (2,5), (3,6), (3,7), (4,8), (4,9), (4,10), (5,11), (7,12), (8,13), (8,14), (11,15), (11,17), (11,16), (12,18), (13, 19), (13, 20), (20, 21), (20, 22), (17, 23), (17, 24), (18, 25), (18, 26), (18, 27)])
    T.add_edges_from([(7,2),(2,4),(4,1),(1,9),(9,6),(6,0),(1,8),(8,3),(8,5)])
    L = nx.Graph()
    # L.add_edges_from([(1,2), (2,20), (2,16), (4,6), (5,17), (6,18), (7,26), (9,10), (10,14), (14, 19), (14, 22), (15, 16), (15,17), (16,24), (21,22), (23,24),(25,26),(25,27),(26,27), (12,27)])
    L.add_edges_from([(0, 1), (0, 2), (0, 6), (0, 7), (0, 8), (0, 9), (0, 5), (1, 2), (1, 3), (1, 4), (1, 5), (1, 6), (1, 7), (1, 8), (2, 3), (2, 7), (2, 9), (2, 4), (2, 6), (3, 4), (3, 5), (3, 7), (3, 8), (4, 5), (4, 6), (4, 8), (4, 9), (6, 5), (6, 7), (6, 9), (7, 5), (8, 5), (8, 9), (9, 5)]  )
    even(T, L)
    return

if __name__ == '__main__':
    main()