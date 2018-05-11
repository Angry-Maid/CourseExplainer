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

QString CourseAPI::GetRandomString(int len) const {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i = 0; i < len; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

bool CourseAPI::regUser(QString username, QString email, QString password) {
    QUrl url = API::uri;
    url.setPath("/users/register");
    qDebug() << url;
    QJsonObject jObj;
    jObj.insert("username", username);
    jObj.insert("user_mail", email);
    jObj.insert("pwd", QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex()));

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    int status = statusCode.toInt();
    qDebug() << status << statusCode;
    if (status != 201) {
        return false;
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    qDebug() << json;

    loop.deleteLater();
    reply->deleteLater();
    return true;
}

bool CourseAPI::authorize(QString username, QString password) {
    QUrl url = API::uri;
    url.setPath("/users/authorize");
    qDebug() << url;

    QJsonObject jObj;
    jObj.insert("username", username);
    QString hashed_pwd = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex());
    QString salt = this->GetRandomString();
    hashed_pwd.append(":"); hashed_pwd += salt;
    jObj.insert("pwd", QString(QCryptographicHash::hash(hashed_pwd.toUtf8(), QCryptographicHash::Sha512).toHex()));
    jObj.insert("salt", salt);

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return false;
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    this->token = json["token"].toString();
    this->userId = json["user_id"].toInt();
    this->username = json["username"].toString();
    this->email = json["email"].toString();
    qDebug() << this->token << this->userId;
    return true;
}

bool CourseAPI::refreshToken() {
    QUrl url = API::uri;
    url.setPath("/users/refresh_token");

    QJsonObject jObj;
    jObj.insert("token", this->token);

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return false;
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    this->token = json["token"].toString();
    return true;
}

bool CourseAPI::exitUser() {
    QUrl url = API::uri;
    url.setPath("/users/logout");

    QJsonObject jObj;
    jObj.insert("token", this->token);
    jObj.insert("user_id", this->userId);

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return false;
    }

    this->userId = 0;
    this->token = "";
    this->username = "";
    this->email = "";

    return true;
}

std::pair<User, bool> CourseAPI::getUser(int id) {
    User user;
    user.id = id;

    QUrl url = API::uri;
    url.setPath("/users");

    QUrlQuery q;
    q.addQueryItem("token", this->token);
    q.addQueryItem("id", QString::number(id));

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->get(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return std::make_pair(user, false);
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    user.username = json["user"].toString();

    return std::make_pair(user, true);
}

std::pair<Regex, bool> CourseAPI::getRegex(int id) {
    Regex regex;
    regex.id = id;

    QUrl url = API::uri;
    url.setPath("/regex");

    QUrlQuery q;
    q.addQueryItem("token", this->token);
    q.addQueryItem("regex_id", QString::number(id));

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->get(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return std::make_pair(regex, false);
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    regex.expression = json["expression"].toString();
    regex.explanation = json["explanation"].toString();
    regex.author = json["author"].toString();

    return std::make_pair(regex, true);
}

std::pair<Regex, bool> CourseAPI::createRegex(QString expression) {
    Regex regex;
    regex.expression = expression;

    QUrl url = API::uri;
    url.setPath("/regex/create");

    QJsonObject jObj;
    jObj.insert("token", this->token);
    jObj.insert("expression", expression);

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return std::make_pair(regex, false);
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    regex.id = json["id"].toInt();
    regex.explanation = json["explanation"].toString();
    regex.author = this->username;

    return std::make_pair(regex, true);
}

bool CourseAPI::deleteRegex(int id) {
    QUrl url = API::uri;
    url.setPath("/regex/delete");

    QUrlQuery q;
    q.addQueryItem("token", this->token);
    q.addQueryItem("regex_id", QString::number(id));
    q.addQueryItem("user_id", QString::number(this->userId));

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->deleteResource(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return false;
    }

    return true;
}

std::pair<Regex, bool> CourseAPI::editRegex(QString expression, int id) {
    std::pair<Regex, bool> regexInfo;

    QUrl url = API::uri;
    url.setPath("/regex/edit");

    QJsonObject jObj;
    jObj.insert("token", this->token);
    jObj.insert("expression", expression);
    jObj.insert("regex_id", QString::number(id));

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->put(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        Regex r;
        return std::make_pair(r, false);
    }

    regexInfo.second = true;
    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    regexInfo.first.id = json["id"].toInt();
    regexInfo.first.expression = json["expression"].toString();
    regexInfo.first.explanation = json["explanation"].toString();
    regexInfo.first.author = json["author"].toString();

    return regexInfo;
}

QList<Regex> CourseAPI::authorPosts() {
    QList<Regex> posts;

    QUrl url = API::uri;
    url.setPath("/regex/author_posts");

    QUrlQuery q;
    q.addQueryItem("token", this->token);
    q.addQueryItem("author_id", QString::number(this->userId));

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->get(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return posts;
    }

    QJsonArray json = QJsonDocument::fromJson(reply->readAll()).array();
    qDebug() << json;
    foreach (const QJsonValue & v, json) {
        Regex r;
        r.id = v["id"].toInt();
        r.expression = v["expression"].toString();
        r.explanation = v["explanation"].toString();
        r.date = v["date"].toString();
        r.authorId = v["author_id"].toInt();
        r.views = v["views"].toInt();
        r.avgMark = v["avg_mark"].toDouble();
        posts.append(r);
    }

    return posts;
}

QList<Regex> CourseAPI::searchPosts(QString expression) {
    QList<Regex> posts;

    QUrl url = API::uri;
    url.setPath("/regex/search");

    QJsonObject jObj;
    jObj.insert("token", this->token);
    jObj.insert("regex", expression);

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->post(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return posts;
    }

    QJsonArray json = QJsonDocument::fromJson(reply->readAll()).array();
    qDebug() << json;
    foreach (const QJsonValue & v, json) {
        Regex r;
        r.id = v["id"].toInt();
        r.expression = v["expression"].toString();
        r.explanation = v["explanation"].toString();
        r.date = v["date"].toString();
        r.authorId = v["author_id"].toInt();
        r.views = v["views"].toInt();
        r.avgMark = v["avg_mark"].toDouble();
        posts.append(r);
    }

    return posts;
}

std::pair<Regex, bool> CourseAPI::getRating(int regexId) {
    Regex regex;
    regex.id = regexId;

    QUrl url = API::uri;
    url.setPath("/rating");

    QUrlQuery q;
    q.addQueryItem("token", this->token);
    q.addQueryItem("regex_id", QString::number(regexId));

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->get(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return std::make_pair(regex, false);
    }

    QJsonObject json = QJsonDocument::fromJson(reply->readAll()).object();
    regex.expression = json["expression"].toString();
    regex.explanation = json["explanation"].toString();
    regex.author = json["author"].toString();
    regex.authorId = json["author_id"].toInt();
    regex.views = json["views"].toInt();
    regex.avgMark = json["avg_mark"].toDouble();

    return std::make_pair(regex, true);
}

QList<Regex> CourseAPI::getAllPosts(int limitBy, int offsetBy) {
    QList<Regex> posts;

    QUrl url = API::uri;
    url.setPath("/rating/posts");

    QUrlQuery q;
    q.addQueryItem("token", this->token);

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->get(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return posts;
    }

    QJsonArray json = QJsonDocument::fromJson(reply->readAll()).array();
    qDebug() << json;
    foreach (const QJsonValue & v, json) {
        Regex r;
        r.id = v["id"].toInt();
        r.expression = v["expression"].toString();
        r.explanation = v["explanation"].toString();
        r.date = v["date"].toString();
        r.authorId = v["author_id"].toInt();
        r.views = v["views"].toInt();
        r.avgMark = v["avg_mark"].toDouble();
        posts.append(r);
    }

    return posts;
}

bool CourseAPI::updatePostRatings(int regexId, int userMark) {
    QUrl url = API::uri;
    url.setPath("/rating/view");

    QJsonObject jObj;
    jObj.insert("token", this->token);
    jObj.insert("regex_id", QString::number(regexId));
    jObj.insert("mark", QString::number(userMark));

    QEventLoop loop;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->put(request, QJsonDocument(jObj).toJson());

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return false;
    }

    return true;
}

QList<Regex> CourseAPI::userViewsHistory() {
    QList<Regex> posts;

    QUrl url = API::uri;
    url.setPath("/rating/history");

    QUrlQuery q;
    q.addQueryItem("token", this->token);

    url.setQuery(q);

    QEventLoop loop;
    QNetworkRequest request(url);

    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QNetworkReply *reply = manager->get(request);

    loop.exec();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusCode.toInt();
    qDebug() << status << statusCode;

    if (status != 200) {
        return posts;
    }

    QJsonArray json = QJsonDocument::fromJson(reply->readAll()).array();
    qDebug() << json;
    foreach (const QJsonValue & v, json) {
        Regex r;
        r.id = v["id"].toInt();
        r.expression = v["expression"].toString();
        r.explanation = v["explanation"].toString();
        r.date = v["date"].toString();
        r.authorId = v["author_id"].toInt();
        r.views = v["views"].toInt();
        r.avgMark = v["avg_mark"].toDouble();
        posts.append(r);
    }

    return posts;
}
