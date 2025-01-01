import econ.camera.see3camcu200m 1.0
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:268
    height:750

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

    Connections{
        target: root

        function onTakeScreenShot(){
            root.imageCapture(CommonEnums.SNAP_SHOT);
        }
        function onGetVideoPinStatus(){
            root.enableVideoPin(true);
        }
        function onGetStillImageFormats(){
            var stillImageFormat = []
            stillImageFormat.push("jpg")
            stillImageFormat.push("bmp")
            stillImageFormat.push("raw")
            stillImageFormat.push("png")
            root.insertStillImageFormat(stillImageFormat);
        }
        function onSkipFrameCount(){
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
        ColumnLayout{
            x:2
            y:5
            spacing:20

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
                Layout.alignment: Qt.AlignCenter

                RadioButton {
                    id: masterMode
                    style:  econRadioButtonStyle
                    text:   qsTr("Master")
                    exclusiveGroup: streamModeGroup
                    activeFocusOnPress: true
                    ToolButton{
                        tooltip: "After choosing master mode, the application starts video streaming. This is a simple mode of operation for the camera without any external trigger capability."
                        width: 15
                        height: 20
                        opacity: 0
                    }
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
                    ToolButton{
                        tooltip: "In Trigger mode, Frames will be out only when external hardware pulses are given to PIN1 of CN4."
                        width: 15
                        height: 20
                        opacity: 0
                    }
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
                Text {
                    id: flash_modes
                    text: "--- Flash Modes ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
            }
            Row{
                x: 23
                y: 235
                spacing: 20
                Layout.alignment: Qt.AlignCenter

                ExclusiveGroup { id: flashModeGroup }
                RadioButton {
                    id: flashDisable
                    style:  econRadioButtonStyle
                    text:   qsTr("Disable")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    ToolButton{
                        tooltip: "It disables both strobe and torch flash controls."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                    onClicked: {
                        see3camcu200m.setFlashMode(SEE3CAM_CU200M.DISABLE)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu200m.setFlashMode(SEE3CAM_CU200M.DISABLE)
                    }
                }
                RadioButton {
                    id: strobeMode
                    style:  econRadioButtonStyle
                    text: qsTr("Strobe")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    ToolButton{
                        tooltip: "When this control is enabled, the LED is ON for each frame’s exposure time while Video Streaming."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                    onClicked: {
                        see3camcu200m.setFlashMode(SEE3CAM_CU200M.STROBE)
                    }
                    Keys.onReturnPressed: {
                        see3camcu200m.setFlashMode(SEE3CAM_CU200M.STROBE)
                    }
                }
                RadioButton {
                    id: torchMode
                    style:  econRadioButtonStyle
                    text: qsTr("Torch")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    ToolButton{
                        tooltip: "When this control is enabled, the LED is ON until the control is disabled."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                    onClicked: {
                        see3camcu200m.setFlashMode(SEE3CAM_CU200M.TORCH)
                    }
                    Keys.onReturnPressed: {
                        see3camcu200m.setFlashMode(SEE3CAM_CU200M.TORCH)
                    }
                }
            }

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
                Layout.alignment: Qt.AlignCenter
                CheckBox {
                    id: flipCtrlHorizotal
                    activeFocusOnPress : true
                    text: "Horizontal"
                    style: econCheckBoxStyle
                    ToolButton{
                        tooltip: "Flips the preview left/right."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                    onClicked:{
                        see3camcu200m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu200m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    ToolButton{
                        tooltip: "Flips the preview vertically up/down."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                    onClicked:{
                        see3camcu200m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu200m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
                Layout.alignment: Qt.AlignCenter
                spacing: 8
                Button {
                    id: resetDevice
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Device Reset"
                    tooltip: "This control is designed to reset all the device settings and provided by default configuration"
                    style: econButtonStyle
                    onClicked:{
                        see3camcu200m.resetDevice()
                    }
                    Keys.onReturnPressed: {
                        see3camcu200m.resetDevice()
                    }
                }
            }

           Row{
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
        }
    }

    SEE3CAM_CU200M{
      id:see3camcu200m

      onStreamModeReceived:{
         currentStreamModeReceived(mode)
      }

      onFlipMirrorModeChanged:{
          currentFlipMirrorMode(mode)
      }

      onFlashModeReceived: {
          currentFlashModeReceived(mode)
      }

      onIndicateFailureStatus:{
          displayMessageBox(title, text)
      }
    }

    function setMasterMode(){
        see3camcu200m.setStreamMode(SEE3CAM_CU200M.MASTER_MODE)

        root.startUpdatePreviewInMasterMode()

        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
    }

    function setTriggerMode(){
        see3camcu200m.setStreamMode(SEE3CAM_CU200M.TRIGGER_MODE)

        root.stopUpdatePreviewInTriggerMode()

        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function currentStreamModeReceived(mode){
        if(mode == SEE3CAM_CU200M.MASTER_MODE){
            masterMode.checked = true

            root.startUpdatePreviewInMasterMode()

            root.checkForTriggerMode(false)
            root.captureBtnEnable(true)
            root.videoRecordBtnEnable(true)
        }
        else if(mode == SEE3CAM_CU200M.TRIGGER_MODE){
           triggerMode.checked = true

            root.stopUpdatePreviewInTriggerMode()

            root.checkForTriggerMode(true)
            root.captureBtnEnable(false)
            root.videoRecordBtnEnable(false)
        }
    }

    function currentFlashModeReceived(mode){
        if(mode == SEE3CAM_CU200M.DISABLE){
           flashDisable.checked = true
        }
        else if(mode == SEE3CAM_CU200M.STROBE){
           strobeMode.checked = true
        }
        else if(mode == SEE3CAM_CU200M.TORCH){
           torchMode.checked = true
        }
    }

    function currentFlipMirrorMode(mode){
        switch(mode){
            case SEE3CAM_CU200M.NORMAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = false
                break;
            case SEE3CAM_CU200M.VERTICAL:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = false
                break;
            case SEE3CAM_CU200M.HORIZONTAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = true
                break;
            case SEE3CAM_CU200M.ROTATE_180:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = true
                break;
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
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

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        see3camcu200m.setToDefault()
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
        see3camcu200m.getStreamMode()
        see3camcu200m.getOrientation()
        see3camcu200m.getFlashMode()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
