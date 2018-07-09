#ifndef WIDGET_CATALOG_H
#define WIDGET_CATALOG_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTreeWidgetItem>
#include "file.h"
namespace Ui {
class widget_catalog;
}

class widget_catalog : public QWidget
{
    Q_OBJECT

public:
    explicit widget_catalog(QWidget *parent = 0);
    ~widget_catalog();
//    QTimer timer;

private slots:
    void init();

    //void on_pushButton_3_clicked();
    void focusInEvent (QFocusEvent * event);
    void on_pushButton_3_clicked();

private:
    Ui::widget_catalog *ui;
    void addchildren(QTreeWidgetItem *faItem,dir* faDir);
    QTreeWidgetItem *proot=new QTreeWidgetItem(QStringList()<< QString::fromStdString("root"));
//    QTreeWidgetItem *pbin=new QTreeWidgetItem("bin");
//    QTreeWidgetItem *pusr=new QTreeWidgetItem("usr");
//    QTreeWidgetItem *pdev=new QTreeWidgetItem("dev");
};

#endif // WIDGET_CATALOG_H
