#ifndef HELPER_H
#define HELPER_H


// Create a UDP socket - socket is bind()'d to port
int udp_socket_init(char* port);

// Free function pointer for List_free()
void (*list_FREE_FN)(void*);

// Encrypt message using ENCRYPTION_KEY
void encrypt_message(char **message);

// Decrypt message using ENCRYPTION_KEY
void decrypt_message(char **message);


#endif