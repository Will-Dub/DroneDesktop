import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item {
    id: root

    Pane{
        anchors.fill: parent

        background: Rectangle {
            color: "#1e1e1e"
            border.color: "#3a3a3a"
            border.width: 1
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 3
            radius: 7
            color: "#e0e0e0"
        }
    }
}
