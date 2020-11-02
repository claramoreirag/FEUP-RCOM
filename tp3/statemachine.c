#include "statemachine.h"
 char control_flag;

void changeState(StateMachine1 *machine){

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
            else if((machine->byte == machine->a )^ machine->c)
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

        default:
        break;
            
    }
    // printf("Current state: %d \n",machine->state);
}


void assembleStateMachine(StateMachine1 *machine, char a, char c){
  
  machine->state = START;
  machine->flag = FLAG;
  machine->a = a;
  machine->c = c;
}


void changeState2(StateMachine2 *machine){

   

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
            else if(machine->byte == 0x85|| machine->byte == 0x81 ||machine->byte == 0x05|| machine->byte == 0x01 )
            {
                machine->state = C_RCV;
                control_flag = machine->byte;
            }
            else
                machine->state = START;
        break;

        case C_RCV:
            if(machine->byte == machine->flag)
                machine->state = FLAG_RCV;
            else if((machine->byte == machine->a )^ control_flag)
                machine->state = BCC_OK;
            else {
               
                machine->state = START;
            }

        break;

        case BCC_OK:
            if(machine->byte == machine->flag)
                machine->state = STOP;
            else 
                machine->state = START;
            

        break;

        default:
        break;
            
    }
    // printf("Current state: %d \n",machine->state);

}


void assembleStateMachine2(StateMachine2 * machine, char a, int r){

    machine->state = START;
    machine->flag = FLAG;
    machine->a = a;
    if(r){
        machine->c1 = 0x85;
        machine->c2 = 0x81;
    }
    else
    {
        machine->c1 = 0x05;
        machine->c2 = 0x01;   
    }
}

void changeInfoState(StateMachineInfo * machine){
    
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
            else if(machine->byte == 0x40 ||machine->byte == 0x00){
                machine->c =machine->byte;
                machine->state = C_RCV;
            }
            else
                machine->state = START;
        break;

        case C_RCV:
            if(machine->byte == machine->flag)
                machine->state = FLAG_RCV;
            else if((machine->byte == machine->a) ^ machine->c)
                machine->state = BCC_OK;
            else
                machine->state = START;

        break;

        case BCC_OK:
            machine->state = DATA_OK;

        break;

        case DATA_OK:
            if(machine->byte == machine->flag)
                machine->state = STOP;
                
        break;

        default:
        break;
            
    }
    //printf("Current state: %d \n",machine->state);

}

void assembleStateMachineInfo(StateMachineInfo * machine, char a, int r){

    machine->state = START;
    machine->flag = FLAG;
    machine->a = a;
}