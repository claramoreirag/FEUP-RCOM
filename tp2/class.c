/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define FLAG 0x7e
#define A_RCV_RSP 0x03
#define A_RCV_CMD 0x01
#define A_SND_CMD 0x03
#define A_SND_RSP 0x01

#define SET 0x03
#define UA 0x07


void send_cmd(int fd, char a, char c){
	unsigned char buf[5];
    buf[0]=FLAG;
    buf[1]=a;
    buf[2]=c;
    buf[3]=a ^ c;
    buf[4]=FLAG;
    write(fd,buf,5);
}

void send_set(int fd)
{
    send_cmd(fd, SET, A_SND_CMD);
}


void send_ua_snd(int fd)
{
    send_resp(fd, UA, A_SND_RSP);
}


volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char buf[255];
    int i, sum = 0, speed = 0;
    
    if ( (argc < 2)) {
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

    /*char input[50];
    gets(input);

    int counter = 0;
    for (int i = 0; i < 50; i++)
    {
      counter++;
      if(input[i] == '\0')
        break;
    }
    printf("counter: %d\n", counter);

    res = write(fd, input, counter);

    printf("wrote input\n");
    char output[counter];
    res = read(fd, output, counter);
    printf("%s \n", output);
*/

    send_set(fd);
    /*
    for (i = 0; i < 255; i++) {
      buf[i] = 'a';
    }
    
    /*testing*/
    /*
    buf[25] = '\n';
    
    res = write(fd,buf,255);   
    printf("%d bytes written\n", res);
    */

  /* 
    O ciclo FOR e as instru��es seguintes devem ser alterados de modo a respeitar 
    o indicado no gui�o 
  */



    sleep(1);
    if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }




    close(fd);
    return 0;
}


