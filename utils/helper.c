#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include "helper.h"


#define ENCRYPTION_KEY 1


int udp_socket_init(char *port) {
    int res, socket_fd;
	struct sockaddr_in sin;

	// Initialize sockaddr struct
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(atoi(port));

	// Create UDP socket 
    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket() in udp_socket_init");
        exit(EXIT_FAILURE);
    }
	// Bind socket to port parameter
    if ((res = bind(socket_fd, (struct sockaddr*) &sin, sizeof(sin))) == -1) {
        perror("bind() in udp_socket_init");
        exit(EXIT_FAILURE);
    }

	return socket_fd;
}

void free_list_item(void* item) {
	free(item);
}
void (*list_FREE_FN)(void*) = &free_list_item;

void encrypt_message(char **message) {
	size_t len = strlen(*message) + 1;
	char encrypted_message[len];

	for (int i = 0; i < len; i++) {
		encrypted_message[i] = (*message)[i] + ENCRYPTION_KEY;
	}
	encrypted_message[len] = '\0';

	memcpy(*message, encrypted_message, len);
}

void decrypt_message(char **message) {
	size_t len = strlen(*message) + 1;
	char decrypted_message[len];

	for (int i = 0; i < len; i++) {
		decrypted_message[i] = (*message)[i] - ENCRYPTION_KEY;
	}
	decrypted_message[len] = '\0';

	memcpy(*message, decrypted_message, len);
}
