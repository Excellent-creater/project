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
	NetWork client(SOCK_STREAM,argv[1],atoi(argv[2]),false); 
	//连接服务器
	if(!client.start_nw())
	{
		cout << "连接服务器失败" << endl;
		return -1;
	}

	//接收发送数据
	char buf[BUF_SIZE];
	for(;;)
	{
		cout << ">>>";
		cin.getline(buf,sizeof(buf));
		if(0 == strncmp(buf,"quit",4))
		{
			cout << "结束通信" << endl;
			break;
		}

		//发消息给服务器
		int ret = client.send(buf,strlen(buf),0);
		if(0 > ret)
		{
			break;
		}
		ret = client.recv(buf, sizeof(buf) - 1,0);
        if (ret <= 0) {
            cout << "服务器已关闭..." << endl;
            break;
        }

        buf[ret] = '\0';
        cout << "Server response: " << buf << endl;
	}
	return 0;
}




