all:  application
CC = gcc
CFLAGS = -Wall

OBJS= statemachine.o data_link.o utils.o application.o


SRCS_A = statemachine.c  data_link.c utils.c application.c
OBJS_A=$(SRCS:.c=.o)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@



application:  $(OBJS)
	$(CC) $(SRCS_A) -o application $(OBJS_A) $(CFLAGS)
clean :
	-rm $(OBJS) $(EXEC)