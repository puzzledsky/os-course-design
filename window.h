#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
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

private:
    Ui::Window *ui;
    SubForm *form;
    Login *login;
    widget_catalog *catalog;
    Dialog_adduser *adduser;
};

#endif // WINDOW_H
