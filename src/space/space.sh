#!/usr/bin/env bash

valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=fredrickson --ignore-fn=edmondsAlgorithm --ignore-fn=main --massif-out-file=space/greedy.100 ./tree-augmentor 100
   
   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=fredrickson --ignore-fn=edmondsAlgorithm --ignore-fn=main --massif-out-file=space/greedy.500 ./tree-augmentor 500

for i in {1..100}; do
   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=greedyHeuristic --ignore-fn=main --massif-out-file=space/fredrickson.100.$i ./tree-augmentor 100
   
   valgrind --tool=massif --stacks=yes --time-unit=ms --detailed-freq=1 --max-snapshots=1000 --ignore-fn=greedyHeuristic --ignore-fn=main --massif-out-file=space/fredrickson.500.$i ./tree-augmentor 500
done
