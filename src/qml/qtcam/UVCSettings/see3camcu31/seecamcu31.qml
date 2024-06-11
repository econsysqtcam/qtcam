import econ.camera.see3camcu31 1.0
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

    property int convergenceSpeedMin: 0
    property int convergenceSpeedMax: 255

    property bool skipUpdateUIAutoExposure: false

    property bool uvcAutoExposureSelected: false

    Action {
        id: firmwareVersion
        onTriggered:
        {
            getFirmwareVersion()
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
        onUpdateCrossStillCaptureProperty:{
            see3camcu31.setPropertiesForCrossStill()
        }
        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
        }
    }
    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle

        Item {
            height: 1200
            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text {
                    id: flipMode
                    text: "--- Orientation Mode ---"
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
                            see3camcu31.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            see3camcu31.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: rnrModeTitle
                        text: "--- Raw Noise Reduction ---"
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
                    ExclusiveGroup { id: rnrModeGroup }
                    RadioButton {
                        id: disableRNR
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: rnrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_DISABLE)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_DISABLE)
                        }
                    }
                    RadioButton {
                        id: enableRNR
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: rnrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_ENABLE)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_ENABLE)
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: triggerModeTitle
                        text: "--- Trigger Mode ---"
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
                    ExclusiveGroup { id: triggerModeGroup }
                    RadioButton {
                        id: triggerDisable
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: triggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            disableTriggerMode()
                        }
                        Keys.onReturnPressed:{
                            disableTriggerMode()
                        }
                    }
                    RadioButton {
                        id: triggerReadOut
                        style:  econRadioButtonStyle
                        text: qsTr("Readout")
                        exclusiveGroup: triggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            setTriggerMode()
                        }
                    }
                    RadioButton {
                        id: triggerShutter
                        style:  econRadioButtonStyle
                        text: qsTr("Shutter")
                        exclusiveGroup: triggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            setTriggerMode()
                        }
                    }
                }
                Text
                {
                    id: convergenceSpeedTitle
                    text: "--- AE Convergence Speed ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: convergenceSpeedSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: convergenceSpeedMin
                        maximumValue: convergenceSpeedMax
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onValueChanged:
                        {
                            convergenceSpeedTextField.text = convergenceSpeedSlider.value
                            if(skipUpdateUIAutoExposure)
                            {
                                see3camcu31.setAEConvergenceSpeed(convergenceSpeedSlider.value)
                            }
                            skipUpdateUIAutoExposure = true
                        }
                    }
                    TextField
                    {
                        id: convergenceSpeedTextField
                        text: convergenceSpeedSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        validator: IntValidator {bottom: convergenceSpeedSlider.minimumValue; top: convergenceSpeedSlider.maximumValue}
                        onTextChanged:
                        {
                            if(text.length > 0)
                            {
                                convergenceSpeedSlider.value = convergenceSpeedTextField.text
                            }
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: autoExpStatusTitle
                        text: "--- AE HOLD ---"
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
                    ExclusiveGroup { id: autoExpStatusGroup }
                    RadioButton {
                        id: autoExposure
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: autoExpStatusGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            setAutoExposure()
                        }
                        Keys.onReturnPressed:  {
                            setAutoExposure()
                        }
                    }
                    RadioButton {
                        id: autoExposureHold
                        style:  econRadioButtonStyle
                        text: qsTr("Hold")
                        exclusiveGroup: autoExpStatusGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            setAutoExposure()
                        }
                        Keys.onReturnPressed: {
                            setAutoExposure()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbStatusTitle
                        text: "--- AWB HOLD ---"
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
                    ExclusiveGroup { id: awbStatusGroup }
                    RadioButton {
                        id: awbRelease
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: awbStatusGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_RELEASE)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_RELEASE)
                        }
                    }
                    RadioButton {
                        id: awbHold
                        style:  econRadioButtonStyle
                        text: qsTr("Hold")
                        exclusiveGroup: awbStatusGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_HOLD)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_HOLD)
                        }
                    }
                }

                Text {
                    id: readTempTitle
                    text: "--- Read Temperature ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row
                {
                    spacing: 13
                    Text {
                        id: readTempText
                        text: "Temperature (°C)"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                    }
                    TextField
                    {
                        id: readTempTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        readOnly: true
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                    }
                    Button {
                        id: readTempBtn
                        activeFocusOnPress : true
                        text: "Get"
                        style: econButtonStyle
                        implicitHeight: 20
                        implicitWidth: 45
                        onClicked: {
                            see3camcu31.readTemperature()
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.readTemperature()
                        }
                    }
                }

                Text
                {
                    id: antiFlickerMode
                    text: "--- Anti-Flicker Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 35
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: antiFlickerGroup }
                    RadioButton {
                        id: flickerAuto
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: antiFlickerGroup
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        activeFocusOnPress: true
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                    RadioButton {
                        id: flicker50hz
                        style:  econRadioButtonStyle
                        text: qsTr("50 Hz")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                }

                Row{
                    spacing: 35
                    Layout.alignment: Qt.AlignCenter

                    RadioButton {
                        id: flicker60hz
                        style:  econRadioButtonStyle
                        text: qsTr("60 Hz")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                    RadioButton {
                        id: flickerDisable
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: cameraModeText
                        text: "--- Enhancement Modes ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }

                Row{
                    spacing: 20
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: cameraModeGroup }

                    RadioButton {
                        id: autoCameraMode
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.AUTO)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.AUTO)
                        }
                    }
                    RadioButton {
                        id: lowLightMode
                        style:  econRadioButtonStyle
                        text: qsTr("Low Light")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LOW_LIGHT)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LOW_LIGHT)
                        }
                    }
                    RadioButton {
                        id: ledMode
                        style:  econRadioButtonStyle
                        text: qsTr("LED")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LED)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LED)
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
                        onClicked: {
                            getSerialNumber()
                        }
                        Keys.onReturnPressed: {
                            getSerialNumber()
                        }
                    }
                }
            }
        }
    }

   SEE3CAM_CU31{
     id:see3camcu31

     onFlipMirrorModeChanged:
     {
         currentFlipMirrorMode(flipMirrorModeValues)
     }

     onCurrentRawNoiseDeductionStatus: {
         if(status == SEE3CAM_CU31.RNR_ENABLE)
         {
            enableRNR.checked = true
         }
         else if(status == SEE3CAM_CU31.RNR_DISABLE)
         {
            disableRNR.checked = true
         }
     }
     onCurrentTriggerMode:{
         if(mode == SEE3CAM_CU31.DISABLE_TRIGGER)
         {
             triggerDisable.checked = true
             root.startUpdatePreviewInMasterMode()

             root.checkForTriggerMode(false)
             root.captureBtnEnable(true)
             root.videoRecordBtnEnable(true)
         }
         else if(mode == SEE3CAM_CU31.READOUT_TRIGGER)
         {
             triggerReadOut.checked = true
             root.stopUpdatePreviewInTriggerMode()

             root.checkForTriggerMode(true)
             root.captureBtnEnable(false)
             root.videoRecordBtnEnable(false)
         }
         else if(mode == SEE3CAM_CU31.SHUTTER_TRIGGER)
         {
             triggerShutter.checked = true
             root.stopUpdatePreviewInTriggerMode()

             root.checkForTriggerMode(true)
             root.captureBtnEnable(false)
             root.videoRecordBtnEnable(false)
         }
     }
     onCurrentAEConvergenceSpeed:{
         skipUpdateUIAutoExposure = false
         convergenceSpeedSlider.value = currentSpeed
         skipUpdateUIAutoExposure = true
     }

     onCurrentAutoExposureStatus:{
         if(status == SEE3CAM_CU31.AE_RELEASE){
            autoExposure.checked = true
         }
         else if(status == SEE3CAM_CU31.AE_HOLD){
            autoExposureHold.checked = true
         }
     }

     onCurrentAutoWhiteBalanceStatus:{
         if(status === SEE3CAM_CU31.AWB_RELEASE){
            awbRelease.checked = true
         } else if(status === SEE3CAM_CU31.AWB_HOLD) {
            awbHold.checked = true
         }
     }

     onCurrentAntiFlickerModeRecieved: {
        currentAntiFlickerMode(mode)
     }

     onCurrentCameraModeReceived: {
        currentCameraMode(mode)
     }

     onCurrentTemperature: {
        temperature = parseFloat((temperature).toFixed(2));
        readTempTextField.text = temperature.toFixed(2)
     }
     onTitleTextChanged: {
         messageDialog.title = _title.toString()
         messageDialog.text = _text.toString()
         messageDialog.open()
     }

     onIndicateFailureStatus:{
         displayMessageBox(title, text)
     }
   }

   function currentCameraMode(mode){
       if(mode === SEE3CAM_CU31.AUTO){
          autoCameraMode.checked = true
       }else if(mode === SEE3CAM_CU31.LOW_LIGHT){
          lowLightMode.checked = true
       }else if(mode === SEE3CAM_CU31.LED){
          ledMode.checked = true
       }
   }

   function currentAntiFlickerMode(mode){
       if(mode === SEE3CAM_CU31.MODE_AUTO){
          flickerAuto.checked = true
       } else if(mode === SEE3CAM_CU31.MODE_50Hz){
          flicker50hz.checked = true
       } else if(mode === SEE3CAM_CU31.MODE_60Hz){
          flicker60hz.checked = true
       } else if(mode === SEE3CAM_CU31.MODE_DISABLE){
          flickerDisable.checked = true
       }
   }

   function setFlickerControl(){
       if(flickerAuto.checked == true){
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_AUTO)
       } else if (flicker50hz.checked == true) {
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_50Hz)
       } else if(flicker60hz.checked == true) {
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_60Hz)
       } else if(flickerDisable.checked == true) {
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_DISABLE)
       }
   }

   function disableTriggerMode(){
       see3camcu31.setTriggerMode(SEE3CAM_CU31.DISABLE_TRIGGER)

       root.startUpdatePreviewInMasterMode()

       root.checkForTriggerMode(false)
       root.captureBtnEnable(true)
       root.videoRecordBtnEnable(true)
   }

   function setTriggerMode(){
       if(triggerReadOut.checked)
       {
           see3camcu31.setTriggerMode(SEE3CAM_CU31.READOUT_TRIGGER)
       }
       else if(triggerShutter.checked)
       {
           see3camcu31.setTriggerMode(SEE3CAM_CU31.SHUTTER_TRIGGER)
       }

       root.stopUpdatePreviewInTriggerMode()

       root.checkForTriggerMode(true)
       root.captureBtnEnable(false)
       root.videoRecordBtnEnable(false)
   }

   function currentFlipMirrorMode(mode)
   {
       switch(mode)
       {
           case SEE3CAM_CU31.NORMAL:
               flipCtrlVertical.checked  = false
               flipCtrlHorizotal.checked = false
               break;
           case SEE3CAM_CU31.VERTICAL:
               flipCtrlVertical.checked  = true
               flipCtrlHorizotal.checked = false
               break;
           case SEE3CAM_CU31.HORIZONTAL:
               flipCtrlVertical.checked  = false
               flipCtrlHorizotal.checked = true
               break;
           case SEE3CAM_CU31.ROTATE_180:
               flipCtrlVertical.checked  = true
               flipCtrlHorizotal.checked = true
               break;
       }
   }

   function setAutoExposure() {
        if(autoExposure.checked == true){
            see3camcu31.setAutoExposureStatus(SEE3CAM_CU31.AE_RELEASE)
        } else if(autoExposureHold.checked == true){
            see3camcu31.setAutoExposureStatus(SEE3CAM_CU31.AE_HOLD)
        }
   }

   function enableDisableAutoExposureControls(autoExposureSelect){
       if(autoExposureSelect){
           uvcAutoExposureSelected = true
           if(autoExposureHold.checked == true){
               displayMessageBox("Message", "AE hold is active")
           }
       }else{
           uvcAutoExposureSelected = false
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
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 50
                implicitHeight: 23
                border.color: "#333"
                border.width: 3
                y: 1
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

    Component {
        id: econComboBoxStyle
        ComboBoxStyle {
            background: Image {
                id: combo_bkgrnd
                source: "../../Views/images/device_box.png"
                Rectangle {
                    width: combo_bkgrnd.sourceSize.width  - 28
                    height: combo_bkgrnd.sourceSize.height
                    color: "#222021"
                    border.color: "white"
                    border.width: control.activeFocus ? 3 : 1
                    radius: control.activeFocus ? 5 : 0
                }
            }
            label:  Text{
                anchors.fill: parent
                color: "#ffffff"
                elide: Text.ElideRight
                text: control.currentText
                verticalAlignment: Text.AlignVCenter
                maximumLineCount: 1
                font.family: "Ubuntu"
                font.pixelSize: 14
            }
        }
    }

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        see3camcu31.setToDefaultValues()
        getValuesFromCamera()
    }

    function getSerialNumber() {
        see3camcu31.get64BitSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function getValuesFromCamera(){
        see3camcu31.getOrientation()
        see3camcu31.getRawNoiseReductionStatus()
        see3camcu31.getTriggerMode()
        see3camcu31.getAEConvergenceSpeed()
        see3camcu31.getAutoExpStatus()
        see3camcu31.getAWBStatus()
        see3camcu31.readTemperature()
        see3camcu31.getCameraMode()
        see3camcu31.getAntiFlickerMode()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
