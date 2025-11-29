#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h>

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int len = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created");
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    printf("Server is listening on port 8080...");
    listen(server_fd, 3);
    
    char buffer[1024] = {0};
    int new_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&len);
    printf("Client conected\n");
    ssize_t valread = recv(new_fd, buffer, 1024, 0);
    if (valread > 0) {
        printf("Message from Client: %s\n", buffer);
    }
    char *response = "hello client";
    send(new_fd, response, strlen(response), 0);
    printf("Response sent to client.\n");

    close(server_fd);
    close(new_fd);


    return 0;
}
