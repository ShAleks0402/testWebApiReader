#include "WebReader.h"

#include <QDebug>

WebReader::WebReader(QObject *parent)
    : QObject{parent}
    , _netManager(new QNetworkAccessManager())
{
    connect(_netManager.get(), &QNetworkAccessManager::finished, this, &WebReader::onReply);
}

void WebReader::start()
{
    _netManager->get(QNetworkRequest(QUrl("https://api.publicapis.org/entries")));
}

void WebReader::onReply(QNetworkReply* reply)
{
    if (reply->error())
    {
        qDebug() << QString("Error %1").arg(reply->errorString());
        return;
    }

    emit newData(QSharedPointer<QJsonDocument>(
                     new QJsonDocument(QJsonDocument::fromJson(reply->readAll()))
                     )
                 );
}
