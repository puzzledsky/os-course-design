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

//void Window::on_pushButton_clicked()
//{
//    form = new SubForm(this);
//    form->setModal(false);
//    form->show();
//}

void Window::on_cLB_Login_clicked()
{
    login = new Login(this);
    //login->show();
    if(login->exec()==QDialog::Accepted){

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

    }
}
