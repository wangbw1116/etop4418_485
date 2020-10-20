#ifndef __RS485_H__
#define __RS485_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int openSerialPort(char *port);
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int sendData(int fd, char *data, int len);
int recvData(int fd, char *data, int len);

#endif // !__RS485_H__