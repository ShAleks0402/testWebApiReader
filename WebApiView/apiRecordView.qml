import QtQuick 2.15
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.15


ApplicationWindow
{
    id: window
    width: 400
    height: 600
    visible: true
    property string name:""
    property string web_page:""
    property string domain:""
    property string country:""
    property string comment:""

    signal qmlSaveSignal(string str)

     ColumnLayout
     {
        Text
        {
            text: window.name
            font.pointSize: 24
            color: "red"
        }

        Text
        {
            text: window.web_page
            font.pointSize: 24
            color: "red"
        }

        Text
        {
            text: window.domain
            font.pointSize: 24
            color: "red"
        }

        Text
        {
            text: window.country
            font.pointSize: 24
            color: "red"
        }

        TextInput
        {
            id: input
            anchors.fill: parent
            text: window.comment
            font.pointSize: 24
            anchors.margins: 2
            color: "green"
            focus: true

            layer.enabled: true
        }

        Button
        {
            id: saveButton
            text: "Save"

            onClicked: {

                qmlSaveSignal(input.text)
                window.close()
            }
        }
     }

}
