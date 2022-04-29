#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "user_input_handler.h"
#include "../utils/helper.h"
#include "../controller/local_list_handler.h"


#define MAX_USER_INPUT_LENGTH 4096

static pthread_t user_input_threadID;


void* user_input_thread() {	
    while (1) {
        char user_input_buffer[MAX_USER_INPUT_LENGTH];
        size_t message_length;
        
        // Get user input
        if (!fgets(user_input_buffer, MAX_USER_INPUT_LENGTH, stdin)) {
            perror("fgets() in user_input_thread");
            continue;
        }

        // Add message to local message list
        message_length = strlen(user_input_buffer);
        char* user_input_message = malloc(message_length * sizeof(char));
        memcpy(user_input_message, user_input_buffer, message_length);
        add_message_local_list(user_input_message);

        // Termination condition 
        if (strcmp(user_input_message, "!exit\n") == 0) {
            return NULL;
        }
    }
}

// Create a empty list and a thread that adds user input to the newly created list
void user_input_handler_init() {
    local_list_init();

    if (pthread_create(&user_input_threadID, NULL, user_input_thread, NULL) != 0) {
        perror("pthread_create() in user_input_handler_init");
        exit(EXIT_FAILURE);
    }
}

// Cancel and wait for thread to finish, then cleanup memory
void user_input_handler_shutdown() {
    pthread_cancel(user_input_threadID);
    int result = pthread_join(user_input_threadID, NULL);
    if (result != 0) {
        perror("pthread_join() in user_input_handler_shutdown");
    }

    local_list_shutdown();
}