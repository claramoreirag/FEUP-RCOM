all: emissor recetor app
CC = gcc
CFLAGS = -Wall

OBJS= statemachine.o data_link.o utils.o application.o


SRCS_A = statemachine.c  data_link.c utils.c application.c
OBJS_A=$(SRCS:.c=.o)

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


app:  $(OBJS)
	$(CC) $(SRCS_A) -o app $(OBJS_A) $(CFLAGS)
clean :
	-rm $(OBJS) $(EXEC)