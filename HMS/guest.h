#ifndef GUEST_H
#define GUEST_H

#include "tools.h"

class Guest
{
	string name;
	char sex;
	string id;
public:
	Guest(const string& name,char sex,const string& id):name(name),sex(sex),id(id) {}
	friend ostream& operator<<(ostream& os,const Guest& guest)
	{
		return os << guest.name	<< " " << guest.sex << " " << guest.id;
	}
};

#endif//GUEST_H
