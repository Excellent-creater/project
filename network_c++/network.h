#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef sockaddr* SP;

class NetWork
{
	int sock;
	int type;
	sockaddr_in addr;
	socklen_t addrlen;
	bool is_svr;
public:
	NetWork(void);
	NetWork(int type,const char* ip,unsigned short port,bool is_svr = true);
	~NetWork(void);

	// 创建socket,绑定、监听、连接
	bool start_nw(void);
	// 等待连接
	NetWork* accept(void);
	//重载发送
	int send(const char* buf,int flag = 0);
	//发送
	int send(const char* buf,size_t len,int flag = 0);
	//接收
	int recv(char* buf,size_t len,int flag = 0);
};



#endif// NETWORK_H
