import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item {
    id: root

    required property var messages

    Pane{
        anchors.fill: parent

        background: Rectangle {
            color: "#1e1e1e"
            border.color: "#3a3a3a"
            border.width: 1
        }

        ScrollView {
            anchors.fill: parent

            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            onContentHeightChanged: {
                if (contentHeight > height) {
                    ScrollBar.vertical.position = 1.0 - ScrollBar.vertical.size
                }
            }

            ColumnLayout {
                width: parent.width
                spacing: 5

                Repeater {
                    model: root.messages

                    delegate: Column {
                        Layout.fillWidth: true
                        Layout.preferredWidth: parent.width
                        spacing: 2

                        Label {
                            text: model.timestamp
                            color: "gray"
                            font.pixelSize: 10
                            font.weight: Font.Bold
                        }

                        Label {
                            text: model.text
                            width: parent.width
                            color: {
                                switch (model.type) {
                                    case 0: return "white" // INFO
                                    case 1: return "orange" // WARNING
                                    case 2: return "red" // ERROR
                                    case 3: return "darkred" // CRITICAL
                                    default: return "white"
                                }
                            }

                            font.pixelSize: 12
                            font.weight: Font.Normal
                            lineHeight: 1.2
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }
    }
}
