    import QtQuick 2.0
    import QtQuick.Controls 1.1
    import QtQuick.Controls.Styles 1.0
    import QtQuick.Dialogs 1.1
    import econ.camera.uvcsettings 1.0
    import econ.camera.see3cam130 1.0


    Item {
    width:268
    height:720

    Action {
    id: firmwareVersion
    onTriggered:
        {
            getFirmwareVersion()
        }
    }

    Action {
    id: serialNumber
    onTriggered:
        {
            getSerialNumber()
        }
    }

    Text {
    id: scene_mode
    text: "--- Scene Mode ---"
    font.pixelSize: 14
    font.family: "Ubuntu"
    color: "#ffffff"
    smooth: true
    x: 79
    y: 194.5
    opacity: 0.50196078431373
    }

    Text {
    id: special_effects
    text: "--- Special Effects ---"
    font.pixelSize: 14
    font.family: "Ubuntu"
    color: "#ffffff"
    smooth: true
    x: 81
    y: 356//376.5
    opacity: 0.50196078431373
    }

    Text {
    id: other_optionsCU130
    text: "--- Other Options ---"
    font.pixelSize: 14
    font.family: "Ubuntu"
    color: "#ffffff"
    smooth: true
    x: 72
    y: 520
    opacity: 0.50196078431373
    }


    Button {
    id: serial_no_selected
    x: 85
    y: 542
    opacity: 1
    action: serialNumber
    activeFocusOnPress : true
    tooltip: "Click to view the Serial Number"
    style: ButtonStyle {
    background: Rectangle {
    border.width: control.activeFocus ? 3 :0
    color: "#222021"
    border.color: control.activeFocus ? "#ffffff" : "#222021"
    radius: 5
    }
    label: Image {
    source: "images/serial_no_selected.png"
    }
    }
    Keys.onReturnPressed: {
        getSerialNumber()
    }
    }

    Button {
    id: f_wversion_selectedCU130
    x: 85
    y: 592
    opacity: 1
    action: firmwareVersion
    activeFocusOnPress : true
    tooltip: "Click to view the firmware version of the camera"
    style: ButtonStyle {
    background: Rectangle {
    border.width: control.activeFocus ? 3 :0
    color: "#222021"
    border.color: control.activeFocus ? "#ffffff" : "#222021"
    radius: 5
    }
    label: Image {
    source: "images/f_wversion_selected.png"
    }
    }
    Keys.onReturnPressed: {
    getFirmwareVersion()
    }
    }

    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        onAccepted: {
            close()
        }
        Component.onCompleted: close()
    }

    Component {
        id: econSliderStyle
        SliderStyle {
            groove:Row {
                spacing: 0
                y: 3
                Rectangle {
                    width: styleData.handlePosition
                    height: 4
                    color: "#dc6239"
                    radius: 5
                }
                Rectangle {
                    width: control.width - styleData.handlePosition
                    height: 4
                    color: "#dddddd"
                    radius: 5
                }
            }
            handle: Image {
                source: "images/handle.png"
                opacity: 1
            }
        }
    }
    Component {
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 40
                implicitHeight: 20
                border.color: "#333"
                border.width: 2
                y: 1
            }
        }
    }
    Component {
    id: uvc130_radioButtonStyle
    RadioButtonStyle {
    label: Text {
    x: 5
    width: 60
    height: 30
    renderType: Text.NativeRendering
    verticalAlignment: Text.AlignVCenter
    font.pixelSize: 14
    font.family: "Ubuntu"
    color: "white"
    text: control.text
    wrapMode: Text.WordWrap
    }
    }
    }
    Component {
    id: uvc130_scrollViewStyle
    ScrollViewStyle {
        scrollToClickedPosition: true
        handle: Image {
            id: scrollhandle
            source: "images/scroller.png"
        }
        scrollBarBackground: Image {
            id: scrollStyle
            source: "images/Scroller_bg.png"
        }
        incrementControl: Image {
            id: increment
            source: "images/down_arrow.png"
        }
        decrementControl: Image {
            id: decrement
            source: "images/up_arrow.png"
        }
    }}

    ScrollView
    {
    id:sceneScroll
    x:13
    y:232
    height:100
    style: uvc130_scrollViewStyle
    Component.onCompleted:
    {
    console.log("sceneScroll completed")
    }
    Grid {
    columns: 2
    spacing: 10

    ExclusiveGroup { id: sceneInputGroup }
    RadioButton {
    id: scene1Button
    style:  uvc130_radioButtonStyle
    text:   qsTr("Normal")
    exclusiveGroup: sceneInputGroup
    //checked: true
    onClicked: {
        console.log("Normal")
        seecam130.setSceneMode(See3Cam130.SCENE_NORMAL)
    }
    }
    RadioButton {
    id: scene2Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Portrait")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Portrait")
        seecam130.setSceneMode(See3Cam130.SCENE_PORTRAIT)
    }
    }
    RadioButton {
    id: scene3Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Land Scape")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("LandScape")
        seecam130.setSceneMode(See3Cam130.SCENE_LANDSCAPE)
    }
    }
    RadioButton {
    id: scene4Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Sport")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Sport")
        seecam130.setSceneMode(See3Cam130.SCENE_SPORT)
    }
    }
    RadioButton {
    id: scene5Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Close-up")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Close up")
        seecam130.setSceneMode(See3Cam130.SCENE_CLOSE_UP)
    }
    }
    RadioButton {
    id: scene6Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Night")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Night")
        seecam130.setSceneMode(See3Cam130.SCENE_NIGHT)
    }
    }
    RadioButton {
    id: scene7Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Twilight")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Twilight")
        seecam130.setSceneMode(See3Cam130.SCENE_TWILIGHT)
    }
    }
    RadioButton {
    id: scene8Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Back Light")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("BackLight")
        seecam130.setSceneMode(See3Cam130.SCENE_BACKLIGHT)
    }
    }
    RadioButton {
    id: scene9Button
    style:  uvc130_radioButtonStyle
    text: qsTr("High Sensitive")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("High Sensitive")
        seecam130.setSceneMode(See3Cam130.SCENE_HIGH_SENSITIVE)
    }
    }
    RadioButton {
    id: scene10Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Night Portrait")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Night Portrait")
        seecam130.setSceneMode(See3Cam130.SCENE_NIGHT_PORTRAIT)
    }
    }
    RadioButton {
    id: scene11Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Beach")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Beach")
        seecam130.setSceneMode(See3Cam130.SCENE_BEACH)
    }
    }
    RadioButton {
    id: scene12Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Document")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Document")
        seecam130.setSceneMode(See3Cam130.SCENE_BEACH)
    }
    }
    RadioButton {
    id: scene13Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Party")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Party")
        seecam130.setSceneMode(See3Cam130.SCENE_PARTY)
    }
    }
    RadioButton {
    id: scene14Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Fireworks")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Fire works")
        seecam130.setSceneMode(See3Cam130.SCENE_FIREWORKS)
    }
    }
    RadioButton {
    id: scene15Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Sunset")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Sunset")
        seecam130.setSceneMode(See3Cam130.SCENE_SUNSET)
    }
    }
    RadioButton {
    id: scene16Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Auto Detect")
    exclusiveGroup: sceneInputGroup
    onClicked: {
        console.log("Auto Detect")
        seecam130.setSceneMode(See3Cam130.SCENE_AUTO_DETECT)
    }
    }
    }
    }

    ScrollView
    {
    id:effectScroll
    x:13
    y:394//414
    height:100
    style: uvc130_scrollViewStyle

    Component.onCompleted:
    {
    console.log("effectScroll completed")
    }
    Grid {
    columns: 2
    spacing: 15
    ExclusiveGroup { id: effectInputGroup }
    RadioButton {
    id: effect1Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Normal")
    exclusiveGroup: effectInputGroup
    //checked: true
    onClicked: {
        console.log("Normal")
        seecam130.setEffectMode(See3Cam130.EFFECT_NORMAL)
    }
    }
    RadioButton {
    id: effect2Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Alien")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Alien")
        seecam130.setEffectMode(See3Cam130.EFFECT_ALIEN)
    }
    }
    RadioButton {
    id: effect3Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Antique")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Antique")
        seecam130.setEffectMode(See3Cam130.EFFECT_ANTIQUE)
    }
    }
    RadioButton {
    id: effect4Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Black & White")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Black & White")
        seecam130.setEffectMode(See3Cam130.EFFECT_BLACK_WHITE)
    }
    }

    RadioButton {
    id: effect5Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Emboss")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Emboss")
        seecam130.setEffectMode(See3Cam130.EFFECT_EMBOSS)
    }
    }
    RadioButton {
    id: effect6Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Emboss Colored")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Emboss Colored")
        seecam130.setEffectMode(See3Cam130.EFFECT_EMBOSS_COLORED)
    }
    }
    RadioButton {
    id: effect7Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Grey")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Grey")
        seecam130.setEffectMode(See3Cam130.EFFECT_GRAY)
    }
    }
    RadioButton {
    id: effect8Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Negative")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Negative")
        seecam130.setEffectMode(See3Cam130.EFFECT_NEGATIVE)
    }
    }
    RadioButton {
    id: effect9Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Blue")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Blue")
        seecam130.setEffectMode(See3Cam130.EFFECT_BLUE)
    }
    }
    RadioButton {
    id: effect10Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Green")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Green")
        seecam130.setEffectMode(See3Cam130.EFFECT_GREEN)
    }
    }
    RadioButton {
    id: effect11Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Red")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Red")
        seecam130.setEffectMode(See3Cam130.EFFECT_RED)
    }
    }
    RadioButton {
    id: effect12Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Poster -ize1")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Posterize1")
        seecam130.setEffectMode(See3Cam130.EFFECT_POSTERIZE_1)
    }
    }

    RadioButton {
    id: effect13Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Poster -ize2")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Posterize2")
        seecam130.setEffectMode(See3Cam130.EFFECT_POSTERIZE_2)
    }
    }
    RadioButton {
    id: effect14Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Sepia1")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Sepia1")
        seecam130.setEffectMode(See3Cam130.EFFECT_SEPIA_1)
    }
    }
    RadioButton {
    id: effect15Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Sepia2")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Sepia2")
        seecam130.setEffectMode(See3Cam130.EFFECT_SEPIA_2)
    }
    }
    RadioButton {
    id: effect16Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Sketch")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Sketch")
        seecam130.setEffectMode(See3Cam130.EFFECT_SKETCH)
    }
    }
    RadioButton {
    id: effect17Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Solarize")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Solarize")
        seecam130.setEffectMode(See3Cam130.EFFECT_SOLARIZE)
    }
    }
    RadioButton {
    id: effect18Button
    style:  uvc130_radioButtonStyle
    text: qsTr("Foggy")
    exclusiveGroup: effectInputGroup
    onClicked: {
        console.log("Foggy")
        seecam130.setEffectMode(See3Cam130.EFFECT_FOGGY)
    }
    }

    }
    }


See3Cam130 {
    id: seecam130

    onSendSceneMode: {
        console.log("scenemode:",sceneMode)
        defaultSceneMode(sceneMode)
    }
    onSendEffectMode: {
        console.log("effectMode:",effectMode)
        defaultEffectMode(effectMode)
    }
}




        Uvccamera {
                id: uvccamera
                onTitleTextChanged: {
                    messageDialog.title = _title.toString()
                    messageDialog.text = _text.toString()
                    messageDialog.open()
                }
                onSerialNumber:{
                    messageDialog.title = qsTr("Serial Number")
                    messageDialog.text = serialNumber;
                }
            }

            Component.onCompleted:{
                uvccamera.initExtensionUnit("See3CAM_CU130")
                seecam130.getSceneMode()
                seecam130.getEffectMode() 
            }
            Component.onDestruction: {
                uvccamera.exitExtensionUnit()
            }

            function getSerialNumber() {
                uvccamera.getSerialNumber()
                messageDialog.open()
            }

            function getFirmwareVersion() {
                uvccamera.getFirmWareVersion()
                messageDialog.open()
            }

            function defaultSceneMode(mode)
            {
                switch(mode)
                {
                    case 1 :
                        scene1Button.checked = true
                        break;
                    case 2 :
                        scene2Button.checked = true
                        break;
                    case 3 :
                        scene3Button.checked = true
                        break;
                    case 4 :
                        scene4Button.checked = true
                        break;
                    case 5 :
                        scene5Button.checked = true
                        break;
                    case 6 :
                        scene6Button.checked = true
                        break;
                    case 7 :
                        scene7Button.checked = true
                        break;
                    case 8 :
                        scene8Button.checked = true
                        break;
                    case 9 :
                        scene9Button.checked = true
                        break;
                    case 10 :
                        scene10Button.checked = true
                        break;
                    case 11 :
                        scene11Button.checked = true
                        break;
                    case 12 :
                        scene12Button.checked = true
                        break;
                    case 13 :
                        scene13Button.checked = true
                        break;
                    case 14 :
                        scene14Button.checked = true
                        break;
                    case 15 :
                        scene15Button.checked = true
                        break;
                    case 16 :
                        scene16Button.checked = true
                        break;
                }
            }
            function defaultEffectMode(mode)
            {
                switch(mode)
                {
                    case 1 :
                        effect1Button.checked = true
                        break;
                    case 2 :
                        effect2Button.checked = true
                        break;
                    case 3 :
                        effect3Button.checked = true
                        break;
                    case 4 :
                        effect4Button.checked = true
                        break;
                    case 5 :
                        effect5Button.checked = true
                        break;
                    case 6 :
                        effect6Button.checked = true
                        break;
                    case 7 :
                        effect7Button.checked = true
                        break;
                    case 8 :
                        effect8Button.checked = true
                        break;
                    case 9 :
                        effect9Button.checked = true
                        break;
                    case 10 :
                        effect10Button.checked = true
                        break;
                    case 11 :
                        effect11Button.checked = true
                        break;
                    case 12 :
                        effect12Button.checked = true
                        break;
                    case 13 :
                        effect13Button.checked = true
                        break;
                    case 14 :
                        effect14Button.checked = true
                        break;
                    case 15 :
                        effect15Button.checked = true
                        break;
                    case 16 :
                        effect16Button.checked = true
                        break;
                    case 17 :
                        effect17Button.checked = true
                        break;
                    case 18 :
                        effect18Button.checked = true
                        break;
                }
            }


    }

