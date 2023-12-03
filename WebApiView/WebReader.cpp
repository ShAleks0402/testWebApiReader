#include "WebReader.h"

#include <QDebug>
#include <QNetworkReply>

WebReader::WebReader(QObject *parent)
    : QObject{parent}
    , _netManager(new QNetworkAccessManager())
{
    connect(_netManager.get(), &QNetworkAccessManager::finished, this, &WebReader::onReply);
}

void WebReader::start()
{
    // Работает асинхронно
    _netManager->get(QNetworkRequest(QUrl("http://universities.hipolabs.com/search?country=Kazakhstan")));

    // TODO: список бесплатных API. Есть проблемы по запросу (SSL). Разобраться почему на некоторых ПК выдает reply->error().
    //_netManager->get(QNetworkRequest(QUrl("https://api.publicapis.org/entries")));
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
