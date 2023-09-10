#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>


using namespace std;

//  从键盘获取指定长度的字符串
char* get_str(char* str,size_t hope_len);

//  从键盘获取密码
char* get_pass(char* str,size_t hope_len,bool is_show);

void anykey_continue(void);
int get_cmd(char start,char end);
void read_ids(int& mgrid,int& deptid,vector<int>& empid);
void write_new_ids(int& mgrid,int& deptid,vector<int>& empid);
int get_mgrid(void);
int get_deptid(void);
int get_empid(int deptid_now);




#endif
