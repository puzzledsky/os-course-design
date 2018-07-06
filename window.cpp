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

void Window::on_pushButton_clicked()
{
    form = new SubForm(this);
    form->setModal(false);
    form->show();
}
