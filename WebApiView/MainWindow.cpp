#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "ApiTableWidget/ApiTableWidget.h"

struct MainWindow::Ui : public Ui_MainWindow
{
    Ui(QMainWindow * window)
        : _apiTableWidget(new ApiTableWidget())
    {
        setupUi(window);

        _apiTableWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        gridLayout->addWidget(_apiTableWidget.data(), 1, 0);
    }

    QScopedPointer<ApiTableWidget> _apiTableWidget;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui(this))
{
}

MainWindow::~MainWindow()
{
}

