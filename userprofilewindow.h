#ifndef USERPROFILEWINDOW_H
#define USERPROFILEWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

#include "postwindow.h"

namespace Ui {
class UserProfileWindow;
}

class UserProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    QStringListModel *postsModel;
    QStringListModel *historyModel;

    explicit UserProfileWindow(QWidget *parent = 0);
    ~UserProfileWindow();

private slots:
    void on_postsView_clicked(const QModelIndex &index);

    void on_historyView_clicked(const QModelIndex &index);

private:
    Ui::UserProfileWindow *ui;
    PostWindow *postWindow;
};

#endif // USERPROFILEWINDOW_H
