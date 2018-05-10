#include <QMap>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "postwindow.h"
#include "welcomescreen.h"
#include "userprofilewindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);
    userPostsModel = new QStringListModel(this);

    QStringList posts;
    posts << "This" << "is" << "Sparta";

    QStringList userPosts;
    userPosts << "([A-Za-z]+)" << "\\[.+?\\]";

    model->setStringList(posts);
    userPostsModel->setStringList(userPosts);

    ui->postsView->setModel(model);
    ui->userPostsView->setModel(userPostsModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUsername(QString username, QString email) {
    ui->usernameLabel->setText(username);
    ui->emailLabel->setText(email);
}

void MainWindow::on_searchButton_clicked()
{
    QString text = ui->searchEdit->text();
    if (text.isEmpty()) {
        return;
    }
    ui->returnButton->setEnabled(true);
    //model->removeRows(0, model->rowCount());
    model->insertRow(model->rowCount());
    QModelIndex index = model->index(model->rowCount() - 1);
    model->setData(index, text);
    ui->searchEdit->clear();
}

void MainWindow::on_postsView_clicked(const QModelIndex &index)
{
    QString text = model->itemData(index).first().toString();
    ui->postsView->clearSelection();
    qDebug() << text;
    postWindow = new PostWindow(this);
    postWindow->setWindowTitle(text);
    postWindow->setPostInfo(text, 0);
    postWindow->show();
}

void MainWindow::on_userPostsView_clicked(const QModelIndex &index)
{
    QString text = userPostsModel->itemData(index).first().toString();
    ui->userPostsView->clearSelection();
    qDebug() << text;
    postWindow = new PostWindow(this);
    postWindow->setWindowTitle(text);
    postWindow->setPostInfo(text, 0);
    postWindow->show();
}

void MainWindow::on_logoutButton_clicked()
{
    api->exitUser();
    WelcomeScreen *wScreen = new WelcomeScreen();
    wScreen->show();
    this->close();
}

void MainWindow::on_userProfilePushButton_clicked()
{
    QString text = QString("User Profile");
    if (false){
        text = api->username;
    }
    userProfileWindow = new UserProfileWindow(this);
    userProfileWindow->setWindowTitle(text);
    userProfileWindow->show();
}
