#pragma once

#include "ApiTableModel.h"

#include <QQuickWidget>
#include <QObject>

class ApiTableWidget : public QQuickWidget
{
    Q_OBJECT
public:
    ApiTableWidget(QWidget* parent = nullptr);

private:
    QScopedPointer<ApiTableModel> _model;
};

