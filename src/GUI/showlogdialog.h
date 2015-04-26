#ifndef SHOWLOGDIALOG_H
#define SHOWLOGDIALOG_H

#include <QDialog>

namespace Ui {
class ShowLogDialog;
}

class ShowLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowLogDialog(QWidget *parent = 0);
    ~ShowLogDialog();

private:
    Ui::ShowLogDialog *ui;
};

#endif // SHOWLOGDIALOG_H
