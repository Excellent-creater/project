#include "service_ctrl_impl.h"

ServiceCtrlImpl::ServiceCtrlImpl(void)
{
	srcMode = new ServiceModeImpl; 
	srcMode->load(deptArr);
	srcMode->load_emp(empArr);
}
ServiceCtrlImpl::~ServiceCtrlImpl(void)
{
	srcMode->save(deptArr);
	srcMode->save_emp(empArr);
	delete srcMode;
}
bool ServiceCtrlImpl::addDept(Department& dept)
{
	for(unsigned int i =0;i<deptArr.size();i++)
	{
		if(!strcmp(deptArr[i].getName(),dept.getName()))
		{
			return false;
		}
	}
	dept.setId(get_deptid());
	deptArr.push_back(dept);	
	return true;
}
int ServiceCtrlImpl::delDept(int id)
{
	vector<Department>::iterator it;
	for(it = deptArr.begin();it!=deptArr.end();it++)
	{
		if(it->getId() == id)
		{
			deptArr.erase(it);
			return 1;
		}
	}	
	return -1;
}
vector<Department>& ServiceCtrlImpl::listDept(void)
{
	return deptArr;
}
bool ServiceCtrlImpl::addEmp(int dept_id,Employee& emp)
{
	for(unsigned int i =0;i<empArr.size();i++)
	{
		if(!strcmp(empArr[i].getName(),emp.getName()))
		{
			return false;
		}
	}
	emp.setId(get_empid(dept_id));
	empArr.push_back(emp);	
	return true;
}
bool ServiceCtrlImpl::delEmp(int id)
{
	vector<Employee>::iterator it;
	for(it = empArr.begin();it!=empArr.end();it++)
	{
		if(it->getId() == id)
		{
			empArr.erase(it);
			return true;
		}
	}	
	return false;
}
bool ServiceCtrlImpl::modEmp(int dept_id,Employee& emp)
{
	Employee new_emp(emp.getName(),emp.getSex(),emp.getAge());
	for(size_t i =0;i <empArr.size();i++)
	{
		if(emp.getId() == empArr[i].getId())
		{
			delEmp(emp.getId());
			addEmp(dept_id,new_emp);
			return true;
		}
	}
	return false;
}
vector<Employee>&ServiceCtrlImpl::listEmp(int dept_id)
{
	return empArr;
}
vector<Department>& ServiceCtrlImpl::listAllEmp(void)
{
	return deptArr;
}
