#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h>

int main()
{
    int server_fd;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created");
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    close(server_fd);
    return 0;
}
