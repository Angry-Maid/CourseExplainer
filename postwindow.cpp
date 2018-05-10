#include <QString>

#include "postwindow.h"
#include "ui_postwindow.h"

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

void PostWindow::setPostInfo(QString regex, int postId)
{
    ui->regex->setText(regex);
}

void PostWindow::on_closeButton_clicked()
{
    this->close();
}
