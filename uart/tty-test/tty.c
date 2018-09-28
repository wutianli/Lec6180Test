#include "uartlib.h"


int main(int argc, char *argv[])
{
	char *dev = "/dev/ttyS5";
	
	int fd;
	
	char buf[8] = "abcdefg\0";
	char r_buf[8] = "\0";
	
	fd = uart_init(dev, 115200, 8, 1, 'n');
	
	if(fd > 0)
		printf("init tty ok\n");
	
	writre(fd, buf, sizeof(buf));
	
	read(fd, r_buf, sizeof(r_buf));
	
	if(memcmp(buf, r_buf, sizeof(buf)) == 0)
		printf("uart test OK\n");
	else
		printf("uart test FAIL\n");
	
	return 0;
}