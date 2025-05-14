import QtQuick
import QtQuick.Controls.Universal

Item {
    id: dataView

    required property string title
    required property string value

    Pane{
        anchors.fill: parent

        background: Rectangle {
            color: "#333232"
            radius: 8
        }

        Label{
            id: dataViewTitle
            text: title
            font.pixelSize: 20
            font.bold: true
            color: "#bdbbb7"

            anchors{
                horizontalCenter: parent.horizontalCenter
            }
        }

        Label{
            text: value
            font.pixelSize: 20
            font.bold: true
            color: "white"

            anchors{
                horizontalCenter: parent.horizontalCenter
                top: dataViewTitle.bottom
            }
        }
    }
}
