import QtQuick
import QtQuick.Controls.Universal

Item {
    id: dataView

    implicitHeight: dataViewTitle.implicitHeight + valueLabel.implicitHeight + 20

    property string title
    property string value

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
            lineHeight: 1.2
            wrapMode: Text.WordWrap
            width: parent.width
            horizontalAlignment: Text.AlignHCenter

            anchors{
                horizontalCenter: parent.horizontalCenter
            }
        }

        Label{
            id: valueLabel
            text: value
            font.pixelSize: 20
            font.bold: true
            color: "white"
            lineHeight: 1.2
            wrapMode: Text.WordWrap
            width: parent.width
            horizontalAlignment: Text.AlignHCenter

            anchors{
                horizontalCenter: parent.horizontalCenter
                top: dataViewTitle.bottom
            }
        }
    }
}
