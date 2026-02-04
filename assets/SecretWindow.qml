import QtQuick
import QtQuick.Controls
import QtQuick.Window

Window {
    id: secretWindow
    signal signalExit
    
    width: 360
    height  : 640
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    
    Button {
        text: qsTr("Back")
        width: 180
        height: 50
        anchors.centerIn: parent
        onClicked: {
            secretWindow.signalExit()
        }
    }
}
