#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent, Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->clientVersionLabel->setText(Version::clientVersionString());
    ui->firmwareVersionLabel->setText(Version::firmwareVersionString());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
