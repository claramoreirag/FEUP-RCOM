#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "macros.h"
#include "statemachine.h"
#include "data_link.h"
#include "application.h"
#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define A_RCV_RSP 0x03
#define A_RCV_CMD 0x01
#define A_SND_CMD 0x03
#define A_SND_RSP 0x01




int main(int argc, char** argv);