#include <iostream>
#include <fstream>
#include <list>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

class Contact
{
	string name;
	string tel;
	char sex;
public:
	Contact(const string& name="",const string& tel="",char sex='\0'):name(name),tel(tel),sex(sex){}
	friend istream& operator>>(istream& is,Contact& con)
	{
		return is >> con.name >> con.tel >> con.sex;
	}
	friend ostream& operator<<(ostream& os,const Contact& con)
	{
		return os << con.name << " " << con.tel  << " " << con.sex;
	}

	const string& get_name(void)
	{
		return name;
	}

	const string& get_tel(void)
	{
		return tel;
	}
};

class Mail
{
	list<Contact> con_list;
	string path;

	char menu(char start,char end)
	{
		cout << "*****通讯录*****" << endl;
		cout << "1、添加	2、删除" << endl;
		cout << "3、修改	4、查询" << endl;
		cout << "5、显示	6、退出" << endl;
		cout << "请输入指令:";
		for(;;)
		{
			char cmd;
			cin >> cmd;
			if(cmd >= start && cmd <= end)
				return cmd;
		}
	}

	void add(void)
	{
		cout << "请输入联系人姓名、电话、性别:";
		Contact con;
		cin >> con;
		con_list.push_back(con);
	}
	void del(void)
	{
		string key;
		cout << "请输入要删除的联系人的关键字(name/tel):";
		cin >> key;
		list<Contact>::iterator it;
		for(it=con_list.begin();it!=con_list.end();it++)
		{
			if(it->get_name() == key || it->get_tel() == key)
			{
				con_list.erase(it);
				cout << "删除成功" << endl;
				sleep(2);
				return;
			}
		}
		cout << "删除失败" << endl;
		sleep(2);
	}
	void mod(void)
	{
		cout << __func__ << endl;
	}
	void find(void)
	{
		cout << __func__ << endl;
	}
	void show(void)
	{
		list<Contact>::iterator it;
		for(it=con_list.begin();it!=con_list.end();it++)
		{
			cout << *it << endl;
		}
		sleep(5);
	}

public:
	Mail(const string& path="con.dat"):path(path)	
	{
		//打开文件 读取数据
		ifstream ifs(path.c_str());
		if(!ifs)
		{
			cout << "文件打开失败" << endl;
		}
		else
		{
			for(;;)
			{
				//存入list
				Contact con;
				ifs >> con;
				if(!ifs.good())	break;
				con_list.push_back(con);
			}
			cout << "文件读取成功" << endl;
				//关闭文件
				ifs.close();
		}
	}
	~Mail(void)
	{
		//打开文件 保存数据
		ofstream ofs(path.c_str());
		if(!ofs)
		{
			cout << "文件保存失败" << endl;
		}
		else
		{
			//遍历list
			list<Contact>::iterator it;
			for(it=con_list.begin();it!=con_list.end();it++)
			{
				ofs << *it << endl;
			}
			cout << "数据保存成功" << endl;
			ofs.close();
		}
	}

	void start(void)
	{
		for(;;)
		{
			system("clear");
			switch(menu('1','6'))
			{
				case '1': add();	break;
				case '2': del();	break;
				case '3': mod();	break;
				case '4': find();	break;
				case '5': show();	break;
				case '6': return;
			}
		}
	}
};

int main(int argc,const char* argv[])
{
	Mail mail;
	mail.start();	
}






