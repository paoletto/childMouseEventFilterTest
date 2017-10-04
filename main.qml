import QtQuick 2.7
import QtQuick.Window 2.2
import MyModule 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        anchors.centerIn: parent
        width: parent.width * 0.5
        height: width
        radius: width * 0.5
        color: 'firebrick'

        CircleMask {
        //Item {
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    console.log("clicked")
                }
                onPressed: {
                    console.log("pressed")
                }
                onReleased: {
                    console.log("released")
                }
                onEntered: {
                    console.log("entered")
                }
                onExited: {
                    console.log("exited")
                }
            }
        }
    }
}
