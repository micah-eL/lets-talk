#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include "remote_list_handler.h"
#include "../utils/helper.h"


List* remote_message_list;
static pthread_mutex_t remote_list_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t non_empty_remote_list = PTHREAD_COND_INITIALIZER;


void remote_list_init() {
    if ((remote_message_list = List_create()) == NULL) {
        printf("Error: List_create() in remote_list_init\n");
		exit(EXIT_FAILURE);
    }
}

void add_message_remote_list(char* message) {
    pthread_mutex_lock(&remote_list_mutex);
        if (List_prepend(remote_message_list, message) == -1) {
            printf("Error: List_prepend() in add_message_remote_list()\n");
            free(message);
        } 
        else if (List_count(remote_message_list) == 1) {
            pthread_cond_signal(&non_empty_remote_list);
        }
    pthread_mutex_unlock(&remote_list_mutex);
}

char* get_message_remote_list() {
    char* message;

    pthread_mutex_lock(&remote_list_mutex);
        if (List_count(remote_message_list) == 0) {
            pthread_cond_wait(&non_empty_remote_list, &remote_list_mutex);
        }
        message = (char*)List_trim(remote_message_list);
    pthread_mutex_unlock(&remote_list_mutex);
    
    return message;
}

void remote_list_shutdown() {
    pthread_mutex_trylock(&remote_list_mutex);
    pthread_mutex_unlock(&remote_list_mutex);
    int res = pthread_mutex_destroy(&remote_list_mutex);
    if (res != 0) {
        perror("pthread_mutex_destroy() in local_list_shutdown");
    }
    if ((res = pthread_cond_destroy(&non_empty_remote_list)) != 0) {
        perror("pthread_cond_destroy() in local_list_shutdown");
    }

    List_free(remote_message_list, (*list_FREE_FN));
}