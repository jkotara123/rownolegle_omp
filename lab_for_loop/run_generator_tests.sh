#!/bin/bash

gcc -fopenmp sample_generator.c -o sample_generator

echo "threads;schedule_type;chunk_size;time;N" > results.csv

SIZES=("10000000" "1000000000")

CHUNKS=("4" "1000" "1000000")

TYPES=("0" "1" "2")

NUM_THREADS=6

echo "Script started..."

for N in "${SIZES[@]}"
do
    for T in "${TYPES[@]}"
    do
        for C in "${CHUNKS[@]}"
        do

            echo $(OMP_NUM_THREADS=$NUM_THREADS ./sample_generator $N $T $C) >> results.csv
                
            echo "Done: N=$N, Typ=$T, Chunk=$C"
        done
    done
done

echo "Ready."