#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QTime>

#include "file.h"
#include "login.h"
#include "subform.h"
#include "widget_catalog.h"
#include "dialog_adduser.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private slots:

    void on_cLB_Login_clicked();

    void on_cLB_openCatalog_clicked();

    void on_cLB_adduser_clicked();

    void on_bt_offwindow_clicked();

    void update();//所有ui的刷新

    void setGrid(QTableWidget* widget,int x,int y,QColor c);

    void blockPrint();


    void initUserList();


    void inodePrint();
    void on_tableWidget_cellClicked(int row, int column);
    void logInfo(QString s); //信息显示，暂时是控制台输出

private:
    Ui::Window *ui;
    SubForm *form;
    Login *login;
    widget_catalog *catalog;
    Dialog_adduser *adduser;
    QTimer timer;
};

#endif // WINDOW_H
