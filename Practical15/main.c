#include "common.h"
client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER; 



void queue_add(client_t *cl){
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!clients[i])
        {
            clients[i] = cl;
            break;
        }
        
    }
    pthread_mutex_unlock(&clients_mutex);
}

void queue_remove(int uid){
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i])
        {
            if (clients[i]->uid == uid)
            {
                clients[i] = NULL;
                break;
            }
            
        }
        
    }
    pthread_mutex_unlock(&clients_mutex);
}


void send_message_to_all (char *msg, int sender_uid){
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i])
        {
            if (clients[i]->uid != sender_uid)
            {
                if (write(clients[i]->sockfd, msg, strlen(msg)) < 0)
                {
                    perror("ERROR: write to descriptor failed");
                    break;
                }
                
            }
            
        }
        
    }
    pthread_mutex_unlock(&clients_mutex);
}




void *client_handle(void *arg){
    client_t *cli = (client_t*)arg;
    char buffer[BUFFER_SIZE];
    char name[32];
    char message[BUFFER_SIZE + 50];
    int leave_flag = 0;

    if (recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) < 2 || strlen(name) >= 32 - 1) {
        printf("Didn't enter the name.\n");
        leave_flag = 1;}
    else{
        strcpy(cli->name, name);
        printf("<< User %s joined the chat >>\n", cli->name);
    }

    memset(buffer, 0, BUFFER_SIZE);

    while (1)
    {
        if (leave_flag){
            break;
        }        
        int bytes_read = recv(cli->sockfd, buffer, BUFFER_SIZE, 0);
        
        if (bytes_read>0){
            buffer[bytes_read] = '\0';
            buffer[strcspn(buffer, "\n")] = 0;
            snprintf(message, sizeof(message), "%s: %s \n", cli->name, buffer);
            send_message_to_all(message, cli->uid);
            printf("%s", message);
        }
        else if (bytes_read == 0 || strcmp(buffer, "exit") == 0)
        {
            sprintf(buffer, "%s has left\n", cli->name);
            printf("%s", buffer);
            send_message_to_all(buffer, cli->uid);
            leave_flag = 1;
        }
        else {
            printf("ERROR: -1\n");
            leave_flag = 1;
        }
        memset(buffer, 0, BUFFER_SIZE);
        
    }
    close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    pthread_detach(pthread_self());
    
    return NULL;
}

void chat(int id, int sock_accept){
    pthread_t tid;
    client_t *cli = (client_t*)malloc(sizeof(client_t));
    cli ->sockfd = sock_accept;
    cli ->uid = id;
    queue_add(cli);
    pthread_create(&tid, NULL, client_handle, (void*)cli);
}


int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    socklen_t len = sizeof(server);
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if ((bind(sock, (struct sockaddr*)&server, len)) < 0){
        perror("ERROR: bind fail");
        return EXIT_FAILURE;
    }
    listen(sock, MAX_CLIENTS);
    printf("Server started listening...");
    int id = 0;
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int sock_accept = accept(sock, (struct sockaddr*)&client_addr, &client_len);
        printf("New connection accepted\n");
        chat(id++, sock_accept);

        
    }
    close(sock);
    return 0;
}
