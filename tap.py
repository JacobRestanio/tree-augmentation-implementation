import datetime
import time
import tracemalloc
import treegenerator as tg
import frederickson

def main():
    date = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
    with open(f"results{date}.txt", "w") as file:
        file.write(f"test, size, density, tree, frederickson\n")
    with open(f"time{date}.txt", "w") as file:
        file.write(f"test, size, density, tree, frederickson\n")
    with open(f"memory{date}.txt", "w") as file:
        file.write(f"test, size, density, tree, frederickson\n")

    sizes = [10]
    densities = [0.1, 0.5, 0.8]
    trees = ["path", "star", "starlike", "caterpillar", "lobster", "random"]
    treeFunc = [tg.path_tree, tg.star_tree, tg.starlike_tree, tg.caterpillar_tree, tg.lobster_tree, tg.random_tree]
    iterations = range(3)
    for s in sizes:
        for d in densities:
            for idx, tree in enumerate(trees):
                for i in iterations:
                    T = treeFunc[idx](s)
                    L = tg.generate_links(T, d)
                    
                    tracemalloc.start()
                    st = time.time()
                    fredericksonNumLinks = frederickson.frederickson(T, L)
                    fredericksonTime = time.time() - st
                    current, fredericksonMem = tracemalloc.get_traced_memory()
                    tracemalloc.stop()

                    with open(f"results{date}.txt", "a") as file:
                        file.write(f"{i+1}, {s}, {d}, {tree}, {fredericksonNumLinks}\n")
                    with open(f"time{date}.txt", "a") as file:
                        file.write(f"{i+1}, {s}, {d}, {tree}, {fredericksonTime}\n")
                    with open(f"memory{date}.txt", "a") as file:
                        file.write(f"{i+1}, {s}, {d}, {tree}, {fredericksonMem}\n")


if __name__ == "__main__":
    main()
