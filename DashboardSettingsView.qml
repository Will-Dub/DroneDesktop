import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item {
    id: root

    required property var settings;
    required property bool isEnabled;

    signal refreshClicked()
    signal changeSettingsValue(int index, string value)

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

                anchors.margins: 10
                columnSpacing: 10
                rowSpacing: 10

                readonly property int checkboxHeight: 25

                RowLayout{
                    Layout.fillWidth: true
                    Layout.columnSpan: parent.columns
                    height: 50

                    Label {
                        text: "Settings"
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
                        enabled: root.isEnabled
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
                    model: root.settings

                    delegate: RowLayout {
                        id: element
                        property bool isSettingEnabled: root.isEnabled && !model.isChanging
                        spacing: 12

                        Label {
                            text: model.title
                            color: element.isSettingEnabled ? "#ffffff" : "#cccccc"
                            enabled: element.isSettingEnabled
                            font.family: "Arial"
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredHeight: 32
                            verticalAlignment: Text.AlignVCenter
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 32
                            Layout.alignment: Qt.AlignVCenter
                            color: element.isSettingEnabled ? "#2a2a2a" : "#1a1a1a"
                            border.color: textfield.activeFocus ? "#4CAF50" : (element.isSettingEnabled ? "#555555" : "#333333")
                            border.width: 1
                            radius: 4

                            TextField {
                                id: textfield
                                anchors.fill: parent
                                enabled: element.isSettingEnabled
                                color: element.isSettingEnabled ? "#ffffff" : "#888888"
                                font.family: "Arial"
                                font.pixelSize: 14
                                verticalAlignment: TextEdit.AlignVCenter
                                inputMethodHints: Qt.ImhDigitsOnly
                                text: model.value
                                leftPadding: 8
                                rightPadding: 8
                                topPadding: 0
                                bottomPadding: 0
                                selectByMouse: true
                                background: Rectangle {
                                    color: "transparent"
                                }
                                validator: IntValidator {
                                    bottom: 0
                                    top: 100
                                }

                                Keys.onReturnPressed: {
                                    textfield.editingFinished()
                                    changeSettingsValue(index, text)
                                }

                                Keys.onEnterPressed: {
                                    textfield.editingFinished()
                                    changeSettingsValue(index, text)
                                }

                                Keys.onEscapePressed: {
                                    textfield.editingFinished()
                                    changeSettingsValue(index, text)
                                }

                                onEditingFinished: {
                                    if (text === "") {
                                        text = "0"
                                        return
                                    }
                                    var num = parseInt(text)
                                    if (isNaN(num)) {
                                        text = "0"
                                        return
                                    }
                                    if (num < 0) {
                                        text = "0"
                                    } else if (num > 100) {
                                        text = "100"
                                    }

                                }

                                onActiveFocusChanged: {
                                    if (activeFocus) {
                                        selectAll()
                                    } else {
                                        editingFinished()
                                        changeSettingsValue(index, text)
                                    }
                                }
                            }
                        }
                    }
                }

                /*Button {
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 30
                    text: "Save"
                    enabled: root.isEnabled
                    font.bold: true
                    font.pixelSize: 16
                    Layout.columnSpan: parent.columns
                    background: Rectangle {
                        color: !parent.enabled ? "#606060" : parent.pressed ? "#3f9437" : (parent.hovered ? "#55c94b" : "#67f05b")
                        radius: 4
                        border.width: !parent.enabled ? "#424242" : parent.visualFocus ? 2 : 1
                        border.color: !parent.enabled ? "#2d2d2d" : parent.visualFocus ? "#10380c" : (parent.hovered ? "#164a10" : "transparent")
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                    onClicked: {
                        console.log("Clicked")
                    }
                }*/
            }
        }
    }
}
