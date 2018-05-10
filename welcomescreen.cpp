#include <QVariant>
#include <QDialog>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QNetworkRequest>

#include "welcomescreen.h"
#include "ui_welcomescreen.h"

WelcomeScreen::WelcomeScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeScreen)
{
    ui->setupUi(this);
    ui->registerWidget->hide();
}

WelcomeScreen::~WelcomeScreen()
{
    delete ui;
}

void WelcomeScreen::on_backButton_clicked()
{
    ui->registerWidget->hide();
    ui->loginWidget->show();
}

void WelcomeScreen::on_registerButton_clicked()
{
    ui->loginWidget->hide();
    ui->registerWidget->show();
}

void WelcomeScreen::on_loginButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();
    qDebug("ping");
    mainWindow.show();
    mainWindow.setUsername(login, password);
    this->close();
}

void WelcomeScreen::on_registerButton_2_clicked()
{
    qDebug() << "Register2: ping";
    QString login = ui->loginEdit_2->text();
    QString email = ui->emailEdit_2->text();
    QString pwd = ui->passwordEdit_2->text();
    if (login.isEmpty() && email.isEmpty() && pwd.isEmpty()) {
        QMessageBox::warning(this,
                             "Register",
                             "Not all fields were filled.",
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        return;
    }
    bool res = api->regUser(login, email, pwd);
    qDebug() << "pong" << res;
    if (res) {
        ui->loginEdit_2->setText("");
        ui->emailEdit_2->setText("");
        ui->passwordEdit_2->setText("");
        ui->registerWidget->hide();
        ui->loginWidget->show();
    } else {
        QMessageBox::warning(this,
                             "Register",
                             "Unable to create user.\nUsername or email already in use.\n",
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
    }
}
