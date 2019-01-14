#ifndef RESPONSE_H
#define RESPONSE_H

#include <QString>
#include <QJsonDocument>

struct Response
{
    bool ok;
    QString errorMessage;
    QJsonDocument jsonDocument;
};

#endif // RESPONSE_H
