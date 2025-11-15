#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   

int main() {
    int N_ROWS = 3; 
    int M_COLS = 4;

    printf("Allocating memory for a %dx%d matrix...\n", N_ROWS, M_COLS);
    int** matrix = malloc(N_ROWS * sizeof(int*));
    for (int i = 0; i < N_ROWS; i++) {
        matrix[i] = malloc(M_COLS * sizeof(int));
    }
    printf("Memory allocated successfully.\n");

    srand(time(NULL));     
    long long total_sum = 0;
    printf("\nMatrix Content:\n");
    for (int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < M_COLS; j++) {
            int random_value = rand() % 100;
            matrix[i][j] = random_value;
            total_sum += matrix[i][j];
            printf("%-4d", matrix[i][j]); 
        }
        printf("\n"); 
    }

    printf("\nTotal sum of all elements: %lld\n", total_sum);
    printf("Freeing memory...\n");
    
    for (int i = 0; i < N_ROWS; i++) {
        free(matrix[i]);
    }
    free(matrix);
    matrix = NULL; 
    printf("Memory freed.\n");

    return 0;
}