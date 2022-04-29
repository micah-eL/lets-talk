#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include "receiver.h"
#include "remote_list_handler.h"
#include "local_list_handler.h"
#include "../utils/helper.h"


#define MAX_RECV_MSG_LENGTH 4096

static int receiver_socket_fd;
static pthread_t receiver_threadID;


void *receiver_thread() {
    while (1) {
        char remote_message_buffer[MAX_RECV_MSG_LENGTH];
        size_t numbytes;
        
        if ((numbytes = recvfrom(receiver_socket_fd, remote_message_buffer, MAX_RECV_MSG_LENGTH, 0, NULL, NULL)) < 0) {
            perror("recvfrom() in receiver_thread\n");
            continue;
        }

        // Add decrypted message to remote message list
        char* remote_message = malloc(numbytes * sizeof(char));
        memcpy(remote_message, remote_message_buffer, numbytes);
        decrypt_message(&remote_message);
        if (strcmp(remote_message, "!status\n") == 0) {
            char* status_message = malloc(8 * sizeof(char));
            memcpy(status_message, "Online\n", 8);
            add_message_local_list(status_message);
            free(remote_message);
        }
        else {
            add_message_remote_list(remote_message);
        }

        // Termination condition 
        if (strcmp(remote_message, "!exit\n") == 0) {
            return NULL;
        }
    }
}

void receiver_init(char *port) {
    receiver_socket_fd = udp_socket_init(port);

    if (pthread_create(&receiver_threadID, NULL, receiver_thread, NULL) != 0) {
        perror("pthread_create() in receiver_init");
        exit(EXIT_FAILURE);
    }
}

void receiver_shutdown() {
    pthread_cancel(receiver_threadID);
    int res = pthread_join(receiver_threadID, NULL);
    if (res != 0) {
        perror("pthread_join() in receiver_shutdown\n");
    }
	if ((res = close(receiver_socket_fd)) != 0) {
		perror("close() in receiver_shutdown\n");
	}
}