#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

struct applicationLayer {

    char fileDescriptor[30];/*Descritor correspondente à porta série*/
    char dataFileEmissor[30]; /*Path para o ficheiro onde estão os dados*/
    char dataFileReceptor[30]; /*Path para o ficheiro que vai receber os dados*/
    int status;/*TRANSMITTER | RECEIVER*/
    int filesize;
    
};

struct linkLayer {
    char port[20];/*Dispositivo /dev/ttySx, x = 0, 1*/
    int baudRate;/*Velocidade de transmissão*/
    unsigned int sequenceNumber;   /*Número de sequência da trama: 0, 1*/
    unsigned int timeout;/*Valor do temporizador: 1 s*/
    unsigned int numTransmissions; /*Número de tentativas em caso defalha*/
    char frame[255];/*Trama*/
};

