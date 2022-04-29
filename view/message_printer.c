#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include "message_printer.h"
#include "../utils/helper.h"
#include "../utils/lets_talk_exit.h"
#include "../controller/remote_list_handler.h"


static pthread_t message_printer_threadID;


void* message_printer_thread() {
	while (1) {
		char* remote_host_message = get_message_remote_list();
        
        // Check for termination condition before printing message
        if (strcmp(remote_host_message, "!exit\n") == 0) {
            free(remote_host_message);
            exit_condition_detected();
            return NULL;
        }

        if (fputs(remote_host_message, stdout) < 0) {
            perror("fputs() in message_printer_thread");
        }
        fflush(stdout);
        free(remote_host_message);
	}
}

void message_printer_init() {
    remote_list_init();

    if (pthread_create(&message_printer_threadID, NULL, message_printer_thread, NULL) != 0) {
        perror("pthread_create() in message_printer_init");
        exit(EXIT_FAILURE);
    }
}

void message_printer_shutdown() {
    pthread_cancel(message_printer_threadID);
    int status = pthread_join(message_printer_threadID, NULL);
    if (status != 0) {
        perror("pthread_join() in message_printer_shutdown()");
    }

    remote_list_shutdown();
}