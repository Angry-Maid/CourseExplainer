#include <QString>
#include <QStringList>
#include <QStringListModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    userPosts << "([A-Za-z]+)" << "\[.+?\]";

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
