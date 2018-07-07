#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
<<<<<<< HEAD
#include <QTableWidget>
=======
#include <QMessageBox>
//#include <bits/stdc++.h>
>>>>>>> 2a614dc736fe079a74bad4ed1cd002ef458fb2fd
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

    void update();//所有ui的刷新
    void setGrid(QTableWidget* widget,int x,int y,QColor c);
    void blockPrint();
private:
    Ui::Window *ui;
    SubForm *form;
    Login *login;
    widget_catalog *catalog;
    Dialog_adduser *adduser;
};

#endif // WINDOW_H
