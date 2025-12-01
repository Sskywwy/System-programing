#include "common.h"

//Configuration
client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

//Prototype
void queue_add(client_t *cl);
void queue_remove(int uid);
void send_message_to_all(char *msg, int sender_uid);
void *client_handle(void *arg);
void createThreadClient(int id, int sock_accept);
void str_trim(char *arr, int length);

//Main function
int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(sock, "Error:socket creation failed")

  //Configuration server
  struct sockaddr_in server;
  socklen_t len = sizeof(server);
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  
  //Bind and listen
  CHECK((bind(sock, (struct sockaddr *)&server, len)), "ERROR:bind failed");
  CHECK((listen(sock, MAX_CLIENTS)), "ERROR: failed listening");
  printf("Server started listening...");

  //Accept new user loop
  int id = 0;
  while (1) {
    //Create struct for every accept
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int sock_accept =
        accept(sock, (struct sockaddr *)&client_addr, &client_len);
    printf("New connection accepted\n");
    createThreadClient(id++, sock_accept);
  }
  //never distination
  close(sock);
  return 0;
}


//queue to add in  *clients
void queue_add(client_t *cl) {
  pthread_mutex_lock(&clients_mutex);
  
  //if  client not exist -> create it in stack
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (!clients[i]) {
      clients[i] = cl;
      break;
    }
  }
  pthread_mutex_unlock(&clients_mutex);
}

//Queue: clear stack of clients[]
void queue_remove(int uid) {
  pthread_mutex_lock(&clients_mutex);

  //If client[i] exist -> NULL
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {
      if (clients[i]->uid == uid) {
        clients[i] = NULL;
        break;
      }
    }
  }
  pthread_mutex_unlock(&clients_mutex);
}

//Send message all handler
void send_message_to_all(char *msg, int sender_uid) {
  pthread_mutex_lock(&clients_mutex);

  //send message all except sender
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {
      if (clients[i]->uid != sender_uid) {
        if (write(clients[i]->sockfd, msg, strlen(msg)) < 0) {
          perror("ERROR: write to descriptor failed");
          break;
        }
      }
    }
  }
  pthread_mutex_unlock(&clients_mutex);
}

//Receive message handler
void *client_handle(void *arg) {
  client_t *cli = (client_t *)arg;
  char buffer[BUFFER_SIZE];
  char name[32];
  char message[BUFFER_SIZE + 50];
  int leave_flag = 0;

  //Accept name from client (Handshake)
  if (recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) < 2 ||
      strlen(name) >= 32 - 1) {
    printf("Didn't enter the name.\n");
    leave_flag = 1;
  } else {
    strcpy(cli->name, name);
    printf("<< User %s joined the chat >>\n", cli->name);
  }

  memset(buffer, 0, BUFFER_SIZE);

  //Chat loop
  while (1) {
    if (leave_flag) {
      break;
    }

    //Receive text from client
    int bytes_read = recv(cli->sockfd, buffer, BUFFER_SIZE, 0);

    if (bytes_read > 0) {
        str_trim(buffer, bytes_read);
        snprintf(message, sizeof(message), "%s: %s \n", cli->name, buffer);
        send_message_to_all(message, cli->uid);
        printf("%s", message);
    } else if (bytes_read == 0 || strcmp(buffer, "exit") == 0) {
        sprintf(buffer, "%s has left\n", cli->name);
        printf("%s", buffer);
        send_message_to_all(buffer, cli->uid);
        leave_flag = 1;
    } else {
        printf("ERROR: -1\n");
        leave_flag = 1;
    }

    //Clear buffer
    memset(buffer, 0, BUFFER_SIZE);
  }

  //Clear memory
  close(cli->sockfd);
  queue_remove(cli->uid);
  free(cli);
  pthread_detach(pthread_self());

  return NULL;
}

//Additional func
//change \n into 0, add at the end \0
void str_trim(char *arr, int length){
    arr[length] = '\0';
    arr[strcspn(arr, "/n")] = 0;
}

//For every client create thread
void createThreadClient(int id, int sock_accept) {

    //Configuration thread
    pthread_t tid;
    client_t *cli = (client_t *)malloc(sizeof(client_t));
    cli->sockfd = sock_accept;
    cli->uid = id;

    //Write client into stack
    queue_add(cli);

    pthread_create(&tid, NULL, client_handle, (void *)cli);
}


