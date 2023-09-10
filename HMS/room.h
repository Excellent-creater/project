#ifndef ROOM_H
#define ROOM_H

#include "guest.h"

class Room
{
	short id;	//	房号
	char type;	//	房型
	char guest_cnt;	//	房客数量
	float price;//	房价
	Guest** guests;	//	房客指针
public:
	Room(short id,char type,float price):id(id),type(type),guest_cnt(0),price(price)
	{
		guests = new Guest*[type];
	}
	
	~Room(void)
	{
		delete[] guests;
	}
	
	//入住
	bool in(Guest* guest)
	{
		if(guest_cnt >= type)
		return false;
		guests[(int)guest_cnt++] = guest;
		return true;
	}
	
	//退房
	bool out(void)
	{
		if(0 == guest_cnt)
			return false;
		for(int i =0;i<guest_cnt;i++)
		{
			delete guests[i];
			guests[i] = NULL;
		}
		guest_cnt = 0;
		return true;
		
	}
	//判断是否空房
	bool is_empty(void)
	{
		return 0 == guest_cnt;
	}
	
	bool operator==(int id)
	{
		return this->id == id;
	}
	
	friend ostream& operator<<(ostream& os,const Room& room)
	{
		os << "房号:" << room.id << " 房型:" << (int)room.type << " 房价:" << room.price << endl;
		for(int i=0; i<room.guest_cnt; i++)
		{
			os << "房客" << i+1 << ":" << *room.guests[i] << endl;
		}
		return os;
	}
	
};




#endif//ROOM_H

