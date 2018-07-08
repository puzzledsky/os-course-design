#include"file.h"
/*全局变量*/

bool B_FLAG[SIZE];//磁盘块是否被使用
block BLOCK[SIZE];//磁盘块
inode INODE[BLOCKTOI * ISIZE];//磁盘中INODE区域
filsys sblock;//超级块，负责inode和数据块的分配回收
memory REM;//内存块
vector<user> USER;//保存所有用户信息
dir* ROOT;//根目录
dir* HOME;// Root\Home\用户目录

dir* newDir(string s) {
	dir* p = new dir(s);
	return p;
}
/*filsys定义*/
filsys::filsys() {
	isize = ISIZE * BLOCKTOI;
	dsize = DSIZE;
	ninode = 0;
	ndata = 0;

	if (IFULL > BLOCKTOI * ISIZE) {
		IFULL = BLOCKTOI * ISIZE / 2;
	}
	init();
}
void filsys::init() {
	for (int i = 0; i < SIZE; i++) {
		B_FLAG[i] = false;
	}
	ROOT = newDir("root");
	HOME = ROOT->addDir("home");
}
void filsys::i_setFree() {
	for (int i = 1; i < isize; i++) {
		if (INODE[i].status == 0) {
			ifree[ninode++] = i;
			if (ninode == IFULL)
				break;
		}
	}
}
void filsys::d_setFree() {
	for (int i = ISIZE + 1; i < SIZE; i++) {
		if (!B_FLAG[i]) {
			dfree[ndata++] = i;
			if (ndata == DFULL)
				break;
		}
	}
}
int filsys::i_get() {
	if (ninode == 0) {
		i_setFree();
		if (ninode == 0) {
			cerr << "inode区空间不足" << endl;
			return -1;
		}
	}
	ninode--;
	int di = ifree[ninode];
	int p = (di-1) / BLOCKTOI + 1;
	if (!B_FLAG[p]) {
		B_FLAG[p] = true;
	}
	INODE[di].status = 1;
	INODE[di].addr[0] = d_get();
	return di;
}
int filsys::d_get() {
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
int filsys::i_put(int di) {
	if (ninode < IFULL) {
		ifree[ninode] = di;
		ninode++;
	}
	INODE[di].status = 0;
	for (int i = 0; i < 8; i++) {
		if (INODE[di].addr[i] != -1) {
			d_put(INODE[di].addr[i]);
			INODE[di].addr[i] = -1;
		}
	}

	bool empty = true;
	int istart = di - ((di-1) % BLOCKTOI);
	for (int i = istart; i < istart + BLOCKTOI; i++) {
		if(INODE[i].status!=0){
			empty = false;
			break;
		}
	}
	if (empty) {
		int p = (di-1) / BLOCKTOI + 1;
		B_FLAG[p] = false;
	}
	return 0;
}
int filsys::d_put(int di) {
	if (ndata < DFULL) {
		dfree[ndata] = di;
		ndata++;
	}
	B_FLAG[di] = false;
	return 0;
}