﻿#ifndef SUBFORM_H
#define SUBFORM_H

#include <QDialog>
#include "file.h"

#define mling 0

namespace Ui {
class SubForm;
}

class SubForm : public QDialog
{
    Q_OBJECT

public:
    explicit SubForm(QWidget *parent = 0,QString string = NULL);
    ~SubForm();

    void closeEvent(QCloseEvent *event);//关闭事件
    void returnReaction();
    void reactionFunction();
    void getFun();
    void outputerror(QString string);
    void setName(QString string);
    void storeFile();

protected:
     bool eventFilter(QObject *obj, QEvent *event);

private slots:
     void on_textEdit_cursorPositionChanged();

private:

    QString name;
    dir *THIS;
    dir *USER;
    QString newfile; //新建文档的内容存储在字符串中
    QString filename;
    bool isEdit;//是否处在编辑状态
    bool endEdit;//完成编辑
    Ui::SubForm *ui;
    QString ml;//用户输入的操作指令
    QStringList mlings; //其中的内容分别为：文件名、文件路径、目的文件路径、目的文件名
    ///////////////////////////////////////////////指令集合
    QString cd = "cd";//改变工作目录--------------------------------
    QString mkdir = "mkdir";//在当前目录下创建新目录，后接目录名--------
    QString rmdir = "rmdir";//在当前目录下删除目录，后接目录名----------
    QString ls = "ls";//列出目录的内容，后接目录名---------------------
    QString find = "find";//搜索文件，后接文件名
    QString rm = "rm";//删除文件，后接文件名 rm test.txt--------------
    QString cat = "cat";//显示文件内容，后接文件名 cat test.txt--------
    QString mv = "mv";//文件更名或移动-------------------------------
    //文件路径写法 /uesr/test/test.txt
    QString vi = "vi";//新建一个文本文件，后接文件名，同时进入文本编辑器----
    QString wq = ":w q";//将文件写盘并退出vi--------------------------
    QString pwd = "pwd";//显示用户当前所在路径的全名--------------------
    QString usermod = "usermod";//将用户加入用户组和退出用户组usermod -g name groupname; uesrmod -r name groupname
    QString groups = "groups";
    QString chmod  = "chmod";//修改文件权限--------------------------

};

#endif // SUBFORM_H
