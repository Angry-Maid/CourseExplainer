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
