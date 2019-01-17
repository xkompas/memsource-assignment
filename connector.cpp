#include "connector.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <functional>

using namespace std;

const QString Connector::serverUrl{"https://cloud.memsource.com/web"};

Connector::Connector(QObject *parent):
    QObject(parent),
    networkManager(new QNetworkAccessManager(this))
{
}

void Connector::get(const QString path, const RequestParams &requestParams, const ProcessResponse &processResponse)
{
    QUrlQuery query;

    for (RequestParams::const_iterator it = requestParams.constBegin(), end = requestParams.constEnd(); it != end; ++it)
    {
        query.addQueryItem(it.key(), it.value());
    }

    QUrl serviceUrl{serverUrl + path};
    serviceUrl.setQuery(query);

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2);

    QNetworkRequest request{serviceUrl};
    request.setSslConfiguration(sslConfig);


    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, std::bind(&Connector::processReply, this, reply, processResponse));
}

void Connector::post(const QString path, const RequestParams &requestParams, const ProcessResponse &processResponse)
{
    QJsonDocument json;
    QJsonObject data;

    for (RequestParams::const_iterator it = requestParams.constBegin(), end = requestParams.constEnd(); it != end; ++it)
    {
        data[it.key()] = it.value();
    }

    json.setObject(data);
    QByteArray jsonPost = QJsonDocument(data).toJson();

    QUrl serviceUrl{serverUrl + path};

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2);

    QNetworkRequest request{serviceUrl};
    request.setSslConfiguration(sslConfig);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json; charset=utf-8"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(jsonPost.size()));

    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    QNetworkReply *reply = networkManager->post(request, jsonPost);

    connect(reply, &QNetworkReply::finished, std::bind(&Connector::processReply, this, reply, processResponse));
}

void Connector::processReply(QNetworkReply *reply, const ProcessResponse &processResponse)
{
    Response response{processReplyImpl(reply)};
    processResponse(response);
}

Response Connector::processReplyImpl(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        return processError(reply);
    }

    QString strReply = static_cast<QString>(reply->readAll());

    Response response;

    QJsonParseError parseError;
    response.jsonDocument = QJsonDocument::fromJson(strReply.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        response.errorMessage = "Error parsing JSON";
        return response;
    }

    response.ok = true;
    return response;
}

Response Connector::processError(const QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QString errorReason{tr("Unknown error")};

    if (statusCode.isValid())
    {
        auto status = statusCode.toInt();
        if (status != 200)
        {
            errorReason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        }
    }

    Response response;
    response.errorMessage = errorReason;

    return response;
}
