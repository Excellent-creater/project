#ifndef SERVICE_CTRL_IMPL_H
#define SERVICE_CTRL_IMPL_H

#include "service_ctrl.h"


class ServiceCtrlImpl : public ServiceCtrl
{
	ServiceMode* srcMode;
	vector<Department> deptArr;
	vector<Employee> empArr;
public:
	ServiceCtrlImpl(void);
	~ServiceCtrlImpl(void);
	bool addDept(Department& dept);
	int delDept(int id);
	vector<Department>& listDept(void);
	bool addEmp(int dept_id,Employee& emp);
	bool delEmp(int id);
	bool modEmp(int dept_id,Employee& emp);
	vector<Employee>& listEmp(int dept_id);
	vector<Department>& listAllEmp(void);
};

#endif
