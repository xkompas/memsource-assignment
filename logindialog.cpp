#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

void LoginDialog::setErrorMessage(const QString errorMessage)
{
    ui->errorMessageLabel->setText(errorMessage);
}

QString LoginDialog::getUserName() const
{
    return ui->userNameEdit->text();
}

QString LoginDialog::getPassword() const
{
    return ui->passwordEdit->text();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
