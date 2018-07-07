#include "login.h"
#include "ui_login.h"

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
    return ui->comboBox_name->currentText();
}
