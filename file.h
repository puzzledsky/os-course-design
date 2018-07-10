#pragma once
#ifndef FILE_H
#define FILE_H

#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<set>

using namespace std;
const int SIZE = 310;//总块数
const int ISIZE = 14;//保存inode的块数
const int DSIZE = 300;//保存数据的块数
const int MSIZE = 144;
const int BLOCKSIZE = 128;//一块的字节大小
const int BLOCKTOI = 16;//一块保存inode数量
/*
Users::addUser("1", "abc");
Users::loginIn("1", "abc");
Users::loginOut("1");
*/

/*
-----------文件权限问题 
-----用户组
----------------------内存显示
目录文件的保存 inode的保存
空闲表的显示
-----文件不存在 读写问题
---------------------用户登出时 login out
父目录和子目录同名
更新时间 
文件名 数字1
*/

class inode;
class user;
extern int UID;
class dir {
private:
	int getFree();//获得空闲节点    
	static dir* getDir(int n);//inode号为n对应的目录
	dir* getParent();//返回父亲节点指针
	int find(string s, int start=0);
	bool haveRight(string user, string name, int method);
	void update();
public:
	int num[100];
	string name[100];
	int nsub;

	dir(string s);

	dir* addDir(string s);//在当前目录下添加目录x
	void remove();
	bool rename(string s);
	void print();
	dir* in(string s);
	dir* out();

	int findFile(string s);
	inode* getFile(string s);
	bool addFile(string s);
	void setUser(string s, string user);
	string getUser(string s);
	void setRight(string s, int right);
	int getRight(string s);
	int openFile(string s, string user, int method);
	bool openDir(string s, string user);
	void closeFlie(string s, int method);
	string lsPrint();
	bool removeFile(string s);
	bool renameFile(string s, string name1);
	string readFile(string s);
	void writeFile(string s, string str);

	string getName();
	int fileNum();
};
class Users {
public:
	static int findUser(string n);
	static bool addUser(string n, string pas);
	static int loginIn(string n, string pas);
	static void loginOut(string n);
	static int getUid(string n);
	static bool removeUser(string n);
    static void setGroup(string user,string group="group0");//group格式： group0,group1,... 默认都在group0
	static int getGid(string n);
	static string getName(int uid);
};
extern dir* newDir(string s);

class block {
public:
	string data = "";//大小为512
};
extern bool B_FLAG[SIZE+5];//是否已使用
extern block BLOCK[SIZE+5];

class memory {
public:
	int num[MSIZE];
	bool flag[MSIZE];//正在被使用
	memory() {
		for (int i = 0; i < MSIZE; i++)
			flag[i] = false;
		for (int i = 0; i < MSIZE; i++)
			num[i] = 0;
	}
	void push(int x) {
		for (int i = 0; i < MSIZE; i++) {
			if (num[i] == x) {
				flag[i] = 1;
				return;
			}
		}

		int p = -1;
		for (int i = 0; i < MSIZE; i++) {
			if (num[i] == 0) {
				p = i;
				break;
			}
		}
		if (p == -1) {
			for (int i = 0; i < MSIZE; i++) {
				if (flag[i] == false) {
					p = i;
					break;
				}
			}
		}
		if (p == -1) {
			cerr << "memory is full" << endl;
			return;
		}
		num[p] = x;
		flag[p] = true;
	}
	void pop(int x) {
		for (int i = 0; i < MSIZE; i++) {
			if (num[i] == x && flag[i] == 1) {
				flag[i] = 0;
				break;
			}
		}
	}
	void print() {
		for (int i = 0; i < MSIZE; i++) {
			if (num[i] || flag[i])
				cout << num[i] << "  " << flag[i] << endl;
		}
		cout << endl;
	}
};
extern memory REM;

class filsys {//超级块
public:
    int IFULL = 100;
    int DFULL = 100;
    int isize;  //inode区总块数
    int dsize;  //存储区总块数
    int ifree[100];  //inode空闲队列
    int dfree[100];  //存储区空闲队列
    int ninode;  //inode空闲队列长度
    int ndata;  //存储区空闲队列长度

    filsys();
	void init();
	void i_setFree();
	void d_setFree();
	int i_get();
	int d_get();
	int i_put(int);
	int d_put(int);
};
extern filsys sblock;

class inode {
private:
	string data;
	void write() {
		if (data.length() > 8 * BLOCKSIZE) {
			cerr << "space is full" << endl;
			return;
		}
		int i = 0;
		for (; i < 8; i++) {
			if (addr[i] == -1) {
				addr[i] = sblock.d_get();
			}
			if (data.length() >(i + 1) * BLOCKSIZE) {
				BLOCK[addr[i]].data = data.substr(i * BLOCKSIZE, BLOCKSIZE);
			}
			else {
				BLOCK[addr[i]].data = data.substr(i * BLOCKSIZE);
				break;
			}
		}
		i++; 
		for (; i < 8 && addr[i] != -1; i++) {
			sblock.d_put(addr[i]);
			addr[i] = -1;
		}
	}
public:
    int type;  //1:文件 2：目录  -1：未知
	int size;  //文件大小，字节
	int status; //状态，是否被分配
	int right; //权限   
	int addr[8]; //直接参照
	int nwrite; //是否正在被写
	int nread; //正在被读的数量
	int uid;//用户id
	int gid;//组id
	dir *pdir;

	inode() {
		size = 0;
		status = 0;
		nwrite = 0;
		nread = 0;
		right = 777;
		uid = 0;
		gid = 0;
		for (int i = 0; i < 8; i++) {
			addr[i] = -1;
		}
	}
	void toFile() {
		type = 1;
		size = 0;
		status = 1;
		data = "";
		write();
	}
	string getData() {
		data = "";
		for (int i = 0; i < 8; i++) {
			if (addr[i] == -1)
				break;
			data.append(BLOCK[addr[i]].data);
		}
		return data;
	}
	void setData(string s) {
		data = s;
		size = s.length();
		write();
	}
	void setDirData() {
		if (type != 2)
			return;
		string data = "";
		for (int i = 0; i < pdir->nsub; i++) {
			data.append(to_string(pdir->num[i]));
			data.append(" ");
			data.append(pdir->name[i]);
			data.append("\n");
		}
		setData(data);
	}
	void printData() {
		for (int i = 0; i < 8; i++) {
			if (addr[i] == -1)
				break;
			cout << addr[i] << "*" << BLOCK[ISIZE + addr[i]].data << endl;
		}
	}
	bool getRight(int x){// rwx rwx rwx |x: 987 654 321 | 自己 用户组 其他 
		int r[4];
		r[3] = right / 100;
		r[2] = right / 10 % 10;
		r[1] = right % 10;
		int p = (x - 1) / 3 + 1;
		int i = x % 3;
		switch (i)
		{
		case 1:
			return r[p] % 2 == 1;
		case 2:
			return r[p] / 2 % 2 == 1;
		case 0:
			return r[p] / 4 % 2 == 1;
		}
	}
	void setRight(int m) {
		right = m;
	}
};
extern inode INODE[BLOCKTOI * ISIZE+5];



class user {
public:
	string name;
	string password;
	dir* pdir;
	int uid;
	int gid;
	int status; // 0:离线 1:在线
};
extern vector<user> USER;
extern dir* ROOT;
extern dir* HOME;


class print {
public:
	static void block() {
		cout << "block" << endl;
		for (int i = 0; i < SIZE; i++) {
			if (i == ISIZE + 1) {
				cout << endl;
			}
			if (B_FLAG[i]) {
				cout << i << "  --  ";
				cout << BLOCK[i].data << endl;
			}
		}
		cout << endl;
	}
	static void inode() {
		cout << "inode:" << endl;
		for (int i = 0; i < ISIZE * BLOCKTOI; i++) {
			if (INODE[i].status) {
				cout << i << endl;
				for (int j = 0; j < 8; j++)
					if (INODE[i].addr[j] != -1)
						cout << "    addr " << j << " : " << INODE[i].addr[j] << " ";
				cout << endl;
			}
		}
	}
	static void all() {
		block();
		inode();
		cout << "------------------" << endl;
	}
	static void users() {
		cout << "user:" << endl;
		for (int i = 0; i < USER.size(); i++) {
			cout << USER[i].name << "  " << USER[i].status << endl;
		}
	}
};
#endif
