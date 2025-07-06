import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Layouts

Item {
    id: root

    Pane{
        anchors.fill: parent

        background: Rectangle {
            color: "#1e1e1e"
            border.color: "#3a3a3a"
            border.width: 1
        }

        ScrollView{
            id: scrollview
            anchors.fill: parent
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            clip: true

            GridLayout{
                width: scrollview.width
                columns: Math.max(1, Math.floor(scrollview.width / 200))

                SingleDataView {
                    Layout.preferredHeight: 100
                    Layout.fillWidth: true

                    title: "Altitude"
                    value: "54321"
                }
                SingleDataView {
                    Layout.preferredHeight: 100
                    Layout.fillWidth: true

                    title: "Battery"
                    value: "100%"
                }
                SingleDataView {
                    Layout.preferredHeight: 100
                    Layout.fillWidth: true

                    title: "Latitude"
                    value: "12345"
                }
            }
        }
    }
}
