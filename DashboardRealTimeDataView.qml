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

        RowLayout{
            anchors.fill: parent
            SingleDataView {
                Layout.fillHeight: true
                Layout.fillWidth: true

                title: "Altitude"
                value: "54321"
            }
            SingleDataView {
                Layout.fillHeight: true
                Layout.fillWidth: true

                title: "Battery"
                value: "100%"
            }
            SingleDataView {
                Layout.fillHeight: true
                Layout.fillWidth: true

                title: "Latitude"
                value: "12345"
            }
        }
    }
}
