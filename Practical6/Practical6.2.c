#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

void* thread_func(void* arg) {
    int* thread_num = (int*)arg;
    int number = *thread_num;
    printf("Im thread %d\n", number + 1);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread_ids[NUM_THREADS];
    int thread_args[NUM_THREADS];
    printf("Main: Creating %d threads...\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_args[i] = i;
        if (pthread_create(&thread_ids[i], NULL, thread_func, (void*)&thread_args[i]) != 0)
        {
            perror("Failed");
            return 1;
        }
        
    }
    printf("Main: All threads created. Waiting for them to join...\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread_ids[i], NULL);
    }
    printf("Main: All threads have finished and joined. Exiting.\n");
    
    return 0;
}