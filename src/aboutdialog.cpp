#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFontDatabase>
#include <QDebug>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->btnWebsite, SIGNAL(clicked()), SLOT(onVisitWebsite()));

    fontId_ = QFontDatabase::addApplicationFont(":/Fonts/museo900-regular.otf");
    ui->lblVisualRenamer->setStyleSheet("font: normal 26pt \"Museo\"; color: rgb(60, 56, 68)");
}

AboutDialog::~AboutDialog()
{
    delete ui;
    QFontDatabase::removeApplicationFont(fontId_);
}

void AboutDialog::onVisitWebsite()
{
    QDesktopServices::openUrl(QUrl("http://www.visualrenamer.com"));
}
