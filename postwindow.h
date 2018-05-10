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
    CourseAPI *api = new CourseAPI();

    explicit PostWindow(QWidget *parent = 0);
    ~PostWindow();

    void setPostInfo(QString regex, int postId);

private slots:
    void on_closeButton_clicked();

private:
    Ui::PostWindow *ui;
};

#endif // POSTWINDOW_H
