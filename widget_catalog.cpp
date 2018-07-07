#include "widget_catalog.h"
#include "ui_widget_catalog.h"

widget_catalog::widget_catalog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_catalog)
{
    ui->setupUi(this);
    ui->table_file->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_file->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
}

widget_catalog::~widget_catalog()
{
    delete ui;
}
