#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

#include "courseapi.h"

#include "postwindow.h"
#include "userprofilewindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QStringListModel *model;
    QStringListModel *userPostsModel;
    CourseAPI *api;

    void setApi(CourseAPI*);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void setUsername(QString username, QString email);
    void loadPosts();

private slots:
    void on_searchButton_clicked();

    void on_postsView_clicked(const QModelIndex &index);

    void on_userPostsView_clicked(const QModelIndex &index);

    void on_logoutButton_clicked();

    void on_userProfilePushButton_clicked();

    void on_returnButton_clicked();

    void on_createButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::MainWindow *ui;
    PostWindow *postWindow;
    UserProfileWindow *userProfileWindow;
};

#endif // MAINWINDOW_H
