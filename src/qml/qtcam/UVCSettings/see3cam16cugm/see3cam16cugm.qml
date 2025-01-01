import econ.camera.see3cam16cugm 1.0
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item {
    width:268
    height:900


    property bool skipUpdateUIFrameRate        : false
    property bool skipUpdateBlackLevelMode     : false


    Action {
        id: firmwareVersion
        onTriggered:
        {
            getFirmwareVersion()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }

    Action {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }


    Connections
    {
        target: root
        onTakeScreenShot:
        {
           root.imageCapture(CommonEnums.SNAP_SHOT);
        }

        onGetVideoPinStatus:
        {
            root.enableVideoPin(true);
        }
        onGetStillImageFormats:
        {
            var stillImageFormat = []
            stillImageFormat.push("jpg")
            stillImageFormat.push("bmp")
            stillImageFormat.push("raw")
            stillImageFormat.push("png")
            root.insertStillImageFormat(stillImageFormat);
        }
        onVideoResolutionChanged:{
//            getCamValuesTimer.start()
        }
        onPreviewFPSChanged:{
//            getCamValuesTimer.start()
        }
        onVideoColorSpaceChanged:{
//            getCamValuesTimer.start()
        }
        onSkipFrameCount:{
            root.setSkipFrameCount(3)
        }
    }

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle

        Item{

            height: 750
            ColumnLayout{
                x:2
                y:5
                spacing:20

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: shutterModes
                        text: "--- Shutter Modes ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                ColumnLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    ExclusiveGroup { id: shutterModeGroup }
                    RadioButton {
                        id: globalShutter
                        style:  econRadioButtonStyle
                        text:   qsTr("Global Shutter")
                        exclusiveGroup: shutterModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam16cugm.setShutterMode(See3CAM_16CUGM.GLOBAL_SHUTTER)
                        }
                        Keys.onReturnPressed:  {
                            see3cam16cugm.setShutterMode(See3CAM_16CUGM.GLOBAL_SHUTTER)
                        }
                    }
                    RadioButton {
                        id: rollingShutter
                        style:  econRadioButtonStyle
                        text: qsTr("Electronic Rolling Shutter")
                        exclusiveGroup: shutterModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam16cugm.setShutterMode(See3CAM_16CUGM.ROLLING_SHUTTER)
                        }
                        Keys.onReturnPressed: {
                            see3cam16cugm.setShutterMode(See3CAM_16CUGM.ROLLING_SHUTTER)
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: strobeModes
                        text: "--- Strobe Modes ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                ColumnLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    ExclusiveGroup { id: strobeModeGroup }
                    RadioButton {
                        id: strobeOn
                        style:  econRadioButtonStyle
                        text:   qsTr("Strobe ON")
                        exclusiveGroup: strobeModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam16cugm.setStrobeMode(See3CAM_16CUGM.ON)
                        }
                        Keys.onReturnPressed:  {
                            see3cam16cugm.setStrobeMode(See3CAM_16CUGM.ON)
                        }
                    }
                    RadioButton {
                        id: strobeOff
                        style:  econRadioButtonStyle
                        text: qsTr("Strobe OFF")
                        exclusiveGroup: strobeModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam16cugm.setStrobeMode(See3CAM_16CUGM.OFF)
                        }
                        Keys.onReturnPressed: {
                            see3cam16cugm.setStrobeMode(See3CAM_16CUGM.OFF)
                        }
                    }
                }

                //Flip Mode
                Text {
                    id: flipMode
                    text: "--- Flip Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 40
                    CheckBox {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        onClicked:{
                            see3cam16cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            see3cam16cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            see3cam16cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            see3cam16cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }

                }


                Text {
                    id: blackLevelMode
                    text: "--- Black level Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Grid{
                   ExclusiveGroup { id: blackLevelGroup }
                   spacing: 25
                   RadioButton {
                       id: blackLevelAuto
                       style:  econRadioButtonStyle
                       text: qsTr("Auto")
                       exclusiveGroup: blackLevelGroup
                       activeFocusOnPress: true
                       onClicked: {
                           setAutoBlackLevel()
                       }
                       Keys.onReturnPressed: {
                           setAutoBlackLevel()
                       }
                   }
                   RadioButton {
                       id: blackLevelManual
                       style:  econRadioButtonStyle
                       text: qsTr("Manual")
                       exclusiveGroup: blackLevelGroup
                       activeFocusOnPress: true
                       onClicked: {
                           setManualBlackLevel()
                       }
                       Keys.onReturnPressed: {
                           setManualBlackLevel()
                       }
                   }
               }

                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: blackLevelSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: blackLevelSlider.minimumValue
                        maximumValue: blackLevelSlider.maximumValue
                        onValueChanged:  {
                            blackLevelTextField.text = blackLevelSlider.value
                            if(skipUpdateBlackLevelMode){
                                see3cam16cugm.setBlackLevelCorrection(See3CAM_16CUGM.MANUAL, blackLevelSlider.value)
                            }
                            skipUpdateBlackLevelMode = true
                        }
                    }
                    TextField
                    {
                        id: blackLevelTextField
                        text: blackLevelSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: blackLevelSlider.minimumValue; top: blackLevelSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                blackLevelSlider.value = blackLevelTextField.text
                            }
                        }
                    }
                }


//                Text
//                {
//                    id: frameRateText
//                    text: "--- Frame Rate Control ---"
//                    font.pixelSize: 14
//                    font.family: "Ubuntu"
//                    color: "#ffffff"
//                    smooth: true
//                    Layout.alignment: Qt.AlignCenter
//                    opacity: 0.50196078431373
//                }
//                Row{
//                    spacing: 35
//                    Slider {
//                        activeFocusOnPress: true
//                        updateValueWhileDragging: false
//                        id: frameRateSlider
//                        width: 150
//                        stepSize: 1
//                        style:econSliderStyle
//                        minimumValue: frameRateSlider.minimumValue
//                        maximumValue: frameRateSlider.maximumValue
//                        onValueChanged:  {
//                            frameRateTextField.text = frameRateSlider.value
//                            if(skipUpdateUIFrameRate){
//                                see3cam16cugm.setFpsValue(frameRateSlider.value)
//                            }
//                            skipUpdateUIFrameRate = true
//                        }
//                    }
//                    TextField {
//                        id: frameRateTextField
//                        text: frameRateSlider.value
//                        font.pixelSize: 10
//                        font.family: "Ubuntu"
//                        smooth: true
//                        horizontalAlignment: TextInput.AlignHCenter
//                        style: econTextFieldStyle
//                        validator: IntValidator {bottom: frameRateSlider.minimumValue; top: frameRateSlider.maximumValue}
//                        onTextChanged: {
//                            if(text.length > 0){
//                                frameRateSlider.value = frameRateTextField.text
//                            }
//                        }
//                    }
//                }

                Text {
                    id: hdrMode
                    text: "--- HDR Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 50
                    ExclusiveGroup { id: hdrModeGroup }

                    RadioButton {
                        id: hdrOn
                        style:  econRadioButtonStyle
                        text:   qsTr("HDR ON")
                        exclusiveGroup: hdrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam16cugm.setHDRMode(See3CAM_16CUGM.HDR_ON)
                        }
                        Keys.onReturnPressed: {
                            see3cam16cugm.setHDRMode(See3CAM_16CUGM.HDR_ON)
                        }
                    }

                    RadioButton {
                        id: hdrOff
                        style:  econRadioButtonStyle
                        text: qsTr("HDR OFF")
                        exclusiveGroup: hdrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam16cugm.setHDRMode(See3CAM_16CUGM.HDR_OFF)
                        }
                        Keys.onReturnPressed: {
                            see3cam16cugm.setHDRMode(See3CAM_16CUGM.HDR_OFF)
                        }
                    }
                }


                //stream mode
                Text {
                    id: streamMode
                    text: "--- Stream Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 50
                    ExclusiveGroup { id: streamModeGroup }

                    RadioButton {
                        id: masterMode
                        style:  econRadioButtonStyle
                        text:   qsTr("Master")
                        exclusiveGroup: streamModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            setMasterMode()
                        }
                        Keys.onReturnPressed: {
                            setMasterMode()
                        }
                    }

                    RadioButton {
                        id: triggerMode
                        style:  econRadioButtonStyle
                        text: qsTr("Trigger")
                        exclusiveGroup: streamModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            setTriggerMode()
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: defaultValue
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Default"
                        tooltip: "Click to set default values in extension controls"
                        action: setDefault
                        style: econButtonStyle
                        Keys.onReturnPressed: {
                            setToDefaultValues()
                        }
                    }
                }

               Row{
                   // Layout.alignment: Qt.AlignCenter
                    Button {
                        id: f_wversion_selected130
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
                    Button {
                        id: serial_no_selected
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
                }

            }//Coloumn Layout
        }//Item in Scroll View
    }//ScrollView

   See3CAM_16CUGM{
     id:see3cam16cugm

     onShutterModeReceived: {
         if(shutterMode == See3CAM_16CUGM.GLOBAL_SHUTTER)
         {
            globalShutter.checked = true
         }
         else if(shutterMode == See3CAM_16CUGM.ROLLING_SHUTTER)
         {
            rollingShutter.checked = true
         }
     }

     onStrobeModeReceived: {
         if(strobeMode == See3CAM_16CUGM.ON)
         {
            strobeOn.checked = true
         }
         else if(strobeMode == See3CAM_16CUGM.OFF)
         {
            strobeOff.checked = true
         }
     }

     onBlackLevelModeReceived: {
         if(blackLevelMode == See3CAM_16CUGM.AUTO)
         {
            blackLevelAuto.checked = true
            blackLevelSlider.enabled = false
            blackLevelSlider.opacity = 0.1

            blackLevelTextField.enabled = false
            blackLevelTextField.opacity = 0.1
         }
         else if(blackLevelMode == See3CAM_16CUGM.MANUAL)
         {
            blackLevelManual.checked = true
            blackLevelSlider.enabled = true
            blackLevelSlider.opacity = 1

            blackLevelTextField.enabled = true
            blackLevelTextField.opacity = 1
         }
     }

     onCurrentOffsetReceived: {
        skipUpdateBlackLevelMode = false
        blackLevelSlider.value = currentOffset
     }

     onMinimumOffsetReceived: {
        blackLevelSlider.minimumValue = minOffset
     }
     onMaximumOffsetReceived: {
        blackLevelSlider.maximumValue = maxOffset
     }

     onFlipMirrorModeChanged:
     {
         currentFlipMirrorMode(flipMirrorModeValues)
     }

     onHdrModeReceived: {
         if(hdrMode == See3CAM_16CUGM.HDR_OFF)
         {
             hdrOff.checked = true
         }
         else if(hdrMode == See3CAM_16CUGM.HDR_ON)
         {
             hdrOn.checked = true
         }
     }

     onStreamModeReceived: {
         if(streamMode == See3CAM_16CUGM.MASTER)
         {
             masterMode.checked = true
             root.checkForTriggerMode(false)
             root.captureBtnEnable(true)
             root.videoRecordBtnEnable(true)
             root.startUpdatePreviewInMasterMode()
         }
         else if(streamMode == See3CAM_16CUGM.TRIGGER)
         {
             triggerMode.checked = true
             root.videoRecordBtnEnable(false)
             root.captureBtnEnable(false)
             root.checkForTriggerMode(true)
             root.stopUpdatePreviewInTriggerMode()
         }
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


    Component {
        id: econRadioButtonStyle
        RadioButtonStyle {
            label: Text {
                text: control.text
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                opacity: 1
            }
            background: Rectangle {
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
            }
        }
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
        id: econCheckBoxStyle
        CheckBoxStyle {
            label: Text {
                text: control.text
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                opacity: 1
            }
            background: Rectangle {
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
            }
        }
    }
    Component {
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 50
                implicitHeight: 20
                border.color: "#333"
                border.width: 2
                y: 1
            }
        }
    }

    Component {
        id: econButtonStyle
        ButtonStyle {
            background: Rectangle {
                implicitHeight: 38
                implicitWidth: 104
                border.width: control.activeFocus ? 3 :0
                color: "#e76943"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
                radius: control.activeFocus ? 5 : 0
            }
            label: Text {
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "Ubuntu"
                font.pointSize: 10
                text: control.text
            }
        }
    }

    Component {
        id: econscrollViewStyle
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
        }
    }


    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case See3CAM_16CUGM.NORMAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        case See3CAM_16CUGM.VERTICAL:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case See3CAM_16CUGM.HORIZONTAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case See3CAM_16CUGM.ROTATE_180:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        }
    }

    function setAutoBlackLevel()
    {
        see3cam16cugm.setBlackLevelCorrection(See3CAM_16CUGM.AUTO, blackLevelSlider.value)
        blackLevelSlider.enabled = false
        blackLevelSlider.opacity = 0.1

        blackLevelTextField.enabled = false
        blackLevelTextField.opacity = 0.1

    }

    function setManualBlackLevel()
    {
        see3cam16cugm.setBlackLevelCorrection(See3CAM_16CUGM.MANUAL, blackLevelSlider.value)
        blackLevelSlider.enabled = true
        blackLevelSlider.opacity = 1

        blackLevelTextField.enabled = true
        blackLevelTextField.opacity = 1
    }

    function setMasterMode(){
        see3cam16cugm.setStreamMode(See3CAM_16CUGM.MASTER)
        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function setTriggerMode(){
        see3cam16cugm.setStreamMode(See3CAM_16CUGM.TRIGGER)
        root.stopUpdatePreviewInTriggerMode()
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        root.stopUpdatePreviewInTriggerMode()
    }

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        see3cam16cugm.setToDefaultValues()
        getValuesFromCamera()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function getValuesFromCamera(){
        see3cam16cugm.getShutterMode()
        see3cam16cugm.getStrobeMode()
        see3cam16cugm.getOrientation()
        see3cam16cugm.getHDRMode()
        see3cam16cugm.getStreamMode()
        see3cam16cugm.getBlackLevelCorrection()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }

}
