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
    ui->textEdit->append(name + " /" + name + "\n$ ");
    ui->textEdit->setTextColor(QColor(0,0,0));
}

void SubForm::setName(QString string)
{
    name = string;
    THIS = HOME->in(name.toStdString());
}

SubForm::~SubForm()
{
    Users::loginOut(name.toStdString());
    delete ui;
}

//没有编辑文件时按下enter键时的操作
void SubForm::returnReaction()
{
    if(!isEdit){
        ui->textEdit->setTextColor(QColor(255,0,255));
        ui->textEdit->append("\n" + QString::fromStdString(THIS->getName()) + " /" + QString::fromStdString(THIS->getName()) + "\n$ ");
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
    if(mlings.at(mling) == cat || mlings.at(mling) == rm || mlings[mling] == vi){
        if(mlings.size() == 2){
            if(mlings[mling] == vi){//创建新文件
                QString t = mlings.at(1);
                if(THIS->addFile(t.toStdString())){
                    isEdit = true;
                    ui->textEdit->clear();
                }else{
                    outputerror("File already exit!");
                }
            }else if(mlings.at(mling) == rm){//删除文件
                //输入文件名，删除文件
                QString tt = mlings.at(1);
                if(THIS->removeFile(tt.toStdString())){
                    outputerror("File delete success!");
                }else{
                    outputerror("File not exit! Delete fail!");
                }
            }else if(mlings.at(mling) == cat){//读文件
                //显示文件内容
                QString tt = mlings.at(1);
                string s = THIS->readFile(tt.toStdString());
                if(!s.empty()){
                    ui->textEdit->append(QString::fromStdString(s));
                }else{
                    outputerror("File not exit!");
                }
            }
        }else{
            outputerror("Input error!");
        }
    }else if(mlings[mling] == ls){//列出目录内容
        if(mlings.size() == 2){
            QString t = mlings[1];
            if(!t.startsWith("/") || t.endsWith("/")){
                outputerror("Input error!");
            }
            int tt = 0;
            QStringList temp = t.split("/");
            //temp中保存文件的路径，根据路径访问文件等
//            tosuper.append(mlings[0]);
            for(int i = 0; i < temp.size(); i++){
                if(temp[i] != ""){
//                    tosuper.append(temp[i]);
                    ui->textEdit->append(temp[i]);
                    tt = 0;
                }else{
                    tt++;
                    if(tt == 2){
                        outputerror("Input error!");
                        break;
                    }
                }
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
        QString t = mlings.at(1);
        if(mlings.size() == 2){
            dir *p = THIS->in(t.toStdString());
            if(p != NULL){
                p->remove();
                outputerror("delete dir success!");
            }else{
                outputerror("dir not exit!");
            }
        }else{
            outputerror("input error!");
        }
    }else{
        outputerror("Input error!");
    }
}

void SubForm::outputerror(QString string)
{
//    ui->textEdit->setTextColor(QColor(255,0,255));
//    ui->textEdit->append("\n" + QString::fromStdString(THIS->getName()) + " /" + QString::fromStdString(THIS->getName()));
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
    t = mlist[mlist.size()-1];
    if(t == "$ :wq"){
        //新建一个文件
        t = mlings.at(1);
//        THIS->addFile(t.toStdString());
        THIS->writeFile(t.toStdString(),newfile.toStdString());
        outputerror("File store success!");
        ui->textEdit->setTextColor(QColor(255,0,255));
        ui->textEdit->append("\n" + QString::fromStdString(THIS->getName()) + " /" + QString::fromStdString(THIS->getName()) + "\n$ ");
        ui->textEdit->setTextColor(QColor(0,0,0));
        isEdit = false;
        endEdit = false;
    }else{
        ui->textEdit->setTextColor(QColor(255,0,0));
        ui->textEdit->append("illegal!");
        ui->textEdit->setTextColor(QColor(0,0,0));
        ui->textEdit->setTextColor(QColor(255,0,255));
        ui->textEdit->append("\n" + QString::fromStdString(THIS->getName()) + " /" + QString::fromStdString(THIS->getName()) + "\n$ ");
        ui->textEdit->setTextColor(QColor(0,0,0));
    }

}
