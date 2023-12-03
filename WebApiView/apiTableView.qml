import QtQuick 2.15
import QtQuick.Controls 1.5


Item {

    id: base

    TableView
    {
        id :tableView
        anchors.fill: parent

        onDoubleClicked:
        {
            var component = Qt.createComponent("apiRecordView.qml")
            var window    = component.createObject(base)
            window.show()
            console.error(window.errorString());
        }

        TableViewColumn
        {
            role: "RolesName"
            title: "Название"
        }

        TableViewColumn
        {
            role: "RolesWebPage"
            title: "Web страница"
        }

        TableViewColumn
        {
            role: "RolesComments"
            title: "Комментарий"
        }

        model: ApiTableModel
    }
}


