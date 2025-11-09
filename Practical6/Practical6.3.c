#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* thread_function(void* arg){
    int number = *(int*)arg;
    printf("Hello im thread. Lets do factorial from %d\n", number);

    unsigned long long result = 1;
    for (int i = 1; i <= number; i++)
    {
        result *= i;
    }
    unsigned long long* heap = malloc(sizeof(unsigned long long));
    *heap = result;



    return (void*)heap;}

int main(){
    void* thread_return;
    int number;
    scanf("%d", &number);

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, thread_function, (void*)&number) !=0){
        perror("Failed");
        return 1;
    }
    printf("Wait for work.....\n");

    if (pthread_join(thread_id, &thread_return) != 0){
        perror("Failed");
        return 2;
    }
    unsigned long long result = *(unsigned long long*)thread_return;
    printf("Our result: %llu\n", result);
    printf("Thread has finished and joined.\n");
    free(thread_return);
    return 0;
}