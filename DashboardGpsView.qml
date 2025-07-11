import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtLocation
import QtPositioning

Item {
    id: root

    required property geoCoordinate droneLocation

    onDroneLocationChanged: {
        if (droneLocation.latitude !== 0 || droneLocation.longitude !== 0) {
            map.center = droneLocation
        }
    }

    Plugin
    {
        id: mapPlugin
        name: "osm"
        /*PluginParameter {
            name: "osm.mapping.custom.host"
            value: "https://tile.openstreetmap.org/"
        }*/
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter {
            name: "osm.mapping.custom.host"
            value: "https://tile.thunderforest.com/street/{z}/{x}/{y}.png?apikey=00fe4dd6a0dc40a6ace16ae7769c94f7"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        zoomLevel: 13
        copyrightsVisible: false
        activeMapType: map.supportedMapTypes[0]

        property geoCoordinate startCentroid
        property bool isNavigating: false
        property real minZoomLevel: 2
        property real maxZoomLevel: 20
        property real zoomStep: 0.1
        property real panSensitivity: 1.0
        property real rotationSensitivity: 1.0

        MapQuickItem {
            id: dronePoint
            coordinate: droneLocation
            anchorPoint.x: droneMarker.width / 2
            anchorPoint.y: droneMarker.height

            sourceItem: Rectangle {
                id: droneMarker
                width: 20
                height: 20
                radius: 10
                color: "#FF4444"
                border.color: "#FFFFFF"
                border.width: 2

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width + 2
                    height: parent.height + 2
                    radius: parent.radius + 1
                    color: "#40000000"
                    z: -1
                }

                SequentialAnimation on scale {
                    running: true
                    loops: Animation.Infinite
                    NumberAnimation { from: 1.0; to: 1.2; duration: 800; easing.type: Easing.InOutQuad }
                    NumberAnimation { from: 1.2; to: 1.0; duration: 800; easing.type: Easing.InOutQuad }
                }
            }
        }

        Behavior on zoomLevel {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutQuad
            }
        }

        Behavior on bearing {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        PinchHandler {
            id: pinch
            target: null
            minimumScale: 0.5
            maximumScale: 3.0
            minimumRotation: -180
            maximumRotation: 180

            onActiveChanged: {
                if (active) {
                    map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                    map.isNavigating = true
                } else {
                    map.isNavigating = false
                }
            }

            onScaleChanged: (delta) => {
                const newZoomLevel = map.zoomLevel + Math.log2(delta)
                if (newZoomLevel >= map.minZoomLevel && newZoomLevel <= map.maxZoomLevel) {
                    map.zoomLevel = newZoomLevel
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
            }

            onRotationChanged: (delta) => {
                map.bearing -= delta * map.rotationSensitivity
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }

            grabPermissions: PointerHandler.TakeOverForbidden
        }

        WheelHandler {
            id: wheel
            acceptedDevices: {
                if (Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland") {
                    return PointerDevice.Mouse | PointerDevice.TouchPad
                }
                return PointerDevice.Mouse
            }

            property: "zoomLevel"
            rotationScale: 5/120

            onWheel: (event) => {
                const delta = event.angleDelta.y / 120
                const newZoomLevel = map.zoomLevel + (delta * map.zoomStep)

                if (newZoomLevel >= map.minZoomLevel && newZoomLevel <= map.maxZoomLevel) {
                    map.zoomLevel = newZoomLevel

                    if (event.position) {
                        const coord = map.toCoordinate(event.position, false)
                        map.alignCoordinateToPoint(coord, event.position)
                    }
                }
            }
        }

        DragHandler {
            id: drag
            target: null

            property point lastPosition
            property real velocity: 0
            property real friction: 0.95

            onActiveChanged: {
                if (active) {
                    map.isNavigating = true
                    lastPosition = centroid.position
                } else {
                    map.isNavigating = false
                    if (velocity > 0.5) {
                        momentumTimer.start()
                    }
                }
            }

            onTranslationChanged: (delta) => {
                const scaledDelta = Qt.point(delta.x * map.panSensitivity, delta.y * map.panSensitivity)
                map.pan(-scaledDelta.x, -scaledDelta.y)

                const distance = Math.sqrt(delta.x * delta.x + delta.y * delta.y)
                velocity = distance
            }
        }

        Timer {
            id: momentumTimer
            interval: 16
            repeat: true
            onTriggered: {
                if (drag.velocity > 0.1) {
                    drag.velocity *= drag.friction
                } else {
                    stop()
                }
            }
        }

        Shortcut {
            enabled: map.zoomLevel < map.maxZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.min(map.zoomLevel + 1, map.maxZoomLevel)
        }

        Shortcut {
            enabled: map.zoomLevel > map.minZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.max(map.zoomLevel - 1, map.minZoomLevel)
        }

        Shortcut {
            sequence: "Ctrl+0"
            onActivated: {
                map.zoomLevel = 13
                map.center = QtPositioning.coordinate(45.5, -73.5)
                map.bearing = 0
            }
        }

        Shortcut {
            sequence: "R"
            onActivated: map.bearing = 0
        }

        // Arrow navigation
        Shortcut {
            sequence: "Up"
            onActivated: map.pan(0, -50)
        }

        Shortcut {
            sequence: "Down"
            onActivated: map.pan(0, 50)
        }

        Shortcut {
            sequence: "Left"
            onActivated: map.pan(-50, 0)
        }

        Shortcut {
            sequence: "Right"
            onActivated: map.pan(50, 0)
        }

        // Double-tap to zoom
        TapHandler {
            onDoubleTapped: {
                const coord = map.toCoordinate(point.position, false)
                map.zoomLevel = Math.min(map.zoomLevel + 2, map.maxZoomLevel)
                map.center = coord
            }
        }

        // Context menu
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton) {
                    // TODO add menu
                }
            }
        }

        // Reduce updates during navigation
        onIsNavigatingChanged: {
            if (isNavigating) {
                map.copyrightsVisible = false
            } else {
                map.copyrightsVisible = false
            }
        }

        function zoomIn() {
            map.zoomLevel = Math.min(map.zoomLevel + 1, map.maxZoomLevel)
        }

        function zoomOut() {
            map.zoomLevel = Math.max(map.zoomLevel - 1, map.minZoomLevel)
        }

        function resetView() {
            map.zoomLevel = 13
            map.center = QtPositioning.coordinate(45.5, -73.5)
            map.bearing = 0
        }

        function fitToCoordinates(coordinates) {
            if (coordinates.length > 0) {
                map.fitViewportToMapItems(coordinates)
            }
        }
    }
}
