import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Calculator

Window {
    id:mainWindow
    width: 360
    height  : 640
    
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    
    visible: true
    title: qsTr("Calculator")
    property int buttonSpacing : 24
    property int buttonColumnLeftMarging : 24
    
    Calculator{
        id:calculator
    }

    property bool btnDotClicked: false
    property bool btnPlusMinusClicked: false
    property bool leftBracket: false
    property bool rightBracket: false

    property bool commandLineMode: false

    property int numberCounter: 0
    property int max_input_integer_number: 25
    property int max_input_fractional_number: 2
    property string error_message : "Incorrect expression"
    property string command_line_string: "CM:"
    function isNumber(name){
        return !Number.isNaN(Number(name))
    }

    function btnNumberClicked(content)
    {
        if(commandLineMode)
        {
            calculationLine.text += content
            calculationResult.text = command_line_string+calculationLine.text

            if(calculationLine.text === "123" && calculationLine.text.length === 3)
            {
                //disableCommandLineMode()
                showSecretWindowTimer.running = true
            }
            return
        }
        var calculationLineTextLength = calculationLine.text.length;

        btnPlusMinusClicked = false
        if(content === ".")
        {
            if(btnDotClicked && calculationLine.text.lastIndexOf(".") > 0)
                return;

            btnDotClicked = true

            if(calculationLineTextLength > 0 && calculationLine.text[calculationLineTextLength-1] === '.')
                return

            if(calculationLineTextLength === 0 || !isNumber(calculationLine.text[calculationLineTextLength-1]))
            {
                calculationLine.text += "0.";
                numberCounter++;
                return
            }
        }

        var dotIndex = btnDotClicked ? calculationLine.text.lastIndexOf(".") : -1
        if(numberCounter === max_input_integer_number && content !== '.' && dotIndex < 0)
            return
        

        calculationLine.text += content
        if(content !== '.')
            numberCounter++;

        btnEqu(false)
    }

    function btnOperationClicked(content)
    {
        if(commandLineMode)
            return

        if(content === "=")
        {
            btnEqu()
            if(calculationResult.text !== calculator.getErrorMessageString())
            {
                calculationLine.text = calculationResult.text
                calculationResult.text = ""
            }
            else
            {
                calculationResult.text = calculator.getErrorMessageString()
            }
            numberCounter = 0
        }
        else if(content === "()")
        {
            if(!leftBracket)
            {
                calculationLine.text += '('
                leftBracket = true
            }
            else if(leftBracket && !rightBracket)
            {
                calculationLine.text += ')'
                rightBracket = true
            }

            if(leftBracket && rightBracket)
            {
                leftBracket = false
                rightBracket = false
            }
            numberCounter = 0
            btnDotClicked = false
            return
        }
        else
        {
            var calculationLineTextLength = calculationLine.text.length;
            if(calculationLineTextLength === 0 && content !== "+/-")
            {
                return
            }

            var text = calculationLine.text;
            if(content === "+/-" )
            {
                if(!btnPlusMinusClicked)
                    calculationLine.text += "-"
                else
                    calculationLine.text = text.slice(0,-1)

                btnPlusMinusClicked = !btnPlusMinusClicked
                numberCounter = 0
                return
            }
            var lastChar = text[calculationLineTextLength-1];

            if(lastChar === "+" || lastChar === "-" || lastChar === "*" || lastChar === "/")
            {
                calculationLine.text = text.slice(0,-1) + content

                return
            }
            calculationLine.text += content
            numberCounter = 0
            btnDotClicked = false
            btnEqu(false)
        }
    }

    function btnEqu(errorFlag = true)
    {
        var pair = calculator.calculate(calculationLine.text)
        if(pair.first || (!pair.first && !errorFlag))
        {
            calculationResult.text = pair.second
        }
        else if(errorFlag && calculationLine.text.length > 0)
        {
            calculationResult.text = error_message
        }
    }

    function enableCommandLineMode()
    {
        commandLineMode = true
        disableCommandLineModeTimer.running = true
        calculationResult.text = "CM:"
        calculationLine.text = ""
    }

    function disableCommandLineMode()
    {
        commandLineMode = false
        disableCommandLineModeTimer.running = false
        calculationResult.text = ""
        calculationLine.text = ""
    }

    function btnEquLongHoldPress()
    {
        enableCommandLineMode()
    }

    function showSecretWindow()
    {
        secretWindow.show()
        mainWindow.hide()
    }

    function btnCancelClicked()
    {
        calculationLine.text = ""
        calculationResult.text = ""
        btnPlusMinusClicked = false
        leftBracket = false
        rightBracket = false
        numberCounter = 0
        btnDotClicked = false
        if(commandLineMode)
            calculationResult.text = command_line_string
    }

    SecretWindow {
        id: secretWindow
        title: qsTr("Secret menu")
        onSignalExit: {
            secretWindow.close()
            mainWindow.show()
        }
    }

    Rectangle
    {
        id : windowBgr
        anchors {
            right: parent.right
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        color: "#024873"
    }

    Item {
        id: calculationField
        width: 360
        height: 180
        anchors.top : parent.top

        Rectangle
        {
            id : backCalculation
            width: calculationField.width
            height: calculationField.height
            color: "#04bfad"
            radius: 25
        }
        FontLoader {
            id: customFontLoader
            source: "fonts/open_sans_semibold.ttf"
        }
        Text{
            id : calculationLine
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.bottom: calculationResult.top
            anchors.bottomMargin: 8
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignBottom
            width: 280
            height: 30

            
            font.family: customFontLoader.name
            font.pixelSize:  20
            color:  "#ffffff"
            fontSizeMode: Text.HorizontalFit
        }
        Text{
            id : calculationResult
            anchors.right: calculationField.right
            anchors.rightMargin: 35
            anchors.bottom: calculationField.bottom
            anchors.bottomMargin: 14
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter

            width: 281
            height: 60

            
            font.family: customFontLoader.name

            font.pixelSize:  50
            font.weight: Font.Normal
            color:  "#ffffff"
            fontSizeMode:  Text.HorizontalFit
        }
    }

    Image {
        width: 360
        height: 24
        source: "images/status_bar.png"
    }
    Item {
        id: buttons
        width: 312
        height: 396
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 39
        anchors.horizontalCenter: parent.horizontalCenter

        Timer {
                id: showSecretWindowTimer
                interval: 500
                repeat: false
                onTriggered:
                {
                    disableCommandLineMode()
                    showSecretWindow()
                }
            }

        Timer {
            id: disableCommandLineModeTimer

            interval: 5000
            repeat: false
            running: false

            onTriggered: disableCommandLineMode()
        }

        Timer {
            id: equButtonPressTimer

            interval: 4000
            repeat: false
            running: false

            onTriggered: btnEquLongHoldPress()
        }

        GridLayout {
            columnSpacing: buttonSpacing
            rowSpacing: buttonSpacing
            columns: 4
            rows: 5
            Repeater {
                model: [
                    {id:"()",source:"images/operations/bkt.png"},
                    {id:"+/-",source:"images/operations/plus_minus.png"},
                    {id:"%",source:"images/operations/procent.png"},
                    {id:"/",source:"images/operations/division.png"},

                    {id:"7"}, {id:"8"}, {id:"9"},{id:"*",source:"images/operations/multiplication.png"},
                    {id:"4"}, {id:"5"}, {id:"6"},{id:"-",source:"images/operations/minus.png"},
                    {id:"1"}, {id:"2"}, {id:"3"},{id:"+",source:"images/operations/plus.png"},
                    {id:"C"}, {id:"0"}, {id:"."},{id:"=",source:"images/operations/equal.png"}
                ]
                CalculationButton {name : modelData.id ; source: modelData.source ;
                    onClicked:{
                        {
                            if(isNumber(name)|| name === ".")
                                btnNumberClicked(name)
                            else if(name === "C")
                                btnCancelClicked()
                            else
                                btnOperationClicked(name)
                        }
                    }
                    onPressedChanged: {
                        if(name !== "=")
                            return

                        if ( pressed )
                            equButtonPressTimer.running = true;
                        else
                            equButtonPressTimer.running = false;
                    }
                }
            }
        }
    }
}
