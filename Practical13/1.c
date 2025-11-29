#include <stdio.h>
#include <string.h>

int main() {
   
    
    int secret = 0;             
    char buffer[10];            

    printf("--- Memory Addresses ---\n");
    printf("Secret is at: %p\n", (void*)&secret);
    printf("Buffer is at: %p\n", (void*)&buffer);
    printf("------------------------\n");

    printf("Enter text: ");
    
    
    scanf("%s", buffer);

    printf("\nBuffer contains: %s\n", buffer);
    printf("Secret value is: %d (Hex: 0x%X)\n", secret, secret);

    
    if (secret != 0) {
        printf("\n[!!!] CRITICAL ALERT: Buffer Overflow detected!\n");
        printf("You overwrote the 'secret' variable without touching it directly.\n");
    } else {
        printf("\nMemory seems safe.\n");
    }

    return 0;
}