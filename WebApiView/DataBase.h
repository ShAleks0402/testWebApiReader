#pragma once

#include "WebReader.h"

#include <QObject>
#include <QSqlQuery>

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void start();
    void exec(QScopedPointer<QSqlQuery> query);

signals:
    void update();
    void execReady(QSharedPointer<QSqlQuery> query);

private slots:
    void onUpdate(const QJsonDocument& data);

private:
    void parseJson(const QJsonDocument& data);
    void createDB();

private:
    QScopedPointer<WebReader> _webReader;

    QString _dbFileName;

    QSqlDatabase _sdb;
};

