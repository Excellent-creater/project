#include <iostream>
#include "network.h"
#include <cstring>
using namespace std;

#define BUF_SIZE 4096

int main(int argc,const char* argv[])
{
	if(3 != argc)
	{
		printf("Usr:./a.out ip port\n");
		return 0;
	}
	//创建服务器的对象
	NetWork server(SOCK_STREAM,argv[1],atoi(argv[2]),true); 
	//启动服务器
	if(!server.start_nw())
	{
		cout << "服务器启动失败" << endl;
		return -1;
	}

	cout << "服务器启动成功，等待客户端连接..." << endl;

	NetWork* client = server.accept();
	if(NULL == client)
	{
		cout << "等待客户端连接失败" << endl;
		return -1;
	}

	cout << "客户端连接成功" << endl;

	//接收发送数据
	char buf[BUF_SIZE];
	for(;;)
	{
		int ret = client->recv(buf,sizeof(buf)-1,0);
		if(0 >= ret || 0 == strncmp(buf,"quit",4))
		{
			cout << "结束通信" << endl;
			break;
		}
		
		buf[ret] = '\0';
		cout << "Recv:" << buf << endl;

		const char* response = "message";
		client->send(response,0);
	}

	delete client;
	return 0;
}




