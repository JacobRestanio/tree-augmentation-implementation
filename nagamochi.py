import sys
import os
import copy

class Edge:
    def __init__(self, thisVertex, otherVertex):
        self.thisVertex = thisVertex
        self.otherVertex = otherVertex

class EdgeLs:
    def __init__(self, e):
        self.e = e
        self.next = None
        self.prev = None

class SwingLs:
    def __init__(self, up, down, e):
        self.up = up
        self.down = down
        self.p_up = get_parent(t,up)
        self.p_down = get_parent(t,down)
        self.e = e
        self.is_solo_edge = 0
        self.in_lower = 0
        self.next = None
        self.prev = None
        self.binding_edges = None

class ChainLs:
    def __init__(self, u, u2, uk):
        self.next = None
        self.prev = None
        self.u = u
        self.u2 = u2
        self.uk = uk
        self.ua = 0
        self.swings = None
        self.binding_edges = None
        self.swing_edges = None
        self.e_p = None

import sys

lemma7_min_edges2 = sys.maxsize
lemma_7_edges2 = None

def lemma7(g, t, r, approx):
    global lemma7_min_edges2
    global lemma_7_edges2
    
    l = (4/approx) - 1
    lemma7_min_edges2 = sys.maxsize # reset max
    
    if lemma_7_edges2:
        l_free(lemma_7_edges2)
        lemma_7_edges2 = None
    
    des = descendants(t, r)
    newold = []
    oldnew = []
    a = []
    b = []
    
    g_new, oldnew, newold = graph_copy(g, r, des, oldnew, newold)
    t_new, a, b = graph_copy(t, r, des, a, b)
    
    g_new = normal_copy(g)
    t_new = normal_copy(t)
    
    lemma7_helper(g_new, t_new, r, approx, 0, None)
    
    print("\nlemma 7 output:")
    
    # print pairs
    for p in l_last(lemma_7_edges2):
        u1 = value(g, p.u)
        u2 = value(g, p.v)
        e = find_edge(g, u1, u2)
        
        if not e:
            print(f"lemma 7 err: edge ({u1}, {u2}) not found")
        else:
            print(f"({e.thisVertex}, {e.otherVertex})  \t{p.blossom_number}")
        
        sys.stdout.flush()
    
    print("now merging")
    
    # print pairs and remove
    for p in l_last(lemma_7_edges2):
        u1 = value(g, p.u)
        u2 = value(g, p.v)
        e = find_edge(g, u1, u2)
        
        if not e:
            print(f"lemma 7 err: edge ({u1}, {u2}) not found")
        else:
            print(f"({e.thisVertex}, {e.otherVertex})  \t{p.blossom_number}")
            retain_merge_trim(g, t, u1, u2)
        
        sys.stdout.flush()
    
    print("\n\n")
    graph_print_all(g)
    graph_free(g_new)
    free(newold)
    free(oldnew)
    ls_free(des)

def trim_lowest_edges(g, t, r, leaves):
    original_root = t.root
    
    lf = leaves
    while lf:
        z = lf.value
        set_root(t, z)
        z_incident = None
        
        for e in g.vert[z].edge:
            z_incident = ls_add(z_incident, value(g, e.otherVertex))
        
        u = z_incident
        while u:
            uu = u.value
            des = descendants(t, uu)
            
            while ls_contains(des, uu):
                des = ls_contains(des, uu)
                des = ls_remove(des)
                des = ls_first(des)
            
            if ls_contains_any(des, z_incident):
                while remove_edge(g, uu, z):
                    pass
            
            ls_free(des)
            u = u.next
        
        ls_free(z_incident)
        lf = lf.next
    
    set_root(t, original_root)

def pair_ls_copy(ls):
    new_ls = None
    
    for ls in l_last(ls):
        u = ls.u
        v = ls.v
        bl = ls.blossom_number
        new_pair = pair_create(u, v, bl)
        new_ls = l_add(new_ls, new_pair)
    
    return new_ls

times_ran = 0

def lemma7_helper(prev_g, t, r, approx, recur_depth, cur_edges):
    global times_ran
    g = normal_copy(prev_g)
    any_cases = 1
    
    while any_cases:
        any_cases = 0
        c = 0
        
        while c = case1(g, t):
            any_cases |= c
        
        while c = case2(g, t):
            any_cases |= c
    
    for retained in g.retain:
        e = retained
        cur_edges = l_add(cur_edges, pair_create(e.thisVertex, e.otherVertex, recur_depth))
    
    l = (4/approx) - 1
    r = value(g, r)
    cur_size = l_size(cur_edges)
    
    print(f"lemma7_helper: cur_size: {cur_size}    recur_depth: {recur_depth}  times_ran {times_ran}")
    
    leafs = leaves(t, r)
    n_leaves = l_size(leafs)
    
    print(f"r: {r},  lf->v: {leafs.value}    n_lfs:{n_leaves}")
    
    if leafs.value == r or g.vert[leafs.value].edge == None:
        if cur_size < lemma7_min_edges2:
            lemma7_min_edges2 = cur_size
            
            if lemma_7_edges2:
                l_free(lemma_7_edges2)
            
            lemma_7_edges2 = cur_edges
            return
        
        l_free(cur_edges)
        l_free(leafs)
        return
    
    trim_lowest_edges(g, t, r, leafs)
    
    lf_bytes = sizeof(int) * n_leaves
    lf_arr = malloc(lf_bytes)
    memset(lf_arr, 0, lf_bytes)
    
    e_k_bytes = sizeof(edge*) * n_leaves
    e_k = malloc(e_k_bytes)
    memset(e_k, 0, e_k_bytes)
    
    c_lf = leafs
    i = 0
    
    while c_lf and i < n_leaves:
        ll = c_lf.value
        lf_arr[i] = ll
        e_k[i] = g.vert[ll].edge
        c_lf = c_lf.next
        
        print(f"{i}  cur_lf: {ll}   {e_k[i]}")
        i += 1
    
    combination_number = 1
    
    for i in range(l*l):
        if recur_depth == 0:
            print(f"combination number: {combination_number}")
        
        new_g = normal_copy(g)
        new_t = normal_copy(t)
        new_edges = pair_ls_copy(cur_edges)
        
        for k in range(n_leaves):
            e = e_k[k]
            
            if not e:
                continue
            
            u = value(new_g, e.thisVertex)
            v = value(new_g, e.otherVertex)
            
            if u == v or u == 0 or v == 0:
                continue
            
            new_edges = l_add(new_edges, pair_create(e.thisVertex, e.otherVertex, recur_depth))
            retain_merge_trim(new_g, new_t, u, v)
        
        lemma7_helper(new_g, new_t, r, approx, recur_depth+1, new_edges)
        
        j = n_leaves - 1
        overflow = 0
        
        while True:
            print(f"e_k[j]: {e_k[j]}")
            e_k[j] = e_k[j].next
            
            if e_k[j] == None:
                e_k[j] = g.vert[lf_arr[j]].edge
                overflow = 1
            
            if recur_depth == 0:
                print(f"lf_arr[j]: {lf_arr[j]}  \tj: {j}  \t of: {overflow}")
            
            j -= 1
            
            if not overflow or j < 0:
                break
        
        if j < 0:
            if recur_depth == 0:
                print("combination overflow!")
            
            break
    
    l_free(leafs)
    free(lf_arr)
    free(e_k)

def print_edge_ls_fn(el):
    e = el.e
    print(f"({e.thisVertex},{e.otherVertex})", end="")

def edge_ls_copy(el):
    ret = None
    while el:
        ret = l_add(ret, edge_ls_create(el.e))
        el = el.next
    return ret

def print_edge_ls(el):
    l_print(el,print_edge_ls_fn)

def edge_ls_create(e):
    nn = EdgeLs()
    nn.e = e
    nn.next = None
    nn.prev = None
    return nn

def create_swing(t, up, down, e):
    swing = SwingLs()
    swing.up = up
    swing.down = down
    swing.p_up = get_parent(t,up)
    swing.p_down = get_parent(t,down)
    swing.e = e
    swing.is_solo_edge = 0
    swing.in_lower = 0
    swing.next = None
    swing.prev = None
    swing.binding_edges = None
    return swing

def create_chain(u, u2, uk):
    chain = ChainLs()
    chain.next = None
    chain.prev = None
    chain.u = u
    chain.u2 = u2
    chain.uk = uk
    chain.ua = 0
    chain.swings = None
    chain.binding_edges = None
    chain.swing_edges = None
    chain.e_p = None
    return chain

def p_ch(chain):
    chain_ls = chain
    print(f"(u:{chain_ls.u} uk:{chain_ls.uk} ua:{chain_ls.ua}), ", end="")

def print_chain(chain_ls):
    l_print(chain_ls, p_ch)

def p_swing(swings):
    swing = swings
    print(f"(up:{swing.up} down:{swing.down} p_up:{swing.p_up} p_down:{swing.p_down})\n ", end="")
    print("\t\tbinding_edges: ", end="")
    print_edge_ls(swing.binding_edges)
    if swing.is_solo_edge:
        print(" solo-edge!", end="")
    print("\n")

def print_swings(swings):
    l_print(swings,p_swing)

def p_ch_n_s(chain):
    chain_ls = chain
    print(f"chain(u:{chain_ls.u} uk:{chain_ls.uk} ua:{chain_ls.ua} uo:{chain_ls.e_p.otherVertex})\n", end="")
    print("\tbinding_edges: ", end="")
    print_edge_ls(chain_ls.binding_edges)
    print("\n", end="")
    if chain_ls.swings:
        print("\tswings: ", end="")
        print_swings(chain_ls.swings)
        print("\n", end="")
        sys.stdout.flush()

def print_chain_and_swings(chain_ls):
    l_print(chain_ls, p_ch_n_s)

def branches(t, v):
    d = descendants(t,v)
    b = None
    cd = d
    while cd:
        u = value(t,cd.value)
        if is_branch(t,u):
            b = ls_add(b,u)
        cd = cd.next
    ls_free(d)
    return b

def find_chains(t, v):
    v = value(t,v)
    chains = None
    brnches = branches(t,v)
    b = brnches
    while b:
        u = value(t,b.value)
        queue = children(t,u)
        end_que = ls_last(queue)
        cur_que = queue
        while cur_que:
            cv = value(t,cur_que.value)
            if is_leaf(t,cv):
                cur_que = cur_que.next
                continue
            if is_branch(t,cv) or is_fringe(t,cv):
                u2 = cv
                while u!=get_parent(t,u2 = get_parent(t,u2)) and u2 != t.root:
                    new_chain = create_chain(u,u2,cv)
                    chains = l_add(chains,new_chain)
            else:
                next = children(t,cv)
                end_que = ls_last(ls_merge(end_que,next))
            cur_que = cur_que.next
        ls_free(queue)
        b = b.next
    return chains

def next_in_chain(t, v):
    v = value(t,v)
    if is_branch(t,v):
        return 0
    kids = children(t,v)
    v = 0
    kid = kids
    while kid:
        v = value(t,kid.value)
        if not is_leaf(t,v):
            break
        kid = kid.next
    ls_free(kids)
    return v

def immediate_thorns(t, v):
    kids = children(t,v)
    kid = kids
    while kid:
        if not is_leaf(t,kid.value):
            kid = ls_remove(kid)
            break
        kid = kid.next
    return ls_first(kid)

def upper_edge(g, t, chain):
    u = value(g,chain.u)
    uk = value(g,chain.uk)
    u2 = value(g,chain.u2)
    d_u2 = descendants(t, u2)
    for v in range(uk, u, -1):
        thorns_and_v = None
        if v != uk:
            thorns_and_v = ls_merge(immediate_thorns(t,v),ls_add(None,v))
        else:
            thorns_and_v = ls_add(None,v)
        tv = thorns_and_v
        while tv:
            e = g.vert[v].edge
            while e:
                other_v = value(g,e.otherVertex)
                if not ls_contains(d_u2,other_v):
                    ls_free(d_u2)
                    return e
                e = e.next
            tv = tv.next
        ls_free(thorns_and_v)
    ls_free(d_u2)
    return None

def find_swings(g, t, chain):
    u = chain.u
    uk = chain.uk
    d_ua = get_depth(t,chain.ua)
    higher = 0
    lower = 0
    cu = chain.u2
    while cu and cu!=uk:
        thrns = immediate_thorns(t,cu)
        num_thrns = ls_size(thrns)
        if num_thrns > 2:
            higher = 0
        elif num_thrns <= 0:
            pass
        elif num_thrns == 2:
            higher = 0
            thrn1 = value(g,thrns.value)
            thrn2 = value(g,thrns.next.value)
            e = find_edge(g,thrn1,thrn2)
            if e:
                new_sw = create_swing(t,thrn1,thrn2,e)
                d_higher = get_depth(t,thrn1)
                d_lower = get_depth(t,thrn2)
                if d_ua<d_higher-1:
                    new_sw.in_lower = 1
                if get_depth(t,thrn1) < get_depth(t,chain.ua) and get_depth(t,thrn2) > get_depth(t,get_parent(t,uk)):
                    new_sw.in_lower = 1
                chain.swings = l_add(chain.swings,new_sw)
                chain.swing_edges = l_add(chain.swing_edges,edge_ls_create(e))
        else:
            if not higher:
                higher = value(g,thrns.value)
            else:
                current_thorn = value(g,thrns.value)
                e = find_edge(g,higher,current_thorn)
                if e:
                    new_sw = create_swing(t,higher,current_thorn,e)
                    d_higher = get_depth(t,higher)
                    d_lower = get_depth(t,current_thorn)
                    if d_ua<d_higher-1:
                        new_sw.in_lower = 1
                    chain.swings = l_add(chain.swings,new_sw)
                    chain.swing_edges = l_add(chain.swing_edges,edge_ls_create(e))
                    higher = 0
                else:
                    higher = current_thorn
        ls_free(thrns)

def find_binding_edges(g, t, chain):
    ua = chain.ua
    uk = chain.uk
    ua_next = next_in_chain(t,ua)
    ua_next = ua_next if ua_next else ua
    ua_to_uk = None
    if ua_next:
        ua_to_uk = tree_path(t,ua_next,get_parent(t,uk))
    for swing in chain.swings:
        upg = swing.p_up
        dng = swing.p_down
        up_in_lower = bool(ls_contains(ua_to_uk,upg))
        down_in_lower = bool(ls_contains(ua_to_uk,dng))
        if up_in_lower and down_in_lower:
            w_candidates = thorns(t,chain.u2)
            l_upg = leaves(t,upg)
            w_candidates = ls_remove_list(w_candidates,l_upg)
            y_candidates = descendants(t,upg)
            for c_w in w_candidates:
                w = value(g,c_w.value)
                for e in g.vert[w].edge:
                    y = value(g,e.otherVertex)
                    if ls_contains(y_candidates,y):
                        zg_not = ls_add(ls_add(ls_add(None, w),swing.up),swing.down)
                        p1 = tree_path(t,dng,y)
                        p1_l = None
                        for p1c in p1:
                            p1_l = ls_merge(p1_l,immediate_thorns(t,p1c.value))
                        p1_l = ls_remove_list(p1_l,zg_not)
                        if p1_l:
                            swing.binding_edges = l_add(swing.binding_edges,edge_ls_create(e))
                            if not l_contains(chain.binding_edges, edge_match, e) and swing.in_lower:
                                chain.binding_edges = l_add(swing.binding_edges,edge_ls_create(e))
                            ls_free(p1)
                            ls_free(p1_l)
                            ls_free(zg_not)
                            continue
                        p2 = tree_path(t,get_parent(t,w),upg)
                        p2_l = None
                        for p2c in p2:
                            p2_l = ls_merge(p2_l,immediate_thorns(t,p2c.value))
                        p2_l = ls_remove_list(p2_l,zg_not)
                        if ls_size(p2_l) >= 2:
                            swing.binding_edges = l_add(swing.binding_edges,edge_ls_create(e))
                            if not l_contains(chain.binding_edges, edge_match, e) and swing.in_lower:
                                chain.binding_edges = l_add(swing.binding_edges,edge_ls_create(e))
                            ls_free(p1)
                            ls_free(p1_l)
                            ls_free(zg_not)
                            ls_free(p2)
                            ls_free(p2_l)
                            continue
                        else:
                            if p2_l:
                                l = value(g,p2_l.value)
                                if not graph_is_edge(g,l,w):
                                    swing.binding_edges = l_add(swing.binding_edges,edge_ls_create(e))
                                    if not l_contains(chain.binding_edges, edge_match, e):
                                        chain.binding_edges = l_add(swing.binding_edges,edge_ls_create(e))
                                    ls_free(p1)
                                    ls_free(p1_l)
                                    ls_free(zg_not)
                                    ls_free(p2)
                                    ls_free(p2_l)
                                    continue
                        ls_free(p1)
                        ls_free(p1_l)
                        ls_free(zg_not)
                        ls_free(p2)
                        ls_free(p2_l)
            if swing.binding_edges == None and swing.in_lower:
                swing.is_solo_edge = 1
            ls_free(w_candidates)
            ls_free(l_upg)
            ls_free(y_candidates)
    ls_free(ua_to_uk)

def process_chains(g, t, chains):
    for ch in chains:
        up_edge = upper_edge(g,t,ch)
        if up_edge:
            ch.e_p = up_edge
            ch.ua = value(g,up_edge.thisVertex)
        find_swings(g,t,ch)
        find_binding_edges(g,t,ch)

def edge_ls_free(el):
    pass

def chain_ls_free(P):
    pass

def E(g, t, x):
    ret = None
    for cx in x:
        u = value(g,cx.value)
        for e in g.vert[u].edge:
            v = value(g,e.otherVertex)
            if not ls_contains(x,v):
                ret = l_add(ret,edge_ls_create(e))
    return ret

def high(g, t, x):
    highest = None
    es = E(g,t,x)
    least_depth = sys.maxsize
    for ec in es:
        e = ec.e
        u = value(g,e.otherVertex)
        v = value(g,e.thisVertex)
        lc = lca(t,u,v)
        d_lc = get_depth(t,lc)
        if d_lc < least_depth:
            highest = e
            least_depth = d_lc
        elif d_lc == least_depth:
            highest = e
    l_free(es)
    return highest

def case1(g, t):
    trim_all_duplicates(g)
    trim_all_duplicates(t)
    ret = 0
    fringe = fringes(t, t.root)
    cur_fringe = fringe
    while cur_fringe:
        v = value(t, cur_fringe.value)
        if l_closed(g, t, v):
            des = leaves(t,v)
            e_retain = blossom_algorithm(g,des)
            ret = l_size(e_retain)
            cur_e = e_retain
            while cur_e:
                u1 = cur_e.e.thisVertex
                u2 = cur_e.e.otherVertex
                retain_merge_trim(g,t,cur_e.e.thisVertex, cur_e.e.otherVertex)
                cur_e = cur_e.next
            remaining_des = descendants(t,v)
            cur_des = remaining_des
            while cur_des:
                u = value(g,cur_des.value)
                if u == v:
                    cur_des = cur_des.next
                    continue
                u2 = g.vert[u].edge.otherVertex
                if u != value(g,u2):
                    retain_merge_trim(g,t,u,u2)
                cur_des = cur_des.next
            ls_free(des)
        cur_fringe = cur_fringe.next
    ls_free(fringe)
    return ret

def case2(g, t):
    fringe = fringes(t, t.root)
    cur_fringe = fringe
    ret = 0
    while cur_fringe:
        parent = value(t, cur_fringe.value)
        if not l_closed(g, t, parent):
            kids = children(t, parent)
            cur_kid = kids
            while cur_kid:
                cur_v = value(g, cur_kid.value)
                triv = trivial(g, t, cur_kid.value)
                if triv and (cur_v != value(g, parent)):
                    ret += 1
                    retain_merge_trim(g, t, cur_v, triv)
                cur_kid = cur_kid.next
            ls_free(kids)
        cur_fringe = cur_fringe.next
    ls_free(fringe)
    return ret

def case3(g, t):
    fringe = fringes(t, t.root)
    cur_fringe = fringe
    ret = 0
    while cur_fringe:
        parent = value(t, cur_fringe.value)
        if not l_closed(g, t, parent):
            kids = children(t, parent)
            if ls_size(kids) == 3:
                iso = isolated(g, t, parent)
                if iso is None:
                    cur_kid = kids
                    while cur_kid:
                        if trivial(g, t, cur_kid.value):
                            break
                        cur_kid = cur_kid.next
                    if not cur_kid:
                        ret += 1
                        par_of_par = get_parent(t, parent)
                        tp = tree_path(t, parent, par_of_par)
                        merge_list(g, tp)
                        merge_list(t, tp)
                        remove_self_edges(g, parent)
                        remove_self_edges(t, parent)
                ls_free(iso)
            ls_free(kids)
        cur_fringe = cur_fringe.next
    ls_free(fringe)
    return ret

def case4(g, t):
    u = t.root
    ret = 0
    fringe = fringes(t, u)
    cur_fri = fringe
    while cur_fri:
        f = value(g, cur_fri.value)
        kids = children(t, f)
        if kids.next and not kids.next.next:
            u1 = kids.value
            u2 = kids.next.value
            if graph_is_edge(g, u1, u2):
                cur_parent = get_parent(t, f)
                while cur_parent:
                    cur_kids = children(t, cur_parent)
                    if cur_kids.next and not cur_kids.next.next:
                        v = cur_parent
                        cur_parent = 0
                        u3 = is_leaf(t, cur_kids.value)
                        u3 = u3 if u3 else is_leaf(t, cur_kids.next.value)
                        if u3:
                            if not l_closed(g, t, v):
                                u1_u3 = graph_is_edge(g, u1, u3)
                                u2_u3 = graph_is_edge(g, u2, u3)
                                unconnected = 0 if u1_u3 else u1
                                unconnected = unconnected if unconnected else 0 if u2_u3 else u2
                                unconnected_connects_outside = 0
                                e = g.vert[unconnected].edge
                                tree_verts = descendants(t, v)
                                if not (u1_u3 and u2_u3):
                                    while e:
                                        cur_e = value(g, e.otherVertex)
                                        if not ls_contains(tree_verts, cur_e):
                                            unconnected_connects_outside = 1
                                            break
                                        e = e.next
                                if (u1_u3 and u2_u3) or unconnected_connects_outside:
                                    prnt = get_parent(t, v)
                                    tp = tree_path(t, v, prnt)
                                    merge_list(g, tp)
                                    merge_list(t, tp)
                                    remove_self_edges(g, prnt)
                                    remove_self_edges(t, prnt)
                                    ret = 1
                                else:
                                    retain_merge_trim(g, t, u1, u2)
                                    ret = 1
                                ls_free(tree_verts)
                    ls_free(cur_kids)
                    if cur_parent:
                        np = get_parent(t, cur_parent)
                        cur_parent = 0 if np == cur_parent else np
        ls_free(kids)
        cur_fri = cur_fri.next
    ls_free(fringe)
    return ret

def prime_edges(g, t, v):
    u = value(g, v)
    prime_edges = None
    fringe = fringes(t, u)
    cur_fri = fringes(t, u)
    while cur_fri:
        f = value(g, cur_fri.value)
        kids = children(t, f)
        if kids.next and not kids.next.next:
            u1 = kids.value
            u2 = kids.next.value
            p1 = find_edge(g, u1, u2)
            if p1:
                prime_edges = l_add(prime_edges, edge_ls_create(p1))
                cur_parent = get_parent(t, f)
                while cur_parent:
                    cur_kids = children(t, cur_parent)
                    if cur_kids.next and not cur_kids.next.next:
                        p_fringe_candidate = cur_parent
                        cur_parent = 0
                        u3 = is_leaf(t, cur_kids.value)
                        u3 = u3 if u3 else is_leaf(t, cur_kids.next.value)
                        if u3:
                            p21 = find_edge(g, u1, u3)
                            p22 = find_edge(g, u2, u3)
                            if p21 and p22:
                                desc_v = descendants(t, p_fringe_candidate)
                                e = g.vert[u1].edge
                                u1_or_u2_bad_edge = 0
                                while e:
                                    other_v = value(g, e.otherVertex)
                                    c = ls_contains(desc_v, other_v)
                                    if not c:
                                        u1_or_u2_bad_edge = 1
                                        break
                                    e = e.next
                                e = g.vert[u2].edge
                                while e:
                                    other_v = value(g, e.otherVertex)
                                    c = ls_contains(desc_v, other_v)
                                    if not c:
                                        u1_or_u2_bad_edge = 1
                                        break
                                    e = e.next
                                if not u1_or_u2_bad_edge:
                                    prime_edges = l_add(prime_edges, edge_ls_create(p21))
                                    prime_edges = l_add(prime_edges, edge_ls_create(p22))
                                ls_free(desc_v)
                    ls_free(cur_kids)
                    if cur_parent:
                        np = get_parent(t, cur_parent)
                        cur_parent = 0 if np == cur_parent else np
        ls_free(kids)
        cur_fri = cur_fri.next
    ls_free(fringe)
    return prime_edges
def prime_edges_type1(g, t, v):
    u = value(g, v)
    prime_edges = []
    fringe = fringes(t, u)
    cur_fri = fringes(t, u)
    while cur_fri:
        f = value(g, cur_fri.value)
        kids = children(t, f)
        if kids.next and not kids.next.next:
            u1 = kids.value
            u2 = kids.next.value
            p1 = None
            if p1 = find_edge(g, u1, u2):
                prime_edges.append(edge_ls_create(p1))
        ls_free(kids)
        cur_fri = cur_fri.next
    ls_free(fringe)
    return prime_edges

def prime_edges_type2(g, t, v):
    u = value(g, v)
    prime_edges = []
    fringe = fringes(t, u)
    cur_fri = fringes(t, u)
    while cur_fri:
        f = value(g, cur_fri.value)
        kids = children(t, f)
        if kids.next and not kids.next.next:
            u1 = kids.value
            u2 = kids.next.value
            p1 = None
            if p1 = find_edge(g, u1, u2):
                cur_parent = get_parent(t, f)
                while cur_parent:
                    cur_kids = children(t, cur_parent)
                    if cur_kids.next and not cur_kids.next.next:
                        p_fringe_candidate = cur_parent
                        cur_parent = 0
                        u3 = is_leaf(t, cur_kids.value)
                        u3 = u3 if u3 else is_leaf(t, cur_kids.next.value)
                        if u3:
                            p21 = None
                            p22 = None
                            if p21 = find_edge(g, u1, u3) and p22 = find_edge(g, u2, u3):
                                desc_v = descendants(t, p_fringe_candidate)
                                e = g.vert[u1].edge
                                u1_or_u2_bad_edge = 0
                                while e:
                                    other_v = value(g, e.otherVertex)
                                    c = ls_contains(desc_v, other_v)
                                    if not c:
                                        u1_or_u2_bad_edge = 1
                                        break
                                    e = e.next
                                e = g.vert[u2].edge
                                while e:
                                    other_v = value(g, e.otherVertex)
                                    c = ls_contains(desc_v, other_v)
                                    if not c:
                                        u1_or_u2_bad_edge = 1
                                        break
                                    e = e.next
                                if not u1_or_u2_bad_edge:
                                    prime_edges.append(edge_ls_create(p21))
                                    prime_edges.append(edge_ls_create(p22))
                                ls_free(desc_v)
                    ls_free(cur_kids)
                    if cur_parent:
                        np = get_parent(t, cur_parent)
                        cur_parent = 0 if np == cur_parent else np
        ls_free(kids)
        cur_fri = cur_fri.next
    ls_free(fringe)
    return prime_edges

def leaf_edges(g, t, v):
    v = value(t, v)
    leaf_edges = []
    leafs = leaves(t, v)
    for leaf in leafs:
        cur_lf = value(g, leaf.value)
        parent_ls = []
        cur_v = get_parent(t, cur_lf)
        while cur_v != get_parent(t, v):
            parent_ls.append(cur_v)
            cur_v = get_parent(t, cur_v)
        for e in g.vert[cur_lf].edge:
            other_v = value(g, e.otherVertex)
            if ls_contains(parent_ls, other_v):
                leaf_edges.append(edge_ls_create(e))
        ls_free(parent_ls)
    ls_free(leafs)
    return leaf_edges

def leaf_to_leaf_edges(g, t, v):
    v = value(t, v)
    leaf_edges = []
    leafs = leaves(t, v)
    for leaf in leafs:
        cur_lf = value(g, leaf.value)
        for e in g.vert[cur_lf].edge:
            other_v = value(g, e.otherVertex)
            if is_leaf(t, other_v) and not edge_ls_contains(leaf_edges, e):
                leaf_edges.append(edge_ls_create(e))
    ls_free(leafs)
    return leaf_edges

def COVER(g, t, v, P):
    F_leaf = leaf_edges(g, t, v)
    E_leaf = leaf_to_leaf_edges(g, t, v)
    E_prime1 = prime_edges_type1(g, t, v)
    E_prime2 = prime_edges_type2(g, t, v)
    E_prime = E_prime1 + E_prime2
    E_bind = []
    E_upper = []
    E_swing = []
    for chain in P:
        if chain.binding_edges:
            E_bind.append(edge_ls_create(chain.binding_edges.e))
        if chain.e_p:
            E_upper.append(edge_ls_create(chain.e_p))
    E1 = E_prime1 + E_prime2 + E_bind + E_upper
    for ec in E1:
        retain_merge_trim(g, t, ec.e.thisVertex, ec.e.otherVertex)
    for chain in P:
        for sw in chain.swings:
            sw_v = value(g, sw.e.thisVertex)
            matches = 0
            for m in matched_v:
                u = value(g, m.value)
                if u == sw_v:
                    matches += 1
            if matches == 2:
                ret_e = sw.binding_edges.e if sw.in_lower else chain.e_p
                u = value(g, ret_e.thisVertex)
                v2 = value(g, ret_e.otherVertex)
                tp = tree_path(t, u, v2)
                components = 0
                for t in tp:
                    for m in matched_v:
                        cur_m = value(g, m.value)
                        if cur_m == tp.value:
                            components += 1
                            break
                if components >= 3:
                    retain_merge_trim(g, t, u, v2)


def graph_print_all(g):
    # implementation of graph_print_all function

def merge2(g, t, P):
    for ch in P:
        if not ch.e_p:
            continue
        x = ch.ua
        ua2 = value(g, ch.e_p.otherVertex)
        ua1 = value(g, ch.e_p.thisVertex)
        if ua1 == ua2:
            continue
        v1_matched = bool(ls_contains(matched_v, ch.u))
        num_upward = 0
        any_in_matching = 0
        for sw in ch.swings:
            if not sw.in_lower:
                num_upward += 1
                u1 = value(g, sw.e.thisVertex)
                u2 = value(g, sw.e.otherVertex)
                if ls_contains_2(matched_v, u1, u2):
                    any_in_matching += 1
        if (any_in_matching and num_upward >= 2) or (v1_matched and num_upward):
            uppers = None
            for chc in P:
                if chc == ch:
                    continue
                uhc1 = value(g, chc.e_p.thisVertex)
                uhc2 = value(g, chc.e_p.otherVertex)
                if uhc1 == uhc2:
                    continue
                v1_matched_c = bool(ls_contains(matched_v, chc.u))
                num_upward_c = 0
                for sw in ch.swings:
                    if not sw.in_lower:
                        num_upward += 1
                if num_upward >= 2:
                    uppers = l_add(uppers, edge_ls_create(chc.e_p))
            if uppers:
                uppers = l_add(uppers, edge_ls_create(ch.e_p))
                for ue in uppers:
                    v1 = value(g, ue.e.thisVertex)
                    v2 = value(g, ue.e.otherVertex)
                    retain_merge_trim(g, t, v1, v2)
                l_free(uppers)

def phase3(g, t, v, P, epsilon):
    cur_desc = descendants(t, v)
    cur_desc = ls_first(ls_remove(ls_contains(cur_desc, v)))
    lf_p = ls_merge(leaves(t, v), pseudo_fringes(g, t, v))
    cur_desc = ls_remove_list(cur_desc, lf_p)
    for cd in cur_desc:
        u = value(g, cd.value)
        ul = ls_add(None, u)
        he = high(g, t, ul)
        u1 = value(g, he.thisVertex)
        u2 = value(g, he.otherVertex)
        uo = u ^ u1 ^ u2
        retain_merge_trim(g, t, u, uo)
        ls_free(ul)
    l_free(F_leaf)
    l_free(E_leaf)
    l_free(E_prime1)
    l_free(E_prime2)
    l_free(E_prime)
    ls_free(P)
    ls_free(vs)
    l_free(m_star)
    ls_free(matched_v)
    ls_free(unmatched_v)
    l_free(M2_s)
    ls_free(newly_matched_v)
    ls_free(Wmmm)
    l_free(E1)
    ls_free(cur_desc)
    ls_free(lf_p)

def A3(g, t, P):
    for ch in P:
        for sw in ch.swings:
            if sw.is_solo_edge:
                return 0
    return 1

def lemma9(g, t, v, P, epsilon):
    u_j = 0
    low_solo = None
    max_depth = 0
    deep_chain = None
    for ch in P:
        for sw in ch.swings:
            if sw.is_solo_edge:
                cur_depth = get_depth(t, sw.up)
                if cur_depth > max_depth:
                    max_depth = cur_depth
                    u_j = sw.up
                    deep_chain = ch
                    low_solo = sw.e
    if deep_chain is None:
        print("you shouldnt have called lemma9")
        fflush(stdout)
    l = (4 / epsilon) - 1
    leafs = leaves(t, u_j)
    n_leaves = ls_size(leafs)
    if n_leaves >= l:
        ch_u = find_chains(t, deep_chain.u)
        process_chains(g, t, ch_u)
        COVER(g, t, u_j, ch_u)
    else:
        lemma7(g, t, u_j, epsilon)
    u_j = value(t, u_j)
    d = descendants(t, u_j)
    print("LEMMA9: %i desc after reduction" % l_size(d))
    e_star = None
    w_star = 0
    for u in get_parent(g, u_j):
        e = find_edge(g, u, u_j)
        if e:
            e_star = e
            w_star = u
        if value(g, u) == value(g, v) or u == value(t, t.root):
            break
    z1 = 0
    z2 = 0
    z1_d = __INT_MAX__
    z2_d = __INT_MAX__
    th = thorns(t, v)
    for cur_th in th:
        th_v = value(g, cur_th.value)
        dep = get_depth(t, th_v)
        if dep < z1_d:
            z1 = th_v
            z1_d = dep
    for cur_th in th:
        th_v = value(g, cur_th.value)
        dep = get_depth(t, th_v)
        if dep < z2_d and z1 != th_v and th_v != u_j:
            z2 = th_v
            z2_d = dep
    if not z2:
        if e_star:
            retain_merge_trim(g, t, e_star.thisVertex, e_star.otherVertex)
        retain_merge_trim(g, t, z1, u_j)
    elif get_depth(t, w_star) <= get_depth(t, z2) - 1:
        retain_merge_trim(g, t, e_star.thisVertex, e_star.otherVertex)
        retain_merge_trim(g, t, z1, z2)
    else:
        retain_merge_trim(g, t, z1, u_j)
        retain_merge_trim(g, t, z1, z2)

def nagamochi(g, t, epsilon):
    set_gm(g)
    set_tm(t)
    if not t.root:
        set_root(t, 1)
    while l_size(children(t, t.root)) > 0:
        any_cases = 1
        while any_cases:
            any_cases = 0
            while c = case1(g, t):
                any_cases |= c
            while c = case2(g, t):
                any_cases |= c
            while c = case3(g, t):
                any_cases |= c
            while c = case4(g, t):
                any_cases |= c
        trim_all_duplicates(g)
        trim_all_duplicates(t)
        mlfc = minimally_lf_closed(g, t, t.root)
        if mlfc:
            P = find_chains(t, mlfc.value)
            process_chains(g, t, P)
            if A3(g, t, P):
                COVER(g, t, mlfc.value, P)
            else:
                lemma9(g, t, mlfc.value, P, epsilon)
            chain_ls_free(P)
            ls_free(mlfc)
        trim_all_duplicates(g)
        trim_all_duplicates(t)
    return t.vert[0].edge

