#ifndef COURSEAPI_H
#define COURSEAPI_H

#include <QUrl>
#include <QChar>
#include <QList>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QDateTime>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

namespace API {
    const QUrl uri = QUrl("http://rabbit-solver.ru");
}

typedef struct
{
    int id;
    QString username;
    QString email;
} User;

typedef struct
{
    int id;
    int authorId;
    QString author;
    QString expression;
    QString explanation;
    QString date;
    int views;
    double avgMark;
    int userMark;
} Regex;

class CourseAPI:
        public QObject
{
public:
    CourseAPI();

    QNetworkAccessManager *manager;

    int userId;
    QString token;
    QString username;
    QString email;

    bool checkAviability();

    //User API part
    int regUser(QString username, QString email, QString password);
    bool authorize(QString username, QString password);
    bool refreshToken();
    bool exitUser();
    std::pair<User, bool> getUser(int id);

    //Regex API part
    std::pair<Regex, bool> getRegex(int id);
    std::pair<Regex, int> createRegex(QString expression);
    bool deleteRegex(int id);
    std::pair<Regex, bool> editRegex(QString expression, int id);
    QList<Regex> authorPosts();
    QList<Regex> searchPosts(QString expression);

    //Rating API part
    std::pair<Regex, bool> getRating(int regexId);
    QList<Regex> getAllPosts(int limitBy=20, int offsetBy=0);
    bool updatePostRatings(int regexId, int userMark=0);
    QList<Regex> userViewsHistory();



    QString GetRandomString(int len=24) const;

private:

protected:
};

#endif // COURSEAPI_H
