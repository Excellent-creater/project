#include "manager_view_impl.h"
#include "manager_ctrl_impl.h"

ManagerViewImpl::ManagerViewImpl(void)
{
	mgrCtrl = new ManagerCtrlImpl;
	srcView = new ServiceViewImpl;
}

ManagerViewImpl::~ManagerViewImpl(void)
{
	delete mgrCtrl;
	delete srcView;
}

void ManagerViewImpl::loginManager(void)
{
	char id[20]={},pwd[20] = {};
	cout << "请输入超级用户ID:";
	get_str(id,20);
	cout << "请输入超级用户密码:";
	get_pass(pwd,20,true);
	
	if(!strcmp(id,"root") && !strcmp(pwd,"1234"))
	{
		menuMgr();
		return;	
	}
	cout << "超级用户ID或密码错误，请检查!" << endl;
	cout << endl;
	anykey_continue();
}

void ManagerViewImpl::loginService(void)
{
	int id;
	char pwd[20] = {};
	cout << "请输入管理员ID:";
	cin >> id;
	cout << "请输入管理员密码:";
	get_pass(pwd,20,true);
	
	vector<Manager>& list_mgr = mgrCtrl->listMgr();
	
	for(auto its = list_mgr.begin();its!=list_mgr.end();its++)
	{
		if(id == its->getId() && 0 == strcmp(its->getPwd(),pwd))
		{
			srcView->menuSrc();
			return ;
		}
	} 
	cout << "管理员ID或密码错误,请检查!" << endl;
	cout << endl;
	anykey_continue();
}

void ManagerViewImpl::menuMgr(void)
{
	for(;;)
	{
		system("clear");
		cout << "**********欢迎进入管理子系统**********" << endl;
		cout << "1、添加管理员" << endl;
		cout << "2、删除管理员" << endl;
		cout << "3、列出管理员" << endl;
		cout << "4、退出子系统" << endl;
		
		switch(get_cmd('1','4'))
		{
			case '1':	addMgr();	break;
			case '2':	delMgr();	break;
			case '3':	listMgr();	break;
			case '4':	return;
		}
	}
}

void ManagerViewImpl::addMgr(void)
{
	char name[20] ={},pwd[20] = {},repwd[20] = {};
	cout << "请输入管理员用户名:";
	get_str(name,20);
	cout << "请输入管理员密码:";
	get_pass(pwd,20,true);
	cout << "请再次输入管理员密码:";
	get_pass(repwd,20,true);
	if(strcmp(pwd,repwd))
	{
		cout << "两次密码不同，添加失败！" << endl;
		cout << endl;
		anykey_continue();
		return;	
	}
	
	Manager mgr(name,pwd);
	
	if(mgrCtrl->addMgr(mgr))
		cout << "添加管理员成功" << endl;
	else
		cout << "管理员用户名重名，添加失败" << endl;
	cout << endl;
	anykey_continue();
}

void ManagerViewImpl::delMgr(void)
{
	int id = 0;
	cout << "请输入要删除的管理员id:";
	cin >> id;
	if(mgrCtrl->delMgr(id))
		cout << "删除管理员成功" << endl;
	else
		cout << "id有误，删除管理员失败" << endl;
	cout << endl;
	anykey_continue();
}

void ManagerViewImpl::listMgr(void)
{
	vector<Manager>& mgrArr = mgrCtrl->listMgr();
	for(unsigned int i=0; i<mgrArr.size(); i++)
	{
		cout << mgrArr[i] << endl;	
	}
	cout << endl;
	anykey_continue();
}















