#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // "open", Flags
#include <stdlib.h>
#include <string.h>

int main()
{
    int file_fd = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC , 0644);
    
    dup2(file_fd, STDOUT_FILENO);
    close (file_fd);
    printf("Redirected output\n");
    printf("This also goes to the file.\n");
    fprintf(stderr, "(This error message goes to the terminal)\n");
    return 0;
}
