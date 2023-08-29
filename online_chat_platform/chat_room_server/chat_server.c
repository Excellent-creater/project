#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "my_network.h"
#include "pthreadpool.h"

#define CLI_MAX (50)
#define BUF_SIZE (4096)

//	存储已连接的客户端NetWork对象
NetWork* clients[CLI_MAX];
//	线程池对象
ThreadPool* thread;
//	服务端NetWork对象
NetWork* svr_nw;

int add_clients(NetWork* nw)
{
	for(int i=0; i<CLI_MAX; i++)
	{
		if(NULL == clients[i])	
		{
			clients[i] = nw;
			return i;
		}
	}
	return -1;
}

void send_all(const char* str,int index)
{
	for(int i=0; i<CLI_MAX; i++)
	{
		if(NULL != clients[i] && i != index)
		{
			send_nw(clients[i],str,strlen(str)+1);	
		}
	}
}

//	消费者业务逻辑
void con_work(void* arg)
{
	//	添加到客户端数组	
	int index = add_clients(arg);
	char* buf = malloc(BUF_SIZE);
	
	//	接收昵称
	int ret = recv_nw(arg,buf,BUF_SIZE);
	if(0 >= ret || 0 == strncmp(buf,"quit",4))
	{
		close_nw(arg);
		clients[index] = NULL;
		free(buf);
		return;
	}

	strcat(buf," 进入聊天室了!");
	send_all(buf,index);

	buf[ret-1] = ':';
	char* msg = buf+ret;

	for(;;)
	{
		int msg_ret = recv_nw(arg,msg,BUF_SIZE-ret);	
		if(0 >= msg_ret || 0 == strncmp(msg,"quit",4))
		{
			sprintf(msg,"退出聊天室!");
			send_all(buf,index);
			close_nw(arg);
			clients[index] = NULL;
			free(buf);
			return;
		}
		send_all(buf,index);
	}
}

//	生产者业务逻辑
void* pro_work(void)
{
	return accept_nw(svr_nw);	
}

void sigint(int num)
{
	destroy_threadpool(thread);
	for(int i=0; i<CLI_MAX; i++) free(clients[i]);
	printf("资源已释放，服务器被关闭\n");
	exit(EXIT_SUCCESS);
}

int main(int argc,const char* argv[])
{
	signal(SIGINT,sigint);

	if(3 != argc)
	{
		printf("Server: ./chat_server <ip> <port>\n");
		return EXIT_SUCCESS;
	}

	//	初始化网络通信
	svr_nw = init_nw(SOCK_STREAM,atoi(argv[2]),argv[1],true);

	//	创建线程池
	thread = create_threadpool(CLI_MAX,1,10,con_work,pro_work);
	//	启动线程池
	start_threadpool(thread);


	pause();
}




