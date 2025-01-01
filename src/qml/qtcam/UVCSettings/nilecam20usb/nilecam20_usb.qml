/*
 * uvc_cu30.qml -- extension settings for other cameras
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
import econ.camera.nilecam20_usb 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {

    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnColorKillVal: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIOnAntiFlickerMode:false
    property bool skipUpdateUIOnLSCMode: false
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
            nilecam20usb.getROIAutoExposure()
        }
    }

    Timer{
        id:exposureCompensationTimer
        interval: 500
        repeat: false
        onTriggered: {
            nilecam20usb.getExposureCompensation()
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
        onSkipFrameCount:{
            root.setSkipFrameCount(3)
        }
        onMouseRightClicked:{
            if(autoexpManual.enabled && autoexpManual.checked){
                nilecam20usb.setROIAutoExposure(nilecam20usb.AUTO_EXP_MANNUAL, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        onVideoResolutionChanged:{
            exposureCompensationTimer.start()
        }
        onPreviewFPSChanged:{
            exposureCompensationTimer.start()
        }
        onVideoColorSpaceChanged:{
            nilecam20usb.getExposureCompensation()
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
                if(sensorStandard.checked){
                    exposureCompSlider.enabled = true
                    exposureCompSlider.opacity  = 1
                    exposureCompTextValue.opacity = 1
                }else if(sensorHdrDlo.checked){
                    exposureCompSlider.enabled = false
                    exposureCompSlider.opacity  = 0.1
                    exposureCompTextValue.opacity = 0.1
                }
            }else{
                autoexpManual.enabled = false
                autoexpCentered.enabled = false
                autoExpoWinSizeCombo.enabled = false
                exposureCompSlider.enabled = false
                autoexpManual.opacity = 0.1
                autoexpCentered.opacity = 0.1
                exposureCompSlider.opacity = 0.1
                exposureCompTextValue.opacity = 0.1
            }
            autoExpROITimer.start()
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

                Row{
                    spacing: 50
                    ExclusiveGroup { id: sensorInputGroup }
                    RadioButton {
                        id: sensorStandard
                        style:  econRadioButtonStyle
                        text:   qsTr("Standard")
                        exclusiveGroup: sensorInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            sensorStandardModeSelected()
                        }
                        Keys.onReturnPressed: {
                            sensorStandardModeSelected()
                        }
                    }
                    RadioButton {
                        id: sensorHdrDlo
                        style:  econRadioButtonStyle
                        text: qsTr("HDR")
                        exclusiveGroup: sensorInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            sensorHDRModeSelected()
                        }
                        Keys.onReturnPressed: {
                            sensorHDRModeSelected()
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
                        id: cameraModeSlave
                        style:  econRadioButtonStyle
                        text: qsTr("Trigger")
                        exclusiveGroup: cameraModeGroup
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
                            nilecam20usb.setSpecialMode(NileCam20_USB.SPECIAL_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setSpecialMode(NileCam20_USB.SPECIAL_NORMAL)
                        }
                    }
                    RadioButton {
                        id: specialModeGreyscale
                        style:  econRadioButtonStyle
                        text: qsTr("Greyscale")
                        exclusiveGroup: specialModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            nilecam20usb.setSpecialMode(NileCam20_USB.SPECIAL_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setSpecialMode(NileCam20_USB.SPECIAL_GREYSCALE)
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
                            nilecam20usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            nilecam20usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            defaultValue.enabled = true
                            nilecam20usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            nilecam20usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
                        id: strobeFlashVidoStreaming
                        text: "Flash For Video Streaming"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.FLASH_FOR_VIDEO_STREAMING)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.FLASH_FOR_VIDEO_STREAMING)
                        }
                    }

                    RadioButton {
                        exclusiveGroup: strobesGrp
                        checked: false
                        id: strobeStillTrigger
                        text: "Still Trigger"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.FLASH_FOR_EXT_STILL_TRIGGER)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.FLASH_FOR_EXT_STILL_TRIGGER)
                        }
                    }

                    RadioButton {
                        exclusiveGroup: strobesGrp
                        checked: false
                        id: strobeTorch
                        text: "Torch"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.TORCH)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.TORCH)
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
                            nilecam20usb.setStrobeMode(NileCam20_USB.OFF)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setStrobeMode(NileCam20_USB.OFF)
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

                        /* setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                           videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                           winSize is required only for manual mode*/

                        onClicked: {
                            nilecam20usb.setROIAutoExposure(NileCam20_USB.CENTERED, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setROIAutoExposure(NileCam20_USB.CENTERED, 0, 0, 0, 0, 0);
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
                            nilecam20usb.setROIAutoExposure(NileCam20_USB.AUTO_EXP_MANNUAL, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = true
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setROIAutoExposure(NileCam20_USB.AUTO_EXP_MANNUAL, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                            nilecam20usb.setROIAutoExposure(NileCam20_USB.AUTO_EXP_MANNUAL, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }


                Text {
                    id: exposureCompTextTitle
                    text: "--- Exposure Compensation ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing:35
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: exposureCompSlider
                        opacity: enabled ? 1 : 0.1
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: expCompMinValue
                        maximumValue: expCompMaxValue
                        onValueChanged:  {
                            exposureCompTextValue.text = exposureCompSlider.value
                            if(skipUpdateUIOnExpComp){
                                nilecam20usb.setExposureCompensation(exposureCompTextValue.text)
                            }
                            skipUpdateUIOnExpComp = true
                        }
                    }
                    TextField {
                        id: exposureCompTextValue
                        text: exposureCompSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        enabled: false
                        opacity: exposureCompSlider.enabled ? 1 : 0.1
                        style: econTextFieldStyle
                        onTextChanged: {
                        }
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
                                nilecam20usb.setColourKill(colourKillValSlider.value)
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
                            nilecam20usb.setBurstLength(burstLengthCombo.currentText)
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
                            nilecam20usb.setDenoiseCtrlMode(NileCam20_USB.DenoiseEnable)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setDenoiseCtrlMode(NileCam20_USB.DenoiseEnable)
                        }
                    }
                    RadioButton {
                        id: denoiseDisable
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            nilecam20usb.setDenoiseCtrlMode(NileCam20_USB.DenoiseDisable)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setDenoiseCtrlMode(NileCam20_USB.DenoiseDisable)
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
                Row{
                    spacing: 90
                    ExclusiveGroup { id: lscModeControlGroup }
                    RadioButton {
                        id: lscModeAuto
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: lscModeControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            nilecam20usb.setLSCMode(NileCam20_USB.LSCModesAuto)
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setLSCMode(NileCam20_USB.LSCModesAuto)
                        }
                    }
                    RadioButton {
                        id: lscModeManual
                        style:  econRadioButtonStyle
                        text: qsTr("Manual")
                        exclusiveGroup: lscModeControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            setLSCModesBasedOnComboIndexSelection()
                        }
                        Keys.onReturnPressed: {
                            setLSCModesBasedOnComboIndexSelection()
                        }
                    }
                }
                Text {
                    id: lscModeManualModeText
                    text: "Light Source:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (lscModeManual.enabled && lscModeManual.checked) ? 1 : 0.1
                }
                ComboBox
                {
                    id: lscModeCombo
                    enabled: (lscModeManual.enabled && lscModeManual.checked) ? true : false
                    opacity: (lscModeManual.enabled && lscModeManual.checked) ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "DayLight" }
                        ListElement { text: "CWF Light" }
                        ListElement { text: "A Light" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnLSCMode){
                            setLSCModesBasedOnComboIndexSelection()
                        }
                        skipUpdateUIOnLSCMode = true
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
                            nilecam20usb.setAntiFlickerMode(NileCam20_USB.FlickerAuto);
                        }
                        Keys.onReturnPressed: {
                            nilecam20usb.setAntiFlickerMode(NileCam20_USB.FlickerAuto);
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
                Row{
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

    NileCam20_USB {
        id: nilecam20usb
        onSensorModeReceived:{
            if(sensorModes == NileCam20_USB.SENSOR_STANDARD){
                sensorStandard.checked = true
                exposureCompSlider.enabled = true
                exposureCompSlider.opacity = 1
                exposureCompTextValue.opacity = 1
            }else if(sensorModes == NileCam20_USB.SENSOR_HDR){
                sensorHdrDlo.checked = true
                exposureCompSlider.enabled = false
                exposureCompSlider.opacity = 0.1
                exposureCompTextValue.opacity = 0.1
            }
        }

        onCameraModeReceived:{
            if(cameraModes == NileCam20_USB.CAMERA_MASTER){
                cameraModeMaster.checked = true
                root.checkForTriggerMode(false)
                root.videoRecordBtnEnable(true)
                root.captureBtnEnable(true)
                root.startUpdatePreviewInMasterMode()
             }else if(cameraModes == NileCam20_USB.CAMERA_SLAVE){
                cameraModeSlave.checked = true
                root.checkForTriggerMode(true)
                root.captureBtnEnable(false)
                root.videoRecordBtnEnable(false)
                root.stopUpdatePreviewInTriggerMode()
            }
        }
        onRoiAutoExpModeValue:{
            if(roiMode == NileCam20_USB.CENTERED){
                autoexpCentered.checked = true
            }else if(roiMode == NileCam20_USB.AUTO_EXP_MANNUAL){
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize - 1
                skipUpdateUIOnExpWindowSize = true
            }
            else if(roiMode == NileCam20_USB.AUTO_EXP_DISABLED){
                autoexpCentered.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
            }
        }
        onSpecialEffectMode:{
            if(specialModes == NileCam20_USB.SPECIAL_NORMAL){
                specialModeNormal.checked = true
            }else if(specialModes == NileCam20_USB.SPECIAL_GREYSCALE){
                specialModeGreyscale.checked = true
            }
        }
        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onStrobeModeChanged:{
            currentStrobeMode(strobeValues)
        }
        onColorKillValueChanged:{
            skipUpdateUIOnColorKillVal = false
            colourKillValSlider.value = ColorKillValue
            skipUpdateUIOnColorKillVal = true
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onFlickerDetectionMode:{
            currentAntiFlickerMode(flickerMode)
        }
        onLscModeChanged:{
            currentLSCMode(lscMode)
        }
        onSendDenoiseValue:{
            if(denoiseValue == NileCam20_USB.DenoiseEnable){
                denoiseEnable.checked = true
            }else if(denoiseValue == NileCam20_USB.DenoiseDisable){
                denoiseDisable.checked = true
            }
        }
        onExposureCompValueReceived:{
            skipUpdateUIOnExpComp = false
            exposureCompSlider.value = exposureCompensation
            skipUpdateUIOnExpComp = true
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function sensorStandardModeSelected(){
        nilecam20usb.setSensorMode(NileCam20_USB.SENSOR_STANDARD)
        defaultValue.enabled = true
        if(autoExposureMode){
            exposureCompSlider.enabled = true
            exposureCompSlider.opacity = 1
            exposureCompTextValue.opacity = 1
        }else{
            exposureCompSlider.enabled = false
            exposureCompSlider.opacity = 0.1
            exposureCompTextValue.opacity = 0.1
        }
    }

    function sensorHDRModeSelected(){
        nilecam20usb.setSensorMode(NileCam20_USB.SENSOR_HDR)
        defaultValue.enabled = true
        exposureCompSlider.enabled = false
        exposureCompSlider.opacity = 0.1
        exposureCompTextValue.opacity = 0.1
    }

    function currentLSCMode(Mode){
        skipUpdateUIOnLSCMode = false
        switch(Mode){
        case NileCam20_USB.LSCModesAuto:
            lscModeAuto.checked = true
            break;
        case NileCam20_USB.DayLight:
            lscModeManual.checked = true
            lscModeCombo.currentIndex  = 0
            break;
        case NileCam20_USB.CWFLight:
            lscModeManual.checked = true
            lscModeCombo.currentIndex  = 1
            break;
        case NileCam20_USB.ALight:
            lscModeManual.checked = true
            lscModeCombo.currentIndex  = 2
            break;
        }
        skipUpdateUIOnLSCMode = true
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function setLSCModesBasedOnComboIndexSelection(){
        if(lscModeCombo.currentIndex == 0)
        {
            nilecam20usb.setLSCMode(NileCam20_USB.DayLight)
        }else if(lscModeCombo.currentIndex == 1)
        {
            nilecam20usb.setLSCMode(NileCam20_USB.CWFLight)
        }else if(lscModeCombo.currentIndex == 2)
        {
            nilecam20usb.setLSCMode(NileCam20_USB.ALight)
        }
    }

    function currentAntiFlickerMode(flickerMode){
        switch(flickerMode){
        case NileCam20_USB.FlickerAuto:
            antiFlickerModeAuto.checked = true
            break
        case NileCam20_USB.Manual50Hz:
            antiFlickerModeManual.checked = true
            skipUpdateUIOnAntiFlickerMode = false
            antiFlickerCombo.currentIndex = 0
            skipUpdateUIOnAntiFlickerMode = true
            break
        case NileCam20_USB.Manual60Hz:
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
            nilecam20usb.setAntiFlickerMode(NileCam20_USB.Manual50Hz)
        else
            nilecam20usb.setAntiFlickerMode(NileCam20_USB.Manual60Hz)
    }

    // selecting strobe mode
    function currentStrobeMode(mode){
        switch(mode){
        case NileCam20_USB.OFF:
            strobeOff.checked = true
            break;
        case NileCam20_USB.FLASH_FOR_VIDEO_STREAMING:
            strobeFlashVidoStreaming.checked = true
            break;
        case NileCam20_USB.TORCH:
            strobeTorch.checked = true
            break;
        case NileCam20_USB.FLASH_FOR_EXT_STILL_TRIGGER:
            strobeStillTrigger.checked = true
            break;
        }
    }

    // Flip mirror modes
    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case NileCam20_USB.Normal:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        case NileCam20_USB.VerticalFlip:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case NileCam20_USB.HorizontalMirror:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case NileCam20_USB.Rotate180:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        }
    }

    function setMasterMode(){
        nilecam20usb.setCameraMode(NileCam20_USB.CAMERA_MASTER)
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
    }

    function setTriggerMode(){
        defaultValue.enabled = true
        nilecam20usb.setCameraMode(NileCam20_USB.CAMERA_SLAVE)
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
        defaultValue.enabled = false
        if(nilecam20usb.setToDefaultValues()){
            nilecam20usb.getROIAutoExposure()
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        nilecam20usb.getOrientation()
        nilecam20usb.getSensorMode()
        nilecam20usb.getCameraMode()
        nilecam20usb.getSpecialMode()
        nilecam20usb.getROIAutoExposure()
        nilecam20usb.getStrobeMode()
        nilecam20usb.getColourKill()
        nilecam20usb.getExposureCompensation()
        nilecam20usb.getBurstLength()
        nilecam20usb.getAntiFlickerMode()
        nilecam20usb.getDenoiseCtrlMode()
        nilecam20usb.getLSCMode()
    }

    Component.onCompleted:{
        autoExpROITimer.start()
        lscModeCombo.currentIndex=0
        getCurrentValuesFromCamera()
    }
}



