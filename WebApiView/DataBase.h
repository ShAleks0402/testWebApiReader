#pragma once

#include "WebReader.h"

#include <QObject>
#include <QSqlQuery>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT

public:

public slots:
    void doWork(QSharedPointer<QSqlQuery> query);

signals:
    void resultReady(QSharedPointer<QSqlQuery> query);
};

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
    void operate(QSharedPointer<QSqlQuery> query);

private slots:
    void handleResults(QSharedPointer<QSqlQuery> query);

private slots:
    void onUpdate(QSharedPointer<QJsonDocument> data);

private:
    void parseJson(QSharedPointer<QJsonDocument> data);
    void createDB();

private:
    QScopedPointer<WebReader> _webReader;

    QString _dbFileName;

    QSqlDatabase _sdb;

    QScopedPointer<Worker> _worker;
    QScopedPointer<QThread> _workerThread;
};

