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
import econ.camera.nilecam30usb 1.0
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
    property bool skipUpdateUIOnDenoise: false	
    property var mcuFirmwareVersion;

    Timer {
        id: burstShotTimer
        interval: 1000
        onTriggered: {
            root.imageCapture(CommonEnums.BURST_SHOT);
            stop()
        }
    }


    Connections
    {
        target: root
        onTakeScreenShot:
        {
            if(!isWebKeyPressed){ // mouse click
                nilecam30usb.enableDisableFaceRectangle(false)
                burstShotTimer.start()
            }else{
                nilecam30usb.enableDisableFaceRectangle(false)
            	root.imageCapture(CommonEnums.BURST_SHOT);                    
            }
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
	onFrameSkipCount:{
            nilecam30usb.setStillSkipCount(stillOutFormat)
        }

    }
   
    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            nilecam30usb.getExpRoiModeNileCam30USB()
            stop()
        }
    }

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            nilecam30usb.getExposureCompensationNileCam30USB()
            nilecam30usb.getFrameRateValueNileCam30USB()
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
                    tooltip: "In this mode, the normal unprocessed UYVY or MJPEG image stream
from the camera"
                    exclusiveGroup: sceneInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        nilecam30usb.setSceneModeNileCam30USB(NileCam30usb.SCENE_NORMAL)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setSceneModeNileCam30USB(NileCam30usb.SCENE_NORMAL)
                    }
                }
                RadioButton {
                    id: sceneDoc
                    style:  econRadioButtonStyle
                    text: qsTr("Document")
                    tooltip: "In this mode, the contrast between letters and background is
increased to make it easier to read black writings on white paper."
                    exclusiveGroup: sceneInputGroup
                    activeFocusOnPress: true
                    onClicked: {                        
                        nilecam30usb.setSceneModeNileCam30USB(NileCam30usb.SCENE_DOCUMENT)
                    }
                    Keys.onReturnPressed: {                        
                        nilecam30usb.setSceneModeNileCam30USB(NileCam30usb.SCENE_DOCUMENT)
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
                    tooltip: "In this mode, the normal unprocessed UYVY or MJPEG image stream from the camera"
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_NORMAL)
                    }
                    Keys.onReturnPressed:  {
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_NORMAL)
                    }
                }
                RadioButton {
                    id: rdoEffectBW
                    style:  econRadioButtonStyle
                    text: qsTr("Black and White")
                    tooltip: "In this mode, thresholding is applied to the normal preview
and the image stream is composed of black and white pixels"
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_BLACK_WHITE)
                    }
                    Keys.onReturnPressed: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_BLACK_WHITE)
                    }
                }
                RadioButton {
                    id: rdoEffectGreyScale
                    style:  econRadioButtonStyle
                    text: qsTr("GreyScale")
                    tooltip: "In this mode, the normal preview is desaturated, and the image
stream is composed of gray shades"
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_GREYSCALE)
                    }
                    Keys.onReturnPressed: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_GREYSCALE)
                    }
                }
                RadioButton {
                    id: rdoEffectSketch
                    style:  econRadioButtonStyle
                    text: qsTr("Sketch")
                    tooltip: "In this mode, an effect of edge dominant image stream useful for edge-
detection is produced"
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_SKETCH)
                    }
                    Keys.onReturnPressed: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_SKETCH)
                    }
                }
                RadioButton {
                    id: rdoEffectNegative
                    style:  econRadioButtonStyle
                    text: qsTr("Negative")
                    tooltip: "In this mode, the normal preview is color inversed"
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_NEGATIVE)
                    }
                    Keys.onReturnPressed: {                        
                        nilecam30usb.setSpecialEffectsNileCam30USB(NileCam30usb.EFFECT_NEGATIVE)
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
                spacing:38
                ExclusiveGroup { id: roiExpogroup }

                RadioButton {
                    exclusiveGroup: roiExpogroup
                    id: autoexpFull
                    text: "Full"
                    tooltip: "In this mode, the full region-based exposure value will be applied to the
frame."
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    // setExpRoiModeNileCam30USB() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                    // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                    // winSize is required only for manual mode
                    onClicked: {
                        nilecam30usb.setExpRoiModeNileCam30USB(NileCam30usb.AutoExpFull, 0, 0, 0, 0, 0);
                        autoExpoWinSizeCombo.enabled = false
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setExpRoiModeNileCam30USB(NileCam30usb.AutoExpFull, 0, 0, 0, 0, 0);
                        autoExpoWinSizeCombo.enabled = false
                    }
                }
                RadioButton {
                    exclusiveGroup: roiExpogroup
                    id: autoexpManual
                    text: "Manual"
                    tooltip: "In this mode, you can select the ROI and at that region the exposure
value will be applied to the entire frame"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        nilecam30usb.setExpRoiModeNileCam30USB(NileCam30usb.AutoExpManual, 0, 0, 0, 0, 0);
                        autoExpoWinSizeCombo.enabled = true
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setExpRoiModeNileCam30USB(NileCam30usb.AutoExpManual, 0, 0, 0, 0, 0);
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
                        nilecam30usb.setExpRoiModeNileCam30USB(NileCam30usb.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
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
			tooltip: "You can set the required exposure compensation value by changing the
value in the text box and click the Set button"
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
			if(skipUpdateUIOnDenoise){
                        	nilecam30usb.setDenoiseValueNileCam30USB(deNoiseSlider.value)
			}
			skipUpdateUIOnDenoise = true
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
                        nilecam30usb.setBurstLengthNileCam30USB(burstLengthCombo.currentText)
                    }
                    skipUpdateUIOnBurstLength = true                    
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
                        if(skipUpdateUIQFactor){
                            nilecam30usb.setQFactorNileCam30USB(qFactorSlider.value)
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
                            nilecam30usb.setFrameRateValueNileCam30USB(frameRateSlider.value)
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
                    tooltip: "This control flips the preview left or right."
                    style: econCheckBoxStyle
                    onClicked:{
                        nilecam30usb.setFlipModeNileCam30USB(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setFlipModeNileCam30USB(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    tooltip: "This control flips the preview up or down."
                    style: econCheckBoxStyle
                    onClicked:{
                        nilecam30usb.setFlipModeNileCam30USB(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setFlipModeNileCam30USB(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
                    tooltip: "Will enable the overlay rectangle around faces"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        nilecam30usb.setFaceDetectionRectNileCam30USB(true, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setFaceDetectionRectNileCam30USB(true, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                }
                RadioButton {
                    exclusiveGroup: faceRectGroup
                    id:faceRectDisable
                    text: "Disable"
                    tooltip: "Will disable the overlay rectangle"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        nilecam30usb.setFaceDetectionRectNileCam30USB(false, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setFaceDetectionRectNileCam30USB(false, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                }
            }
            Row{
                spacing: 5
                CheckBox {
                    id: faceDetectEmbedData
                    activeFocusOnPress : true
                    text: "Embed Data"
                    tooltip: "When embed data option is selected, the last section of the frame will be replaced
with face details"
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
                        nilecam30usb.setFaceDetectionRectNileCam30USB(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setFaceDetectionRectNileCam30USB(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
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
                    tooltip: "Enable the smile detection"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        nilecam30usb.setSmileDetectionStateNileCam30USB(true, smileDetectEmbedData.checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setSmileDetectionStateNileCam30USB(true, smileDetectEmbedData.checked)
                    }
                }
                RadioButton {
                    exclusiveGroup: smileDetectGroup
                    id:smileDetectDisable
                    text: "Disable"
                    tooltip: "Will disable the smile detection"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        nilecam30usb.setSmileDetectionStateNileCam30USB(false, smileDetectEmbedData.checked)
                    }
                    Keys.onReturnPressed: {
                        nilecam30usb.setSmileDetectionStateNileCam30USB(false, smileDetectEmbedData.checked)
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
                    tooltip: "When embed data option is selected, the last section of the frame will be replaced
with smile details"
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
	    
            Text{
                id: flashCtrlText
                x: 85
                y: 200
                text: "--- Flash Control ---"
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
                        id: flashModeStrobe
                        text: "Strobe"
                        tooltip: "When you select the Strobe mode, the LED is switched ON for each frame exposure time while video streaming"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            nilecam30usb.setFlashNileCam30USB(NileCam30usb.FLASHMODE_STROBE)
                        }
                        Keys.onReturnPressed: {
                            nilecam30usb.setFlashNileCam30USB(NileCam30usb.FLASHMODE_STROBE)
                        }
                    }
                }
                Column{
                    RadioButton {
                        exclusiveGroup: flashGrp
                        checked: false
                        id: flashModeTorch
                        text: "Torch"
                        tooltip: "When you select the Torch mode, the LED is switched ON until the control is disabled"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            nilecam30usb.setFlashNileCam30USB(NileCam30usb.FLASHMODE_TORCH)
                        }
                        Keys.onReturnPressed: {
                            nilecam30usb.setFlashNileCam30USB(NileCam30usb.FLASHMODE_TORCH)
                        }
                    }
                }
                Column{
                    RadioButton {
                        exclusiveGroup: flashGrp
                        checked: false
                        id: flashModeOff
                        text: "OFF"
                        tooltip: "Disables both flash controls."
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            nilecam30usb.setFlashNileCam30USB(NileCam30usb.FLASHMODE_OFF)
                        }
                        Keys.onReturnPressed: {
                            nilecam30usb.setFlashNileCam30USB(NileCam30usb.FLASHMODE_OFF)
                        }
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
	    readMCUFirmwareVersion()
            messageDialog.text = _text+"\nMCU FW Version:"+mcuFirmwareVersion;	    
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


    NileCam30usb {
        id: nilecam30usb
        onFlashModeValue:{
            currentFlashMode(flashMode)
        }
        onSendEffectMode:{
            switch(effectMode){
            case NileCam30usb.EFFECT_NORMAL:
                rdoEffectNormal.checked = true
                break;
            case NileCam30usb.EFFECT_BLACK_WHITE:
                rdoEffectBW.checked = true
                break;
            case NileCam30usb.EFFECT_GREYSCALE:
                rdoEffectGreyScale.checked = true
                break;
            case NileCam30usb.EFFECT_SKETCH:
                rdoEffectSketch.checked = true
                break;
            case NileCam30usb.EFFECT_NEGATIVE:
                rdoEffectNegative.checked = true
                break;
            }
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onSendDenoiseValue:{
	    skipUpdateUIOnDenoise = false
	    deNoiseSlider.value = denoiseValue
            skipUpdateUIOnDenoise = true
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
            currentROIAutoExposureMode(roiMode, winSize)
        }

        onFaceDetectModeValue:{
            updateFaceDetectModeUI(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect)
        }
        onSmileDetectModeValue:{
            updateSmileDetectModeUI(smileDetectMode, smileDetectEmbedDataValue)
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
                nilecam30usb.getExposureCompensationNileCam30USB()
            }
        }
        onMcuFirmwareVersionReceived:{
		mcuFirmwareVersion = mcuFwVersion
	}
	onUpdateFrameToSkipfromCam:{
            root.updateFrametoSkip(stillSkip)
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
    // current ROI auto exposure mode
    function currentROIAutoExposureMode(roiMode, winSize){       
        switch(roiMode){
            case NileCam30usb.AutoExpFull:
                autoexpFull.checked = true
                autoExpoWinSizeCombo.enabled = false
                break
            case NileCam30usb.AutoExpManual:
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
            case NileCam30usb.AutoExpDisabled:
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
                break
        }
    }

    function currentFlashMode(mode){
        switch(mode){
        case NileCam30usb.FLASHMODE_TORCH:
            flashModeTorch.checked = true
            break;
        case NileCam30usb.FLASHMODE_STROBE:
            flashModeStrobe.checked = true
            break;
        case NileCam30usb.FLASHMODE_OFF:
            flashModeOff.checked = true
            break;
        }
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case NileCam30usb.FLIP_ON_MIRROR_ON:
                 flipCtrlVertical.checked = true
                 flipCtrlHorizotal.checked = true
                break;
            case NileCam30usb.FLIP_OFF_MIRROR_ON:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case NileCam30usb.FLIP_ON_MIRROR_OFF:
                 flipCtrlVertical.checked = false
                 flipCtrlHorizotal.checked = true
                break;
            case NileCam30usb.FLIP_OFF_MIRROR_OFF:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
        }
    }

    function updateSmileDetectModeUI(smileDetectMode, smileDetectEmbedDataValue){
        if(smileDetectMode == NileCam30usb.SmileDetectEnable){
            smileDetectEnable.checked = true
            if(smileDetectEmbedDataValue == NileCam30usb.SmileDetectEmbedDataEnable){
                smileDetectEmbedData.checked = true
            }
	}else if(smileDetectMode == NileCam30usb.SmileDetectDisable){
            smileDetectDisable.checked = true
            if(smileDetectEmbedDataValue == NileCam30usb.SmileDetectEmbedDataEnable){
                smileDetectEmbedData.checked = true
            }else{
                smileDetectEmbedData.checked = false
            }
	}
    }

    function updateFaceDetectModeUI(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect){        
        if(faceDetectMode == NileCam30usb.FaceRectEnable){
            faceRectEnable.checked = true
            if(faceDetectEmbedDataValue == NileCam30usb.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }
            if(faceDetectOverlayRect == NileCam30usb.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }
        }else if(faceDetectMode == NileCam30usb.FaceRectDisable){
            faceRectDisable.checked = true
            if(faceDetectEmbedDataValue == NileCam30usb.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }else{
                faceDetectEmbedData.checked = false
            }
            if(faceDetectOverlayRect == NileCam30usb.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }else{
                overlayRect.checked = false
            }
        }
    }

    function enableFaceDetectEmbedData(){
        if(nilecam30usb.setFaceDetectionRectNileCam30USB(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document NileCAM30_USB_Face_and_Smile_Detection for more details"))
            }
        }        
    }

    function exposureCompSetButtonClicked(){
        exposureCompSet.enabled = false
        setButtonClicked = true
        nilecam30usb.setExposureCompensationNileCam30USB(exposureCompValue.text)
        exposureCompSet.enabled = true        
    }
   
    function enableSmileDetectEmbedData(){
        setButtonClicked = false        
        if(nilecam30usb.setSmileDetectionStateNileCam30USB(true, smileDetectEmbedData.checked)){
            if(smileDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document NileCAM30_USB_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }        
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
            case NileCam30usb.SCENE_NORMAL:
                sceneNormal.checked = true
                break;
            case NileCam30usb.SCENE_DOCUMENT:
                sceneDoc.checked = true
                break;
        }
    }

    function readMCUFirmwareVersion(){
	nilecam30usb.readMCUFirmwareVersion()
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
        nilecam30usb.restoreDefaultNileCam30USB()
        getCameraValues()
        defaultValue.enabled = true
    }

    function getCameraValues(){
        nilecam30usb.getSpecialEffectsNileCam30USB()
        nilecam30usb.getDenoiseValueNileCam30USB()
        nilecam30usb.getSceneModeNileCam30USB()
        nilecam30usb.getExpRoiModeNileCam30USB()
        nilecam30usb.getBurstLengthNileCam30USB()
        nilecam30usb.getQFactorNileCam30USB()
        nilecam30usb.getFlipModeNileCam30USB()
        nilecam30usb.getFrameRateValueNileCam30USB()
        nilecam30usb.getExposureCompensationNileCam30USB()
        nilecam30usb.getFaceDetectionRectNileCam30USB()
        nilecam30usb.getSmileDetectionStateNileCam30USB()
    	nilecam30usb.getFlashNileCam30USB()
    }

    Connections{
         target: root
         onMouseRightClicked:{
             if(autoexpManual.enabled && autoexpManual.checked){
                nilecam30usb.setExpRoiModeNileCam30USB(NileCam30usb.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }
         }
         onAutoExposureSelected:{
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         onEnableFaceRectafterBurst:{
             nilecam30usb.enableDisableFaceRectangle(true)
         }
         onBeforeRecordVideo:{
            nilecam30usb.enableDisableFaceRectangle(false)
         }
         onAfterRecordVideo:{
            nilecam30usb.enableDisableFaceRectangle(true)
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



