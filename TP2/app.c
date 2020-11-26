#include "app.h"


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




int connectTCP(ftp_info* info, int port){

    char *ip;
    ip = getHostIP(info);

    int sockfd;
    struct sockaddr_in server_addr;
    int bytes;

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


int parse_arg(char * arg, ftp_info * info){
   // if(strncmp(arg, "ftp://",6)!=0)return -1;
    printf("enter func\n");

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
    else if(sscanf(arg, "ftp://%[^/]%*[/]%s", info->host, info->path) == 2){
        char *delimiter = strrchr(info->path,'/');
        if(delimiter==NULL) {
            strcpy(info->filename,info->path);
        }
        else{
           delimiter++; 
           strcpy(info->filename,delimiter);
        } 
        
    }
    else{
        //TODO Error
        return -1;
    }
 
        printf("\n============ Parsed url and authentication ============\n\n");
        printf("User:      %s\n", info->user);
        printf("Pass:      %s\n", info->password);
        printf("Host:      %s\n", info->host);
        printf("Path:      %s\n", info->path);
        printf("Filename:  %s\n", info->filename);
    return 0;
}

int main(int argc, char **argv)
{
    ftp_info info;
    

    if(argc!=2 || (parse_arg(argv[1],&info)==-1)){
        print_usage();
        return 1;
    }

    int sockfd =connectTCP(&info,21);
    

    return 0;
}


