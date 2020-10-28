#include "application.h"


struct applicationLayer application;

int dataSize = 50; //Tamanho de cada vez que se vai ler a data do ficheiro; tamanho provisório


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
/* 
unsigned char l1(long fileSize) {
    int nbits = ceil(log2(fileSize));
    int bytes_read= nbits / 8;
    if (nbits % 8)bytes_read++;

    return bytes_read;
}

void v1(long fileSize, char* size_in_octets, unsigned arr_size) {
    char octet = (unsigned char)fileSize;

    for (int i = arr_size - 1; i >= 0; --i) {
        size_in_octets[i] = octet;
        fileSize = fileSize >> 8;
        octet = (unsigned char)fileSize;
    }
}

void buildControlPackages(char * startControl, char * endControl, long fileSize){ //Testar
    startControl[0] = APP_START;
    endControl[0] = APP_END;
    startControl[1] = T_FILESIZE;
    endControl[1] = T_FILESIZE;
    startControl[2] = l1(fileSize);
    endControl[2] = l1(fileSize);
    char v1[endControl[2]];
    v1(bytes, v1, l1)

} */

/* void buildDataPackage(int sequence_number, char * data, char * dataPackage){ //Testar
    dataPackage[0] = APP_DATA;
    char * sequence_number;
    sprintf(sequence_number, "%x", sequence_number);
    strcat(dataPackage, sequence_number);
    //Adicionar aqui L1 e L2
    strcat(dataPackage, data);
}
 */
int appTransmitter(int fd){ //Testar
   printf("transmitter\n");
    char data[255]="";
    char dataPackage[255]=""; //
    char controlPackage[255];
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
  

    /* char startControl[255]="";
    char endControl[255]="";
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

    close(fd_file); */

     /*building control package*/
    controlPackage[0] = APP_START;
    controlPackage[1] = T_FILESIZE;
    controlPackage[2] = sizeof(fileInfo.st_size); /* sending size of off_t */
    memcpy(&controlPackage[3],&fileInfo.st_size,sizeof(fileInfo.st_size));

    printf("Control Package: %s \n", controlPackage);
    llwrite(fd,controlPackage,strlen(controlPackage));

    while( (bytes_read = read(fd_file,file_data,255-4)) != 0){
        /*building data package*/
        dataPackage[0] = APP_DATA;
        dataPackage[1] = sequenceNumber % 255;
        dataPackage[2] = bytes_read / 256;
        dataPackage[3] = bytes_read % 256;
        memcpy(&dataPackage[4],file_data,bytes_read);

        llwrite(fd,dataPackage,bytes_read+4);

        sequenceNumber++;
    }

    controlPackage[0] = APP_END;

    llwrite(fd,controlPackage,sizeof(fileInfo.st_size) + 5 + strlen(application.dataFileEmissor));


    return fd;

}


int appReceiver(int fd){
    printf("receiver");
    unsigned char package[255]="";
    int fd_file;
    int dataSize;
    while(TRUE){
        llread(fd,package);
         /* if(package[0]==APP_START){
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
         } */


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
    return 0;
}