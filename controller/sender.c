#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "sender.h"
#include "local_list_handler.h"
#include "../utils/lets_talk_exit.h"
#include "../utils/helper.h"


static pthread_t sender_threadID;
static struct addrinfo hints, *remote_machine_addrinfo, *itr;
static char *remote_machine_port, *remote_machine_name;
static int remote_machine_socket_fd;


void *sender_thread() {
    // Setup hints addrinfo
    memset(&hints, 0, sizeof hints); 
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // Generate all possible addrinfo structs for given IP and port number
    int result = getaddrinfo(remote_machine_name, remote_machine_port, &hints, &remote_machine_addrinfo);
    if (result != 0) {
		perror("getaddrinfo() in sender_thread\n");
		exit(EXIT_FAILURE);
	}

    // Loop through all addrinfo structs and bind our port to the first we can
    for(itr = remote_machine_addrinfo; itr != NULL; itr = itr->ai_next) {
		if ((remote_machine_socket_fd = socket(itr->ai_family, itr->ai_socktype, itr->ai_protocol)) == -1) {
			perror("socket() in sender_thread\n");
			continue;
		}
		break;
	}
    if (itr == NULL) {
		perror("socket() in sender_thread\n");
		exit(EXIT_FAILURE);
	}

    while (1) {
        // Get message from local message list, then encrypt message before sending it
        int exit = 0;
		char* message = get_message_local_list();
        if (strcmp(message, "!exit\n") == 0) {
            exit = 1;
        }
        if (strcmp(message, "!status\n") == 0) {
            // Set a timer
        }
        encrypt_message(&message);
		int numbytes = sendto(remote_machine_socket_fd, message, strlen(message), 0, itr->ai_addr, itr->ai_addrlen);
        if (numbytes < 0) {
            perror("sendto() in sender_thread");
        }

        if (exit) {
            free(message);
            exit_condition_detected();
            return NULL;
        }
        free(message);
	}
}

void sender_init(char *machine_name, char *port) {
    remote_machine_name = machine_name;
    remote_machine_port = port;
    
    if (pthread_create(&sender_threadID, NULL, sender_thread, NULL) != 0) {
        perror("pthread_create() in sender_init");
        exit(EXIT_FAILURE);
    }
}

void sender_shutdown() {
    pthread_cancel(sender_threadID);
    int res = pthread_join(sender_threadID, NULL);
    if (res != 0) {
        perror("pthread_join() in sender_shutdown\n");
    }
    if ((res = close(remote_machine_socket_fd)) != 0) {
		perror("close() in sender_shutdown\n");
	}
}