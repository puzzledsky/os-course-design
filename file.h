#pragma once
#ifndef FILE_H
#define FILE_H

#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int SIZE = 210;
const int ISIZE = 5;
const int DSIZE = 200;
const int BLOCKSIZE = 8;
const int BLOCKTOI = 2;
/*
用户问题
文件权限问题
删除目录没删子目录和对象
*/

class dir;
class inode;

class block {
public:
	string data = "";//大小为512
};
extern bool B_FLAG[SIZE];//是否已使用
extern block BLOCK[SIZE];

class filsys {//超级块
private:
	static const int IFULL = 50;
	static const int DFULL = 100;

	int isize;  //inode区总块数
	int dsize;  //存储区总块数
	int ifree[IFULL];  //inode空闲队列
	int dfree[DFULL];  //存储区空闲队列
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
	dir *pdir;

	inode() {
		size = 0;
		status = 0;
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
	void reset() {

	}
	string getData() {
		data = "";
		for (int i = 0; i < 8; i++) {
			if (addr[i] == -1)
				break;
			data.append(BLOCK[ISIZE + addr[i]].data);
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
extern inode INODE[BLOCKTOI * ISIZE];

class memory {
	
};
extern memory mem;

class dir {
private:
	int num[100];
	string name[100];
	int nsub;

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
public:
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
	static dir* getDir(int n) {
		return INODE[n].pdir;
	}
	dir* getParent() {
		return getDir(num[0]);
	}
	int find(string s) {//查找文件或者目录，返回在目录表中的下标
		for (int i = 0; i < nsub; i++) {
			if (num[i] && name[i] == s) {
				return i;
			}
		}
		return -1;
	}
	void addDir(dir& x) {//命名冲突？
		int p = getFree();
		name[p] = x.name[1];
		num[p] = x.num[1];
		x.num[0] = num[1];
		x.name[0] = name[1];
	}
	bool removeDir(string s) {//还有子目录的问题
		int x = find(s);
		if (x != -1 && INODE[x].type==1) {
			num[x] = 0;
			return true;
		}
		return false;
	}
	bool rename(string s) {
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
	void print() {
		for (int i = 0; i < nsub; i++) {
			//if(num[i]||i<2)
			cout << name[i] << "  " << num[i] << endl;
		}
		cout << endl;
	}
	dir* in(string s) {
		int p = find(s);
		if (p != -1 && INODE[num[p]].type == 2)
			return INODE[num[p]].pdir;
		return this;
	}
	dir* out() {
		if (num[0] == 0)
			return this;
		return INODE[num[0]].pdir;
	}
	
	int findFile(string s) {
		int p = find(s);
		if (INODE[num[p]].type != 1) {
			p = -1;
		}
		return p;
	}
	bool addFile(string s) {
		if (find(s) != -1)// 命名冲突
			return false;
		int di = sblock.i_get();
		INODE[di].toFile();
		int p = getFree();
		name[p] = s;
		num[p] = di;
		return true;
	}
	bool removeFile(string s) {
		int p = findFile(s);
		if (p == -1)
			return false;
		INODE[num[p]].reset();
		num[p] = 0;
		return true;
	}
	bool renameFile(string s,string name1) {
		int p = findFile(s);
		int p1 = find(name1);
		if (p1 != -1)
			return false;
		name[p] = name1;
	}
	string readFile(string s) {
		int p = findFile(s);
		return INODE[num[p]].getData();
	}
	void writeFile(string s,string str) {
		int p = findFile(s);
		INODE[num[p]].setData(str);
	}

	inode* getInode(string s) {
		int p = findFile(s);
		if (p == -1)
			return NULL;
		return &INODE[num[p]];
	}
};

class print {
public:
	static void block() {
		cout << "block" << endl;
		for (int i = 0; i < SIZE; i++) {
			if (i == ISIZE+1) {
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
			if(INODE[i].status){
				cout << i << endl;
				for (int j = 0; j < 8; j++)
					if (INODE[i].addr[j] != -1)
						cout <<"    addr "<<j<<" : "<< INODE[i].addr[j] << " ";
				cout << endl;
			}
		}
	}
	static void all() {
		block();
		inode();
		cout << "------------------" << endl;
	}
};
#endif
