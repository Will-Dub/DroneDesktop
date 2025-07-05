import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item {
    id: root

    Pane {
        anchors.fill: parent
        padding: 16

        background: Rectangle {
            color: "#1e1e1e"

            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#00ffffff" }
                    GradientStop { position: 1.0; color: "#10ffffff" }
                }
            }
        }

        ScrollView{
            id: scrollview
            anchors.fill: parent
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            clip: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            GridLayout {
                width: scrollview.width
                columns: Math.max(1, Math.floor(scrollview.width / 200))

                readonly property int checkboxHeight: 25

                RowLayout{
                    Layout.fillWidth: true
                    Layout.columnSpan: parent.columns
                    height: 50

                    Label {
                        text: "Components"
                        color: "#ffffff"
                        font.family: "Arial"
                        font.pixelSize: 22
                        font.weight: Font.Medium
                    }

                    Button {
                        icon.source: "refresh.png"
                        icon.width: 24
                        icon.height: 24
                        icon.color: "#ffffff"

                        background: Rectangle {
                            color: parent.pressed ? "#404040" : parent.hovered ? "#2a2a2a" : "transparent"
                            radius: 6
                            border.color: parent.hovered ? "#555555" : "transparent"
                            border.width: 1
                        }

                        Layout.preferredWidth: 30
                        Layout.preferredHeight: 30
                    }
                }

                CustomCheckbox{
                    labelText: "Motors"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Motor 1"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Motor 2"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Motor 3"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Motor 4"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Get motor informations"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Use MPU 6050"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Use QMC 5883l"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Use GPS"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
                CustomCheckbox{
                    labelText: "Get logs"
                    Layout.fillWidth: true
                    height: parent.checkboxHeight
                }
            }
        }
    }
}
