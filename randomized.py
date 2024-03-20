import sys
import networkx as nx
import matplotlib.pyplot as plt
import random
import numpy as np
import time

def main():
    treeType = sys.argv[1]
    output = open(f'output/{treeType}.txt','w')
    output.write("tree,edges,time\n")

    for s in [10,100,1000]:
        for d in [1,10,100]:
            for n in range(1,6):
                print(f'Loading {treeType}{s}_{d}_{n}')
                E = []
                L = set()
                g = open(f'data/{s}/random/density-{d}/{treeType}{s}_{d}_{n}.txt', "r")
                content = g.read().split("\n")
                addLinks = False

                numDuplicates = 0

                for line in content[2:]:
                    if line != "":
                        if line == "L":
                            addLinks = True
                            continue
                        if not addLinks:
                            i = int(line.split()[0])
                            j = int(line.split()[1])
                            if (j, i) not in E:
                                E.append((i, j))
                        else:
                            i = int(line.split()[0])
                            j = int(line.split()[1])
                            if (j, i) not in L:
                                L.add((i, j))

                Tree = nx.Graph()
                Tree.add_edges_from(E)
                Links = nx.Graph()
                Links.add_edges_from(L)

                st = time.process_time()
                minimumSolution = 0
                for i in range(1):
                    S = Tree.copy()
                    Lt = Links.copy()

                    for u in Tree.nodes():
                        if S.degree(u) == 1:
                            v = random.choice(list(Lt[u]))
                            S.add_edge(u, v)
                            Lt.remove_edge(u, v)

                    while nx.has_bridges(S):
                        randomEdge = random.choice(list(Lt.edges()))
                        S.add_edge(randomEdge[0], randomEdge[1])
                        Lt.remove_edge(randomEdge[0], randomEdge[1])

                    currentSolution = S.number_of_edges() - Tree.number_of_edges()

                    if not minimumSolution:
                        minimumSolution = currentSolution

                    minimumSolution = currentSolution if currentSolution < minimumSolution else minimumSolution

                    # print(f'iteration: {i+1}, currentSolution: {currentSolution}, minimumSolution: {minimumSolution}')

                et = time.process_time()
                t = et-st
                output.write(f'{treeType}{s}_{d}_{n},{minimumSolution},{t}\n')
                g.close()
    output.close()


if __name__ == '__main__':
    main()
