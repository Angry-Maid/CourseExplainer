#include <QList>
#include <QString>

#include "courseapi.h"

CourseAPI::CourseAPI() {}

bool CourseAPI::regUser(QString username, QString password) {
    return;
}

bool CourseAPI::authorize(QString username, QString password) {
    return;
}

bool CourseAPI::refreshToken() {
    return;
}

bool CourseAPI::exitUser() {
    return;
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
    return;
}

pair<Regex, bool> CourseAPI::editRegex(QString expression) {
    pair<Regex, bool> regexInfo;

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
    return;
}

QList<Regex> CourseAPI::userViewsHistory() {
    QList<Regex> posts;

    return posts;
}
