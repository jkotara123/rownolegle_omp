#!/bin/bash

PROGRAM=./sort_1
MAX_VALUE=10000

THREADS_LIST=(1 2 4 8 10 12 18 24 36 48)
N_LIST=(60000000 200000000 600000000)

# TU wpisz optymalny wynik z sequential (np. 128 albo 256)
BINS=1024

OUTPUT=parallel_results.csv

echo "threads,N,bins,gen,dist,sort,merge,total" > $OUTPUT

for N in "${N_LIST[@]}"
do
    for T in "${THREADS_LIST[@]}"
    do
        echo "Running PAR: N=$N THREADS=$T BINS=$BINS"

        export OMP_NUM_THREADS=$T

        result=$($PROGRAM $N $MAX_VALUE $BINS)

        echo "$T,$N,$BINS,$result" >> $OUTPUT
    done
done

echo "Done: $OUTPUT"