#ifndef WIDGET_CATALOG_H
#define WIDGET_CATALOG_H

#include <QWidget>

namespace Ui {
class widget_catalog;
}

class widget_catalog : public QWidget
{
    Q_OBJECT

public:
    explicit widget_catalog(QWidget *parent = 0);
    ~widget_catalog();

private:
    Ui::widget_catalog *ui;
};

#endif // WIDGET_CATALOG_H
