#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000003

int array[ARRAY_SIZE];

long long partial_sums[NUM_THREADS];



typedef struct {
    int thread_id;     // ID для логування (не обов'язково, але корисно)
    int start_index; // Включний початковий індекс
    int end_index;   // Виключний кінцевий індекс (тобто, [start, end))
} ThreadArgs;


void* thread_routine(void* arg) {
    // 1. Cast (перетворюємо) вхідний `void*` назад у наш `ThreadArgs*`
    ThreadArgs* args = (ThreadArgs*)arg;

    // 2. Розраховуємо нашу "часткову суму"
    long long my_sum = 0;
    for (int i = args->start_index; i < args->end_index; i++) {
        my_sum += array[i];
    }

    // 3. Зберігаємо наш результат у "загальному" масиві результатів.
    // Ми знаємо, що кожен потік пише у *свій* унікальний слот,
    // тому тут немає "race condition" (гонки станів).
    partial_sums[args->thread_id] = my_sum;

    // 4. Завершуємо потік. Нам не потрібно нічого повертати,
    // оскільки ми зберегли результат у partial_sums.
    return NULL;
}


int main() {
    // --- 1. Initialization (Ініціалізація) ---
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

        // Кожен потік бере "chunk_size"
        int current_chunk_size = chunk_size;

        // Останній потік забирає "залишок"
        if (i == NUM_THREADS - 1) {
            current_chunk_size += remainder;
        }

        thread_args[i].end_index = current_start + current_chunk_size;
        current_start = thread_args[i].end_index;

        printf("  Thread %d: processing indices [%d, %d) (total %d)\n",
               i, thread_args[i].start_index, thread_args[i].end_index,
               current_chunk_size);

        // --- 3. Thread Creation (Створення потоків) ---
        pthread_create(&threads[i], NULL, thread_routine, &thread_args[i]);
    }

    // --- 4. Synchronization (Синхронізація) ---
    // Головний потік (main) тепер буде "чекати" (join) на
    // завершення кожного дочірнього потоку.
    // Це "synchronization barrier" (бар'єр синхронізації).
    printf("Waiting for all threads to join...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("All threads have finished.\n");


    // --- 5. Aggregation (Підсумок) ---
    // На цьому етапі ми *гарантовано* знаємо,
    // що всі потоки завершились, і `partial_sums` заповнений.
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    // --- 6. Result (Результат) ---
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