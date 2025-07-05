import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item{
    id: root
    required property string labelText
    property alias checked: checkbox.checked
    property alias enabled: checkbox.enabled

    signal toggled(bool checked)

    RowLayout {
        anchors.fill: parent

        CheckBox {
            id: checkbox
            Layout.alignment: Qt.AlignVCenter

            onToggled: root.toggled(checked)

            indicator: Rectangle {
                width: 20
                height: 20
                radius: 4
                color: checkbox.checked ? "#0078d4" : "#2d2d2d"
                border.color: checkbox.checked ? "#0078d4" : "#555555"
                border.width: 2
                opacity: checkbox.enabled ? 1.0 : 0.5

                Rectangle {
                    width: 8
                    height: 8
                    x: 6
                    y: 6
                    radius: 2
                    color: "white"
                    visible: checkbox.checked
                }
            }

            Rectangle {
                anchors.fill: checkbox.indicator
                radius: checkbox.indicator.radius
                z: 1
                color: "#20ffffff"
                visible: checkbox.hovered && !checkbox.checked && checkbox.enabled
            }
        }

        Label {
            text: root.labelText
            color: root.enabled ? "#ffffff" : "#888888"
            font.family: "Arial"
            font.pixelSize: 14
            font.weight: Font.Medium
            Layout.alignment: Qt.AlignVCenter
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
