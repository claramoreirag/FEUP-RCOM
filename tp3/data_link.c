#include "data_link.h"


int ns=0;

int resend=0, conta=0;

void atende()                   // atende alarme
{
	printf("alarme # %d\n", conta);
	resend=1;
}


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

void send_set(int fd)
{
    unsigned char buf[5];
    buf[0]=FLAG;
    buf[1]=0x03;
    buf[2]=SET;
    buf[3]=0x03 ^SET;
    buf[4]=FLAG;
    write(fd,buf,5);
   printf("SET sent %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
}



int byte_stuffing(char* buf, char *res){

  int j=0; 
  for(int i=0; i<strlen(buf);i++){
    if(buf[i]==FLAG){
      res[j]=ESC;
      res[++j]=FLAG^BYTE_STUFF;
    }
    else if(buf[i]==ESC){
      res[j]=ESC;
      res[++j]=ESC^BYTE_STUFF;
    }
    else{
      res[j]=buf[i];
    }
    j++;
  }
 
  return 0;

}

int byte_destuffing(char* buf, char *res){

  int j=0; 
  for(int i=0; i<strlen(buf);i++){
    if(buf[i]==ESC && buf[i+1]==(FLAG^BYTE_STUFF)){
        i++;
        res[j]=FLAG;
    }
    else if(buf[i]==ESC && buf[i+1]==(ESC^BYTE_STUFF)){
      i++;
      res[j]=ESC;
    }
    else
      res[j]=buf[i];
    
    j++;
  }
  
  return 0;

}


int createInfoFrame(char * buf, int s, char * frame){
  char res[255];
  byte_stuffing(buf,res);
  frame[0]=FLAG;
  frame[1]=A_CMD_EMISSOR;
  if(s==0)frame[2]=0x00;
  else frame[2]=0x40;
  for(int i=0; i<strlen(res);i++){
    frame[i+3]=res[i];
  }
  char bcc;
  for(int i=0;i<strlen(buf);i++){
    bcc=bcc^ buf[i];
  }
  frame[2+strlen(res)]=bcc;
  frame[3+strlen(res)]=FLAG;
  
  return 0;
}


int llopen(char * porta, char flag){
    
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];
    int i, sum = 0, speed = 0;
   

    fd = open(porta, O_RDWR | O_NOCTTY );
    if (fd <0) {perror(porta); exit(-1); }

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

    newtio.c_cc[VTIME]    = 5;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */

    (void) signal(SIGALRM, atende); 


    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

   
    if(flag==TRANSMITTER){

          StateMachine machine;
          
          assembleStateMachine(&machine,A_RSP_RECETOR,UA);

          do {
            conta++;
            i=0;
            resend = 0;

            printf("Before send set\n");
            send_set(fd);

            alarm(3);

            while (machine.state != STOP && !resend) {       /* loop for input */
              
              res = read(fd,&buf[i],1);   /* returns after 1 char has been input */
              if (res == 0) continue;
              printf("byte: %x\n", buf[i]);
              machine.byte = buf[i];
              changeState(&machine);
              i++;
            }

          } while (conta< 3 && machine.state != STOP);

          
          if(conta>=3){
            printf("UA not received, exiting\n");
            sleep(1);
            if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
              perror("tcsetattr");
              exit(-1);
            }
            close(fd);
            return -1;
          }
          else printf("UA received \n");

    }

    else if(flag==RECEIVER){
          newtio.c_lflag = 0;
          newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
          newtio.c_cc[VMIN]     = 1; 

          tcflush(fd, TCIOFLUSH);

          if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
            perror("tcsetattr");
            exit(-1);
          }
        

           StateMachine machine_r;
          assembleStateMachine(&machine_r,A_CMD_EMISSOR,SET);

          char buf[255];
        
          int stop = 0;
          int i = 0;
          while(!stop)
          {
            res = read(fd, &buf[i], 1);
            printf("byte read: %x\n", buf[i]);
            machine_r.byte = buf[i];
            changeState(&machine_r);
            if(machine_r.state==STOP)stop = 1;
            i++;
          }
          printf("SET received! \n");

          send_ua(fd);
  }
    
    else  return -1;

    
    return fd;

}

int llwrite(int fd, char * buffer, int length){
    

}

int llread(int fd, char * buffer){

}

int llclose(int fd){
    
}