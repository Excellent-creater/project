#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/sendfile.h>
#include <time.h>
#include <getch.h>
#include "ftp_client.h"
#include "tools.h"

//	发送指令
static void send_cmd(FTPClient* ftp)
{
	ssize_t size = send(ftp->cli_sock,ftp->buf,strlen(ftp->buf),0);	
	if(0 >= size)
	{
		error("send");
		destroy_FTPClient(ftp);
		exit(EXIT_FAILURE);
	}
}

//	接收指令结果并判断
static int recv_status(FTPClient* ftp,int status,bool is_die)
{
	ssize_t ret = recv(ftp->cli_sock,ftp->buf,BUF_SIZE,0);	
	if(0 >= ret)
	{
		error("recv");
		destroy_FTPClient(ftp);
		exit(EXIT_FAILURE);
	}

	ftp->buf[ret] = '\0';
	printf("%s",ftp->buf);

	int ret_status = 0;
	sscanf(ftp->buf,"%d",&ret_status);
	if(status != ret_status && is_die)
	{
		destroy_FTPClient(ftp);
		exit(EXIT_FAILURE);	
	}
	
	return status == ret_status ? ret : -1;
}

//	让socket连接服务器
static int _connect(int sock,const char* ip,uint16_t port)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	if(connect(sock,(SP)&addr,sizeof(addr)))
	{
		error("connect");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//	开启并连接数据通道
static int pasv(FTPClient* ftp)
{
	ftp->pasv_sock = socket(AF_INET,SOCK_STREAM,0);
	if(0 > ftp->pasv_sock) 
	{
		error("socket");
		return EXIT_FAILURE;
	}

	sprintf(ftp->buf,"PASV\n");
	send_cmd(ftp);
	if(-1 == recv_status(ftp,227,false)) 
	{
		close(ftp->pasv_sock);
		ftp->pasv_sock = -1;
		return EXIT_FAILURE;
	}

	uint8_t port1,port2;
	sscanf(ftp->buf,"227 Entering Passive Mode (%*d,%*d,%*d,%*d,%hhu,%hhu)",&port1,&port2);
	uint16_t port = port1*256 + port2;
	
	return _connect(ftp->pasv_sock,ftp->ip,port);
}

//	创建FTP客户端对象
FTPClient* create_FTPClient(void)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	FTPClient* ftp = malloc(sizeof(FTPClient));
	ftp->cli_sock = socket(AF_INET,SOCK_STREAM,0);
	if(0 > ftp->cli_sock)
	{
		free(ftp);
		error("socket");
		return NULL;
	}

	ftp->pasv_sock = -1;
	ftp->buf = malloc(BUF_SIZE);
	return ftp;
}

//	销毁FTP客户端对象
void destroy_FTPClient(FTPClient* ftp)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	close(ftp->cli_sock);
	if(0 < ftp->pasv_sock) close(ftp->pasv_sock);
	free(ftp->buf);
	free(ftp);
}

//	连接服务器命令通道
int connect_FTPClient(FTPClient* ftp,const char* ip,uint16_t port)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	strcpy(ftp->ip,ip);

	if(_connect(ftp->cli_sock,ip,port))
	{
		return EXIT_FAILURE;	
	}
	recv_status(ftp,220,true);
	return EXIT_SUCCESS;
}

//	发送用户名
void user_FTPClient(FTPClient* ftp,const char* user)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"USER %s\n",user);
	send_cmd(ftp);
	recv_status(ftp,331,false);
}

//	发送密码
void pass_FTPClient(FTPClient* ftp,const char* pass)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"PASS %s\n",pass);
	send_cmd(ftp);
	recv_status(ftp,230,true);
}

//	pwd
void pwd_FTPClient(FTPClient* ftp)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"PWD\n");
	send_cmd(ftp);
	int ret = recv_status(ftp,257,false);

	if(0 < ret)
	{
		sscanf(ftp->buf,"%*d \"%s",ftp->server_path);
		*strchr(ftp->server_path,'\"') = '\0';
		printf("server_path:%s\n",ftp->server_path);
	}
}

//	cd
void cd_FTPClient(FTPClient* ftp,const char* path)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"CWD %s\n",path);
	send_cmd(ftp);
	recv_status(ftp,250,false);

	pwd_FTPClient(ftp);
}

//	mkdir
void mkdir_FTPClient(FTPClient* ftp,const char* dir)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"MKD %s\n",dir);
	send_cmd(ftp);
	recv_status(ftp,257,false);
}

//	rmdir
void rmdir_FTPClient(FTPClient* ftp,const char* dir)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"RMD %s\n",dir);
	send_cmd(ftp);
	recv_status(ftp,250,false);
}

//	delete
void delete_FTPClient(FTPClient* ftp,const char* filename)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"DELE %s\n",filename);
	send_cmd(ftp);
	recv_status(ftp,250,false);
}

//	bye
void bye_FTPClient(FTPClient* ftp)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	sprintf(ftp->buf,"QUIT\n");
	send_cmd(ftp);
	recv_status(ftp,221,false);
	destroy_FTPClient(ftp);
	exit(EXIT_SUCCESS);
}

//	ls
void ls_FTPClient(FTPClient* ftp)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	if(pasv(ftp)) return;

	sprintf(ftp->buf,"LIST -al\n");
	send_cmd(ftp);
	if(-1 == recv_status(ftp,150,false))
	{
		close(ftp->pasv_sock);
		ftp->pasv_sock = -1;
		return;
	}

	//	接收数据
	file_oi(ftp->pasv_sock,STDOUT_FILENO);
	recv_status(ftp,226,false);
	close(ftp->pasv_sock);
	ftp->pasv_sock = -1;
}

//	put
/*
void put_FTPClient(FTPClient* ftp,const char* filename)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
	int file_fd = open(filename,O_RDONLY);
	if(0 > file_fd)
	{
		printf("%s 文件不存在，请检查\n",filename);
		return;
	}

	if(pasv(ftp)) 
	{
		close(file_fd);
		return;
	}

	//	数据上传
	sprintf(ftp->buf,"STOR %s\n",filename);
	send_cmd(ftp);
	if(-1 == recv_status(ftp,150,false))
	{
		close(file_fd);
		close(ftp->pasv_sock);
		ftp->pasv_sock = -1;
		return;
	}
	
	//	1、file_oi是从磁盘拷贝到内存、内存再拷贝到另一个磁盘文件完成传输，拷贝次数多、用户态内核态的切换频繁，效率低
	//	2、零拷贝目的是减少不必要拷贝，降低内核态用户态的切换次数
	//	3、零拷贝的方法：mmap、sendfile
	//file_oi(file_fd,ftp->pasv_sock);
	
	struct stat stat_buf = {};
	fstat(file_fd,&stat_buf);

	sendfile(ftp->pasv_sock,file_fd,0,stat_buf.st_size);
	close(file_fd);
	close(ftp->pasv_sock);
	ftp->pasv_sock = -1;
	
	recv_status(ftp,226,false);
}
*/

//	获取文件最后修改时间
int get_file_mdtm(int fd,char* mdtm)
{
	struct stat stat_buf = {};
	if(fstat(fd,&stat_buf)) return EXIT_FAILURE;

	struct tm* t = localtime(&stat_buf.st_mtime);
	sprintf(mdtm,"%4d%02d%02d%02d%02d%02d",
		t->tm_year+1900,
		t->tm_mon+1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	return EXIT_SUCCESS;
}

//	获取文件大小
int file_size(const char* file)
{
	int fd = open(file,O_RDONLY);
	if(0 > fd) return -1;

	size_t size = lseek(fd,0,SEEK_END);
	close(fd);
	return size;
}

//	断点续传 上传
void put_FTPClient(FTPClient* ftp,const char* filename)
{
	//	是否存在本地文件
	int file_fd = open(filename,O_RDONLY);
	if(0 > file_fd)
	{
		//	否：结束
		printf("%s 文件不存在\n",filename);
		return;
	}

	//	时间戳
	char sfile_mdtm[15] = {};
	char cfile_mdtm[15] = {};

	//	获取本地文件的时间戳
	get_file_mdtm(file_fd,cfile_mdtm);

	//	是：检查服务器是否存在同名文件 SIZE
	sprintf(ftp->buf,"SIZE %s\n",filename);
	send_cmd(ftp);
	if(-1 != recv_status(ftp,213,false))
	{
		//	记录服务器同名文件的大小
		size_t sfile_size = 0;
		sscanf(ftp->buf,"%*d %lu",&sfile_size);
		
		//	是：根据时间戳检查是否与本地文件为同一个文件
		//	获取服务器文件时间戳
		sprintf(ftp->buf,"MDTM %s\n",filename);
		send_cmd(ftp);
		recv_status(ftp,213,false);
		sscanf(ftp->buf,"%*d %s",sfile_mdtm);
		if(0 == strcmp(sfile_mdtm,cfile_mdtm))
		{
			//	是：根据文件大小检查是否需要续传
			//	获取本地文件的大小
			size_t cfile_size = file_size(filename);
			if(cfile_size == sfile_size)
			{
				//	大小相同：结束传输
				printf("上传完成\n");
				close(file_fd);
				return;
			}
			else
			{
				//	不相同： 断点续传
				printf("开始断点续传\n");
				sprintf(ftp->buf,"REST %lu\n",sfile_size);
				send_cmd(ftp);
				recv_status(ftp,350,false);

				lseek(file_fd,sfile_size,SEEK_SET);
			}
		}
		else
		{
			//	否：不是同一个文件，询问是否覆盖
			printf("已有同名文件存在，是否覆盖?（y/n)");
			if('y' != getch())
			{
				printf("取消覆盖\n");
				close(file_fd);
				return;
			}

		}
	}
		//	否：正常传输
	if(pasv(ftp)) 
	{
		close(file_fd);
		return;
	}

	//	数据上传
	sprintf(ftp->buf,"STOR %s\n",filename);
	send_cmd(ftp);
	if(-1 == recv_status(ftp,150,false))
	{
		close(file_fd);
		close(ftp->pasv_sock);
		ftp->pasv_sock = -1;
		return;
	}
	
	file_oi(file_fd,ftp->pasv_sock);
	close(file_fd);
	close(ftp->pasv_sock);
	recv_status(ftp,226,false);

	//	上传文件的最后修改时间
	sprintf(ftp->buf,"MDTM %s %s/%s\n",cfile_mdtm,ftp->server_path,filename);
	printf("buf:%s\n",ftp->buf);
	send_cmd(ftp);
	recv_status(ftp,213,false);
}

//	get
void get_FTPClient(FTPClient* ftp,const char* filename)
{
	debug("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);

}































