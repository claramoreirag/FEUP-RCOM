#include "application.h"


struct applicationLayer application;

int dataSize = 255; //Tamanho de cada vez que se vai ler a data do ficheiro; tamanho provisório





int parseArgs(int argc, char ** argv){
    
    if(argc != 4){
        return -1; 
    }
    strcpy(application.fileDescriptor, argv[3]); //Implementar aqui erros
    
    if(!strcmp(argv[1], "receiver")){
        application.status = RECEIVER;
        printf("%s\n", argv[2]);
        strcpy(application.dataFileReceptor, argv[2]);
        
    }
    
    else if(!strcmp(argv[1], "transmitter")){
        application.status = TRANSMITTER;
        printf("ARG: %s\n", argv[2]);
        strcpy(application.dataFileEmissor, argv[2]);
        printf("Data File Receptor: %s\n", application.dataFileEmissor);
    }

    else
        return -1;

}


int createControlPackage(unsigned char controlByte, unsigned char *packetBuffer, int fileSize)
{
    packetBuffer[0] = controlByte;

    packetBuffer[1] = T_FILESIZE;

    int length = 0;
    int currentFileSize = fileSize;

    // cicle to separate file size (v1) in bytes
    while (currentFileSize > 0)
    {
        int rest = currentFileSize % 256;
        int div = currentFileSize / 256;
        length++;

        // shifts all bytes to the right, to make space for the new byte
        for (unsigned int i = 2 + length; i > 3; i--)
            packetBuffer[i] = packetBuffer[i - 1];

        packetBuffer[3] = (unsigned char)rest;

        currentFileSize = div;
    }

    packetBuffer[2] = (unsigned char)length;

    return 3 + length ; // total length of the packet
}



int appTransmitter(int fd){ //Testar
   printf("transmitter\n");
    char data[255]="";
    char dataPackage[255]=""; //
    unsigned char controlPackage[10];
    int bytes_read;
    int sequenceNumber = 0;
    char * file_data[255]; //
    printf("%s\n",application.dataFileEmissor);
    int fd_file;
    printf("DATA FILE EMISSOR IN APP TRANSMITTER: %s\n", application.dataFileEmissor);
    fd_file = open(application.dataFileEmissor, O_RDONLY);
    if(fd_file < 0){
        printf("TRANSMITTER FILE NOT OPEN\n");
        return -1;
    }
    struct stat fileInfo;
    if (stat(application.dataFileEmissor, &fileInfo)<0)return -1;
  
     /*building control package*/
    int size =createControlPackage(APP_START,controlPackage,fileInfo.st_size);
    print_hex(controlPackage,size);
    llwrite(fd,controlPackage,size);


    while( (bytes_read = read(fd_file,file_data,255-4)) != 0){
        /*building data package*/
        dataPackage[0] = APP_DATA;
        dataPackage[1] = sequenceNumber % 255;
        dataPackage[2] = bytes_read / 256;
        dataPackage[3] = bytes_read % 256;
        memcpy(&dataPackage[4],file_data,bytes_read);

        llwrite(fd,dataPackage,bytes_read+4);
        printf("sequence number: %d \n",sequenceNumber);
        sequenceNumber++;
    }

    controlPackage[0] = APP_END;

    llwrite(fd,controlPackage,size);

    return fd;

}


int appReceiver(int fd){
    printf("receiver");
    unsigned char package[255]="";
    int fd_file;
    int dataSize;
    while(TRUE){
        int packetSize=llread(fd,package);
        print_hex(package,packetSize);
         if(package[0]==APP_START){
            parseControl(package);
            fd_file= open(application.dataFileReceptor,O_WRONLY | O_CREAT | O_TRUNC , 0777);
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
        printf("Going to enter app transmitter\n");
        appTransmitter(fd);

    }
    else if(application.status == RECEIVER){
        printf("Going to enter app receiver\n");
        appReceiver(fd);
    }

    llclose(fd);
    return 0;
}