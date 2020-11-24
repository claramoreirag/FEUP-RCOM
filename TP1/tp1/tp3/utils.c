#include "utils.h"

char calculateBCC2(char *data, int size)
{
  char bcc = data[0];

  for (int i = 1; i < size; i++)
  {
    bcc = bcc ^ data[i];
  }

  return bcc;
}

int send_ua(int fd, char flag)
{
  unsigned char buf[5];
  buf[0] = FLAG;
  if (flag == TRANSMITTER)
    buf[1] = A_RSP_EMISSOR;
  else if (flag == RECEIVER)
    buf[1] = A_RSP_RECETOR;
  else
    return -1;
  buf[2] = UA;
  buf[3] = A_RSP_RECETOR ^ UA;
  buf[4] = FLAG;
  write(fd, buf, 5);

  printf("UA sent: %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

  return 0;
}

void send_set(int fd)
{
  unsigned char buf[5];
  buf[0] = FLAG;
  buf[1] = A_CMD_EMISSOR;
  buf[2] = SET;
  buf[3] = A_CMD_EMISSOR ^ SET;
  buf[4] = FLAG;
  write(fd, buf, 5);
  printf("SET sent %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
}

void send_rej(int fd, int ns)
{
  unsigned char buf[5];
  unsigned char control;
  buf[0] = FLAG;
  buf[1] = A_RSP_RECETOR;
  if (ns == 0)
    control = REJ;
  else
    control = 0x80 | REJ;
  buf[2] = control;
  buf[3] = A_RSP_RECETOR ^ control;
  buf[4] = FLAG;
  write(fd, buf, 5);
  printf("REJ sent %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
}

void send_rr(int fd, int ns)
{

  unsigned char buf[5];
  unsigned char control;
  buf[0] = FLAG;
  buf[1] = A_RSP_RECETOR;
  if (ns == 0)
    control = RR;
  else
    control = 0x80 | RR;
  buf[2] = control;
  buf[3] = A_RSP_RECETOR ^ control;
  buf[4] = FLAG;
  write(fd, buf, 5);
  printf("RR sent %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);
}

int byte_stuffing(char *buf, char *res, int size)
{

  int j = 0;
  for (int i = 0; i < size; i++)
  {
    if (buf[i] == FLAG)
    {
      res[j] = ESC;
      res[++j] = FLAG ^ BYTE_STUFF;
    }
    else if (buf[i] == ESC)
    {
      res[j] = ESC;
      res[++j] = ESC ^ BYTE_STUFF;
    }
    else
    {
      res[j] = buf[i];
    }
    j++;
  }

  return j;
}

int byte_destuffing(char *buf, char *res, int size)
{

  int j = 0;
  for (int i = 0; i < size - 1; i++)
  {
    if (buf[i] == ESC && buf[i + 1] == (FLAG ^ BYTE_STUFF))
    {
      i++;
      res[j] = FLAG;
    }
    else if (buf[i] == ESC && buf[i + 1] == (ESC ^ BYTE_STUFF))
    {
      i++;
      res[j] = ESC;
    }
    else
      res[j] = buf[i];

    j++;
  }

  res[j++] = buf[size - 1];

  return j;
}

int createInfoFrame(char *buf, int s, char *frame, int size)
{
  char res[255] = "";
  int resSize = byte_stuffing(buf, res, size); // buffer size
  frame[0] = FLAG;
  frame[1] = A_CMD_EMISSOR;
  if (s == 0)
    frame[2] = 0x00;
  else
    frame[2] = 0x40;
  frame[3] = frame[2] ^ frame[1];
  for (int i = 0; i < resSize; i++)
  {
    frame[i + 4] = res[i];
  }
  char bcc = calculateBCC2(buf, size);
  frame[4 + resSize] = bcc;
  frame[5 + resSize] = FLAG;
  
  return resSize + 6;
}

int check_destuffing(char *buf, char bcc2, int size)
{

  char cmp = buf[0];
  for (int i = 1; i < size-1; i++)
  {
    cmp = cmp ^ buf[i];
  }
 
  return cmp == bcc2;
}

int send_disc(int fd, char flag)
{

  unsigned char buf[5];
  buf[0] = FLAG;
  if (flag == TRANSMITTER)
    buf[1] = A_CMD_EMISSOR;
  else if (flag == RECEIVER)
    buf[1] = A_CMD_RECETOR;
  else
    return -1;
  buf[2] = DISC;
  buf[3] = buf[1] ^ buf[2];
  buf[4] = FLAG;

  write(fd, buf, 5);

  printf("DISC sent: %x %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

  return 0;
}

void print_hex(char *string, int size)
{
  printf("info: ");
  for (int i = 0; i < size; i++)
  {

    printf("%02x ", string[i]);
  }
  printf("\n");
}