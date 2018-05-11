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

void WelcomeScreen::setApi(CourseAPI *api) {
    this->api = api;
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
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Login"),
                             tr("Not all fields were filled."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        return;
    }

    bool res = api->authorize(login, password);

    if (res) {
        mainWindow.show();
        mainWindow.setApi(this->api);
        mainWindow.setUsername(api->username, api->email);
        mainWindow.loadPosts();
        this->close();
    } else {
        QMessageBox::warning(this,
                             tr("Login"),
                             tr("Unable to login.\nUsername or password are incorrect.\n"),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
    }
}

void WelcomeScreen::on_registerButton_2_clicked()
{
    QString login = ui->loginEdit_2->text();
    QString email = ui->emailEdit_2->text();
    QString pwd = ui->passwordEdit_2->text();
    if (login.isEmpty() || email.isEmpty() || pwd.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Register"),
                             tr("Not all fields were filled."),
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
                             tr("Register"),
                             tr("Unable to create user.\nUsername or email already in use.\n"),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
    }
}
