#include <stdio.h>
#include <stdlib.h>

#define NUMBER 10

int main()
{
    int* arr = malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++)
    {
        arr[i] = i+1;
    }
    
    for (int i = 0; i < NUMBER; i++)
    {
        printf("%d ", arr[i]);
        if (i+1 == NUMBER) {
            printf("\n");
        }
    }
    
    free(arr);
    return 0;
}
