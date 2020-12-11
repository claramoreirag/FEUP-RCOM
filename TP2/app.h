
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

#define SUCCESS 0
#define ERROR 1
#define FAIL 2
#define PORT 21

typedef struct{
    char user[MAX_SIZE];
    char password[MAX_SIZE];
    char host[MAX_SIZE];
    char path[MAX_SIZE];
    char filename[MAX_SIZE];
    int port_to_read;
}ftp_info;
