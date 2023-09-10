#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "network.h"
using namespace std;

//	只被accept调用
NetWork::NetWork(void)
{
	type = SOCK_STREAM;
	is_svr = true;
}

NetWork::NetWork(int type,const char* ip,unsigned short port,bool is_svr):type(type),is_svr(is_svr)
{
	//	不能在构造函数创建socket 因为可能会失败，但构造函数没有返回值
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	addrlen = sizeof(addr);
}

NetWork::~NetWork(void)
{
	close(sock);
}

//	创建socket、绑定、监听、连接
bool NetWork::start_nw(void)
{
	sock = socket(AF_INET,type,0);
	if(0 > sock)
	{
		perror("socket");
		return false;
	}

	if(is_svr)
	{
		if(bind(sock,(SP)&addr,addrlen))
		{
			perror("bind");
			return false;
		}
		
		if(SOCK_STREAM == type)
		{
			if(listen(sock,10))
			{
				perror("listen");
				return false;
			}
		}
	}
	else if(SOCK_STREAM == type)
	{
		if(connect(sock,(SP)&addr,addrlen))
		{
			perror("connect");
			return false;
		}
	}
	return true;
}

//	等待连接
NetWork* NetWork::accept(void)
{
	if(SOCK_STREAM != type || !is_svr)
	{
		cout << "必须为TCP服务端才可调用" << endl;
		return NULL;
	}

	//	调用无参构造
	NetWork* cli_nw = new NetWork;

	cli_nw->sock = ::accept(sock,(SP)&cli_nw->addr,&cli_nw->addrlen);
	if(0 >cli_nw->sock)
	{
		perror("::accept");
		delete cli_nw;
		return NULL;
	}
	return cli_nw;
}

//	重载发送
int NetWork::send(const char* buf,int flag)
{
	if(SOCK_STREAM == type)
	{
		int ret = ::send(sock,buf,strlen(buf)+1,flag);
		if(0 >= ret)
			perror("::send");
		return ret;
	}
	else
	{
		int ret = sendto(sock,buf,strlen(buf)+1,flag,(SP)&addr,addrlen);
		if(0 >= ret)
			perror("::sendto");
		return ret;
	}

}

//	发送
int NetWork::send(const char* buf,size_t len,int flag)
{
	if(SOCK_STREAM == type)
	{
		int ret = ::send(sock,buf,len,flag);
		if(0 >= ret)
			perror("::send");
		return ret;
	}
	else
	{
		int ret = sendto(sock,buf,len,flag,(SP)&addr,addrlen);
		if(0 >= ret)
			perror("::sendto");
		return ret;
	}
	
}

//	接收
int NetWork::recv(char* buf,size_t len,int flag)
{
	if(SOCK_STREAM == type)
	{
		int ret = ::recv(sock,buf,len,flag);
		if(0 >= ret)
			perror("::recv");
		return ret;
	}
	else
	{
		int ret = recvfrom(sock,buf,len,flag,(SP)&addr,&addrlen);
		if(0 >= ret)
			perror("::recvfrom");
		return ret;
	}
	
}



