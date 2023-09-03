#ifndef FTP_CLIENT_H
#define FTP_CLIENT_H

#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define BUF_SIZE (4096)

#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug(...)
#endif

#define error(...) printf("%s %s:%d %s %m %s %s\n",__FILE__,__func__,__LINE__,__VA_ARGS__,__DATE__,__TIME__)

typedef struct FTPClient
{
	int cli_sock;		//	命令通道
	int pasv_sock;		//	数据通道
	char ip[16];		//	服务器ip
	char server_path[PATH_MAX];	//	服务器的工作路径
	char client_path[PATH_MAX];	//	客户端的工作路径
	char* buf;			//	发送和接收命令
}FTPClient;

typedef struct sockaddr* SP;

//	创建FTP客户端对象
FTPClient* create_FTPClient(void);
//	销毁FTP客户端对象
void destroy_FTPClient(FTPClient* ftp);
//	连接服务器
int connect_FTPClient(FTPClient* ftp,const char* ip,uint16_t port);
//	发送用户名
void user_FTPClient(FTPClient* ftp,const char* user);
//	发送密码
void pass_FTPClient(FTPClient* ftp,const char* pass);
//	pwd
void pwd_FTPClient(FTPClient* ftp);
//	cd
void cd_FTPClient(FTPClient* ftp,const char* path);
//	mkdir
void mkdir_FTPClient(FTPClient* ftp,const char* dir);
//	rmdir
void rmdir_FTPClient(FTPClient* ftp,const char* dir);
//	delete
void delete_FTPClient(FTPClient* ftp,const char* filename);
//	bye
void bye_FTPClient(FTPClient* ftp);
//	ls
void ls_FTPClient(FTPClient* ftp);
//	put
void put_FTPClient(FTPClient* ftp,const char* filename);
//	get
void get_FTPClient(FTPClient* ftp,const char* filename);

#endif//FTP_CLIENT_H
