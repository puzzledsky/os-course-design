#include "window.h"
#include "ui_window.h"


Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    dir root("root");
    root.print();
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
            form=new SubForm();
            form->show();
        }
        else if(v==-1){
            QMessageBox::critical(login,"登陆失败","用户不存在！");
        }
        else if(v==0){
            QMessageBox::critical(login,"登陆失败","用户已登录！");
        }
        else if(v==-2){
            QMessageBox::critical(login,"登陆失败","密码错误！");
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
           QMessageBox::critical(adduser,"添加失败","用户名已存在！");
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
