import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 960
    visible: true
    title: qsTr("Drone application")

    // Connect usb popup
    Popup {
        id: connectPopup
        anchors.centerIn: parent
        width: window.width / 2
        height: window.height / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout{
            anchors.fill: parent

            Label {
                text: drone.connected ? "Disconnect a usb device" : "Connect a usb device"
                font.pixelSize: 30
                font.bold: true
                font.weight: Font.Medium
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            RowLayout{
                // Refresh btn
                Button{
                    icon.source: "refresh.png"
                    icon.width: 24
                    icon.height: 24
                    onClicked: {
                        drone.onRefreshUsbDevices()
                    }
                }

                // Filler
                Item{
                    Layout.fillWidth: true
                }

                CheckBox {
                    text: "Autoconnect usb on start"

                    Component.onCompleted: {
                        checked = drone.isUsbAutoConnect
                    }

                    onCheckedChanged: {
                        drone.isUsbAutoConnect = checked
                    }
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
                            textColor: "#2c3e50"
                        }
                        DataColumn {
                            width: 150
                            text: modelData.manufacturer || "N/A"
                            textColor: "#2c3e50"
                        }
                        DataColumn {
                            width: 120
                            text: modelData.serialNumber || "N/A"
                            textColor: "#2c3e50"
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

                Rectangle {
                    anchors.fill: parent
                    visible: listViewUsbDevice.count === 0
                    color: "#ffffff"
                    border.color: "#bdc3c7"
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: "No usb device connected"
                        color: "#7f8c8d"
                        font.pixelSize: 14
                        font.italic: true
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
                        connectPopup.close();
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
                            drone.connectToUsb(portName);
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
                        const response = drone.disconnectUsb();
                    }
                }

                /*Button {
                    text: "Send packet"

                    onClicked: {
                        const response = drone.sendDataTest();
                    }
                }*/
            }
        }
    }

    // Connect gamepad popup
    Popup {
        id: gamepadPopup
        anchors.centerIn: parent
        width: window.width / 2
        height: window.height / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout{
            anchors.fill: parent

            Label {
                text: drone.gamepadConnected ? "Disconnect a gamepad" : "Connect a gamepad"
                font.pixelSize: 30
                font.bold: true
                font.weight: Font.Medium
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Action buttons
            RowLayout{
                Layout.fillWidth: true

                // Refresh btn
                Button{
                    icon.source: "refresh.png"
                    icon.width: 24
                    icon.height: 24
                    visible: !drone.gamepadConnected
                    onClicked: {
                        drone.onRefreshGamepadDevices()
                    }
                }

                // Gamepad setup
                Button{
                    icon.source: "settings.png"
                    icon.width: 24
                    icon.height: 24
                    visible: drone.gamepadConnected
                    onClicked: {
                        gamepadSetupPopup.open()
                    }
                }

                // Filler
                Item{
                    Layout.fillWidth: true
                }

                CheckBox {
                    text: "Autoconnect gamepad on start"

                    Component.onCompleted: {
                        checked = drone.isGamepadAutoConnect
                    }

                    onCheckedChanged: {
                        drone.isGamepadAutoConnect = checked
                    }
                }
            }

            // List of gamepad devices
            ListView {
                id: listViewGamepadDevice

                Layout.fillHeight: true
                Layout.fillWidth: true

                model: drone.gamepadDevices
                currentIndex: -1

                clip: true

                enabled: !drone.gamepadConnected

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
                            text: "GUID"
                        }
                        HeaderColumn {
                            width: 200
                            text: "Name"
                        }
                        HeaderColumn {
                            width: 150
                            text: "Type"
                        }
                    }
                }

                delegate: Rectangle {
                    width: ListView.view.width
                    height: 40

                    color: modelData.isConnected ? "#5de37a" : ListView.isCurrentItem ? "#3498db" : (gamepadMouseArea.containsMouse ? "#ecf0f1" : "#ffffff")
                    border.color: ListView.isCurrentItem ? "#2980b9" : "#bdc3c7"
                    border.width: 1

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15

                        DataColumn {
                            width: 100
                            text: modelData.guid || "N/A"
                            bold: true
                            textColor: "#2c3e50"
                        }
                        DataColumn {
                            width: 200
                            text: modelData.name || "N/A"
                            textColor: "#2c3e50"
                        }
                        DataColumn {
                            width: 150
                            text: modelData.type || "N/A"
                            textColor: "#2c3e50"
                        }
                    }

                    MouseArea {
                        id: gamepadMouseArea
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

                Rectangle {
                    anchors.fill: parent
                    visible: listViewGamepadDevice.count === 0
                    color: "#ffffff"
                    border.color: "#bdc3c7"
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: "No udb device connected"
                        color: "#7f8c8d"
                        font.pixelSize: 14
                        font.italic: true
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
                        gamepadPopup.close();
                    }
                }

                // Connect btn
                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    enabled: listViewGamepadDevice.currentIndex >= 0
                    visible: !drone.gamepadConnected;
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
                        const index = listViewGamepadDevice.currentIndex

                        if (index >= 0) {
                            const joystickId = listViewGamepadDevice.model[index].id;
                            drone.connectToGamepad(joystickId);
                        }
                    }
                }

                // Disconnect btn
                Button {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    visible: drone.gamepadConnected;
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
                        const response = drone.disconnectGamepad();
                    }
                }

                /*Button {
                    text: "Send packet"

                    onClicked: {
                        const response = drone.sendDataTest();
                    }
                }*/
            }
        }
    }

    // Gamepad setup popup
    Popup {
        id: gamepadSetupPopup
        anchors.centerIn: parent
        width: window.width / 2
        height: window.height / 2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        ColumnLayout{
            anchors.fill: parent

            Label {
                text: "Setup a gamepad"
                font.pixelSize: 30
                font.bold: true
                font.weight: Font.Medium
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Processed input
            Pane{
                Layout.fillWidth: true

                ColumnLayout{
                    anchors.fill: parent

                    Label {
                        text: "Processed input"
                        font.pixelSize: 20
                        font.bold: true
                        font.weight: Font.Medium
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Repeater {
                            model: 16

                            RadioButton {
                                id: gamepadButton
                                text: index.toString()
                                visible: drone.gamepadButtonInput[index] === true
                                checked: true
                                enabled: false
                            }
                        }
                    }

                    GridLayout{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 2
                        rows: 2

                        Repeater {
                            model: 6

                            delegate: ColumnLayout {
                                Layout.fillWidth: true

                                Label {
                                    text: "Axis " + index
                                }

                                Slider {
                                    id: slider
                                    Layout.fillWidth: true
                                    from: -100
                                    to: 100
                                    value: drone.gamepadAxisInput[index]
                                    enabled: false
                                }

                                Label {
                                    text: Math.round(slider.value) + "%"
                                    font.pixelSize: 12
                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }
                        }
                    }
                }
            }

            // Edit processing
            Pane{
                Layout.fillWidth: true
                ColumnLayout{
                    Layout.fillWidth: parent

                    Label {
                        text: "Edit processing"
                        font.pixelSize: 20
                        font.bold: true
                        font.weight: Font.Medium
                    }

                    RowLayout{
                        ColumnLayout{
                            Label{
                                text: "Maximum percent"
                            }

                            TextField {
                                id: maxGamepadAxisPercentageTextField
                                placeholderText: "0–100"
                                inputMethodHints: Qt.ImhDigitsOnly
                                text: drone.maxGamepadAxisPercentage

                                validator: IntValidator {
                                    bottom: 0
                                    top: 100
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

                                    focus = false

                                    drone.maxGamepadAxisPercentage = num
                                }

                                onActiveFocusChanged: {
                                    if (activeFocus) {
                                        selectAll()
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Layout filler
            Item{
                Layout.fillHeight: true
            }

            // Bottom action row
            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Button {
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 40
                    text: "Back"
                    font.bold: true
                    font.pixelSize: 24
                    background: Rectangle {
                        color: parent.pressed ? "#302f2f" : (parent.hovered ? "#525252" : "#757575")
                        radius: 4
                        border.width: parent.visualFocus ? 2 : 1
                        border.color: parent.visualFocus ? "#242424" : (parent.hovered ? "#525252" : "transparent")
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                    onClicked: {
                        gamepadSetupPopup.close();
                    }
                }
            }
        }
    }

    // Top header
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
        spacing: -1

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

                // Connect usb btn
                Button{
                    Layout.fillWidth: true
                    Layout.margins: 10
                    Layout.bottomMargin: 0
                    Layout.preferredHeight: 50

                    background: Rectangle {
                        color: parent.pressed ? "#1972c2" : (parent.hovered ? "#0276de" : "#0086ff")
                        radius: 4
                        border.width: parent.visualFocus ? 2 : 0
                        border.color: parent.visualFocus ? "#074175" : (parent.hovered ? "#0968bd" : "transparent")

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 150 }
                        }
                    }

                    contentItem: RowLayout{
                        clip: true
                        Image{
                            source: "link.png"
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 16
                        }
                        Label{
                            text: drone.connected ? "Disconnect" : "Connect"
                            font.pixelSize: 24
                            font.bold: true
                            color: "black"
                            elide: Text.ElideRight
                            Layout.alignment: Qt.AlignLeft
                        }
                    }

                    onClicked: {
                        connectPopup.open()
                    }
                }

                // Connect gamepad
                Button{
                    Layout.fillWidth: true
                    Layout.margins: 10
                    Layout.topMargin: 0
                    Layout.preferredHeight: 50

                    background: Rectangle {
                        id: gamepadBackground
                        color: parent.pressed ? "#dc2626" : (parent.hovered ? "#b82121" : "#991b1b")
                        radius: 4
                        border.width: parent.visualFocus ? 2 : 0
                        border.color: parent.visualFocus ? "#7f1d1d" : "transparent"

                        Behavior on color {
                            ColorAnimation { duration: 200; easing.type: Easing.OutCubic }
                        }
                        Behavior on border.color {
                            ColorAnimation { duration: 200; easing.type: Easing.OutCubic }
                        }
                    }

                    contentItem: RowLayout{
                        clip: true
                        Image{
                            source: "gamepad.png"
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            Layout.alignment: Qt.AlignVCenter
                            Layout.leftMargin: 16
                        }
                        Label{
                            text: drone.gamepadConnected ? "Disconnect" : "Connect"
                            font.pixelSize: 24
                            font.bold: true
                            color: "white"
                            elide: Text.ElideRight
                            Layout.alignment: Qt.AlignLeft
                        }
                    }

                    onClicked: {
                        gamepadPopup.open()
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

                    SidebarCheckbox{
                        labelText: "Logs"
                        mouseArea.onClicked: stackView.currentItem.isLogViewVisible = !stackView.currentItem.isLogViewVisible
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

                    SidebarCheckbox{
                        labelText: "Component Status"
                        mouseArea.onClicked: stackView.currentItem.isComponentViewVisible = !stackView.currentItem.isComponentViewVisible
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
            property bool isGpsViewVisible: true
            property bool isRealTimeDataViewVisible: true
            property bool isVideoViewVisible: true
            property bool isLogViewVisible: true
            property bool isCalibrationViewVisible: true
            property bool isComponentViewVisible: true

            spacing: 0

            anchors.fill: parent

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0
                visible: isGpsViewVisible || isRealTimeDataViewVisible

                /*
                DashboardGpsView {
                    Layout.preferredHeight: parent.height * 0.8
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isGpsViewVisible
                }

                DashboardRealTimeDataView {
                    Layout.preferredHeight: parent.height * 0.2
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isRealTimeDataViewVisible
                }*/
            }

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 0
                visible: isVideoViewVisible || isCalibrationViewVisible || isComponentViewVisible || isLogViewVisible

                /*DashboardVideoView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isVideoViewVisible
                }
                DashboardVideoView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isCalibrationViewVisible
                }*/
                DashboardComponentView {
                    components: drone.components;
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: isComponentViewVisible
                    isRefreshEnabled: drone.connected
                    onToggleComponentStatus: function(index){
                        drone.toggleComponentStatus(index)
                    }
                    onRefreshClicked: {
                        drone.refreshComponentStatus()
                    }
                }
                DashboardLogView {
                    messages: drone.logs;
                    Layout.preferredHeight: 300
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    visible: isLogViewVisible
                }
            }
        }
    }
}
