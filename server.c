#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_MSG_SIZE 300
#define MAX_CLIENTS 3

//Structure for the message 
struct message{
long mtype;
pid_t client_pid;
char mtext[MAX_MSG_SIZE];
};

//Structure for connected client
struct connected_client{
pid_t client_pid;
int msg_queue_id;
pthread_t thread_id;
};

//Global variables
key_t key;
int msgQueueId;
struct connected_client clients[MAX_CLIENTS];
int numClients = 0;
sem_t clientsSemaphore;

//Function to process client requests
void* processClient(void* arg){
struct connected_client* client = (struct connected_client*)arg;
struct message msg;

//Continuously listen for messages from client
while(1){
 // Receive a message from the client
        if (msgrcv(client->msg_queue_id, &msg, sizeof(msg.mtext), getpid(), 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        // Print the received message
        printf("Received message from client %d: %s\n", msg.client_pid, msg.mtext);

        // Process the message (e.g., perform desired operations)

        // Send a response back to the client
        char response[MAX_MSG_SIZE + 200];
        snprintf(response, sizeof(response), "Response from server: %s", msg.mtext);
        strcpy(msg.mtext, response);

        msg.mtype = msg.client_pid;
        if (msgsnd(client->msg_queue_id, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    return NULL;
}
