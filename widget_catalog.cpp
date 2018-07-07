#include "widget_catalog.h"
#include "ui_widget_catalog.h"

widget_catalog::widget_catalog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_catalog)
{
    ui->setupUi(this);
}

widget_catalog::~widget_catalog()
{
    delete ui;
}
