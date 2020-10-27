#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <math.h>

#include "data_link.h"
#include "macros.h"
#include "interface.h"

int parseArgs(int argc, char ** argv);

int appReceiver(int fd);

void parseControl(unsigned char * control);