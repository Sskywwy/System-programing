#include <stdio.h>
#include <stdlib.h>



int main()
{
    int* arr = malloc(3 * sizeof(int));
    for (int i = 0; i < 3; i++)
    {
        arr[i] = i+1;
    }
    
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", arr[i]);
        if (i+1 == 3) {
            printf("\n");
        }
    }

    int* ptr = realloc(arr, 6*sizeof(int));
    arr = ptr;
    
    for (int i = 0; i < 6; i++)
    {
        arr[i] = i+1;
    }
    for (int i = 0; i < 6; i++)
    {
        printf("%d ", arr[i]);
        if (i+1 == 6) {
            printf("\n");
        }
    }
    
    free(arr);
    return 0;
}
