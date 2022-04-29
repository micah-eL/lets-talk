#ifndef LOCAL_LIST_HANDLER_H
#define LOCAL_LIST_HANDLER_H

#include "../model/list.h"


// Global List pointer for local list to be used by input thread and send thread 
extern List* local_message_list;

// Create local message list 
void local_list_init();

// Add message to the local message list
void add_message_local_list(char* message);

// Get first message from local message list
char* get_message_local_list();

// Free local_message_list
void local_list_shutdown();


#endif