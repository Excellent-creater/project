#ifndef MANAGER_CTRL_IMPL_H
#define MANAGER_CTRL_IMPL_H

#include "manager_ctrl.h"
#include "manager_mode.h"

class ManagerCtrlImpl : public ManagerCtrl
{
	vector<Manager> mgrArr;
	ManagerMode* mgrMode;
public:
	ManagerCtrlImpl(void);
	~ManagerCtrlImpl(void);
	bool addMgr(Manager& mgr); 
	bool delMgr(int id); 
	vector<Manager>& listMgr(void);
};

#endif//MANAGER_CTRL_IMPL_H


