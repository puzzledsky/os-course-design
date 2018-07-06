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

private:
    Ui::SubForm *ui;
};

#endif // SUBFORM_H
