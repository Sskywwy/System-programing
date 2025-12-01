#include "common.h"
int sock = 0;
char name[32];
volatile __sig_atomic_t flag = 0;



//Prototypes
void *send_msg_handler();
void *rcv_msg_handler();

//Main function
int main()
{
    //Handshake
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    //Client configuration
    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t len = sizeof(serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //Check address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    //Connect and handshake
    CHECK((connect(sock, (struct sockaddr *)&serv_addr, len)), "ERROR")
    send(sock, name, 32, 0);
    printf("=== WELCOME TO THE CHATROOM, %s ===\n", name);

    //Create thread for sending message
    pthread_t send_msg;
    pthread_create(&send_msg, NULL, (void *) send_msg_handler, NULL);

    //Create thread for receive message
    pthread_t rcv_msg;
    pthread_create(&rcv_msg, NULL, (void *) rcv_msg_handler, NULL);

    //Loop exit command
    while (1)
    {
        if (flag)
        {
            printf("bye\n");
            break;
        }
        
    }

    //Clean memory
    close(sock);
    
    return 0;
}


//Send msg handler
void *send_msg_handler(){
    char buffer[BUFFER_SIZE];
    while (1)
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        else{
            send(sock, buffer, strlen(buffer), 0);
        }
        memset(buffer, 0, BUFFER_SIZE);
    }
    close(sock);
    flag = 1;
    return NULL;
}

//Receive message handler
void *rcv_msg_handler(){
    char buffer[BUFFER_SIZE];
    while (1){
        int receive = recv(sock, buffer, BUFFER_SIZE, 0);
        if (receive > 0)
        {
            printf("%s", buffer);

            //For immediately print
            fflush(stdout);
        }
        else if (receive == 0)
        {
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
        

    }
    return NULL;
}
