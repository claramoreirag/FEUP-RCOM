#include "application.h"


struct applicationLayer application;


int parseArgs(int argc, char ** argv){
    if(argc != 4)
        return -1; 

    strcpy(application.fileDescriptor, argv[3]); //Implementar aqui erros
    
    if(!strcmp(argv[1], "receiver")){
        application.status = RECEIVER;
        strcpy(application.dataFileEmissor, argv[2]);
    }
    
    else if(!strcmp(argv[1], "transmitter")){
        application.status = TRANSMITTER;
        strcpy(application.dataFileReceptor, argv[2]);
    }

    else
        return -1;

}

void buildControlPackages(char * startControl, char * endControl, int fileSize){
    startControl[0] = '2';
    endControl[0] = '3';
    startControl[1] = '0';
    endControl[1] = '0';
    startControl[2] = '1';
    endControl[2] = '1';
    char * temp;
    char size;
    sprintf(temp, "%d", fileSize);
    sprintf(size, "%x", strlen(temp));
    startControl[2] = size;
    endControl[2] = size ;
    strcat(startControl, temp);
    strcat(endControl, temp);
    
}

int appTransmitter(int fd){
    
}



int appReceiver(int fd){
    
    unsigned char package[255]="";
     int fd_file;
     int dataSize;
    while(TRUE){
        llread(fd,package);
         if(package[0]==APP_START){
            parseControl(package);
            fd_file= open(application.dataFileReceptor,O_RDWR | O_CREAT, 0777);
            continue;
         }
         else if (package[0]==APP_END){
             break;

         }
         else if(package[0] == APP_DATA){
            dataSize = 256* package[2]+ package[3];
            write(fd_file, &package[4], dataSize);
         }


    }

     if (close(fd_file)<0)return -1;

  
  return fd;
}


void parseControl(unsigned char * control){
    int filesize=0;
    int infoSize=0;
    
    if(control[1]==T_FILESIZE){
        infoSize=control[2];
        char size[infoSize];
        for (int i =3; i < infoSize+3; i++){
            size[i-3]=control[i];
        }
        filesize=atoi(size);
    }
    application.filesize=filesize;
}



int main(int argc, char **argv){

    parseArgs(argc, argv);

    int fd = llopen(application.fileDescriptor, application.status);

    if (fd < 0)
        return fd;

    if(application.status == TRANSMITTER){
        
    }
    else if(application.status == RECEIVER){

    }
}