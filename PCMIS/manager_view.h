#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include "manager.h"

class ManagerView
{
public:
	virtual ~ManagerView(void){}
	virtual void loginManager(void) = 0;
    virtual void loginService(void) = 0;
    virtual void menuMgr(void) = 0;
    virtual void addMgr(void) = 0;
    virtual void delMgr(void) = 0;
    virtual void listMgr(void) = 0;

};

#endif
