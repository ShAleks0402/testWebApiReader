#include "ApiTableWidget.h"

#include <QQmlEngine>
#include <QQmlContext>

ApiTableWidget::ApiTableWidget(QWidget *parent)
    : QQuickWidget(parent)
    , _model(new ApiTableModel())
{
    engine()->rootContext()->setContextProperty("ApiTableModel", _model.get());
    setSource(QUrl("qrc:/apiTableView.qml"));

}
