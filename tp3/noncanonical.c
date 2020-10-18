#include "noncanonical.h"

/*Non-Canonical Input Processing*/






void send_ua(int fd)
{
  	unsigned char buf[5];
    buf[0]=FLAG;
    buf[1]=A_RSP_RECETOR;
    buf[2]=UA;
    buf[3]=A_RSP_RECETOR^ UA;
    buf[4]=FLAG;
    write(fd,buf,5);

    printf("UA sent: %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

}

void assembleStateMachine(StateMachine *machine){
  
  machine->state = START;
  machine->flag = FLAG;
  machine->a = A_CMD_EMISSOR;
  machine->c = SET;
  
}


int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;


    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS10", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS11", argv[1])!=0) ) ) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
  
    
    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) pr�ximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");


    StateMachine machine;
    assembleStateMachine(&machine);

    char buf[255];
   
    int stop = 0;
    int i = 0;
    while(!stop)
    {
      res = read(fd, &buf[i], 1);
      printf("byte read: %x\n", buf[i]);
      machine.byte = buf[i];
      changeState(&machine);
      if(machine.state==STOP)stop = 1;
      i++;
    }
    printf("SET received! \n");

    send_ua(fd);



    sleep(1);
    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}