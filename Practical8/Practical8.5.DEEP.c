#include <stdio.h>
#include <stdlib.h> // Для malloc/free, rand/srand
#include <time.h>   // Для time() (щоб "засіяти" rand)

int main() {
    int N_ROWS = 3; // Кількість рядків
    int M_COLS = 4; // Кількість стовпців

    printf("Allocating memory for a %dx%d matrix...\n", N_ROWS, M_COLS);

    // --- 1. Allocation (Виділення) ---
    
    // Етап 1: Виділяємо пам'ять для N "вказівників на рядки" (int*)
    // `matrix` - це "вказівник на вказівник на int"
    int** matrix = (int**)malloc(N_ROWS * sizeof(int*));

    if (matrix == NULL) {
        perror("Failed to allocate row pointers");
        return 1;
    }

    // Етап 2: Виділяємо пам'ять для кожного окремого рядка (M int-ів)
    for (int i = 0; i < N_ROWS; i++) {
        matrix[i] = (int*)malloc(M_COLS * sizeof(int));
        
        if (matrix[i] == NULL) {
            perror("Failed to allocate memory for a row");
            // Safety: Якщо тут сталась помилка, ми маємо звільнити
            // все, що вже встигли виділити до цього.
            for (int k = 0; k < i; k++) {
                free(matrix[k]);
            }
            free(matrix);
            return 1;
        }
    }

    printf("Memory allocated successfully.\n");

    // --- 2. Filling & Summing (Заповнення та підсумок) ---
    
    // "Засіюємо" генератор випадкових чисел. Робимо це ОДИН РАЗ.
    srand(time(NULL)); 
    
    long long total_sum = 0; // Використовуємо long long про всяк випадок

    printf("\nMatrix Content:\n");
    for (int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < M_COLS; j++) {
            // Генеруємо випадкове число від 0 до 99
            int random_value = rand() % 100;
            
            // Використовуємо "classic" (класичний) синтаксис [i][j]
            matrix[i][j] = random_value;
            total_sum += matrix[i][j];

            // %-4d = вирівнювання (padding) для красивого друку
            printf("%-4d", matrix[i][j]); 
        }
        printf("\n"); // Новий рядок
    }

    // --- 3. Output (Вивід) ---
    printf("\nTotal sum of all elements: %lld\n", total_sum);

    // --- 4. Deallocation (Звільнення пам'яті) ---
    // Ми МУСИМО звільняти пам'ять у ЗВОРОТНОМУ порядку:
    // спочатку стовпці, ПОТІМ вказівники на рядки.
    
    printf("Freeing memory...\n");
    
    // Етап 1: Звільняємо кожен рядок (стовпці)
    for (int i = 0; i < N_ROWS; i++) {
        free(matrix[i]);
    }
    
    // Етап 2: Звільняємо масив вказівників на рядки
    free(matrix);

    matrix = NULL; // Good practice
    printf("Memory freed.\n");

    return 0;
}