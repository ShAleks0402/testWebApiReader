import QtQuick 2.15
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1


Item {

    id: base

    TableView
    {
        id :tableView
        anchors.fill: parent

        model: ApiTableModel

        onDoubleClicked:
        {
            var component = Qt.createComponent("apiRecordView.qml")
            var window    = component.createObject(base)
            var idx = ApiTableModel.index(row, 0)
            console.log(model.data(idx, "RolesName"))

            window.show()
            console.error(row);
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



    }
}


