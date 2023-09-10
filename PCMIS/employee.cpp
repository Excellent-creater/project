#include "employee.h"

Employee::Employee(const char* name,char sex,int age)
{
	strcpy(m_strName,name);
	m_cSex = sex;
	m_iAge = age;
}
void Employee::setId(int id)
{
	m_iId = id;
}
void Employee::setAge(int age)
{
	m_iAge = age;
}
void Employee::setName(const char* name)
{
	strcpy(m_strName,name);
	size_t len = strlen(name);
    size_t copy_len = (len<sizeof(m_strName)-1) ? len :sizeof(m_strName)-1;
    strncpy(m_strName,name,copy_len); 
    m_strName[copy_len] = '\0';
}
void Employee::setSex(char sex)
{
	m_cSex = sex;
}
int Employee::getId(void)
{
	return m_iId;
}
int Employee::getAge(void)
{
	return m_iAge;
}
char* Employee::getName(void)
{
	return m_strName;
}
char Employee::getSex(void)
{
	return m_cSex;
}

ostream& operator<<(ostream& os,const Employee& emp)
{
	return os << emp.m_iId << " " << emp.m_strName << " " << emp.m_cSex << " " << emp.m_iAge;
}

/*istream& operator>>(istream& is,Employee& emp)
{
    is >> emp.m_iId;
	is >> emp.m_strName; 
	is >> emp.m_cSex;
	is >> emp.m_iAge;
	return is;
}*/







