#include "courseapi.h"
#include "welcomescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CourseAPI *api = new CourseAPI();
    WelcomeScreen w;
    w.show();


    w.setApi(api);

    return a.exec();
}
