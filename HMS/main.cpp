#include "hotel.h"

int main(int argc,const char* argv[])
{
	Hotel& hotel = Hotel::get_Hotel();
	
	if(2 != argc)
	{
		cout << "User: ./HMS <path>";
		return 0;
	}
	if(!hotel.load(argv[1]))
	{
		return -1;
	}
	hotel.start();
	cout << "退出系统" << endl;
}

