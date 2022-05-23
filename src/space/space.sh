#!/usr/bin/env bash

for i in {1..10}; do
   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=greedyHeuristic --ignore-fn=main --massif-out-file=space/fredrickson.100.$i ./tree-augmentor 100 1

   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=fredrickson --ignore-fn=edmondsAlgorithm --ignore-fn=main --massif-out-file=space/greedy.100.$i ./tree-augmentor 100 1

done

for i in {1..1}; do
   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=greedyHeuristic --ignore-fn=main --massif-out-file=space/fredrickson.1000.$i ./tree-augmentor 1000 1

   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=fredrickson --ignore-fn=edmondsAlgorithm --ignore-fn=main --massif-out-file=space/greedy.1000.$i ./tree-augmentor 1000 1

done
