import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 640
    height: 480
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

    Drawer {
        id: drawer
        width: 0.3 * window.width
        height: window.height

        ListView{
            id: drawerListView
            currentIndex: 0
            anchors.fill: parent
            model: navigationModel

            delegate: ItemDelegate{
                width: parent.width
                text: model.displayText
                highlighted: drawerListView.currentIndex === index

                onClicked: {
                    drawerListView.currentIndex = index

                    contentLabel.text = model.displayText
                }
            }
        }
    }

    DrawerButton{
        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 10
        }
    }

    // Contenue
    Item {
        anchors.fill: parent

        Label {
            id: contentLabel
            anchors.centerIn: parent
            text: "Selected: Home"
            font.pixelSize: 24
        }
    }
}
