#include "widget_catalog.h"
#include "ui_widget_catalog.h"

widget_catalog::widget_catalog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_catalog)
{
    ui->setupUi(this);
    this->setWindowTitle("dir");
    //ui->table_file->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->table_file->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    init();
//    timer.setInterval(8000);
//    connect(&timer,SIGNAL(timeout()),this,SLOT(init()));
//    if(!timer.isActive())   timer.start();
}

widget_catalog::~widget_catalog()
{
    delete ui;
}

void widget_catalog::init(){
    ui->treeWidget->clear();
    proot=new QTreeWidgetItem(ui->treeWidget,QStringList(QString::fromStdString("root")));
    //ui->treeWidget->addTopLevelItem(proot);//添加顶级项root
    addchildren(proot,ROOT);
    ui->treeWidget->expandAll();
}

void widget_catalog::addchildren(QTreeWidgetItem *faItem,dir*faDir ){
    for(int i=2;i<faDir->nsub;i++){
        if(faDir->num[i]==0)
            continue;
        if(INODE[faDir->num[i]].type==1){
            faItem->addChild(new QTreeWidgetItem(faItem,QStringList(QString::fromStdString(faDir->name[i]))));
        }
        else if(INODE[faDir->num[i]].type==2){
            QTreeWidgetItem *newitem=new QTreeWidgetItem(faItem,QStringList(QString::fromStdString(faDir->name[i])));
            faItem->addChild(newitem);
            addchildren(newitem,INODE[faDir->num[i]].pdir);
        }
    }
}

void widget_catalog::focusInEvent(QFocusEvent *event){
    cout<<"focus"<<endl;
    init();
}

//void widget_catalog::on_pushButton_3_clicked()
//{
//    init();
//}
