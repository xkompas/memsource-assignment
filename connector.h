#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <functional>
#include "response.h"

QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager)
QT_FORWARD_DECLARE_CLASS(QNetworkReply)

using RequestParams = QMap<QString, QString>;
using ProcessResponse = std::function<void(Response)>;

class Connector : public QObject
{
    Q_OBJECT

public:
    explicit Connector(QObject *parent = nullptr);
    void get(const QString path, const RequestParams &requestParams, const ProcessResponse &processResponse);

private:
    QNetworkAccessManager *networkManager;

    void processReply(QNetworkReply *reply, const ProcessResponse &processResponse);

    Response processReplyImpl(QNetworkReply *reply);

    Response processError(const QNetworkReply *reply);

};

#endif
