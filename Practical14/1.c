#include <stdio.h>

// Функція, яка виконує ділення
int calculate_risk(int a, int b) {
    // BUG HERE: Якщо b буде 0, програма впаде
    return a / b;
}

int main() {
    int a = 1000;
    int b = 0; // Ой, захисту немає!
    
    printf("Starting calculation...\n");
    int result = calculate_risk(a, b);
    
    printf("Result: %d\n", result);
    return 0;
}