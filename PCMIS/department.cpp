#include "department.h"

Department::Department(const char* name)
{
	strcpy(m_strName,name);
}
Department::~Department(void)
{
	
}
void Department::setId(int id)
{
	m_iId = id;
}
void Department::setName(char* name)
{
	strcpy(m_strName,name);
	size_t len = strlen(name);
    size_t copy_len = (len<sizeof(m_strName)-1) ? len :sizeof(m_strName)-1;
    strncpy(m_strName,name,copy_len); 
    m_strName[copy_len] = '\0';
}
int Department::getId(void)
{
	return m_iId;
}
char* Department::getName(void)
{
	return m_strName;
}
ostream& operator<<(ostream& os,const Department& dept)
{
	return os << dept.m_iId << " " << dept.m_strName; 
}


/*istream& operator>>(istream& is,Department& dept)
{
	if (is.read(reinterpret_cast<char*>(&dept.m_iId), sizeof(dept.m_iId)).fail()) return is;
    if (is.read(dept.m_strName, sizeof(dept.m_strName)).fail()) return is;

    size_t numEmployees;
    if (is.read(reinterpret_cast<char*>(&numEmployees), sizeof(numEmployees)).fail()) return is;
    dept.empArr.resize(numEmployees);
    for (auto& emp : dept.empArr) {
        if (is >> emp) return is;
    }

    return is;
}*/



