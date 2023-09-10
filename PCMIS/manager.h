#ifndef MANAGER_H
#define MANAGER_H

#include "tools.h"

class Manager
{
	int m_iId;
	char m_strName[20];
	char m_strPwd[20];
public:
	Manager(const char* name="",const char* pwd="");
	int getId(void)const;
	const char* getName(void)const;
	const char* getPwd(void)const;
	void setId(int id);
	void setName(const char* name);
	void setPwd(const char* pwd);
	
	friend ostream& operator<<(ostream& os,const Manager& mgr);
};

#endif//MANAGER_H










