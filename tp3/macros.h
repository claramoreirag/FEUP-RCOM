#pragma once



#define FALSE 0
#define TRUE 1


#define FLAG 0x7e



//A
#define A_CMD_EMISSOR 0x03
#define A_RSP_RECETOR 0x03
#define A_RSP_EMISSOR 0x01
#define A_CMD_RECETOR 0x01

//C
#define SET 0x03
#define DISC 0x0b
#define UA 0x07
#define RR 0x05
#define REJ 0x01



//C for I frames
#define C_NS0 0x00
#define C_NS1 0x40

//for stuffing
#define BYTE_STUFF 0x20
#define ESC 0x7d


//open modes
#define TRANSMITTER 0
#define RECEIVER 1