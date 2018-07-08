#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <vector>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent,QString name,QString pwd);
    ~Login();
    QString getpwd();
    QString getname();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
