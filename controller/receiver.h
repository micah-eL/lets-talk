#ifndef RECEIVER_H
#define RECEIVER_H


// Creates local socket then starts receiver_thread
void receiver_init(char *port);

// Adds messages from remote message to remote message list, exits if message == "!exit"
void *receiver_thread();

// Terminates receiver_thread 
void receiver_shutdown();


#endif