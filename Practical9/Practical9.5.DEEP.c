#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // "open", Flags
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096
int main()
{
    printf("\nPrint smth and u will be able to see it in file: ");
    fflush(stdout);
    int temp_fd = open("file1.txt", O_CREAT | O_WRONLY | O_TRUNC);
    dup2(temp_fd, STDOUT_FILENO);
    close(temp_fd);
    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
     
    return 0;
}
