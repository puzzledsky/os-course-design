#include "window.h"
#include "ui_window.h"

void test(){//测试代码
    ROOT->addFile("1.txt");
    ROOT->writeFile("1.txt","1111111111111111111111111111111111111111");
}

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    //ui配置
    ui->tableWidget->setRowCount(SIZE/16+1);


    test();
    //刷新一次
    update();
}

Window::~Window()
{
    delete ui;
}

//void Window::on_pushButton_clicked()
//{
//    form = new SubForm(this);
//    form->setModal(false);
//    form->show();
//}

void Window::on_cLB_Login_clicked()
{
    login = new Login(this);
    //login->show();
    if(login->exec()==QDialog::Accepted){
        //todo:验证账密
        form=new SubForm();
        form->show();
    }

}

void Window::on_cLB_openCatalog_clicked()
{
    catalog=new widget_catalog();
    catalog->show();
}

void Window::on_cLB_adduser_clicked()
{
    adduser=new Dialog_adduser(this);
    if(adduser->exec()==QDialog::Accepted){

    }
}

void Window::update(){
    blockPrint();
}

void Window::setGrid(QTableWidget* widget,int x,int y,QColor c){
    QTableWidgetItem *item=new QTableWidgetItem("");
    item->setBackgroundColor(c);
    widget->setItem(x,y,item);
}

void Window::blockPrint(){
    QTableWidget *w=ui->tableWidget;
    setGrid(w,0,0,Qt::red);
    setGrid(w,0,1,Qt::blue);
    int i=1;
    while (i<ISIZE) {
        if(B_FLAG[i])
            setGrid(w,(i+2)/16,(i+2)%16,Qt::yellow);
        i++;
    }
    while (i<SIZE) {
        if(B_FLAG[i]){
             setGrid(w,(i+2)/16,(i+2)%16,Qt::green);
        }
        i++;
    }
}
