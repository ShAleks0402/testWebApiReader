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

Q_INVOKABLE QVariant ApiTableModel::data(const QModelIndex& index, int role) const
{
    const auto temp = roleNames();
    const auto key =  temp.value(role);

    return _data[index.row()].value(key);
}

int ApiTableModel::rowCount(const QModelIndex& parent) const
{
    return _data.size();
}

int ApiTableModel::columnCount(const QModelIndex& parent) const
{
    return roleNames().size();
}

Q_INVOKABLE QVariant ApiTableModel::itemData(int row, int column)
{
    QVariant rVal = data(this->index(row, column), column + RolesName);
    if (rVal.isValid())
        return rVal;

    return QVariant("");
}

void ApiTableModel::onUpdate()
{
    QScopedPointer<QSqlQuery> query(new QSqlQuery);

    query->prepare("SELECT entries.name, country, web_page, domain, comments.comment FROM entries "
                   "LEFT JOIN comments ON entries.name = comments.name");
    _dataBase->exec(QScopedPointer<QSqlQuery>(query.take()));
}

void ApiTableModel::onExecReady(QSharedPointer<QSqlQuery> query)
{
    _data.clear();

    QSqlRecord rec = query->record();

    const int indexName     = rec.indexOf( "name" );
    const int indexCountry  = rec.indexOf( "country" );
    const int indexWebPage  = rec.indexOf( "web_page" );
    const int indexDomain   = rec.indexOf( "domain" );
    const int indexComments = rec.indexOf( "comments" );

    beginResetModel();
    while (query->next())
    {
        _data.push_back(QMap<QString, QVariant>());

        _data.back()["RolesName"]       = query->value(indexName);
        _data.back()["RolesWebPage"]    = query->value(indexWebPage);
        _data.back()["RolesDomain"]     = query->value(indexDomain);
        _data.back()["RolesCountry"]    = query->value(indexCountry);
        _data.back()["RolesComments"]   = query->value(indexComments);
    }
    endResetModel();
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
