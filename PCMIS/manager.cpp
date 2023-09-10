#include "manager.h"

Manager::Manager(const char* name,const char* pwd)
{
	strcpy(m_strName,name);
	strcpy(m_strPwd,pwd);
}

int Manager::getId(void)const
{
	return m_iId;
}

const char* Manager::getName(void)const
{
	return m_strName;
}

const char* Manager::getPwd(void)const
{
	return m_strPwd;
}

void Manager::setId(int id)
{
	m_iId = id;
}

void Manager::setName(const char* name)
{
	strcpy(m_strName,name);
	size_t len = strlen(name);
    size_t copy_len = (len<sizeof(m_strName)-1) ? len :sizeof(m_strName)-1;
    strncpy(m_strName,name,copy_len); 
    m_strName[copy_len] = '\0';

}

void Manager::setPwd(const char* pwd)
{
	strcpy(m_strPwd,pwd);
	size_t len = strlen(pwd);
    size_t copy_len = (len<sizeof(m_strPwd)-1) ? len :sizeof(m_strPwd)-1;
    strncpy(m_strPwd,pwd,copy_len); 
    m_strPwd[copy_len] = '\0';
}

ostream& operator<<(ostream& os,const Manager& mgr)
{
	return os << mgr.m_iId << " " << mgr.m_strName << " " << mgr.m_strPwd;
}







