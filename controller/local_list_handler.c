#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include "local_list_handler.h"
#include "../utils/helper.h"


List* local_message_list;
static pthread_mutex_t local_list_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t non_empty_local_list = PTHREAD_COND_INITIALIZER;


void local_list_init() {
    if ((local_message_list = List_create()) == NULL) {
        printf("Error: List_create() in local_list_init\n");
		exit(EXIT_FAILURE);
    }
}

void add_message_local_list(char* message) {
    pthread_mutex_lock(&local_list_mutex);
        if (List_prepend(local_message_list, message) == -1) {
            printf("Error: List_prepend() in add_message_local_list()\n");
            free(message);
        } 
        else if (List_count(local_message_list) == 1) {
            pthread_cond_signal(&non_empty_local_list);
        }
    pthread_mutex_unlock(&local_list_mutex);
}

char* get_message_local_list() {
    char* message;

    pthread_mutex_lock(&local_list_mutex);
        if (List_count(local_message_list) == 0) {
            pthread_cond_wait(&non_empty_local_list, &local_list_mutex);
        }
        message = (char*)List_trim(local_message_list);
    pthread_mutex_unlock(&local_list_mutex);
    
    return message;
}

void local_list_shutdown() {
    pthread_mutex_trylock(&local_list_mutex);
    pthread_mutex_unlock(&local_list_mutex);
    int res = pthread_mutex_destroy(&local_list_mutex);
    if (res != 0) {
        perror("pthread_mutex_destroy() in local_list_shutdown\n");
    }
    if ((res = pthread_cond_destroy(&non_empty_local_list)) != 0) {
        perror("pthread_cond_destroy() in local_list_shutdown\n");
    }

    List_free(local_message_list, (*list_FREE_FN));
}