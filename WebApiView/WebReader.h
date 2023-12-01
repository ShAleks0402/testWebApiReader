#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

class WebReader : public QObject
{
    Q_OBJECT
public:
    explicit WebReader(QObject *parent = nullptr);
    void start();

signals:
    void newData(const QJsonDocument& data);

private slots:
    void onReply(QNetworkReply* reply);

private:
    QScopedPointer<QNetworkAccessManager> _netManager;
};

