#include <unistd.h>
#include <string.h>
#include <stdio.h>
int main()
{
    char *msg = "Hello world!";
    size_t len = strlen(msg);
    ssize_t bytewrite = write(STDOUT_FILENO, msg, len);
    printf("Written bytes: %ld", bytewrite);
    return 0;
}
