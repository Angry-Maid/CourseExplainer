#include <QMap>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QStringListModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "postwindow.h"
#include "welcomescreen.h"
#include "userprofilewindow.h"
#include "courseapi.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);
    userPostsModel = new QStringListModel(this);

    QStringList userPosts;
    userPosts << "([A-Za-z]+)" << "\\[.+?\\]";

    userPostsModel->setStringList(userPosts);

    ui->userPostsView->setModel(userPostsModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setApi(CourseAPI *api) {
    this->api = api;
}

void MainWindow::setUsername(QString username, QString email) {
    ui->usernameLabel->setText(username);
    ui->emailLabel->setText(email);
}

void MainWindow::loadPosts() {
    QStringList posts;
    QList<Regex> apiPosts = api->getAllPosts();

    foreach (Regex r, apiPosts) {
        posts.append(QString("%1 | %2 | Views: %3 Mark: %4 | Created: %5").arg(QString::number(r.id), r.expression, QString::number(r.views), QString::number(r.avgMark), r.date));
    }

    model->setStringList(posts);
    ui->postsView->setModel(model);

    QStringList userHistory;
    QList<Regex> apiHistoryPosts = api->userViewsHistory();

    foreach(Regex r, apiHistoryPosts) {
        userHistory.append(QString("%1 | %2 | %3").arg(QString::number(r.id), r.expression, r.date));
    }

    userPostsModel->setStringList(userHistory);
    ui->userPostsView->setModel(userPostsModel);
}

void MainWindow::on_searchButton_clicked()
{
    QString text = ui->searchEdit->text();
    if (text.isEmpty()) {
        return;
    }
    ui->returnButton->setEnabled(true);

    QStringList posts;
    QList<Regex> apiSearch = api->searchPosts(text);

    foreach (Regex r, apiSearch) {
        posts.append(QString("%1 | %2 | Views: %3 Mark: %4 | Created: %5").arg(QString::number(r.id), r.expression, QString::number(r.views), QString::number(r.avgMark), r.date));
    }

    model->setStringList(posts);
    ui->postsView->setModel(model);

    ui->searchEdit->clear();
}

void MainWindow::on_postsView_clicked(const QModelIndex &index)
{
    QString text = model->itemData(index).first().toString();
    ui->postsView->clearSelection();
    qDebug() << text;
    postWindow = new PostWindow(this);
    postWindow->setApi(this->api);
    postWindow->setWindowTitle(text);
    postWindow->setPostInfo(text);
    this->loadPosts();
    postWindow->show();
}

void MainWindow::on_userPostsView_clicked(const QModelIndex &index)
{
    QString text = userPostsModel->itemData(index).first().toString();
    ui->userPostsView->clearSelection();
    qDebug() << text;
    postWindow = new PostWindow(this);
    postWindow->setApi(this->api);
    postWindow->setWindowTitle(text);
    postWindow->setPostInfo(text);
    postWindow->show();
}

void MainWindow::on_logoutButton_clicked()
{
    api->exitUser();
    WelcomeScreen *wScreen = new WelcomeScreen();
    wScreen->setApi(this->api);
    wScreen->show();
    this->close();
}

void MainWindow::on_userProfilePushButton_clicked()
{
    QString text = api->username;
    userProfileWindow = new UserProfileWindow(this);
    userProfileWindow->setApi(this->api);
    userProfileWindow->setWindowTitle(text);
    userProfileWindow->loadPosts();
    userProfileWindow->show();
}

void MainWindow::on_returnButton_clicked()
{
    ui->returnButton->setEnabled(false);
    this->loadPosts();
}

void MainWindow::on_createButton_clicked()
{
    QString text = ui->searchEdit->text();
    if (text.isEmpty()) {
        return;
    }

    std::pair<Regex, int> answer = api->createRegex(text);
    if (answer.second != 1) {
        switch (answer.second) {
            case 0: {
                QMessageBox::information(this,
                                        tr("Regex"),
                                        tr("Incorrect expression."),
                                        QMessageBox::Ok | QMessageBox::Escape,
                                        QMessageBox::NoButton);
                return;
            }
            case 2: {
                QMessageBox::information(this,
                                        tr("Regex"),
                                        tr("Expression already exists."),
                                        QMessageBox::Ok | QMessageBox::Escape,
                                        QMessageBox::NoButton);
                return;
            }
        }
    }
    this->loadPosts();
}
