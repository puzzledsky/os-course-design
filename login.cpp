#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent,QString name,QString pwd) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("login");
    ui->lineEdit_name->setText(name);
    ui->lineEdit_pwd->setText(pwd);
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
