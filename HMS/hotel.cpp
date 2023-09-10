#include <stdio.h>
#include <stdlib.h>
#include "hotel.h"

//	静态成员变量类外定义
Hotel Hotel::hotel;

//加载房间信息
bool Hotel::load(const char* path)
{
	FILE* frp = fopen(path,"r");
	if(NULL == frp)
	{
		perror("fopen");
		return false;	
	}
	
	fscanf(frp,"%d",&level_cnt);
	
	rooms = new Room**[level_cnt];
	level_room = new int[level_cnt];
	
	for(int i=0; i<level_cnt; i++)
	{
		fscanf(frp,"%d",&level_room[i]);
		rooms[i] = new Room*[level_room[i]];
		for(int j=0; j<level_room[i]; j++)
		{
			short id;
			char type;
			float price;
			fscanf(frp,"%hd %hhd %f",&id,&type,&price);
			rooms[i][j] = new Room(id,type,price);
		}
	}
	fclose(frp);
	return true;
}

//酒店运行
void Hotel::start(void)
{
	for(;;)
	{
		switch(menu())
		{
			case '1':  check_in();		break;
			case '2':  query_room();	break;
			case '3':  check_out();		break;
			case '4':  list_empty();	break;
			case '5':  upgrade_room();	break;
			case '6':  return;
		}
	}
}

//菜单
int Hotel::menu(void)
{
	system("clear");
	cout << "****欢迎使用指针酒店管理系统****" << endl;
	cout << "1、入住    2、查询房间" << endl;
	cout << "3、退房    4、查看空房" << endl;
	cout << "5、升级    6、退出系统" << endl;
	return get_cmd('1','6');
}

//入住
void Hotel::check_in(void)
{
	int room_id;
	cout << "请输入要入住的房间号:";
	cin >> room_id;
	Room* room = find_room(room_id);
	if(NULL == room)
	{
		cout << "未找到该房间" << room_id << "请检查" << endl;
		anykey_continue();
		return;
	}	
	
	string name,id;
	char sex;
	cout << "请输入住人姓名、性别、身份证号:"<< endl;;
	cin >> name >> sex >> id;
	Guest* guest = new Guest(name,sex,id);
	
	if(room->in(guest))
	{
		cout << "入住成功" << endl;
	} 
	else
	{
		cout << "该房间已满" << endl;
		delete guest;
	}
	anykey_continue();
}

//显示空房
void Hotel::list_empty(void)
{
	for(int i =0;i<level_cnt;i++)
	{
		for(int j =0;j<level_room[i];j++)
		{
			if(rooms[i][j]->is_empty())
			{
				cout << *rooms[i][j];
			}
		}
	}
	anykey_continue();
}

//查询已入住
void Hotel::query_room(void)
{
	for(int i =0;i<level_cnt;i++)
	{
		for(int j =0;j<level_room[i];j++)
		{
			if(!rooms[i][j]->is_empty())
			{
				cout << *rooms[i][j];
			}
		}
	}
	anykey_continue();
}

//退房
void Hotel::check_out(void)
{
	int room_id;
	cout <<"请输入要退房的房间号:";
	cin >> room_id;
	
	Room* room = find_room(room_id);
	if(NULL == room)
	{
		cout << "未找到该房间" << room_id << "请检查" << endl;
		return;
	}	
	
	//全部退房
	if(room->out())
	{
		cout << "退房成功" << endl;
	}
	else
	{
		cout << "此房间为空" << endl;
	}
	anykey_continue();
}

//升级
void Hotel::upgrade_room(void)
{
	cout << __func__ << endl;
}

//	找房间
Room* Hotel::find_room(int room_id)
{
	for(int i=0; i<level_cnt; i++)
	{
		for(int j=0; j<level_room[i]; j++)
		{
			if(*rooms[i][j] == room_id)
			{
				return rooms[i][j];
			}
		}
	}
	return NULL;
}











