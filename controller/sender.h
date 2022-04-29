#ifndef SENDER_H
#define SENDER_H


// Creates socket for remote machine then starts sender_thread
void sender_init(char *machine_name, char *port);

// Sends messages from local message list to remote machine, signals exit condition if message == "!exit"
void *sender_thread();

// Terminates sender_thread 
void sender_shutdown();


#endif