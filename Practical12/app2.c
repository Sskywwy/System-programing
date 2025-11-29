#include <stdio.h>
#include "utils.h"

int main() {
    // Можеш замінити на "madam" або "tenet" для тесту
    char word[100];
    printf("Enter a word to check: ");
    scanf("%99s", word);
    
    if (is_palindrome(word)) {
        printf("Yes! '%s' is a palindrome.\n", word);
    } else {
        printf("No, '%s' is just a normal word.\n", word);
    }
    return 0;
}