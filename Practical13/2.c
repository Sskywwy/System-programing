#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 16

int main() {
    char buffer[BUFFER_SIZE];

    printf("Enter text (safe limit %d chars): ", BUFFER_SIZE - 1);

    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        
        
        buffer[strcspn(buffer, "\n")] = '\0';

        printf("\nYou entered: '%s'\n", buffer);
        printf("Length: %lu\n", strlen(buffer));

        if (strlen(buffer) == BUFFER_SIZE - 1) {
             printf("[Info] Buffer is full. Some characters might have been truncated.\n");
        }

    } else {
        printf("Error reading input.\n");
    }

    return 0;
}