#include <QList>
#include <QString>

#include "courseapi.h"

CourseAPI::CourseAPI() {}

bool CourseAPI::regUser(QString username, QString password) {
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
