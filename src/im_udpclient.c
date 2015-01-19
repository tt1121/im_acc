/** 文 件 名: im_json_parse.c
** Copyright (c), imove(SHENTHEN) Co., Ltd.
** 日    期: 2014-12-28
** 描    述:
** 版    本:
** 修改历史:
** 2014-12-28   张斌创建本文件；
##############################################################################*/

#include "im_wideband_speedup.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define IFNAME "br-lan" 
#define MCAST_PORT (65501)

int im_udpclient_init(int *socketfd, struct sockaddr_in *addrto)
{
	int ret = MSG_CODE_SUCESS;
	const int opt = 1;
	struct ifreq ifr;
	
	if (!socketfd || !addrto)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	if ((*socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)	 
	{	  
		IM_DEBUG("socket fail");  
		return -1;  
	}	  
	#if 0
	strncpy(ifr.ifr_name,IFNAME,strlen(IFNAME));
	//发送命令，获得网络接口的广播地址
	if(ioctl(*socketfd, SIOCGIFBRDADDR, &ifr) == -1){
		IM_DEBUG("ioctl SIOCGIFBRDADDR error");
		return -1;
	}
	
	bzero(addrto, sizeof(struct sockaddr_in));  
	//将获得的广播地址复制到addrto
	memcpy(addrto, &ifr.ifr_broadaddr, sizeof(struct sockaddr_in));
	//设置广播端口号
	printf("broadcast IP is:%s\n",inet_ntoa(addrto->sin_addr));
	#endif
	inet_aton("192.168.222.255", &addrto->sin_addr);
//	printf("broadcast IP is:%s\n",inet_ntoa(addrto->sin_addr));
    addrto->sin_family = AF_INET;  
//    addrto->sin_addr.s_addr=htonl(INADDR_BROADCAST);  
    addrto->sin_port = htons(MCAST_PORT);  
	printf("broadcast IP is:%s\n",inet_ntoa(addrto->sin_addr));

	  
    //设置该套接字为广播类型，  
    ret = setsockopt(*socketfd, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
    if(ret < 0 )  
    {  
        IM_DEBUG("set socket error...");  
    }  
	
	return ret;
}

int im_udp_sendmsg(void *msg)
{
	int socketfd = -1;
	struct sockaddr_in addrto;
	int nlen = sizeof(struct sockaddr);
	int ret = MSG_CODE_SUCESS;

	if (!msg)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	ret = im_udpclient_init(&socketfd, &addrto);
	if (ret != MSG_CODE_SUCESS)
	{
		close(socketfd);
		IM_DEBUG("im_udpclient_init fail");
		return MSG_CODE_UNKNOWN;
	}
	printf("udp = %s\n",msg);
    ret = sendto(socketfd, msg, strlen(msg), 0, (struct sockaddr*)&addrto, nlen);  
    if (ret < 0)
    {  
        IM_DEBUG("sendto fail");
		ret = MSG_CODE_UNKNOWN;
    } 
	else
	{
		ret = MSG_CODE_SUCESS;
	}
	
	close(socketfd);
	return ret;
}

 
int im_get_local_ipv4(char *ifname, char *local_ip)
{
	int socket_fd;
	struct sockaddr_in *sin;
	struct ifreq ifr;
	int ret = MSG_CODE_SUCESS;
	
	if (!ifname || !local_ip)
	{
		IM_DEBUG("parameter error");
		return MSG_CODE_UNKNOWN;
	}
	
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(socket_fd ==-1)
	{
		IM_DEBUG("socket error!\n");
		return MSG_CODE_UNKNOWN;
	}

	strcpy(ifr.ifr_name, ifname);

	if(ioctl(socket_fd, SIOCGIFADDR, &ifr) < 0)
	{
		IM_DEBUG("ioctl error\n");
		ret = MSG_CODE_UNKNOWN;
	}
	else
	{
		sin = (struct sockaddr_in *)&(ifr.ifr_addr);
		strcpy(local_ip, inet_ntoa(sin->sin_addr));
		//printf("current IP = %s\n",inet_ntoa(sin->sin_addr));
	}

	close(socket_fd);
	return ret;
	
}

