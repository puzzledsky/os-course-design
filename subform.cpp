#include "subform.h"
#include "ui_subform.h"
#include <QKeyEvent>
#include <iostream>
using namespace std;

SubForm::SubForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubForm)
{
    ui->setupUi(this);
    isEdit = false;
    endEdit = false;

    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);

    ui->textEdit->setTextColor(QColor(255,0,255));
//    ui->textEdit->append(QString::fromStdString(THIS->getName()) + "   /" + QString::fromStdString(THIS->getName()) + "\n$ ");
    ui->textEdit->append(name + "   /" + name + "\n$ ");
    ui->textEdit->setTextColor(QColor(0,0,0));


}

SubForm::~SubForm()
{
    delete ui;
}

//没有编辑文件时按下enter键时的操作
void SubForm::returnReaction()
{
    this->reactionFunction();
    if(!isEdit){
        ui->textEdit->setTextColor(QColor(255,0,255));
        ui->textEdit->append("\n$ ");
        ui->textEdit->setTextColor(QColor(0,0,0));
    }
}

void SubForm::reactionFunction()
{
    ml = ui->textEdit->toPlainText();
    QStringList mlist = ml.split("\n");
    ml = mlist[mlist.size()-1];
    //ml为按下回车前该行的内容
    getFun();
}

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
    if(mlings.at(mling) == cat || mlings.at(mling) == rm || mlings[mling] == vi){
        if(mlings.size() == 2){
            if(mlings[mling] == vi){
                isEdit = true;
                ui->textEdit->clear();
            }else if(mlings.at(mling) == rm){

            }else if(mlings.at(mling) == cat){

            }
        }else{
            outputerror();
        }
    }else if(mlings[mling] == ls){
        if(mlings.size() == 2){
            QString t = mlings[1];
            if(!t.startsWith("/") || t.endsWith("/")){
                outputerror();
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
                        outputerror();
                        break;
                    }
                }
            }
        }else{
            outputerror();
        }
    }else{
        outputerror();
    }
}

void SubForm::outputerror()
{
    ui->textEdit->setTextColor(QColor(255,0,0));
    ui->textEdit->append("input error!");
    ui->textEdit->setTextColor(QColor(0,0,0));
}

void SubForm::setName(QString string)
{
    name = string;
    THIS = HOME->in(name.toStdString());
}

bool SubForm::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->textEdit){
        if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if(!isEdit && keyEvent->key() == Qt::Key_Return){
                    returnReaction();
                    return true;
                }
                if(endEdit && keyEvent->key() == Qt::Key_Return){
                    storeFile();
                    endEdit = false;
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
        isEdit = false;
    }else{
        ui->textEdit->setTextColor(QColor(255,0,0));
        ui->textEdit->append("illegal!");
        ui->textEdit->setTextColor(QColor(0,0,0));
        ui->textEdit->setTextColor(QColor(255,0,255));
        ui->textEdit->append("\n$ ");
        ui->textEdit->setTextColor(QColor(0,0,0));
    }

}
