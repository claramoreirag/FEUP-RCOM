#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include "macros.h"
#include "statemachine.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */




void atende();

void send_set(int fd);

int compare_flags(char * buf);

void assembleStateMachine(StateMachine *machine);

int main(int argc, char** argv);

int createInfoFrame(char * buf, int s, char * frame);

int byte_stuffing(char* buf, char *res);