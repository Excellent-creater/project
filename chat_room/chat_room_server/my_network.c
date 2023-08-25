#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include "my_network.h"

//	分配内存、创建socket、初始化地址、绑定、监听、连接
NetWork* init_nw(int type,uint16_t port,const char* ip,bool is_svr)
{
	NetWork* nw = malloc(sizeof(NetWork));
	nw->type = type;
	nw->is_svr = is_svr;
	nw->sock_fd = socket(AF_INET,type,0);
	if(0 > nw->sock_fd)
	{
		perror("socket");
		free(nw);
		return NULL;
	}

	nw->addr.sin_family = AF_INET;
	nw->addr.sin_port = htons(port);
	nw->addr.sin_addr.s_addr = inet_addr(ip);
	nw->addrlen = sizeof(nw->addr);

	//	是服务端
	if(is_svr)
	{
		if(bind(nw->sock_fd,(SP)&nw->addr,nw->addrlen))	
		{
			perror("bind");
			free(nw);
			return NULL;
		}
		if(SOCK_STREAM == type)
		{
			if(listen(nw->sock_fd,10))
			{
				perror("listen");
				free(nw);
				return NULL;
			}
		}
	}
	else if(SOCK_STREAM == type)
	{
		if(connect(nw->sock_fd,(SP)&nw->addr,nw->addrlen))
		{
			perror("connect");
			free(nw);
			return NULL;
		}
	}
	return nw;
}

//	等待连接 只能tcp的服务端执行
NetWork* accept_nw(NetWork* svr_nw)
{
	if(SOCK_STREAM != svr_nw->type || !svr_nw->is_svr)
	{
		printf("只能TCP服务端使用本函数\n");
		return NULL;
	}

	NetWork* nw = malloc(sizeof(NetWork));
	memcpy(nw,svr_nw,sizeof(NetWork));
	nw->sock_fd = accept(svr_nw->sock_fd,(SP)&nw->addr,&nw->addrlen);
	if(0 > nw->sock_fd)
	{
		perror("accept");
		free(nw);
		return NULL;
	}
	return nw;
}

//	发送函数send/sendto
int send_nw(NetWork* nw,const void* buf,size_t len)
{
	if(SOCK_STREAM == nw->type)
	{
		int ret = send(nw->sock_fd,buf,len,0);
		if(0 >= ret)
		{
			perror("send");	
		}
		return ret;
	}
	else
	{
		int ret = sendto(nw->sock_fd,buf,len,0,(SP)&nw->addr,nw->addrlen);	
		if(0 >= ret)
		{
			perror("sendto");	
		}
		return ret;
	}
}

//	接收函数recv/recvfrom
int recv_nw(NetWork* nw,void* buf,size_t len)
{
	if(SOCK_STREAM == nw->type)
	{
		int ret = recv(nw->sock_fd,buf,len,0);
		if(0 >= ret)
		{
			perror("recv");	
		}
		return ret;
	}
	else
	{
		int ret = recvfrom(nw->sock_fd,buf,len,0,(SP)&nw->addr,&nw->addrlen);	
		if(0 >= ret)
		{
			perror("recvfrom");	
		}
		return ret;
	}
}

//	关闭socket且释放内存
void close_nw(NetWork* nw)
{
	close(nw->sock_fd);
	free(nw);
}



