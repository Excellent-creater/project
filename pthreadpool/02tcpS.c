#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "pthreadpool.h"

typedef struct sockaddr* SP;

int sockfd;//服务连接客户端的sockfd

//消费者业务逻辑函数
void con_work(void* arg)
{
	int* cli_fd = (int*)arg;
	char buf[4096] = {};
	for(;;)
	{
		//	接收请求
		int ret = read(*cli_fd,buf,sizeof(buf));
		if(0 >= ret || 0 == strncmp(buf,"quit",4))
		{
			printf("客户端%d退出\n",*cli_fd);
			close(*cli_fd);
			free(cli_fd);
			return;
		}
		printf("from: %d recv:%s ret:%d\n",
			*cli_fd,buf,ret);

		//	回声服务器
		strcat(buf,":server send");
		ret = send(*cli_fd,buf,strlen(buf)+1,0);
		if(0 >= ret)
		{
			printf("客户端%d退出\n",*cli_fd);
			close(*cli_fd);
			free(cli_fd);
			return;
		}
	}
	close(*cli_fd);
	exit(EXIT_SUCCESS);
}

//生产者线程业务逻辑函数
void* pro_work(void)
{
	int* cli_fd = malloc(sizeof(int));
	*cli_fd = accept(sockfd,NULL,NULL);
	if(0 > *cli_fd)
    {   
    	perror("accept");
		return NULL;
	}
	return cli_fd;
}

int main(int argc,const char* argv[])
{
	//	创建socket
	sockfd = socket(AF_INET,SOCK_STREAM,0);
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

	//	绑定
	if(bind(sockfd,(SP)&addr,sizeof(addr)))
	{
		perror("bind");
		return EXIT_FAILURE;
	}

	//	监听
	if(listen(sockfd,5))
	{
		perror("listen");
		return EXIT_FAILURE;
	}

	//创建线程池
	ThreadPool* thread = create_threadpool(5,1,10,con_work,pro_work);

	//启动
	start_threadpool(thread);

	for(;;);
}







