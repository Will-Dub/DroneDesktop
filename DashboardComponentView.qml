import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item {
    id: root

    required property var components;
    required property bool isRefreshEnabled;

    signal refreshClicked()
    signal toggleComponentStatus(int index)

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
                        enabled: root.isRefreshEnabled
                        onPressed: refreshClicked()

                        background: Rectangle {
                            color: !parent.enabled ? "#2e2e2e" : parent.pressed ? "#404040" : parent.hovered ? "#2a2a2a" : "transparent"
                            radius: 6
                            border.color: !parent.enabled ? "transparent" : parent.hovered ? "#555555" : "transparent"
                            border.width: 1
                        }

                        Layout.preferredWidth: 30
                        Layout.preferredHeight: 30
                    }
                }

                Repeater{
                    model: root.components

                    delegate: CustomCheckbox{
                        text: model.name
                        isTristate: true
                        isEnabled: model.status === 1 || model.status === 3
                        checkState: {
                            switch(model.status){
                            case 0:
                            case 1:
                                return Qt.Unchecked
                            case 2:
                                return Qt.PartiallyChecked
                            case 3:
                            case 4:
                                return Qt.Checked
                            default:
                                return Qt.Unchecked
                            }
                        }
                        Layout.fillWidth: true
                        height: parent.checkboxHeight
                        onClicked: root.toggleComponentStatus(index)
                    }
                }
            }
        }
    }
}
