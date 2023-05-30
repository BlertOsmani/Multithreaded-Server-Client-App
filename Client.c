#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 300

// Structure for the message
struct message {
    long mtype;
    pid_t client_pid;
    char mtext[MAX_MSG_SIZE];
};

int main() {
    key_t key;
    int msgQueueId;
    struct message msg;
    int connected = 0;

    key = ftok(".", 'm');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    msgQueueId = msgget(key, 0666);
    if (msgQueueId == -1) {
        printf("Error: Failed to create or access the message queue.\n");
        perror("msgget");
        exit(1);
    }

    printf("Client started. PID: %d\n", getpid());

    // Send requests to the server
    while (1) {
        printf("Enter message: ");
        fgets(msg.mtext, sizeof(msg.mtext), stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
       

    if(strcmp(msg.mtext, "Connect") == 0){
    msg.mtype = 2;
    msg.client_pid = getpid();
    connected = 1;
    }
   
    if(connected == 1){
        // Check if the input is "Disconnect"
        if (strcmp(msg.mtext, "Disconnect") == 0) {
            msg.mtype = 2; // Use message type 2 for disconnect request
            msg.client_pid = getpid();

            if (msgsnd(msgQueueId, &msg, sizeof(msg.mtext), 0) == -1) {
                printf("Error: Disconnect request not sent\n");
                perror("msgsnd");
                exit(1);
            }

            printf("Disconnected from the server.\n");
            break;
        }

        // Send the message to the server for processing
        msg.mtype = 1; // Use message type 1 for regular requests
        msg.client_pid = getpid();

        if (msgsnd(msgQueueId, &msg, sizeof(msg.mtext), 0) == -1) {
            printf("Error: Message not sent\n");
            perror("msgsnd");
            exit(1);
        }

        if (msgrcv(msgQueueId, &msg, sizeof(msg.mtext), getpid(), 0) == -1) {
            printf("Error: Response not received\n");
            perror("msgrcv");
            exit(1);
        }

        printf("Received response from server: %s\n", msg.mtext);
        sleep(1);
       
    }
    else{
       printf("Error: Not connected to the server. Enter 'Connect' to establish a connection.\n");
            continue;
   
    }
    }
   

    return 0;
}
