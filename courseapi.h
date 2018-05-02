#ifndef COURSEAPI_H
#define COURSEAPI_H

#include <QList>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

typedef struct {
    int id;
    QString username;
    QString email;
} User;

typedef struct {
    int id;
    int authorId;
    QString expression;
    QString explanation;
    QDateTime date;
    int views;
    int avgMark;
    int userMark;
} Regex;

class CourseAPI:
        QObject
{
public:
    CourseAPI();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    int userId;
    QString token;
    QString username;
    QString email;
    QString pwd;

    //User API part
    bool regUser(QString username, QString password);
    bool authorize(QString username, QString password);
    bool refreshToken();
    bool exitUser();
    User getUser(int id);

    //Regex API part
    Regex getRegex(int id);
    Regex createRegex(QString expression);
    bool deleteRegex(int id);
    std::pair<Regex, bool> editRegex(QString expression);
    QList<Regex> authorPosts();
    QList<Regex> searchPosts(QString expression);

    //Rating API part
    Regex getRating(int regexId);
    QList<Regex> getAllPosts(int limitBy, int offsetBy);
    bool updatePostRatings(int regexId, int userMark=0);
    QList<Regex> userViewsHistory();

private slots:
    void managerFinished(QNetworkReply *reply);

protected:
};

#endif // COURSEAPI_H
