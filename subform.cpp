#include "subform.h"
#include "ui_subform.h"
#include <QKeyEvent>
#include <iostream>
using namespace std;

SubForm::SubForm(QWidget *parent, QString string) :
    QDialog(parent),
    ui(new Ui::SubForm)
{
    ui->setupUi(this);
    isEdit = false;
    endEdit = false;
    name = string;
    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);
    ui->textEdit->setTextColor(QColor(255,0,255));
    ui->textEdit->append(name + " /root/home/" + name + "\n$ ");
    ui->textEdit->setTextColor(QColor(0,0,0));
    this->setWindowTitle(name);
}

void SubForm::setName(QString string)
{
    name = string;
    THIS = HOME->in(name.toStdString());
    USER = THIS;
}

SubForm::~SubForm()
{
    if(isEdit){
        QString str = ui->textEdit->toPlainText();
        THIS->writeFile(filename.toStdString(),str.toStdString());
        THIS->closeFlie(filename.toStdString(),2);
    }
    delete ui;
}

void SubForm::closeEvent(QCloseEvent *event){
    cout<<"exit"<<endl;
    Users::loginOut(name.toStdString());
}

//没有编辑文件时按下enter键时的操作
void SubForm::returnReaction()
{
    if(!isEdit){
        ui->textEdit->setTextColor(QColor(255,0,255));
        QString b;
        dir *p = THIS;
        while(p->getName() != "root"){
//            a.append(p->getName());
            b = "/" + QString::fromStdString(p->getName()) + b;
            p = p->out();
        }
        b = "/root" + b;
        ui->textEdit->append("\n" + name + " " + b + "\n$ ");
        ui->textEdit->setTextColor(QColor(0,0,0));
    }
}

//解析用户输入的命令
void SubForm::reactionFunction()
{
    ml = ui->textEdit->toPlainText();
    QStringList mlist = ml.split("\n");
    ml = mlist[mlist.size()-1];  //ml为按下回车前该行的内容
}

//根据用户命令进行不同的动作
void SubForm::getFun()
{
    if(ml == "$ "){
        return;
    }
    QStringList mls = ml.split(" ");
    mlings.clear();
    for(int i = 1; i < mls.size(); i++){
        if(mls[i] != ""){
            mlings.append(mls[i]);
        }
    }
    //根据不同的命令进行不同的操作
    if(mlings.at(mling) == mv){
        if(mlings.size() != 3){
            outputerror("input error!");
            return;
        }else{
            QString t1 = mlings.at(1);
            QString t2 = mlings.at(2);
            if(t1 == t2){
                outputerror("file name error!");
                return;
            }
            int t = THIS->openFile(t1.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
            if(t == -1){
                outputerror("File not exit!");
                THIS->closeFlie(t1.toStdString(),2);
                return;
            }else if(t == -2){
                outputerror("You have no property!");
                THIS->closeFlie(t1.toStdString(),2);
            }else if(t == 0){
                outputerror("The file is in use!");
                THIS->closeFlie(t1.toStdString(),2);
            }else{
                string str = THIS->readFile(t1.toStdString());
                if(!t2.startsWith("/")){//t2表示一个文件名
                    int t0 = THIS->openFile(t2.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                    if(t0 == -1){
                        //重命名为t2
                        THIS->renameFile(t1.toStdString(),t2.toStdString());
                        THIS->closeFlie(t2.toStdString(),2);
                        THIS->closeFlie(t1.toStdString(),2);
                        outputerror("rename file success!");
                    }else if(t0 == -2){
                        outputerror("You have no property!");
                        THIS->closeFlie(t2.toStdString(),2);
                    }else if(t == 0){
                        outputerror("The file is in use!");
                        THIS->closeFlie(t2.toStdString(),2);
                    }else{
                        //将file1重命名为file2，并删除file2
                        THIS->removeFile(t2.toStdString());
                        THIS->renameFile(t1.toStdString(),t2.toStdString());
                        THIS->closeFlie(t2.toStdString(),2);
                        THIS->closeFlie(t1.toStdString(),2);
                        outputerror("modify file success!");
                    }
                }else if(t2.startsWith("/")){//t2表示新的文件路径
                    int tt = 0;
                    QStringList temp = t2.split("/");
                    //temp中保存文件的路径，根据路径访问文件等
                    for(int i = 0; i < temp.size(); i++){
                        if(temp[i] != ""){
                            tt = 0;
                        }else{
                            temp.removeAt(i);
                            tt++;
                            if(tt == 2){
                                outputerror("Input error!");
                                return;
                            }
                        }
                    }
                        QString s = temp.at(0);
                        dir * p = HOME->in(s.toStdString());
                        if(p->getName() == "home"){
                            outputerror("dir is not exit!p == home!");
                            return;
                        }else{
                            dir * q = p;
                            for(int i = 1; i < temp.size(); i++){
                                s = temp.at(i);
                                q = p->in(s.toStdString());
                                if(p->getName() == q->getName()){
                                    outputerror("path not exit!");
                                    return;
                                }
                            }
                            ui->textEdit->append(QString::fromStdString(q->getName()));
                            int a = q->openFile(t1.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                            if(a == -1){
                                THIS->removeFile(t1.toStdString());
                                //设置file的权限
                                q->addFile(t1.toStdString());
                                q->setRight(t1.toStdString(),THIS->getRight(t1.toStdString()));
                                q->setUser(t1.toStdString(),name.toStdString());
                                q->writeFile(t1.toStdString(),str);
                                outputerror("move file success!");
                            }else if(a == -2){
                                outputerror("You have no property!");
                                return;
                            }else if(a == 0){
                                outputerror("The file is in use!");
                                return;
                            }else{
                                q->writeFile(t1.toStdString(),str);
                                q->setRight(t1.toStdString(),THIS->getRight(t1.toStdString()));
                                q->setUser(t1.toStdString(),name.toStdString());
                                THIS->removeFile(t1.toStdString());
                                outputerror("move file success!");
                            }
                            q->closeFlie(t1.toStdString(),2);
                            THIS->closeFlie(t1.toStdString(),2);
                        }
                }
            }
        }
    }else if(mlings.at(mling) == groups){
        ui->textEdit->setTextColor(QColor(0,0,0));
        ui->textEdit->append("group" + QString::number(Users::getGid(name.toStdString())));
    }else if(mlings.at(mling) == usermod){
        if(mlings.size() != 3){
            outputerror("input error!");
            return;
        }else{
            if(mlings.at(1) == "-r"){
                Users::setGroup(name.toStdString());
                outputerror("User is not in the group!");
            }else if(mlings.at(1) == "-g"){
                QString s = mlings.at(2);
                if(s.startsWith("group")){
                    Users::setGroup(name.toStdString(),s.toStdString());
                    outputerror("User is in the group!");
                }else{
                    outputerror("into group fail!");
                }
            }else{
                outputerror("the option function is not exit!");
                return;
            }
        }
    }else if(mlings.at(mling) == chmod){
        QString s = mlings.at(1);
        int a = s.toInt();
        QString ss = mlings.at(2);
        if(s.length() == 3 && a <= 777){
            int b = THIS->openFile(ss.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
            if(b == -1){
                outputerror("File not exit!");
                return;
            }else if(b == -2){
                if(THIS->getUser(ss.toStdString()) == name.toStdString()){
                    THIS->setRight(ss.toStdString(),a);
                    outputerror("Property modify success!");
                    return;
                }
                outputerror("You have no property!");
                return;
            }else if(b == 0){
                outputerror("The file is in use!");
                return;
            }else{
                THIS->setRight(ss.toStdString(),a);
                outputerror("Property modify success!");
                return;
            }
            THIS->closeFlie(ss.toStdString(),2);
        }else{
            outputerror("quanxian error!");
            return;
        }
    }else if(mlings.at(mling) == cat || mlings.at(mling) == rm || mlings[mling] == vi){

        if(mlings[mling] == vi){//创建新文件
            if(mlings.size() == 3){
                QString s = mlings.at(1);
                int a = s.toInt();
                if(s.length() == 3 && a <= 777){
                    QString tt = mlings.at(2);
                    filename  = tt;
//                    cout << tt.toStdString() << endl;
                    int t = THIS->openFile(tt.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                    if(t == -1){
                        THIS->addFile(tt.toStdString());
                        THIS->openFile(tt.toStdString(),name.toStdString(),2);
                        THIS->setUser(tt.toStdString(),name.toStdString());
                        THIS->setRight(tt.toStdString(),a);
                        isEdit = true;
                        ui->textEdit->clear();
                    }else if(t == -2){
                        outputerror("You have no property!");
                    }else if(t == 0){
                        outputerror("The file is in use!");
                    }else if(t == -3){
                        outputerror("there is a dir that has the same name!");
                    }else{
                        string str = THIS->readFile(tt.toStdString());
                        THIS->setUser(tt.toStdString(),name.toStdString());
                        THIS->setRight(tt.toStdString(),a);
                        isEdit = true;
                        ui->textEdit->clear();
                        ui->textEdit->append(QString::fromStdString(str));
                    }
                }
                }else if(mlings.size() == 2){
                    QString tt = mlings.at(1);
                    filename = tt;
                    int t = THIS->openFile(tt.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                    if(t == -1){
                        outputerror("file not exit!");
                        return;
                    }else if(t == -2){
                        outputerror("you have no property!");
                        return;
                    }else if(t == 0){
                        outputerror("The file is in use!");
                        return;
                    }else if(t == -3){
                        outputerror("there is a dir that has the same name!");
                    }else{
                        string str = THIS->readFile(tt.toStdString());
                        isEdit = true;
                        ui->textEdit->clear();
                        ui->textEdit->append(QString::fromStdString(str));
                    }
                }else{
                    outputerror("quanxain error!");
                }
        }
        else if(mlings.size() == 2){
            if(mlings.at(mling) == rm){//删除文件
                //输入文件名，删除文件
                QString tt = mlings.at(1);
                int t = THIS->openFile(tt.toStdString(),name.toStdString(),2);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                if(t == -1){
                    outputerror("File not exit!");
                }else if(t == -2){
                    outputerror("You have no property!");
                }else if(t == 0){
                    outputerror("File is now in use!");
                }else{
                    THIS->removeFile(tt.toStdString());
                    outputerror("File delete success!");
                }
                THIS->closeFlie(tt.toStdString(),2);

            }else if(mlings.at(mling) == cat){//读文件
                //显示文件内容
                QString tt = mlings.at(1);
                int t = THIS->openFile(tt.toStdString(),name.toStdString(),1);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                if(t == -1){
                    outputerror("File not exit!");
                }else if(t == -2){
                    outputerror("You have no property!");
                }else{
                    string s = THIS->readFile(tt.toStdString());
                    ui->textEdit->append(QString::fromStdString(s));
                }
                THIS->closeFlie(tt.toStdString(),1);
            }
        }else{
            outputerror("Input error!");
        }

    }else if(mlings[mling] == ls){//列出目录内容
        if(mlings.size() == 1){
            for(int i = 2; i < THIS->nsub; i++){
                if(THIS->num[i] != 0){
                    ui->textEdit->append(QString::fromStdString(THIS->name[i]));
                }
            }
        }else if(mlings.size() == 2){
             QString s = mlings.at(1);
             if(s == "-l"){

             }else{
                 outputerror("option function error!");
             }
        }else{
            outputerror("Input error!");
        }

    }else if(mlings[mling] == mkdir){//创建目录
        if(mlings.size() == 2){
            QString t = mlings.at(1);
            if(THIS->addDir(t.toStdString())){
                outputerror("add dir success!");
            }
            else{
                outputerror("dir already exit!");
            }
        }else{
            outputerror("input error!");
        }

    }else if(mlings[mling] == rmdir){//删除目录
        int num1 = 0;
        if(mlings.size() == 2){
            QString t = mlings.at(1);
            dir *p = THIS->in(t.toStdString());
            if(p->getName() == THIS->getName()){
                outputerror("dir not exit!");
                return;
            }
            for(int i = 2; i < p->nsub; i++){
                ui->textEdit->append(QString::fromStdString(p->getName()));
                if(p->num[i] != 0){
                    ui->textEdit->append(QString::fromStdString(p->name[i]));
                    num1++;
                }
            }
            if(num1 != 0){
                 outputerror("the dir is not empty,use -r to delete it and the file in it!");
                return;
            }else{
                p->remove();
                outputerror("delete dir success!");
            }
        }else if(mlings.size() == 3){
            QString t = mlings.at(2);
            dir *p = THIS->in(t.toStdString());
            if(mlings.at(1) == "-r" && p->getName() != THIS->getName()){
                p->remove();
                outputerror("delete dir success!");
            }else if(p->getName() != THIS->getName()){
                outputerror("dir is not exit");
            }else{
                outputerror("input error!");
            }
        }else{
            outputerror("input error!");
        }
    }else if(mlings[mling] == pwd){
        QStringList q;
        QString string;
        q.append(QString::fromStdString(THIS->getName()));
        dir *p = THIS->out();
        while(p->getName() != "root"){
            q.append(QString::fromStdString(p->getName()));
            p = p->out();
        }
        q.append("root");
        for(int i = q.size()-1; i>=0; i--){
            string.append("/" + q.at(i));
        }
        ui->textEdit->append(string);

    }else if(mlings[mling] == cd){//跳转目录
        if(mlings.size() == 1){
            THIS = USER;
        }else if(mlings.size() == 2){
            if(mlings.at(1) == ".."){
                THIS = THIS->out();
            }else if(mlings.at(1) == "/"){
                THIS = ROOT;
            }else{
                QString t = mlings.at(1);
                dir *p = THIS->in(t.toStdString());
                if(p->getName() != THIS->getName()){
                    if(THIS->openDir(t.toStdString(),name.toStdString())){
                        THIS = p;
                    }else{
                        outputerror("You have no property to get into the dir!");
                    }
                }else{
                    outputerror("dir not exit!");
                }
            }
        }else{
            outputerror("input error");
        }
    }else if(mlings.at(mling) == find){
        if(mlings.size() != 2){
            outputerror("input error!");
            return;
        }else{
            QString t1 = mlings.at(1);
            if(!t1.startsWith("/")){//t2表示一个文件名
                int t0 = THIS->openFile(t1.toStdString(),name.toStdString(),1);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                if(t0 == -1){
                    outputerror("file not exit!");
                }else if(t0 == -2){
                    outputerror("You have no property!");
                }else if(t0 == 0){
                    outputerror("The file is in use!");
                }else{
                    string s = THIS->readFile(t1.toStdString());
                    ui->textEdit->append(QString::fromStdString(s));
                    THIS->closeFlie(t1.toStdString(),1);
                }
            }else if(t1.startsWith("/")){//t2表示新的文件路径
                int tt = 0;
                QStringList temp = t1.split("/");
                //temp中保存文件的路径，根据路径访问文件等
                for(int i = 0; i < temp.size(); i++){
                    if(temp[i] != ""){
                        tt = 0;
                    }else{
                        temp.removeAt(i);
                        tt++;
                        if(tt == 2){
                            outputerror("Input error!");
                            return;
                        }
                    }
                }
                QString s = temp.at(0);
                dir * p = HOME->in(s.toStdString());
                if(p->getName() == "home"){
                    outputerror("dir is not exit!p == home!");
                    return;
                }else{
                    dir * q = p;
                    for(int i = 1; i < temp.size()-1; i++){
                        s = temp.at(i);
                        q = p->in(s.toStdString());
                        if(p->getName() == q->getName()){
                            outputerror("path not exit!");
                            return;
                        }
                    }
                    QString fname = temp.at(temp.size()-1);
                    int a = q->openFile(fname.toStdString(),name.toStdString(),1);//返回值  -1:不存在 -2:无权限 0:被占用  1:成功
                    if(a == -1){
                        outputerror("file not exit!");
                    }else if(a == -2){
                        outputerror("You have no property!");
                        return;
                    }else if(a == 0){
                        outputerror("The file is in use!");
                        return;
                    }else{
                        string s = q->readFile(fname.toStdString());
                        ui->textEdit->append(QString::fromStdString(s));
                        q->closeFlie(fname.toStdString(),1);
                    }
                }
            }
        }
    }else{
        outputerror("Input error!");
    }
}

void SubForm::outputerror(QString string)
{
    ui->textEdit->setTextColor(QColor(255,0,0));
    ui->textEdit->append(string);
    ui->textEdit->setTextColor(QColor(0,0,0));
}

bool SubForm::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->textEdit){
        if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if(!isEdit && keyEvent->key() == Qt::Key_Return){
                    reactionFunction();
                    getFun();
                    returnReaction();
                    return true;
                }
                if(endEdit && keyEvent->key() == Qt::Key_Return){
                    storeFile();
                    return true;
                }
                if(isEdit && keyEvent->key() == Qt::Key_F1){
                    //添加操作，当按下F1键时结束写文件
                    newfile = ui->textEdit->toPlainText();
                    ui->textEdit->setTextColor(QColor(255,0,255));
                    ui->textEdit->clear();
                    ui->textEdit->append("end of file\n$ ");
                    ui->textEdit->setTextColor(QColor(0,0,0));
                    endEdit = true;
                    return true;
                }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void SubForm::storeFile(){
    QString t = ui->textEdit->toPlainText();
    QStringList mlist = t.split("\n");
    t = mlist[mlist.size()-1];//t为此时输入的一条指令
    if(t == "$ :wq"){
        //新建一个文件

        if(mlings.size() == 3){
            t = mlings.at(2);
        }else{
            t = mlings.at(1);
        }
//        THIS->addFile(t.toStdString());
//        ui->textEdit->append(t + "-------------------");

        THIS->writeFile(t.toStdString(),newfile.toStdString());
        THIS->closeFlie(t.toStdString(),2);
        outputerror("File store success!");
        ui->textEdit->setTextColor(QColor(255,0,255));

        QString b;
        dir *p = THIS;
        while(p->getName() != "root"){
//            a.append(p->getName());
            b = "/" + QString::fromStdString(p->getName()) + b;
            p = p->out();
        }
        b = "/root" + b;
        ui->textEdit->append("\n" + name + " " + b + "\n$ ");

        ui->textEdit->setTextColor(QColor(0,0,0));
        isEdit = false;
        endEdit = false;
    }else{
        ui->textEdit->setTextColor(QColor(255,0,0));
        ui->textEdit->append("illegal!");
        ui->textEdit->setTextColor(QColor(0,0,0));
        ui->textEdit->setTextColor(QColor(255,0,255));

        QString b;
        dir *p = THIS;
        while(p->getName() != "root"){
//            a.append(p->getName());
            b = "/" + QString::fromStdString(p->getName()) + b;
            p = p->out();
        }
        b = "/root" + b;
        ui->textEdit->append("\n" + name + " " + b + "\n$ ");

        ui->textEdit->setTextColor(QColor(0,0,0));
    }
}

void SubForm::on_textEdit_cursorPositionChanged()
{
    if(!isEdit){
        ui->textEdit->setTextColor(QColor(0,0,0));
    }
}
