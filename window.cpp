#include "window.h"
#include "ui_window.h"

void test(){//测试代码
    ROOT->addFile("1.txt");
    ROOT->writeFile("1.txt","1111111111111111111111111111111111111111");
}

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    //ui配置
    ui->tableWidget->setRowCount(SIZE/16+1);
    ui->tableWidget_2->setRowCount(MSIZE/10);
    //ui->tableWidget->setStyleSheet("selection-background-color:");
    ui->tableWidget_3->setRowCount(ISIZE);
   // ui->lt_users->horizontalHeader()->setStretchLastSection(true);
    //ui->lt_users->setHorizontalHeaderItem(0,new QTableWidgetItem("用户名"));
    //ui->lt_users->setHorizontalHeaderItem(1,new QTableWidgetItem("状态"));
    //ui->lt_users->setHorizontalHeaderItem(2,new QTableWidgetItem("所在组"));
    //ui->lt_users->setEditTriggers(QAbstractItemView::NoEditTriggers);

    test();



    //刷新
    timer.setInterval(500);
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    if(!timer.isActive())
        timer.start();
    qDebug()<<u8"主界面构造完毕";

    //ui->tableWidget->setStyleSheet("selection-background-color:");


    test();
    //刷新一次


}

Window::~Window()
{
    delete ui;
}

void Window::on_cLB_Login_clicked()
{

    QString qname="";
    QString qpwd="";
    bool success=false;
    while (!success) {
        login = new Login(this,qname,qpwd);
        if(login->exec()==QDialog::Accepted){
            qname=login->getname();
            qpwd=login->getpwd();
            string name=qname.toStdString();
            string pwd=qpwd.toStdString();
            if(name==""){
                QMessageBox::critical(login,u8"登陆失败",u8"用户名不能为空！");
                continue;
            }
            if(pwd==""){
                QMessageBox::critical(login,u8"登陆失败",u8"密码不能为空！");
                continue;
            }
            int v=Users::loginIn(name,pwd);
            if(v==1){
                form=new SubForm(this,QString::fromStdString(name));
                form->show();
                form->setName(QString::fromStdString(name));
                success=true;
            }
            else if(v==-1){
                QMessageBox::critical(login,u8"登陆失败",u8"用户不存在！");
                name="";
            }
            else if(v==0){
                QMessageBox::critical(login,u8"登陆失败",u8"用户已登录！");
                name="";
                pwd="";
            }
            else if(v==-2){
                QMessageBox::critical(login,u8"登陆失败",u8"密码错误！");
                pwd="";
            }
        }else{
            break;
        }
    }
}

void Window::on_cLB_openCatalog_clicked()
{
    catalog=new widget_catalog();
    catalog->show();
}

void Window::on_cLB_adduser_clicked()
{
    bool finish=false;
    QString qname="";
    QString qpwd="";
    while(!finish){
        adduser=new Dialog_adduser(this,qname,qpwd);
        if(adduser->exec()==QDialog::Accepted){
            qname=adduser->getname();
            qpwd=adduser->getpwd();
            string name=qname.toStdString();
            string pwd=qpwd.toStdString();
            if(name==""){
                QMessageBox::critical(adduser,u8"添加失败",u8"用户名不能为空！");
                continue;
            }
            if(pwd==""){
                QMessageBox::critical(adduser,u8"添加失败",u8"密码不能为空！");
                continue;
            }
            int v=Users::addUser(name,pwd);
            if(v==true){
                finish=true;
            }
            else{
               QMessageBox::critical(adduser,u8"添加失败",u8"用户名已存在！");
               qname="";
            }
        }
        else{
            finish=true;
        }
    }
}

//void Window::on_bt_offwindow_clicked()
//{
//    if(QMessageBox::Yes==QMessageBox::question(this,"exit","确认退出？",
//                                               QMessageBox::Yes | QMessageBox::No,
//                                               QMessageBox::Yes));
//        this->close();
//}

void Window::update(){
    //qDebug()<<"update";
    blockPrint();
    initUserList();
    inodePrint();
    memoryPrint();
}

void Window::logInfo(QString s){
    ui->Logs->setText(s);
}

void Window::setGrid(QTableWidget* widget,int x,int y,QColor c){
    QTableWidgetItem *item=new QTableWidgetItem("");
    item->setBackgroundColor(c);
    widget->setItem(x,y,item);
}

void Window::blockPrint(){
    QTableWidget *w=ui->tableWidget;
    setGrid(w,0,0,Qt::red);
    setGrid(w,0,1,QColor(0, 0, 138));
    int i=1;
    while (i<ISIZE) {
        if(B_FLAG[i])
            setGrid(w,(i+1)/16,(i+1)%16,Qt::yellow);
        else
            setGrid(w,(i+1)/16,(i+1)%16,Qt::white);
        i++;
    }
    while (i<SIZE) {
        if(B_FLAG[i])
             setGrid(w,(i+1)/16,(i+1)%16,QColor(85, 170, 255));
        else
            setGrid(w,(i+1)/16,(i+1)%16,Qt::white);

        i++;
    }

}

void Window::initUserList(){
    //ui->lt_users->clear();//clear包括表头
    //ui->lt_users->setColumnCount(2);
    ui->lt_users->setRowCount(USER.size()+3);
    //ui->lt_users->setHorizontalHeaderItem(0,new QTableWidgetItem("用户名"));
    //ui->lt_users->setHorizontalHeaderItem(1,new QTableWidgetItem("状态"));
    //ui->lt_users->setHorizontalHeaderItem(2,new QTableWidgetItem("所在组"));
    for(int i=0;i<USER.size();i++){//表头不包含在行中
        ui->lt_users->setItem(i,0,new QTableWidgetItem(QString::fromStdString(USER[i].name)));
        ui->lt_users->setItem(i,1,new QTableWidgetItem(USER[i].status==1?u8"在线":u8"登出"));
        ui->lt_users->setItem(i,2,new QTableWidgetItem(QString::number(USER[i].uid)));
        ui->lt_users->setItem(i,3,new QTableWidgetItem(QString::number(USER[i].gid)));
    }
}

void Window::inodePrint(){
     QTableWidget *w=ui->tableWidget_3;
     int i=1;
     while (i<ISIZE*BLOCKTOI) {
         if(INODE[i].status==0)
            setGrid(w,(i-1)/16,(i-1)%16,Qt::white);
         else if(INODE[i].type==2)
            setGrid(w,(i-1)/16,(i-1)%16,QColor(85, 170, 255));
         else if(INODE[i].type==1)
            setGrid(w,(i-1)/16,(i-1)%16,Qt::yellow);
         i++;
     }

}

void Window::memoryPrint(){
    int i=0;
    QTableWidget *w=ui->tableWidget_2;
    while (i<MSIZE) {
        if(REM.num[i]==0)
            setGrid(w,i/12,i%12,Qt::white);
        else if(REM.flag[i]==0)
            setGrid(w,i/12,i%12,Qt::gray);
        else if(REM.flag[i]==1)
            setGrid(w,i/12,i%12,QColor(85, 170, 255));
        i++;
    }
}

void Window::on_tableWidget_cellClicked(int row, int column)
{
    int num=16*row+column;
    QString s=u8"";
    if(num==0){
        s.append(u8"系统引导块\n");
        logInfo(s);
        return;
    }
    else if(num==1){
        s.append(u8"超级块\n");
        s.append("---------------\n");
        s.append(u8"inode区块数:"+QString::number(sblock.isize/BLOCKTOI)+"\n");
        s.append(u8"存储区块数:"+QString::number(sblock.dsize)+"\n");
        s.append(u8"inode空闲队列长度:"+QString::number(sblock.ninode)+"\n");
        s.append(u8"存储区空闲队列长度:"+QString::number(sblock.ndata)+"\n");
        s.append("---------------\n");
        s.append(u8"inode区空闲队列\n");
        for(int i=sblock.ninode-1;i>0;i--){
            s.append(QString::number(sblock.ifree[i])+" ");
            if(sblock.ninode-i+1>20){
                s.append("\n");
                break;
            }
            if((sblock.ninode-i)%5==0)
                s.append("\n");
            else if(i==1){
                s.append("\n");
            }
        }
        s.append("---------------\n");
        s.append(u8"存储区空闲队列\n");
        for(int i=sblock.ndata-1;i>0;i--){
            s.append(QString::number(sblock.dfree[i])+" ");
            if(sblock.ndata-i+1>20){
                s.append("\n");
                break;
            }
            if((sblock.ndata-i)%5==0)
                s.append("\n");
            else if(i==1){
                s.append("\n");
            }
        }
        //for(int i=0;i<filsys.ifree)

        logInfo(s);
        return;
    }
    num-=1;
    s.append(QString::number(num));
    if(num<=ISIZE){
        s.append(u8"  inode块\n");
        s.append(u8"inode范围:\n");
        s.append(QString::number(16*(num-1)+1));
        s.append(u8" - ");
        s.append(QString::number(16*(num-1)+16));
        s.append(u8"\n已使用inode信息:\n");
        for(int i=1;i<=BLOCKTOI;i++){
            int x=16*(num-1)+i;
            if(INODE[x].status==1){
                s.append(QString::number(x));
                s.append(u8"\n");
            }
        }
        logInfo(s);
        return;
    }
    s.append(u8"  数据块\n");
    s.append("---------------\n");
    s.append(QString::fromStdString(BLOCK[num].data)+"\n");
    logInfo(s);
}

void Window::on_tableWidget_3_cellClicked(int row, int column)
{
    QString s=u8"";

    int n=16*row+column;
    n+=1;
    s.append(u8"inode号:"+QString::number(n)+"\n");
    s.append(u8"状态:");
    s.append(INODE[n].status==0?u8"未分配":u8"已分配");
    s.append((INODE[n].type==1)?u8"\n类型:文件":u8"\n类型:目录");
    s.append(u8"\n用户ID:");
    s.append(QString::number(INODE[n].uid));
    s.append(u8"\n组ID:");
    s.append(QString::number(INODE[n].gid));
    s.append(u8"\n权限:"+QString::number(INODE[n].right));
    s.append(u8"\n长度:"+QString::number(INODE[n].size));
    s.append(u8"\naddr[]信息:\n");
    for(int i=0;i<8;i++){
        if(INODE[n].addr[i]!=-1){
            s.append(QString::number(i)+u8"  "+QString::number(INODE[n].addr[i])+"\n");
        }
    }



    logInfo(s);
}

void Window::on_tableWidget_2_cellClicked(int row, int column)
{
    QString s=u8"";
    int n=10*row+column;
    int num=REM.num[n];
    s.append(u8"序号:"+QString::number(n)+"\n");
    s.append(u8"磁盘inode号:"+QString::number(num));
    s.append((INODE[n].type==1)?u8"\n类型:文件":u8"\n类型:目录");
    s.append(u8"\n用户ID:");
    s.append(QString::number(INODE[n].uid));
    s.append(u8"\n组ID:");
    s.append(QString::number(INODE[n].gid));
    s.append(u8"\n权限:"+QString::number(INODE[num].right));
    s.append(u8"\n访问状态:");//是否加锁
    if(INODE[n].nread>0||INODE[n].nwrite>0)
        s.append("已加锁");
    else
        s.append("未加锁");

    s.append(u8"\n长度:"+QString::number(INODE[num].size));
    s.append(u8"\naddr[]信息:\n");
    for(int i=0;i<8;i++){
        if(INODE[num].addr[i]!=-1){
            s.append(QString::number(i)+u8"  "+QString::number(INODE[num].addr[i])+"\n");
        }
    }
    logInfo(s);
}
