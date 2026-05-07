#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "generator.h"
#include "bucket.h"
#define min(a,b) ((a) < (b) ? (a) : (b))

void bucket_sort_parallel(int *array, long long N, int max_value, int bucket_num);
int find_bucket(int num, int max_value, int bucket_num);
int cmp(const void *a, const void *b);
int is_sorted(int *array, long long N);

int main(int argc, char *argv[]) {
    long long N = 1000000;
    int max_value = 1000;
    int bucket_num = 20;

    if (argc > 1) {
        N = atoll(argv[1]);
    }
    if (argc > 2) {
        max_value = atoi(argv[2]);
    }
    if (argc > 3) {
        bucket_num = atoi(argv[3]);
    }

    double t_total_start = omp_get_wtime();

    int *array = malloc((size_t)N * sizeof(int));
    if (!array) {
        printf("Memory error\n");
        return 1;
    }
    
    double t_gen_start = omp_get_wtime();
    generate_array(array, N, max_value, 0, 1000);
    double t_gen_end = omp_get_wtime();
    printf("%f ", t_gen_end - t_gen_start);
    
    bucket_sort_parallel(array, N, max_value, bucket_num);

    double t_total_end = omp_get_wtime();

    printf("%f", t_total_end - t_total_start);
    
    if (!is_sorted(array, N)) {
        printf("Error: Array is not sorted.");
        return -1;
    }
    printf("\n");
    free(array);
    return 0;
}

void bucket_sort_parallel(int *array, long long N, int max_value, int bucket_num) {
    double t_dist = 0.0;
    double t_sort = 0.0;
    double t_merge = 0.0;
    
    long long *thread_sizes = malloc(omp_get_max_threads() * sizeof(long long));
    int num_threads = omp_get_max_threads();

    bucket_t *global_buckets;
    global_buckets = malloc(bucket_num * sizeof(bucket_t));
    long long approx_per_bucket = N / bucket_num;
    for (int i = 0; i < bucket_num; i++) {
        bucket_init(&global_buckets[i], approx_per_bucket / num_threads + 1);
    }

    int bucket_range = (max_value + bucket_num - 1) / bucket_num;
    int base = bucket_num / num_threads;
    int rem  = bucket_num % num_threads;

    double t1 = omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start_bucket = tid * base + min(tid, rem);
        int end_bucket   = start_bucket + base + (tid < rem ? 1 : 0);
        
        for(long long i = 0; i < N; i++){
            int num = array[i];
            int bucket = find_bucket(num, bucket_range, bucket_num);
            
            if (bucket >= start_bucket && bucket < end_bucket) {
                bucket_push(&global_buckets[bucket], num);
            }
        }
    }
    double t2 = omp_get_wtime();
    t_dist = t2 - t1;
    
    double t3 = omp_get_wtime();
    #pragma omp parallel
    {        
        int tid = omp_get_thread_num();
        int start_bucket = tid * base + min(tid, rem);
        int end_bucket   = start_bucket + base + (tid < rem ? 1 : 0);

        long long local_size = 0;
        for (int i = start_bucket; i < end_bucket; i++) {
            local_size += global_buckets[i].size;
            qsort(global_buckets[i].data, global_buckets[i].size, sizeof(int), cmp);
        }
        thread_sizes[tid] = local_size;
    }
    double t4 = omp_get_wtime();
    t_sort = t4 - t3;

    double t5 = omp_get_wtime();


    long long *offsets = malloc(num_threads * sizeof(long long));
    offsets[0] = 0;

    for (int i = 1; i < num_threads; i++) {
        offsets[i] = offsets[i-1] + thread_sizes[i-1];
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start_bucket = tid * base + min(tid, rem);
        int end_bucket   = start_bucket + base + (tid < rem ? 1 : 0);
        long long current_index = offsets[tid];

        for (int i = start_bucket; i < end_bucket; i++) {
            
            for (long long j = 0; j < global_buckets[i].size; j++){
                array[current_index++] = global_buckets[i].data[j];
            }
            bucket_free(&global_buckets[i]);
        }
    }    
    double t6 = omp_get_wtime();
    t_merge = t6 - t5;
    
    free(global_buckets);
    printf("%f ", t_dist);
    printf("%f ", t_sort);
    printf("%f ", t_merge);

    free(thread_sizes);
    free(offsets);
}

int find_bucket(int num, int bucket_range, int bucket_num) {
    int b = num / bucket_range;

    if (b >= bucket_num) b = bucket_num - 1;
    return b;
}

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int is_sorted(int *array, long long N) {
    for (long long i = 1; i < N; i++) {
        if (array[i-1] > array[i]) {
            return 0;
        }
    }
    return 1;
}