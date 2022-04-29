#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include "lets_talk_exit.h"


static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t program_terminated = PTHREAD_COND_INITIALIZER;


void exit_condition_wait() {
    pthread_mutex_lock(&mutex);
        pthread_cond_wait(&program_terminated, &mutex);
    pthread_mutex_unlock(&mutex);
}

void exit_condition_detected() {
    pthread_mutex_lock(&mutex);
        pthread_cond_signal(&program_terminated);
    pthread_mutex_unlock(&mutex);
}

void exit_cleanup() {
    int res; 
	if ((res = pthread_mutex_destroy(&mutex)) != 0) {
		perror("pthread_mutex_destroy() in exit_cleanup\n");
	}
	if ((res = pthread_cond_destroy(&program_terminated)) != 0) {
		perror("pthread_cond_destroy()) in exit_cleanup\n");
	}
}