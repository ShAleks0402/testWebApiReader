#include "DataBase.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QApplication>

#include <QtSql>

#include <QMutex>
QMutex _mutex;

DataBase::DataBase(QObject *parent)
    : QObject{parent}
    , _webReader(new WebReader())
    , _dbFileName(QApplication::applicationDirPath() + "/db.sqlite")
    , _worker(new Worker())
    , _workerThread(new QThread())
{
    if (!QFile::exists(_dbFileName))
        createDB();

    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);
    if (!_sdb.open())
        qDebug()<<"ERROR. DataBase. DB not open.";

    qRegisterMetaType<QSharedPointer<QSqlQuery>>("QSharedPointer<QSqlQuery>");
    _worker->moveToThread(_workerThread.get());
    connect(_workerThread.get(), &QThread::finished, _worker.get(), &QObject::deleteLater);
    connect(this, &DataBase::operate, _worker.get(), &Worker::doWork);
    connect(_worker.get(), &Worker::resultReady, this, &DataBase::handleResults);
    _workerThread->start();

    connect(_webReader.get(), &WebReader::newData, this, &DataBase::onUpdate);
    _webReader->start();
}

DataBase::~DataBase()
{
    _workerThread->quit();
    _workerThread->wait();

    if (_sdb.isOpen())
        _sdb.close();
}

void DataBase::start()
{
    _webReader->start();
}

void DataBase::exec(QScopedPointer<QSqlQuery> query)
{
    if (!_sdb.isOpen())
    {
        qDebug()<<"ERROR. exec. DB not open.";
        return;
    }

    //_mutex.lock();
    emit operate(QSharedPointer<QSqlQuery>(query.take()));
}

void DataBase::handleResults(QSharedPointer<QSqlQuery> query)
{
    //_mutex.unlock();
    emit execReady(query);
}

void DataBase::onUpdate(QSharedPointer<QJsonDocument> data)
{
    //parseJson(data);

    emit update();
}

void DataBase::parseJson(QSharedPointer<QJsonDocument> data)
{
    if (!_sdb.isOpen())
    {
        qDebug()<<"ERROR. parseJson. DB not open.";
        return;
    }

    QSqlQuery query;
    query.exec("DELETE FROM entries");

    if (data->isObject())
    {
        const auto json = data->object();
        //const auto elementsCount = json["count"].toInt();

        const auto jsonEntries = json["entries"].toArray();
        for (const auto& jsonEntrie: jsonEntries)
        {
            if (jsonEntrie.isObject())
            {
                QString queryString("INSERT INTO entries (api, description, link, auth, category) "
                                    "VALUES ('%1', '%2', '%3', '%4', '%5')");
                queryString = queryString.arg(jsonEntrie.toObject()["API"].toString(),
                                              jsonEntrie.toObject()["Description"].toString(),
                                              jsonEntrie.toObject()["Auth"].toString(),
                                              jsonEntrie.toObject()["Link"].toString(),
                                              jsonEntrie.toObject()["Category"].toString());

                if (!query.exec(queryString))
                    qDebug()<<"ERROR. parseJson. INSERT INTO.";
            }
        }
    }
}

void DataBase::createDB()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(_dbFileName);

    if (sdb.open())
    {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE entries (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                              "api TEXT,"
                                              "description TEXT,"
                                              "link TEXT,"
                                              "auth TEXT,"
                                              "category TEXT,"
                                              "visible BOOL)"))
            qDebug()<<"ERROR. createDB. CREATE TABLE entries.";

        if (!query.exec("CREATE TABLE comments (api TEXT,"
                        "comment TEXT)"))
            qDebug()<<"ERROR. createDB. CREATE TABLE comments.";

        sdb.close();
    }
}

void Worker::doWork(QSharedPointer<QSqlQuery> query)
{
    query->exec();

    emit resultReady(query);
}
