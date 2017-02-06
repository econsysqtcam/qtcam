import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam51 1.0
import econ.camera.see3camControl 1.0
import "../../JavaScriptFiles/tempValue.js" as JS
import cameraenum 1.0

Item {
    width:268
    height:720
    property bool masterMode
    property bool triggerMode

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            if(JS.masterMode_cu51 === 1)
            {
                root.imageCapture(CommonEnums.SNAP_SHOT);
            }
            else
            {
                if(isWebKeyPressed)
                {
                    root.imageCapture(CommonEnums.TRIGGER_SHOT);
                }
            }
        }
        onGetVideoPinStatus:
        {
            var videoPin = JS.masterMode_cu51 === 1 ? true : false
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
            JS.enableMasterMode_cu51()
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
        id: exposureAction
        onTriggered: {
            exposureSet()
        }
    }

    Action {
        id:  captureAction
        onTriggered: {
             root.imageCapture(CommonEnums.SNAP_SHOT);
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
    }
        Button {
            id: set_exposure
            x: 26
            y: 373
            opacity: 1
            action: exposureAction
            activeFocusOnPress : true
            text: "Set Exposure"
            tooltip: "Click to set exposure"
            style: econcu51exposureStyle
            Keys.onReturnPressed: {

            }
        }
        Button {
            id: captureExposedImage
            x: 139
            y: 373
            opacity: 1
            action: captureAction
            activeFocusOnPress : true
            text: "Capture"
            tooltip: "Click to capture exposed image"
            style: econcu51exposureStyle
            //visible: trigger_mode_selected.activeFocus ? true : false
            Keys.onReturnPressed: {

            }
        }
    Text {
        id: exposure
        text: "Exposure :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 18
        y: 327.5
        opacity: 1
    }

    TextField {
        id: exposurevalue
        x: 104
        y: 327.5
        font.pixelSize: 10//8
        font.family: "Ubuntu"
        smooth: true
        horizontalAlignment: TextInput.AlignHCenter
        opacity: 1
        validator: IntValidator { id: exposureValidator ; bottom: 1 ; top: 30000;}
        style: econTextFieldStyle
        onTextChanged: {

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
        id: torch_mode
        text: "Torch Mode :"
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
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 150
                implicitHeight: 20
                border.color: "#333"
                border.width: 2
                y: 1
            }
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


    Row {
        x: 111
        y: 474
        spacing: 25
        ExclusiveGroup { id: groupFlashcu51 }
        RadioButton {
            exclusiveGroup: groupFlashcu51
            id: radioOn
            checked: false
            text: "On"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked:{
               see3camctrl.setTorchControlState(1)
            }
        }
        RadioButton {
            exclusiveGroup: groupFlashcu51
            id: radioOff
            checked: false
            text: "Off"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked: {
                see3camctrl.setTorchControlState(0)

            }
        }
    }

    Text {
        id: other_optionsCU51
        text: "--- Other Options ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 72
        y: 529.5
        opacity: 0.50196078431373
    }

    Button {
        id: serial_no_selected
        x: 85
        y: 562
        opacity: 1
        action: serialNumber
        activeFocusOnPress : true
        text: "Serial No"
        tooltip: "Click to view the Serial Number"
        style: econcu51ButtonStyle
        Keys.onReturnPressed: {
            getSerialNumber()
        }
    }

    Button {
        id: f_wversion_selected
        x: 85
        y: 617
        opacity: 1
        action: firmwareVersion
        activeFocusOnPress : true
        text: "F/W Version"
        tooltip: "Click to see the firmware version of the camera"
        style: econcu51ButtonStyle
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
    Component {
        id: econcu51exposureStyle
        ButtonStyle {
            background: Rectangle {
                implicitHeight: 19
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
        id: econcu51ButtonStyle
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
    See3Cam51 {
        id: seecam51
        onUpdateFlashCheckBox:   {
            if(flash_Check_state === "1") {
                radioOn.checked = true;
            }
            else {
                radioOff.checked = true;
            }
        }
        onSendExposure: {            
            exposurevalue.text = xposureValue.toString()
        }
        onTriggerModeCapture:
        {                
                root.seeCamCu51triggerModeCapture()
        }


    }


    function enableMasterMode() {
       masterModeCapture();
        masterMode = seecam51.enableMasterMode()
        if(masterMode) {
            masterModeTimer.start()
            JS.enableMasterMode_cu51()
            captureExposedImage.visible = true
        }
        else {
            messageDialog.title = "Failure"
            messageDialog.text = "Master Mode enabling failed"
            messageDialog.open()
        }
    }

    function enableTriggerMode() {
        triggerModeCapture()
        triggerMode = seecam51.enableTriggerMode()
        if(triggerMode) {
            JS.enableTriggerMode_cu51()
            captureExposedImage.visible = false
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

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function exposureSet()
    {
        var value = exposurevalue.text * 1
        if (value == 0)
            value = exposureValidator.bottom
            seecam51.setExposure(value);
    }


    Component.onCompleted:{
        seecam51.getTorchLevel()
        mastermmode_selected.forceActiveFocus()
        seecam51.getExposure()
    }

}
