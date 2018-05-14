#include <QRegExp>
#include <QString>
#include <QMessageBox>

#include "postwindow.h"
#include "ui_postwindow.h"

#include "courseapi.h"

PostWindow::PostWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PostWindow)
{
    ui->setupUi(this);
    for (int i = 1; i <= 5; i++){
        ui->markBox->addItem(QString::number(i));
    }
}

PostWindow::~PostWindow()
{
    delete ui;
}

void PostWindow::setApi(CourseAPI *api) {
    this->api = api;
}

void PostWindow::setPostInfo(QString regex)
{
    QRegExp rx("(\\d+) \\|.*");
    int pos = rx.indexIn(regex);
    postId = 0;
    if (pos >= 0) {
        QString res = rx.cap(1);
        postId = res.toInt();
    }

    qDebug() << postId;

    bool res = api->updatePostRatings(postId);

    std::pair<Regex, bool> answer = api->getRating(postId);
    if (!answer.second) {
        this->close();
    }
    this->setWindowTitle(answer.first.expression);
    ui->textBrowser->setText(answer.first.explanation);
    ui->regex->setText(answer.first.expression);
    ui->authorLabel->setText(answer.first.author);
    ui->viewsLabel->setText(QString::number(answer.first.views));
    ui->ratingLabel->setText(QString::number(answer.first.avgMark));

    qDebug() << answer.first.authorId << api->userId;

    if (answer.first.authorId == api->userId) {
        ui->deleteButton->setEnabled(true);
    }
}

void PostWindow::updatePost() {
    std::pair<Regex, bool> answer = api->getRating(postId);
    if (!answer.second) {
        this->close();
    }
    this->setWindowTitle(answer.first.expression);
    ui->textBrowser->setText(answer.first.explanation);
    ui->regex->setText(answer.first.expression);
    ui->authorLabel->setText(answer.first.author);
    ui->viewsLabel->setText(QString::number(answer.first.views));
    ui->ratingLabel->setText(QString::number(answer.first.avgMark));
}

void PostWindow::on_closeButton_clicked()
{
    this->close();
}

void PostWindow::on_pushButton_clicked()
{
    QString s = ui->markBox->currentText();
    if (s.isEmpty()) {
        return;
    }
    bool res = api->updatePostRatings(postId, s.toInt());
    if (!res) {
        return;
    }
    this->updatePost();
}

void PostWindow::on_deleteButton_clicked()
{
    if (api->userId == 1) {
        QMessageBox::information(this,
                                tr("Regex"),
                                tr("Anonymous user cannot delete created expressions."),
                                QMessageBox::Ok | QMessageBox::Escape,
                                QMessageBox::NoButton);
        return;
    }
    bool res = api->deleteRegex(postId);
    if (res) {
        this->close();
    }
}
