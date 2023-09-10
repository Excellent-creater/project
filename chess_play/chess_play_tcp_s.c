#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char board[15][15];
char role = '@';
char key_x,key_y;

//	初始化棋盘
void init_board(void)
{
	for(int i=0; i<15; i++)
	{
		for(int j=0; j<15; j++)
		{
			board[i][j] = '*';	
		}
	}
}

//	显示棋盘
void show_board(void)
{
	system("clear");
	for(int i=0; i<15; i++)
	{
		for(int j=0; j<15; j++)
		{
			printf("%c ",board[i][j]);	
		}
		printf("\n");
	}
}

//	落子
void get_key(void)
{
	printf("请%c落子:",role);
	for(;;)
	{
		scanf("%hhd%hhd",&key_x,&key_y);
		if(0>key_x || 14<key_x || 0>key_y || 14<key_y)
		{
			printf("坐标非法，请重新输入:");
			continue;
		}
		if('*' != board[key_x][key_y])
		{
			printf("该位置已有棋子,请重新输入:");
			continue;
		}
		board[key_x][key_y] = role;
		return;
	}
}

//	计算某一个小方向有几个连续棋子
int dir_count(int ox,int oy)
{
	int cnt = 0;
	for(int x=key_x+ox,y=key_y+oy; 0<=x && 15>x && 0<=y && 15>y; 
		x += ox,y += oy)
	{
		if(board[x][y] != role)
		{
			return cnt;	
		}
		cnt++;
	}
	return cnt;
}

//	判断是否五子连珠
bool is_win(void)
{
	if(dir_count(0,-1) + dir_count(0,1) >= 4)	return true;
	if(dir_count(-1,0) + dir_count(1,0) >= 4)	return true;
	if(dir_count(-1,-1) + dir_count(1,1) >= 4)	return true;
	if(dir_count(-1,1) + dir_count(1,-1) >= 4)	return true;
	return false;
}

int main(int argc,const char* argv[])
{
	init_board();
	for(;;)
	{
		show_board();

		get_key();

		if(is_win())
		{
			show_board();
			printf("%c赢了!\n",role);
			return 0;
		}

		//	交换角色
		role = role == '@'?'$':'@';
	}
}




