#include "connector.h"
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <functional>

using namespace std;

Connector::Connector(QObject *parent):
    QObject(parent),
    networkManager(new QNetworkAccessManager(this))
{
}

void Connector::get(const QString path, const RequestParams &requestParams, const ProcessResponse &processResponse)
{
    QString server{"https://cloud.memsource.com/web"};

    QUrlQuery query;

    for (RequestParams::const_iterator it = requestParams.constBegin(), end = requestParams.constEnd(); it != end; ++it)
    {
        query.addQueryItem(it.key(), it.value());
    }

    QUrl serviceUrl{server + path};
    serviceUrl.setQuery(query);

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2);

    QNetworkRequest request{serviceUrl};
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = networkManager->get(request);

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
