import QtQuick 2.15
import QtQuick.Controls 1.5

Item {

    TableView
    {
        id :tableView
        anchors.fill: parent

        TableViewColumn
        {
            role: "RolesAPI"
            title: "Название"
        }

        TableViewColumn
        {
            role: "RolesCategory"
            title: "Категория"
        }

        model: ApiTableModel
    }
}
