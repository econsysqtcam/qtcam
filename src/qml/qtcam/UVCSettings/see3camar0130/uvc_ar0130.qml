import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3camar0130 1.0
import econ.camera.see3camControl 1.0
import econ.camera.see3camGpioControl 1.0
import "../../JavaScriptFiles/tempValue.js" as JS
import cameraenum 1.0

Item {
    width:268
    height:720
    property bool outputPinFlag
    property bool masterMode
    property bool triggerMode

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            if(JS.masterMode_12cuinr === 1)
            {
                if(root.webcamKeyAccept) {
                    root.imageCapture(CommonEnums.SNAP_SHOT);
                    root.webcamKeyAccept = false
                }
            }
            else
            {
                if(isWebKeyPressed)
                {
                    // Added by Sankari : 09 Nov 2016
                    // Trigger shot  - continuous shots - init trigger shot
                    if(root.webcamKeyTriggerShot){
                        seecamar0130.initTriggerShotCapture()
                    }
                }
            }
        }
        onGetVideoPinStatus:
        {
            var videoPin = JS.masterMode_12cuinr === 1 ? true : false
            root.enableVideoPin(videoPin);
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
        onCameraDeviceUnplugged:
        {
            JS.enableMasterMode_12cuinr()
        }
        onSetMasterMode:
        {
            enableMasterMode();
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            displayFirmwareVersion()
        }
    }

    Timer {
        id: masterModeTimer
        interval: 1000
        onTriggered: {
            masterModeCapture()
            stop()
        }
    }

    Action {
        id: masterModeAction
        onTriggered: {
            enableMasterMode()
        }
    }

    Action {
        id: triggerModeAction
        onTriggered: {
            enableTriggerMode()
        }
    }

    Action {
        id: croppedMode
        onTriggered: {
            croppedModeDisplay()
        }
    }

    Action {
        id: binnedMode
        onTriggered: {
            binnedModeDisplay()
        }
    }

    Component {
        id: econAR0130ButtonStyle
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

    Button {
        id: mastermmode_selected
        x: 19
        y: 241
        opacity: 1
        action: masterModeAction
        activeFocusOnPress : true
        tooltip: "Set Camera in Master Mode"
        text: "Master Mode"
        style: econAR0130ButtonStyle
        Keys.onReturnPressed: {
             enableMasterMode()
        }
    }

    Button {
        id: trigger_mode_selected
        x: 145
        y: 241
        opacity: 1
        action: triggerModeAction
        activeFocusOnPress : true
        tooltip: "Set Camera in Trigger Mode"
        text: "Trigger Mode"
        style: econAR0130ButtonStyle
        Keys.onReturnPressed: {
             enableTriggerMode()
        }
        onFocusChanged: {
            // Disable saving image when focus is changed from trigger mode to master mode
            // or changing to any other camera if it is m_saveImage flag set as true to avoid displaying unnecessary pop up dialog.
           root.disableSaveImage()
        }
    }

    Button {
        id: vga60fps_selected
        x: 19
        y: 302
        opacity: 1
        action: croppedMode
        activeFocusOnPress : true
        tooltip: "Set the camera in cropped mode"
        text: "Cropped VGA"
        style:econAR0130ButtonStyle
        Keys.onReturnPressed: {
            croppedModeDisplay()
        }
    }
    Button {
        id: vga30fps_selected
        x: 144
        y: 302
        opacity: 1
        action: binnedMode
        activeFocusOnPress : true
        tooltip: "Set the camera in binned mode"
        text: "Binned VGA"
        style: econAR0130ButtonStyle
        Keys.onReturnPressed: {
            binnedModeDisplay()
        }
    }
    Text {
        id: camera_controls
        text: "--- Camera Mode Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 45
        y: 194.5
        opacity: 0.50196078431373
    }

    Text {
        id: flash_mode
        text: "Flash Mode :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 16
        y: 472.5
        opacity: 1
    }
    Text {
        id: flash_controls
        text: "--- Flash Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 431.5
        opacity: 0.50196078431373
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

    Text {
        id: gpo_level
        text: "GPO Level :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 24
        y: 394.5
        opacity: 1
    }
    Row {
        x: 111
        y: 394.5
        spacing: 25
        ExclusiveGroup { id: groupGpioar0130 }
        RadioButton {
            exclusiveGroup: groupGpioar0130
            id: radioHighar0130
            text: "High"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked:{
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT3,See3CamGpio.High)//Pin Number & Value
            }
            Keys.onReturnPressed: {
                checked = true
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT3,See3CamGpio.High)//Pin Number & Value
            }
        }
        RadioButton {
            exclusiveGroup: groupGpioar0130
            checked: false
            id: radioLowar0130
            text: "Low"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked: {
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT3,See3CamGpio.Low)//Pin Number & Value
            }
            Keys.onReturnPressed: {
                checked = true
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT3,See3CamGpio.Low)//Pin Number & Value
            }
        }
    }

    Text {
        id: gpio_controls
        text: "--- GPIO Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 356.5
        opacity: 0.50196078431373
    }

    Row {
        x: 111
        y: 474
        spacing: 25
        ExclusiveGroup { id: groupFlashar0130 }
        RadioButton {
            exclusiveGroup: groupFlashar0130
            id: radioOn
            checked: false
            text: "On"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked:{
               see3camctrl.setFlashControlState(1)
            }            
        }
        RadioButton {
            exclusiveGroup: groupFlashar0130
            id: radioOff
            checked: false
            text: "Off"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked: {
                see3camctrl.setFlashControlState(0)
            }           
        }
    }


    Text {
        id: other_optionsAR0130
        text: "--- Other Options ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 72
        y: 549.5
        opacity: 0.50196078431373
    }

    Button {
        id: serial_no_selected
        x: 85
        y: 582
        opacity: 1
        action: serialNumber
        activeFocusOnPress : true
        text: "Serial No"
        tooltip: "Click to view the Serial Number"
        style: econAR0130ButtonStyle
        Keys.onReturnPressed: {
            getSerialNumber()
        }
    }

    Button {
        id: f_wversion_selected
        x: 85
        y: 637
        opacity: 1
        action: firmwareVersion
        activeFocusOnPress : true
        text: "F/W Version"
        tooltip: "Click to see the firmware version of the camera"
        style: econAR0130ButtonStyle
        Keys.onReturnPressed: {
            displayFirmwareVersion()
        }
    }

    See3CamCtrl {
        id: see3camctrl
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
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

    See3Camar0130 {
        id: seecamar0130
        onUpdateFlashCheckBox:   {
            if(flash_Check_state === "1") {
                radioOn.checked = true;
            }
            else {
                radioOff.checked = true;
            }
        }
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
        onTriggershotSignal:{
            root.takeTriggershot()
        }

    }
    function enableMasterMode() {
       masterModeCapture();
        masterMode = seecamar0130.enableMasterMode()
        if(masterMode) {
            masterModeTimer.start()
            JS.enableMasterMode_12cuinr()
            if(!vga60fps_selected.enabled) {
                vga60fps_selected.enabled = true
                vga60fps_selected.opacity = 1
                vga30fps_selected.enabled = true
                vga30fps_selected.opacity = 1
            }

        }
        else {
            messageDialog.title = "Failure"
            messageDialog.text = "Master Mode enabling failed"
            messageDialog.open()
        }
    }

    function enableTriggerMode() {
        triggerModeCapture()
        triggerMode = seecamar0130.enableTriggerMode()
        if(triggerMode) {
            JS.enableTriggerMode_12cuinr()
            vga60fps_selected.enabled = false
            vga60fps_selected.opacity = 0.2
            vga30fps_selected.enabled = false
            vga30fps_selected.opacity = 0.2
        }
        else {
            messageDialog.title = "Failure"
            messageDialog.text = "Trigger Mode enabling failed"
            messageDialog.open()
        }
    }

    function displayFirmwareVersion() {
        uvccamera.getFirmWareVersion()
    }

    function croppedModeDisplay() {
         seecamar0130.setCroppedVGAMode()
    }

    function binnedModeDisplay() {
        seecamar0130.setBinnedVGAMode()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    See3CamGpio {
        id: see3camGpio
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
        onGpioLevel:{
            if(level === 0) {
                if(outputPinFlag)
                    radioLowar0130.checked = true
            } else {
                if(outputPinFlag)
                    radioHighar0130.checked = true
            }
        }
    }

    Component.onCompleted:{
        seecamar0130.getFlashLevel()
        outputPinFlag = true
        see3camGpio.getGpioLevel(See3CamGpio.OUT3)
        mastermmode_selected.forceActiveFocus()
        if(JS.triggerMode_12cuinr === 1) {
            vga60fps_selected.enabled = false
            vga60fps_selected.opacity = 0.2
            vga30fps_selected.enabled = false
            vga30fps_selected.opacity = 0.2
        }
    }
//    Connections{
//         target: root
//         // Init Trigger shot capture
//         onInitTriggershot:{
//            seecamar0130.initTriggerShotCapture()
//         }
//    }
}
