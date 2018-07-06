#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "file.h"
#include "subform.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Window *ui;
    SubForm *form;
};

#endif // WINDOW_H
