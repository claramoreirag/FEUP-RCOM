#include "application.h"


struct applicationLayer application;

int dataSize = 50; //Tamanho de cada vez que se vai ler a data do ficheiro; tamanho provisório


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

void buildControlPackages(char * startControl, char * endControl, int fileSize){ //Testar
    startControl[0] = APP_START;
    endControl[0] = APP_END;
    startControl[1] = T_FILESIZE;
    endControl[1] = T_FILESIZE;
    startControl[2] = '1';
    endControl[2] = '1';
    char * temp;
    char * size;
    sprintf(temp, "%d", fileSize);
    sprintf(size, "%x", strlen(temp));
    strcat(startControl, size);
    strcat(endControl, size);
    strcat(startControl, temp);
    strcat(endControl, temp);
    
}


void buildDataPackage(int sequence_number, char * data, char * dataPackage){ //Testar
    dataPackage[0] = APP_DATA;
    char * sequence_number;
    sprintf(sequence_number, "%x", sequence_number);
    strcat(dataPackage, sequence_number);
    //Adicionar aqui L1 e L2
    strcat(dataPackage, data);
}


int appTransmitter(int fd){ //Testar

    char data[255];
    char dataPackage[255];
    int bytes_read;
    int sequence_number = 0;

    int fd_file;
    fd_file = open(application.dataFileEmissor, O_RDONLY);
    if(fd_file < 0)
        return -1;

    char startControl[255];
    char endControl[255];
    buildControlPackages(startControl, endControl, application.filesize);
    llwrite(fd, startControl, strlen(startControl));

    while(bytes_read = read(fd_file, data, dataSize)){

        buildDataPackage(sequence_number, data, dataPackage);
        llwrite(fd, dataPackage, strlen(dataPackage));
        sequence_number++;
        //clear data e dataPackage
        memset(data, 0, strlen(data));
        memset(dataPackage, 0, strlen(dataPackage));
    }

    llwrite(fd, endControl, strlen(endControl));

    close(fd_file);

    return fd;

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