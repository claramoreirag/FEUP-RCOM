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


#define FLAG 0x7e
#define A_RCV_RSP 0x03
#define A_RCV_CMD 0x01
#define A_SND_CMD 0x03
#define A_SND_RSP 0x01

#define SET 0x03
#define UA 0x07


void atende();

void send_set(int fd);

int compare_flags(char * buf);

void assembleStateMachine(StateMachine *machine);

int main(int argc, char** argv);

int createInfoFrame(char * buf, int s);

int byte_stuffing(char* buf, char *res);