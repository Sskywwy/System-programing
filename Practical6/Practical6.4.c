#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000003

int array[ARRAY_SIZE];

long long partial_sums[NUM_THREADS];



typedef struct {
    int thread_id;
    int start_index;
    int end_index;
} ThreadArgs;


void* thread_routine(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;

    long long my_sum = 0;
    for (int i = args->start_index; i < args->end_index; i++) {
        my_sum += array[i];
    }
    partial_sums[args->thread_id] = my_sum;
    return NULL;
}


int main() {
    printf("Initializing array with %d elements...\n", ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];

    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    int remainder = ARRAY_SIZE % NUM_THREADS;

    printf("Starting %d threads...\n", NUM_THREADS);
    int current_start = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_id = i;
        thread_args[i].start_index = current_start;

        int current_chunk_size = chunk_size;

        if (i == NUM_THREADS - 1) {
            current_chunk_size += remainder;
        }

        thread_args[i].end_index = current_start + current_chunk_size;
        current_start = thread_args[i].end_index;

        printf("  Thread %d: processing indices [%d, %d) (total %d)\n",
               i, thread_args[i].start_index, thread_args[i].end_index,
               current_chunk_size);

        pthread_create(&threads[i], NULL, thread_routine, &thread_args[i]);
    }

    printf("Waiting for all threads to join...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("All threads have finished.\n");

    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    printf("\n--- Final Result ---\n");
    printf("Expected sum: %d\n", ARRAY_SIZE);
    printf("Calculated sum: %lld\n", total_sum);

    if (total_sum == ARRAY_SIZE) {
        printf("SUCCESS: The calculation is correct!\n");
    } else {
        printf("FAILURE: The calculation is incorrect!\n");
    }

    return 0;
}