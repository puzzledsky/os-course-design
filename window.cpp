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
    //ui->tableWidget->setStyleSheet("selection-background-color:");
    ui->tableWidget_3->setRowCount(ISIZE);
    ui->lt_users->horizontalHeader()->setStretchLastSection(true);
    ui->lt_users->setEditTriggers(QAbstractItemView::NoEditTriggers);

    test();




    //刷新
    timer.setInterval(500);
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    if(!timer.isActive())
        timer.start();
    qDebug()<<"主界面构造完毕";
}

Window::~Window()
{
    delete ui;
}

void Window::on_cLB_Login_clicked()
{
    login = new Login(this);

    if(login->exec()==QDialog::Accepted){
        QString qname=login->getname();
        QString qpwd=login->getpwd();
        string name=qname.toStdString();
        string pwd=qpwd.toStdString();
        int v=Users::loginIn(name,pwd);
        if(v==1){
            form=new SubForm(this,QString::fromStdString(name));
            form->show();
            form->setName(QString::fromStdString(name));
        }
        else if(v==-1){
            QMessageBox::critical(login,u8"登陆失败",u8"用户不存在！");
        }
        else if(v==0){
            QMessageBox::critical(login,u8"登陆失败",u8"用户已登录！");
        }
        else if(v==-2){
            QMessageBox::critical(login,u8"登陆失败",u8"密码错误！");
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
    adduser=new Dialog_adduser(this);
    if(adduser->exec()==QDialog::Accepted){
        QString qname=adduser->getname();
        QString qpwd=adduser->getpwd();
        string name=qname.toStdString();
        string pwd=qpwd.toStdString();
        int v=Users::addUser(name,pwd);
        if(v==true){
            adduser->close();
        }
        else{
           QMessageBox::critical(adduser,u8"添加失败",u8"用户名已存在！");
        }
    }
}

void Window::on_bt_offwindow_clicked()
{
    if(QMessageBox::Yes==QMessageBox::question(this,"exit","确认退出？",
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::Yes));
        this->close();
}

void Window::update(){
    //qDebug()<<"update";
    blockPrint();
    initUserList();
    inodePrint();
}

void Window::logInfo(QString s){
   cout<<" "<<s.toStdString()<<endl;
}

void Window::setGrid(QTableWidget* widget,int x,int y,QColor c){
    QTableWidgetItem *item=new QTableWidgetItem("");
    item->setBackgroundColor(c);
    widget->setItem(x,y,item);
}

void Window::blockPrint(){
    QTableWidget *w=ui->tableWidget;
    setGrid(w,0,0,Qt::red);
    setGrid(w,0,1,Qt::blue);
    int i=1;
    while (i<ISIZE) {
        if(B_FLAG[i])
            setGrid(w,(i+2)/16,(i+2)%16,Qt::yellow);
        else
            setGrid(w,(i+2)/16,(i+2)%16,Qt::white);
        i++;
    }
    while (i<SIZE) {
        if(B_FLAG[i])
             setGrid(w,(i+2)/16,(i+2)%16,Qt::green);
        else
            setGrid(w,(i+2)/16,(i+2)%16,Qt::white);

        i++;
    }

}

void Window::initUserList(){
    int row=ui->lt_users->rowCount();
    for(int i=row;i>=0;i--){
        ui->lt_users->removeRow(i);
        qDebug()<<i;
    }
    for(int i=0;i<USER.size();i++){
        qDebug()<<"inituserlist"<<i;
        ui->lt_users->insertRow(i+1);
        ui->lt_users->setItem(i,0,new QTableWidgetItem(QString::fromStdString(USER[i].name)));
        ui->lt_users->setItem(i,1,new QTableWidgetItem(USER[i].status));
    }
}

void Window::inodePrint(){
     QTableWidget *w=ui->tableWidget_3;
     int i=1;
     while (i<ISIZE*BLOCKTOI) {
         if(INODE[i].status==0)
            setGrid(w,i/16,i%16,Qt::white);
         else if(INODE[i].type==2)
            setGrid(w,i/16,i%16,Qt::blue);
         else if(INODE[i].type==1)
            setGrid(w,i/16,i%16,Qt::green);
         i++;
     }

}

void Window::on_tableWidget_cellClicked(int row, int column)
{
    cout<<row<<" "<<column<<endl;
    int num=16*row+column;
    num-=2;
    cout<<num<<endl;
    logInfo(QString::fromStdString(BLOCK[num].data));
}
