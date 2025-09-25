#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// int SumArray(int *arr, int count) {
//     int i;
//     int sum = 0;
//     // printf("arr:%d\n", count);
//     for (i = 0; i < count; i++)
//     {
//         sum += *(arr + i);
//     }
//     return sum;
    
// }
int main()
{
    // int a = 10;
    // int* adress = &a;
    // printf("Adress memory: %p\n Variable: %d\n", adress, a);

    // int array[] = {2, 245, 12, 33};
    // int* ptr = array;
    // int i;
    // int count =(sizeof(array)/sizeof(array[0]));
    // for (i = 0; i < count; i++)
    // {
    //     printf("array number %d: %d\n", i+1,  *(ptr + i));
    // }

    // int array[] = {12, 34, 5, 66, 454};
    // int result;
    // int count =(sizeof(array)/sizeof(array[0]));
    // result = SumArray(array, count);
    // printf("SumArray:%d", result);


    // int *arr;
    // int size = 10;
    // arr = (int*) malloc(size * sizeof(int));
    // for (int i = 0; i < size; i++)
    // {
    //     arr[i]= i + 1;
    // }
    // printf("Масив: ");
    // for (int i = 0; i < size; i++) {
    //     printf("%d ", arr[i]);
    // }
    // free(arr);

    int **matrix;
    int Rows = rand()% 100;
    int Column = rand()% 100;
    int size = Rows * Column;
    matrix = (int**) malloc(Column * sizeof(int*));
    for (int C = 0; C < Column; C++)
    {
        matrix[C] = (int*) malloc(Rows * sizeof(int));
    }

    for (int C = 0; C < Column; C++)
    {
        for (int R = 0; R < Rows; R++)
        {
            matrix[C][R] = rand()%100;
        };
        
    }
    for (int C = 0; C < Column; C++)
    {
        for (int R = 0; R < Rows; R++)
        {
            printf("%d ", matrix[C][R]);
        };
        
    }
    free(matrix);
    
    return 0;


}
