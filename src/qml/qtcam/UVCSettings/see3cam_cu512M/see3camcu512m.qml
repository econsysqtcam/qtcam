import econ.camera.see3camcu512m 1.0
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
                        see3camcu512m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu512m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3camcu512m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu512m.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
            ColumnLayout {
                x: 23
                y: 235
                spacing: 15
                ExclusiveGroup { id: flashModeGroup }
                RadioButton {
                    id: flashDisable
                    style:  econRadioButtonStyle
                    text:   qsTr("Disable")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu512m.setFlashMode(See3CAM_CU512M.DISABLE)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu512m.setFlashMode(See3CAM_CU512M.DISABLE)
                    }
                }
                RadioButton {
                    id: strobeMode
                    style:  econRadioButtonStyle
                    text: qsTr("Strobe")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu512m.setFlashMode(See3CAM_CU512M.STROBE)
                    }
                    Keys.onReturnPressed: {
                        see3camcu512m.setFlashMode(See3CAM_CU512M.STROBE)
                    }
                }
                RadioButton {
                    id: torchMode
                    style:  econRadioButtonStyle
                    text: qsTr("Torch")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu512m.setFlashMode(See3CAM_CU512M.TORCH)
                    }
                    Keys.onReturnPressed: {
                        see3camcu512m.setFlashMode(See3CAM_CU512M.TORCH)
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

   See3CAM_CU512M{
     id:see3camcu512m

     onStreamModeReceived:{
         if(streamMode == See3CAM_CU512M.MASTER)
         {
            masterMode.checked = true
         }
         else if(streamMode == See3CAM_CU512M.TRIGGER)
         {
            triggerMode.checked = true
         }
     }

     onFlipMirrorModeChanged:
     {
         currentFlipMirrorMode(flipMirrorModeValues)
     }

     onFlashModeReceived: {

         if(flashMode == See3CAM_CU512M.DISABLE)
         {
            flashDisable.checked = true
         }
         else if(flashMode == See3CAM_CU512M.STROBE)
         {
            strobeMode.checked = true
         }
         else if(flashMode == See3CAM_CU512M.TORCH)
         {
            torchMode.checked = true
         }
     }
   }

   function setMasterMode(){
       see3camcu512m.setStreamMode(See3CAM_CU512M.MASTER)

       root.startUpdatePreviewInMasterMode()

       root.checkForTriggerMode(false)
       root.captureBtnEnable(true)
       root.videoRecordBtnEnable(true)
   }

   function setTriggerMode(){
       see3camcu512m.setStreamMode(See3CAM_CU512M.TRIGGER)

       root.stopUpdatePreviewInTriggerMode()

       root.checkForTriggerMode(true)
       root.captureBtnEnable(false)
       root.videoRecordBtnEnable(false)
   }

   function currentFlipMirrorMode(mode)
   {
       switch(mode)
       {
       case See3CAM_CU512M.NORMAL:
           flipCtrlVertical.checked  = false
           flipCtrlHorizotal.checked = false
           break;
       case See3CAM_CU512M.VERTICAL:
           flipCtrlVertical.checked  = true
           flipCtrlHorizotal.checked = false
           break;
       case See3CAM_CU512M.HORIZONTAL:
           flipCtrlVertical.checked  = false
           flipCtrlHorizotal.checked = true
           break;
       case See3CAM_CU512M.ROTATE_180:
           flipCtrlVertical.checked  = true
           flipCtrlHorizotal.checked = true
           break;
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
        see3camcu512m.setToDefaultValues()
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
        see3camcu512m.getStreamMode()
        see3camcu512m.getOrientation()
        see3camcu512m.getFlashMode()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
