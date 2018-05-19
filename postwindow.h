#ifndef POSTWINDOW_H
#define POSTWINDOW_H

#include <QMainWindow>
#include <QString>

#include "courseapi.h"

namespace Ui {
class PostWindow;
}

class PostWindow : public QMainWindow
{
    Q_OBJECT

public:
    CourseAPI *api;

    int postId;

    void setApi(CourseAPI*);

    explicit PostWindow(QWidget *parent = 0);
    ~PostWindow();

    void loadPostInfo(int id);
    void setPostInfo(QString regex);

    void updatePost();

private slots:
    void on_closeButton_clicked();

    void on_pushButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::PostWindow *ui;
};

#endif // POSTWINDOW_H
