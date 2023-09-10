#include "tools.h"
#include "emis.h"
#include <getch.h>

//  从键盘获取指定长度的字符串
char* get_str(char* str,size_t hope_len)
{
    if(NULL == str || 1 >= hope_len) return NULL;
    
    //  清理输入缓冲区
    stdin->_IO_read_ptr = stdin->_IO_read_end;

    size_t cnt = 0;
    while(cnt < hope_len-1)
    {   
        char key_val = getch();
        if(10 == key_val) break;
        if(127 == key_val)
        {
            if(0 < cnt)
            {
                cnt--;
                printf("\b \b");
            }
            continue;
        }
        printf("%c",key_val);
        str[cnt++] = key_val;
    }
    str[cnt] = '\0';
    printf("\n");
    return str;
}

//  从键盘获取密码
char* get_pass(char* str,size_t hope_len,bool is_show)
{
    if(NULL == str || 1 >= hope_len) return NULL;

    //  清理输入缓冲区
    stdin->_IO_read_ptr = stdin->_IO_read_end;

    size_t cnt = 0;
    while(cnt < hope_len-1)
    {
        char key_val = getch();
        //这个表示回车建
        if(10 == key_val) break;
        //这个表示退格键
        if(127 == key_val)
        {
            if(0 < cnt)
            {
                cnt--;
                if(is_show) printf("\b \b");
            }
            continue;
        }
        if(is_show) printf("*");
        str[cnt++] = key_val;
    }
    str[cnt] = '\0';
    printf("\n");
    return str;
}

//按任意键继续
void anykey_continue(void)
{
    stdin->_IO_read_ptr = stdin->_IO_read_end;
    cout << "请按任意键继续......" << endl;
    getch();
}

int get_cmd(char start,char end)
{
    puts("请输入指令:");
    for(;;)
    {   
        stdin->_IO_read_ptr = stdin->_IO_read_end;
        char cmd = getch();
        if(start <= cmd && cmd <= end)
        {
            cout << cmd << endl;
            return cmd;
        }
    }   
}

//读取ID配置文件
void read_ids(int& mgrid,int& deptid,vector<int>& empid)
{
    ifstream ifs(DATA_ID_PATH);
    string line;
    if(!ifs)
    {
        cout << "ID配置文件读取失败" << endl;
        mgrid = 1000;
        deptid = 100;
        empid = {1100};
    }
    getline(ifs,line);
    mgrid = stoi(line);
    getline(ifs,line);
    deptid = stoi(line);
    getline(ifs,line);
    istringstream iss(line);
    int emp_id;
    empid.reserve(100);
    while(iss >> emp_id)
    {
        empid.push_back(emp_id);
    }
    ifs.close();
}
//写入更新ID配置文件
void write_new_ids(int& mgrid,int& deptid,vector<int>& empid)
{
    ofstream ofs(DATA_ID_PATH);

    if(!ofs)
    {
        cout << "ID配置文件更新失败" << endl;
    }
    ofs << mgrid << endl << deptid << endl;
    vector<int>::const_iterator its;
    for(its = empid.begin();its!=empid.end();its++)
    {
        ofs << *its << ' ';
    }
    ofs.close();
}

//生成唯一的管理员ID
int get_mgrid(void)
{
    int mgrid,deptid;
    vector<int> empid;
    read_ids(mgrid,deptid,empid);
    mgrid++;
    write_new_ids(mgrid,deptid,empid);
    return mgrid;
}

//生成唯一的部门ID
int get_deptid(void)
{
    int mgrid,deptid;
    vector<int> empid;
    read_ids(mgrid,deptid,empid);
    deptid = deptid+10;
    write_new_ids(mgrid,deptid,empid);
    return deptid;
}

//生成唯一的员工ID,需要提供是什么部门的
int get_empid(int deptid_now)
{
    int mgrid,deptid;
    int new_empid=0;
    vector<int> empid;
    read_ids(mgrid,deptid,empid);
    vector<int>::iterator its;
    bool flag = false;

    for(its=empid.begin();its!=empid.end();its++)
    {
        if((*its)/10 == deptid_now && deptid_now <= deptid)
        {
            *its = *its+1;
            flag = true;
            new_empid = *its;
        }
    }

    if(!flag && deptid_now <= deptid)
    {
        new_empid = deptid*10+1;
        empid.push_back(new_empid);
    }
    write_new_ids(mgrid,deptid,empid);
    return new_empid;
}


                                           


