#include <QUrl>
#include <QList>
#include <QDialog>
#include <QThread>
#include <QObject>
#include <QString>
#include <QUrlQuery>
#include <QDateTime>
#include <QEventLoop>
#include <QJsonValue>
#include <QJsonArray>
#include <QEventLoop>
#include <QMessageBox>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QNetworkAccessManager>

#include "courseapi.h"
#include "mainwindow.h"

CourseAPI::CourseAPI() {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply) {
                qDebug() << "manager finished";
                if (reply->error()) {
                    qDebug() << reply->errorString();
                    return;
                }
            });
}

bool CourseAPI::regUser(QString username, QString email, QString password) {
    QUrl url = API::uri;
    url.setPath("/users/register");
    qDebug() << url;
    QJsonObject jObj;
    jObj.insert("username", username);
    jObj.insert("user_mail", email);
    jObj.insert("pwd", QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex()));

    //TODO: generate json here

    QEventLoop loop;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(url);

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }

    qDebug() << "pong";

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    int status = statusCode.toInt();
    qDebug() << status << statusCode;
    if (status != 201) {
        return false;
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    qDebug() << json;

    reply->deleteLater();
    return true;
}

bool CourseAPI::authorize(QString username, QString password) {
    return true;
}

bool CourseAPI::refreshToken() {
    return true;
}

bool CourseAPI::exitUser() {
    return true;
}

User CourseAPI::getUser(int id) {
    User user;

    return user;
}

Regex CourseAPI::getRegex(int id) {
    Regex regex;

    return regex;
}

Regex CourseAPI::createRegex(QString expression) {
    Regex regex;

    return regex;
}

bool CourseAPI::deleteRegex(int id) {
    return true;
}

std::pair<Regex, bool> CourseAPI::editRegex(QString expression) {
    std::pair<Regex, bool> regexInfo;

    return regexInfo;
}

QList<Regex> CourseAPI::authorPosts() {
    QList<Regex> posts;

    return posts;
}

QList<Regex> CourseAPI::searchPosts(QString expression) {
    QList<Regex> posts;

    return posts;
}

Regex CourseAPI::getRating(int regexId) {
    Regex regex;

    return regex;
}

QList<Regex> CourseAPI::getAllPosts(int limitBy, int offsetBy) {
    QList<Regex> posts;

    return posts;
}

bool CourseAPI::updatePostRatings(int regexId, int userMark) {
    return true;
}

QList<Regex> CourseAPI::userViewsHistory() {
    QList<Regex> posts;

    return posts;
}
