#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "macros.h"
#include "interface.h"
#include "statemachine.h"
#include "utils.h"

#define BAUDRATE B19200
#define _POSIX_SOURCE 1 /* POSIX compliant source */

void atende();


int main(int argc, char** argv);



int llopen(char * porta, char flag); // flag: TRANSMITTER | RECEIVER

int llwrite(int fd, char * buffer, int length);

int llread(int fd, char * buffer);

int llclose(int fd);