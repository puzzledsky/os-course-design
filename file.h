#pragma once
#ifndef FILE_H
#define FILE_H

#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int SIZE = 210;
const int ISIZE = 10;
const int DSIZE = 200;
/*
用户问题
文件权限问题
删除目录没删子目录和对象
*/
class dir;
class filsys;
class block {
public:
	string data = "";//大小为512
};
extern bool B_FLAG[SIZE];//是否已使用
extern block BLOCK[SIZE];

class inode {
private:
	string data;
	void write() {
		int i = 0;
		for (; i < 8; i++) {
			if (data.length() >(i + 1) * 512) {
				//cout << "* " << i << " " << data.substr(i * 512, 512) << endl;
				BLOCK[ISIZE + addr[i]].data = data.substr(i * 512, 512);
			}
			else {
				//cout << "* " << i << " " << data.substr(i * 512) << endl;
				BLOCK[ISIZE + addr[i]].data = data.substr(i * 512);
				break;
			}
		}
		while (i < 7) {
			i++;
			BLOCK[ISIZE + addr[i]].data = "";
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
			data.append(BLOCK[ISIZE + addr[i]].data);
		}
		return data;
	}
	void setData(string s) {
		data = s;
		size = s.length();
		write();
	}
};
extern inode INODE[16 * ISIZE];

class filsys {//超级块
private:
	int isize;  //inode区总块数
	int dsize;  //存储区总块数
	int ifree[100];  //inode空闲队列
	int dfree[100];  //存储区空闲队列
	int ninode;  //inode空闲队列长度
	int ndata;  //存储区空闲队列长度
	//bool ilock; //inode队列锁
	//bool dlock; //存储区队列锁
public:
	filsys() {
		isize = ISIZE * 16;
		dsize = DSIZE;
		ninode = 0;
		ndata = 0;
		//ilock = false;
		//dlock = false;
	}
    void init(){
        for (int i = 0; i < SIZE; i++) {
            B_FLAG[i] = false;
        }
    }
	void i_setFree() {
		for (int i = 1; i < isize; i++) {
			if (INODE[i].status == 0) {
				ifree[ninode++] = i;
				if (ninode == 50)
					break;
			}
		}
	}
	void d_setFree() {
		for (int i = 1; i < dsize; i++) {
			if (!B_FLAG[ISIZE + i]) {
				dfree[ndata++] = i;
				if (ndata == 100)
					break;
			}
		}
	}
	int i_get() {
		if (ninode == 0) {
			i_setFree();
			if (ninode == 0) {
				cerr << "inode区空间不足" << endl;
				return -1;
			}
		}
		ninode--;
		int di = ifree[ninode];
		INODE[di].status = 1;
		for (int i = 0; i < 8; i++) {
			INODE[di].addr[i] = d_get();
		}
		return di;
	}
	int d_get() {
		if (ndata == 0) {
			d_setFree();
			if (ndata == 0) {
				cerr << "存储区空间不足" << endl;
				return -1;
			}
		}
		ndata--;
		int di = dfree[ndata];
		B_FLAG[di] = true;
		return di;
	}
};
extern filsys sblock;

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

	int find(string s) {
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
};

#endif
