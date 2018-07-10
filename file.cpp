#include"file.h"
/*全局变量*/

bool B_FLAG[SIZE+5];//磁盘块是否被使用
block BLOCK[SIZE+5];//磁盘块
inode INODE[BLOCKTOI * ISIZE+5];//磁盘中INODE区域
memory REM;//内存块
filsys sblock;//超级块，负责inode和数据块的分配回收
vector<user> USER;//保存所有用户信息
dir* ROOT;//根目录
dir* HOME;// Root\Home\用户目录
int UID;

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
	//ROOT = newDir("root");
	//HOME = ROOT->addDir("home");
	UID = 100;
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
			cerr << "inode space is full" << endl;
			return -1;
		}
	}
	ninode--;
	int di = ifree[ninode];
	int p = (di-1) / BLOCKTOI + 1;
	if (!B_FLAG[p]) {
		B_FLAG[p] = true;
	}
	REM.push(di);
	INODE[di].status = 1;
	INODE[di].addr[0] = d_get();
	return di;
}
int filsys::d_get() {
	if (ndata == 0) {
		d_setFree();
		if (ndata == 0) {
			cerr << "space is full" << endl;
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
	REM.pop(di);
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

/*User定义*/
int Users::findUser(string n) {
	for (int i = 0; i < USER.size(); i++) {
		if (USER[i].name == n)
			return i;
	}
	return -1;
}
bool Users::addUser(string n, string pas) {
	if (findUser(n) != -1) {
        cerr << "user"+ n +" is exist" << endl;
		return false;
	}
	user *p = new user;
	p->name = n;
	p->password = pas;
	p->status = 0;
    if(n=="root")
        p->uid=0;
    else
        p->uid = UID;
    p->gid = 0;
	UID++;

	p->pdir = HOME->addDir(n);

    //HOME->setRight(n,700);/*想设置自己目录私有*/
	USER.push_back(*p);
	return true;
}
int Users::loginIn(string n, string pas) {
	int x = findUser(n);
	if (x == -1) {
		cerr << "user is not exist" << endl;
		return -1;
	}
	else if (USER[x].status == 1) {
		cerr << "user is online" << endl;
		return 0;
	}
	else if (USER[x].password != pas) {
		cerr << "password is error" << endl;
		return -2;
	}
	else {//成功
		USER[x].status = 1;
		REM.push(USER[x].pdir->num[1]);
		return 1;
	}
}
void Users::loginOut(string n) {
	int x = findUser(n);
	if (x == -1)
		return;
	USER[x].status = 0;
	REM.pop(USER[x].pdir->num[1]);
}
int Users::getUid(string n) {
	int p = findUser(n);
	if (p == -1)
		return -1;
	return USER[p].uid;
}
void Users::setGroup(string user, string group) {
	int p = findUser(user);
	if (p == -1)
		return;
	if (group.length()>5)
		USER[p].gid = group[5]-'0';
}
int Users::getGid(string n) {
	int p = findUser(n);
	if (p == -1)
		return 0;
	return USER[p].gid;
}
string Users::getName(int uid) {
	if (uid == 0)
		return "root";
	for (int i = 0; i < USER.size(); i++) {
		if (USER[i].uid == uid)
			return USER[i].name;
	}
	return "";
}

/*dir定义*/
//private
int dir::getFree() {
	if (nsub == 100)
		return -1;
	int p = 2;
	while (p < nsub)
	{
		if (num[p] == 0)
			break;
		p++;
	}
	if (p == nsub)
		nsub++;
	return p;
}
dir* dir::getDir(int n) {
	return INODE[n].pdir;
}
dir* dir::getParent() {
	return getDir(num[0]);
}
int dir::find(string s,int start) {//查找文件或者目录，返回在目录表中的下标
	REM.push(num[1]);
	for (int i = start; i < nsub; i++) {
		if (num[i] && name[i] == s) {
			return i;
		}
	}
	return -1;
}
bool dir::haveRight(string user, string name, int method) { //method 1：读  2：写  3:执行
	if (user == "root")
		return true;
	int id = Users::getUid(user);
	int p = find(name);
	if (INODE[num[p]].uid == id) {
		return INODE[num[p]].getRight(10 - method);
	}
	//组的放这里
	if (INODE[num[p]].gid == Users::getGid(user))
		return INODE[num[p]].getRight(7 - method);

	return INODE[num[p]].getRight(4 - method);
}
void dir::update() {
	if (num[1] != 0)
		INODE[num[1]].setDirData();
	if (num[0] != 0)
		INODE[num[0]].setDirData();
}
//public
dir::dir(string s) {
	nsub = 2;
	name[0] = "NULL";
	num[0] = 0;
	name[1] = s;
	int di = sblock.i_get();
	num[1] = di;
	INODE[di].pdir = this;
	INODE[di].type = 2;
	update();
}
dir* dir::addDir(string s) {//s:目录名  //在当前目录下添加目录x

    if (this->find(s,2) != -1) {
		cerr << "命名冲突" << endl;
		return NULL;
	}
	dir* x = newDir(s);

	int p = getFree();
	name[p] = x->name[1];
	num[p] = x->num[1];
	x->num[0] = num[1];
	x->name[0] = name[1];
	x->update();
	update();
	return x;
}
void dir::remove() {//删除目录自己，包括子目录和文件
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
		p->update();
	}
	sblock.i_put(num[1]);

	this->~dir();
}
bool dir::rename(string s) {//目录重命名
	if (s == name[1])
		return true;
	dir* p = getParent();
	int x1 = p->find(s);
	if (x1 == -1) {
		int x = p->find(name[1]);
		name[1] = s;
		update();
		p->name[x] = s;
		p->update();
		for (int i = 2; i < nsub; i++) {
			if (num[i]) {
				dir* x = getDir(num[i]);
				x->name[0] = s;
				x->update();
			}
		}
		return true;
	}
	return false;
}
void dir::print() {//输出目录表
	for (int i = 0; i < nsub; i++) {
		cout << name[i] << "  " << num[i] << endl;
	}
	cout << endl;
}
dir* dir::in(string s) {//进入子目录，失败时留在当前目录
	int p = find(s);
	if (p != -1 && INODE[num[p]].type == 2) {
		REM.push(num[p]);
		return INODE[num[p]].pdir;
	}
	return this;
}
dir* dir::out() {//返回上一级目录，失败时留在当前目录
	if (num[0] == 0)
		return this;
	REM.pop(num[0]);
	return INODE[num[0]].pdir;
}
int dir::findFile(string s) {//根据文件名找文件，没找到则返回-1
	int p = find(s);
	if (p != -1 && INODE[num[p]].type != 1) {
		p = -1;
	}
	if (p != -1)
		REM.push(num[p]);
	return p;
}
inode* dir::getFile(string s) {//在当前目录查找指定文件，返回inode，失败时返回NULL
	int p = findFile(s);
	if (p == -1)
		return NULL;
	return &INODE[p];
}
bool dir::addFile(string s) {//s：文件名，添加文件
    int x=find(s,2);
    if (x!= -1){// 命名冲突判断
		return false;
    }
	int di = sblock.i_get();
	INODE[di].toFile();
	int p = getFree();
	name[p] = s;
	num[p] = di;
	update();
	return true;
}
void dir::setUser(string s, string user) {
	int p = find(s);
	if (p == -1)
		return;
	int uid = Users::getUid(user);
	int gid = Users::getGid(user);
	INODE[num[p]].uid = uid;
	INODE[num[p]].gid = gid;
}
string dir::getUser(string s) {
	int p = find(s);
	if (p == -1)
		return "";
	int uid = INODE[num[p]].uid;
	return Users::getName(uid);
}
void dir::setRight(string s, int right) {
	int p = find(s);
	if (p == -1)
		return;
	INODE[num[p]].setRight(right);
}
int dir::getRight(string s) {//返回0时查找失败
	int p = find(s);
	if (p == -1)
		return 0;
	return INODE[num[p]].right;
}
int dir::openFile(string s, string user, int method) {//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
	int p = findFile(s);	                    //method 1：读  2：写   3:执行
	//cout << "p--index:" << p << endl;
	if (p == -1)
		return -1;
	if (!haveRight(user, s, method))
		return -2;
	if (INODE[num[p]].nwrite == 1)
		return 0;
	if (method == 1) {
		INODE[num[p]].nread++;
		return 1;
	}
	if (method == 2 && INODE[num[p]].nread == 0) {
		INODE[num[p]].nwrite = 1;
		return 1;
	}
	return 0;
}
void dir::closeFlie(string s, int method) {//method 1：读  2：写
	int p = findFile(s);
	if (p == -1)
		return;
	if (method == 1) {
		if (INODE[num[p]].nread > 0)
			INODE[num[p]].nread--;
	}
	if (method == 2) {
		INODE[num[p]].nwrite = 0;
	}
}
bool dir::removeFile(string s) {//s：文件名，删除文件
	int p = findFile(s);
	if (p == -1)
		return false;
	sblock.i_put(num[p]);
	num[p] = 0;
	update();
	return true;
}
bool dir::renameFile(string s, string name1) {//s:文件名，name1：新文件名， 文件重命名
	int p = findFile(s);
	int p1 = find(name1);
	if (p1 != -1)
		return false;
	name[p] = name1;
	update();
}
string dir::readFile(string s) {//  返回文件全部内容
	int p = findFile(s);
	return INODE[num[p]].getData();
}
void dir::writeFile(string s, string str) {//s：文件名，str：新的内容（全部） 重写文件 
	int p = findFile(s);
    if(p==-1){
        cerr<<s+"write fail"<<endl;
        return;
    }
	INODE[num[p]].setData(str);
}
string dir::getName() {
	return name[1];
}
