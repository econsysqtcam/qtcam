/*
 * see3camcu22.qml -- extension settings for other cameras
 * Copyright © 2024  e-con Systems India Pvt. Limited
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
import econ.camera.see3camcu22 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {

    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnColorKillVal: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIOnAntiFlickerMode:false
    property bool skipUpdateUIOnExpComp: false
    property bool setButtonClicked: false
    property int expCompMinValue : 1
    property int expCompMaxValue : 60
    property bool autoExposureMode: true

    width:240
    height:720
    property int colorValMin: 0
    property int colorValMax: 5

    // Delay is needed to get correct auto exposure ROI mode
    Timer{
        id:autoExpROITimer
        interval: 2000
        onTriggered: {
            see3camcu22.getAutoExpROIModeAndWindowSize()
        }
    }

    Connections
    {
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
        onMouseRightClicked:{
            if(autoexpManual.enabled && autoexpManual.checked){
                see3camcu22.setROIAutoExposure(See3Camcu22.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }

        onAutoExposureSelected:{
            autoExposureMode = autoExposureSelect
            if(autoExposureSelect){
                autoexpManual.enabled = true
                autoexpCentered.enabled = true
                if(autoexpManual.checked)
                    autoExpoWinSizeCombo.enabled = true
                if(autoexpCentered.checked)
                    autoExpoWinSizeCombo.enabled = false
                autoexpManual.opacity = 1
                autoexpCentered.opacity = 1
            }else{
                autoexpManual.enabled = false
                autoexpCentered.enabled = false
                autoExpoWinSizeCombo.enabled = false
                autoexpManual.opacity = 0.1
                autoexpCentered.opacity = 0.1
            }
            autoExpROITimer.start()
        }
        onExtensionTabVisible:{
            if(visible)
                getCurrentValuesFromCamera()
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
        Item {
            height: 1500
            ColumnLayout{
                x:2
                y:5
                spacing:20
                Text {
                    id: sensor_mode
                    text: "--- Sensor Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Grid {
                    columns: 2
                    spacing: 20
                    ExclusiveGroup { id: sensorInputGroup } //Last Edited by M VishnuMurali(24 Mar 2020)
                    RadioButton {
                        id: sensorStandard
                        style:  econRadioButtonStyle
                        text:   qsTr("Standard")
                        exclusiveGroup: sensorInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu22.setSensorMode(See3Camcu22.SENSOR_STANDARD)
                            defaultValue.enabled = true
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setSensorMode(See3Camcu22.SENSOR_STANDARD)
                            defaultValue.enabled = true
                        }
                    }
                    RadioButton {   //Last edited by M Vishnu Murali(10/04/2020):Removed tooltip since ubuntu 14.04 doesnt support.
                        id: sensorHdr
                        style:  econRadioButtonStyle
                        text: qsTr("HDR")
                        opacity: enabled ? 1 : 0.5
                        exclusiveGroup: sensorInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu22.setSensorMode(See3Camcu22.SENSOR_HDR)
                            defaultValue.enabled = true
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setSensorMode(See3Camcu22.SENSOR_HDR)
                            defaultValue.enabled = true
                        }
                    }
                }
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
                Row{
                    spacing: 60
                    ExclusiveGroup { id: cameraModeGroup }
                    RadioButton {
                        id: cameraModeMaster
                        style:  econRadioButtonStyle
                        text:   qsTr("Master")
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
                        id: cameraModeTrigger
                        style:  econRadioButtonStyle
                        text: qsTr("Trigger")                       
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            defaultValue.enabled = true
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            setTriggerMode()
                        }
                    }
                }
                Text {
                    id: specialMode
                    text: "--- Special Effect ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 60
                    ExclusiveGroup { id: specialModeGroup }
                    RadioButton {
                        id: specialModeNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")                     
                        exclusiveGroup: specialModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu22.setSpecialMode(See3Camcu22.SPECIAL_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setSpecialMode(See3Camcu22.SPECIAL_NORMAL)
                        }
                    }
                    RadioButton {
                        id: specialModeGreyscale
                        style:  econRadioButtonStyle
                        text: qsTr("Greyscale")                      
                        exclusiveGroup: specialModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            defaultValue.enabled = true
                            see3camcu22.setSpecialMode(See3Camcu22.SPECIAL_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu22.setSpecialMode(See3Camcu22.SPECIAL_GREYSCALE)
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
                            defaultValue.enabled = true
                            see3camcu22.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu22.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            defaultValue.enabled = true
                            see3camcu22.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu22.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                }
                Text {
                    id: roiAutoExpMode
                    text: "--- ROI - Auto Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:60
                    ExclusiveGroup { id: roiExpogroup }
                    RadioButton {
                        exclusiveGroup: roiExpogroup
                        id: autoexpCentered
                        text: "Centered"                   
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                        // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                        // winSize is required only for manual mode
                        onClicked: {
                            see3camcu22.setROIAutoExposure(See3Camcu22.AutoExpCentered, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setROIAutoExposure(See3Camcu22.AutoExpCentered, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                    }
                    RadioButton {
                        exclusiveGroup: roiExpogroup
                        id: autoexpManual
                        text: "Manual"                       
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            see3camcu22.setROIAutoExposure(See3Camcu22.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = true
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setROIAutoExposure(See3Camcu22.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = true
                        }
                    }
                }
                Text {
                    id: windowSizeText
                    text: "Window Size :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (autoexpManual.enabled && autoexpManual.checked) ? 1 : 0.1
                }
                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (autoexpManual.enabled && autoexpManual.checked) ? true : false
                    opacity: (autoexpManual.enabled && autoexpManual.checked) ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "1" }
                        ListElement { text: "2" }
                        ListElement { text: "3" }
                        ListElement { text: "4" }
                        ListElement { text: "5" }
                        ListElement { text: "6" }
                        ListElement { text: "7" }
                        ListElement { text: "8" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnExpWindowSize){
                            see3camcu22.setROIAutoExposure(See3Camcu22.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }

                Text {
                    id: colourKillValStatusText
                    text: "--- Colour Kill ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 35
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: colourKillValSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: colorValMin
                        maximumValue: colorValMax
                        onValueChanged:  {
                            colorKillTextField.text = colourKillValSlider.value
                            if(skipUpdateUIOnColorKillVal){
                                see3camcu22.setColourKill(colourKillValSlider.value)
                            }
                            skipUpdateUIOnColorKillVal = true
                        }
                    }
                    TextField {
                        id: colorKillTextField
                        text: colourKillValSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: colourKillValSlider.minimumValue; top: colourKillValSlider.maximumValue}
                    }
                }
                Text {
                    id: imgCapText
                    text: "--- Image Capture ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
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
                        root.stillBurstLength(burstLengthCombo.currentIndex + 1) // combobox index starts from 0
                        if(skipUpdateUIOnBurstLength){
                            see3camcu22.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }
                Text {
                    id: denoiseControlText
                    text: "--- Denoise Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 80
                    ExclusiveGroup { id: denoiseControlGroup }
                    RadioButton {
                        id: denoiseEnable
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu22.setDenoiseCtrlMode(See3Camcu22.DenoiseEnable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setDenoiseCtrlMode(See3Camcu22.DenoiseEnable)
                        }
                    }
                    RadioButton {
                        id: denoiseDisable
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu22.setDenoiseCtrlMode(See3Camcu22.DenoiseDisable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setDenoiseCtrlMode(See3Camcu22.DenoiseDisable)
                        }
                    }
                }

                Text {
                    id: lscModeText
                    text: "--- Lens Correction Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Grid{
                    spacing:20
                    columns :2
                    ExclusiveGroup { id: lscMode }
                    RadioButton {
                        exclusiveGroup: lscMode
                        id: warmWhiteMode
                        text: "Warm White"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            see3camcu22.setLSCMode(See3Camcu22.LSC_WARMWHITE)
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setLSCMode(See3Camcu22.LSC_WARMWHITE)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: lscMode
                        id: brightWhiteMode
                        text: "Bright  White"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            see3camcu22.setLSCMode(See3Camcu22.LSC_BRIGHTWHITE)
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setLSCMode(See3Camcu22.LSC_BRIGHTWHITE)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: lscMode
                        id: dayLightMode
                        text: "Day Light"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            see3camcu22.setLSCMode(See3Camcu22.LSC_DAYLIGHT)
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setLSCMode(See3Camcu22.LSC_DAYLIGHT)
                        }
                    }
                }

                Text {
                    id: antiFlickerMode
                    text: "--- Anti Flicker Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:90
                    ExclusiveGroup { id: antiFlickerModegroup }
                    RadioButton {
                        exclusiveGroup: antiFlickerModegroup
                        id: antiFlickerModeAuto
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            see3camcu22.setAntiFlickerMode(See3Camcu22.AntiFlickerAuto);
                        }
                        Keys.onReturnPressed: {
                            see3camcu22.setAntiFlickerMode(See3Camcu22.AntiFlickerAuto);
                        }
                    }
                    RadioButton {
                        exclusiveGroup: antiFlickerModegroup
                        id: antiFlickerModeManual
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            setAntiFlickerMode()
                        }
                        Keys.onReturnPressed: {
                            setAntiFlickerMode()
                        }
                    }
                }

                Text {
                    id: frequency
                    text: "Frequency :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? 1 : 0.1
                }
                ComboBox
                {
                    id: antiFlickerCombo
                    enabled: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? true : false
                    opacity: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "50 Hz" }
                        ListElement { text: "60 Hz" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnAntiFlickerMode){
                            setAntiFlickerMode()
                        }
                        skipUpdateUIOnAntiFlickerMode = true
                    }
                }
                Row{                                         //Added by M.Vishnu MUrali(25/03/2020)
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: defaultValue
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Default"
                        tooltip: "Click to set default values in extension controls"
                        style: econButtonStyle
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

    See3Camcu22 {
        id: see3camcu22
        onDenoiseModeChanged:{
            if(denoiseMode == See3Camcu22.DenoiseEnable){
                denoiseEnable.checked = true
            }else if(denoiseMode == See3Camcu22.DenoiseDisable){
                denoiseDisable.checked = true
            }
        }

        onSensorModeReceived:{
            if(sensorMode == See3Camcu22.SENSOR_STANDARD){
                sensorStandard.checked = true
            }else if(sensorMode == See3Camcu22.SENSOR_HDR){
                sensorHdr.checked = true
            }
        }
        onCameraModeReceived:{
            if(cameraMode == See3Camcu22.CAMERA_MASTER){
                cameraModeMaster.checked = true
                root.checkForTriggerMode(false)
                root.videoRecordBtnEnable(true)
                root.captureBtnEnable(true)
                root.startUpdatePreviewInMasterMode()
            }else if(cameraMode == See3Camcu22.CAMERA_TRIGGER){
                cameraModeTrigger.checked = true
                root.checkForTriggerMode(true)
                root.videoRecordBtnEnable(false)
                root.captureBtnEnable(false)
                root.stopUpdatePreviewInTriggerMode()
            }
        }
        onSpecialModeReceived:{
            if(specialMode == See3Camcu22.SPECIAL_NORMAL){
                specialModeNormal.checked = true
            }else if(specialMode == See3Camcu22.SPECIAL_GREYSCALE){
                specialModeGreyscale.checked = true
            }
        }
        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onRoiAutoExpModeChanged:{
            if(roiMode == See3Camcu22.AutoExpCentered){
                autoexpCentered.checked = true
            }else if(roiMode == See3Camcu22.AutoExpManual){
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize - 1
                skipUpdateUIOnExpWindowSize = true
            }
            else if(roiMode == See3Camcu22.AutoExpDisabled){
                autoexpCentered.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
            }
        }
        onColorKillValueChanged:{
            skipUpdateUIOnColorKillVal = false
            colourKillValSlider.value = colorKillValue
            skipUpdateUIOnColorKillVal = true
        }
        onBurstLengthChanged:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onAntiFlickerModeChanged:{
            currentAntiFlickerMode(flickerMode)
        }
        onLscModeChanged:{
            currentLSCMode(lscMode)
        }
        onSetdefaultValueFailed:{
            displayMessageBox(qsTr("Failure"), qsTr("Setting default value is failed"))
        }
        onDisableHDR: {
            if(hdrstatus){
                sensorHdr.enabled = false
                sensorStandard.checked = true
            }
            else{
                sensorHdr.enabled = true
            }
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function currentLSCMode(Mode){
        switch(Mode){
        case See3Camcu22.LSC_WARMWHITE:
            warmWhiteMode.checked = true
            break;
        case See3Camcu22.LSC_BRIGHTWHITE:
            brightWhiteMode.checked = true
            break;
        case See3Camcu22.LSC_DAYLIGHT:
            dayLightMode.checked = true
            break;
        }
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function currentAntiFlickerMode(flickerMode){
        switch(flickerMode){
        case See3Camcu22.AntiFlickerAuto:
            antiFlickerModeAuto.checked = true
            break
        case See3Camcu22.AntiFlicker50Hz:
            antiFlickerModeManual.checked = true
            skipUpdateUIOnAntiFlickerMode = false
            antiFlickerCombo.currentIndex = 0
            skipUpdateUIOnAntiFlickerMode = true
            break
        case See3Camcu22.AntiFlicker60Hz:
            antiFlickerModeManual.checked = true
            skipUpdateUIOnAntiFlickerMode = false
            antiFlickerCombo.currentIndex = 1
            skipUpdateUIOnAntiFlickerMode = true
            break
        }
    }

    // set anti flicker mode in the camera based on anti flicker index in combobox
    function setAntiFlickerMode(){
        if(antiFlickerCombo.currentIndex == 0)
            see3camcu22.setAntiFlickerMode(See3Camcu22.AntiFlicker50Hz)
        else
            see3camcu22.setAntiFlickerMode(See3Camcu22.AntiFlicker60Hz)
    }

    // Flip mirror modes
    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case See3Camcu22.SetBothFlipEnable:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        case See3Camcu22.SetVertiFlip:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case See3Camcu22.SetHorzFlip:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case See3Camcu22.SetBothFlipDisable:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        }
    }
    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        root.stopUpdatePreviewInTriggerMode()
        see3camcu22.setCameraMode(See3Camcu22.CAMERA_TRIGGER)
    }

    function setMasterMode(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        see3camcu22.setCameraMode(See3Camcu22.CAMERA_MASTER)
    }

    function setToDefaultValues(){ //Added by M Vishnu Murali(25/03/2020): Function to restore defaullt values in extension settings.
        root.checkForTriggerMode(false)
        defaultValue.enabled = false
        root.captureBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        root.videoRecordBtnEnable(true)
        if(see3camcu22.setToDefaultValues()){
            see3camcu22.getAutoExpROIModeAndWindowSize()
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        see3camcu22.getSensorMode()
        see3camcu22.getCameraMode()
        see3camcu22.getSpecialMode()
        see3camcu22.getOrientation()
        see3camcu22.getColourKill()
        see3camcu22.getBurstLength()
        see3camcu22.getAntiFlickerMode()
        see3camcu22.getDenoiseCtrlMode()
        see3camcu22.getLSCMode()
    }

    Component.onCompleted:{
        autoExpROITimer.start()
        getCurrentValuesFromCamera()
    }
}




