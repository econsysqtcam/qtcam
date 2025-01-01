/*
 * ecam512usb.qml -- extension settings for e-CAM512_USB camera
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

import econ.camera.ecam_512usb 1.0
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

    property int denoiseMin: 0
    property int denoiseMax: 15

    property int qFactorMin: 0
    property int qFactorMax: 96

    property int expoCompMin: 8000
    property int expoCompMax: 1000000

    property int minFrameRate: 0
    property int maxFrameRate: 60

    property bool setButtonClicked: false
    property bool skipUpdateUIQFactor: false
    property bool skipUpdateUIDenoise: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIOnAntiFlickerMode:false
    property bool skipUpdateUIOnExpWindowSize   : false
    property bool skipUpdateUIFrameRate: false

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
    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 1000
        onTriggered: {
            ecam512usb.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Timer {
        id: getCamValuesTimer
        interval: 500
        onTriggered: {
            ecam512usb.getExposureCompensation()
            ecam512usb.getFrameRateCtrlValue()
            stop()
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
        onSetExpCompensation:{//To set the exposure while cross resolution still capture
            ecam512usb.setExposureCompensation(exposureCompValue.text)
        }
        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        onVideoResolutionChanged:{
            getCamValuesTimer.start()
        }
        onPreviewFPSChanged:{
            getCamValuesTimer.start()
        }
        onVideoColorSpaceChanged:{
            getCamValuesTimer.start()
        }
        onMouseRightClicked:{
            if(autoexpManual.enabled && autoexpManual.checked){
               ecam512usb.setROIAutoExposure(ECAM_512USB.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
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
            height:1600

            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text
                {
                     id: special_effects
                     text: "--- Special Effects ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     Layout.alignment: Qt.AlignCenter
                     opacity: 0.50196078431373
                }
                Grid
                {
                    columns: 2
                    spacing: 20

                    ExclusiveGroup { id: specialEffectsGroup }
                    RadioButton
                    {
                        id: rdoEffectNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: specialEffectsGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_NORMAL)
                        }
                        Keys.onReturnPressed:  {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_NORMAL)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectBW
                        style:  econRadioButtonStyle
                        text: qsTr("Black and White")
                        exclusiveGroup: specialEffectsGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_BLACK_WHITE)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_BLACK_WHITE)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectGreyScale
                        style:  econRadioButtonStyle
                        text: qsTr("GreyScale")
                        exclusiveGroup: specialEffectsGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_GREYSCALE)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectSketch
                        style:  econRadioButtonStyle
                        text: qsTr("Sketch")
                        exclusiveGroup: specialEffectsGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_SKETCH)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_SKETCH)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectNegative
                        style:  econRadioButtonStyle
                        text: qsTr("Negative")
                        exclusiveGroup: specialEffectsGroup
                        activeFocusOnPress: true
                        onClicked:
                        {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_NEGATIVE)
                        }
                        Keys.onReturnPressed:
                        {
                            ecam512usb.setSpecialMode(ECAM_512USB.SPECIAL_NEGATIVE)
                        }
                    }
                }

                Text
                {
                    id: denoiseText
                    text: "--- De-Noise ---"
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
                        id: deNoiseSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: denoiseMin
                        maximumValue: denoiseMax
                        onValueChanged:
                        {
                            deNoiseTextField.text = deNoiseSlider.value
                            if(skipUpdateUIDenoise)
                            {
                                ecam512usb.setDenoiseValue(deNoiseSlider.value)
                            }
                            skipUpdateUIDenoise = true
                        }
                    }
                    TextField
                    {
                        id: deNoiseTextField
                        text: deNoiseSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: deNoiseSlider.minimumValue; top: deNoiseSlider.maximumValue}
                        onTextChanged:
                        {
                            if(text.length > 0)
                            {
                                deNoiseSlider.value = deNoiseTextField.text
                            }
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

                ColumnLayout{
                    spacing:25
                    ExclusiveGroup { id: roiExpogroup }
                    RadioButton {
                        exclusiveGroup: roiExpogroup
                        id: autoexpFull
                        text: "Full"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                        // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                        // winSize is required only for manual mode
                        onClicked: {
                            ecam512usb.setROIAutoExposure(ECAM_512USB.AutoExpFull, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = false
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setROIAutoExposure(ECAM_512USB.AutoExpFull, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                            ecam512usb.setROIAutoExposure(ECAM_512USB.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = true
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setROIAutoExposure(ECAM_512USB.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = true
                        }
                    }
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
                            ecam512usb.setROIAutoExposure(ECAM_512USB.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }

                Text
                {
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
                    model: ListModel
                           {
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
                            ecam512usb.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }

                Text
                {
                    id: qFactorSliderTitle
                    text: "--- QFactor Slider ---"
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
                        id: qFactorSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: qFactorMin
                        maximumValue: qFactorMax
                        onValueChanged:
                        {
                            qFactorTextField.text = qFactorSlider.value
                            if(skipUpdateUIQFactor)
                            {
                                ecam512usb.setQFactorValue(qFactorSlider.value)
                            }
                            skipUpdateUIQFactor = true
                        }
                    }
                    TextField
                    {
                        id: qFactorTextField
                        text: qFactorSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: qFactorSlider.minimumValue; top: qFactorSlider.maximumValue}
                        onTextChanged:
                        {
                            if(text.length > 0)
                            {
                                qFactorSlider.value = qFactorTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: flipText
                    text: "--- Flip Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row
                {
                    spacing: 40
                    CheckBox
                    {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        onClicked:{
                            ecam512usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox
                    {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            ecam512usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                }

                Text {
                    id: faceDetectionText
                    text: "--- Face Detection ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 62
                    ExclusiveGroup { id: faceRectGroup }
                    RadioButton {
                        exclusiveGroup: faceRectGroup
                        id: faceRectEnable
                        text: "Enable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            ecam512usb.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: faceRectGroup
                        id:faceRectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            ecam512usb.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                    }
                }
                Row{
                    spacing: 5
                    CheckBox {
                        id: faceDetectEmbedData
                        activeFocusOnPress : true
                        text: "Embed \nData"
                        style: econCheckBoxTextWrapModeStyle
                        enabled: faceRectEnable.checked ? true : false
                        opacity: enabled ? 1 : 0.1
                        onClicked:{
                            enableFaceDetectEmbedData()
                        }
                        Keys.onReturnPressed: {
                            enableFaceDetectEmbedData()
                        }
                    }
                    CheckBox {
                        id: overlayRect
                        activeFocusOnPress : true
                        text: "Overlay Rectangle"
                        style: econCheckBoxTextWrapModeStyle
                        enabled: faceRectEnable.checked ? true : false
                        opacity: enabled ? 1 : 0.1
                        onClicked:{
                            ecam512usb.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        }
                    }
                }


                Text {
                    id: smileDetectionText
                    text: "--- Smile Detection ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 62
                    ExclusiveGroup { id: smileDetectGroup }
                    RadioButton {
                        exclusiveGroup: smileDetectGroup
                        id: smileDetectEnable
                        text: "Enable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            ecam512usb.setSmileDetection(true, smileDetectEmbedData.checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setSmileDetection(true, smileDetectEmbedData.checked)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: smileDetectGroup
                        id:smileDetectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            ecam512usb.setSmileDetection(false, smileDetectEmbedData.checked)
                        }
                        Keys.onReturnPressed: {
                            ecam512usb.setSmileDetection(false, smileDetectEmbedData.checked)
                        }
                    }
                }
                Row{
                    spacing: 5
                    CheckBox {
                        id: smileDetectEmbedData
                        activeFocusOnPress : true
                        text: "Embed Data"
                        style: econCheckBoxStyle
                        enabled: smileDetectEnable.checked ? true : false
                        opacity: enabled ? 1 : 0.1
                        onClicked:{
                            enableSmileDetectEmbedData()
                        }
                        Keys.onReturnPressed: {
                            enableSmileDetectEmbedData()
                        }
                    }
                }

                Row
                {
                    spacing: 9

                    Text
                    {
                        id: exposureCompText
                        text: "value(µs)[8000 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        opacity: 1
                    }
                    TextField
                    {
                        id: exposureCompValue
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        opacity: 1
                        style: econTextFieldStyle
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: expoCompMin; top: expoCompMax}
                    }
                    Button
                    {
                        id: exposureCompSet
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: true
                        opacity: 1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            exposureCompSet.enabled = false
                            setButtonClicked = true
                            ecam512usb.setExposureCompensation(exposureCompValue.text)
                            exposureCompSet.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            exposureCompSet.enabled = false
                            setButtonClicked = true
                            see3camcu83.setExposureCompensation(exposureCompValue.text)
                            exposureCompSet.enabled = true
                        }
                    }

                }

                Text{
                    id: flashModeText
                    x: 85
                    y: 200
                    text: "--- Flash Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                RowLayout{
                    x:25
                    y:230
                    spacing: 25
                    ExclusiveGroup { id: flashGrp }

                    Column{
                        RadioButton {
                            exclusiveGroup: flashGrp
                            checked: false
                            id: flashModeOff
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                ecam512usb.setFlashMode(ECAM_512USB.FLASH_DISABLE)
                            }
                            Keys.onReturnPressed: {
                                ecam512usb.setFlashMode(ECAM_512USB.FLASH_DISABLE)
                            }
                        }
                    }
                    Column{
                        RadioButton {
                            exclusiveGroup: flashGrp
                            checked: false
                            id: flashModeStrobe
                            text: "Strobe"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                ecam512usb.setFlashMode(ECAM_512USB.FLASH_STROBE)
                            }
                            Keys.onReturnPressed: {
                                ecam512usb.setFlashMode(ECAM_512USB.FLASH_STROBE)
                            }
                        }
                    }
                    Column{
                        RadioButton {
                            exclusiveGroup: flashGrp
                            checked: false
                            id: flashModeTorch
                            text: "Torch"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                ecam512usb.setFlashMode(ECAM_512USB.FLASH_TORCH)
                            }
                            Keys.onReturnPressed: {
                                ecam512usb.setFlashMode(ECAM_512USB.FLASH_TORCH)
                            }
                        }
                    }
                }

                Text
                {
                    id: frameRateText
                    text: "--- Frame Rate Control ---"
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
                        id: frameRateSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: minFrameRate
                        maximumValue: maxFrameRate
                        onValueChanged:  {
                            frameRateTextField.text = frameRateSlider.value
                            if(skipUpdateUIFrameRate){
                                ecam512usb.setFrameRateCtrlValue(frameRateSlider.value)
                            }
                            skipUpdateUIFrameRate = true
                        }
                    }
                    TextField {
                        id: frameRateTextField
                        text: frameRateSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: frameRateSlider.minimumValue; top: frameRateSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                frameRateSlider.value = frameRateTextField.text
                            }
                        }
                    }
                }


                Text
                {
                    id: antiFlickerMode
                    text: "--- Anti Flicker Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row
                {
                      spacing:90
                      ExclusiveGroup { id: antiFlickerModegroup }

                      RadioButton
                      {
                          exclusiveGroup: antiFlickerModegroup
                          id: antiFlickerModeAuto
                          text: "Auto"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          onClicked: {
                              ecam512usb.setAntiFlickerMode(ECAM_512USB.MODE_AUTO);
                          }
                          Keys.onReturnPressed: {
                              ecam512usb.setAntiFlickerMode(ECAM_512USB.MODE_AUTO);
                          }
                      }
                      RadioButton
                      {
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

                Text
                {
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
                    model: ListModel
                           {
                                ListElement { text: "50 Hz" }
                                ListElement { text: "60 Hz" }
                                ListElement { text: "DISABLE" }
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
                        id: storeSettings
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Store Settings"
                        tooltip: "Click to store the settings in extension controls"
                        style: econButtonStyle
                        onClicked: {
                            setButtonClicked = true
                            ecam512usb.storeSettings()
                            setButtonClicked = false
                        }
                        Keys.onReturnPressed: {
                            setButtonClicked = true
                            ecam512usb.storeSettings()
                            setButtonClicked = false
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
                       onClicked: {
                           setButtonClicked = true
                           setToDefaultValues()
                           setButtonClicked = false
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           setToDefaultValues()
                           setButtonClicked = false
                       }
                   }
               }


            }
        }
    }

   ECAM_512USB{
     id:ecam512usb

     onSpecialModeReceived:
     {
         currentSpecialEffectsStatus(specialMode)
     }
     onBurstLengthValueRecieved:
     {
         skipUpdateUIOnBurstLength = false
         burstLengthCombo.currentIndex = burstLenVal - 1
         skipUpdateUIOnBurstLength = true
     }
     onQFactorValueReceived: {
         skipUpdateUIQFactor = false
         qFactorSlider.value = Qfactor
         skipUpdateUIQFactor = true
     }
     onFlipMirrorModeReceived:
     {
         currentFlipMirrorModeStatus(flipMode)
     }
     onFaceDetectModeValue:{
         currentFaceDetectionStatus(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect)
     }
     onSmileDetectModeValue:{
         currentSmileDetectionStatus(smileDetectMode, smileDetectEmbedDataValue)
     }
     onDenoiseValueReceived:
     {
         skipUpdateUIDenoise = false
         deNoiseSlider.value = denoiseValue
         skipUpdateUIDenoise = true
     }
     onRoiAutoExpMode:{

         currentROIAutoExposureMode(roiMode, winSize)
     }
     onExposureCompValueRecieved:
     {
          exposureCompValue.text = exposureCompVal
     }
     onFlashModeReceived:{
         currentFlashMode(flashMode)
     }
     onFrameRateCtrlValueRecieved:
     {
         skipUpdateUIFrameRate = false
         frameRateSlider.value = frameRateVal
         skipUpdateUIFrameRate = true
     }
     onAntiFlickerModeRecieved:
     {
         currentAntiFlickerModeStatus(antiFlickerVal)
     }

     //Signal for command Prompt
     onIndicateCommandStatus:{
         if(setButtonClicked){
             displayMessageBox(title, text)
             setButtonClicked = false
         }
     }

     onIndicateExposureValueRangeFailure:{
         if(setButtonClicked){
             displayMessageBox(title, text)
             setButtonClicked = false
             ecam512usb.getExposureCompensation()
             ecam512usb.getFrameRateCtrlValue()
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
        id: econCheckBoxTextWrapModeStyle
        CheckBoxStyle {
            label: Text {
                text: control.text
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                opacity: 1
                width: 100
                wrapMode: Text.WordWrap
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

    function currentSpecialEffectsStatus(specialMode)
    {
        switch(specialMode)
        {
            case ECAM_512USB.SPECIAL_NORMAL:
                rdoEffectNormal.checked = true
                break
            case ECAM_512USB.SPECIAL_BLACK_WHITE:
                rdoEffectBW.checked = true
                break
            case ECAM_512USB.SPECIAL_GREYSCALE:
                rdoEffectGreyScale.checked = true
                break
            case ECAM_512USB.SPECIAL_NEGATIVE:
                rdoEffectNegative.checked = true
                break
            case ECAM_512USB.SPECIAL_SKETCH:
                rdoEffectSketch.checked = true
                break
        }
    }

    function currentROIAutoExposureMode(roiMode, winSize){

        switch(roiMode){
        case ECAM_512USB.AutoExpFull:
            autoexpFull.checked = true
            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.currentIndex = winSize-1
            break
        case ECAM_512USB.AutoExpManual:
            skipUpdateUIOnExpWindowSize = false
            autoexpManual.checked = true
            // If window size is got from camera is 0 then set window size to 1 in UI
            if(winSize == 0){
                autoExpoWinSizeCombo.currentIndex = 0
            }else
                autoExpoWinSizeCombo.currentIndex = winSize-1
            break
        }
    }

    function currentFlipMirrorModeStatus(mode)
    {
        switch(mode)
        {
        case ECAM_512USB.VERTICAL:
            flipCtrlVertical.checked  = true
            flipCtrlHorizotal.checked = false
            break;
        case ECAM_512USB.HORIZONTAL:
            flipCtrlVertical.checked  = false
            flipCtrlHorizotal.checked = true
            break;
        case ECAM_512USB.ROATATE_180:
            flipCtrlVertical.checked  = true
            flipCtrlHorizotal.checked = true
            break;
        case ECAM_512USB.NORMAL:
            flipCtrlVertical.checked  = false
            flipCtrlHorizotal.checked = false
            break;
        }
    }

    function currentFaceDetectionStatus(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect)
    {
        if(faceDetectMode == ECAM_512USB.FaceRectEnable){
            faceRectEnable.checked = true
            if(faceDetectEmbedDataValue == ECAM_512USB.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }
            if(faceDetectOverlayRect == ECAM_512USB.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }
        }else if(faceDetectMode == ECAM_512USB.FaceRectDisable){
            faceRectDisable.checked = true
            if(faceDetectEmbedDataValue == ECAM_512USB.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }else{
                faceDetectEmbedData.checked = false
            }
            if(faceDetectOverlayRect == ECAM_512USB.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }else{
                overlayRect.checked = false
            }
        }
    }

    function currentSmileDetectionStatus(smileDetectMode, smileDetectEmbedDataValue)
    {
        if(smileDetectMode == ECAM_512USB.SmileDetectEnable){
            smileDetectEnable.checked = true
            if(smileDetectEmbedDataValue == ECAM_512USB.SmileDetectEmbedDataEnable){
                smileDetectEmbedData.checked = true
            }
        }else if(smileDetectMode == ECAM_512USB.SmileDetectDisable){
            smileDetectDisable.checked = true
            if(smileDetectEmbedDataValue == ECAM_512USB.SmileDetectEmbedDataEnable){
                smileDetectEmbedData.checked = true
            }else{
                smileDetectEmbedData.checked = false
            }
        }
    }

    function currentFlashMode(mode){
        switch(mode){
        case ECAM_512USB.FLASH_TORCH:
            flashModeTorch.checked = true
            break;
        case ECAM_512USB.FLASH_STROBE:
            flashModeStrobe.checked = true
            break;
        case ECAM_512USB.FLASH_DISABLE:
            flashModeOff.checked = true
            break;
        }
    }

    function currentAntiFlickerModeStatus(antiFlickerVal)
    {
        switch(antiFlickerVal)
        {
            case ECAM_512USB.MODE_AUTO:
                antiFlickerModeAuto.checked = true
                break
            case ECAM_512USB.MODE_50Hz:
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case ECAM_512USB.MODE_60Hz:
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
            case ECAM_512USB.MODE_DISABLE://need to check the flags
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 2
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }

    // Enable Face detect embed data
    function enableFaceDetectEmbedData(){
        if(ecam512usb.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by face data.Refer document FSCAM_CU135_Face_and_Smile_Detection_Application_Note for more details")
                messageDialog.open()
            }
        }
    }

    // enable smile detect embed data
    function enableSmileDetectEmbedData(){
        if(ecam512usb.setSmileDetection(true, smileDetectEmbedData.checked)){
            if(smileDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document FSCAM_CU135_Face_and_Smile_Detection_Application_Note for more details")
                messageDialog.open()
            }
        }
    }

    function setAntiFlickerMode()
    {
        if(antiFlickerCombo.currentIndex === 0)
            ecam512usb.setAntiFlickerMode(ECAM_512USB.MODE_50Hz)
        else if(antiFlickerCombo.currentIndex === 1)
            ecam512usb.setAntiFlickerMode(ECAM_512USB.MODE_60Hz)
        else if(antiFlickerCombo.currentIndex === 2)
            ecam512usb.setAntiFlickerMode(ECAM_512USB.MODE_DISABLE)
    }

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        ecam512usb.setToDefaultValues()
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

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        getAutoExpsoureControlValues.start()
    }
    function getValuesFromCamera(){
        ecam512usb.getSpecialMode()
        ecam512usb.getDenoiseValue()
        ecam512usb.getBurstLength()
        ecam512usb.getQFactorValue()
        ecam512usb.getOrientation()
        ecam512usb.getAutoExpROIModeAndWindowSize()
        ecam512usb.getFaceDetectMode()
        ecam512usb.getSmileDetectMode()
        getCamValuesTimer.start()
        ecam512usb.getFlashMode()
        ecam512usb.getAntiFlickerMode()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
