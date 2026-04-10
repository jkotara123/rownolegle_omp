#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // default parameters
    long long N = 100000000; 
    int schedule_type = 0; 
    int chunk_size = 1000;

    int total_threads = omp_get_max_threads();

    // given parameters
    if (argc > 1) N = atoll(argv[1]);
    if (argc > 2) schedule_type = atoi(argv[2]);
    if (argc > 3) chunk_size = atoi(argv[3]);

    int *array = malloc(N * sizeof(int));
    
    if (array == NULL) {
        printf("Allocation error for %lld numbers.\n", N);
        return 1;
    }
    
    double start_time = omp_get_wtime();
    
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        unsigned int seed = time(NULL) ^ thread_id;
        
        if (schedule_type == 0) {
            #pragma omp for schedule(static, chunk_size)
            for (long long i = 0; i < N; i++) array[i] = rand_r(&seed) % 1000;
        } 
        else if (schedule_type == 1) {
            #pragma omp for schedule(dynamic, chunk_size)
            for (long long i = 0; i < N; i++) array[i] = rand_r(&seed) % 1000;
        } 
        else {
            #pragma omp for schedule(guided, chunk_size)
            for (long long i = 0; i < N; i++) array[i] = rand_r(&seed) % 1000;
        }
    }

    double end_time = omp_get_wtime();

    printf("%d;%d;%d;%lld;%f\n", total_threads, schedule_type, chunk_size, N, end_time - start_time);

    free(array);
    return 0;
}