#include "tools.h"

int get_cmd(char start,char end)
{
	puts("请输入指令:");
	for(;;)
	{
		stdin->_IO_read_ptr = stdin->_IO_read_end;
		char cmd = getch();
		if(start <= cmd && cmd <= end)
		{
			printf("%c\n",cmd);
			return cmd;
		}
	}
}

void anykey_continue(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	cout << "请按任意键继续....." << endl;
	getch();
}
