#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct sockaddr* SP;

int main(int argc,const char* argv[])
{

	//	创建socket
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(0 > sockfd)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	//	准备通信地址
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5566);
	
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//	连接服务器
	if(connect(sockfd,(SP)&addr,sizeof(addr)))
	{
		perror("connect");
		return EXIT_FAILURE;
	}

	char buf[4096] = {};
	for(;;)
	{
		printf(">>>");
		scanf("%s",buf);

		int ret = send(sockfd,buf,strlen(buf)+1,0);
		if(0 >= ret)
		{
			printf("服务器正在升级，请稍候...\n");
			break;
		}
		if(0 == strncmp(buf,"quit",4))
		{
			printf("退出通信\n");
			break;
		}
		
		ret = recv(sockfd,buf,sizeof(buf),0);
		if(0 >= ret)
		{
			printf("服务器正在升级，请稍候...\n");
			break;
		}
		printf("read:%s ret:%d\n",buf,ret);
	}
	close(sockfd);
}









