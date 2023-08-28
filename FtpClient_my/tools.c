#include "tools.h"
#include <getch.h>

//	从键盘获取指定长度的字符串
char* get_str(char* str,size_t hope_len)
{
	if(NULL == str || 1 >= hope_len) return NULL;
	
	//	清理输入缓冲区
	stdin->_IO_read_ptr = stdin->_IO_read_end;

	int cnt = 0;
	while(cnt < hope_len-1)
	{
		char key_val = getch();
		if(10 == key_val) break;
		if(127 == key_val)
		{
			if(0 < cnt)
			{
				cnt--;
				printf("\b \b");
			}
			continue;
		}
		printf("%c",key_val);
		str[cnt++] = key_val;
	}
	str[cnt] = '\0';
	printf("\n");
	return str;
}

//	从键盘获取密码
char* get_pass(char* str,size_t hope_len,bool is_show)
{
	if(NULL == str || 1 >= hope_len) return NULL;
	
	//	清理输入缓冲区
	stdin->_IO_read_ptr = stdin->_IO_read_end;

	int cnt = 0;
	while(cnt < hope_len-1)
	{
		char key_val = getch();
		if(10 == key_val) break;
		if(127 == key_val)
		{
			if(0 < cnt)
			{
				cnt--;
				if(is_show)	printf("\b \b");
			}
			continue;
		}
		if(is_show) printf("*");
		str[cnt++] = key_val;
	}
	str[cnt] = '\0';
	printf("\n");
	return str;
}

//	文件数据传输
void file_oi(int ofd,int ifd)
{
	char buf[4096] = {};
	int ret = 0;
	while((ret = read(ofd,buf,sizeof(buf))))
	{
		write(ifd,buf,ret);	
	}
}










