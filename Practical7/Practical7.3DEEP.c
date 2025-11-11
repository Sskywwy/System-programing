#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define COSTUMERS 3
#define PRODUCERS 2
#define ITEM_PER_PRODUCER 20
#define BUFFER_SIZE 10

sem_t semaphore_in;
sem_t semaphore_out;

pthread_mutex_t mutex;

int buffer[BUFFER_SIZE];

int in_index = 0;
int out_index = 0;

void* costumer_func(void* arg){
    long id = (long)arg;
    while (1)
    {
        sem_wait(&semaphore_out);
        pthread_mutex_lock(&mutex);
        int item = buffer[out_index];
        out_index = (out_index + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&semaphore_in);
        printf("Consumer %ld: Consumed item %d\n", id, item);
        usleep(100000); // Імітуємо роботу

    }
    
}

void* producer_func(void* arg){
    long id = (long)arg;
    for (int i = 0; i < ITEM_PER_PRODUCER; i++)
    {
    int item = (id*ITEM_PER_PRODUCER)+i;
    sem_wait(&semaphore_in);
    pthread_mutex_lock(&mutex);
    buffer[in_index] = item;
    in_index = (in_index + 1) % BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&semaphore_out);
    printf("Producer %ld: Produced item %d\n", id, item);
    usleep(50000); // Імітуємо роботу

    }
    printf("--- Producer %ld: FINISHED. ---\n", id);
    return NULL;

}
int main()
{
    pthread_t producer_threads[PRODUCERS];
    pthread_t costumer_threads[COSTUMERS];
    
    sem_init(&semaphore_in, 0, BUFFER_SIZE);
    sem_init(&semaphore_out, 0, 0);

    pthread_mutex_init(&mutex, NULL);
    printf("--- Starting Factory (Buffer Size: %d) ---\n", BUFFER_SIZE);
    //Producer threads

    for (long i = 0; i < PRODUCERS; i++)
    {
        pthread_create(&producer_threads[i], NULL, producer_func, (void*)i);
    }
    //Costumer threads
    for (long i = 0; i < COSTUMERS; i++)
    {
        pthread_create(&costumer_threads[i], NULL, costumer_func, (void*)i);
    }
    
    for (long i = 0; i < COSTUMERS; i++)
    {
        pthread_join(costumer_threads[i], NULL);
    }
    printf("--- All producers finished. Factory shutting down... ---\n");
    sem_destroy(&semaphore_in);
    sem_destroy(&semaphore_out);
    pthread_mutex_destroy(&mutex);
    
    exit(0);
}
