#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 2048
#define MAX_CLIENTS 10

typedef struct {
    int sockfd;
    struct sockaddr_in address;
    int uid;
    char name[32];
} client_t;