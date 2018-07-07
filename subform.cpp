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

    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);

    ui->textEdit->setTextColor(QColor(255,0,255));
    ui->textEdit->append("$ ");
    ui->textEdit->setTextColor(QColor(0,0,0));

}

SubForm::~SubForm()
{
    delete ui;
}

void SubForm::returnReaction()
{
    this->reactionFunction();
    ui->textEdit->setTextColor(QColor(255,0,255));
    ui->textEdit->append("\n$ ");
    ui->textEdit->setTextColor(QColor(0,0,0));
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
    QStringList mls = ml.split(" ");
    mling = mls[1];
    if(mling == "cat"){
        filename = mls[2];
        ui->textEdit->append(filename);
    }
}

bool SubForm::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->textEdit){
        if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if(keyEvent->key() == Qt::Key_Return){
                    returnReaction();
                    return true;
                }
         }
    }
    return QWidget::eventFilter(obj,event);
}
