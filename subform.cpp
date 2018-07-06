#include "subform.h"
#include "ui_subform.h"

SubForm::SubForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubForm)
{
    ui->setupUi(this);
}

SubForm::~SubForm()
{
    delete ui;
}
