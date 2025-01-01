/*
 * see3cam_50cug_m.qml -- extension settings for other cameras
 * Copyright © 2024 e-con Systems India Pvt. Limited
 *
 * This file is part of Qtcam.
 *
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam_50cug_m 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item{

    width:240
    height:720

    property int imageBurstMin: 0
    property int imageBurstMax: 5

    property int blackLevelMin: 0
    property int blackLevelMax: 4095

    property bool skipUpdateBlackLevelMode  : false

    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIFlickerCtrl:false
    property int  triggerCtrl

    Action {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }

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
    Connections{
        target: root

        onTakeScreenShot:
        {
            root.imageCapture(CommonEnums.BURST_SHOT);
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

    ScrollView
    {
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle

        Item{
           height: 900
            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text {
                    id: cameraMode
                    text: "--- Camera Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ColumnLayout{
                   ExclusiveGroup { id: cameraModeGroup }
                   spacing: 25
                   RadioButton {
                       id: masterMode
                       style:  econRadioButtonStyle
                       text: qsTr("Master")
                       exclusiveGroup: cameraModeGroup
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
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       onClicked: {
                           //enable trigger combobox
                           triggerCombo.enabled = true
                           triggerCombo.opacity = 1

                           setTriggerMode()
                       }
                       Keys.onReturnPressed: {
                           triggerCombo.enabled = true
                           triggerCombo.opacity = 1

                           setTriggerMode()
                       }
                   }
                   ComboBox
                   {
                       id: triggerCombo
                       opacity: 1
                       enabled: true
                       model: ListModel {
                           ListElement { text: "Exposure control" }
                           ListElement { text: "Acquisition control" }
                       }
                       activeFocusOnPress: true
                       style: econComboBoxStyle
                       onCurrentIndexChanged: {
                           setTriggerMode()
                       }
                   }
               }

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
                            defaultValue.enabled = true
                            see3cam50cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3cam50cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            defaultValue.enabled = true
                            see3cam50cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3cam50cugm.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                }

                Text {
                    id: blackLevelAdj
                    text: "--- black Level Adjustment ---"
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
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: blackLevelSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: blackLevelMin
                        maximumValue: blackLevelMax
                        onValueChanged:  {
                            blackLevelTextField.text = blackLevelSlider.value
                            if(skipUpdateBlackLevelMode){
                                see3cam50cugm.setBlackLevelAdjustment(blackLevelSlider.value)
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

                Text {
                    id: strobe
                    text: "--- Strobe ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ColumnLayout{
                    spacing: 10
                    ExclusiveGroup { id: strobesGrp }
                    RadioButton {
                        exclusiveGroup: strobesGrp
                        checked: false
                        id: strobeOn
                        text: "On"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            see3cam50cugm.setStrobeMode(SEE3CAM_50CUGM.FLASH)
                        }
                        Keys.onReturnPressed: {
                            see3cam50cugm.setStrobeMode(SEE3CAM_50CUGM.FLASH)
                        }
                    }

                    RadioButton {
                        exclusiveGroup: strobesGrp
                        checked: false
                        id: strobeOff
                        text: "Off"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            see3cam50cugm.setStrobeMode(SEE3CAM_50CUGM.OFF)
                        }
                        Keys.onReturnPressed: {
                            see3cam50cugm.setStrobeMode(SEE3CAM_50CUGM.OFF)
                        }
                    }

                    Row{
                        Layout.alignment: Qt.AlignCenter
                        Text {
                            id: imgCapText
                            text: "--- Image Burst ---"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            Layout.alignment: Qt.AlignCenter
                            opacity: 0.50196078431373
                        }
                    }
                    Text {
                        id: burstLength
                        text: "Burst Length :"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                    ComboBox
                    {
                        id: burstLengthCombo
                        opacity: 1
                        enabled: true
                        model: ListModel {
                            ListElement { text: "1" }
                            ListElement { text: "2" }
                            ListElement { text: "3" }
                            ListElement { text: "4" }
                            ListElement { text: "5" }
                        }

                        activeFocusOnPress: true
                        style: econComboBoxStyle
                        onCurrentIndexChanged: {
                            root.stillBurstLength(burstLengthCombo.currentIndex + 1)
                            if(skipUpdateUIOnBurstLength){
                                see3cam50cugm.setBurstLength(burstLengthCombo.currentText)
                            }
                            skipUpdateUIOnBurstLength = true
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
                        style: econButtonStyle
                        action: setDefault
                        onClicked:{
                            setToDefaultValues()
                        }
                        Keys.onReturnPressed: {
                            setToDefaultValues()
                        }
                    }
                }


                Row{
                    Button {
                        id: f_wversion_selected130
                        opacity: 1
                        activeFocusOnPress : true
                        action: firmwareVersion
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
                        onClicked: {
                            getFirmwareVersion()
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
                        action: serialNumber
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

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: saveConfig
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Save Configuration"
                        style: econButtonStyle
                        onClicked:{
                            saveConfigurations()
                        }
                        Keys.onReturnPressed: {
                            saveConfigurations()
                        }
                    }
                }

           }
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

    SEE3CAM_50CUGM{
        id:see3cam50cugm

        onCameraModeChanged:
        {
            if(cameraMode == SEE3CAM_50CUGM.MASTER)
            {
                masterMode.checked = true
                triggerCombo.enabled = false
                triggerCombo.opacity = 0.1

                strobeOn.enabled = false
                strobeOn.opacity = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1

                root.checkForTriggerMode(false)
                root.videoRecordBtnEnable(true)
                root.captureBtnEnable(true)
                root.startUpdatePreviewInMasterMode()
            }
            else
            {
                triggerCombo.enabled = true
                triggerCombo.opacity = 1
                triggerMode.checked = true

                root.checkForTriggerMode(true)
                root.videoRecordBtnEnable(false)
                root.captureBtnEnable(false)
                root.stopUpdatePreviewInTriggerMode()

                if(cameraMode == SEE3CAM_50CUGM.TRIGGER_EXP_CONTROL)
                {
                    strobeOn.enabled = false
                    strobeOn.opacity = 0.1
                    strobeOff.enabled   = false
                    strobeOff.opacity   = 0.1

                    triggerCombo.currentIndex = 0
                }
                else if(cameraMode == SEE3CAM_50CUGM.TRIGGER_ACQ_CONTROL)
                {
                    triggerCombo.currentIndex = 1

                    strobeOn.enabled = true
                    strobeOn.opacity = 1
                    strobeOff.enabled   = true
                    strobeOff.opacity   = 1
                }
            }
        }

        onFlipMirrorModeChanged:
        {
            currentFlipMirrorMode(flipMode)
        }

        onStrobeModeChanged:
        {
            currentStrobeMode(strobe)
            if(masterMode.checked == true)
            {
                strobeOn.enabled = false
                strobeOn.opacity = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
            }
            else if(triggerCombo.currentIndex == 0)
            {
                strobeOn.enabled = false
                strobeOn.opacity = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
            }
            else
            {
                strobeOn.enabled = true
                strobeOn.opacity = 1
                strobeOff.enabled   = true
                strobeOff.opacity   = 1
            }
        }

        onBlackLevelValueChanged:{
            skipUpdateBlackLevelMode = false
            blackLevelSlider.value = blackLevelValue
        }

        onImageBurstChanged:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onIndicateCommandStatus:{
            displayMessageBox(title, text)
        }
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function saveConfigurations()
    {
        see3cam50cugm.saveConfiguration()
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()

    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function setMasterMode()
    {
        //disable strobe mode when the camera is in Master mode
        strobeOn.enabled = false
        strobeOn.opacity = 0.1
        strobeOff.enabled   = false
        strobeOff.opacity   = 0.1

        //disable trigger combobox
        triggerCombo.enabled = false
        triggerCombo.opacity = 0.1

        see3cam50cugm.setCameraMode(SEE3CAM_50CUGM.MASTER)

        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
    }

    function setTriggerMode()
    {
        if(triggerCombo.currentText.toString() == "Exposure control")
        {
           setTriggerExpControl()
        }
        else if(triggerCombo.currentText.toString() == "Acquisition control")
        {
           setTriggerAcqControl()
        }
    }

    function setTriggerExpControl()
    {
        root.checkForTriggerMode(true)
        root.videoRecordBtnEnable(false)
        root.captureBtnEnable(false)
        root.stopUpdatePreviewInTriggerMode()

        //disable strobe mode when the camera is in Master mode
        strobeOn.enabled = false
        strobeOn.opacity = 0.1
        strobeOff.enabled   = false
        strobeOff.opacity   = 0.1

        see3cam50cugm.setCameraMode(SEE3CAM_50CUGM.TRIGGER_EXP_CONTROL)
    }

    function setTriggerAcqControl()
    {
        root.checkForTriggerMode(true)
        root.videoRecordBtnEnable(false)
        root.captureBtnEnable(false)
        root.stopUpdatePreviewInTriggerMode()

        //enable strobe mode when the camera is in trigger mode
        strobeOn.enabled = true
        strobeOn.opacity = 1
        strobeOff.enabled   = true
        strobeOff.opacity   = 1

        see3cam50cugm.setCameraMode(SEE3CAM_50CUGM.TRIGGER_ACQ_CONTROL)
    }


    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case SEE3CAM_50CUGM.NORMAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        case SEE3CAM_50CUGM.VERTICAL:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case SEE3CAM_50CUGM.HORIZONTAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case SEE3CAM_50CUGM.ROTATE_180:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        }
    }

    function currentStrobeMode(mode){
        switch(mode){
            case SEE3CAM_50CUGM.OFF:
                strobeOff.checked   = true
                strobeOn.checked = false
                break;
            case SEE3CAM_50CUGM.FLASH:
                strobeOff.checked   = false
                strobeOn.checked = true
                break;
        }
    }

    function setToDefaultValues(){
        defaultValue.enabled = false //To avoid multiple clicks over Default button
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
	root.startUpdatePreviewInMasterMode()

        if(see3cam50cugm.setToDefaultValues())
        {
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        see3cam50cugm.getCameraMode()
        see3cam50cugm.getOrientation()
        see3cam50cugm.getBlackLevelAdjustment()
        see3cam50cugm.getStrobeMode()
        see3cam50cugm.getBurstLength()
    }
    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
}
