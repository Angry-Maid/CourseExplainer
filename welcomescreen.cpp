#include <QRegExp>
#include <QDialog>
#include <QVariant>
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
    ui->loginButton->setEnabled(false);
    bool resp = api->checkAviability();
    if (!resp) {
        QMessageBox::warning(this,
                             tr("Internet Connection"),
                             tr("There is no internet connection."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->loginButton->setEnabled(true);
        return;
    }
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Login"),
                             tr("Not all fields were filled."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->loginButton->setEnabled(true);
        return;
    }

    bool res = api->authorize(login, password);

    if (res) {
        mainWindow.show();
        mainWindow.setApi(this->api);
        mainWindow.setUsername(api->username, api->email);
        mainWindow.loadPosts();
        ui->loginButton->setEnabled(true);
        this->close();
    } else {
        QMessageBox::warning(this,
                             tr("Login"),
                             tr("Unable to login.\nUsername or password are incorrect.\n"),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->loginButton->setEnabled(true);
    }
}

void WelcomeScreen::on_registerButton_2_clicked()
{
    ui->registerButton_2->setEnabled(false);
    bool resp = api->checkAviability();
    if (!resp) {
        QMessageBox::warning(this,
                             tr("Internet Connection"),
                             tr("There is no internet connection."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->registerButton_2->setEnabled(true);
        return;
    }
    QString login = ui->loginEdit_2->text();
    QString email = ui->emailEdit_2->text();
    QString pwd = ui->passwordEdit_2->text();
    if (login.isEmpty() || email.isEmpty() || pwd.isEmpty()) {
        QMessageBox::warning(this,
                             tr("Register"),
                             tr("Not all fields were filled."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->registerButton_2->setEnabled(true);
        return;
    }
    int pos;
    QRegExp rx("^[^\\s]+$");
    pos = rx.indexIn(login);
    if (pos < 0) {
        QMessageBox::warning(this,
                             tr("Register"),
                             tr("Username incorrect."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->registerButton_2->setEnabled(true);
        return;
    }
    pos = rx.indexIn(pwd);
    if (pos < 0) {
        QMessageBox::warning(this,
                             tr("Register"),
                             tr("Password incorrect."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->registerButton_2->setEnabled(true);
        return;
    }
    qDebug() << login.length() << pwd.length();
    if (!((login.length() >= 5) && (login.length() <= 35))) {
        QMessageBox::warning(this,
                             tr("Register"),
                             tr("Username or password have incorrect length.\nUsername length between 5 and 35."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->registerButton_2->setEnabled(true);
        return;
    }
    if (!((pwd.length() >= 5) && (pwd.length() <= 60))) {
        QMessageBox::warning(this,
                             tr("Register"),
                             tr("Username or password have incorrect length.\nPassword length between 5 and 60."),
                             QMessageBox::Ok | QMessageBox::Escape,
                             QMessageBox::NoButton);
        ui->registerButton_2->setEnabled(true);
        return;
    }
    int res = api->regUser(login, email, pwd);
    qDebug() << "pong" << res;
    if (res == 1) {
        ui->loginEdit_2->setText("");
        ui->emailEdit_2->setText("");
        ui->passwordEdit_2->setText("");
        ui->registerButton_2->setEnabled(true);
        ui->registerWidget->hide();
        ui->loginWidget->show();
    } else {
        switch (res) {
            case 0: {
                QMessageBox::warning(this,
                                     tr("Register"),
                                     tr("Unable to create user.\nUsername or email already in use.\n"),
                                     QMessageBox::Ok | QMessageBox::Escape,
                                     QMessageBox::NoButton);
                ui->registerButton_2->setEnabled(true);
            }
            case 2: {
                QMessageBox::warning(this,
                                     tr("Register"),
                                     tr("Invalid email"),
                                     QMessageBox::Ok | QMessageBox::Escape,
                                     QMessageBox::NoButton);
                ui->registerButton_2->setEnabled(true);
            }
        }
    }
}
