MAIN_OBJS = main.o
UTILS_OBJS = utils/helper.o utils/lets_talk_exit.o
MODEL_OBJS = model/list.o
VIEW_OBJS = view/user_input_handler.o view/message_printer.o
CONTROLLER_OBJS = controller/sender.o controller/receiver.o controller/local_list_handler.o controller/remote_list_handler.o
OBJS = ${MAIN_OBJS} ${UTILS_OBJS} ${MODEL_OBJS} ${VIEW_OBJS} ${CONTROLLER_OBJS}

EXEC = lets-talk

CFLAGS = -Wall -g


all: lets-talk

lets-talk: ${OBJS}
	gcc ${CFLAGS} ${OBJS} -lpthread -o ${EXEC}

valgrind:
	valgrind ./lets-talk 11111 127.0.0.1 11111

clean:
	rm -f ${EXEC} ${OBJS}
