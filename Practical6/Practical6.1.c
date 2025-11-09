#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void* thread_function(void* arg){
    printf("Hello from thread\n");
    sleep(1);
    return NULL;
}

int main()
{
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, thread_function, NULL) !=0){
        perror("Failed");
        return 1;
    }
    printf("hello from main\n");

    if (pthread_join(thread_id, NULL) != 0){
        perror("Failed");
        return 2;
    }
    printf("Thread has finished and joined.\n");
    return 0;
}
