#include "ApiTableModel.h"

#include <QSqlRecord>

ApiTableModel::ApiTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , _dataBase(new DataBase())
{
    connect(_dataBase.get(), &DataBase::update, this, &ApiTableModel::onUpdate);

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

Q_INVOKABLE void ApiTableModel::save(int row, const QString& comment)
{
    bool newComment = !_data[row].contains("RolesComments");

    beginResetModel();
    _data[row]["RolesComments"] = comment;
    endResetModel();

    QScopedPointer<QSqlQuery> query(new QSqlQuery);
    QString queryString;
    if (newComment)
        queryString = "INSERT INTO comments (name, comment) VALUES('%1', '%2')";
    else
        queryString = "UPDATE comments SET comment='%2' WHERE name='%1'";

    queryString = queryString.arg(_data[row]["RolesName"].toString(), comment);

    query->prepare(queryString);
    _dataBase->exec(QScopedPointer<QSqlQuery>(query.take()));
}

void ApiTableModel::onUpdate()
{
    connect(_dataBase.get(), &DataBase::execReady, this, &ApiTableModel::onExecReady);
    QScopedPointer<QSqlQuery> query(new QSqlQuery);

    query->prepare("SELECT entries.name, country, web_page, domain, comments.comment FROM entries "
                   "LEFT JOIN comments ON entries.name = comments.name");
    _dataBase->exec(QScopedPointer<QSqlQuery>(query.take()));
}

void ApiTableModel::onExecReady(QSharedPointer<QSqlQuery> query)
{
    disconnect(_dataBase.get(), &DataBase::execReady, this, &ApiTableModel::onExecReady);

    _data.clear();

    QSqlRecord rec = query->record();

    const int indexName     = rec.indexOf( "name" );
    const int indexCountry  = rec.indexOf( "country" );
    const int indexWebPage  = rec.indexOf( "web_page" );
    const int indexDomain   = rec.indexOf( "domain" );
    const int indexComments = rec.indexOf( "comment" );

    beginResetModel();
    while (query->next())
    {
        _data.push_back(QMap<QString, QVariant>());

        _data.back()["RolesName"]       = query->value(indexName);
        _data.back()["RolesWebPage"]    = query->value(indexWebPage);
        _data.back()["RolesDomain"]     = query->value(indexDomain);
        _data.back()["RolesCountry"]    = query->value(indexCountry);

        const auto value = query->value(indexComments);
        if (!value.toString().isEmpty())
            _data.back()["RolesComments"] = value;
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
