#include "manager_ctrl_impl.h"
#include "manager_mode_impl.h"

ManagerCtrlImpl::ManagerCtrlImpl(void)
{
	mgrMode = new ManagerModeImpl;
	mgrMode->load(mgrArr);
}

ManagerCtrlImpl::~ManagerCtrlImpl(void)
{
	mgrMode->save(mgrArr);
	delete mgrMode;
}

bool ManagerCtrlImpl::addMgr(Manager& mgr)
{
	for(unsigned int i=0; i<mgrArr.size(); i++)
	{
		if(!strcmp(mgrArr[i].getName(),mgr.getName()))
		{
			return false;
		}	
	}
	
	mgr.setId(get_mgrid());
	mgrArr.push_back(mgr);
	return true;
}

bool ManagerCtrlImpl::delMgr(int id)
{
	vector<Manager>::iterator it;
	for(it=mgrArr.begin(); it!=mgrArr.end(); it++)
	{
		if(it->getId() == id)
		{
			mgrArr.erase(it);
			return true;
		}
	}
	return false;
}

vector<Manager>& ManagerCtrlImpl::listMgr(void)
{
	return mgrArr;
}








