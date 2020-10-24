#pragma once


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "macros.h"
#include <string.h>

char calculateBCC2(char * data);

void send_rej(int fd, int ns);

void send_rr(int fd, int ns);

int send_ua(int fd, char flag);

void send_set(int fd);


int createInfoFrame(char * buf, int s, char * frame);

int byte_stuffing(char* buf, char *res);


int byte_destuffing(char* buf, char *res);

int check_destuffing(char * buf, char bcc2);

int send_disc(int fd, char flag);