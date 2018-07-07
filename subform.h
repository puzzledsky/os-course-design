#ifndef SUBFORM_H
#define SUBFORM_H

#include <QDialog>

namespace Ui {
class SubForm;
}

class SubForm : public QDialog
{
    Q_OBJECT

public:
    explicit SubForm(QWidget *parent = 0);
    ~SubForm();

    void returnReaction();
    void reactionFunction();
    void getFun();


protected:
     bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::SubForm *ui;
    QString ml;//用户输入的操作指令
    QString mling;
    QString filename;//文件名
    QString filepath;//文件路径
    QString despath;//目的文件路径
    QString desfilename;//目的文件名
    ///////////////////////////////////////////////指令集合
    QString cd = "cd";//改变工作目录
    QString mkdir = "mkdir";//在当前目录下创建新目录，后接目录名
    QString rmdir = "rmdir";//在当前目录下删除目录，后接目录名
    QString ls = "ls";//列出目录的内容，后接目录名
    QString find = "find";//搜索文件，后接文件名
    QString rm = "rm";//删除文件，后接文件名
    QString cat = "cat";//显示文件内容，后接文件名
    QString mv = "mv";//文件更名或移动
    //文件路径写法/uesr/test/test.txt]
    QString vi = "vi";//新建一个文本文件，后接文件名，同时进入文本编辑器
    QString wq = ":w q";//将文件写盘并退出vi

};

#endif // SUBFORM_H
