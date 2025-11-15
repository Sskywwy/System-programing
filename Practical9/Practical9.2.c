#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // "open", Flags
#include <stdlib.h>
#include <string.h>

int main()
{
    char *msg = "Hello world!";
    ssize_t len = strlen(msg);
    int t_fd = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC , 0644);
    write(t_fd, msg, len);
    close(t_fd);

    int fd1 = open("file.txt", O_RDONLY);
    printf("Original file descriptor: t_fd1 = %d\n", fd1);
    int fd2 = dup(fd1);
    printf("Duplicated file descriptor: fd2 = %d\n", fd2);
     
    return 0;
}
