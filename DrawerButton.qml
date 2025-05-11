import QtQuick

Image {
    id: drawerButtonImage
    source: "menuicon.png"

    fillMode: Image.Stretch
    width: 32
    height: 32

    antialiasing: true
    smooth: true
    mipmap: true

    PropertyAnimation{
        id: scaleAnimation
        target: drawerButtonImage
        property: "scale"
        to: 1.2
        duration: 200
        easing.type: Easing.OutQuad
    }

    PropertyAnimation{
        id: resetAnimation
        target: drawerButtonImage
        property: "scale"
        to: 1
        duration: 200
        easing.type: Easing.OutQuad
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        focus: true

        onEntered: {
            scaleAnimation.start()
        }

        onExited: {
            resetAnimation.start()
        }

        onClicked: {
            drawer.open()
        }
    }
}
