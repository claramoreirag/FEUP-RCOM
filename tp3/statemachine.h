#pragma once


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "macros.h"

typedef enum State{

    START,
    FLAG_RCV,
    A_RCV,
    C_RCV,
    BCC_OK,
    STOP

} State;


typedef struct StateMachine{

    State state;
    char byte;
    char flag;
    char a;
    char c;


}StateMachine;



void changeState(StateMachine *machine);


void assembleStateMachine(StateMachine *machine, char a, char c);