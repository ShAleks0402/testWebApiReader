#pragma once

#include "../DataBase.h"

#include <QObject>
#include <QAbstractTableModel>
#include <QtCore/QMetaEnum>

class ApiTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ApiTableRoles
    {
        RolesAPI = Qt::UserRole + 1,   // Название
        RolesDescription,              // Описание
        RolesLink,                     // Ссылка
        RolesAuth,                     // Авторизация
        RolesCategory,                 // Категория
        RolesComments,                 // Комментарий
    };
    Q_ENUM(ApiTableRoles)

    explicit ApiTableModel(QObject *parent = 0);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

private slots:
    void onUpdate();
    void onExecReady(QSharedPointer<QSqlQuery> data);

private:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<QMap<QString, QVariant>> _data;

    QScopedPointer<DataBase> _dataBase;
};

