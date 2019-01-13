#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QList>
#include <QDateTime>

struct Project
{
    QString name;
    QString sourceLanguage;
    QStringList targetLanguages;
    QDateTime created;
};

#endif // PROJECT_H
