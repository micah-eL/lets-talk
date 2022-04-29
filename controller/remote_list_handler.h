#ifndef REMOTE_LIST_HANDLER_H
#define REMOTE_LIST_HANDLER_H


#include "../model/list.h"


// Global List pointer for remote list to be used by printer thread and receiver thread 
extern List* remote_message_list;

// Create remote message list 
void remote_list_init();

// Add message to the remote message list
void add_message_remote_list(char* message);

// Get first message from remote message list
char* get_message_remote_list();

// Free remote_message_list
void remote_list_shutdown();


#endif