import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Button {
    id : button
    property var name: btnText.text
    property var source : icon.source

    property int buttonW : 60
    property int buttonH : 60

    icon.source: source
    icon.width: 30
    icon.height: 30
    icon.color:  "#ffffff"
    Layout.preferredWidth: buttonW
    Layout.preferredHeight: buttonH

    background: Item{
        id : outerBgr
        anchors.fill: parent
        Rectangle{
            id : backBgr
            anchors.fill: parent
            radius: 30
            opacity:
            {
               name === "C" ? 1 : 0
            }
        }

        Rectangle{
            id : btnBgr
            anchors.fill: parent
            radius: 50
            height: 100
            width: 100

            color:
            {
                if(isNumber(name)|| name === ".")
                {
                    button.down ? "#04bfad" : "#b0d1d8"
                }
                else if(name === "C")
                {
                    color: button.down ? "#f25e5e" : "#f25e5e"
                }
                else
                {
                    button.down ? "#f7e425" : "#0889a6"
                }
            }

            opacity:
            {
                if(name === "C")
                    button.down ? 0.5 : 1
                else
                    1
            }

            Text{
                id : btnText
                font.family: customFontLoader.name;
                font.pixelSize:  24
                anchors.centerIn: parent
                text:
                {
                    if(isNumber(name) || name === "C" || name === ".")
                        name
                    else
                        ""
                }

                color:
                {
                    if(isNumber(name) || name === ".")
                        button.down ? "#ffffff" : "#024873"
                    else if(name === "C")
                        "#ffffff"
                    else
                        ""
                }
            }
        }
    }
}
