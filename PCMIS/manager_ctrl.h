#ifndef MANAGER_CTRL_H
#define MANAGER_CTRL_H

#include "manager.h"
#include <vector>

class ManagerCtrl
{
public:
	virtual ~ManagerCtrl(void){}
	virtual bool addMgr(Manager& mgr)=0; 
	virtual bool delMgr(int id)=0; 
	virtual vector<Manager>& listMgr(void)=0;
};

#endif//MANAGER_CTRL_H


