#include "login.h"
#include "ui_login.h"
//999
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

QString Login::getpwd(){
    return ui->lineEdit_pwd->text();
}

QString Login::getname(){
    return ui->lineEdit_name->text();
}
