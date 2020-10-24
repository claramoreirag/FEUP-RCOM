all: emissor recetor
CC = gcc
CFLAGS = -Wall

OBJS=noncanonical.o statemachine.o writenoncanonical.o data_link.o utils.o

SRCS_R = statemachine.c noncanonical.c data_link.c utils.c
OBJS_R= $(SRCS:.c=.o)

SRCS_E =statemachine.c writenoncanonical.c data_link.c utils.c
OBJS_E= $(SRCS:.c=.o)
%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
emissor: $(OBJS)
	$(CC) $(SRCS_E) -o emissor $(OBJS_E) $(CFLAGS)
recetor: $(OBJS)
	$(CC) $(SRCS_R) -o recetor $(OBJS_R) $(CFLAGS)
clean :
	-rm $(OBJS) $(EXEC)