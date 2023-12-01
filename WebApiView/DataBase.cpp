#include "DataBase.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QApplication>

#include <QtSql>

DataBase::DataBase(QObject *parent)
    : QObject{parent}
    , _webReader(new WebReader())
    , _dbFileName(QApplication::applicationDirPath() + "/db.sqlite")
{
    if (!QFile::exists(_dbFileName))
        createDB();

    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);
    if (!_sdb.open())
        qDebug()<<"ERROR. DataBase. DB not open.";

    connect(_webReader.get(), &WebReader::newData, this, &DataBase::onUpdate);
    _webReader->start();
}

DataBase::~DataBase()
{
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
        return;

    query->exec();

    emit execReady(QSharedPointer<QSqlQuery>(query.take()));
}

void DataBase::onUpdate(const QJsonDocument& data)
{
    //parseJson(data);

    emit update();
}

void DataBase::parseJson(const QJsonDocument& data)
{
    if (!_sdb.isOpen())
    {
        qDebug()<<"ERROR. parseJson. DB not open.";
        return;
    }

    QSqlQuery query;
    query.exec("DELETE FROM entries");

    if (data.isObject())
    {
        const auto json = data.object();
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
