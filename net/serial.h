
#ifndef _LIB_SERIAL_H_
#define _LIB_SERIAL_H_

#define FALSE 1
#define TRUE 0

void set_speed(int fd, int speed);
int set_parity(int fd,int databits,int stopbits,int parity, int mode);

#endif

