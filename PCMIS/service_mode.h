#ifndef SERVICE_MODE_H
#define SERVICE_MODE_H


#include "department.h"
#define DATA_SER_PATH "./data/services.dat"
#define DATA_EMP_PATH "./data/employee.dat"

class ServiceMode
{
public:
	virtual ~ServiceMode(void){}
	virtual void load(vector<Department>& deptArr) = 0;
	virtual void load_emp(vector<Employee>& empArr) = 0;
	virtual void save(vector<Department>& deptArr) = 0;
	virtual void save_emp(vector<Employee>& empArr) = 0;
	
};


#endif

