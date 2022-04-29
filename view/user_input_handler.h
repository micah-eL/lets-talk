#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H


// Creates local message list then starts user_input_thread() 
void user_input_handler_init();

// Handles user input, exits if user message == "!exit"
void *user_input_thread();

// Terminates user_input_thread and shuts down local message list 
void user_input_handler_shutdown();


#endif