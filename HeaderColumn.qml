import QtQuick

Rectangle {
    property string text: ""
    height: parent.height
    color: "transparent"

    Text {
        anchors.centerIn: parent
        text: parent.text
        color: "white"
        font.bold: true
        font.pointSize: 11
    }

    Rectangle {
        width: 1
        height: parent.height * 0.6
        color: "#7f8c8d"
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        visible: parent !== parent.parent.children[parent.parent.children.length - 1]
    }
}
