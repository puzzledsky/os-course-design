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
const int ISIZE = 10;//保存inode的块数
const int DSIZE = 300;//保存数据的块数
const int MSIZE = 200;
const int BLOCKSIZE = 8;//一块的字节大小
const int BLOCKTOI = 16;//一块保存inode数量

/*
文件权限问题 用户组
内存显示
目录文件的保存 inode的保存
空闲表的显示
文件不存在 读写问题
用户登出时 login out
父目录和子目录同名
*/

class dir;
class inode;
class user;
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
			cerr << "内存已满" << endl;
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
private:
	int IFULL = 100;
	int DFULL = 100;

	int isize;  //inode区总块数
	int dsize;  //存储区总块数
	int ifree[100];  //inode空闲队列
	int dfree[100];  //存储区空闲队列
	int ninode;  //inode空闲队列长度
	int ndata;  //存储区空闲队列长度
public:
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
			cerr << "存储空间不足" << endl;
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
	int type;  //1:文件 2：目录
	int size;  //文件大小，字节
	int status; //状态，是否被分配
	int right; //权限，   
	int addr[8]; //直接参照
	int nwrite; //是否正在被写
	int nread; //正在被读的数量
	dir *pdir;

	inode() {
		size = 0;
		status = 0;
		nwrite = 0;
		nread = 0;
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
	void printData() {
		for (int i = 0; i < 8; i++) {
			if (addr[i] == -1)
				break;
			cout << addr[i] << "*" << BLOCK[ISIZE + addr[i]].data << endl;
		}
	}

};
extern inode INODE[BLOCKTOI * ISIZE+5];



class dir {
private:
    //获得空闲节点
	int getFree() {
		if (nsub == 100)
			return -1;
		int p = 2;
		while (p<nsub)
		{
			if (num[p] == 0)
				break;
			p++;
		}
		if (p == nsub)
			nsub++;
		return p;
	}

    //inode号为n对应的目录
	static dir* getDir(int n) {
		return INODE[n].pdir;
	}

    //返回父亲节点指针
	dir* getParent() {
		return getDir(num[0]);
	}
	int find(string s) {//查找文件或者目录，返回在目录表中的下标
		REM.push(num[1]);
		for (int i = 0; i < nsub; i++) {
			if (num[i] && name[i] == s) {
				return i;
			}
		}
		return -1;
	}
   
public:
	int num[100];
	string name[100];
	int nsub;

	dir(string s) {
		nsub = 2;
		name[0] = "NULL";
		num[0] = 0;
		name[1] = s;
		int di = sblock.i_get();
		num[1] = di;
		INODE[di].pdir = this;
		INODE[di].type = 2;
	}
	

    //在当前目录下添加目录x
	dir* addDir(string s) {//s:目录名
		
		if (this->find(s) != -1) {
			cerr << "命名冲突" << endl;
			return NULL;
		}
		dir* x = newDir(s);

		int p = getFree();
		name[p] = x->name[1];
		num[p] = x->num[1];
		x->num[0] = num[1];
		x->name[0] = name[1];
		return x;
	}
	void remove() {//删除目录自己，包括子目录和文件
		for (int i = 2; i < nsub; i++) {
			if (INODE[num[i]].type == 1) {
				sblock.i_put(num[i]);
			}
			else if (INODE[num[i]].type == 2) {
				getDir(num[i])->remove();
			}
		}
		if (num[0] != 0) {
			dir *p = getParent();
			int x = p->find(name[1]);
			p->num[x] = 0;
		}
		sblock.i_put(num[1]);
		this->~dir();
	}
	bool rename(string s) {//目录重命名
		if (s == name[1])
			return true;
		dir* p = getParent();
		int x1 = p->find(s);
		if (x1 == -1) {
			int x = p->find(name[1]);
			name[1] = s;
			p->name[x] = s;
			for (int i = 2; i < nsub; i++) {
				if (num[i]) {
					getDir(num[i])->name[0] = s;
				}
			}
			return true;
		}
		return false;
	}
	void print() {//输出目录表
		for (int i = 0; i < nsub; i++) {
			cout << name[i] << "  " << num[i] << endl;
		}
		cout << endl;
	}
	dir* in(string s) {//进入子目录，失败时留在当前目录
		int p = find(s);
		if (p != -1 && INODE[num[p]].type == 2) {
			REM.push(num[p]);
			return INODE[num[p]].pdir;
		}
		return this;
	}
	dir* out() {//返回上一级目录，失败时留在当前目录
		if (num[0] == 0)
			return this;
		REM.pop(num[0]);
		return INODE[num[0]].pdir;
	}
	
	
	int findFile(string s) {//根据文件名找文件，没找到则返回-1
		int p = find(s);     
        if (p!=-1 && INODE[num[p]].type != 1) {
			p = -1;
		}
		if (p != -1)
			REM.push(num[p]);
		return p;
	}
	inode* getFile(string s) {//在当前目录查找指定文件，返回inode，失败时返回NULL
		int p = findFile(s);
		if (p == -1)
			return NULL;
		return &INODE[p];
	}
	bool addFile(string s) {//s：文件名，添加文件
		if (find(s) != -1)// 命名冲突
			return false;
		int di = sblock.i_get();
		INODE[di].toFile();
		int p = getFree();
		name[p] = s;
		num[p] = di;
		return true;
	}
	
	/*权限还没写*/
	// ->openFile("1.txt","user",1)
	int openFile(string s, string user,int method) {//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
		int p = findFile(s);	                    //method 1：读  2：写
        cout<<"p--index:"<<p<<endl;
        if (p == -1)
			return -1;
		//return -2;
		if (INODE[num[p]].nwrite == 1)
			return 0;
		if (method == 1) {
			INODE[num[p]].nread++;
			return 1;
		}
		if (method == 2 && INODE[num[p]].nread==0) {
			INODE[num[p]].nwrite = 1;
			return 1;
		}
		return 0;
	}
	//->closeFile("1.txt",1)
	void closeFlie(string s,int method) {//method 1：读  2：写
		int p = findFile(s);            
		if (p == -1)
			return;
		if (method == 1) {
			if(INODE[num[p]].nread>0)
				INODE[num[p]].nread--;
		}
		if (method == 2) {
			INODE[num[p]].nwrite = 0;
		}
	}
	
	bool removeFile(string s) {//s：文件名，删除文件
		int p = findFile(s);
		if (p == -1)
			return false;
		sblock.i_put(num[p]);
		num[p] = 0;
		return true;
	}
	bool renameFile(string s,string name1) {//s:文件名，name1：新文件名， 文件重命名
		int p = findFile(s);
		int p1 = find(name1);
		if (p1 != -1)
			return false;
		name[p] = name1;
	}
	string readFile(string s) {//  返回文件全部内容
		int p = findFile(s);
		return INODE[num[p]].getData();
	}
	void writeFile(string s,string str) {//s：文件名，str：新的内容（全部） 重写文件 
		int p = findFile(s);
		INODE[num[p]].setData(str);
	}

    string getName(){
        return name[1];
    }
};

class user {
public:
	string name;
	string password;
	dir* pdir;
	int status; // 0:离线 1:在线
};
extern vector<user> USER;
extern dir* ROOT;
extern dir* HOME;

/*
Users::addUser("1", "abc");
Users::loginIn("1", "abc");
Users::loginOut("1");
*/
class Users{
public:
	static int findUser(string n) {
		for (int i = 0; i < USER.size(); i++) {
			if (USER[i].name == n)
				return i;
		}
		return -1;
	}
	static bool addUser(string n,string pas) {
		if (findUser(n) != -1) {
			cerr << "用户名冲突" << endl;
			return false;
		}
		user *p = new user;
		p->name = n;
		p->password = pas;
		p->status = 0;

		HOME->addDir(n);
		USER.push_back(*p);
        return true;
	}
	static int loginIn(string n, string pas) {
		int x = findUser(n);
		if (x == -1) {
			cerr << "用户不存在" << endl;
			return -1;
		}
		else if (USER[x].status == 1) {
			cerr << "用户已登录" << endl;
			return 0;
		}
		else if (USER[x].password != pas) {
			cerr << "密码错误" << endl;
			return -2;
		}
		else {//成功
			USER[x].status = 1;
			return 1; 
		}
	}
	static void loginOut(string n){
		int x = findUser(n);
		USER[x].status = 0;
	}
	static bool removeUser(string n);
};

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
