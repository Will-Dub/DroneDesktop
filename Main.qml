import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 960
    visible: true
    title: qsTr("Drone application")

    Popup {
        id: popup
        anchors.centerIn: parent
        width: window.width / 2
        height: window.height / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout{
            anchors.fill: parent

            Label {
                text: drone.connected ? "Connect" : "Disconnect"
                font.pixelSize: 30
                font.bold: true
                font.weight: Font.Medium
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Refresh btn
            Button{
                icon.source: "refresh.png"
                icon.width: 24
                icon.height: 24
                onClicked: {
                    drone.onRefreshUsbDevices()
                }
            }

            // List of usb devices
            ListView {
                id: listViewUsbDevice

                Layout.fillHeight: true
                Layout.fillWidth: true

                model: drone.usbDevices
                currentIndex: -1

                clip: true

                enabled: !drone.connected

                header: Rectangle {
                    width: parent.width
                    height: 45
                    color: "#252525"

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15

                        HeaderColumn {
                            width: 100
                            text: "Port"
                        }
                        HeaderColumn {
                            width: 200
                            text: "System Location"
                        }
                        HeaderColumn {
                            width: 150
                            text: "Manufacturer"
                        }
                        HeaderColumn {
                            width: 120
                            text: "Serial Number"
                        }
                    }
                }

                delegate: Rectangle {
                    width: ListView.view.width
                    height: 40

                    color: ListView.isCurrentItem ? "#3498db" : (mouseArea.containsMouse ? "#ecf0f1" : "#ffffff")
                    border.color: ListView.isCurrentItem ? "#2980b9" : "#bdc3c7"
                    border.width: 1

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15

                        DataColumn {
                            width: 100
                            text: modelData.portName || "N/A"
                            bold: true
                            textColor: "#2c3e50"
                        }
                        DataColumn {
                            width: 200
                            text: modelData.systemLocation || "N/A"
                        }
                        DataColumn {
                            width: 150
                            text: modelData.manufacturer || "N/A"
                        }
                        DataColumn {
                            width: 120
                            text: modelData.serialNumber || "N/A"
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            if(parent.ListView.view.currentIndex !== model.index){
                                parent.ListView.view.currentIndex = model.index
                            }else{
                                parent.ListView.view.currentIndex = -1
                            }
                        }
                    }
                }
            }

            // Bottom action row
            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                // Cancel btn
                Button {
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: "Cancel"
                    font.bold: true
                    font.pixelSize: 24
                    background: Rectangle {
                        color: parent.pressed ? "#a61e1e" : (parent.hovered ? "#c92a2a" : "#e03131")
                        radius: 4
                        border.width: parent.visualFocus ? 2 : 1
                        border.color: parent.visualFocus ? "#862e2e" : (parent.hovered ? "#c92a2a" : "transparent")
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                    onClicked: {
                        popup.close();
                    }
                }

                // Connect btn
                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    enabled: listViewUsbDevice.currentIndex >= 0
                    visible: !drone.connected;
                    text: "Connect"
                    font.bold: true
                    font.pixelSize: 24
                    background: Rectangle {
                        color: !parent.enabled ? "#404142" : parent.pressed ? "#2d8a2f" : (parent.hovered ? "#2e7d32" : "#4caf50")
                        radius: 4
                        border.width: parent.visualFocus && parent.enabled ? 2 : 1
                        border.color: parent.visualFocus && parent.enabled ? "#1b5e20" : (parent.hovered && parent.enabled ? "#2d8a2f" : "transparent")
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                    onClicked: {
                        const index = listViewUsbDevice.currentIndex

                        if (index >= 0) {
                            const portName = listViewUsbDevice.model[index].portName;
                            drone.connectToDrone(portName);
                        }
                    }
                }

                // Disconnect btn
                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    visible: drone.connected;
                    text: "Disconnect"
                    font.bold: true
                    font.pixelSize: 24
                    background: Rectangle {
                        color: parent.pressed ? "#d32f2f" : (parent.hovered ? "#f44336" : "#ff5722")
                        radius: 4
                        border.width: parent.visualFocus ? 2 : 1
                        border.color: parent.visualFocus ? "#b71c1c" : (parent.hovered ? "#d32f2f" : "transparent")
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                    onClicked: {
                        const response = drone.disconnectDrone();
                    }
                }

                /*Button {
                    text: "Send packet"

                    onClicked: {
                        const response = drone.sendDataTest();
                    }
                }*/

                Connections {
                    target: drone
                    function onConnectedChanged() {
                        console.log("Connection status changed:", drone.connected);
                    }
                }
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

                    text: drone.connected ? "Disconnect" : "Connect"
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
                        labelText: "Gps"
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
        }

        // Main
        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            StackView {
                background: Rectangle {
                    color: "#1e1e1e"
                    border.color: "#3a3a3a"
                    border.width: 1
                }

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
