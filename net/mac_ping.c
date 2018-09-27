/****************************************************************************/
/*                          LEC-3170 API                                     */
/*                          Copyright 2015-2020 Ls-China                    */
/****************************************************************************/
/*
**
** MODULE AUTHOR(S):    CHEN Pengpeng
**
** MODULE TITLE:        mac_ping.c
**
** MODULE INPUTS:       None
**
** MODULE FUNCTION:  
**
** MODULE OUTPUTS:
**
** DOCUMENT REFERENCES:
**
******************************************************************************
**  MODIFICATION HISTORY:
**  
**  Date           Person    CR       Change     Email
**  --------       -------   ------   -------    -------------------------- 
**  3/25/2015      CHEN Pengpeng      Create     cp_china@163.com
**
*****************************************************************************/
#include <stdio.h>
#include <sys/types.h>  
#include <stdlib.h>        	//perror
#include <string.h>        	//strcpy,memset
#include <sys/socket.h>        	//socket
#include <sys/ioctl.h>        	//ioctl
#include <net/if.h>        	//ifreq
#include <linux/if_packet.h>    //sockaddr_sll
#include <linux/if_ether.h>     //ETH_P_ALL
 
#include <arpa/inet.h>  
#include <features.h>           /* for the glibc version number */   
//#include <netpacket/packet.h>  
#include <net/ethernet.h>       /* the L2 protocols */  
#include <asm/types.h>  
//#include <linux/if_ether.h>   /* The L2 protocols */   
#include <netinet/in.h>   
#include <errno.h> 
#include <pthread.h>

#include <fcntl.h>
#include <unistd.h>
#include "libdev.h"
#include "libgpio.h"



//#define IFRNAME0 "eth0"
//#define IFRNAME1 "eth1"
 
#define RECV_BUFFER 128

#define BUF_SIZE 1514
unsigned char recvbuf[BUF_SIZE];
unsigned char sendbuf[BUF_SIZE];

#define RECV_TIMEOUT  1
#define COUNT_TIMEOUT 500000
#define BUFSIZE 128  //128
//unsigned char buf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
static unsigned char buf[BUFSIZE] = {0x00, 0x90, 0x0B, 0x22, 0xA0, 0xEB, 0x00, 0x90, 0x0B, 0x22, 0xA0, 0xEA, 0x41, 0x42, 0x41, 0x42, 0x43, 0x30, 0x31, 0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A};
  
unsigned char source_mac[6];
unsigned char dest_mac[6]; 

unsigned int delayms = 1000;
unsigned int length = 128;

unsigned char recvflag = 0;
unsigned char sendflag = 0;
int times = 32;
unsigned long tt = 0;

/** 
 * get_hwaddr - get netdevice mac addr  
 * @name: device name, e.g: eth0 
 * @hwaddr: where to save mac, 6 byte hwaddr[6] 
 * @return: 0 on success, -1 on failure 
 */ 
int get_hwaddr(const unsigned char *name, unsigned char *hwaddr)  
{  
    struct ifreq ifr;  
    unsigned char memzero[6];  
    int sock;  
  
    if(name == NULL || hwaddr == NULL){  
        printf("get_hwaddr: NULL para\n");  
        return -1;  
    }  
  
    sock = socket(AF_INET, SOCK_STREAM, 0);  
    if(sock < 0){  
        printf("get_hwaddr: socket error\n");  
        return -1;  
    }  
  
    //get eth1 mac addr  
    memset(hwaddr, 0, 6);  
    memset(&ifr, 0, sizeof(ifr));  
    strncpy(ifr.ifr_name, name, 6);  
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0){  
            perror("get_hwaddr ioctl:");  
            close(sock);  
            return -1;  
    } else {  
            memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, 6);  
            //printf("hwaddr:%2x:%2x:%2x:%2x:%2x:%2x\n", hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);  
    }  
    memset(memzero, 0, 6);  
    if(memcmp(memzero, hwaddr, 6) == 0){  
        printf("no mac\n");  
	close(sock);
        return -1;  
    }  
  
    close(sock);  
    return 0;  
} 
/** 
 * printhex - print the netdevice mac addr  
 * @hex:
 * @len:
 * @tag:
 * @return: NO 
 */
void printhex(void *hex, int len, char *tag)  
{  
    int i;   
    unsigned char *p = (unsigned char *)hex;  
  
    if(len < 1)  
        return;  
  
    for(i = 0; i < len - 1; i++){  
        if(*p < 0x10)  
            printf("0%x%s", *p++, tag);  
        else  
            printf("%2x%s", *p++, tag);  
    }  
  
    if(*p < 0x10)  
        printf("0%x", *p++);  
    else  
        printf("%2x", *p++);  

}  

/** 
 * mac_recv - mac recv test  
 * @devname: recv form device name, e.g: eth0
 * @return: number of recv data 
 */
int mac_recv(const unsigned char *devname)
{
	int i, sfd, len;
	struct sockaddr_ll sll;
	struct ifreq ifr;

	if ((sfd=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
		perror("socket");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	//strcpy(ifr.ifr_name, IFRNAME1);
	strcpy(ifr.ifr_name, devname);	
	if ((ioctl(sfd, SIOCGIFINDEX, &ifr)) == -1) {
		//strcpy(ifr.ifr_name, IFRNAME0);
		//if ((ioctl(sfd, SIOCGIFINDEX, &ifr)) == -1) {	
			perror("ioctl 1");
			close(sfd);
			return -1;
		//}
	}

	memset(&sll, 0, sizeof(sll));
	sll.sll_family = PF_PACKET;
	sll.sll_ifindex =  ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);
	int length_sll = sizeof(sll);
	if ((bind(sfd, (struct sockaddr *)&sll, sizeof(sll))) == -1) {
		perror("bind");
		close(sfd);
		return -1;
	}
	//printf("111\n");
	//
	struct timeval timeout = {1, 0};//
	//int timeout = 10000;//10s
	setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	while (1) {
		len = recvfrom(sfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&sll, &length_sll);
		//printf("[%s]recv data: [%d].\n", devname, len);	
		if (len == length ) {
			//printf("[%s]recv data: [%d]\n", devname, len);
			//printf("[%s] received %d bytes: seq=%d\n", devname, len, tt);
			if(0 == memcmp(sendbuf, recvbuf, length)){
/*
				for(i=12; i<len; i++)
					//printf("%x", recvbuf[i]);
					printf("%c", recvbuf[i]);
				printf("\n");
*/				
				close(sfd);
				recvflag = 1;
				//break;
				return len;
			}
			else{
				printf("[%s] recvbuf data error!\n", devname);
				close(sfd);
				return -1;	
			}		
		}
		else if(/*(len == 60) || (len == 42)*/ len > 0){
			continue;
		}
		else{
			printf("[%s] recv timeout error![%d bytes]\n", devname, len);
/*			
			i++;
			if(i > 9){
				printf("[%s] recv timeout error!\n", devname);
				close(sfd);
				return -1;
			}
*/			
			close(sfd);
			return -1;
		}
	}
	//printf("222\n");
	//close(sfd);
	//return len;
}

/** 
 * mac_send - mac send test  
 * @devname1: source device name, e.g: eth0
 * @devname2: destination device name, e.g: eth1
 * @return: 
 */
//send.c
/*
void cleanup(void *arg)
{
	int sfd = *((int *)arg);
	close(sfd);	
}
*/
int mac_send(const unsigned char *devname1, const unsigned char *devname2)
{
	int i, sfd/*, len*/;
    	struct sockaddr_ll sll;
    	struct ifreq ifr;
	
	int *sfd_fp = malloc(sizeof(int)); 

	if(strcmp(devname1, devname2) < 0){
		for(i=0; i<6; i++)
		{
			buf[i] = dest_mac[i];	
		}
		for(i=0; i<6; i++)
		{
			buf[i+6] = source_mac[i];
		}
		
	}
	else if(strcmp(devname1, devname2) > 0){
		for(i=0; i<6; i++)
		{
			buf[i] = source_mac[i];	
		}
		for(i=0; i<6; i++)
		{
			buf[i+6] = dest_mac[i];
		}
	}

	if(buf[5] < buf[11]){
		buf[12] = 0x42;
		buf[13] = 0x41;
	}
	else{
		buf[12] = 0x41;
		buf[13] = 0x42;
	}

	if ((sfd=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        	perror("socket");
        	return -1;
    	}
/*
	if ((*sfd_fp=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))== -1)) {
        	perror("socket");
        	return -1;
    	}	
	pthread_cleanup_push(cleanup, sfd_fp);	
*/	
    	memset(&ifr, 0, sizeof(ifr));
    	//strcpy(ifr.ifr_name, IFRNAME0);
	strcpy(ifr.ifr_name, devname1);
    	if ((ioctl(sfd, SIOCGIFINDEX, &ifr)) == -1) {
        	//strcpy(ifr.ifr_name, IFRNAME1);
        	//if ((ioctl(sfd, SIOCGIFINDEX, &ifr)) == -1) {           
            		perror("ioctl 1");
            		close(sfd);
            		return -1;
        	//}
    	}
        
    	memset(&sll, 0, sizeof(sll));
	sll.sll_family = PF_PACKET;
    	sll.sll_ifindex =  ifr.ifr_ifindex;
    	sll.sll_protocol = htons(ETH_P_ALL);
 
    	if ((bind(sfd, (struct sockaddr *)&sll, sizeof(sll))) == -1) {
        	perror("bind");
        	close(sfd);
        	return -1;
    	}

    	//while (1) {
		memset(sendbuf, 0, length);
		memcpy(sendbuf, buf, length);
        	//len = sizeof(sendbuf);
        	sendto(sfd, sendbuf, length, 0, (struct sockaddr *)&sll, sizeof(sll));
        	//printf("[%s]send data: [%d]\n", devname1, length);
		//printf("[%s] transmitted %d bytes: seq=%d\n", devname1, length, tt);
/*
		for(i=0; i<length; i++)
            		printf("%x", buf[i]);
        	printf("\n");
*/
		//usleep(100*delayms);
/*
		if(recvflag == 1){
			close(sfd); 
			//sendflag = 1;
			break;
			//return 0;
		}
*/
    	//}
	close(sfd); 
	return 0;
}

struct mypara
{
	unsigned char *devname1;
	unsigned char *devname2;
};

void pthread_send_test(void *arg)
{
	//pthread_detach(pthread_self());
	unsigned char *dev1 = NULL;
	unsigned char *dev2 = NULL;
	struct mypara *send_para;
	send_para = (struct mypara *)arg;
	dev1 = (*send_para).devname1;
	dev2 = (*send_para).devname2;

        sleep(1);
	mac_send(dev1, dev2);
/*
	if(0 == mac_send(dev1, dev2)){
		//End this pthread
		pthread_kill(pthread_self(), 0);	
	}
*/  
	pthread_exit(0);
}

int mac1_sent_to_mac2(const unsigned char *devname1, const unsigned char *devname2)
{
        //char buf[BUFSIZE];
	int nrecv = -1;
        int timeout = 0;
	int time_interval = 1000;
	
	int ret = 0;
	pthread_t pid_send;
        pthread_attr_t attr = {};
        pthread_attr_init (&attr);
        pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	struct mypara para;
	para.devname1 = devname1;
	para.devname2 = devname2;
	
	ret = pthread_create(&pid_send, &attr, (void *)pthread_send_test, (void *)&para);
	//pthread_detach(pid_send);
	usleep(100*delayms);

        while(1){
		//printf("333\n");
                //memset(buf, 0, BUFSIZE);

                /* recv from devname2 */
		nrecv = mac_recv(devname2);
		//printf("nrecv = [%d].\n", nrecv);
                timeout++;
		//printf("timeout = [%d].\n", timeout);
                if(timeout > RECV_TIMEOUT){
                        return 1;
                }
                if(nrecv <= 0){
                        usleep(COUNT_TIMEOUT); // delay = 50ms
                        continue;
                }
                if(recvflag == 1){
			recvflag = 0;
			//sendflag = 0; // clear flag
			//pthread_cancel(pid_send);

			//printf("444\n");
                        return 0;
                }
        }
        return 1;
}



void beep_runled(void){
	/*
	lec_gpio_init(0);
	lec_set_gpio_output(GPIO_SET_RUN, 1);
        //lec_set_beep(1);
        usleep(100000);
        lec_set_gpio_output(GPIO_SET_RUN, 0);
        //lec_set_beep(0);
	*/
	system("buzzer 1");
	return;
}

void runled_set(int i){
	//lec_gpio_init(0);
	//lec_set_gpio_output(GPIO_SET_RUN, i);
        //lec_set_beep(1);
        //usleep(100000);
        //lec_set_gpio_output(GPIO_SET_RUN, 0);
        //lec_set_beep(0);
	return;
}


int  lec_multi_test(const unsigned char *devname1, const unsigned char *devname2)
{
	unsigned char systembuf[RECV_BUFFER];
        unsigned char systembuf1[RECV_BUFFER];
	int ret = 0;
        int count = 0;
        int count1 = 0;
        int lost = 0;
        int lost1 = 0;
        int sum = 0;
        int sum1 = 0;
        int RETIME = 10;
	int Rtotal = 0;
	int Total = 0;
	int Rtotal1 = 0;
	int Total1 = 0;
	int Percent = 0;
	int Percent1 = 0; 
	
	int i = 0;
	//printf("times = %d\n",times);
	if(times < 0){	
		while(1){
			//printf("aabbccdd\n");
			tt++;
                	ret = mac1_sent_to_mac2(devname1, devname2);
			//printf("ret1 = %d\n",ret);
                	if(0 == ret){
                        	count++;
                        	if(count == RETIME){sum++; count = 0;}
				//printf("count = %d\n",count);
                	}
                	else{
                        	lost++;
				//printf("lost = %d\n",lost);
			}
                	//usleep(100000);//delay = 100ms
			//beep_runled();		

                	ret = mac1_sent_to_mac2(devname2, devname1);
			//printf("ret2 = %d\n",ret);
                	if(0 == ret){
                        	count1++;
                        	if(count1 == RETIME){sum1++; count1 = 0;}
				//printf("count1 = %d\n",count1);
                	}
                	else{
                        	lost1++;
				//printf("lost1 = %d\n",lost1);
			}
			//beep_runled();
			
			if((lost > 10) || (lost1 > 10)){
				runled_set(1);
				break;
			}
        	}
	}
	else{
		//printf("aabbccee\n");
		i = times;
		while(1){
			tt++;
                	ret = mac1_sent_to_mac2(devname1, devname2);
                	if(0 == ret){
                        	count++;
                        	if(count == RETIME){sum++; count = 0;}
                	}
                	else{
                        	lost++;
			}
                	//usleep(100000);//delay = 100ms
			//beep_runled();

                	ret = mac1_sent_to_mac2(devname2, devname1);
                	if(0 == ret){
                        	count1++;
                        	if(count1 == RETIME){sum1++; count1 = 0;}
                	}
                	else{
                        	lost1++;
			}
			//beep_runled();

			if((lost > 10) || (lost1 > 10)){
				runled_set(1);
				break;
			}
	            Rtotal = count+sum*RETIME;
            	Rtotal1 = count1+sum1*RETIME;
            	Total = Rtotal + lost;
            	Total1 = Rtotal1 + lost1;
            	Percent = 100* lost/Total;
            	Percent1 = 100* lost1/Total1;
            	
            	memset(systembuf, 0, BUFSIZE);
            	sprintf(systembuf, "echo [ %s ] PacketSize[ %d ]Bytes, %d packets transmitted, %d packets received, %d %% packet loss. > /tmp/log_eth", 
            			devname2, length, Total, Rtotal, Percent, devname2);
                    system(systembuf);
            
                    memset(systembuf1, 0, BUFSIZE);
            	sprintf(systembuf1, "echo [ %s ] PacketSize[ %d ]Bytes, %d packets transmitted, %d packets received, %d %% packet loss. >> /tmp/log_eth", 
            			devname1, length, Total1, Rtotal1, Percent1, devname1);
                    system(systembuf1);	
            	
            	//printf("[ %s ] PacketSize[ %d ]Bytes, %d packets transmitted, %d packets received, %d %% packet loss.\n", 
            	//		devname2, length, Total, Rtotal, Percent);
            	//printf("[ %s ] PacketSize[ %d ]Bytes, %d packets transmitted, %d packets received, %d %% packet loss.\n", 
            	//		devname1, length, Total1, Rtotal1, Percent1);
			
        	}
	}

        return 0;
}

void macPingprint(const unsigned char *devname1, const unsigned char *devname2)
{
	int i;
	unsigned char hwaddr0[6];  
	unsigned char hwaddr1[6];
	get_hwaddr(devname1, hwaddr0);
    	get_hwaddr(devname2, hwaddr1);

	for(i=0; i<6; i++)
	{
		dest_mac[i] = hwaddr1[i];	
	}
	for(i=0; i<6; i++)
	{
		source_mac[i] = hwaddr0[i];
	}

	printf("MacPing ");
	printf("%s(", devname1);
	printhex(hwaddr0, 6, ":");
	printf(") <=> %s(", devname2);
	printhex(hwaddr1, 6, ":");
	printf("): %d data bytes\n", length);
}

void argv_help(const char *buf)
{
        printf("MacPing Tool v1.0 (2015-03-25) binary\n");
        printf("Usage:\n");
	printf("    %s  -recv      0            -- recv data from eth0.\n", buf);
	printf("    %s  -recv      1            -- recv data from eth1.\n", buf);
	printf("    %s  -send      0 1          -- send 128 Bytes data from eth0 to eth1.\n", buf);
	printf("    %s  -send      1 0          -- send 128 Bytes data from eth1 to eth0.\n", buf);
	printf("    %s  -mt        0 1          -- Multi  Test eth0 <=> eth1 with 32 TIMEs and 128 Bytes.\n", buf);
	printf("    %s  -mt_ti     0 1 32       -- Multi  Test eth0 <=> eth1 and TIMEs = 32[-1 ~ 1000].\n", buf);
	printf("    %s  -mt_len    0 1 128      -- Multi  Test eth0 <=> eth1 and DataLenght = 128 Bytes[60 ~ 1514].\n", buf);
	printf("    %s  -mt_tilen  0 1 32 128   -- Multi  Test eth0 <=> eth1 and TIMEs = 32, DataLenght = 128 Bytes.\n", buf);
}

/**
	cmd: lsof -p $PID | w -l //look numbers of sfd!
*/
int main(int argc, char *argv[])
{
/*
	unsigned char systembuf[RECV_BUFFER];
	int ulimit = 1000000;	

	memset(systembuf, 0, BUFSIZE);
	sprintf(systembuf, "ulimit -n %d", ulimit);
	//"ulimit -n 1000000"
	system(systembuf);
	usleep(100000);
*/
    	//1.
	//macPingprint("eth0","eth1");
	
	//2.
	//runled_set(1);

	if(argc == 3){
		if(!strcmp(argv[1], "-recv")){
			if(atoi(argv[2]) == 0){
				//mac_recv("eth0");
			}
			else if(atoi(argv[2]) == 1){
				//mac_recv("eth1");
			}
			else{
				argv_help(argv[0]);
			}	
		}
            	else{
			argv_help(argv[0]);
	    	}
    	}
    	else if(argc == 4){
	    	if(!strcmp(argv[1], "-send")){			
			if((atoi(argv[2]) == 0) && (atoi(argv[3]) == 1)){
				//macPingprint("eth0","eth1");
				mac_send("eth0","eth1");
			}
			else if((atoi(argv[2]) == 1) && (atoi(argv[3]) == 0)){
				//macPingprint("eth0","eth1");
				mac_send("eth1","eth0");
			}
			else{
				argv_help(argv[0]);
			}	
	    	}
		else if(!strcmp(argv[1], "-mt")){
			if((atoi(argv[2]) == 0) && (atoi(argv[3]) == 1)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth0","eth1");
			}
			else if((atoi(argv[2]) == 1) && (atoi(argv[3]) == 0)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth1","eth0");
			}
			else{
				argv_help(argv[0]);
			}
		}
            	else{
			argv_help(argv[0]);	
            	}
    	}
	else if(argc == 5){
		if(!strcmp(argv[1], "-mt_ti")){
			if(atoi(argv[4]) < 0x4F1A00){ //time = 24hours
				times = atoi(argv[4]);
			}
			if((atoi(argv[2]) == 0) && (atoi(argv[3]) == 1)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth0","eth1");
			}
			else if((atoi(argv[2]) == 1) && (atoi(argv[3]) == 0)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth1","eth0");
			}
			else{
				argv_help(argv[0]);
			}
		}
		else if(!strcmp(argv[1], "-mt_len")){
			if((atoi(argv[4]) < 1515) && (atoi(argv[4]) > 60)){
				length = atoi(argv[4]);
			}
			if((atoi(argv[2]) == 0) && (atoi(argv[3]) == 1)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth0","eth1");
			}
			else if((atoi(argv[2]) == 1) && (atoi(argv[3]) == 0)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth1","eth0");
			}
			else{
				argv_help(argv[0]);
			}
		}
		else{
			argv_help(argv[0]);	
            	}
	}
	else if(argc == 6){
		if(!strcmp(argv[1], "-mt_tilen")){
			if(atoi(argv[4]) < 0x4F1A00){//time = 24hours
				times = atoi(argv[4]);
			}
			if((atoi(argv[5]) < 1515) && (atoi(argv[5]) > 60)){
				length = atoi(argv[5]);
			}
			if((atoi(argv[2]) == 0) && (atoi(argv[3]) == 1)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth0","eth1");
			}
			else if((atoi(argv[2]) == 1) && (atoi(argv[3]) == 0)){
				macPingprint("eth0","eth1");
				lec_multi_test("eth1","eth0");
			}
			else{
				argv_help(argv[0]);
			}
		}
		else{
			argv_help(argv[0]);
		}
	}
    	else{
            argv_help(argv[0]);
    	} 	
	//3.
    	return 0;
}
