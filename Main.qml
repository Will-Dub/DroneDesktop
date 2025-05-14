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

            ColumnLayout{
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 20

                Button{
                    Layout.fillWidth: true
                    Layout.margins: 10
                    Layout.preferredHeight: 50

                    text: "Connect"
                    font.bold: true
                    font.pixelSize: 24

                    background: Rectangle {
                        color: parent.pressed ? "#1972c2" : (parent.hovered ? "#0276de" : "#0086ff")
                        radius: 4
                        border.width: parent.visualFocus ? 2 : 1
                        border.color: parent.visualFocus ? "#074175" : (parent.hovered ? "#0968bd" : "transparent")

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }

                    Image{
                        source: "link.png"
                        width: 24
                        height: 24

                        anchors{
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 15
                        }
                    }

                    onClicked: {
                        popup.open()
                    }
                }

                Label {
                    Layout.leftMargin: 20

                    text: "Dashboard"

                    font.pixelSize: 18
                    color: "#7b7c7b"
                }
            }
            /*ListView {
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
            }*/
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
                SingleDataView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    title: "Altitude"
                    value: "54321"
                }
                SingleDataView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    title: "Battery"
                    value: "100%"
                }
                SingleDataView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    title: "Latitude"
                    value: "12345"
                }
            }
        }
    }
}
