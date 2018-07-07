#include"file.h"
/*全局变量*/
bool B_FLAG[SIZE];
block BLOCK[SIZE];
inode INODE[16 * ISIZE];
filsys sblock;


/*filsys定义*/
filsys::filsys() {
	isize = ISIZE * 16;
	dsize = DSIZE;
	ninode = 0;
	ndata = 0;
	//ilock = false;
	//dlock = false;
}
void filsys::init() {
	for (int i = 0; i < SIZE; i++) {
		B_FLAG[i] = false;
	}
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
	for (int i = 1; i < dsize; i++) {
		if (!B_FLAG[ISIZE + i]) {
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
		INODE[di].addr[i] = -1;
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