#include "statemachine.h"


void changeState(StateMachine *machine){

    switch(machine->state){
        
        case START:
            if(machine->byte == machine->flag)
                machine->state = FLAG_RCV;
        break;

        case FLAG_RCV:
            if(machine->byte == machine->flag)
                machine->state = FLAG_RCV;
            else if (machine->byte == machine->a)
                machine->state = A_RCV;
            else
                machine->state = START;
        break;
        
        case A_RCV:
            if(machine->byte == machine->flag)
                machine->state = FLAG_RCV;
            else if(machine->byte == machine->c)
                machine->state = C_RCV;
            else
                machine->state = START;
        
        break;

        case C_RCV:
            if(machine->byte == machine->flag)
                machine->state = FLAG_RCV;
            else if(machine->byte == machine->a ^ machine->c)
                machine->state = BCC_OK;
            else
                machine->state = START;

        break;

        case BCC_OK:
            if(machine->byte == machine->flag)
                machine->state = STOP;
            else 
                machine->state = START;
        break;

            
    }
    printf("Current state: %d \n",machine->state);
}