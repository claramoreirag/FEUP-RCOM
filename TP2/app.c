#include "app.h"

/* Prints in the console the right way to use the application */
void print_usage(){
    printf("\nUsage: download ftp://[<user>:<password>@]<host>/<url-path>\n");
}



char* getHostIP(ftp_info* info){
    struct hostent *h;

    if ((h = gethostbyname(info->host)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }

    return inet_ntoa(*((struct in_addr *)h->h_addr_list[0]));
}



/* Makes the connection */
int connectTCP(ftp_info* info, int port){

    char *ip;
    ip = getHostIP(info);

    int sockfd;
    struct sockaddr_in server_addr;
    //int bytes;

    /*server address handling*/
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    /*open an TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket()");
        exit(0);
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("connect()");
        exit(0);
    }

    printf("> telnet %s %d\n", info->host, port);

    return sockfd;
    
}




/* Parses the URL and fills the ftp_info struct with the right information */
int parse_arg(char * arg, ftp_info * info){
   
    //when user and password are provided in the url
     if (sscanf(arg, "ftp://%[^:]%*[:]%[^@]%*[@]%[^/]%*[/]%s", info->user, info->password, info->host,info->path) == 4) {
     char *delimiter = strrchr(info->path,'/');
       if(delimiter==NULL) {
            strcpy(info->filename,info->path);
        }
        else{
           delimiter++; 
           strcpy(info->filename,delimiter);
        } 
     
    }

    //when user and password are not provided in the url
    else if(sscanf(arg, "ftp://%[^/]%*[/]%s", info->host, info->path) == 2){
        char *delimiter = strrchr(info->path,'/');
        strcpy(info->user, "anonymous");
        strcpy(info->password, "password");
        if(delimiter==NULL) {
            strcpy(info->filename,info->path);
        }
        else{
           delimiter++; 
           strcpy(info->filename,delimiter);
        } 
        
    }
    else{
        return -1;
    }
 
        printf("\nParsed url and authentication:\n\n");
        printf("User:      %s\n", info->user);
        printf("Pass:      %s\n", info->password);
        printf("Host:      %s\n", info->host);
        printf("Path:      %s\n", info->path);
        printf("Filename:  %s\n\n", info->filename);
    return 0;
}


/* reads the ftp reply and puts it in buf, returning the number of bytes read */
int FTP_reply(int fd, char* buf) {
    char byte;
    int nBytes = 0;
    bool isLastLine = false;
    while (read(fd, &byte, 1) >= 0) {
        buf[nBytes] = byte;

        //condition to check if we are in the last line
        if ((buf[nBytes - 4] == '\n' || nBytes <= 3) 
        && isdigit(buf[nBytes - 1]) 
        && isdigit(buf[nBytes - 2]) 
        && isdigit(buf[nBytes - 3]) 
        && buf[nBytes] != '-') 
            isLastLine = true;
        //if we just read the last byte, it exits the read loop
        if (isLastLine && byte == '\n') 
            break;
        nBytes++;
    } 

    //prints the reply received
    printf("< %s\n", buf);  
    
    return nBytes;
}

/* Sends a command to the server  */
int send_command(int fd, char* command){
    
    printf("> %s", command);
    int res = write(fd, command, strlen(command));
    
    if(res < 0){
        printf("\nError sending command: %s", command);
    }
    return res;
}

/* Does login */
int login(int fd, ftp_info* info){
    char user_command[MAX_SIZE+ 6];
    char user_reply[MAX_SIZE+1];


    //sends username
    sprintf(user_command, "user %s\n", info->user);
    if(send_command(fd, user_command) < 0)
        return 1;

    FTP_reply(fd, user_reply);

    //sends password
    char psw_command[MAX_SIZE+10];
    char psw_reply[MAX_SIZE+1];

    sprintf(psw_command,"pass %s\n", info->password);
    if(send_command(fd, psw_command) < 0)
        return 1;

    FTP_reply(fd, psw_reply);
   

    return 0;

}

int get_port(char * reply){
    int msb, lsb;
    //gets the port number from the ftp reply
    sscanf( reply, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%d,%d).", &msb, &lsb );

    return msb * 256 + lsb;
}


//reads the file 
int download_file(int fd, char* filename){
    FILE *file;
    file = fopen(filename, "wb+");
    char buffer[MAX_SIZE]="";
    int res;

    while((res = read(fd, buffer, MAX_SIZE)) > 0){
        fwrite(buffer, sizeof(char), res, file);
    }

    fclose(file);

    return 0;
}

//sends the command to read file
int retrieve_file(int fd1, int fd2, ftp_info* info){
    char command[MAX_SIZE+6];

    sprintf(command, "retr %s\n", info->path);
    if(send_command(fd1, command) < 0)
        return 1;


    char reply[MAX_SIZE+1];
    FTP_reply(fd1, reply);
   

    download_file(fd2, info->filename);

    return 0;
}



int main(int argc, char **argv)
{
    ftp_info info;
    

    if(argc!=2 || (parse_arg(argv[1],&info)==-1)){
        print_usage();
        return 1;
    }


    //Connection TCP to host
    int sockfd =connectTCP(&info,PORT);
    

    char reply[MAX_SIZE];

    FTP_reply(sockfd,reply);
  

    //Login in to host
    if (( login(sockfd, &info)) > 0) return 1;

    //Enter passive mode 
    if(send_command(sockfd, "pasv\n")<0)
        return 1;
    
    char passive_reply[MAX_SIZE]="";
    if(FTP_reply(sockfd,passive_reply)<0){
        printf("Error reading reply to passive command\n");
        return 1;
    }
    info.port_to_read=get_port(passive_reply);

    int sockfd2 =connectTCP(&info,info.port_to_read);


    if(retrieve_file(sockfd, sockfd2, &info)){
        printf("\nError retrieving file in path %s.\n", info.path);
        return 1;
    }

    if(send_command(sockfd, "quit\n") )
        return 1;

    close(sockfd);
    close(sockfd2);
    return 0;
}


