/*Non-Canonical Input Processing*/

#include "writenoncanonical.h"






int main(int argc, char** argv)
{

  if ( (argc < 2) || (strcmp("/dev/ttyS0", argv[1])!=0) &&
      (strcmp("/dev/ttyS1", argv[1])!=0) && 
        ((strcmp("/dev/ttyS10", argv[1])!=0) && 
        (strcmp("/dev/ttyS11", argv[1])!=0) ) ) {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
    exit(1);
  }

   int fd=llopen(argv[1], TRANSMITTER);


    close(fd);
    return 0;
}
