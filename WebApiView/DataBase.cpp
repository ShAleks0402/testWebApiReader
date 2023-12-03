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

// TODO: уйти от query, QScopedPointer заставляет отдать управление.
void DataBase::exec(QScopedPointer<QSqlQuery> query)
{
    if (!_sdb.isOpen())
    {
        qDebug()<<"ERROR. exec. DB not open.";
        return;
    }

    //_mutex.lock(); TODO: для отладки
    // TODO: по хорошенму нужен пул потоков.
    emit operate(QSharedPointer<QSqlQuery>(query.take()));
}

void DataBase::handleResults(QSharedPointer<QSqlQuery> query)
{
    //_mutex.unlock(); TODO: для отладки
    emit execReady(query);
}

void DataBase::onUpdate(QSharedPointer<QJsonDocument> data)
{
    parseJson(data);

    emit update();
}

void DataBase::parseJson(QSharedPointer<QJsonDocument> data)
{
    if (!_sdb.isOpen())
    {
        qDebug()<<"ERROR. parseJson. DB not open.";
        return;
    }

    // TODO: по хорошему надо выставлять visible в false.
    QSqlQuery query;
    query.exec("DELETE FROM entries");

    if (data->isArray())
    {
        const auto jsonEntries = data->array();

        for (const auto& jsonEntrie: jsonEntries)
        {
            if (jsonEntrie.isObject())
            {
                QString queryString("INSERT INTO entries (name, country, web_page, domain) "
                                    "VALUES ('%1', '%2', '%3', '%4')");

                // TODO: По хорошему надо проверять наличие Array.
                try
                {
                    queryString = queryString.arg(jsonEntrie.toObject()["name"].toString(),
                                                  jsonEntrie.toObject()["country"].toString(),
                                                  jsonEntrie.toObject()["web_pages"].toArray().begin()->toString(),
                                                  jsonEntrie.toObject()["domains"].toArray().at(0).toString()
                                                  );
                }
                catch(...)
                {
                }

                if (!query.exec(queryString))
                    qDebug()<<"ERROR. parseJson. INSERT INTO.";
            }
        }
    }
}

// TODO: нужно вынести обработку в отдельный поток.
void DataBase::createDB()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(_dbFileName);

    if (sdb.open())
    {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE entries (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                              "name TEXT,"
                                              "web_page TEXT,"
                                              "domain TEXT,"
                                              "country TEXT,"
                                              "visible BOOL)"))
            qDebug()<<"ERROR. createDB. CREATE TABLE entries.";

        if (!query.exec("CREATE TABLE comments (name TEXT,"
                        "comment TEXT)"))
            qDebug()<<"ERROR. createDB. CREATE TABLE comments.";

        sdb.close();
    }
}

// TODO: уйти от QSqlQuery, передавать QString. Придется открывать файл с базой.
void Worker::doWork(QSharedPointer<QSqlQuery> query)
{
    query->exec();

    emit resultReady(query);
}
