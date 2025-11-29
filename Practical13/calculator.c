#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 16

void unsafe_calc(const char *input){
    char buffer[BUFFER_SIZE];
    int a, b;
    char op;
    int result = 0;
    strcpy(buffer, input);
    if (sscanf(buffer, "%d %c %d", &a, &op, &b) == 3)
        {
            if(op == '+'){
                result = a+b;
            }
            else if (op == '*')
            {
                result = a*b;
            }
            else if (op == '-'){
                result = a - b;
            }
            else if (op == '/'){
                result = a/b;
            }
        }
        else{
            printf("Error: Format not recognized or expression too long.\n");
        }
        printf("result: %d", result);
}


void calculator(){
    printf("\n[Safe Mode] Enter expression (e.g., 100 + 200): ");
    char buffer[128];
    char intput_buffer[BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        int written = snprintf(intput_buffer, BUFFER_SIZE, "%s", buffer);
        if (written >= BUFFER_SIZE)
        {
            printf("[Warning] Input was too long! Truncated to '%s'\n", buffer);
        }
        int a, b;
        char op;
        int result = 0;
        if ((sscanf(buffer, "%d %c %d", &a, &op, &b)) == 3)
        {
            if(op == '+'){
                result = a+b;
            }
            else if (op == '*')
            {
                result = a*b;
            }
            else if (op == '-'){
                result = a-b;
            }
            else if (op == '/'){
                result = a/b;
            }
        }
        else{
            printf("Error: Format not recognized or expression too long.\n");
        }
        
        printf("result: %d", result);

    }
    
}
int main(int argc, char const *argv[])
{
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0)
    {
        char *load = "10000000 + 20000000 THIS_STRING_IS_TOO_LONG_FOR_STACK";
        unsafe_calc(load);
    }
    else{
        calculator();
    }
    
    return 0;
}
