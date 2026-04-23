#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "generator.h"

void generate_array(int *array, long long N, int schedule_type, int chunk_size) {
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        unsigned int seed = (unsigned int)(omp_get_wtime() * 1e6) ^ thread_id;

        if (schedule_type == 0) {
            #pragma omp for schedule(static, chunk_size)
            for (long long i = 0; i < N; i++)
                array[i] = rand_r(&seed) % 1000;
        } 
        else if (schedule_type == 1) {
            #pragma omp for schedule(dynamic, chunk_size)
            for (long long i = 0; i < N; i++)
                array[i] = rand_r(&seed) % 1000;
        } 
        else {
            #pragma omp for schedule(guided, chunk_size)
            for (long long i = 0; i < N; i++)
                array[i] = rand_r(&seed) % 1000;
        }
    }
}