#include "service_view_impl.h"	

ServiceViewImpl::ServiceViewImpl(void)
{
	srcCtrl = new ServiceCtrlImpl;
}
	
ServiceViewImpl::~ServiceViewImpl(void)
{
	delete srcCtrl;
}
void ServiceViewImpl::menuSrc(void)
{
	for(;;)
	{
		system("clear");
		cout << "**********欢迎进入业务子系统**********" << endl;
		cout << "1、增加部门		2、删除部门" << endl;
		cout << "3、增加成员		4、删除成员" << endl;
		cout << "5、生成部门列表		6、生成成员列表" << endl;
		cout << "7、退出子系统" << endl;
		
		switch(get_cmd('1','8'))
		{
			case '1':	addDept();		break;
			case '2':	delDept();		break;
			case '3':	addEmp();		break;
			case '4':	delEmp();		break;
			case '5':	listDept();		break;
			case '6':	listEmp();		break;
			case '7':	return;
		}
	}
}
void ServiceViewImpl::addDept(void)
{
	char name[20] = {};
	cout << "请输入部门名:";
	cin >> name;
	Department dep;
	dep.setName(name);
	if(srcCtrl->addDept(dep))
		cout << "增加部门成功!" << endl;
	else
		cout << "增加部门失败!" << endl;
	cout << endl;
	anykey_continue();
}
void ServiceViewImpl::delDept(void)
{
	int id;
	cout << "请输入要删除的部门ID:";
	cin >> id;
	if(1 == srcCtrl->delDept(id))
		cout << "删除部门成功!" << endl;
	else
		cout << "删除部门失败!" << endl;
	cout << endl;
	anykey_continue();
}
void ServiceViewImpl::listDept(void)
{
	vector<Department>& dept = srcCtrl->listDept();
	for(unsigned int i =0;i<dept.size();i++)
	{
		cout << dept[i] << endl;
		cout << "------------" << endl;
	}
	cout << endl;
	anykey_continue();
}
void ServiceViewImpl::addEmp(void)
{
	char name[20] = {},sex;
	int age,dept_id;
	cout << "请输入部门ID:";
	cin >> dept_id;
	cout << "请输入成员姓名:";
	get_str(name,20);
	cout << "请输入成员性别(w/m):";
	cin >> sex;
	cout << "请输入成员年龄:";
	cin >> age;
	
	Employee emp(name,sex,age);

	if(srcCtrl->addEmp(dept_id,emp))
		cout << "添加成员成功!" << endl;
	else
		cout << "添加成员失败!" << endl;
	cout << endl;
	anykey_continue();

}
void ServiceViewImpl::delEmp(void)
{
	int id = 0;
	cout << "请输入要删除的成员id:";
	cin >> id;
	if(srcCtrl->delEmp(id))
		cout << "删除成员成功" << endl;
	else
		cout << "id有误,删除管理员失败" << endl;
	cout << endl;
	anykey_continue();
}
void ServiceViewImpl::modEmp(void)
{
	int dept_id,id,new_id,age;
	char name[20] = {},sex;
	cout << "请输入要修改成员的部门ID:";
	cin >> dept_id;
	cout << "请输入要修改成员的ID:";
	cin >> id;
	cout << "请重新输入修改成员的姓名:";
	get_str(name,20);
	cout << "请重新输入修改成员的年龄";
	cin >> age;
	cout << "请重新输入修改成员的性别:";
	cin >> sex;
	new_id = id;
	Employee emp (name,sex,age);
	emp.setId(new_id);
	if(srcCtrl->modEmp(dept_id,emp))
	{
		cout << "修改成员成功!" << endl;
	}else{
		cout << "修改成员失败!" << endl;
	}
	
	cout << endl;
	anykey_continue();
}
void ServiceViewImpl::listEmp(void)
{
	vector<Department> dept = srcCtrl->listDept();
	for(size_t i = 0;i< dept.size();i++)
	{
		vector<Employee> emp = srcCtrl->listEmp(dept[i].getId());
		cout << dept[i] << endl;;
		for(auto its = emp.begin();its!=emp.end();its++)
		{
			if((its->getId())/10 == dept[i].getId())
			{
				cout << *its << endl;
			}	
		}
		cout << "------------" << endl;
	}
	cout << endl;
	anykey_continue();
}

