#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void onVisitWebsite();

private:
    Ui::AboutDialog *ui;
    QFont *fontMuseo900Regular_;
    int fontId_;
};

#endif // ABOUTDIALOG_H
