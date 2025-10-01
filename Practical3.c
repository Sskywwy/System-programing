#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main()
{
    // FILE *fptr;
    // fptr = fopen("test.txt", "r");
    // if (fptr == NULL){
    //     printf("Error. FILE NOT EXIST");
    // }
    // fclose(fptr);

    // FILE *fptr;
    // fptr = fopen("text.txt", "r");
    // char buffer[100];
    // fgets(buffer, 100, fptr);
    // printf("%s", buffer);
    // fclose(fptr);

    CreateFile("text-windows.txt", GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL)
    WriteFile()
    return 0;
}
