#ifndef MESSAGE_PRINTER_H
#define MESSAGE_PRINTER_H


// Creates remote message list then starts message_printer_thread() 
void message_printer_init();

// Prints messages from remote host, terminates if remote host message == "!exit"
void *message_printer_thread();

// Terminates message_printer_thread and shuts down remote message list 
void message_printer_shutdown();


#endif