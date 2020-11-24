#pragma once


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "macros.h"
#include "utils.h"

typedef enum State{

    START,
    FLAG_RCV,
    A_RCV,
    C_RCV,
    BCC_OK,
    DATA_OK,
    STOP

}State;


typedef struct StateMachine1{

    State state;
    char byte;
    char flag;
    char a;
    char c;
}StateMachine1;


typedef struct StateMachine2{

    State state;
    unsigned char byte;
    char flag;
    char a;
    unsigned char c1;
    unsigned char c2;

}StateMachine2;


typedef struct StateMachineInfo{

    State state;
    char byte;
    char flag;
    char a;
    char c;

}StateMachineInfo;

void changeState(StateMachine1 *machine);
void assembleStateMachine(StateMachine1 *machine, char a, char c);

void changeState2(StateMachine2 *machine);
void assembleStateMachine2(StateMachine2 * machine, char a, int r); 

void changeInfoState(StateMachineInfo * machine);
void assembleStateMachineInfo(StateMachineInfo * machine, char a, int r);

