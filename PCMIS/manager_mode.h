#ifndef MANAGER_MODE_H
#define MANAGER_MODE_H

#include "manager.h"

class ManagerMode
{
public:
	virtual ~ManagerMode(void){}
	virtual void load(vector<Manager>& mgrArr)=0;
	virtual void save(vector<Manager>& mgrArr)=0;
};

#endif//MANAGER_MODE_H

