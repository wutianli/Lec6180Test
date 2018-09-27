/****************************************************************************/
/*                          Lec3070 API                                     */
/*                          Copyright 2010-2015 Ls-China                    */
/****************************************************************************/
/*
**
** MODULE AUTHOR(S):    ZHANG Wei
**
** MODULE TITLE:        uart.c
**
** MODULE INPUTS:       None
**
** MODULE FUNCTION:  
**
**
** MODULE OUTPUTS:
**
** DOCUMENT REFERENCES:
**
******************************************************************************
**  MODIFICATION HISTORY:
**  
**  Date           Person    CR       Change
**  --------       -------   ------   ------------------------------------ 
**  6/29/2010      Zhang wei          Create
**
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>


/******************************************************************************
 *   Bundrate configure
 *
 ******************************************************************************/
#define FALSE 1
#define TRUE  0
#define  B0	 0000000
#define  B50	 0000001
#define  B75	 0000002
#define  B110	 0000003
#define  B134	 0000004
#define  B150	 0000005
#define  B200	 0000006
#define  B300	 0000007
#define  B230400 0010003
#define  B460800 0010004
#define  B500000 0010005
#define  B576000 0010006
#define  B921600 0010007

static int speed_arr[] = {B921600, B576000, B500000, B460800, B230400, B115200, B38400, 
                                 B19200, B9600, B4800, B2400, B1200, B600, B300, B200, B150, B134, B110, B75, B50};
static int name_arr[] = {921600, 576000, 500000, 460800, 230400, 115200, 38400,  
                           19200,  9600,  4800,  2400,  1200,  600, 300, 200,  150, 134, 110, 75, 50};

void baud_rate_help(int speed)
{
        int i = 0;
        
        printf("Not valid baudrate [%d]\n", speed);
        printf("You can use baudrate\n");
        
        for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++){
                printf(" %d,", name_arr[i]);
                if(i == 7)
                        printf("\n");
        }
        printf("\n");
        exit(-1);
}

void set_speed(int fd, int speed)
{
        int   i, status;
        struct termios   Opt;
        
        tcgetattr(fd, &Opt);
        for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++){
                if  (speed == name_arr[i]){
                        tcflush(fd, TCIOFLUSH);
                        cfsetispeed(&Opt, speed_arr[i]);
                        cfsetospeed(&Opt, speed_arr[i]);
                        status = tcsetattr(fd, TCSANOW, &Opt);
                        if  (status != 0)
                                perror("tcsetattr fd1");
                        return;
                }
                tcflush(fd,TCIOFLUSH);
        }
        baud_rate_help(speed);
}

/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄*
*@param  databits 类型  int 数据位   取值 为 7 或者8*
*@param  stopbits 类型  int 停止位   取值为 1 或者2*
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
int set_parity(int fd,int databits,int stopbits,int parity, int mode)
{
        struct termios options;
        
        if  ( tcgetattr( fd,&options)  !=  0){
                perror("SetupSerial 1");
                return(FALSE);
        }
        options.c_cflag &= ~CSIZE;
        switch (databits) /*设置数据位数*/{
        case 7:
                options.c_cflag |= CS7;
                break;
        case 8:
                options.c_cflag |= CS8;
                break;
        default:
                fprintf(stderr,"Unsupported data size\n");
                return (FALSE);
        }

        switch (parity){
  	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);  /* 设置为奇效验*/ 
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/  
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return (FALSE);
        }
/* 设置停止位*/   
        switch (stopbits){
        case 1:
                options.c_cflag &= ~CSTOPB;
                break;
        case 2:
                options.c_cflag |= CSTOPB;
                break;
        default:
                fprintf(stderr,"Unsupported stop bits\n");
                return (FALSE);
        }
        /* Set input parity option */
        if (parity != 'n')
                options.c_iflag |= INPCK;
        options.c_cc[VTIME] = 0; // 15 seconds
        options.c_cc[VMIN] = 0;
        if(1 == mode){
                options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
                options.c_oflag  &= ~OPOST;   /*Output*/
                // options.c_iflag  &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);/*fqh add*/
        }

        tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
        if (tcsetattr(fd,TCSANOW,&options) != 0){
                perror("SetupSerial 3");
                return (FALSE);
        }
        return (TRUE);
 }

/******************************************************************************
 *   Define Something
 *
 ******************************************************************************/
static const char *tty_head = "/dev/ttyF";
#define RECV_BUFFER 128
#define MAX_RECV_BUFFER 16
#define MAX_SYST_BUFFER 32
#define SIMPLE_COUNT_TIMEOUT  50000
#define SIMPLE_READ_TIMEOUT  64
#define COUNT_TIMEOUT  50000
#define READ_TIMEOUT  64
static int fd_tty1 = -1;
static int fd_tty2 = -1;

int  uart_open(const char *tty1, const char* tty2, int baudrate)
{
        if (-1 == (fd_tty1 = open(tty1, O_RDWR ))){
                printf("open %s Error\n", tty1);
                return -1;
        }
        if (-1 == (fd_tty2 = open(tty2, O_RDWR ))){
                printf("open %s Error\n", tty2);
                return -1;
        }
        set_speed(fd_tty1, baudrate);
        set_speed(fd_tty2, baudrate);
        if (set_parity(fd_tty1,8,1,'N', 1)== FALSE){
                printf("Set Parity Error\n");
                return -1;
        }
        if (set_parity(fd_tty2,8,1,'N', 1)== FALSE){
                printf("Set Parity Error\n");
                return -1;
        }
        return 0;
}

int  lec_single_test(char * argv[])
{
        char buf[MAX_RECV_BUFFER];
	int nread = -1;
        int timeout = SIMPLE_READ_TIMEOUT;
        int Baud_rate = 9600;
        char tty_name1[MAX_SYST_BUFFER];
        char tty_name2[MAX_SYST_BUFFER];
        
        Baud_rate = atoi(argv[2]);
        memset(tty_name1, 0, MAX_SYST_BUFFER);
        memset(tty_name2, 0, MAX_SYST_BUFFER);
        sprintf(tty_name1, "%s%s", tty_head, argv[3]);
        sprintf(tty_name2, "%s%s", tty_head, argv[4]);
        if(-1 == uart_open(tty_name1, tty_name2, Baud_rate))
                return -1;

        write(fd_tty1, "uart_test", 9);
        while(timeout--){
               memset(buf, 0, MAX_RECV_BUFFER);
                /* read from uart */
                nread = read(fd_tty2,buf,MAX_RECV_BUFFER);
                if(nread <= 0){
                        usleep(SIMPLE_COUNT_TIMEOUT);
                }
                else{
                        if(!strncmp(buf, "uart_test", 9)){
                                printf("%s -- [ok]\n", buf);
                                close(fd_tty1);
                                close(fd_tty2);
                                return 0;
                        }
                }
        }
        printf(" ----- [error]\n");
        close(fd_tty1);
        close(fd_tty2);
        return -1;
}

int  tty1_sent_to_tty2(int fd1, int fd2)
{
        char buf[MAX_RECV_BUFFER];
	int nread = -1;
        int timeout = 0;

        write(fd1, "uart_test", 9);
        while(1){
               memset(buf, 0, MAX_RECV_BUFFER);
                /* read from uart */
                nread = read(fd2,buf,MAX_RECV_BUFFER);
                timeout++;
                if(timeout > READ_TIMEOUT){
                        return 1;
                }
                if(nread <= 0){
                        usleep(COUNT_TIMEOUT);
                        continue;
                }
                if(!strncmp(buf, "uart_test", 9)){
                        return 0;
                }
        }
        return 1;
}

int  lec_multi_test(char *argv[])
{
        int Baud_rate = 0;
        char tty_name1[MAX_SYST_BUFFER];
        char tty_name2[MAX_SYST_BUFFER];
        char systembuf[RECV_BUFFER];
        char systembuf1[RECV_BUFFER];
	int ret = 0;
        int count = 0;
        int count1 = 0;
        int lost = 0;
        int lost1 = 0;
        int sum = 0;
        int sum1 = 0;
        int RETIME = 50000;
        
        Baud_rate = atoi(argv[2]);
        memset(tty_name1, 0, MAX_SYST_BUFFER);
        memset(tty_name2, 0, MAX_SYST_BUFFER);
        sprintf(tty_name1, "%s%s", tty_head, argv[3]);
        sprintf(tty_name2, "%s%s", tty_head, argv[4]);
        if(-1 == uart_open(tty_name1, tty_name2, Baud_rate))
                return -1;
        
        while(1){
                ret = tty1_sent_to_tty2(fd_tty1, fd_tty2);
                if(0 == ret){
                        count++;
                        if(count == RETIME){sum++; count = 0;}
                }
                else
                        lost++;
                usleep(100000);
                ret = tty1_sent_to_tty2(fd_tty2, fd_tty1);
                if(0 == ret){
                        count1++;
                        if(count1 == RETIME){sum1++; count1 = 0;}
                }
                else
                        lost1++;

                memset(systembuf, 0, RECV_BUFFER);
                sprintf(systembuf, "echo %s Receive[%d + %d*%d]times, lost[%d] > /tmp/log_%s", 
                        tty_name2, count, sum, RETIME, lost, tty_name2+5);
                system(systembuf);
                memset(systembuf1, 0, RECV_BUFFER);
                sprintf(systembuf1, "echo %s Receive[%d + %d*%d]times, lost[%d] > /tmp/log_%s", 
                        tty_name1, count1, sum1, RETIME, lost1, tty_name1+5);
                system(systembuf1);
        }
        return 0;
}

void pthread_write_test(int *interval)
{
        char buf[RECV_BUFFER];
        int timeout = *interval*1000;
        int count = 0;
        printf("timeout=%d\n", timeout);
        sleep(1);
        while(1){
                memset(buf, 0, RECV_BUFFER);
                sprintf(buf, "%d:1234567890abcdefghijklmn", count);
                write(fd_tty1, buf, strlen(buf));
                usleep(timeout);
                count++;
        }
        pthread_exit(0);
}


//uart -b 115200 -t 200 2 1
int  lec_limits_test(char * argv[])
{
        char buf[RECV_BUFFER];
	int nread = -1;
        int time_interval = 0;
        int Baud_rate = 0;
        char tty_name1[MAX_SYST_BUFFER];
        char tty_name2[MAX_SYST_BUFFER];
        
        int ret = 0;
        pthread_t pid_write;
        pthread_attr_t attr = {};
        pthread_attr_init (&attr);
        pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

        Baud_rate = atoi(argv[2]);
        time_interval = atoi(argv[4]);
        printf("Baud_rate=%d\n", Baud_rate);
        printf("time_interval=%d\n", time_interval);
        memset(tty_name1, 0, MAX_SYST_BUFFER);
        memset(tty_name2, 0, MAX_SYST_BUFFER);
        sprintf(tty_name1, "%s%s", tty_head, argv[5]);
        sprintf(tty_name2, "%s%s", tty_head, argv[6]);
        if(-1 == uart_open(tty_name1, tty_name2, Baud_rate))
                return -1;
        
        ret = pthread_create(&pid_write, &attr,(void *)pthread_write_test,(void *)&time_interval);
        while(1){
               memset(buf, 0, RECV_BUFFER);
                nread = read(fd_tty2,buf,RECV_BUFFER);
                if(nread <= 0){
                        usleep(5);
                }
                else{
                        printf("[%s]\n", buf);
                }
        }
        close(fd_tty1);
        close(fd_tty2);
        return 0;
}

int  lec_read(char * argv[])
{
        char buf[RECV_BUFFER];
	int nread = -1;
	int i = 0;
        int Baud_rate = 9600;
        char tty_name[MAX_SYST_BUFFER];
        
        Baud_rate = atoi(argv[2]);
        memset(tty_name, 0, MAX_SYST_BUFFER);
        sprintf(tty_name, "%s%s", tty_head, argv[3]);

        printf("set %s[%d] ........\nBegin recv ..\n", tty_name, Baud_rate);

        if (-1 == (fd_tty1 = open(tty_name, O_RDWR ))){
                return -1;
        }
        set_speed(fd_tty1, Baud_rate);
        if (set_parity(fd_tty1,8,1,'N', 1)== FALSE){
                printf("Set Parity Error\n");
                return -1;
        }
      
        while(1){
               memset(buf, 0, RECV_BUFFER);
                /* read from uart */
                nread = read(fd_tty1,buf,RECV_BUFFER);
                if(nread <= 0){
                        usleep(5);
                }
                else{
                    for(i = 0; i < nread; i++)
                        printf("0x%x\n", buf[i]);
                }
        }
        printf(" ----- [error]\n");
        close(fd_tty1);
        return -1;
}

int  lec_write(char * argv[])
{
        char wbuf[MAX_RECV_BUFFER];
	int nwrite = -1;
        int Baud_rate = 9600;
        char tty_name[MAX_SYST_BUFFER];
        
        Baud_rate = atoi(argv[2]);
        memset(tty_name, 0, MAX_SYST_BUFFER);
        sprintf(tty_name, "%s%s", tty_head, argv[3]);

        printf("set %s [%d] \nWriting '0x1/0x2/0x3/0x4/0x5/0x5/0xff'\n", tty_name, Baud_rate);

        if (-1 == (fd_tty1 = open(tty_name, O_RDWR ))){
                return -1;
        }
        set_speed(fd_tty1, Baud_rate);
        if (set_parity(fd_tty1,8,1,'N', 1)== FALSE){
                printf("Set Parity Error\n");
                return -1;
        }
        memset(wbuf, 0, MAX_RECV_BUFFER);
        wbuf[0] = 0x1;
        wbuf[1] = 0x2;
        wbuf[2] = 0x3;
        wbuf[3] = 0x4;
        wbuf[4] = 0x5;
        wbuf[5] = 0x6;
        wbuf[6] = 0xFF;
        nwrite = write(fd_tty1, wbuf, 7);
        printf("Write successful %d bytes\n", nwrite);
        close(fd_tty1);
        return 0;
}

void argv_help(const char *buf)
{
        printf("Uart Tool v2.0 (2012-12-06) binary\n");
        printf("Usage:\n");
        printf("    %s -mt 115200 1 2        -- Multi  Test ttyS1 <=> ttyS2\n", buf);
        printf("    %s -st 115200 1 2        -- Single Test ttyS1  => ttyS2\n", buf);
        printf("    %s -lt 115200 -t 200 1 2 -- Limits Test ttyS1  => ttyS2\n", buf);
        printf("    %s -r  115200 1          -- Read From ttyS1\n", buf);
        printf("    %s -w  115200 1          -- Write To  ttyS1\n\n", buf);
}

int main(int argc, char * argv[])
{
        if(argc == 4){
                if(!strcmp(argv[1], "-r"))
                        lec_read(argv);
                if(!strcmp(argv[1], "-w"))
                        lec_write(argv);
                else
                        argv_help(argv[0]);
        }
        else if(argc == 5){
                if(!strcmp(argv[1], "-mt"))
                        lec_multi_test(argv);
                else if(!strcmp(argv[1], "-st")){
                        if(0 == lec_single_test(argv)) 
                                return 0;
                }
                else
                        argv_help(argv[0]);
        }
        else if(argc == 7){
                lec_limits_test(argv);
        }
        else{
                argv_help(argv[0]);
        }
        return 1;
}
