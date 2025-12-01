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
#define MAX_CLIENTS 3
//ERROR HANDLE
//if value < 0, print error
#define CHECK(x, msg) \
    if ((x)<0) { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    }

typedef struct {
    int sockfd;
    struct sockaddr_in address;
    int uid;
    char name[32];
} client_t;