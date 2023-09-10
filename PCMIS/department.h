#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "tools.h"
#include "employee.h"

class Department
{
	int m_iId;
	char m_strName[20];
public:
//	vector<Employee> empArr;
	Department(const char* name="");
	~Department(void);
	void setId(int id);
	void setName(char* name);
	int getId(void);
	char* getName(void);
	friend ostream& operator<<(ostream& os,const Department& dept);
	//friend istream& operator>>(istream& is,Department& dept);
};

#endif
