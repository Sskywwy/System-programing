#include <stdio.h>
#include <stdlib.h>



int main()
{
    size_t ptr = 5;
    float* arr = calloc(ptr, sizeof(float));
    for (int i = 0; i < 5; i++)
    {
        printf("%f", arr[i]);
    }
    
    return 0;
}
