#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

QString LoginDialog::getUserName()
{
    return ui->userNameEdit->text();
}

QString LoginDialog::getPassword()
{
    return ui->passwordEdit->text();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
