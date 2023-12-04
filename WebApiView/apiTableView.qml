import QtQuick 2.15
import QtQuick.Window 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQml.Models 2.15


Item {

    id: base

    TableView
    {
        id :tableView
        anchors.fill: parent

        model: ApiTableModel
        property int row

        onDoubleClicked:
        {
            var component = Qt.createComponent("apiRecordView.qml")
            var window    = component.createObject(base)
            window.name = ApiTableModel.itemData(row, "0")
            window.web_page = ApiTableModel.itemData(row, "1")
            window.domain = ApiTableModel.itemData(row, "2")
            window.country = ApiTableModel.itemData(row, "3")
            window.comment = ApiTableModel.itemData(row, "4")
            tableView.row = row;

            window.qmlSaveSignal.connect(onSave)

            window.show()
        }

        function onSave(str) {
            console.log("Sending to email: ")
            ApiTableModel.save(tableView.row, str)
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


