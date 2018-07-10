#include "dialog_adduser.h"
#include "ui_dialog_adduser.h"

Dialog_adduser::Dialog_adduser(QWidget *parent,QString name,QString pwd) :
    QDialog(parent),
    ui(new Ui::Dialog_adduser)
{
    ui->setupUi(this);
    this->setWindowTitle("new user");
    ui->lineEdit_name->setText(name);
    ui->lineEdit_pwd->setText(pwd);
}

Dialog_adduser::~Dialog_adduser()
{
    delete ui;
}

QString Dialog_adduser::getname(){
    return ui->lineEdit_name->text();
}

QString Dialog_adduser::getpwd(){
    return ui->lineEdit_pwd->text();
}
