#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 10
#define MAX_CONCURENT 3

sem_t semaphor;

void* thread_func (void* arg){
    long id = (long)arg;
    printf("Thread %ld: Arrived. WAITING for a slot...\n", id);

    sem_wait(&semaphor);
    printf("Thread %ld: --- ENTERED slot. Working for 2s... ---\n", id);
    sleep(2);
    printf("Thread %ld: --- LEAVING slot. Releasing...\n", id);
    sem_post(&semaphor);

    return NULL;
}

int main()
{
    sem_init(&semaphor, 0, MAX_CONCURENT); //the second arg responds for ONLY thread or for ALL PROCESS
    pthread_t threads[NUM_THREADS];
    printf("Starting %d threads, but limiting concurrency to %d...\n",
           NUM_THREADS, MAX_CONCURENT);
    printf("-------------------------------------------------------\n");
    for ( long i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }
    
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&semaphor);
    printf("-------------------------------------------------------\n");
    printf("All threads have completed.\n");
    return 0;
}
