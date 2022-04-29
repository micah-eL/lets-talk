#include <stdio.h>
#include <stdlib.h>

#include "utils/helper.h"
#include "utils/lets_talk_exit.h"
#include "view/user_input_handler.h"
#include "view/message_printer.h"
#include "controller/sender.h"
#include "controller/receiver.h"


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Error: Not enough arguments\n\n");
        printf("Usage: \n");
        printf("    ./lets-talk <local port> <remote machine name> <remote port>\n");
        printf("Examples: \n");
        printf("    ./lets-talk 3000 192.168.0.513 3001\n");
        printf("    ./lets-talk 3000 some-computer-name 3001\n");
        return 0;
    }

    char* local_machine_port = argv[1];
    char* remote_machine_name = argv[2];
    char* remote_machine_port = argv[3];

    user_input_handler_init();
    message_printer_init();
    sender_init(remote_machine_name, remote_machine_port);
    receiver_init(local_machine_port);

    exit_condition_wait();

    receiver_shutdown();
    sender_shutdown();
    message_printer_shutdown();
    user_input_handler_shutdown();

    exit_cleanup();

    return 0;
}