/*
 * uvc_cu30.qml -- extension settings for other cameras
 * Copyright © 2015  e-con Systems India Pvt. Limited
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
import econ.camera.see3camcu30 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:240
    height:720
    property int defaultNoiseVal: 8
    property int denoiseMin: 0
    property int denoiseMax: 15
    property int qFactorMin: 10
    property int qFactorMax: 96
    property int frameRateMin: 1
    property int frameRateMax: 120
    property int expoCompMin: 8000
    property int expoCompMax: 1000000
    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIFrameRate: false
    property bool setButtonClicked: false

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            see3camcu30.enableDisableFaceRectangle(false)
            burstShotTimer.start()
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
    }

    Timer {
         id: burstShotTimer
         interval: 1000
         onTriggered: {
             root.imageCapture(CommonEnums.BURST_SHOT);
             stop()
         }
     }

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            see3camcu30.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            see3camcu30.getExposureCompensation()
            see3camcu30.getFrameRateCtrlValue()
            stop()
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

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle
        ColumnLayout{
            x:2
            y:5
            spacing:20

            Text {
            id: scene_mode
            text: "--- Scene Mode ---"
            font.pixelSize: 14
            font.family: "Ubuntu"
            color: "#ffffff"
            smooth: true
            Layout.alignment: Qt.AlignCenter
            opacity: 0.50196078431373
            }


            Grid {
                columns: 2
                spacing: 50

                ExclusiveGroup { id: sceneInputGroup }
                RadioButton {
                    id: sceneNormal
                    style:  econRadioButtonStyle
                    text:   qsTr("Normal")
                    exclusiveGroup: sceneInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu30.setSceneMode(See3Camcu30.SCENE_NORMAL)
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setSceneMode(See3Camcu30.SCENE_NORMAL)
                    }
                }
                RadioButton {
                    id: sceneDoc
                    style:  econRadioButtonStyle
                    text: qsTr("Document")
                    exclusiveGroup: sceneInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        defaultValue.enabled = true
                        see3camcu30.setSceneMode(See3Camcu30.SCENE_DOCUMENT)
                    }
                    Keys.onReturnPressed: {
                        defaultValue.enabled = true
                        see3camcu30.setSceneMode(See3Camcu30.SCENE_DOCUMENT)
                    }

                }

            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                   id: special_effects
                   text: "--- Special Effects ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   opacity: 0.50196078431373
               }
            }
            Grid {
                x: 23
                y: 235

                columns: 2
                spacing: 15

                ExclusiveGroup { id: effectInputGroup }
                RadioButton {
                    id: rdoEffectNormal
                    style:  econRadioButtonStyle
                    text:   qsTr("Normal")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_NORMAL)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_NORMAL)
                    }
                }
                RadioButton {
                    id: rdoEffectBW
                    style:  econRadioButtonStyle
                    text: qsTr("Black and White")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_BLACK_WHITE)
                    }
                    Keys.onReturnPressed: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_BLACK_WHITE)
                    }
                }
                RadioButton {
                    id: rdoEffectGreyScale
                    style:  econRadioButtonStyle
                    text: qsTr("GreyScale")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_GREYSCALE)
                    }
                    Keys.onReturnPressed: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_GREYSCALE)
                    }
                }
                RadioButton {
                    id: rdoEffectSketch
                    style:  econRadioButtonStyle
                    text: qsTr("Sketch")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_SKETCH)
                    }
                    Keys.onReturnPressed: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_SKETCH)
                    }
                }
                RadioButton {
                    id: rdoEffectNegative
                    style:  econRadioButtonStyle
                    text: qsTr("Negative")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_NEGATIVE)
                    }
                    Keys.onReturnPressed: {
                        defaultValue.enabled = true
                        see3camcu30.setEffectMode(See3Camcu30.EFFECT_NEGATIVE)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
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
            }

            Row{
                  spacing:90
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
                          see3camcu30.setROIAutoExposure(See3Camcu30.AutoExpFull, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = false
                      }
                      Keys.onReturnPressed: {
                          see3camcu30.setROIAutoExposure(See3Camcu30.AutoExpFull, 0, 0, 0, 0, 0);
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
                          defaultValue.enabled = true
                          see3camcu30.setROIAutoExposure(See3Camcu30.AutoExpManual, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = true
                      }
                      Keys.onReturnPressed: {
                          defaultValue.enabled = true
                          see3camcu30.setROIAutoExposure(See3Camcu30.AutoExpManual, 0, 0, 0, 0, 0);
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
                    defaultValue.enabled = true
                    if(skipUpdateUIOnExpWindowSize){
                        see3camcu30.setROIAutoExposure(See3Camcu30.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
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
                    spacing: 9
                    Text {
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
                    TextField {
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
                    Button {
                        id: exposureCompSet
                        activeFocusOnPress : true
                        text: "Set"
                        style: econButtonStyle
                        enabled: true
                        opacity: 1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked: {
                            exposureCompSetButtonClicked()
                        }
                        Keys.onReturnPressed: {
                            exposureCompSetButtonClicked()
                        }
                    }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: denoiseText
                    text: "--- De-Noise Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
            }

            Row{
                spacing: 30
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: deNoiseSlider
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: denoiseMin
                    maximumValue: denoiseMax
                    onValueChanged:  {
                        deNoiseTextField.text = deNoiseSlider.value
                        see3camcu30.setDenoiseValue(deNoiseSlider.value)
                        defaultValue.enabled = true
                    }
                }
                TextField {
                    id: deNoiseTextField
                    text: deNoiseSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: deNoiseSlider.minimumValue; top: deNoiseSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            deNoiseSlider.value = deNoiseTextField.text
                            defaultValue.enabled = true
                        }
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
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
                        see3camcu30.setBurstLength(burstLengthCombo.currentText)
                    }
                    skipUpdateUIOnBurstLength = true
                    defaultValue.enabled = true
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: qFactorText
                    text: "--- Q Factor ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
            }

            Row{
                spacing: 35
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: qFactorSlider
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: qFactorMin
                    maximumValue: qFactorMax
                    onValueChanged:  {
                        qFactorTextField.text = qFactorSlider.value
                        defaultValue.enabled = true
                        if(skipUpdateUIQFactor){
                            see3camcu30.setQFactor(qFactorSlider.value)
                        }
                        skipUpdateUIQFactor = true
                    }
                }
                TextField {
                    id: qFactorTextField
                    text: qFactorSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: qFactorSlider.minimumValue; top: qFactorSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            qFactorSlider.value = qFactorTextField.text
                            defaultValue.enabled = true
                        }
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: frameRateText
                    text: "--- Frame Rate Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
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
                    minimumValue: frameRateMin
                    maximumValue: frameRateMax
                    onValueChanged:  {
                        frameRateTextField.text = frameRateSlider.value
                        if(skipUpdateUIFrameRate){
                            see3camcu30.setFrameRateCtrlValue(frameRateSlider.value)
                        }
                        skipUpdateUIFrameRate = true
                        defaultValue.enabled = true
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
                            defaultValue.enabled = true
                        }
                    }
                }
            }

            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: flipText
                    text: "--- Flip Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
            }
            Row{
                spacing: 55
                CheckBox {
                    id: flipCtrlHorizotal
                    activeFocusOnPress : true
                    text: "Horizontal"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3camcu30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        defaultValue.enabled = true
                    }
                }
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3camcu30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        defaultValue.enabled = true
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
                        see3camcu30.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        defaultValue.enabled = true
                    }
                }
                RadioButton {
                    exclusiveGroup: faceRectGroup
                    id:faceRectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        see3camcu30.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        defaultValue.enabled = true
                    }
                }
            }
            Row{
                spacing: 5
                CheckBox {
                    id: faceDetectEmbedData
                    activeFocusOnPress : true
                    text: "Embed Data"
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
                        see3camcu30.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        defaultValue.enabled = true
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
                        see3camcu30.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                        defaultValue.enabled = true
                    }
                }
                RadioButton {
                    exclusiveGroup: smileDetectGroup
                    id:smileDetectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        see3camcu30.setSmileDetection(false, smileDetectEmbedData.checked, smileThreshold.text)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu30.setSmileDetection(false, smileDetectEmbedData.checked, smileThreshold.text)
                        defaultValue.enabled = true
                    }
                }
            }
            Row{
                spacing: 14
                Text {
                    id: smileThresholdText
                    text: "Threshold value[40-75]"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    width: 80
                    wrapMode: Text.WordWrap
                    opacity: (smileDetectEnable.enabled && smileDetectEnable.checked) ? 1 : 0.1
                }
                TextField {
                    id: smileThreshold
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: (smileDetectEnable.enabled && smileDetectEnable.checked) ? true : false
                    opacity: (smileDetectEnable.enabled && smileDetectEnable.checked) ? 1 : 0.1
                    style: econTextFieldStyle
                    implicitHeight: 25
                    implicitWidth: 70
                    validator: IntValidator {bottom: 40; top: 75}
                }
                Button {
                    id: smileThresholdSet
                    activeFocusOnPress : true
                    text: "Set"
                    style: econButtonStyle
                    enabled: (smileDetectEnable.enabled && smileDetectEnable.checked) ? true : false
                    opacity: (smileDetectEnable.enabled && smileDetectEnable.checked) ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 60
                    onClicked: {
                        smileThresholdSetButtonClicked()
                    }
                    Keys.onReturnPressed: {
                        smileThresholdSetButtonClicked()
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
            Row{
                Layout.alignment: Qt.AlignCenter
                Button {
                    id: defaultValue
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Default"
                    tooltip: "Click to set default values"
                    style: econButtonStyle
                    onClicked: {
                        setDefaultValues()
                    }
                    Keys.onReturnPressed: {
                        setDefaultValues()
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
                    id: dummybutton
                    opacity: 0
		    enabled: false
                }
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
        id: econComboBoxStyle
        ComboBoxStyle {
            background: Image {
                id: burstLengthCombo_bkgrnd
                source: "../../Views/images/device_box.png"
                Rectangle {
                    width: burstLengthCombo_bkgrnd.sourceSize.width  - 28
                    height: burstLengthCombo_bkgrnd.sourceSize.height
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
    }}

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


    See3Camcu30 {
        id: see3camcu30
        onSendEffectMode:{
            switch(effectMode){
            case See3Camcu30.EFFECT_NORMAL:
                rdoEffectNormal.checked = true
                break;
            case See3Camcu30.EFFECT_BLACK_WHITE:
                rdoEffectBW.checked = true
                break;
            case See3Camcu30.EFFECT_GREYSCALE:
                rdoEffectGreyScale.checked = true
                break;
            case See3Camcu30.EFFECT_SKETCH:
                rdoEffectSketch.checked = true
                break;
            case See3Camcu30.EFFECT_NEGATIVE:
                rdoEffectNegative.checked = true
                break;
            }
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onSendDenoiseValue:{
            deNoiseSlider.value = denoiseValue
        }
        onSceneModeValue: {
            currentSceneMode(sceneMode)
        }
        onQFactorValue:{
          skipUpdateUIQFactor = false
          qFactorSlider.value = qFactor
          skipUpdateUIQFactor = true
        }
        onRoiAutoExpModeValue:{
            if(roiMode == See3Camcu30.AutoExpFull){
                autoexpFull.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Camcu30.AutoExpManual){
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize-1
            }
            else if(roiMode == See3Camcu30.AutoExpDisabled){
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
            }
        }

        onFaceDetectModeValue:{
            updateFaceDetectModeUI(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect)
        }
        onSmileDetectModeValue:{
            updateSmileDetectModeUI(smileDetectMode, smileDetectThresholdValue, smileDetectEmbedDataValue)
        }

        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onExposureCompValueReceived:{
            exposureCompValue.text = exposureCompensation
        }
        onFrameRateCtrlValueReceived:{
            skipUpdateUIFrameRate = false
            frameRateSlider.value = frameRateCtrlValue
            skipUpdateUIFrameRate = true
        }
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
                see3camcu30.getExposureCompensation()
            }
        }
        onIndicateSmileThresholdRangeFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
                see3camcu30.getSmileDetectMode()
            }
        }
    }


    Component.onCompleted:{        
        getCameraValues()
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case See3Camcu30.FLIP_ON_MIRROR_ON:
                 flipCtrlVertical.checked = true
                 flipCtrlHorizotal.checked = true
                break;
            case See3Camcu30.FLIP_OFF_MIRROR_ON:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case See3Camcu30.FLIP_ON_MIRROR_OFF:
                 flipCtrlVertical.checked = false
                 flipCtrlHorizotal.checked = true
                break;
            case See3Camcu30.FLIP_OFF_MIRROR_OFF:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
        }
    }

    function updateSmileDetectModeUI(smileDetectMode, smileDetectThresholdValue, smileDetectEmbedDataValue){
        smileThreshold.text = smileDetectThresholdValue
        if(smileDetectMode == See3Camcu30.SmileDetectEnable){
            smileDetectEnable.checked = true
            if(smileDetectEmbedDataValue == See3Camcu30.smileDetectEmbedDataEnable){
                smileDetectEmbedData.checked = true
            }
        }else if(smileDetectMode == See3Camcu30.SmileDetectDisable){
            smileDetectDisable.checked = true
            if(smileDetectEmbedDataValue == See3Camcu30.smileDetectEmbedDataEnable){
                smileDetectEmbedData.checked = true
            }else{
                smileDetectEmbedData.checked = false
            }
        }
    }

    function updateFaceDetectModeUI(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect){        
        if(faceDetectMode == See3Camcu30.FaceRectEnable){
            faceRectEnable.checked = true
            if(faceDetectEmbedDataValue == See3Camcu30.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }
            if(faceDetectOverlayRect == See3Camcu30.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }
        }else if(faceDetectMode == See3Camcu30.FaceRectDisable){
            faceRectDisable.checked = true
            if(faceDetectEmbedDataValue == See3Camcu30.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }else{
                faceDetectEmbedData.checked = false
            }
            if(faceDetectOverlayRect == See3Camcu30.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }else{
                overlayRect.checked = false
            }
        }
    }

    function enableFaceDetectEmbedData(){
        if(see3camcu30.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_CU30_Face_and_Smile_Detection for more details"))
            }
        }
        defaultValue.enabled = true
    }

    function exposureCompSetButtonClicked(){
        exposureCompSet.enabled = false
        setButtonClicked = true
        see3camcu30.setExposureCompensation(exposureCompValue.text)
        exposureCompSet.enabled = true
        defaultValue.enabled = true
    }

    function smileThresholdSetButtonClicked(){
        smileThresholdSet.enabled = false
        setButtonClicked = true
        see3camcu30.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
        smileThresholdSet.enabled = true
        defaultValue.enabled = true
    }

    function enableSmileDetectEmbedData(){
        setButtonClicked = false
        see3camcu30.getSmileDetectMode()
        if(see3camcu30.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)){
            if(smileDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document See3CAM_CU30_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }
        defaultValue.enabled = true
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            autoexpManual.enabled = true
            autoexpFull.enabled = true
            if(autoexpManual.checked)
                autoExpoWinSizeCombo.enabled = true
            if(autoexpFull.checked)
                autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 1
            autoexpFull.opacity = 1
            exposureCompValue.enabled = true
            exposureCompValue.opacity = 1
            exposureCompSet.enabled = true
            exposureCompSet.opacity = 1
            exposureCompText.opacity = 1
        }else{
            autoexpManual.enabled = false
            autoexpFull.enabled = false
            autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 0.1
            autoexpFull.opacity = 0.1
            exposureCompValue.enabled = false
            exposureCompValue.opacity = 0.1
            exposureCompSet.enabled = false
            exposureCompSet.opacity = 0.1
            exposureCompText.opacity = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    function currentSceneMode(mode)
    {
        switch(mode)
        {
            case See3Camcu30.SCENE_NORMAL:
                sceneNormal.checked = true
                break;
            case See3Camcu30.SCENE_DOCUMENT:
                sceneDoc.checked = true
                break;
        }
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function setDefaultValues(){
        defaultValue.enabled = false //To avoid multiple clicks over Default button
        see3camcu30.setToDefault()        
        getCameraValues()
    }

    function getCameraValues(){
        see3camcu30.getEffectMode()
        see3camcu30.getDenoiseValue()
        see3camcu30.getSceneMode()
        see3camcu30.getAutoExpROIModeAndWindowSize()
        see3camcu30.getBurstLength()
        see3camcu30.getQFactor()
        see3camcu30.getOrientation()
        see3camcu30.getFrameRateCtrlValue()
        see3camcu30.getExposureCompensation()
        see3camcu30.getFaceDetectMode()
        see3camcu30.getSmileDetectMode()
    }

    Connections{
         target: root
         onMouseRightClicked:{
             if(autoexpManual.enabled && autoexpManual.checked){
                see3camcu30.setROIAutoExposure(See3Camcu30.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }
         }
         onAutoExposureSelected:{
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         onEnableFaceRectafterBurst:{
           see3camcu30.enableDisableFaceRectangle(true)
         }
         onBeforeRecordVideo:{
            see3camcu30.enableDisableFaceRectangle(false)
         }
         onAfterRecordVideo:{
            see3camcu30.enableDisableFaceRectangle(true)
         }
         onVideoResolutionChanged:{
             getexposureCompFrameRateCtrlTimer.start()
         }
         onPreviewFPSChanged:{
             getexposureCompFrameRateCtrlTimer.start()
         }
         onVideoColorSpaceChanged:{
             getexposureCompFrameRateCtrlTimer.start()
         }
    }
}



