#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftp_client.h"
#include "tools.h"

int main(int argc,const char* argv[])
{
	FTPClient* ftp = NULL;
	ftp = create_FTPClient();
	if(2 == argc)
	{
		if(connect_FTPClient(ftp,argv[1],21)) return EXIT_FAILURE;
	}
	else if(3 == argc)
	{
		// 	假设ftp服务端是自己实现，需要重新约定命令通道的端口号
		if(connect_FTPClient(ftp,argv[1],atoi(argv[2]))) return EXIT_FAILURE;	
	}
	else
	{
		printf("User: ./FTP xxx.xxx.xxx.xxx [port]\n");
		return EXIT_SUCCESS;
	}

	//	输入用户名 发送
	char user[30] = {};
	printf("Name: ");
	get_str(user,30);
	user_FTPClient(ftp,user);

	//	输入密码 发送
	char pass[30] = {};
	printf("PassWord: ");
	get_pass(pass,30,false);
	pass_FTPClient(ftp,pass);

	//	接收用户命令
	char cmd[PATH_MAX+10] = {};
	short cmd_size = sizeof(cmd);

	pwd_FTPClient(ftp);

	for(;;)
	{
		printf("FTP>");
		get_str(cmd,cmd_size);

		if(0 == strcmp(cmd,"pwd")) 				pwd_FTPClient(ftp);
		else if(0 == strcmp(cmd,"ls")) 			ls_FTPClient(ftp);
		else if(0 == strncmp(cmd,"cd ",3)) 		cd_FTPClient(ftp,cmd+3);
		else if(0 == strncmp(cmd,"mkdir ",6)) 	mkdir_FTPClient(ftp,cmd+6);
		else if(0 == strncmp(cmd,"rmdir ",6)) 	rmdir_FTPClient(ftp,cmd+6);
		else if(0 == strncmp(cmd,"delete ",7)) 	delete_FTPClient(ftp,cmd+7);
		else if(0 == strncmp(cmd,"get ",4)) 	get_FTPClient(ftp,cmd+4);
		else if(0 == strncmp(cmd,"put ",4)) 	put_FTPClient(ftp,cmd+4);
		else if(0 == strcmp(cmd,"bye")) 		bye_FTPClient(ftp);
		else if('!' == cmd[0])					system(cmd+1);
		else	printf("指令未定义!\n");
	}
}












