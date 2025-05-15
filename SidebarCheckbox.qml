import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Rectangle {
    property bool isOn: false
    required property string labelText
    property alias mouseArea: checkboxMouseArea

    Layout.fillWidth: true
    Layout.preferredHeight: 48

    color: isOn ? "#9ff5ad" : "#333232"
    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    border.width: 3
    border.color: isOn ? "#0af531" : "#424242"
    Behavior on border.color {
        ColorAnimation {
            duration: 200
        }
    }

    Label {
        anchors{
            left: parent.left
            leftMargin: 32
            verticalCenter: parent.verticalCenter
        }

        text: labelText
        font.pixelSize: 14
        color: parent.isOn ? "black" : "white"
        elide: Text.ElideRight
        font.weight: parent.isOn ? Font.Medium : Font.Normal
    }

    MouseArea{
        id: checkboxMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            parent.isOn = !parent.isOn
        }
    }
}
