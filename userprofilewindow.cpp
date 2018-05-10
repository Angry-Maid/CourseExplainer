#include <QStringList>
#include <QStringListModel>

#include "userprofilewindow.h"
#include "ui_userprofilewindow.h"

#include "postwindow.h"

UserProfileWindow::UserProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserProfileWindow)
{
    ui->setupUi(this);

    postsModel = new QStringListModel(this);
    historyModel = new QStringListModel(this);

    QStringList posts;
    posts << "Some" << "Test" << "Data";
    QStringList history;
    history << "Some" << "More" << "Test" << "Data";

    postsModel->setStringList(posts);
    historyModel->setStringList(history);

    ui->postsView->setModel(postsModel);
    ui->historyView->setModel(historyModel);
}

UserProfileWindow::~UserProfileWindow()
{
    delete ui;
}

void UserProfileWindow::on_postsView_clicked(const QModelIndex &index)
{
    QString text = postsModel->itemData(index).first().toString();
    ui->postsView->clearSelection();
    qDebug() << text;
    postWindow = new PostWindow(this);
    postWindow->setWindowTitle(text);
    postWindow->setPostInfo(text, 0);
    postWindow->show();
}

void UserProfileWindow::on_historyView_clicked(const QModelIndex &index)
{
    QString text = historyModel->itemData(index).first().toString();
    ui->historyView->clearSelection();
    qDebug() << text;
    postWindow = new PostWindow(this);
    postWindow->setWindowTitle(text);
    postWindow->setPostInfo(text, 0);
    postWindow->show();
}
