#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "my_network.h"

#define BUF_SIZE (4096)

void* run(void* arg)
{
	char recv_buf[BUF_SIZE] = {};
	for(;;)
	{
		int ret = recv_nw(arg,recv_buf,BUF_SIZE);
		if(0 >= ret)
		{
			printf("服务器正在升级...\n");
			close_nw(arg);
			exit(EXIT_FAILURE);
		}
		printf("\r%s\n>>>",recv_buf);
		fflush(stdout);
	}
	return NULL;
}

int main(int argc,const char* argv[])
{
	if(3 != argc)
	{
		printf("client: ./client <ip> <port>\n");
		return EXIT_SUCCESS;
	}

	NetWork* nw = init_nw(SOCK_STREAM,atoi(argv[2]),argv[1],false);
	if(NULL == nw)	return EXIT_FAILURE;

	char send_buf[BUF_SIZE] = {};
	printf("请输入昵称:");
	scanf("%s",send_buf);
	send_nw(nw,send_buf,strlen(send_buf)+1);

	pthread_t tid;
	pthread_create(&tid,NULL,run,nw);

	for(;;)
	{
		printf(">>>");
		scanf("%s",send_buf);
		int ret = send_nw(nw,send_buf,strlen(send_buf)+1);
		if(0 >= ret)
		{
			printf("服务器正在升级，请稍候....\n");
			close_nw(nw);
			return EXIT_FAILURE;
		}

		if(0 == strncmp(send_buf,"quit",4))
		{
			printf("你已退出聊天室\n");
			close_nw(nw);
			return EXIT_SUCCESS;
		}
	}
}







