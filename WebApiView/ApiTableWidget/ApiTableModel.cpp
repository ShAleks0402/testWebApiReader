#include "ApiTableModel.h"

#include <QSqlRecord>

ApiTableModel::ApiTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , _dataBase(new DataBase())
{
    connect(_dataBase.get(), &DataBase::update, this, &ApiTableModel::onUpdate);
    connect(_dataBase.get(), &DataBase::execReady, this, &ApiTableModel::onExecReady);
    _dataBase->start();
}

QVariant ApiTableModel::data(const QModelIndex& index, int role) const
{
    return QVariant();

    const auto temp = roleNames();
    const auto key =  temp.value(role);

    _data[index.row()].value(key);

    return QVariant();
}

int ApiTableModel::rowCount(const QModelIndex& parent) const
{
    return _data.size();
}

int ApiTableModel::columnCount(const QModelIndex& parent) const
{
    return roleNames().size();
}

void ApiTableModel::onUpdate()
{
    QScopedPointer<QSqlQuery> query(new QSqlQuery);

    query->prepare("SELECT api, description, link, auth, category FROM entries");
    _dataBase->exec(QScopedPointer<QSqlQuery>(query.take()));
}

void ApiTableModel::onExecReady(QSharedPointer<QSqlQuery> query)
{
    _data.clear();

    QSqlRecord rec = query->record();

    const int indexAPI          = rec.indexOf( "API" );
    const int indexDescription  = rec.indexOf( "Description" );
    const int indexAuth         = rec.indexOf( "Auth" );
    const int indexLink         = rec.indexOf( "Link" );
    const int indexCategory     = rec.indexOf( "Category" );

    while (query->next())
    {
        _data.push_back(QMap<QString, QVariant>());

        _data.back()["RolesAPI"]            = query->value(indexAPI);
        _data.back()["RolesDescription"]    = query->value(indexDescription);
        _data.back()["RolesLink"]           = query->value(indexAuth);
        _data.back()["RolesAuth"]           = query->value(indexLink);
        _data.back()["RolesCategory"]       = query->value(indexCategory);
    }
}

QHash<int, QByteArray> ApiTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    QMetaEnum metaEnum = QMetaEnum::fromType<ApiTableRoles>();
    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
        roles[metaEnum.value(i)] = QByteArray(metaEnum.key(i));
    }

    return roles;
}
