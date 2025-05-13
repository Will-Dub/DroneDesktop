import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 960
    visible: true
    title: qsTr("Drone application")


    ListModel{
        id: navigationModel
        ListElement {
            displayText: "Home"
        }
        ListElement {
            displayText: "Options"
        }
    }

    Popup {
        id: popup
        anchors.centerIn: parent
        width: window.width / 2
        height: window.height / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        Label {
            text: "Connect"
            font.pixelSize: 30
            font.bold: true
            font.weight: Font.Medium
            anchors{
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 30
            }
        }
    }

    header: ToolBar {
        id: mainToolBar
        height: 56

        RowLayout {
            anchors.fill: parent
            spacing: 8

            Label {
                id: headerTitle
                text: "Drone Application"
                font.pixelSize: 20
                font.weight: Font.Medium
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: settingsButton
                icon.source: "settings.png"
                icon.width: 24
                icon.height: 24
                ToolTip.visible: hovered
                ToolTip.text: "Settings"
            }

            ToolButton {
                id: connectButton
                icon.source: "link.png"
                icon.width: 24
                icon.height: 24

                background: Rectangle {
                    color: connectButton.pressed ? "#3b8e3c" : (connectButton.hovered ? "#5dc961" : "#4CAF50")
                    radius: 4
                    border.width: connectButton.visualFocus ? 2 : 1
                    border.color: connectButton.visualFocus ? "#0078d7" : (connectButton.hovered ? "#75d679" : "transparent")

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                    Behavior on border.color {
                        ColorAnimation { duration: 150 }
                    }
                }

                ToolTip.visible: hovered
                ToolTip.text: "Connect"

                onClicked: {
                    popup.open()
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Pane{
            Layout.preferredWidth: 0.2 * window.width
            Layout.fillHeight: true
            padding: 0
            background: Rectangle {
                color: "#f5f5f5"
                border.color: "#e0e0e0"
                border.width: 1
            }

            ListView {
                id: drawerListView
                anchors.fill: parent
                currentIndex: 0
                model: navigationModel
                clip: true

                delegate: ItemDelegate {
                    width: parent.width
                    height: 48

                    Label {
                        anchors{
                            left: parent.left
                            leftMargin: 16
                            verticalCenter: parent.verticalCenter
                        }

                        text: model.displayText
                        font.pixelSize: 14
                        elide: Text.ElideRight
                        color: drawerListView.currentIndex === index ? "#2196F3" : "#424242"
                        font.weight: drawerListView.currentIndex === index ? Font.Medium : Font.Normal
                    }

                    background: Rectangle {
                        color: drawerListView.currentIndex === index ? "#e3f2fd" : "transparent"

                        Rectangle {
                            width: 4
                            height: parent.height
                            anchors.left: parent.left
                            color: drawerListView.currentIndex === index ? "#2196F3" : "transparent"
                        }
                    }

                    onClicked: {
                        drawerListView.currentIndex = index
                    }
                }
            }
        }

        // Main
        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            StackView {
                id: stackView
                Layout.fillWidth: true
                Layout.fillHeight: true
                initialItem: dashboardPage
            }
        }
    }

    Component {
        id: dashboardPage

        RowLayout{
            anchors.fill: parent

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0

                Loader {
                    Layout.preferredHeight: parent.height * 0.8
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    sourceComponent: gpsView
                }

                Loader {
                    Layout.preferredHeight: parent.height * 0.2
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    sourceComponent: realTimeDataView
                }
            }
        }
    }

    Component {
        id: gpsView

        Pane{
            anchors.fill: parent

            background: Rectangle {
                color: "#1e1e1e"
                border.color: "#3a3a3a"
                border.width: 1
            }

            Rectangle {
                anchors.fill: parent
                anchors.margins: 3
                radius: 7
                color: "#e0e0e0"
            }
        }
    }

    Component {
        id: realTimeDataView

        Pane{
            anchors.fill: parent

            background: Rectangle {
                color: "#292827"
                border.color: "#3a3a3a"
                border.width: 1
            }

            RowLayout{
                anchors.fill: parent
                Loader {
                    sourceComponent: dataView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Loader {
                    sourceComponent: dataView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Loader {
                    sourceComponent: dataView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
    }

    Component {
        id: dataView

        Pane{
            anchors.fill: parent

            background: Rectangle {
                color: "#333232"
                radius: 8
            }

            Label{
                id: dataViewTitle
                text: "Altitude"
                font.pixelSize: 20
                font.bold: true
                color: "#bdbbb7"

                anchors{
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Label{
                text: "78 m"
                font.pixelSize: 20
                font.bold: true
                color: "white"

                anchors{
                    horizontalCenter: parent.horizontalCenter
                    top: dataViewTitle.bottom
                }
            }
        }
    }
}
