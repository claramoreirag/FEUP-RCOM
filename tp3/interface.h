#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

struct applicationLayer {

    int fileDescriptor;/*Descritor correspondente à porta série*/
    int status;/*TRANSMITTER | RECEIVER*/
};

struct linkLayer {
    char port[20];/*Dispositivo /dev/ttySx, x = 0, 1*/
    int baudRate;/*Velocidade de transmissão*/
    unsigned int sequenceNumber;   /*Número de sequência da trama: 0, 1*/
    unsigned int timeout;/*Valor do temporizador: 1 s*/
    unsigned int numTransmissions; /*Número de tentativas em caso defalha*/
    char frame[MAX_SIZE];/*Trama*/
};

