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


// Function to add a client to the list of connected clients
void addClient(pid_t client_pid, int msg_queue_id) {
    sem_wait(&clientsSemaphore);

    if (numClients < MAX_CLIENTS) {
        clients[numClients].client_pid = client_pid;
        clients[numClients].msg_queue_id = msg_queue_id;

        // Create a new thread for the client
        if (pthread_create(&clients[numClients].thread_id, NULL, processClient, &clients[numClients]) != 0) {
            fprintf(stderr, "Failed to create thread for client %d\n", client_pid);
            exit(1);
        }

        numClients++;

        // Send a response to the client
        struct message response;
        response.mtype = client_pid;
        response.client_pid = getpid();
        snprintf(response.mtext, sizeof(response.mtext), "You have been connected to the server");
        if (msgsnd(msg_queue_id, &response, sizeof(response.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    } else {
        fprintf(stderr, "Max number of clients reached. Unable to add client %d\n", client_pid);
    }

    sem_post(&clientsSemaphore);
}

// Function to remove a client from the list of connected clients
void removeClient(pid_t client_pid) {
    sem_wait(&clientsSemaphore);

    int i, clientIndex = -1;
    for (i = 0; i < numClients; i++) {
        if (clients[i].client_pid == client_pid) {
            clientIndex = i;
            break;
        }
    }

    if (clientIndex != -1) {
        // Terminate the client's thread
        if (pthread_cancel(clients[clientIndex].thread_id) != 0) {
            fprintf(stderr, "Failed to terminate thread for client %d\n", client_pid);
            exit(1);
        }

        // Remove the client from the list
        numClients--;
        for (i = clientIndex; i < numClients; i++) {
            clients[i] = clients[i + 1];
        }
    } else {
        fprintf(stderr, "Client %d not found in the list of connected clients\n", client_pid);
    }

    sem_post(&clientsSemaphore);
}
