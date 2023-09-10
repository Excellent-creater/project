#ifndef SERVICE_CTRL_H
#define SERVICE_CTRL_H


#include "department.h"
#include "service_mode_impl.h"

class ServiceCtrl
{
public:
	virtual ~ServiceCtrl(void){}
	virtual bool addDept(Department& dept) = 0;
	virtual int delDept(int id) = 0;
	virtual vector<Department>& listDept(void) = 0;
	virtual bool addEmp(int dept_id,Employee& emp) = 0;
	virtual bool delEmp(int id) = 0;
	virtual bool modEmp(int dept_id,Employee& emp) = 0;
	virtual vector<Employee>& listEmp(int dept_id) = 0;
	virtual vector<Department>& listAllEmp(void) = 0;
};

#endif
