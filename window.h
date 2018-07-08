#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
<<<<<<< HEAD
#include <QTimer>
#include <QDebug>
=======
#include <QTime>
>>>>>>> f9f3272f26b507e0c60421b6fb9c613beadedf65
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
<<<<<<< HEAD

    void initUserList();

=======
    void inodePrint();
    void on_tableWidget_cellClicked(int row, int column);
    void logInfo(QString s); //信息显示，暂时是控制台输出
>>>>>>> f9f3272f26b507e0c60421b6fb9c613beadedf65
private:
    Ui::Window *ui;
    SubForm *form;
    Login *login;
    widget_catalog *catalog;
    Dialog_adduser *adduser;
    QTimer timer;
};

#endif // WINDOW_H
