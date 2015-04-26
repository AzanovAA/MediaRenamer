#include "showlogdialog.h"
#include "ui_showlogdialog.h"

//extern QStringList renameLogs_;

ShowLogDialog::ShowLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowLogDialog)
{
    ui->setupUi(this);

    //ui->listWidget->addItems(renameLogs_);
}

ShowLogDialog::~ShowLogDialog()
{
    delete ui;
}
