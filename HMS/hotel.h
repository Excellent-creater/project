#ifndef HOTEL_H
#define HOTEL_H

#include "room.h"

class Hotel
{
	static Hotel hotel;	//	单例静态酒店对象
	int level_cnt;	//	层数
	int* level_room;	//	每层房间数
	Room*** rooms;		//	房间对象指针
	
	Hotel(void){};	//	构造函数私有化
	Hotel(const Hotel& that){}	//	拷贝构造私有化
	
public:
	static Hotel& get_Hotel(void)	//	公开静态接口函数
	{
		return hotel;
	}
	//	加载房间信息
	bool load(const char* path);	
	//	酒店运行
	void start(void);
	//	菜单
	int menu(void);
	//	入住
	void check_in(void);
	//	显示空房
	void list_empty(void);
	//	查询已入住
	void query_room(void);
	//	退房
	void check_out(void);
	//	升级
	void upgrade_room(void);
	
	//	找房间
	Room* find_room(int room_id);
	
};

#endif//HOTEL_H

