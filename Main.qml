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
            id: sidebar
            Layout.minimumWidth: 50
            Layout.preferredWidth: 0.2 * window.width
            Layout.fillHeight: true
            padding: 0
            background: Rectangle {
                color: "#252525"
                border.color: "#3a3a3a"
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

                SidebarSection{
                    text: "Dashboard"
                }

                // Dashboard section
                ColumnLayout{
                    spacing: 0
                    SidebarCheckbox{
                        labelText: "GPS"
                        mouseArea.onClicked: stackView.currentItem.isGpsViewVisible = !stackView.currentItem.isGpsViewVisible
                        isOn: true
                    }

                    SidebarCheckbox{
                        labelText: "Stats"
                        mouseArea.onClicked: stackView.currentItem.isRealTimeDataViewVisible = !stackView.currentItem.isRealTimeDataViewVisible
                        isOn: true
                    }

                    SidebarCheckbox{
                        labelText: "Video"
                        mouseArea.onClicked: stackView.currentItem.isVideoViewVisible = !stackView.currentItem.isVideoViewVisible
                        isOn: true
                    }
                }

                SidebarSection{
                    text: "Controls"
                }

                // Control section
                ColumnLayout{
                    spacing: 0
                    SidebarCheckbox{
                        labelText: "Calibration"
                        mouseArea.onClicked: stackView.currentItem.isCalibrationViewVisible = !stackView.currentItem.isCalibrationViewVisible
                        isOn: true
                    }
                }

                SidebarSection{
                    text: "Tools"
                }

                // Control section
                ColumnLayout{
                    spacing: 0
                    SidebarCheckbox{
                        labelText: "Drone settings"
                        mouseArea.onClicked: stackView.currentItem.isDroneSettingsViewVisible = !stackView.currentItem.isDroneSettingsViewVisible
                        isOn: true
                    }
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
            property alias gpsView: gpsView
            property alias realTimeDataView: realTimeDataView
            property alias videoView: videoView
            property alias calibrationViewVisible: calibrationViewVisible
            property alias droneSettingsViewVisible: droneSettingsViewVisible
            property bool isGpsViewVisible: true
            property bool isRealTimeDataViewVisible: true
            property bool isVideoViewVisible: true
            property bool isCalibrationViewVisible: true
            property bool isDroneSettingsViewVisible: true

            spacing: 0

            anchors.fill: parent

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0
                visible: isGpsViewVisible || isRealTimeDataViewVisible

                DashboardGpsView {
                    id: gpsView
                    Layout.preferredHeight: parent.height * 0.8
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isGpsViewVisible
                }

                DashboardRealTimeDataView {
                    id: realTimeDataView
                    Layout.preferredHeight: parent.height * 0.2
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isRealTimeDataViewVisible
                }
            }

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0
                visible: isVideoViewVisible || isCalibrationViewVisible || isDroneSettingsViewVisible

                DashboardVideoView {
                    id: videoView
                    Layout.preferredHeight: parent.height * 0.8
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isVideoViewVisible
                }
                DashboardVideoView {
                    id: calibrationViewVisible
                    Layout.preferredHeight: parent.height * 0.8
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isCalibrationViewVisible
                }
                DashboardVideoView {
                    id: droneSettingsViewVisible
                    Layout.preferredHeight: parent.height * 0.8
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isDroneSettingsViewVisible
                }
            }
        }
    }
}
