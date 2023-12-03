import QtQuick 2.15
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.15


ApplicationWindow
{
    id: window
    width: 400
    height: 400
    visible: true
    property string name:""
    property string web_page:""
    property string domain:""
    property string country:""
    property string comment:""

     ColumnLayout
     {
        Text
        {
            text: window.name
            font.pointSize: 24
            color: "red"
        }

        Button
        {
            id: saveButton
            text: "Save"
            onClicked: popup.open()
        }
     }

}
