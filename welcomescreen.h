#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QMainWindow>

#include "mainwindow.h"
#include "courseapi.h"

namespace Ui {
class WelcomeScreen;
}

class WelcomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    CourseAPI api = new CourseAPI();

    explicit WelcomeScreen(QWidget *parent = 0);
    ~WelcomeScreen();

private slots:
    void on_backButton_clicked();

    void on_registerButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::WelcomeScreen *ui;
    MainWindow mainWindow;
};

#endif // WELCOMESCREEN_H
