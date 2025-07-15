import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item{
    id: root
    property string text
    property var checkState
    property bool isEnabled: true
    property bool isTristate: false

    signal clicked()

    RowLayout {
        anchors.fill: parent

        CheckBox {
            id: checkbox
            Layout.alignment: Qt.AlignVCenter
            tristate: root.isTristate
            enabled: root.isEnabled
            checkState: root.checkState

            onClicked: root.clicked()

            indicator: Rectangle {
                width: 20
                height: 20
                radius: 4
                color: checkbox.enabled
                       ? (checkbox.checked ? "#0078d4" : "#2d2d2d")
                       : (checkbox.checked ? "#8bc5f7" : "#9d9d9d")
                border.color: !checkbox.enabled ? "#555555" : (checkbox.checked ? "#0078d4" : "#555555")
                border.width: 2
                opacity: checkbox.enabled ? 1.0 : 0.5
                anchors.verticalCenter: parent.verticalCenter

                Rectangle {
                    width: 8
                    height: 8
                    x: 6
                    y: 6
                    radius: 2
                    color: "white"
                    visible: checkbox.checked
                }

                Rectangle {
                    width: 8
                    height: 4
                    anchors.centerIn: parent
                    radius: 2
                    color: "gray"
                    visible: checkbox.checkState == Qt.PartiallyChecked
                }
            }

            Rectangle {
                anchors.fill: checkbox.indicator
                radius: checkbox.indicator.radius
                z: 1
                color: "#20ffffff"
                visible: checkbox.hovered && checkbox.enabled
            }
        }

        Label {
            text: root.text
            Layout.fillHeight: true
            color: root.isEnabled ? "#ffffff" : "#cccccc"
            font.family: "Arial"
            font.pixelSize: 14
            font.weight: Font.Medium
            verticalAlignment: Text.AlignVCenter
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
