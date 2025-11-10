/*
 * see3camcu200.qml -- extension settings for SEE3CAM_CU200 camera
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
import econ.camera.see3camcu200 1.0
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item{

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
        id: ispVersion
        onTriggered:
        {
            getISPFirmwareVersion()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }

    Action {
        id: triggerGainAction
        onTriggered: {
            see3camcu200.setGainMode(SEE3CAM_CU200.AUTO_GAIN,SEE3CAM_CU200.GAIN_SINGLE_SHOT, 1)
        }
    }

    Action {
        id: triggerWbAction
        onTriggered: {
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.AUTO_WB,SEE3CAM_CU200.WB_SINGLE_SHOT, 0, 0)
        }
    }

    Action {
        id: triggerExposureAction
        onTriggered: {
            see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
        }
    }

    Action {
        id: readStatisticsCall
        onTriggered:
        {
            see3camcu200.readStatistics()
        }
    }

    Timer {
        id: getCameraSettingsTimer
        interval: 500
        onTriggered: {
            see3camcu200.getExposureCompensation()
            see3camcu200.getTargetBrightness()
            see3camcu200.readStatistics()
            stop()
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

        function onGetBrightnessFromUVC(brightnessFromUVC){
            skipUpdateBrightness = false
            brightnessSlider.value = brightnessFromUVC.toFixed(3)
            see3camcu200.setBrightness(brightnessFromUVC)
            skipUpdateBrightness = true
        }
        function onGetContrastFromUVC(contrastFromUVC){
            contrastSlider.value = contrastFromUVC
        }
        function onGetSaturationFromUVC(saturationFromUVC){
            skipUpdateSaturation = false
            saturationSlider.value = saturationFromUVC.toFixed(3)
            see3camcu200.setSaturation(saturationFromUVC)
            skipUpdateSaturation = true
        }
        function onGetGammaFromUVC(gammaFromUVC){
            gammaCorrectionSlider.value = gammaFromUVC
        }
        function onGetColorTempFromUVC(colorTempFromUVC){
            colorTempTextField.text = colorTempFromUVC

            colorTempSlider.value = colorTempFromUVC

            //To get color correction matrix and RB gain after color temperature is set
            see3camcu200.getColorCorrectionMatrix()
            see3camcu200.getRBGain()
        }
        function onGetExposureFromUVC(exposureFromUVC){
            manualExpTextField.text = exposureFromUVC
        }
        function onGetFlipMode(){ //To get the Flip status when 2 or more cameras are connected
            sendFlipStatus()
        }
        function onUpdateCrossStillCaptureProperty(isEnable){
            if(isEnable){
                see3camcu200.setPropertiesForCrossStill(true)
            } else{
                see3camcu200.setPropertiesForCrossStill(false)
            }
        }
        function onAutoExposureSelected(autoExposureSelect){
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        function onAutoWhiteBalanceSelected(autoWhiteBalanceSelect){

        }
        function onMouseRightClicked(x, Y, width, height){
            if(expRoiManual.enabled && expRoiManual.checked){
                see3camcu200.setROIAutoExposure(SEE3CAM_CU200.MANUAL_ROI, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        function onVideoResolutionChanged(){
            getCameraSettingsTimer.start()
        }
        function onPreviewFPSChanged(){
            getCameraSettingsTimer.start()
        }
        function onVideoColorSpaceChanged(){
            getCameraSettingsTimer.start()
        }
        function onExtensionTabVisible(visible){
            if(visible){
                see3camcu200.getWhiteBalanceMode()
            }
        }
        function onSkipFrameCount(){
            root.setSkipFrameCount(3)
        }
    }

    property real adjustedRGain: 0.0
    property real adjustedBGain: 0.0
    property real adjustedBrightness: 0.0
    property real adjustedSaturation: 0.0
    property real adjustedDenoiseStrength: 0.0
    property real adjustedSharpnessStrength: 0.0
    property real adjustedGammaCorrection: 0.0
    property real spinBoxMin: -3.99
    property real spinBoxMax: 3.99
    property real spinBoxStepSize: 0.005

    property int exposureMin: 100
    property int exposureMax: 1000000

    property bool skipUpdateBlackLevelMode      : false
    property bool skipUpdateBrightness          : false
    property bool skipUpdateContrast            : false
    property bool skipUpdateSaturation          : false
    property bool skipUpdateDenoiseStrength     : false
    property bool skipUpdateSharpnessStrength   : false
    property bool skipUpdateColorTemperature    : false
    property bool skipUpdateGammaCorrection     : false
    property bool setButtonClicked              : false

    property bool wbSingleShotBtnClicked        : false
    property bool expSingleShotBtnClicked       : false

    property bool skipUpdateRGainMode           : false
    property bool skipUpdateBGainMode           : false

    property bool skipUpdateSingleShotExposure  : false
    property bool skipUpdateSingleShotWhiteBalance  : false
    property bool skipUpdateUIOnWBPresetMode    : false
    property bool skipUpdateTargetBrightness    : false

    property bool skipUpdateExposureCompensation : false
    property bool skipUpdateExposureUpperLimit  : false
    property bool skipUpdateUIOnExpWindowSize   : false
    property bool skipUpdateUIOnAntiFlickerMode : false

    property int minLimit: 100
    property int maxLimit: 1000000

    property int brightnessInt: 0
    property int saturationInt: 0
    property int gammaInt: 0
    property int denoiseStrengthInt : 0
    property int sharpnessStrengthInt : 0
    property int exposureInt: 0
    property int exposureText: 0

    property real currentRGainValue: 0.0
    property real currentBGainValue: 0.0

    property real autoRr : 0.0
    property real autoRg : 0.0
    property real autoRb : 0.0

    property real autoGr : 0.0
    property real autoGg : 0.0
    property real autoGb : 0.0

    property real autoBr : 0.0
    property real autoBg : 0.0
    property real autoBb : 0.0

    property bool uvcAutoExpSelected  : false

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle
        Item{
            height: 3000

            ColumnLayout{
                x:2
                y:5
                spacing:18

                Text {
                    id: whiteBalance
                    text: "--- White Balance Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 30
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: whiteBalanceGroup }
                    RadioButton {
                        id: autoWb
                        exclusiveGroup: whiteBalanceGroup
                        checked: false
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "This control enables user to set white balance in automatic mode."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setAutoWhiteBalance()
                        }
                        Keys.onReturnPressed: {
                            setAutoWhiteBalance()
                        }
                    }
                    RadioButton {
                        id: manualWb
                        exclusiveGroup: whiteBalanceGroup
                        checked: false
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "This control enables user to set the white balance in manual mode."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setManualWhiteBalance()
                        }
                        Keys.onReturnPressed: {
                            setManualWhiteBalance()
                        }
                    }
                }

                Text
                {
                    id: autoWbFeaturesTitle
                    text: "--- Auto White Balance ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing:10
                    ExclusiveGroup { id: autoWbFeatureGroup }
                    RadioButton {
                        exclusiveGroup: autoWbFeatureGroup
                        id: wbContinious
                        text: "Continuous"
                        activeFocusOnPress: true
                        enabled: (autoWb.enabled && autoWb.checked) ? true : false
                        opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "The camera detects the light source and sets the white balance value continuously with respect to change in scenes."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            skipUpdateSingleShotWhiteBalance = true
                            setAutoWhiteBalance()
                        }
                        Keys.onReturnPressed: {
                            skipUpdateSingleShotWhiteBalance = true
                            setAutoWhiteBalance()
                        }
                    }
                }

                Row{
                    spacing:25
                    RadioButton {
                        exclusiveGroup: autoWbFeatureGroup
                        id: wbSingleShot
                        text: "Single Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        enabled: (autoWb.enabled && autoWb.checked) ? true : false
                        opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The camera calculates the white balance based on the current scene just once and strict with it until next request."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setAutoWhiteBalance()
                        }
                        Keys.onReturnPressed: {
                            setAutoWhiteBalance()
                        }
                    }
                    Button {
                        id: triggerWbBtn
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (wbSingleShot.enabled && wbSingleShot.checked) ? true : false
                        opacity: (wbSingleShot.enabled && wbSingleShot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        action: (wbSingleShot.enabled && wbSingleShot.checked) ? triggerWbAction : null
                        tooltip: "Allows camera to calculate the white balance value once in Single shot mode"
                        Keys.onReturnPressed: {
                            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.AUTO_WB,SEE3CAM_CU200.WB_SINGLE_SHOT, 0, 0)
                        }
                    }
                }

                Text
                {
                    id: manualWbFeaturesTitle
                    text: "--- Manual White Balance ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Column
                {
                    spacing: 15

                    RadioButton {
                         id: colorTemperature
                         exclusiveGroup: manualWbGroup
                         checked: false
                         text: "Color Temperature"
                         activeFocusOnPress: true
                         style: econRadioButtonStyle
                         enabled: (manualWb.enabled && manualWb.checked) ? true : false
                         opacity: (manualWb.enabled && manualWb.checked) ? 1 : 0.1
                         ToolButton{
                             tooltip: "It is used for white balancing the scene based on the temperature set by applying predefined R and B digital gain."
                             width: 15
                             height: 20
                             opacity: 0
                         }
                         onClicked: {
                             setManualWhiteBalance()
                         }
                         Keys.onReturnPressed: {
                             setManualWhiteBalance()
                         }
                     }

                     Row{
                         spacing: 35
                         Slider {
                             id: colorTempSlider
                             activeFocusOnPress: true
                             updateValueWhileDragging: false
                             width: 150
                             style:econSliderStyle
                             opacity: (colorTemperature.enabled && colorTemperature.checked) ? 1 : 0.1
                             enabled: (colorTemperature.enabled && colorTemperature.checked) ? true : false
                             onValueChanged:  {
                                 colorTempTextField.text = colorTempSlider.value
                                 if(skipUpdateColorTemperature){

                                     //Sending color temperature value to update white balance in UVC
                                     root.sendColorTemperatureToUVC(colorTempSlider.value)

                                     see3camcu200.setColorTemperature(colorTempSlider.value)

                                     //To get color correction matrix and RB gain after color temperature is set
                                     see3camcu200.getColorCorrectionMatrix()
                                     see3camcu200.getRBGain()
                                 }

                                 skipUpdateColorTemperature = true
                             }
                         }
                         TextField {
                             id: colorTempTextField
                             text: colorTempSlider.value
                             font.pixelSize: 10
                             font.family: "Ubuntu"
                             smooth: true
                             horizontalAlignment: TextInput.AlignHCenter
                             style: econTextFieldStyle
                             opacity: (colorTemperature.enabled && colorTemperature.checked) ? 1 : 0.1
                             enabled: (colorTemperature.enabled && colorTemperature.checked) ? true : false
                             validator: IntValidator {bottom: colorTempSlider.minimumValue; top: colorTempSlider.maximumValue}
                             onTextChanged: {
                                 if(text.length > 0){
                                     colorTempSlider.value = colorTempTextField.text
                                 }
                             }
                         }
                     }
                }

                Column{
                    spacing: 15
                    ExclusiveGroup { id: manualWbGroup }
                    RadioButton {
                        id: wbPreset
                        exclusiveGroup: manualWbGroup
                        checked: false
                        text: "Preset "
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        enabled: (manualWb.enabled && manualWb.checked) ? true : false
                        opacity: (manualWb.enabled && manualWb.checked) ? 1 : 0.1
                        ToolButton{
                            tooltip: "It is used to set the predefined colour temperature values by applying predefined R and B digital gain."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setWbPresetMode()
                        }
                        Keys.onReturnPressed: {
                            setWbPresetMode()
                        }
                    }
                    ComboBox
                    {
                        id: wbPresetCombo
                        model: ListModel {
                            ListElement { text: "HZ(2300K)" }
                            ListElement { text: "A(2800K)" }
                            ListElement { text: "U30(3000K)" }
                            ListElement { text: "TL84(4000K)" }
                            ListElement { text: "CWF(4100K)" }
                            ListElement { text: "D50(5000K)" }
                            ListElement { text: "D60(6000K)" }
                            ListElement { text: "D65(6500K)" }
                        }
                        activeFocusOnPress: true
                        style: econComboBoxStyle
                        opacity: (wbPreset.enabled && wbPreset.checked) ? 1 : 0.1
                        enabled: (wbPreset.enabled && wbPreset.checked) ? true : false
                        onCurrentIndexChanged: {
                            if(skipUpdateUIOnWBPresetMode)
                            {
                                setWbPresetMode()
                            }
                            skipUpdateUIOnWBPresetMode = true
                        }
                    }
                }

                RadioButton {
                    id: proMode
                    exclusiveGroup: manualWbGroup
                    checked: false
                    text: "Pro Mode"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    enabled: (manualWb.enabled && manualWb.checked) ? true : false
                    opacity: (manualWb.enabled && manualWb.checked) ? 1 : 0.1
                    ToolButton{
                        tooltip: "This control enables to set the Red gain, Blue gain and Colour Correction Matrix values manually."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                    onClicked: {
                        setManualWhiteBalance()
                    }
                    Keys.onReturnPressed: {
                        setManualWhiteBalance()
                    }
                }

                Text
                {
                    id: gainRTitle
                    text: "--- Red Gain ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    width: 80
                    Layout.alignment: Qt.AlignCenter
                    wrapMode: Text.WordWrap
                    opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                    enabled: (proMode.enabled && proMode.checked) ? true : false
                    ToolButton{
                        tooltip: "Modifies the digital gain of R channel in steps of 0.005. "
                        width: 200
                        opacity: 0
                    }
                }
                Row
                {
                     spacing: 35
                     Slider
                     {
                         id: gainRSlider
                         activeFocusOnPress: true
                         updateValueWhileDragging: false
                         width: 150
                         style:econSliderStyle
                         opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                         enabled: (proMode.enabled && proMode.checked) ? true : false
                         onValueChanged:  {
                             gainRTextField.text = gainRSlider.value
                             if(skipUpdateRGainMode){
                                 adjustedRGain = parseFloat((gainRSlider.value).toFixed(3));
                                 gainRTextField.text = adjustedRGain
                                 see3camcu200.setRBGain(adjustedRGain, gainBSlider.value)
                             }
                             skipUpdateRGainMode = true
                         }
                     }
                     TextField
                     {
                         id: gainRTextField
                         text:gainRSlider.value
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         validator: IntValidator {bottom: gainRSlider.minimumValue; top: gainRSlider.maximumValue}
                         opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                         enabled: (proMode.enabled && proMode.checked) ? true : false
                         onTextChanged: {
                             const match = gainRTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                             if(match){
                                 gainRTextField.text = match[1];
                                 gainRSlider.value = gainRTextField.text
                             }
                         }
                     }
                 }

                Text
                {
                    id: gainBTitle
                    text: "--- Blue Gain ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    width: 80
                    Layout.alignment: Qt.AlignCenter
                    wrapMode: Text.WordWrap
                    opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                    enabled: (proMode.enabled && proMode.checked) ? true : false
                    ToolButton{
                        tooltip: "Modifies the digital gain of B channel in steps of 0.005. "
                        width: 200
                        opacity: 0
                    }
                }

                Row
                {
                     spacing: 35
                     Slider
                     {
                         id: gainBSlider
                         activeFocusOnPress: true
                         updateValueWhileDragging: false
                         width: 150
                         style:econSliderStyle
                         opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                         enabled: (proMode.enabled && proMode.checked) ? true : false
                         onValueChanged:  {
                             gainBTextField.text = gainBSlider.value
                             if(skipUpdateBGainMode){
                                 adjustedBGain = parseFloat((gainBSlider.value).toFixed(3));
                                 gainBTextField.text = adjustedBGain
                                 see3camcu200.setRBGain(gainRSlider.value, adjustedBGain)
                             }
                             skipUpdateBGainMode = true
                         }
                     }
                     TextField
                     {
                         id: gainBTextField
                         text:gainBSlider.value
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         validator: IntValidator {bottom: gainBSlider.minimumValue; top: gainBSlider.maximumValue}
                         opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                         enabled: (proMode.enabled && proMode.checked) ? true : false
                         onTextChanged: {
                             const match = gainBTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                             if(match){
                                 gainBTextField.text = match[1];
                                 gainBSlider.value = gainBTextField.text
                             }
                         }
                     }
                 }

                Text
                {
                    id: colorCorrectionMaxTitle
                    text: "-- Colour Correction Matrix --"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    width: 80
                    Layout.alignment: Qt.AlignCenter
                    wrapMode: Text.WordWrap
                    opacity: (proMode.enabled && proMode.checked) ? 1 : 0.5
                    enabled: (proMode.enabled && proMode.checked) ? true : false
                    ToolButton{
                        tooltip: "It is used for applying manual 3x3 matrix to RGB channel in steps of 0.005."
                        width: 200
                        opacity: 0
                    }
                }

                ColumnLayout{
                    id:ccmLayout
                    spacing: 10
                    Layout.alignment: Qt.AlignCenter

                    Row{
                        spacing: 10
                        Layout.alignment: Qt.AlignCenter

                        Column {
                            spacing: 2

                            SpinBox{
                                id:spinBoxRr
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Rr"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxRr.horizontalCenter
                            }
                        }

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxRg
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Rg"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxRg.horizontalCenter
                            }
                        }

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxRb
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Rb"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxRb.horizontalCenter
                            }
                        }
                    }

                    Row{
                        spacing: 9
                        Layout.alignment: Qt.AlignCenter

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxGr
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Gr"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxGr.horizontalCenter
                            }
                        }

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxGg
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Gg"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxGg.horizontalCenter
                            }
                        }

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxGb
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Gb"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxGb.horizontalCenter
                            }
                        }
                    }

                    Row{
                        spacing: 10
                        Layout.alignment: Qt.AlignCenter

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxBr
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Br"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxBr.horizontalCenter
                            }
                        }

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxBg
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Bg"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxBg.horizontalCenter
                            }
                        }

                        Column {
                            spacing: 2
                            SpinBox{
                                id:spinBoxBb
                                decimals: 3
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                            }
                            Label {
                                text: "Bb"
                                font.pixelSize: 13
                                font.family: "Ubuntu"
                                color: "white"
                                opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                                enabled: (proMode.enabled && proMode.checked) ? true : false
                                anchors.horizontalCenter: spinBoxBb.horizontalCenter
                            }
                        }
                    }

                }

                Button
                {
                    id: colorMatrixSetBtn
                    activeFocusOnPress : true
                    Layout.alignment: Qt.AlignCenter
                    text: "Set"
                    tooltip: "You can set the required colour correction matrix elements by changing the value in the SpinBox and click the Set button"
                    style: econButtonStyle
                    opacity: (proMode.enabled && proMode.checked) ? 1 : 0.1
                    enabled: (proMode.enabled && proMode.checked) ? true : false
                    implicitHeight: 25
                    implicitWidth: 60
                    onClicked:
                    {
                        see3camcu200.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)
                    }
                    Keys.onReturnPressed:
                    {
                        see3camcu200.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)
                    }
                }

                Text
                {
                    id: saturationTitle
                    text: "--- Saturation ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the gain of Z curve applied to UV channel in steps of 0.005 by changing the saturation of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: saturationSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle

                        onValueChanged:  {
                            saturationTextField.text = saturationSlider.value.toFixed(3)
                            if(skipUpdateSaturation){
                                // Round the slider and TextField to three decimal places
                                adjustedSaturation = parseFloat((saturationSlider.value).toFixed(3));

                                //Sending HID value to UVC
                                saturationInt = adjustedSaturation * 200
                                root.sendSaturationToUVC(saturationInt)

                                saturationTextField.text = adjustedSaturation.toFixed(3)
                                see3camcu200.setSaturation(adjustedSaturation.toFixed(3))
                            }
                            skipUpdateSaturation = true
                        }
                    }
                    TextField {
                        id: saturationTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: saturationSlider.minimumValue; top: saturationSlider.maximumValue}
                        onTextChanged: {
                            const match = saturationTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                            if(match){
                                saturationTextField.text = match[1];
                                saturationSlider.value = saturationTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: brightnessTitle
                    text: "--- Target Brightness ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Target Brightness control allows to set the brightness of the image preview should maintained in auto exposure and auto gain modes."
                        opacity: 0
                        width: 200
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: targetBrightness
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            targetBrightnessTextField.text = targetBrightness.value
                            if(skipUpdateTargetBrightness){
                                see3camcu200.setTargetBrightness(targetBrightness.value)
                            }
                            skipUpdateTargetBrightness = true
                        }
                    }
                    TextField {
                        id: targetBrightnessTextField
                        text: targetBrightness.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: targetBrightness.minimumValue; top: targetBrightness.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                targetBrightness.value = targetBrightnessTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: expModeTitle
                    text: "--- Exposure Mode---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 30
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: expModeGroup }
                    RadioButton {
                        id: autoExposure
                        exclusiveGroup: expModeGroup
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "This control enables user to set exposure in automatic mode."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setExposureProperties()
                        }
                        Keys.onReturnPressed: {
                            setExposureProperties()
                        }
                    }

                    RadioButton {
                        id: manualExposure
                        exclusiveGroup: expModeGroup
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "This control enables user to set the manual exposure value."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setExposureProperties()
                        }
                        Keys.onReturnPressed: {
                            setExposureProperties()
                        }
                    }
                }

                Text {
                    id: upperLimitMode
                    text: "--- Auto Gain Upper Limit ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "You can set the required limit by changing the value in the slider"
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: gainUpperLimitSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainUpperLimitTextField.text = gainUpperLimitSlider.value
                            if(skipUpdateExposureUpperLimit){
                                see3camcu200.setAutoGainUpperLimit(gainUpperLimitSlider.value)
                            }
                            skipUpdateExposureUpperLimit = true
                        }
                    }
                    TextField {
                        id: gainUpperLimitTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainUpperLimitSlider.minimumValue; top: gainUpperLimitSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                gainUpperLimitSlider.value = gainUpperLimitTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: expCompTitle
                    text: "--- Exposure Compensation ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        width: 200
                        opacity: 0
                    }
                }

                Row
                {
                    spacing: 9
                    Text
                    {
                        id: expCompLabel
                        text: "value(µs)[100 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                    }
                    TextField
                    {
                        id: expCompensationTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: exposureMin; top: exposureMax}
                    }
                    Button
                    {
                        id: expCompSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            expCompSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu200.setExposureCompensation(expCompensationTextField.text);
                            expCompSetBtn.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            expCompSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu200.setExposureCompensation(expCompensationTextField.text);
                            expCompSetBtn.enabled = true
                        }
                    }
                }


                Text {
                    id: autoExpModeTitle
                    text: "--- Auto Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:10
                    ExclusiveGroup { id: autoExposureFeatureGroup }
                    RadioButton {
                        exclusiveGroup: autoExposureFeatureGroup
                        id: exposureContinious
                        text: "Continuous"
                        activeFocusOnPress: true
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "The camera detects the light source and sets the exposure value continuously with respect to change in scenes."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            expSingleShotBtnClicked = false
                            see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.CONTINIOUS_EXPOSURE, manualExpTextField.text)
                        }
                        Keys.onReturnPressed: {
                            expSingleShotBtnClicked = false
                            see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.CONTINIOUS_EXPOSURE, manualExpTextField.text)
                        }
                    }
                }

                Row{
                    spacing:25
                    RadioButton {
                        exclusiveGroup: autoExposureFeatureGroup
                        id: exposureSingleShot
                        text: "Single Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The camera calculates the exposure based on the current scene just once and strict with it until next request."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            if(!expSingleShotBtnClicked){
                                expSingleShotBtnClicked = true
                                see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
                            }
                        }
                        Keys.onReturnPressed: {
                            if(!expSingleShotBtnClicked){
                                expSingleShotBtnClicked = true
                                see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
                            }
                        }
                    }
                    Button {
                        id: triggerExpBtn
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (exposureSingleShot.enabled && exposureSingleShot.checked) ? true : false
                        opacity: (exposureSingleShot.enabled && exposureSingleShot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        action: (exposureSingleShot.enabled && exposureSingleShot.checked) ? triggerExposureAction : null
                        tooltip: "Allows camera to calculate the exposure value once in Single shot mode"
                        Keys.onReturnPressed: {
                            see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
                        }
                    }
                }

                Text {
                    id: manualExpModeTitle
                    text: "--- Manual Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "It controls the integration time of the sensor. It changes in the range of 100µs to 1s. It is expressed in microseconds(µs)."
                        width: 200
                        opacity: 0
                    }
                }

                Row
                {
                    spacing: 9
                    Text
                    {
                        id: manualExpTitle
                        text: "value(µs)[100 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                    }
                    TextField
                    {
                        id: manualExpTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: exposureMin; top: exposureMax}
                    }
                    Button
                    {
                        id: exposureSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            exposureSetBtn.enabled = false
                            setButtonClicked = true

                            see3camcu200.setExposure(SEE3CAM_CU200.MANUAL_EXPOSURE, 0, manualExpTextField.text);

                            //Sending HID value to UVC
                            exposureText = manualExpTextField.text
                            if(manualExpTextField.text <= 1000000)
                            {
                                exposureInt = manualExpTextField.text / 100
                                root.sendExposureToUVC(exposureInt)
                            }
                            else{
                                root.sendExposureToUVC(100000)
                            }
                            manualExpTextField.text = exposureText

                            exposureSetBtn.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            exposureSetBtn.enabled = false
                            setButtonClicked = true

                            //Sending HID value to UVC
                            root.sendExposureToUVC(manualExpTextField.text)

                            manualExpTextField.text = exposureText

                            see3camcu200.setExposure(SEE3CAM_CU200.MANUAL_EXPOSURE, 0, manualExpTextField.text);
                            exposureSetBtn.enabled = true
                        }
                    }
                }

                Text
                {
                    id: roiAutoExpMode
                    text: "--- Auto Exposure ROI ---"
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
                      ExclusiveGroup { id: roiExpogroup }
                      Layout.alignment: Qt.AlignCenter

                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: expRoiFull
                          text: "Full"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          ToolButton{
                              tooltip: "Full region-based exposure value will be applied to the frame."
                              width: 15
                              height: 20
                              opacity: 0
                          }
                          onClicked: {
                              setROIAutoExposure()
                          }
                          Keys.onReturnPressed: {
                              setROIAutoExposure()
                          }
                      }
                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: expRoiManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          ToolButton{
                              tooltip: "Users can select the region of interest to calculate the exposure value for that particular area. This value will then be applied to the entire frame.
The mouse click button can be used to select the auto exposure area in the preview."
                              width: 15
                              height: 20
                              opacity: 0
                          }
                          onClicked: {
                              setROIAutoExposure()
                          }
                          Keys.onReturnPressed: {
                              setROIAutoExposure()
                          }
                      }
                }
                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (expRoiManual.checked && expRoiManual.enabled) ? true : false
                    opacity: (expRoiManual.checked && expRoiManual.enabled) ? 1 : 0.1
                    ToolButton{
                        tooltip: "Window size defines the size of the exposure region. For window size 8, the entire frame will be the focus region.
If the exposure region exceeds the frame boundary, the ROI will be clipped automatically."
                        width: 200
                        opacity: 0
                    }
                    model: ListModel
                           {
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
                            see3camcu200.setROIAutoExposure(SEE3CAM_CU200.MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
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

                ComboBox
                {
                    id: antiFlickerCombo
                    model: ListModel
                           {
                                ListElement { text: "50 Hz" }
                                ListElement { text: "60 Hz" }
                                ListElement { text: "DISABLE" }
                           }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    ToolButton{
                        tooltip: "Disable : It disables Anti-Flicker mode
50 Hz   : 50Hz flicker frequency is applied.
60 Hz   : 60Hz flicker frequency is applied."
                        width: 200
                        opacity: 0
                    }
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnAntiFlickerMode){
                            setAntiFlicker()
                        }
                        skipUpdateUIOnAntiFlickerMode = true
                    }
                }

                Text
                {
                    id: brightnessText
                    text: "--- Brightness ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the Y channel gain in steps of 0.005 by changing the brightness of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: brightnessSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            brightnessTextField.text = brightnessSlider.value.toFixed(3)
                            if(skipUpdateBrightness){
                                // Round the slider and TextField to three decimal places
                                adjustedBrightness = parseFloat((brightnessSlider.value).toFixed(3));

                                //Sending HID value to UVC
                                brightnessInt = adjustedBrightness * 200;
                                root.sendBrightnessToUVC(brightnessInt)

                                brightnessTextField.text = adjustedBrightness.toFixed(3)
                                see3camcu200.setBrightness(adjustedBrightness.toFixed(3))
                            }
                            skipUpdateBrightness = true
                        }
                    }
                    TextField {
                        id: brightnessTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: brightnessSlider.minimumValue; top: brightnessSlider.maximumValue}
                        onTextChanged: {
                            const match = brightnessTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                            if(match){
                                brightnessTextField.text = match[1];
                                brightnessSlider.value = brightnessTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: contrastTitle
                    text: "--- Contrast ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the strengths of S curve applied to Y channel by changing the contrast of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: contrastSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            contrastTextField.text = contrastSlider.value
                            if(skipUpdateContrast){
                                //Sending HID value to UVC
                                root.sendContrastToUVC(contrastSlider.value)

                                see3camcu200.setContrast(contrastSlider.value)
                            }
                            skipUpdateContrast = true
                        }
                    }
                    TextField {
                        id: contrastTextField
                        text: contrastSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: contrastSlider.minimumValue; top: contrastSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                contrastSlider.value = contrastTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: gammaCorrectionText
                    text: "--- Gamma Correction ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the strength of gamma correction curve applied to Y channel in steps of 0.1 by changing the gamma of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: gammaCorrectionSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            if(skipUpdateGammaCorrection){
                                gammaCorrectionTextField.text = gammaCorrectionSlider.value
                                // Round the slider and TextField to three decimal places
                                adjustedGammaCorrection = parseFloat((gammaCorrectionSlider.value).toFixed(1));

                                //Sending HID value to UVC
                                gammaInt = adjustedGammaCorrection * 10
                                root.sendGammaToUVC(gammaInt)

                                gammaCorrectionTextField.text = adjustedGammaCorrection.toFixed(1)
                                see3camcu200.setGammaCorrection(adjustedGammaCorrection.toFixed(1))
                            }
                            skipUpdateGammaCorrection = true
                        }
                    }
                    TextField {
                        id: gammaCorrectionTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        readOnly:true
                        validator: IntValidator {bottom: gammaCorrectionSlider.minimumValue; top: gammaCorrectionSlider.maximumValue}
                        onTextChanged: {
                            const match = gammaCorrectionTextField.text.match(/^(-?\d+(\.\d{0,1})?).*/);
                            if(match){
                                gammaCorrectionTextField.text = match[1];
                                gammaCorrectionSlider.value = gammaCorrectionTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: blackLevelAdj
                    text: "--- Black Level ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Black level is the value which is subtracted from the image signal to compensate the thermally generated noise.
 Note: Changing the value will affect image quality."
                        opacity: 0
                        width: 200
                    }
                }
                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: blackLevelSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            blackLevelTextField.text = blackLevelSlider.value
                            if(skipUpdateBlackLevelMode){
                                see3camcu200.setBlackLevel(blackLevelSlider.value)
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
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
                        }
                        Keys.onReturnPressed: {
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
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
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
                        }
                        Keys.onReturnPressed: {
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
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
                  spacing: 25
                  Layout.alignment: Qt.AlignCenter
                  ExclusiveGroup { id: cameraModeGroup }

                  RadioButton {
                      id: masterMode
                      style:  econRadioButtonStyle
                      text: qsTr("Master")
                      exclusiveGroup: cameraModeGroup
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
                      exclusiveGroup: cameraModeGroup
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
               Row{
                   spacing: 30
                   Layout.alignment: Qt.AlignCenter
                   ExclusiveGroup { id: strobesGrp }
                   RadioButton {
                       id: strobeOn
                       exclusiveGroup: strobesGrp
                       checked: false
                       text: "On"
                       ToolButton{
                           tooltip: "When this control is enabled, the LED is ON for each frame’s exposure time while Video Streaming."
                           width: 15
                           height: 20
                           opacity: 0
                       }
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_ON)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_ON)
                       }
                   }

                   RadioButton {
                       id: strobeOff
                       exclusiveGroup: strobesGrp
                       checked: false
                       text: "Off"
                       ToolButton{
                           tooltip: "It disables strobe control."
                           width: 15
                           height: 20
                           opacity: 0
                       }
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_OFF)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_OFF)
                       }
                   }
               }

               Text {
                   id: readStatisticsTitle
                   text: "--- Read Statistics ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "To Read current Gain, Exposure and CCM in auto mode"
                       width: 200
                       opacity: 0
                   }
               }

               Row
               {
                   spacing: 9
                   Layout.alignment: Qt.AlignCenter
                   Text
                   {
                       id: exposureStatisticsTitle
                       text: "Exposure(µs)"
                       font.pixelSize: 14
                       font.family: "Ubuntu"
                       color: "#ffffff"
                       smooth: true
                       width: 80
                       wrapMode: Text.WordWrap
                       ToolButton{
                           tooltip: "This control displays the calculated current exposure value in Auto exposure mode."
                           width: 200
                           opacity: 0
                       }
                   }
                   TextField
                   {
                       id: exposureStatistics
                       font.pixelSize: 10
                       font.family: "Ubuntu"
                       readOnly: true
                       smooth: true
                       horizontalAlignment: TextInput.AlignHCenter
                       style: econTextFieldStyle
                       enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                       opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                       implicitHeight: 25
                       implicitWidth: 80
                   }
               }

               Row
               {
                   spacing: 35
                   Layout.alignment: Qt.AlignCenter
                   Text {
                       id: gainLabel
                       text: "Gain "
                       font.pixelSize: 14
                       font.family: "Ubuntu"
                       color: "#ffffff"
                       smooth: true
                       ToolButton{
                           tooltip: "This control displays the calculated current gain value in Auto gain mode."
                           width: 200
                           opacity: 0
                       }
                   }
                   TextField
                   {
                       id: gainStatTextField
                       font.pixelSize: 10
                       font.family: "Ubuntu"
                       readOnly: true
                       smooth: true
                       horizontalAlignment: TextInput.AlignHCenter
                       style: econTextFieldStyle
                       enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                       opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                   }
               }

               Row
               {
                   spacing: 10
                   Layout.alignment: Qt.AlignCenter
                   Row
                   {
                       spacing: 20
                       Text {
                           id: rGainLabel
                           text: "R Gain"
                           font.pixelSize: 14
                           font.family: "Ubuntu"
                           color: "#ffffff"
                           smooth: true
                           ToolButton{
                               tooltip: "This control displays the calculated current digital gain of R channel."
                               width: 200
                               opacity: 0
                           }
                       }
                       TextField
                       {
                           id: rGainStatTextField
                           font.pixelSize: 10
                           font.family: "Ubuntu"
                           readOnly: true
                           smooth: true
                           horizontalAlignment: TextInput.AlignHCenter
                           style: econTextFieldStyle
                           enabled: (autoWb.enabled && autoWb.checked) ? true : false
                           opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                           onTextChanged: {
                               const match = rGainStatTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                               if(match){
                                   rGainStatTextField.text = match[1];
                               }
                           }
                       }
                   }

                   Row
                   {
                       spacing: 20
                       Text {
                           id: bGainLabel
                           text: "B Gain"
                           font.pixelSize: 14
                           font.family: "Ubuntu"
                           color: "#ffffff"
                           smooth: true
                           ToolButton{
                               tooltip: "This control displays the calculated current digital gain of B channel."
                               width: 200
                               opacity: 0
                           }
                       }
                       TextField
                       {
                           id: bGainStatTextField
                           font.pixelSize: 10
                           font.family: "Ubuntu"
                           readOnly: true
                           smooth: true
                           horizontalAlignment: TextInput.AlignHCenter
                           style: econTextFieldStyle
                           enabled: (autoWb.enabled && autoWb.checked) ? true : false
                           opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                           onTextChanged: {
                               const match = bGainStatTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                               if(match){
                                   bGainStatTextField.text = match[1];
                               }
                           }
                       }
                   }
               }

               Text {
                   id: readCCMTitle
                   text: "Colour Correction Matrix"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   ToolButton{
                       tooltip: "This control displays the calculated current 3x3 matrix of RGB channel."
                       width: 200
                       opacity: 0
                   }
               }

               ColumnLayout{
                   id:readCCMLayout
                   spacing: 10
                   Layout.alignment: Qt.AlignCenter

                   Row{
                       spacing: 10
                       Layout.alignment: Qt.AlignCenter

                       Column {
                           spacing: 2
                           TextField{
                               id:readRr
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               onTextChanged: {
                                   const match = readRr.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readRr.text = match[1];
                                   }
                               }

                               horizontalAlignment: TextInput.AlignHCenter
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               style: econTextFieldStyle
                           }
                           Label {
                               text: "Rr"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readRr.horizontalCenter
                           }
                       }

                       Column {
                           spacing: 2
                           TextField{
                               id:readRg
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readRg.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readRg.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Rg"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readRg.horizontalCenter
                           }
                       }

                       Column {
                           spacing: 2
                           TextField{
                               id:readRb
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readRb.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readRb.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Rb"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readRb.horizontalCenter
                           }
                       }
                   }


                   Row{
                       spacing: 10
                       Layout.alignment: Qt.AlignCenter

                       Column {
                           spacing: 2
                           TextField{
                               id:readGr
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readGr.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readGr.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Gr"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readGr.horizontalCenter
                           }
                       }

                       Column {
                           spacing: 2
                           TextField{
                               id:readGg
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               style: econTextFieldStyle
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               onTextChanged: {
                                   const match = readGg.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readGg.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Gg"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readGg.horizontalCenter
                           }
                       }

                       Column {
                           spacing: 2
                           TextField{
                               id:readGb
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readGb.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readGb.text = match[1];
                                   }
                               }
                           }

                           Label {
                               text: "Gb"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readGb.horizontalCenter
                           }
                       }
                   }

                   Row{
                       spacing: 10
                       Layout.alignment: Qt.AlignCenter

                       Column {
                           spacing: 2
                           TextField{
                               id:readBr
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readBr.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readBr.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Br"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readBr.horizontalCenter
                           }
                       }

                       Column {
                           spacing: 2
                           TextField{
                               id:readBg
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readBg.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readBg.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Bg"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readBg.horizontalCenter
                           }
                       }

                       Column {
                           spacing: 2
                           TextField{
                               id:readBb
                               font.pixelSize: 10
                               font.family: "Ubuntu"
                               readOnly: true
                               smooth: true
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               horizontalAlignment: TextInput.AlignHCenter
                               style: econTextFieldStyle
                               onTextChanged: {
                                   const match = readBb.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                                   if(match){
                                       readBb.text = match[1];
                                   }
                               }
                           }
                           Label {
                               text: "Bb"
                               font.pixelSize: 12
                               font.family: "Ubuntu"
                               color: "white"
                               enabled: (autoWb.enabled && autoWb.checked) ? true : false
                               opacity: (autoWb.enabled && autoWb.checked) ? 1 : 0.1
                               anchors.horizontalCenter: readBb.horizontalCenter
                           }
                       }
                   }
               }



               Row{
                   Layout.alignment: Qt.AlignCenter
                   Button {
                       id: readStatisticsBtn
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Read Statistics"
                       action: readStatisticsCall
                       style: econButtonStyle
                       Keys.onReturnPressed: {
                           see3camcu200.readStatistics()
                       }
                   }
               }

               Text {
                       id: enableDisableDenoise
                       text: "--- Enable/Disable Denoising ---"
                       font.pixelSize: 14
                       font.family: "Ubuntu"
                       color: "#ffffff"
                       smooth: true
                       Layout.alignment: Qt.AlignCenter
                       opacity: 0.50196078431373
              }

              Row{
                spacing:75
                ExclusiveGroup { id: denoiseRectGroup }
                RadioButton {
                    exclusiveGroup: denoiseRectGroup
                    id: rectEnable
                    text: "Enable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        see3camcu200.enableDisableDenoising(true)
                        root.setDenoiseFlag(true)
                    }
                    Keys.onReturnPressed: {
                        see3camcu200.enableDisableDenoising(true)
                        root.setDenoiseFlag(true)
                    }
                }
                RadioButton {
                    exclusiveGroup: denoiseRectGroup
                    id:rectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        see3camcu200.enableDisableDenoising(false)
                        root.setDenoiseFlag(false)
                    }
                    Keys.onReturnPressed: {
                        see3camcu200.enableDisableDenoising(false)
                        root.setDenoiseFlag(false)
                   }
                }
              }

               Text {
                   id: denoiseTitle
                   text: "--- Denoise Strength---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "This control provides the functionality to store the denoise and sharpness strength."
                       width: 200
                       opacity: 0
                   }
               }

               Row{
                   spacing: 35
                   enabled : rectEnable.checked? true : false
                   opacity : rectEnable.checked ? 1 : 0.1
                   Slider {
                       id: denoiseSlider
                       activeFocusOnPress: true
                       updateValueWhileDragging: false
                       width: 150
                       style:econSliderStyle

                       onValueChanged:  {
                           denoiseTextField.text = denoiseSlider.value.toFixed(1)
                           if(skipUpdateDenoiseStrength){
                               // Round the slider and TextField to three decimal places
                               adjustedDenoiseStrength = parseFloat((denoiseSlider.value).toFixed(1));

                               //Sending HID value to UVC
                               denoiseStrengthInt = adjustedDenoiseStrength

                               denoiseTextField.text = adjustedDenoiseStrength.toFixed(1)
                               see3camcu200.setDenoiseStrength(adjustedDenoiseStrength.toFixed(1))
                           }
                           skipUpdateDenoiseStrength = true
                       }
                   }
                   TextField {
                       id: denoiseTextField
                       font.pixelSize: 10
                       font.family: "Ubuntu"
                       smooth: true
                       horizontalAlignment: TextInput.AlignHCenter
                       style: econTextFieldStyle
                       validator: IntValidator {bottom: denoiseSlider.minimumValue; top: denoiseSlider.maximumValue}
                       onTextChanged: {
                           const match = denoiseTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                           if(match){
                               denoiseTextField.text = match[1];
                               denoiseSlider.value = denoiseTextField.text
                               root.getDenoiseStrength(denoiseSlider.value)
                           }
                       }
                   }
               }

               Text {
                   id: sharpnessTitle
                   text: "--- Sharpness Strength---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "This control provides the functionality to store the denoise and sharpness strength."
                       width: 200
                       opacity: 0
                   }
               }

               Row{
                   spacing: 35
                   enabled : rectEnable.checked? true : false
                   opacity : rectEnable.checked ? 1 : 0.1
                   Slider {
                       id: sharpnessSlider
                       activeFocusOnPress: true
                       updateValueWhileDragging: false
                       width: 150
                       style:econSliderStyle

                       onValueChanged:  {
                           sharpnessTextField.text = sharpnessSlider.value.toFixed(1)
                           if(skipUpdateSharpnessStrength){
                               // Round the slider and TextField to three decimal places
                               adjustedSharpnessStrength = parseFloat((sharpnessSlider.value).toFixed(1));

                               //Sending HID value to UVC
                               sharpnessStrengthInt = adjustedSharpnessStrength

                               sharpnessTextField.text = adjustedSharpnessStrength.toFixed(1)
                               see3camcu200.setSharpnessStrength(adjustedSharpnessStrength.toFixed(1))
                           }
                           skipUpdateSharpnessStrength = true
                       }
                   }
                   TextField {
                       id: sharpnessTextField
                       font.pixelSize: 10
                       font.family: "Ubuntu"
                       smooth: true
                       horizontalAlignment: TextInput.AlignHCenter
                       style: econTextFieldStyle
                       validator: IntValidator {bottom: sharpnessSlider.minimumValue; top: sharpnessSlider.maximumValue}
                       onTextChanged: {
                           const match = sharpnessTextField.text.match(/^(-?\d+(\.\d{0,1})?).*/);
                           if(match){
                               sharpnessTextField.text = match[1];
                               sharpnessSlider.value = sharpnessTextField.text
                               root.getSharpnessStrength(sharpnessSlider.value)
                           }
                       }
                   }
               }



               Text {
                   id: userDefinedTitle
                   text: "--- User Defined Configuration ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "This control provides the functionality to securely store all currently configured settings in the device's memory.
Following the configuration save, it is recommended to initiate a device reset to seamlessly load the saved configuration as the default."
                       width: 200
                       opacity: 0
                   }
               }

               Row{
                   Layout.alignment: Qt.AlignCenter
                   spacing: 8

                   Button {
                       id: userDefinedConfig
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Save Configuration"
                       style: econButtonStyle
                       onClicked:{
                           setButtonClicked = true
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.USER_DEFINED)
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.USER_DEFINED)
                       }
                   }
                   Button {
                       id: resetDevice
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Device Reset"
                       style: econButtonStyle
                       onClicked:{
                           see3camcu200.resetDevice()
                       }
                       Keys.onReturnPressed: {
                           see3camcu200.resetDevice()
                       }
                   }
               }

               ColumnLayout{
                   Layout.alignment: Qt.AlignCenter
                   Button {
                       id: defaultConfig
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Default Configuration"
                       tooltip: "This control is designed to restore the factory-provided default configuration.
Upon activation, the device will undergo an automatic reset to seamlessly load and apply the default configuration settings."
                       style: econButtonStyle
                       onClicked:{
                           setButtonClicked = true
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.DEFAULT)
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.DEFAULT)
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
                       id: ispFirmwareVersion
                       opacity: 1
                       action: ispVersion
                       text: "ISP Version"
                       activeFocusOnPress : true
                       tooltip: "Click to view the ISP firmware version"
                       style: econButtonStyle
                       Keys.onReturnPressed: {
                           see3camcu200.readISPFirmwareVersion()
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
                       tooltip: "Default button is used to set all the extension unit controls to their default values."
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

    SEE3CAM_CU200{
        id:see3camcu200

        onRGainPropertiesReceived: {
            skipUpdateRGainMode = false
            gainRSlider.minimumValue = minRGain
            gainRSlider.maximumValue = maxRGain
            gainRSlider.stepSize = rGainStepValue

            currentRGain = parseFloat((currentRGain).toFixed(3));
            gainRSlider.value   = currentRGain
            gainRTextField.text = currentRGain

            skipUpdateRGainMode = true
        }

        onBGainPropertiesReceived: {
            skipUpdateBGainMode = false
            gainBSlider.minimumValue = minBGain
            gainBSlider.maximumValue = maxBGain
            gainBSlider.stepSize = bGainStepValue

            currentBGain = parseFloat((currentBGain).toFixed(3));
            gainBSlider.value   = currentBGain
            gainBTextField.text = currentBGain

            skipUpdateBGainMode = true
        }

        onCurrentRrValuesReceived: {

            spinBoxRr.minimumValue = spinBoxMin
            spinBoxRr.maximumValue = spinBoxMax
            spinBoxRr.stepSize     = spinBoxStepSize
            spinBoxRr.value        = currentRr
        }
        onCurrentRgValuesReceived: {
            spinBoxRg.minimumValue = spinBoxMin
            spinBoxRg.maximumValue = spinBoxMax
            spinBoxRg.stepSize     = spinBoxStepSize
            spinBoxRg.value        = currentRg
        }
        onCurrentRbValuesReceived: {
            spinBoxRb.minimumValue = spinBoxMin
            spinBoxRb.maximumValue = spinBoxMax
            spinBoxRb.stepSize     = spinBoxStepSize
            spinBoxRb.value        = currentRb
        }

        onCurrentGrValuesReceived:{
            spinBoxGr.minimumValue = spinBoxMin
            spinBoxGr.maximumValue = spinBoxMax
            spinBoxGr.stepSize     = spinBoxStepSize
            spinBoxGr.value        = currentGr
        }
        onCurrentGgValuesReceived:{
            spinBoxGg.minimumValue = spinBoxMin
            spinBoxGg.maximumValue = spinBoxMax
            spinBoxGg.stepSize     = spinBoxStepSize
            spinBoxGg.value        = currentGg
        }
        onCurrentGbValuesReceived:{
            spinBoxGb.minimumValue = spinBoxMin
            spinBoxGb.maximumValue = spinBoxMax
            spinBoxGb.stepSize     = spinBoxStepSize
            spinBoxGb.value        = currentGb
        }

        onCurrentBrValuesReceived:{
            spinBoxBr.minimumValue = spinBoxMin
            spinBoxBr.maximumValue = spinBoxMax
            spinBoxBr.stepSize     = spinBoxStepSize
            spinBoxBr.value        = currentBr
        }
        onCurrentBgValuesReceived:{
            spinBoxBg.minimumValue = spinBoxMin
            spinBoxBg.maximumValue = spinBoxMax
            spinBoxBg.stepSize     = spinBoxStepSize
            spinBoxBg.value        = currentBg
        }
        onCurrentBbValuesReceived:{
            spinBoxBb.minimumValue = spinBoxMin
            spinBoxBb.maximumValue = spinBoxMax
            spinBoxBb.stepSize     = spinBoxStepSize
            spinBoxBb.value        = currentBb
        }

        onCurrentBlackLevelReceived: {
            blackLevelSlider.value = currentBlackLevel
        }
        onMinBlackLevelReceived:{
            blackLevelSlider.minimumValue = minBlackLevel
        }
        onMaxBlackLevelReceived:{
            blackLevelSlider.maximumValue = maxBlackLevel
        }
        onBlackLevelStepValueReceived:{
            blackLevelSlider.stepSize = blackLevelStepValue
        }

        onBrightnessPropertiesReceived: {
            skipUpdateBrightness = false

            brightnessSlider.minimumValue = minBrightness
            brightnessSlider.maximumValue = maxBrightness
            brightnessSlider.stepSize = stepValue

            currentBrightness = parseFloat((currentBrightness).toFixed(3));
            brightnessSlider.value = currentBrightness
            brightnessTextField.text = currentBrightness

            adjustedBrightness = parseFloat((brightnessSlider.value).toFixed(3));

            //Sending HID value to UVC
            brightnessInt = adjustedBrightness * 200;
            root.sendBrightnessToUVC(brightnessInt)

            skipUpdateBrightness = true
        }

        onDenoiseFlagValue :{
            if(flag){
                rectEnable.checked = true
            } else{
                rectDisable.checked = true
            }
            root.setDenoiseFlag(flag)
        }

        onDenoiseStrengthValuesReceived: {
            skipUpdateDenoiseStrength = false

            denoiseSlider.minimumValue = min
            denoiseSlider.maximumValue = max
            denoiseSlider.stepSize = step

            current = parseFloat((current).toFixed(1));
            denoiseSlider.value = current
            denoiseTextField.text = current
            root.getDenoiseStrength(denoiseSlider.value)

            adjustedDenoiseStrength = parseFloat((denoiseSlider.value).toFixed(1));

            skipUpdateDenoiseStrength = true
        }

        onSharpnessStrengthValuesReceived: {
            skipUpdateSharpnessStrength = false

            sharpnessSlider.minimumValue = min
            sharpnessSlider.maximumValue = max
            sharpnessSlider.stepSize = step

            current = parseFloat((current).toFixed(1));
            sharpnessSlider.value = current
            sharpnessTextField.text = current

            root.getSharpnessStrength(sharpnessSlider.value)
            adjustedSharpnessStrength = parseFloat((sharpnessSlider.value).toFixed(1));

            skipUpdateSharpnessStrength = true
        }


        onCurrentContrastReceived: {
            skipUpdateContrast = false
            contrastSlider.value = currentContrast
            root.sendContrastToUVC(contrastSlider.value)
            skipUpdateContrast = true
        }
        onMinContrastReceived: {
            contrastSlider.minimumValue = minContrast
        }
        onMaxContrastReceived: {
            contrastSlider.maximumValue = maxContrast
        }
        onContrastStepValueReceived: {
            contrastSlider.stepSize = stepValue
        }

        onSaturationPropertiesReceived: {
            skipUpdateSaturation = false
            saturationSlider.minimumValue = minSaturation
            saturationSlider.maximumValue = maxSaturation
            saturationSlider.stepSize = stepValue

            currentSaturation = parseFloat((currentSaturation).toFixed(3));
            saturationSlider.value = currentSaturation.toFixed(3)
            saturationTextField.text = currentSaturation.toFixed(3)

            adjustedSaturation = parseFloat((saturationSlider.value).toFixed(3));

            //Sending HID value to UVC
            saturationInt = adjustedSaturation * 200
            root.sendSaturationToUVC(saturationInt)

            skipUpdateSaturation = true
        }

        onColorTemperatureReceived: {
            skipUpdateColorTemperature = false

            colorTempSlider.minimumValue = min
            colorTempSlider.maximumValue = max
            colorTempSlider.stepSize     = stepValue
            colorTempSlider.value        = colorTemp

            root.sendColorTemperatureToUVC(colorTemp)

            skipUpdateColorTemperature = true
        }

        onGammaPropertiesReceived: {
            skipUpdateGammaCorrection = false

            gammaCorrectionSlider.minimumValue = minGamma
            gammaCorrectionSlider.maximumValue = maxGamma
            gammaCorrectionSlider.stepSize = stepValue

            currentGamma = parseFloat((currentGamma).toFixed(1));

            gammaCorrectionSlider.value = currentGamma.toFixed(1)
            gammaCorrectionTextField.text = currentGamma.toFixed(1)

            adjustedGammaCorrection = parseFloat((gammaCorrectionSlider.value).toFixed(1));

            //Sending HID value to UVC
            gammaInt = adjustedGammaCorrection * 10
            root.sendGammaToUVC(gammaInt)

            skipUpdateGammaCorrection = true
        }

        onExposurePropertiesReceived: {
            currentExposureMode(mode, autoFeature, manualValue)
        }

        onCameraModeReceived:{
            getCameraModeReceived(cameraMode)
        }

        onStrobeModeReceived: {
            currentStrobeMode(strobeMode)
        }

        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorModeValues)
        }

        onAutoGainUpperLimitReceived:{
            skipUpdateExposureUpperLimit = false

            gainUpperLimitSlider.minimumValue = min
            gainUpperLimitSlider.maximumValue = max
            gainUpperLimitSlider.stepSize     = step
            gainUpperLimitSlider.value        = current

            skipUpdateExposureUpperLimit = true
        }

        onExposureCompensationReceived:{
            skipUpdateExposureCompensation = false
            expCompensationTextField.text = exposure
            skipUpdateExposureCompensation = true
        }

        onRoiAutoExpModeReceived: {
            currentROIAutoExposureMode(roiMode, winSize)
        }

        onTargetBrightnessPropertiesReceived: {
            skipUpdateTargetBrightness = false

            targetBrightness.minimumValue = min
            targetBrightness.maximumValue = max
            targetBrightness.stepSize = stepValue
            targetBrightness.value = current

            skipUpdateTargetBrightness = true
        }

        onAntiFlickerModeReceived: {
            getAntiFlickerModes(antiFlicker)
        }

        onWhiteBalancePropertiesReceived:{
            if(mode === SEE3CAM_CU200.AUTO_WB) {
                autoWb.checked = true
                root.sendWhiteBalanceModeToUVC(true, false)
            } else if(mode === SEE3CAM_CU200.MANUAL_WB) {
                manualWb.checked = true

                if(manualFeature === SEE3CAM_CU200.COLOR_TEMPERATURE) {
                    root.sendWhiteBalanceModeToUVC(false, true)
                }else{
                    root.sendWhiteBalanceModeToUVC(false, false)
                }
            }

            if(autoFeature === SEE3CAM_CU200.WB_CONTINIOUS) {
                wbContinious.checked = true
                wbSingleShotBtnClicked = false
            } else if(autoFeature === SEE3CAM_CU200.WB_SINGLE_SHOT) {
                wbSingleShot.checked = true
                wbSingleShotBtnClicked = true
            }

            if(manualFeature === SEE3CAM_CU200.PRESET) {
                wbPreset.checked = true
            } else if(manualFeature === SEE3CAM_CU200.COLOR_TEMPERATURE) {
                colorTemperature.checked = true
                see3camcu200.getColorTemperature()
            } else if(manualFeature === SEE3CAM_CU200.PRO_MODE) {
                proMode.checked = true
                see3camcu200.getRBGain()
                see3camcu200.getColorCorrectionMatrix()
            }
        }

        onWhiteBalancePresetModeReceived: {
            currentWbPresetModeReceived(presetMode)
        }

        //For Read Statistics
        onCurrentGainProperties:{

            currentRGainValue = parseFloat((rGain).toFixed(3));
            currentBGainValue = parseFloat((bGain).toFixed(3));

            gainStatTextField.text  = autoGain
            bGainStatTextField.text = currentBGainValue
            rGainStatTextField.text = currentRGainValue
        }

        onCurrentAutoExposure: {
            exposureStatistics.text = exposure
        }

        onCurrentRMatrixElements:{
            autoRr = parseFloat((Rr).toFixed(3));
            autoRg = parseFloat((Rg).toFixed(3));
            autoRb = parseFloat((Rb).toFixed(3));

            readRr.text = autoRr
            readRg.text = autoRg
            readRb.text = autoRb
        }

        onCurrentGMatrixElements:{
            autoGr = parseFloat((Gr).toFixed(3));
            autoGg = parseFloat((Gg).toFixed(3));
            autoGb = parseFloat((Gb).toFixed(3));

            readGr.text = autoGr
            readGg.text = autoGg
            readGb.text = autoGb
        }

        onCurrentBMatrixElements:{
            autoBr = parseFloat((Br).toFixed(3));
            autoBg = parseFloat((Bg).toFixed(3));
            autoBb = parseFloat((Bb).toFixed(3));

            readBr.text = autoBr
            readBg.text = autoBg
            readBb.text = autoBb
        }

        //For Message Box
        onIndicateCommandStatus:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
            }
        }

        onIndicateExposureValueRangeFailure:{
            if(setButtonClicked){
                setButtonClicked = false

                displayMessageBox(title, text)
                see3camcu200.getExposure()
            }
        }

        onIndicateExposureCompensationRangeFailure:{
            if(setButtonClicked){
                setButtonClicked = false

                displayMessageBox(title, text)
                see3camcu200.getExposureCompensation()
            }
        }

        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }
    }

    function setROIAutoExposure(){
        if(expRoiFull.checked == true){
            see3camcu200.setROIAutoExposure(SEE3CAM_CU200.FULL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)

            //To disable comboBox in full roi mode
            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.opacity = 0.1
        }else if(expRoiManual.checked == true){
            see3camcu200.setROIAutoExposure(SEE3CAM_CU200.MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)

            //To disable comboBox in full roi mode
            autoExpoWinSizeCombo.enabled = true
            autoExpoWinSizeCombo.opacity = 1
        }
    }

    function currentFlipMirrorMode(mode) {
        switch(mode) {
            case SEE3CAM_CU200.NORMAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = false
                root.getFlipStatus(false, false)
                break;
            case SEE3CAM_CU200.VERTICAL:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = false
                root.getFlipStatus(false, true)
                break;
            case SEE3CAM_CU200.HORIZONTAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = true
                root.getFlipStatus(true, false)
                break;
            case SEE3CAM_CU200.ROTATE_180:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = true
                root.getFlipStatus(false, false)
                break;
        }
    }

    function currentROIAutoExposureMode(roiMode, winSize) {
        switch(roiMode) {
            case SEE3CAM_CU200.FULL_ROI:
                expRoiFull.checked = true
                expRoiFull.enabled = true

                //To disable comboBox in full roi mode
                autoExpoWinSizeCombo.enabled = false
                autoExpoWinSizeCombo.opacity = 0.1
                break
            case SEE3CAM_CU200.MANUAL_ROI:
                skipUpdateUIOnExpWindowSize = false

                expRoiManual.enabled = true
                expRoiManual.checked = true

                //To enable comboBox in manual mode
                autoExpoWinSizeCombo.enabled = true
                autoExpoWinSizeCombo.opacity = 1
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
        }
    }

    function getAntiFlickerModes(antiFlicker) {
        switch(antiFlicker) {
            case SEE3CAM_CU200.MODE_50Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case SEE3CAM_CU200.MODE_60Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
            case SEE3CAM_CU200.MODE_DISABLE:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 2
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }


    function sendFlipStatus() {
        if((flipCtrlVertical.checked) && (flipCtrlHorizotal.checked)) {
            root.getFlipStatus(true, true)
        }
        else if(!(flipCtrlVertical.checked) && (flipCtrlHorizotal.checked)) {
            root.getFlipStatus(true, false)
        }
        else if((flipCtrlVertical.checked) && !(flipCtrlHorizotal.checked)) {
            root.getFlipStatus(false, true)
        }
        else if(!(flipCtrlVertical.checked) && !(flipCtrlHorizotal.checked)) {
            root.getFlipStatus(false, false)
        }
    }


    function getCameraModeReceived(mode) {
        if(mode == SEE3CAM_CU200.MASTER_MODE) {
            masterMode.checked = true

            root.startUpdatePreviewInMasterMode()
            root.checkForTriggerMode(false)
            root.videoRecordBtnEnable(true)
            root.captureBtnEnable(true)
        }
        else if(mode = SEE3CAM_CU200.TRIGGER_MODE) {
            triggerMode.checked = true

            root.stopUpdatePreviewInTriggerMode()
            root.checkForTriggerMode(true)
            root.videoRecordBtnEnable(false)
            root.captureBtnEnable(false)
        }
    }

    function setMasterMode() {
        see3camcu200.setCameraMode(SEE3CAM_CU200.MASTER_MODE)

        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function setTriggerMode() {
        see3camcu200.setCameraMode(SEE3CAM_CU200.TRIGGER_MODE)

        root.stopUpdatePreviewInTriggerMode()
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function currentStrobeMode(mode) {
        switch(mode) {
            case SEE3CAM_CU200.STROBE_OFF:
                strobeOff.checked = true
                strobeOn.checked  = false
                break;
            case SEE3CAM_CU200.STROBE_ON:
                strobeOff.checked = false
                strobeOn.checked  = true
                break;
        }
    }

    function currentGainMode(mode, autoFeature){
        if(mode === SEE3CAM_CU200.AUTO_GAIN) {
            autoGain.checked = true
        } else if(mode === SEE3CAM_CU200.MANUAL_GAIN) {
            manualGain.checked = true
        }
    }

    function currentWbPresetModeReceived(presetMode) {
        switch(presetMode) {
            case SEE3CAM_CU200.HZ_2300K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 0
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.A_2800K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 1
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.U30_3000K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 2
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.TL84_4000K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 3
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.CWF_4100K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 4
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.D50_5000K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 5
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.D60_6000K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 6
                skipUpdateUIOnWBPresetMode = true
                break
            case SEE3CAM_CU200.D65_6500K:
                skipUpdateUIOnWBPresetMode = false
                wbPresetCombo.currentIndex = 7
                skipUpdateUIOnWBPresetMode = true
                break
         }
    }

    function enableDisableAutoWhiteBalanceControls(autoWhiteBalanceSelect){
        if(autoWhiteBalanceSelect){
            autoWb.checked   = true
            manualWb.checked = false
        }else {
            autoWb.checked   = false
            manualWb.checked = true
        }
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){

            uvcAutoExpSelected = autoExposureSelect

            manualExposure.checked = false
            autoExposure.checked   = true

            expRoiManual.enabled   = true
            expRoiFull.enabled     = true

            expRoiManual.opacity   = 1
            expRoiFull.opacity     = 1

            if(expRoiManual.checked){
                autoExpoWinSizeCombo.enabled = true
                autoExpoWinSizeCombo.opacity = 1
            }else if(expRoiFull.checked){
                autoExpoWinSizeCombo.enabled = false
                autoExpoWinSizeCombo.opacity = 0.1
            }
        }else{
            uvcAutoExpSelected = autoExposureSelect

            manualExposure.checked = true
            autoExposure.checked   = false

            expRoiManual.enabled   = false
            expRoiFull.enabled     = false

            expRoiManual.opacity   = 0.1
            expRoiFull.opacity     = 0.1

            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.opacity = 0.1
        }
    }

    function currentExposureMode(mode, autoFeature, manualValue) {
        if(mode === SEE3CAM_CU200.AUTO_EXPOSURE){
            autoExposure.checked   = true
            manualExposure.checked = false
        }else if(mode === SEE3CAM_CU200.MANUAL_EXPOSURE){
            manualExposure.checked = true
            autoExposure.checked   = false
        }

        //Auto Exposure Features
        if(autoFeature === SEE3CAM_CU200.CONTINIOUS_EXPOSURE){
            exposureContinious.checked = true
            expSingleShotBtnClicked    = false
        }
        else if(autoFeature === SEE3CAM_CU200.SINGLE_SHOT_EXPOUSRE){
            exposureSingleShot.checked = true
            expSingleShotBtnClicked    = true
        }

        //Manual Exposure Values
        manualExpTextField.text = manualValue

        if(manualExpTextField.text <= 1000000){
            exposureInt = manualExpTextField.text / 100
            root.sendExposureToUVC(exposureInt)
        }
        else{
            root.sendExposureToUVC(100000)
        }
    }

    function sendExposureStatus(){
        if(autoExposure.checked == true){
            root.sendExposureStatusToUVC(true, 0)
        }else if(manualExposure.checked == true){
            root.sendExposureStatusToUVC(false, (manualExpTextField.text / 100))
        }
    }

    function setExposureProperties() {
        if(autoExposure.checked == true){
            root.sendExposureStatusToUVC(true, 0)
            if(exposureContinious.checked == true) {
                see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.CONTINIOUS_EXPOSURE, manualExpTextField.text)
            }
            else if(exposureSingleShot.checked == true) {
                see3camcu200.setExposure(SEE3CAM_CU200.AUTO_EXPOSURE,SEE3CAM_CU200.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
            }
        }else if(manualExposure.checked == true){
            root.sendExposureStatusToUVC(false, (manualExpTextField.text / 100))
            see3camcu200.setExposure(SEE3CAM_CU200.MANUAL_EXPOSURE, 0, manualExpTextField.text)
        }
    }

    function setAutoWhiteBalance() {
        if(wbContinious.checked == true) {
            wbSingleShotBtnClicked = false
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.AUTO_WB, SEE3CAM_CU200.WB_CONTINIOUS, 0, 0)
        } else if(wbSingleShot.checked == true) {
            //if(!wbSingleShotBtnClicked){
                wbSingleShotBtnClicked = true
                see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.AUTO_WB, SEE3CAM_CU200.WB_SINGLE_SHOT, 0, 0)
            //}
        }
        root.sendWhiteBalanceModeToUVC(true, false)
    }

    function setManualWhiteBalance() {

        if(wbPreset.checked == true) {
            see3camcu200.getWhiteBalanceMode()
            setWbPresetMode()
            root.sendWhiteBalanceModeToUVC(false, false)
        } else if(colorTemperature.checked == true) {
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.COLOR_TEMPERATURE, wbPresetCombo.currentText.toString())

            //Send get Request for color temperature
            see3camcu200.getColorTemperature()
            see3camcu200.setColorTemperature(colorTempSlider.value)
            root.sendWhiteBalanceModeToUVC(false, true)

        } else if(proMode.checked == true) {
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRO_MODE, wbPresetCombo.currentText.toString())

            //Send get Request for colorCorrectionMatrix & RB Gain
            see3camcu200.getColorCorrectionMatrix()
            see3camcu200.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)

            see3camcu200.getRBGain()
            see3camcu200.setRBGain(gainRSlider.value, gainBSlider.value)
            root.sendWhiteBalanceModeToUVC(false, false)
        }
    }

    function setWbPresetMode() {
        if(wbPresetCombo.currentText.toString() === "HZ(2300K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.HZ_2300K)
        } else if(wbPresetCombo.currentText.toString() === "A(2800K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.A_2800K)
        } else if(wbPresetCombo.currentText.toString() === "U30(3000K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.U30_3000K)
        } else if(wbPresetCombo.currentText.toString() === "TL84(4000K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.TL84_4000K)
        } else if(wbPresetCombo.currentText.toString() === "CWF(4100K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.CWF_4100K)
        } else if(wbPresetCombo.currentText.toString() === "D50(5000K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.D50_5000K)
        } else if(wbPresetCombo.currentText.toString() === "D60(6000K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.D60_6000K)
        } else if(wbPresetCombo.currentText.toString() === "D65(6500K)"){
            see3camcu200.setWhiteBalanceMode(SEE3CAM_CU200.MANUAL_WB, 0, SEE3CAM_CU200.PRESET, SEE3CAM_CU200.D65_6500K)
        }

        see3camcu200.getRBGain()
        see3camcu200.getWhiteBalanceMode()
    }

    function setAntiFlicker() {
        if(antiFlickerCombo.currentIndex === 0)
            see3camcu200.setAntiFlickerMode(SEE3CAM_CU200.MODE_50Hz)
        else if(antiFlickerCombo.currentIndex === 1)
            see3camcu200.setAntiFlickerMode(SEE3CAM_CU200.MODE_60Hz)
        else if(antiFlickerCombo.currentIndex === 2)
            see3camcu200.setAntiFlickerMode(SEE3CAM_CU200.MODE_DISABLE)
    }


    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function displayMessageBox(title, text) {
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function getISPFirmwareVersion(){
        see3camcu200.readISPFirmwareVersion()
        messageDialog.open()
    }

    function setToDefaultValues() {
        defaultValue.enabled = false
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()

        if(see3camcu200.setToDefault()) {
            getCurrentValuesFromCamera()
            //To send Exposure properties when set default is clicked
            sendExposureStatus()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera() {
        getCameraSettingsTimer.start()
        see3camcu200.getRBGain()
        see3camcu200.getColorCorrectionMatrix()
        see3camcu200.getBlackLevel()
        see3camcu200.getBrightness()
        see3camcu200.getContrast()
        see3camcu200.getSaturation()
        see3camcu200.getColorTemperature()
        see3camcu200.getGammaCorrection()
        see3camcu200.getExposure()
        see3camcu200.getCameraMode()
        see3camcu200.getStrobeMode()
        see3camcu200.getOrientation()
        see3camcu200.getAutoExpROIModeAndWindowSize()
        see3camcu200.getAntiFlickerMode()
        see3camcu200.getWhiteBalanceMode()
        see3camcu200.getAutoGainUpperLimit()
        see3camcu200.getDenoiseFlag()
        see3camcu200.getDenoiseStrength()
        see3camcu200.getSharpnessStrength()
    }
    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
}
