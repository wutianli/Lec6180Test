#ifndef MR_UARTLIB_H_
#define MR_UARTLIB_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#include <signal.h>

int ComOpen(unsigned char *port);
int ComSetBaud(int fd, int baud);
int ComSetParity(int fd, int databits, int stopbits, char parity);
int uart_init(char *dev, int baudrate, int databits, int stopbits, char parity);


#endif



