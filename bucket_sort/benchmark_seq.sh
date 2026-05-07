#!/bin/bash

PROGRAM=./sort_1
MAX_VALUE=10000

THREADS=1
N=200000000

BINS_LIST=(2 16 32 64 128 256  512 640 768 896 1024 1280 1536 2048 3060)

OUTPUT=sequential_results.csv

echo "threads,N,bins,gen,dist,sort,merge,total" > $OUTPUT

for BINS in "${BINS_LIST[@]}"
do
    echo "Running SEQ: N=$N BINS=$BINS"

    export OMP_NUM_THREADS=1

    result=$(OMP_NUM_THREADS=1 $PROGRAM $N $MAX_VALUE $BINS)

    echo "1,$N,$BINS,$result" >> $OUTPUT
done

echo "Done: $OUTPUT"