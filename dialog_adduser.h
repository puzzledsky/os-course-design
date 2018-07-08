#ifndef DIALOG_ADDUSER_H
#define DIALOG_ADDUSER_H

#include <QDialog>

namespace Ui {
class Dialog_adduser;
}

class Dialog_adduser : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_adduser(QWidget *parent,QString name,QString pwd);
    ~Dialog_adduser();
    QString getname();
    QString getpwd();
private:
    Ui::Dialog_adduser *ui;
};

#endif // DIALOG_ADDUSER_H
