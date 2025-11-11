#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 10000000

long long counter;
pthread_mutex_t g_lock;

void* thread_func_unsafe(){
    for (long long i = 0; i < INCREMENTS; i++)
    {
        counter++;
    }
    return NULL;
}

void* thread_func_safe(){
    for (long long i = 0; i < INCREMENTS; i++)
    {
        pthread_mutex_lock(&g_lock);
        counter++;
        pthread_mutex_unlock(&g_lock);
    }
    return NULL;

}



int main(){
    pthread_t thread_id[NUM_THREADS];
    long long expected_value = NUM_THREADS * INCREMENTS;
    printf("--- Running WITHOUT Mutex (Race Condition) ---\n");
    printf("Expected value: %lld\n", expected_value);

    counter = 0;

    //Unsafe thread func
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&thread_id[i], NULL, thread_func_unsafe, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
    
    printf("Actual value:   %lld\n", counter);
    if (counter != expected_value) {
        printf("FAILURE: Race condition detected! (Lost %lld increments)\n", 
               expected_value - counter);
    } else {
        printf("SUCCESS (Дуже малоймовірно!)\n");
    }
    //Mutex
    pthread_mutex_init(&g_lock, NULL);
    printf("\n--- Running WITH Mutex (Thread-Safe) ---\n");
    printf("Expected value: %lld\n", expected_value);

    counter = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&thread_id[i], NULL, thread_func_safe, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
    pthread_mutex_destroy(&g_lock);

    printf("Actual value:   %lld\n", counter);
    if (counter == expected_value) {
        printf("SUCCESS: The result is correct and consistent!\n");
    } else {
        printf("FAILURE: Something went wrong with the lock!\n");
    }

    return 0;
    


}