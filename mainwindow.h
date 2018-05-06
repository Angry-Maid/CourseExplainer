#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModeL>

#include "courseapi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QStringListModel *model;
    QStringListModel *userPostsModel;
    CourseAPI api = new CourseAPI();

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void setUsername(QString username, QString email);

private slots:
    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
