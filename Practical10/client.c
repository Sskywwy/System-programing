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
    const char *hello = "Hello world";
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    printf("Connecting to server...\n");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("connect success");

    send(sock, hello, strlen(hello), 0);
    printf("Message sent");

    ssize_t valread = recv(sock, buffer, 1024, 0);
    if (valread > 0) {
        printf("Message from SERVER: %s\n", buffer);
    }

    close(sock);
    return 0;
}
