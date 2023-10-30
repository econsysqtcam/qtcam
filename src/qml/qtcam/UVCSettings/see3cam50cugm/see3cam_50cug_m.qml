/*
 * see3cam_50cug_m.qml -- extension settings for other cameras
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

    property int minExpInSeconds: 0
    property int maxExpInSeconds: 14
    property int minExpInMilliSeconds: 0
    property int maxExpInMilliSeconds: 999
    property int minExpInMicroSeconds: 0
    property int maxExpInMicroSeconds: 999

    property bool skipUpdateUltraShortExposure  : false
    property bool skipUpdateBlackLevelMode      : false
    property bool skipUpdateTargetBrightness    : false
    property bool skipUpdateLowerLimitSlider    : false
    property bool skipUpdateUpperLimitSlider    : false
    property bool skipUpdateUIOnAntiFlickerMode : false
    property bool skipUpdateGainMode            : false

    property bool setButtonClicked: false


    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIFlickerCtrl:false
    property int  triggerCtrl
    property int  convertedExposure : 0
    property int  secondInt : 0
    property int  milliSecondInt : 0
    property int  microSecondInt : 0

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


    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 1000
        onTriggered: {
            stop()
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
        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        //Signals getting values from UVC & set its values to the HID controls
        onSendGainValueToHID:{
            gainSlider.value = gainHid
        }
        onExposureComponentsFromUVC:{
            expInSecondsTextField.text  = seconds
            expInMilliSecTextField.text = milliSeconds
            expInMicroSecTextField.text = microSeconds
        }
    }

    ScrollView
    {
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle

        Item{
           height: 3200
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
                           ListElement { text: "Software control" }
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
                    id: gainModeTitle
                    text: "--- Gain ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 45
                    ExclusiveGroup { id: gainModeGroup }
                    Layout.alignment: Qt.AlignCenter
                    RadioButton {
                        id: autoGainMode
                        exclusiveGroup: gainModeGroup
                        checked: false
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            setAutoGain()
                        }
                        Keys.onReturnPressed: {
                            setAutoGain()
                        }
                    }

                    RadioButton {
                        id: manualGainMode
                        exclusiveGroup: gainModeGroup
                        checked: false
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            setManualGain()
                        }
                        Keys.onReturnPressed: {
                            setManualGain()
                        }
                    }
                }

                Text {
                    id: autoGainFeatureTitle
                    text: "--- Auto Gain Features ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 10
                    ExclusiveGroup { id: autoGainFeatureGroup }
                    RadioButton {
                        id: gainContinuous
                        exclusiveGroup: autoGainFeatureGroup
                        checked: false
                        text: "Continuous"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.CONTINIOUS_GAIN, gainSlider.value)
                        }
                        Keys.onReturnPressed: {
                            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.CONTINIOUS_GAIN, gainSlider.value)
                        }
                    }
                }

                Row{
                    spacing: 25
                    RadioButton {
                        id: gainSingleShot
                        exclusiveGroup: autoGainFeatureGroup
                        checked: false
                        text: "Single Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_GAIN, gainSlider.value)
                        }
                        Keys.onReturnPressed: {
                            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_GAIN, gainSlider.value)
                        }
                    }

                    Button {
                        id: gainTriggerBtn
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (autoGainMode.checked && gainSingleShot.checked) ? true : false
                        opacity: (autoGainMode.checked && gainSingleShot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        onClicked: {
                            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_GAIN_TRIGGER, gainSlider.value)
                        }
                        Keys.onReturnPressed: {
                            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_GAIN_TRIGGER, gainSlider.value)
                        }
                    }
                }

                Text {
                    id: manualGainTitle
                    text: "--- Manual Gain ---"
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
                        id: gainSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        onValueChanged:  {
                            gainTextField.text = gainSlider.value

                            //Sending HID value to UVC
                            root.getGainValueFromHID(gainSlider.value)

                            if(skipUpdateGainMode){
                                see3cam50cugm.setGainMode(SEE3CAM_50CUGM.MANUAL_GAIN_MODE, 0, gainSlider.value)
                            }
                            skipUpdateGainMode = true
                        }
                    }
                    TextField
                    {
                        id: gainTextField
                        text: gainSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: gainSlider.minimumValue; top: gainSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                gainSlider.value = gainTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: gainLowerLimit
                    text: "--- Gain Lower Limit ---"
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
                        id: gainLowerLimitSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            gainLowerLimitTextField.text = gainLowerLimitSlider.value
                            if(skipUpdateLowerLimitSlider){
                                see3cam50cugm.setAutoGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
                            }
                            skipUpdateLowerLimitSlider = true
                        }
                    }
                    TextField {
                        id: gainLowerLimitTextField
                        text: gainLowerLimitSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: gainLowerLimitSlider.minimumValue; top: gainLowerLimitSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                gainLowerLimitSlider.value = gainLowerLimitTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: gainUpperLimit
                    text: "--- Gain Upper Limit ---"
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
                        id: gainUpperLimitSlider
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            gainUpperLimitTextField.text = gainUpperLimitSlider.value
                            if(skipUpdateUpperLimitSlider){
                                see3cam50cugm.setAutoGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
                            }
                            skipUpdateUpperLimitSlider = true
                        }
                    }
                    TextField {
                        id: gainUpperLimitTextField
                        text: gainLowerLimitSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: gainUpperLimitSlider.minimumValue; top: gainUpperLimitSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                gainUpperLimitSlider.value = gainUpperLimitTextField.text
                            }
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

                    Text {
                        id: exposureModeTitle
                        text: "--- Exposure ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    Row{
                        spacing: 45
                        ExclusiveGroup { id: exposureModeGroup }
                        Layout.alignment: Qt.AlignCenter
                        RadioButton {
                            id: autoExpMode
                            exclusiveGroup: exposureModeGroup
                            checked: false
                            text: "Auto"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                root.sendExposureStatusToUVC(1 , convertedExposure)

                                setAutoExposure()
                            }
                            Keys.onReturnPressed: {
                                root.sendExposureStatusToUVC(1 , convertedExposure)
                                setAutoExposure()
                            }
                        }

                        RadioButton {
                            id: manualExpMode
                            exclusiveGroup: exposureModeGroup
                            checked: false
                            text: "Manual"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                //Sending exposure value from HID to UVC
                                sendConvertedExpToUVC()
                                setManualExposure()
                            }
                            Keys.onReturnPressed: {
                                //Sending exposure value from HID to UVC
                                sendConvertedExpToUVC()

                                setManualExposure()
                            }
                        }
                    }

                    Text {
                        id: autoExpFeatureTitle
                        text: "--- Auto Exposure Features ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    Row{
                        spacing: 10
                        ExclusiveGroup { id: autoExpFeatureGroup }
                        RadioButton {
                            id: expContinuous
                            exclusiveGroup: autoExpFeatureGroup
                            checked: false
                            text: "Continuous"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.CONTINIOUS_EXPOSURE, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.CONTINIOUS_EXPOSURE, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            }
                        }
                    }

                    Row{
                        spacing: 25
                        RadioButton {
                            id: expSingleShot
                            exclusiveGroup: autoExpFeatureGroup
                            checked: false
                            text: "Single Shot"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_EXPOSURE, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_EXPOSURE, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            }
                        }

                        Button {
                            id: expTriggerBtn
                            activeFocusOnPress : true
                            text: "Trigger"
                            style: econButtonStyle
                            enabled: (expSingleShot.checked && autoExpMode.checked) ? true : false
                            opacity: (expSingleShot.checked && autoExpMode.checked) ? 1 : 0.1
                            implicitHeight: 25
                            implicitWidth: 120
                            onClicked: {
                                see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            }
                        }
                    }

                    Text {
                        id: manualExpTitle
                        text: "--- Manual Exposure Fields ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 0
                        Layout.alignment: Qt.AlignCenter

                        Row
                        {
                            Layout.alignment: Qt.AlignCenter
                            spacing: 10
                            TextField
                            {
                                id: expInSecondsTextField
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle

                                // Units Name
                                Text {
                                    text: "   sec"
                                    anchors.top: expInSecondsTextField.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInSeconds; top: maxExpInSeconds}
                            }


                            TextField
                            {
                                id: expInMilliSecTextField
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Units Name
                                Text {
                                    text: "   ms"
                                    anchors.top: expInMilliSecTextField.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                }
                                validator: IntValidator {bottom: minExpInMilliSeconds; top: maxExpInMilliSeconds}
                            }

                            TextField
                            {
                                id: expInMicroSecTextField
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle

                                // Unit Name
                                Text {
                                    text: "    us"
                                    anchors.top: expInMicroSecTextField.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInMicroSeconds ; top: maxExpInMicroSeconds}
                            }

                            Button {
                                id: manualExpSetBtn
                                activeFocusOnPress : true
                                text: "Set"
                                style: econButtonStyle
                                enabled: (manualExpMode.enabled && manualExpMode.checked) ? true : false
                                opacity: (manualExpMode.enabled && manualExpMode.checked) ? 1 : 0.1
                                implicitHeight: 25
                                implicitWidth: 50
                                onClicked: {
                                    //Sending exposure value from HID to UVC
                                    sendConvertedExpToUVC()

                                    see3cam50cugm.setExposure(SEE3CAM_50CUGM.MANUAL_EXPOSURE_MODE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                                }
                                Keys.onReturnPressed: {
                                    //Sending exposure value from HID to UVC
                                    sendConvertedExpToUVC()

                                    see3cam50cugm.setExposure(SEE3CAM_50CUGM.MANUAL_EXPOSURE_MODE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                                }
                            }
                        }
                    }

                    Text {
                        id: lowerLimitMode
                        text: "--- Exposure Lower Limit ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 1
                        Layout.alignment: Qt.AlignCenter
                        Row
                        {
                            Layout.alignment: Qt.AlignCenter
                            spacing: 20
                            TextField
                            {
                                id: lowerLimitSeconds
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Unit Name
                                Text {
                                    text: "   sec"
                                    anchors.top: lowerLimitSeconds.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInSeconds; top: maxExpInSeconds}
                            }

                            TextField
                            {
                                id: lowerLimitMilliSec
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Unit Name
                                Text {
                                    text: "    ms"
                                    anchors.top: lowerLimitMilliSec.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInMilliSeconds; top: maxExpInMilliSeconds}
                            }

                            TextField
                            {
                                id: lowerLimitMicroSec
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Unit Name
                                Text {
                                    text: "    us"
                                    anchors.top: lowerLimitMicroSec.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInMicroSeconds ; top: maxExpInMicroSeconds}
                            }
                        }
                    }


                    Text {
                        id: upperLimitMode
                        text: "--- Exposure Upper Limit ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 1
                        Layout.alignment: Qt.AlignCenter

                        Row
                        {
                            Layout.alignment: Qt.AlignCenter
                            spacing: 20
                            TextField
                            {
                                id: upperLimitSeconds
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Unit Name
                                Text {
                                    text: "   sec"
                                    anchors.top: upperLimitSeconds.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInSeconds; top: maxExpInSeconds}
                            }

                            TextField
                            {
                                id: upperLimitMilliSec
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Unit Name
                                Text {
                                    text: "    ms"
                                    anchors.top: upperLimitMilliSec.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInMilliSeconds; top: maxExpInMilliSeconds}
                            }

                            TextField
                            {
                                id: upperLimitMicroSec
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                // Unit Name
                                Text {
                                    text: "    us"
                                    anchors.top: upperLimitMicroSec.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                validator: IntValidator {bottom: minExpInMicroSeconds ; top: maxExpInMicroSeconds}
                            }
                        }
                    }

                    Row{
                        Layout.alignment: Qt.AlignCenter
                        Button {
                            id: limitSetBtn
                            activeFocusOnPress : true
                            text: "Set Limit"
                            style: econButtonStyle
                            enabled: (autoExpMode.checked && autoExpMode.enabled) ? true : false
                            opacity: (autoExpMode.checked && autoExpMode.enabled) ? 1 : 0.1
                            implicitHeight: 25
                            implicitWidth: 80
                            onClicked: {
                                see3cam50cugm.setAutoExposureUpperLimit(upperLimitSeconds.text, upperLimitMilliSec.text, upperLimitMicroSec.text)
                                see3cam50cugm.setAutoExposureLowerLimit(lowerLimitSeconds.text, lowerLimitMilliSec.text, lowerLimitMicroSec.text)
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.setAutoExposureUpperLimit(upperLimitSeconds.text, upperLimitMilliSec.text, upperLimitMicroSec.text)
                                see3cam50cugm.setAutoExposureLowerLimit(lowerLimitSeconds.text, lowerLimitMilliSec.text, lowerLimitMicroSec.text)
                            }
                        }
                    }

                    Text
                    {
                        id: brightnessText
                        text: "--- Target Brightness ---"
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
                            id: targetBrightness
                            activeFocusOnPress: true
                            updateValueWhileDragging: false
                            width: 150
                            style:econSliderStyle
                            enabled: (manualExpMode.checked || manualGainMode.checked) ? true : false
                            opacity: (manualExpMode.checked || manualGainMode.checked) ? 1 : 0.1
                            onValueChanged:  {
                                brightnessTextField.text = targetBrightness.value
                                if(skipUpdateTargetBrightness){
                                    see3cam50cugm.setTargetBrightness(targetBrightness.value)
                                }
                                skipUpdateTargetBrightness = true
                            }
                        }
                        TextField {
                            id: brightnessTextField
                            text: targetBrightness.value
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (manualExpMode.checked || manualGainMode.checked) ? true : false
                            opacity: (manualExpMode.checked || manualGainMode.checked) ? 1 : 0.1
                            validator: IntValidator {bottom: targetBrightness.minimumValue; top: targetBrightness.maximumValue}
                            onTextChanged: {
                                if(text.length > 0){
                                    targetBrightness.value = brightnessTextField.text
                                }
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

                    Text {
                        id: frequency
                        text: "Frequency :"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        enabled: (autoExpMode.enabled && autoExpMode.checked) ? true : false
                        opacity: (autoExpMode.enabled && autoExpMode.checked) ? 1 : 0.1
                    }
                    ComboBox
                    {
                        id: antiFlickerCombo
                        model: ListModel {
                            ListElement { text: "50 Hz" }
                            ListElement { text: "60 Hz" }
                            ListElement { text: "Disable" }
                        }
                        activeFocusOnPress: true
                        style: econComboBoxStyle
                        enabled: (autoExpMode.enabled && autoExpMode.checked) ? true : false
                        opacity: (autoExpMode.enabled && autoExpMode.checked) ? 1 : 0.1
                        onCurrentIndexChanged: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlickerMode()
                            }
                            skipUpdateUIOnAntiFlickerMode = true
                        }
                    }

                    Text {
                        id: readStatistics
                        text: "--- Read Statistics ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 1
                        Row
                        {
                            Layout.alignment: Qt.AlignCenter
                            spacing: 5
                            Text {
                                id: expStatisticsText
                                text: "Exposure"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                            }
                            TextField
                            {
                                id: secondsExpStat
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                readOnly: true
                                Text {
                                    text: "   sec"
                                    anchors.top: secondsExpStat.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }

                            TextField
                            {
                                id: milliSecExpStat
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                readOnly: true
                                Text {
                                    text: "    ms"
                                    anchors.top: milliSecExpStat.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }

                            TextField
                            {
                                id: microSecExpStat
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                readOnly: true
                                Text {
                                    text: "    us"
                                    anchors.top: microSecExpStat.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                            }
                        }
                    }

                    Row
                    {
                        spacing: 35
                        Text {
                            id: gainStatisticsText
                            text: "Gain"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
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
                            Text {
                                text: "   db"
                                anchors.top: gainStatTextField.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                    Row{
                        Layout.alignment: Qt.AlignCenter

                        Button {
                            id: getStatisticsBtn
                            activeFocusOnPress : true
                            text: "Get Statistics"
                            style: econButtonStyle
                            enabled: ((gainStatTextField.enabled || microSecExpStat.enabled) || (gainStatTextField.enabled && microSecExpStat.enabled)) ? true : false
                            opacity: ((gainStatTextField.enabled || microSecExpStat.enabled) || (gainStatTextField.enabled && microSecExpStat.enabled)) ? 1 : 0.1
                            implicitHeight: 25
                            implicitWidth: 90
                            onClicked: {
                                see3cam50cugm.readStatistics()
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.readStatistics()
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
                        spacing: 29
                        Text {
                            id: readTempText
                            text: "Temperature"
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
                                see3cam50cugm.readTemperature()
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.readTemperature()
                            }
                        }
                    }


                    Text {
                        id: multiExpTriggerTitle
                        text: "--- Multi Exposure Trigger ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 10

                        ExclusiveGroup { id: multiExpTriggerGroup }

                        RadioButton {
                            id: disableMultiExp
                            exclusiveGroup: multiExpTriggerGroup
                            checked: false
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                see3cam50cugm.setMultiExposureTrigger(SEE3CAM_50CUGM.MET_DISABLE, 0)
                            }
                            Keys.onReturnPressed: {
                                see3cam50cugm.setMultiExposureTrigger(SEE3CAM_50CUGM.MET_DISABLE, 0)
                            }
                        }

                        Row{
                            spacing: 15

                                RadioButton {
                                    id: enableMultiExp
                                    exclusiveGroup: multiExpTriggerGroup
                                    checked: false
                                    text: "Enable"
                                    activeFocusOnPress: true
                                    style: econRadioButtonStyle
                                    onClicked: {
                                        see3cam50cugm.setMultiExposureTrigger(SEE3CAM_50CUGM.MET_ENABLE, multiExpTextField.text)
                                    }
                                    Keys.onReturnPressed: {
                                        see3cam50cugm.setMultiExposureTrigger(SEE3CAM_50CUGM.MET_ENABLE, multiExpTextField.text)
                                    }
                                }
                                TextField
                                {
                                    id: multiExpTextField
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    enabled: (enableMultiExp.enabled && enableMultiExp.checked) ? true : false
                                    opacity: (enableMultiExp.enabled && enableMultiExp.checked) ? 1 : 0.1
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                }

                                Button {
                                    id: multiExpTriggerBtn
                                    activeFocusOnPress : true
                                    text: "Trigger"
                                    style: econButtonStyle
                                    enabled: (enableMultiExp.enabled && enableMultiExp.checked) ? true : false
                                    opacity: (enableMultiExp.enabled && enableMultiExp.checked) ? 1 : 0.1
                                    implicitHeight: 25
                                    implicitWidth: 80
                                    onClicked: {
//                                        setButtonClicked = true
                                        see3cam50cugm.setMultiExposureTrigger(SEE3CAM_50CUGM.MET_ENABLE, multiExpTextField.text)
                                    }
                                    Keys.onReturnPressed: {
//                                        setButtonClicked = true
                                        see3cam50cugm.setMultiExposureTrigger(SEE3CAM_50CUGM.MET_ENABLE, multiExpTextField.text)
                                    }
                                }
                        }
                    }

                    Text {
                        id: multiFrameSetTitle
                        text: "--- Multi Frame Set ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 10
                        ExclusiveGroup { id: multiFrameSetGroup }
                        RadioButton {
                            id: frameSetOff
                            exclusiveGroup: multiFrameSetGroup
                            checked: false
                            text: "Off"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                disableFrameSet()
                            }
                            Keys.onReturnPressed: {
                                disableFrameSet()
                            }
                        }

                        RadioButton {
                            id: frameSet2
                            exclusiveGroup: multiFrameSetGroup
                            checked: false
                            text: "2-Frame Set"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                setFrameSet2()
                            }
                            Keys.onReturnPressed: {
                                setFrameSet2()
                            }
                        }

                        RadioButton {
                            id: frameSet4
                            exclusiveGroup: multiFrameSetGroup
                            checked: false
                            text: "4-Frame Set"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                setFrameSet4()
                            }
                            Keys.onReturnPressed: {
                                setFrameSet4()
                            }
                        }
                    }

                    Text {
                        id: frameSetTitle
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    ColumnLayout{
                        spacing: 35

                        ColumnLayout{
                            spacing: 25
                            Row
                            {
                                spacing: 35
                                Layout.alignment: Qt.AlignLeft
                                Text {
                                    id: gainLabel1
                                    text: "Gain 1"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                TextField
                                {
                                    id: gainStatTextField1
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   db"
                                        anchors.top: gainStatTextField1.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                            Row
                            {
                                Text {
                                    id: exposure1
                                    text: "Exposure 1"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                Layout.alignment: Qt.AlignLeft
                                spacing: 5
                                TextField
                                {
                                    id: second1
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   sec"
                                        anchors.top: second1.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: milliSecond1
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    ms"
                                        anchors.top: milliSecond1.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: microSecond1
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    us"
                                        anchors.top: microSecond1.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }


                        }
                        ColumnLayout{
                            spacing: 25
                            Row
                            {
                                spacing: 35
                                Layout.alignment: Qt.AlignLeft
                                Text {
                                    id: gainLabel2
                                    text: "Gain 2"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                TextField
                                {
                                    id: gainStatTextField2
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   db"
                                        anchors.top: gainStatTextField2.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                            Row
                            {
                                Layout.alignment: Qt.AlignCenter
                                spacing: 5
                                Text {
                                    id: exposure2
                                    text: "Exposure 2"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                TextField
                                {
                                    id: second2
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   sec"
                                        anchors.top: second2.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: milliSecond2
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    ms"
                                        anchors.top: milliSecond2.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: microSecond2
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    us"
                                        anchors.top: microSecond2.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                        }
                        ColumnLayout{
                            spacing: 25
                            Row
                            {
                                spacing: 35
                                Layout.alignment: Qt.AlignLeft
                                Text {
                                    id: gainLabel3
                                    text: "Gain 3"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                TextField
                                {
                                    id: gainStatTextField3
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   db"
                                        anchors.top: gainStatTextField3.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                            Row
                            {
                                Layout.alignment: Qt.AlignCenter
                                spacing: 5
                                Text {
                                    id: exposure3
                                    text: "Exposure 3"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                TextField
                                {
                                    id: second3
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   sec"
                                        anchors.top: second3.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: milliSecond3
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    us"
                                        anchors.top: milliSecond3.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: microSecond3
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    us"
                                        anchors.top: microSecond3.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                        }
                        ColumnLayout{
                            spacing: 25

                            Row
                            {
                                spacing: 35
                                Layout.alignment: Qt.AlignLeft
                                Text {
                                    id: gainLabel4
                                    text: "Gain 4"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }
                                TextField
                                {
                                    id: gainStatTextField4
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   db"
                                        anchors.top: gainStatTextField4.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                            Row
                            {
                                Layout.alignment: Qt.AlignCenter
                                spacing: 5
                                Text {
                                    id: exposure4
                                    text: "Exposure 4"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                }

                                TextField
                                {
                                    id: second4
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "   sec"
                                        anchors.top: second4.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: milliSecond4
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    ms"
                                        anchors.top: milliSecond4.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }

                                TextField
                                {
                                    id: microSecond4
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    style: econTextFieldStyle
                                    Text {
                                        text: "    us"
                                        anchors.top: microSecond4.bottom
                                        font.pixelSize: 14
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 1
                                        horizontalAlignment: Text.AlignHCenter
                                    }
                                }
                            }

                        }

                        Row
                        {
                            Layout.alignment: Qt.AlignCenter
                            Button {
                                id: multiFrameSetTriggerBtn
                                activeFocusOnPress : true
                                text: "Trigger"
                                style: econButtonStyle
                                enabled: (frameSetOff.checked) ? false : true
                                opacity: (frameSetOff.checked) ? 0.1 : 1
                                implicitHeight: 25
                                implicitWidth: 80
                                onClicked: {
                                    if(frameSet2.checked)
                                    {
                                        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_2, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)
                                    }
                                    else if(frameSet4.checked)
                                    {
                                        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_4, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)
                                    }
                                }
                                Keys.onReturnPressed: {
                                    if(frameSet2.checked)
                                    {
                                        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_2, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)
                                    }
                                    else if(frameSet4.checked)
                                    {
                                        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_4, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)
                                    }
                                }
                            }
                        }
                    }

                    Text {
                        id: ultraShortExpTitle
                        text: "--- Ultra Short Exposure ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }
                    ColumnLayout{
                        spacing: 10
                        Row{
                            spacing: 25
                            ExclusiveGroup { id: ultraShortExposureGrp }
                            RadioButton {
                                id: enableUSExp
                                exclusiveGroup: ultraShortExposureGrp
                                checked: false
                                text: "Enable"
                                activeFocusOnPress: true
                                style: econRadioButtonStyle
                                onClicked: {
                                    see3cam50cugm.setUltraShortExposure(SEE3CAM_50CUGM.EXP_ENABLE, ultraShortExpSlider.value)
                                }
                                Keys.onReturnPressed: {
                                    see3cam50cugm.setUltraShortExposure(SEE3CAM_50CUGM.EXP_ENABLE, ultraShortExpSlider.value)
                                }
                            }

                            RadioButton {
                                id: disableUSExp
                                exclusiveGroup: ultraShortExposureGrp
                                checked: false
                                text: "Disable"
                                activeFocusOnPress: true
                                style: econRadioButtonStyle
                                onClicked: {
                                    see3cam50cugm.setUltraShortExposure(SEE3CAM_50CUGM.EXP_DISABLE, ultraShortExpSlider.value)
                                }
                                Keys.onReturnPressed: {
                                    see3cam50cugm.setUltraShortExposure(SEE3CAM_50CUGM.EXP_DISABLE, ultraShortExpSlider.value)
                                }
                            }
                        }

                        Row
                        {
                            spacing: 35
                            Slider
                            {
                                id: ultraShortExpSlider
                                activeFocusOnPress: true
                                updateValueWhileDragging: false
                                width: 150
                                stepSize: 1
                                style:econSliderStyle
                                enabled: (enableUSExp.enabled && enableUSExp.checked) ? true : false
                                opacity: (enableUSExp.enabled && enableUSExp.checked) ? 1 : 0.1
                                onValueChanged:  {
                                    ultraShortExpTextField.text = ultraShortExpSlider.value
                                    if(skipUpdateUltraShortExposure){
                                        see3cam50cugm.setUltraShortExposure(SEE3CAM_50CUGM.EXP_ENABLE, ultraShortExpSlider.value)
                                    }
                                    skipUpdateUltraShortExposure = true
                                }
                            }
                            TextField
                            {
                                id: ultraShortExpTextField
                                text: ultraShortExpSlider.value
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                enabled: (enableUSExp.enabled && enableUSExp.checked) ? true : false
                                opacity: (enableUSExp.enabled && enableUSExp.checked) ? 1 : 0.1
                                style: econTextFieldStyle
                                validator: IntValidator {bottom: ultraShortExpSlider.minimumValue; top: ultraShortExpSlider.maximumValue}
                                onTextChanged: {
                                    if(text.length > 0){
                                        ultraShortExpSlider.value = ultraShortExpTextField.text
                                    }
                                }
                            }
                        }
                    }

                    Text {
                        id: softwareTriggerTitle
                        text: "--- Software Trigger ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    Row
                    {
                        spacing: 9

                        Text {
                            id: softwareTriggerText
                            text: "Range [0 - 255]"
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
                            id: softwareTriggerTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            enabled: ((triggerCombo.currentIndex == 2) && (triggerMode.checked)) ? true : false
                            opacity: ((triggerCombo.currentIndex == 2) && (triggerMode.checked)) ? 1 : 0.1
                            style: econTextFieldStyle
                        }

                        Button {
                            id: softwareTriggerBtn
                            activeFocusOnPress : true
                            text: "Set"
                            style: econButtonStyle
                            enabled: ((triggerCombo.currentIndex == 2) && (triggerMode.checked)) ? true : false
                            opacity: ((triggerCombo.currentIndex == 2) && (triggerMode.checked)) ? 1 : 0.1
                            implicitHeight: 25
                            implicitWidth: 65
                            onClicked: {
                                setButtonClicked = true
                                see3cam50cugm.setSoftwareTrigger(softwareTriggerTextField.text)
                            }
                            Keys.onReturnPressed: {
                                setButtonClicked = true
                                see3cam50cugm.setSoftwareTrigger(softwareTriggerTextField.text)
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
                implicitHeight: 23
                border.color: "#333"
                border.width: 3
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
            if(cameraMode === SEE3CAM_50CUGM.MASTER)
            {
                masterMode.checked = true
                triggerCombo.enabled = false
                triggerCombo.opacity = 0.1

                strobeOn.enabled  = false
                strobeOn.opacity  = 0.1
                strobeOff.enabled = false
                strobeOff.opacity = 0.1
            }
            else
            {
                triggerCombo.enabled = true
                triggerCombo.opacity = 1
                triggerMode.checked  = true

                root.checkForTriggerMode(true)
                root.videoRecordBtnEnable(false)
                root.captureBtnEnable(false)

                if(cameraMode === SEE3CAM_50CUGM.TRIGGER_EXP_CONTROL)
                {
                    strobeOn.enabled    = false
                    strobeOn.opacity    = 0.1
                    strobeOff.enabled   = false
                    strobeOff.opacity   = 0.1

                    triggerCombo.currentIndex = 0
                }
                else if(cameraMode === SEE3CAM_50CUGM.TRIGGER_ACQ_CONTROL)
                {
                    triggerCombo.currentIndex = 1

                    strobeOn.enabled    = true
                    strobeOn.opacity    = 1
                    strobeOff.enabled   = true
                    strobeOff.opacity   = 1
                }
                else if(cameraMode === SEE3CAM_50CUGM.TRIGGER_SOFTWARE_CONTROL)
                {
                    triggerCombo.currentIndex = 2

                    strobeOn.enabled    = false
                    strobeOn.opacity    = 0.1
                    strobeOff.enabled   = false
                    strobeOff.opacity   = 0.1
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
                strobeOn.enabled    = false
                strobeOn.opacity    = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
            }
            else if(triggerCombo.currentIndex == 0)
            {
                strobeOn.enabled    = false
                strobeOn.opacity    = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
            }
            else if(triggerCombo.currentIndex == 1)
            {
                strobeOn.enabled    = true
                strobeOn.opacity    = 1
                strobeOff.enabled   = true
                strobeOff.opacity   = 1
            }
            else if(triggerCombo.currentIndex == 2)
            {
                strobeOn.enabled    = false
                strobeOn.opacity    = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
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

        //Second Phase Features

        onGainModeReceived:{
            currentGainMode(gainMode)
        }
        onAutoGainFeatureReceived:{
            currentAutoGainMode(autoGainFeature)
        }
        onManualGainValuesReceived:{
            skipUpdateGainMode = false
            gainSlider.minimumValue = min
            gainSlider.maximumValue = max
            gainSlider.value        = current
            gainSlider.stepSize     = stepValue
            skipUpdateGainMode = true
        }

        onExposureModeReceived: {
            currentExposureMode(exposureMode)
        }

        onAutoExposureFeaturesReceived: {
            currentAutoExposureMode(autoExpFeature)
        }

        onManualExposureValuesReceived: {
            expInSecondsTextField.text = seconds
            expInMilliSecTextField.text = milliSeconds
            expInMicroSecTextField.text = microSeconds
        }

        onCurrentBrightnessReceived:{
            skipUpdateTargetBrightness = false
            targetBrightness.value     = current
            skipUpdateTargetBrightness = true
        }
        onMinBrightnessReceived:{
            targetBrightness.minimumValue = min
        }
        onMaxBrightnessReceived:{
            targetBrightness.maximumValue = max
        }
        onBrightnessStepValueReceived:{
            targetBrightness.stepSize     = stepValue
        }


        //Gain lower & upper limit
        onSliderMinimumReceived: {
            gainUpperLimitSlider.minimumValue = min
            gainLowerLimitSlider.minimumValue = min
        }

        onSliderMaximumReceived: {
            gainUpperLimitSlider.maximumValue = max
            gainLowerLimitSlider.maximumValue = max
        }

        onLowerGainLimitReceived:{
            skipUpdateLowerLimitSlider = false
            gainLowerLimitSlider.value = lowerLimit
            skipUpdateLowerLimitSlider = true
        }

        onUpperGainLimitReceived:{
            skipUpdateUpperLimitSlider = false
            gainUpperLimitSlider.value = upperLimit
            skipUpdateUpperLimitSlider = true
        }

        onGainLimitStepValueReceived:{
            gainUpperLimitSlider.stepSize = stepValue
            gainLowerLimitSlider.stepSize = stepValue
        }

        //lower limit exposure
        onLowerLimitExposure: {
            lowerLimitSeconds.text  = seconds
            lowerLimitMilliSec.text = milliSeconds
            lowerLimitMicroSec.text = microSeconds
        }

        onUpperLimitExposure: {
            upperLimitSeconds.text  = seconds
            upperLimitMilliSec.text = milliSeconds
            upperLimitMicroSec.text = microSeconds
        }

        onAntiFlickerModeReceived: {
            getAntiFlickerModes(antiFlicker)
        }
        onGainStatisticsReceived: {
            gainStatTextField.text = gainStatistics
        }
        onExposureStatisticsReceived: {
            secondsExpStat.text  = seconds
            milliSecExpStat.text = milliSeconds
            microSecExpStat.text = microSeconds
        }

        onTemperatureReceived: {
            readTempTextField.text = temperature
        }
        onMultiExposureModeReceived:{
            currentMultiExposureModeReceived(mode)
        }
        onMultiExposureValueReceived:{
            multiExpTextField.text = count
        }

        onUltraShortExposureModeReceived:{
            currentUltraShortExposureReceived(mode)
        }
        onUltraShortExposureValuesReceived:{
            skipUpdateUltraShortExposure     = false
            ultraShortExpSlider.value        = current
            ultraShortExpSlider.minimumValue = min
            ultraShortExpSlider.maximumValue = max
            skipUpdateUltraShortExposure     = true
        }

        onCurrentTriggerValueReceived:{
            softwareTriggerTextField.text = triggerValue
        }

        onMultiFrameSetModeReceived:{
            if(mode === SEE3CAM_50CUGM.FRAME_SET_OFF)
            {
                frameSetOff.checked = true

                frameSetTitle.text = "--- Multi Frame Disabled ---"

                gainStatTextField1.enabled = false
                gainStatTextField1.opacity = 0.1

                gainStatTextField2.enabled = false
                gainStatTextField2.opacity = 0.1

                gainStatTextField3.enabled = false
                gainStatTextField3.opacity = 0.1

                gainStatTextField4.enabled = false
                gainStatTextField4.opacity = 0.1

                second1.enabled      = false
                second1.opacity      = 0.1
                milliSecond1.enabled = false
                milliSecond1.opacity = 0.1
                microSecond1.enabled = false
                microSecond1.opacity = 0.1

                second2.enabled      = false
                second2.opacity      = 0.1
                milliSecond2.enabled = false
                milliSecond2.opacity = 0.1
                microSecond2.enabled = false
                microSecond2.opacity = 0.1

                second3.enabled      = false
                second3.opacity      = 0.1
                milliSecond3.enabled = false
                milliSecond3.opacity = 0.1
                microSecond3.enabled = false
                microSecond3.opacity = 0.1

                second4.enabled      = false
                second4.opacity      = 0.1
                milliSecond4.enabled = false
                milliSecond4.opacity = 0.1
                microSecond4.enabled = false
                microSecond4.opacity = 0.1
            }
            else if(mode === SEE3CAM_50CUGM.FRAME_SET_2)
            {
                frameSet2.checked = true

                frameSetTitle.text = "--- 2-Frame Set ---"

                gainStatTextField1.enabled = true
                gainStatTextField1.opacity = 1

                gainStatTextField2.enabled = true
                gainStatTextField2.opacity = 1

                gainStatTextField3.enabled = false
                gainStatTextField3.opacity = 0.1

                gainStatTextField4.enabled = false
                gainStatTextField4.opacity = 0.1

                second1.enabled      = true
                second1.opacity      = 1
                milliSecond1.enabled = true
                milliSecond1.opacity = 1
                microSecond1.enabled = true
                microSecond1.opacity = 1

                second2.enabled      = true
                second2.opacity      = 1
                milliSecond2.enabled = true
                milliSecond2.opacity = 1
                microSecond2.enabled = true
                microSecond2.opacity = 1

                second3.enabled      = false
                second3.opacity      = 0.1
                milliSecond3.enabled = false
                milliSecond3.opacity = 0.1
                microSecond3.enabled = false
                microSecond3.opacity = 0.1

                second4.enabled      = false
                second4.opacity      = 0.1
                milliSecond4.enabled = false
                milliSecond4.opacity = 0.1
                microSecond4.enabled = false
                microSecond4.opacity = 0.1
            }
            else if(mode === SEE3CAM_50CUGM.FRAME_SET_4)
            {
                frameSetTitle.text = "--- 4-Frame Set ---"

                frameSet4.checked = true

                gainStatTextField1.enabled = true
                gainStatTextField1.opacity = 1

                gainStatTextField2.enabled = true
                gainStatTextField2.opacity = 1

                gainStatTextField3.enabled = true
                gainStatTextField3.opacity = 1

                gainStatTextField4.enabled = true
                gainStatTextField4.opacity = 1

                second1.enabled      = true
                second1.opacity      = 1
                milliSecond1.enabled = true
                milliSecond1.opacity = 1
                microSecond1.enabled = true
                microSecond1.opacity = 1

                second2.enabled      = true
                second2.opacity      = 1
                milliSecond2.enabled = true
                milliSecond2.opacity = 1
                microSecond2.enabled = true
                microSecond2.opacity = 1

                second3.enabled      = true
                second3.opacity      = 1
                milliSecond3.enabled = true
                milliSecond3.opacity = 1
                microSecond3.enabled = true
                microSecond3.opacity = 1

                second4.enabled      = true
                second4.opacity      = 1
                milliSecond4.enabled = true
                milliSecond4.opacity = 1
                microSecond4.enabled = true
                microSecond4.opacity = 1
            }
        }

        onFrameSet1Received:{
            gainStatTextField1.text = gainVal1
            second1.text            = sec1
            milliSecond1.text       = milliSec1
            microSecond1.text       = microSec1
        }
        onFrameSet2Received:{
            gainStatTextField2.text = gainVal2
            second2.text            = sec2
            milliSecond2.text       = milliSec2
            microSecond2.text       = microSec2
        }
        onFrameSet3Received:{
            gainStatTextField3.text  = gainVal3
            second3.text             = sec3
            milliSecond3.text        = milliSec3
            microSecond3.text        = microSec3
        }onFrameSet4Received:{
            gainStatTextField4.text  = gainVal4
            second4.text             = sec4
            milliSecond4.text        = milliSec4
            microSecond4.text        = microSec4
        }

        onIndicateCommandStatus:{
            displayMessageBox(title, text)
        }
        onIndicateExposureValueRangeFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
            }
        }

        onIndicateExposureLimitsInvalidInput:{
            displayMessageBox(title, text)
            see3cam50cugm.getAutoExposureLowerLimit();
            see3cam50cugm.getAutoExposureUpperLimit();
        }

        onIndicateSoftwareTriggerFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
                see3cam50cugm.getSoftwareTrigger()
            }
        }
        onIndicateMultipleExposureTriggerFailure:{
            displayMessageBox(title, text)
            see3cam50cugm.getMultiExposureTrigger()
        }
        onIndicateGainValueRangeFailure:{
            see3cam50cugm.getAutoGainLimit()
        }
        onIndicateMultiFrameSetInvalidInput:{
            displayMessageBox(title, text)
            see3cam50cugm.getMultiFrameSet()
        }
    }

    function currentGainMode(mode)
    {
        if(mode === SEE3CAM_50CUGM.AUTO_GAIN_MODE)
        {
            autoGainMode.checked = true

            //Auto features enabled
            gainContinuous.enabled = true
            gainContinuous.opacity = 1
            gainSingleShot.enabled = true
            gainSingleShot.opacity = 1

            //Disabling manual features
            gainSlider.enabled    = false
            gainSlider.opacity    = 0.1
            gainTextField.enabled = false
            gainTextField.opacity = 0.1

            //To enable gainLimit in Auto Gain mode
            gainLowerLimitSlider.enabled    = true
            gainLowerLimitSlider.opacity    = 1
            gainLowerLimitTextField.enabled = true
            gainLowerLimitTextField.opacity = 1

            gainUpperLimitSlider.enabled    = true
            gainUpperLimitSlider.opacity    = 1
            gainUpperLimitTextField.enabled = true
            gainUpperLimitTextField.opacity = 1

            //Enable gain statistics
            gainStatisticsText.enabled = true
            gainStatisticsText.opacity = 1

            gainStatTextField.enabled = true
            gainStatTextField.opacity = 1
        }
        else if(mode === SEE3CAM_50CUGM.MANUAL_GAIN_MODE)
        {
            manualGainMode.checked = true

            //Disabling manual features
            gainSlider.enabled    = true
            gainSlider.opacity    = 1
            gainTextField.enabled = true
            gainTextField.opacity = 1

            //Auto features disabled
            gainContinuous.enabled = false
            gainContinuous.opacity = 0.1
            gainSingleShot.enabled = false
            gainSingleShot.opacity = 0.1

            //To enable gainLimit in Auto Gain mode
            gainLowerLimitSlider.enabled    = false
            gainLowerLimitSlider.opacity    = 0.1
            gainLowerLimitTextField.enabled = false
            gainLowerLimitTextField.opacity = 0.1

            gainUpperLimitSlider.enabled    = false
            gainUpperLimitSlider.opacity    = 0.1
            gainUpperLimitTextField.enabled = false
            gainUpperLimitTextField.opacity = 0.1

            //Disable gain statistics
            gainStatisticsText.enabled = false
            gainStatisticsText.opacity = 0.1

            gainStatTextField.enabled = false
            gainStatTextField.opacity = 0.1
        }
    }

    function currentAutoGainMode(mode)
    {
        if(mode === SEE3CAM_50CUGM.CONTINIOUS_GAIN)
        {
            gainContinuous.checked = true
        }
        else if((mode === SEE3CAM_50CUGM.SINGLE_SHOT_GAIN) || mode === SEE3CAM_50CUGM.SINGLE_SHOT_GAIN_TRIGGER)
        {
            gainSingleShot.checked = true
        }
    }

    function currentExposureMode(mode)
    {
        if(mode === SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE)
        {
            autoExpMode.checked = true

            //Manual features disabled
            expInSecondsTextField.enabled  = false
            expInMilliSecTextField.enabled = false
            expInMicroSecTextField.enabled = false
            manualExpSetBtn.enabled        = false

            expInSecondsTextField.opacity  = 0.1
            expInMilliSecTextField.opacity = 0.1
            expInMicroSecTextField.opacity = 0.1
            manualExpSetBtn.opacity        = 0.1

            //Lower limit & Upper limit enabled
            lowerLimitSeconds.enabled  = true
            lowerLimitMilliSec.enabled = true
            lowerLimitMicroSec.enabled = true
            lowerLimitSeconds.opacity  = 1
            lowerLimitMilliSec.opacity = 1
            lowerLimitMicroSec.opacity = 1

            upperLimitSeconds.enabled  = true
            upperLimitMilliSec.enabled = true
            upperLimitMicroSec.enabled = true
            upperLimitSeconds.opacity  = 1
            upperLimitMilliSec.opacity = 1
            upperLimitMicroSec.opacity = 1

            //Auto features enabled
            expContinuous.enabled = true
            expContinuous.opacity = 1
            expSingleShot.enabled = true
            expSingleShot.opacity = 1

            //Enable exposure statistics in auto exposure mode
            expStatisticsText.opacity = 1

            secondsExpStat.enabled  = true
            milliSecExpStat.enabled = true
            microSecExpStat.enabled = true

            secondsExpStat.opacity  = 1
            milliSecExpStat.opacity = 1
            microSecExpStat.opacity = 1
        }
        else if(mode === SEE3CAM_50CUGM.MANUAL_EXPOSURE_MODE)
        {
            manualExpMode.checked = true

            //Manual Features enabled
            expInSecondsTextField.enabled  = true
            expInMilliSecTextField.enabled = true
            expInMicroSecTextField.enabled = true
            manualExpSetBtn.enabled        = true

            expInSecondsTextField.opacity  = 1
            expInMilliSecTextField.opacity = 1
            expInMicroSecTextField.opacity = 1
            manualExpSetBtn.opacity        = 1

            //Auto features disabled
            expContinuous.enabled = false
            expContinuous.opacity = 0.1

            expSingleShot.enabled = false
            expSingleShot.opacity = 0.1

            //Lower limit & Upper limit disabled
            lowerLimitSeconds.enabled  = false
            lowerLimitMilliSec.enabled = false
            lowerLimitMicroSec.enabled = false
            lowerLimitSeconds.opacity  = 0.1
            lowerLimitMilliSec.opacity = 0.1
            lowerLimitMicroSec.opacity = 0.1

            upperLimitSeconds.enabled  = false
            upperLimitMilliSec.enabled = false
            upperLimitMicroSec.enabled = false
            upperLimitSeconds.opacity  = 0.1
            upperLimitMilliSec.opacity = 0.1
            upperLimitMicroSec.opacity = 0.1

            //Disable exposure statistics in exposure manual mode
            expStatisticsText.opacity = 0.1

            secondsExpStat.enabled  = false
            milliSecExpStat.enabled = false
            microSecExpStat.enabled = false

            secondsExpStat.opacity  = 0.1
            milliSecExpStat.opacity = 0.1
            microSecExpStat.opacity = 0.1
        }
    }

    function currentAutoExposureMode(mode)
    {
        if(mode === SEE3CAM_50CUGM.CONTINIOUS_EXPOSURE)
        {
            expContinuous.checked = true
        }
        else if((mode === SEE3CAM_50CUGM.SINGLE_SHOT_EXPOSURE) || (mode === SEE3CAM_50CUGM.SINGLE_SHOT_EXP_TRIGGER))
        {
            expSingleShot.checked = true
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
        else if(triggerCombo.currentText.toString() == "Software control")
        {
           setTriggerSoftwareControl()
        }
    }

    function setTriggerExpControl()
    {
        root.checkForTriggerMode(true)
        root.videoRecordBtnEnable(false)
        root.captureBtnEnable(false)

        //Disable strobe mode
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

        //Enable strobe mode
        strobeOn.enabled = true
        strobeOn.opacity = 1
        strobeOff.enabled   = true
        strobeOff.opacity   = 1

        see3cam50cugm.setCameraMode(SEE3CAM_50CUGM.TRIGGER_ACQ_CONTROL)
    }

    function setTriggerSoftwareControl()
    {
        root.checkForTriggerMode(true)
        root.videoRecordBtnEnable(false)
        root.captureBtnEnable(false)

        //Disable strobe mode
        strobeOn.enabled    = false
        strobeOn.opacity    = 0.1
        strobeOff.enabled   = false
        strobeOff.opacity   = 0.1

        see3cam50cugm.setCameraMode(SEE3CAM_50CUGM.TRIGGER_SOFTWARE_CONTROL)
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

    function getAntiFlickerModes(antiFlicker)
    {
        switch(antiFlicker)
        {
            case SEE3CAM_50CUGM.MODE_50Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case SEE3CAM_50CUGM.MODE_60Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
            case SEE3CAM_50CUGM.MODE_DISABLE:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 2
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }

    function currentMultiExposureModeReceived(mode)
    {
        if(mode === SEE3CAM_50CUGM.MET_ENABLE)
        {
            enableMultiExp.checked = true
        }
        else if(mode === SEE3CAM_50CUGM.MET_DISABLE)
        {
            disableMultiExp.checked = true
        }
    }

    function currentUltraShortExposureReceived(mode)
    {
        if(mode === SEE3CAM_50CUGM.EXP_ENABLE)
        {
            enableUSExp.checked = true
        }
        else if(mode === SEE3CAM_50CUGM.EXP_DISABLE)
        {
            disableUSExp.checked = true
        }
    }

    //Set functions

    function setAutoExposure()
    {
        if(expContinuous.checked == true)
        {
            see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.CONTINIOUS_EXPOSURE, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
        }
        else if(expSingleShot.checked == true)
        {
            see3cam50cugm.setExposure(SEE3CAM_50CUGM.AUTO_EXPOSURE_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_EXPOSURE, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
        }

        //Auto features enabled
        expContinuous.enabled = true
        expContinuous.opacity = 1
        expSingleShot.enabled = true
        expSingleShot.opacity = 1

        //Manual features disabled
        expInSecondsTextField.enabled  = false
        expInMilliSecTextField.enabled = false
        expInMicroSecTextField.enabled = false
        manualExpSetBtn.enabled = false

        expInSecondsTextField.opacity  = 0.1
        expInMilliSecTextField.opacity = 0.1
        expInMicroSecTextField.opacity = 0.1
        manualExpSetBtn.opacity = 0.1

        //Lower limit & Upper limit enabled
        lowerLimitSeconds.enabled  = true
        lowerLimitMilliSec.enabled = true
        lowerLimitMicroSec.enabled = true
        lowerLimitSeconds.opacity  = 1
        lowerLimitMilliSec.opacity = 1
        lowerLimitMicroSec.opacity = 1

        upperLimitSeconds.enabled  = true
        upperLimitMilliSec.enabled = true
        upperLimitMicroSec.enabled = true
        upperLimitSeconds.opacity  = 1
        upperLimitMilliSec.opacity = 1
        upperLimitMicroSec.opacity = 1

        //Enable exposure statistics in auto exposure mode
        expStatisticsText.opacity = 1

        secondsExpStat.enabled  = true
        milliSecExpStat.enabled = true
        microSecExpStat.enabled = true

        secondsExpStat.opacity  = 1
        milliSecExpStat.opacity = 1
        microSecExpStat.opacity = 1
    }
    function setManualExposure()
    {
        see3cam50cugm.setExposure(SEE3CAM_50CUGM.MANUAL_EXPOSURE_MODE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)

        //Auto features disabled
        expContinuous.enabled = false
        expContinuous.opacity = 0.1

        expSingleShot.enabled = false
        expSingleShot.opacity = 0.1

        //Manual Features enabled
        expInSecondsTextField.enabled  = true
        expInMilliSecTextField.enabled = true
        expInMicroSecTextField.enabled = true
        manualExpSetBtn.enabled = true

        expInSecondsTextField.opacity  = 1
        expInMilliSecTextField.opacity = 1
        expInMicroSecTextField.opacity = 1
        manualExpSetBtn.opacity = 1

        //Lower limit & Upper limit disabled
        lowerLimitSeconds.enabled  = false
        lowerLimitMilliSec.enabled = false
        lowerLimitMicroSec.enabled = false
        lowerLimitSeconds.opacity  = 0.1
        lowerLimitMilliSec.opacity = 0.1
        lowerLimitMicroSec.opacity = 0.1

        upperLimitSeconds.enabled  = false
        upperLimitMilliSec.enabled = false
        upperLimitMicroSec.enabled = false
        upperLimitSeconds.opacity  = 0.1
        upperLimitMilliSec.opacity = 0.1
        upperLimitMicroSec.opacity = 0.1

        //Disable exposure statistics in exposure manual mode
        expStatisticsText.opacity = 0.1

        secondsExpStat.enabled  = false
        milliSecExpStat.enabled = false
        microSecExpStat.enabled = false

        secondsExpStat.opacity  = 0.1
        milliSecExpStat.opacity = 0.1
        microSecExpStat.opacity = 0.1
    }

    function setAutoGain(mode)
    {
        //Checking Auto Gain features
        if(gainContinuous.checked == true)
        {
            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.CONTINIOUS_GAIN, gainSlider.value)
        }
        else if(gainSingleShot.checked == true)
        {
            see3cam50cugm.setGainMode(SEE3CAM_50CUGM.AUTO_GAIN_MODE, SEE3CAM_50CUGM.SINGLE_SHOT_GAIN, gainSlider.value)
        }

        //Auto features enabled
        gainContinuous.enabled = true
        gainContinuous.opacity = 1
        gainSingleShot.enabled = true
        gainSingleShot.opacity = 1

        //Disabling manual features
        gainSlider.enabled    = false
        gainSlider.opacity    = 0.1
        gainTextField.enabled = false
        gainTextField.opacity = 0.1

        //To enable gainLimit in Auto Gain mode
        gainLowerLimitSlider.enabled    = true
        gainLowerLimitSlider.opacity    = 1
        gainLowerLimitTextField.enabled = true
        gainLowerLimitTextField.opacity = 1

        gainUpperLimitSlider.enabled    = true
        gainUpperLimitSlider.opacity    = 1
        gainUpperLimitTextField.enabled = true
        gainUpperLimitTextField.opacity = 1

        //Enable gain statistics
        gainStatisticsText.enabled = true
        gainStatisticsText.opacity = 1

        gainStatTextField.enabled = true
        gainStatTextField.opacity = 1
    }

    function setManualGain()
    {
        see3cam50cugm.setGainMode(SEE3CAM_50CUGM.MANUAL_GAIN_MODE, 0, gainSlider.value)

        //Disabling manual features
        gainSlider.enabled    = true
        gainSlider.opacity    = 1
        gainTextField.enabled = true
        gainTextField.opacity = 1

        //Auto features disabled
        gainContinuous.enabled = false
        gainContinuous.opacity = 0.1
        gainSingleShot.enabled = false
        gainSingleShot.opacity = 0.1

        //To enable gainLimit in Auto Gain mode
        gainLowerLimitSlider.enabled    = false
        gainLowerLimitSlider.opacity    = 0.1
        gainLowerLimitTextField.enabled = false
        gainLowerLimitTextField.opacity = 0.1

        gainUpperLimitSlider.enabled    = false
        gainUpperLimitSlider.opacity    = 0.1
        gainUpperLimitTextField.enabled = false
        gainUpperLimitTextField.opacity = 0.1

        //Disable gain statistics
        gainStatisticsText.enabled = false
        gainStatisticsText.opacity = 0.1

        gainStatTextField.enabled = false
        gainStatTextField.opacity = 0.1
    }


    function setAntiFlickerMode()
    {
        if(antiFlickerCombo.currentIndex === 0)
            see3cam50cugm.setAntiFlickerMode(SEE3CAM_50CUGM.MODE_50Hz)
        else if(antiFlickerCombo.currentIndex === 1)
            see3cam50cugm.setAntiFlickerMode(SEE3CAM_50CUGM.MODE_60Hz)
        else if(antiFlickerCombo.currentIndex === 2)
            see3cam50cugm.setAntiFlickerMode(SEE3CAM_50CUGM.MODE_DISABLE)
    }

    function disableFrameSet()
    {
        frameSetTitle.text = "--- Multi Frame Disabled ---"

        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_OFF, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)

        gainStatTextField1.enabled = false
        gainStatTextField1.opacity = 0.1

        gainStatTextField2.enabled = false
        gainStatTextField2.opacity = 0.1

        gainStatTextField3.enabled = false
        gainStatTextField3.opacity = 0.1

        gainStatTextField4.enabled = false
        gainStatTextField4.opacity = 0.1

        second1.enabled      = false
        second1.opacity      = 0.1
        milliSecond1.enabled = false
        milliSecond1.opacity = 0.1
        microSecond1.enabled = false
        microSecond1.opacity = 0.1

        second2.enabled      = false
        second2.opacity      = 0.1
        milliSecond2.enabled = false
        milliSecond2.opacity = 0.1
        microSecond2.enabled = false
        microSecond2.opacity = 0.1

        second3.enabled      = false
        second3.opacity      = 0.1
        milliSecond3.enabled = false
        milliSecond3.opacity = 0.1
        microSecond3.enabled = false
        microSecond3.opacity = 0.1

        second4.enabled      = false
        second4.opacity      = 0.1
        milliSecond4.enabled = false
        milliSecond4.opacity = 0.1
        microSecond4.enabled = false
        microSecond4.opacity = 0.1
    }

    function setFrameSet2()
    {
        frameSetTitle.text = "--- 2-Frame Set ---"

        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_2, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)

        gainStatTextField1.enabled = true
        gainStatTextField1.opacity = 1

        gainStatTextField2.enabled = true
        gainStatTextField2.opacity = 1

        gainStatTextField3.enabled = false
        gainStatTextField3.opacity = 0.1

        gainStatTextField4.enabled = false
        gainStatTextField4.opacity = 0.1

        second1.enabled      = true
        second1.opacity      = 1
        milliSecond1.enabled = true
        milliSecond1.opacity = 1
        microSecond1.enabled = true
        microSecond1.opacity = 1

        second2.enabled      = true
        second2.opacity      = 1
        milliSecond2.enabled = true
        milliSecond2.opacity = 1
        microSecond2.enabled = true
        microSecond2.opacity = 1

        second3.enabled      = false
        second3.opacity      = 0.1
        milliSecond3.enabled = false
        milliSecond3.opacity = 0.1
        microSecond3.enabled = false
        microSecond3.opacity = 0.1

        second4.enabled      = false
        second4.opacity      = 0.1
        milliSecond4.enabled = false
        milliSecond4.opacity = 0.1
        microSecond4.enabled = false
        microSecond4.opacity = 0.1
    }

    function setFrameSet4()
    {
        frameSetTitle.text = "--- 4-Frame Set ---"

        see3cam50cugm.setMultiFrameSet(SEE3CAM_50CUGM.FRAME_SET_4, gainStatTextField1.text, second1.text, milliSecond1.text, microSecond1.text,  gainStatTextField2.text, second2.text, milliSecond2.text, microSecond2.text,  gainStatTextField3.text, second3.text, milliSecond3.text, microSecond3.text, gainStatTextField4.text, second4.text, milliSecond4.text, microSecond4.text)

        gainStatTextField1.enabled = true
        gainStatTextField1.opacity = 1

        gainStatTextField2.enabled = true
        gainStatTextField2.opacity = 1

        gainStatTextField3.enabled = true
        gainStatTextField3.opacity = 1

        gainStatTextField4.enabled = true
        gainStatTextField4.opacity = 1

        second1.enabled      = true
        second1.opacity      = 1
        milliSecond1.enabled = true
        milliSecond1.opacity = 1
        microSecond1.enabled = true
        microSecond1.opacity = 1

        second2.enabled      = true
        second2.opacity      = 1
        milliSecond2.enabled = true
        milliSecond2.opacity = 1
        microSecond2.enabled = true
        microSecond2.opacity = 1

        second3.enabled      = true
        second3.opacity      = 1
        milliSecond3.enabled = true
        milliSecond3.opacity = 1
        microSecond3.enabled = true
        microSecond3.opacity = 1

        second4.enabled      = true
        second4.opacity      = 1
        milliSecond4.enabled = true
        milliSecond4.opacity = 1
        microSecond4.enabled = true
        microSecond4.opacity = 1
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            autoExpMode.checked = true

            root.sendExposureStatusToUVC(1 , convertedExposure)

            //Auto features enabled
            expContinuous.enabled = true
            expContinuous.opacity = 1
            expSingleShot.enabled = true
            expSingleShot.opacity = 1

            //Manual features disabled
            expInSecondsTextField.enabled  = false
            expInMilliSecTextField.enabled = false
            expInMicroSecTextField.enabled = false
            manualExpSetBtn.enabled = false

            expInSecondsTextField.opacity  = 0.1
            expInMilliSecTextField.opacity = 0.1
            expInMicroSecTextField.opacity = 0.1
            manualExpSetBtn.opacity = 0.1

            //Lower limit & Upper limit enabled
            lowerLimitSeconds.enabled  = true
            lowerLimitMilliSec.enabled = true
            lowerLimitMicroSec.enabled = true
            lowerLimitSeconds.opacity  = 1
            lowerLimitMilliSec.opacity = 1
            lowerLimitMicroSec.opacity = 1

            upperLimitSeconds.enabled  = true
            upperLimitMilliSec.enabled = true
            upperLimitMicroSec.enabled = true
            upperLimitSeconds.opacity  = 1
            upperLimitMilliSec.opacity = 1
            upperLimitMicroSec.opacity = 1

            //Enable readStatistics in auto exposure mode
            expStatisticsText.opacity     = 1

            secondsExpStat.enabled  = true
            milliSecExpStat.enabled = true
            microSecExpStat.enabled = true

            secondsExpStat.opacity  = 1
            milliSecExpStat.opacity = 1
            microSecExpStat.opacity = 1
        }else{
            manualExpMode.checked = true

            sendConvertedExpToUVC()

            //Auto features disabled
            expContinuous.enabled = false
            expContinuous.opacity = 0.1

            expSingleShot.enabled = false
            expSingleShot.opacity = 0.1

            //Manual Features enabled
            expInSecondsTextField.enabled  = true
            expInMilliSecTextField.enabled = true
            expInMicroSecTextField.enabled = true
            manualExpSetBtn.enabled        = true

            expInSecondsTextField.opacity  = 1
            expInMilliSecTextField.opacity = 1
            expInMicroSecTextField.opacity = 1
            manualExpSetBtn.opacity        = 1

            //Lower limit & Upper limit disabled
            lowerLimitSeconds.enabled  = false
            lowerLimitMilliSec.enabled = false
            lowerLimitMicroSec.enabled = false
            lowerLimitSeconds.opacity  = 0.1
            lowerLimitMilliSec.opacity = 0.1
            lowerLimitMicroSec.opacity = 0.1

            upperLimitSeconds.enabled  = false
            upperLimitMilliSec.enabled = false
            upperLimitMicroSec.enabled = false
            upperLimitSeconds.opacity  = 0.1
            upperLimitMilliSec.opacity = 0.1
            upperLimitMicroSec.opacity = 0.1

            //Disable exposure statistics in exposure manual mode
            expStatisticsText.opacity = 0.1

            secondsExpStat.enabled  = false
            milliSecExpStat.enabled = false
            microSecExpStat.enabled = false

            secondsExpStat.opacity  = 0.1
            milliSecExpStat.opacity = 0.1
            microSecExpStat.opacity = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    function sendConvertedExpToUVC()
    {
        if(expInSecondsTextField.text <= 2)
        {
            secondInt = parseInt(expInSecondsTextField.text);
            milliSecondInt = parseInt(expInMilliSecTextField.text);
            microSecondInt = parseInt(expInMicroSecTextField.text);

            convertedExposure = (secondInt*1000000) + (milliSecondInt*1000) + microSecondInt
            convertedExposure = convertedExposure/100

            root.sendExposureStatusToUVC(0 , convertedExposure)
        }
        else{
            root.sendExposureToUVC(20000)
        }
    }

    function setToDefaultValues(){
        defaultValue.enabled = false //To avoid multiple clicks over Default button
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)

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
        see3cam50cugm.getTargetBrightness()
        see3cam50cugm.getGainMode()
        see3cam50cugm.getAutoGainLimit()
        see3cam50cugm.getExposure()
        see3cam50cugm.getAutoExposureLowerLimit()
        see3cam50cugm.getAutoExposureUpperLimit()
        see3cam50cugm.getAntiFlickerMode()
        see3cam50cugm.readStatistics()
        see3cam50cugm.readTemperature()
        see3cam50cugm.getMultiExposureTrigger()
        see3cam50cugm.getMultiFrameSet()
        see3cam50cugm.getUltraShortExposure()
        see3cam50cugm.getSoftwareTrigger()
    }
    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
}



