#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char msg[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    printf("Connecting to server...\n");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("connect success");

    while (1)
    {
        printf("Client (You): ");
        fgets(msg, 1024, stdin);
        send(sock, msg, strlen(msg), 0);
        if (strncmp(msg, "exit", 4) == 0) {
            printf("Disconnecting...\n");
            break;}
        
        memset(buffer, 0, 1024);
        int valread = recv(sock, buffer, 1024, 0);
        if (valread <= 0) {
            printf("Server disconnected.\n");
            break;}

        printf("\nServer: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server closed connection.\n");
            break;
    }
    }
        
    close(sock);
    return 0;
}
