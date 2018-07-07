#include "dialog_adduser.h"
#include "ui_dialog_adduser.h"

Dialog_adduser::Dialog_adduser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_adduser)
{
    ui->setupUi(this);
}

Dialog_adduser::~Dialog_adduser()
{
    delete ui;
}
