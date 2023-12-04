#pragma once

#include "../DataBase.h"

#include <QObject>
#include <QAbstractTableModel>
#include <QtCore/QMetaEnum>
#include <qqml.h>

class ApiTableModel: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum ApiTableRoles
    {
        RolesName = Qt::UserRole + 1,   // Название
        RolesWebPage,                   // Web страница
        RolesDomain,                    // Домин
        RolesCountry,                   // Регион
        RolesComments,                  // Комментарий
    };
    Q_ENUM(ApiTableRoles)

    explicit ApiTableModel(QObject *parent = 0);

    Q_INVOKABLE QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    Q_INVOKABLE QVariant itemData(int row, int column);

private slots:
    void onUpdate();
    void onExecReady(QSharedPointer<QSqlQuery> data);

private:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QMap<QString, QVariant>> _data;

    QScopedPointer<DataBase> _dataBase;
};

