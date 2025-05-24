import QtQuick

Rectangle {
    property string text: ""
    property bool bold: false
    property color textColor: "#495057"

    height: parent.height
    color: "transparent"

    Text {
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        text: parent.text
        color: parent.textColor
        font.bold: parent.bold
        font.pointSize: 10
        elide: Text.ElideRight
        width: parent.width - 16
    }
}
