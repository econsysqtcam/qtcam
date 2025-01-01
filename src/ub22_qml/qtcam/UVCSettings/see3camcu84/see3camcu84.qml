/*
 * see3camcu84.qml -- extension settings for other cameras
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
import QtQuick.Layouts 1.1
import cameraenum 1.0
import econ.camera.see3camcu84 1.0
Item{
    width:240
    height:720

    property int denoiseMin: 0
    property int denoiseMax: 15
    property int frameRateMin: 0
    property int frameRateMax: 60
    property int expoCompMin: 50
    property int expoCompMax: 1000000

    property int qFactorMin: 10
    property int qFactorMax: 96

    property bool skipUpdateUIQFactor           : false

    property bool skipUpdateUIDenoise: false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIFrameRate: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIOnAntiFlickerMode:false

    property bool setButtonClicked: false

    signal destroyWindow()

    Timer {
        id: getCamValuesTimer
        interval: 500
        onTriggered: {
            see3camcu84.getExposureCompensation()
            see3camcu84.getFrameRateCtrlValue()
            stop()
        }
    }
    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            see3camcu84.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }
    Connections
    {
        target: root
        function onTakeScreenShot()
        {
            root.imageCapture(CommonEnums.BURST_SHOT);
        }
        function onGetVideoPinStatus()
        {
            root.enableVideoPin(true);
        }
        function onGetStillImageFormats()
        {
            var stillImageFormat = []
            stillImageFormat.push("jpg")
            stillImageFormat.push("bmp")
            stillImageFormat.push("raw")
            stillImageFormat.push("png")
            root.insertStillImageFormat(stillImageFormat);
        }
        function onAutoExposureSelected(autoExposureSelect)
        {
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        function onVideoResolutionChanged(){
            getCamValuesTimer.start()
        }
        function onPreviewFPSChanged(){
            getCamValuesTimer.start()
        }
        function onVideoColorSpaceChanged(){
            getCamValuesTimer.start()
        }
        function onSetExpCompensation(){
            see3camcu84.setExposureCompensation(exposureCompValue.text)
        }
        function onSetHIDControls(){
            see3camcu84.setFrameRateCtrlValue(frameRateSlider.value)
        }
        function onMouseRightClicked(x, Y, width, height){
            if(expRoiManual.enabled && expRoiManual.checked){
                see3camcu84.setROIAutoExposure(SEE3CAM_CU84.MANUAL_ROI, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        function onSkipFrameCount(){
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
        height: 1400
        ColumnLayout
        {
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

            ExclusiveGroup { id: effectInputGroup }
            RadioButton
            {
                id: rdoEffectNormal
                style:  econRadioButtonStyle
                text:   qsTr("Normal")
                exclusiveGroup: effectInputGroup
                activeFocusOnPress: true
                onClicked: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_NORMAL)
                }
                Keys.onReturnPressed:  {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_NORMAL)
                }
            }
            RadioButton
            {
                id: rdoEffectBW
                style:  econRadioButtonStyle
                text: qsTr("Black and White")
                exclusiveGroup: effectInputGroup
                activeFocusOnPress: true
                onClicked: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_BLACK_WHITE)
                }
                Keys.onReturnPressed: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_BLACK_WHITE)
                }
            }
            RadioButton
            {
                id: rdoEffectGreyScale
                style:  econRadioButtonStyle
                text: qsTr("GreyScale")
                exclusiveGroup: effectInputGroup
                activeFocusOnPress: true
                onClicked: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_GREYSCALE)
                }
                Keys.onReturnPressed: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_GREYSCALE)
                }
            }

            RadioButton
            {
                id: rdoEffectNegative
                style:  econRadioButtonStyle
                text: qsTr("Negative")
                exclusiveGroup: effectInputGroup
                activeFocusOnPress: true
                onClicked:
                {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_NEGATIVE)
                }
                Keys.onReturnPressed:
                {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_NEGATIVE)
                }
            }

            RadioButton
            {
                id: rdoEffectSketch
                style:  econRadioButtonStyle
                text: qsTr("Sketch")
                exclusiveGroup: effectInputGroup
                activeFocusOnPress: true
                onClicked: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_SKETCH)
                }
                Keys.onReturnPressed: {
                    see3camcu84.setSpecialMode(SEE3CAM_CU84.EFFECT_SKETCH)
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
                        see3camcu84.setDenoiseValue(deNoiseSlider.value)
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
                    see3camcu84.setBurstLength(burstLengthCombo.currentText)
                }
                skipUpdateUIOnBurstLength = true
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
                    see3camcu84.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                }
                Keys.onReturnPressed: {
                    see3camcu84.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                }
            }
            CheckBox
            {
                id: flipCtrlVertical
                activeFocusOnPress : true
                text: "Vertical"
                style: econCheckBoxStyle
                onClicked:{
                    see3camcu84.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                }
                Keys.onReturnPressed: {
                    see3camcu84.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                }
            }
        }

        Text
        {
            id: roiAutoExpMode
            text: "--- ROI - Auto Exposure ---"
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
                  // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                  // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                  // winSize is required only for manual mode
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
                    see3camcu84.setROIAutoExposure(SEE3CAM_CU84.MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                }
                skipUpdateUIOnExpWindowSize = true
            }
        }

        Text
        {
             id: exposureCompTextTitle
             text: "--- Exposure Compensation ---"
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
                Text
                {
                    id: exposureCompText
                    text: "value(µs)[50 - 1000000]"
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
                        see3camcu84.setExposureCompensation(exposureCompValue.text)
                        exposureCompSet.enabled = true
                    }
                    Keys.onReturnPressed:
                    {
                        exposureCompSet.enabled = false
                        setButtonClicked = true
                        see3camcu84.setExposureCompensation(exposureCompValue.text)
                        exposureCompSet.enabled = true
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
                    minimumValue: frameRateSlider.minimumValue
                    maximumValue: frameRateSlider.maximumValue
                    onValueChanged:  {
                        frameRateTextField.text = frameRateSlider.value
                        if(skipUpdateUIFrameRate){
                            see3camcu84.setFrameRateCtrlValue(frameRateSlider.value)
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
            ComboBox
            {
                id: antiFlickerCombo
                model: ListModel
                       {
                            ListElement { text: "AUTO" }
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
                Layout.alignment: Qt.AlignCenter

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


            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: flash_modes
                    text: "--- Flash Mode ---"
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
                ExclusiveGroup { id: flashModeGroup }
                Layout.alignment: Qt.AlignCenter
                RadioButton {
                    id: strobeOff
                    style:  econRadioButtonStyle
                    text:   qsTr("Strobe Off")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu84.setFlashMode(SEE3CAM_CU84.STROBE_OFF)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu84.setFlashMode(SEE3CAM_CU84.STROBE_OFF)
                    }
                }
                RadioButton {
                    id: strobeOn
                    style:  econRadioButtonStyle
                    text: qsTr("Strobe On")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu84.setFlashMode(SEE3CAM_CU84.STROBE_ON)
                    }
                    Keys.onReturnPressed: {
                        see3camcu84.setFlashMode(SEE3CAM_CU84.STROBE_ON)
                    }
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
                            see3camcu84.setQFactor(qFactorSlider.value)
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
                        setFaceDetectionMode()
                    }
                    Keys.onReturnPressed: {
                        setFaceDetectionMode()
                    }
                }
                RadioButton {
                    exclusiveGroup: faceRectGroup
                    id:faceRectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        setFaceDetectionMode()
                    }
                    Keys.onReturnPressed: {
                        setFaceDetectionMode()
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
                        setFaceDetectionMode()
                    }
                    Keys.onReturnPressed: {
                        setFaceDetectionMode()
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

    SEE3CAM_CU84{
        id: see3camcu84
        onSpecialModeReceived:
        {
            switch(specialMode)
            {
                case SEE3CAM_CU84.EFFECT_NORMAL:
                    rdoEffectNormal.checked = true
                    break
                case SEE3CAM_CU84.EFFECT_BLACK_WHITE:
                    rdoEffectBW.checked = true
                    break
                case SEE3CAM_CU84.EFFECT_GREYSCALE:
                    rdoEffectGreyScale.checked = true
                    break
                case SEE3CAM_CU84.EFFECT_NEGATIVE:
                    rdoEffectNegative.checked = true
                    break
                case SEE3CAM_CU84.EFFECT_SKETCH:
                    rdoEffectSketch.checked = true
                    break
            }
        }

        onDenoiseValueReceived:
        {
            skipUpdateUIDenoise = false
            deNoiseSlider.value = denoiseValue
            skipUpdateUIDenoise = true
        }

        onRoiAutoExpModeRecieved:
        {
            currentROIAutoExposureMode(roiMode, winSize)
        }

        onFlipMirrorModeReceived:
        {
            currentFlipMirrorMode(flipMode)
        }

        onExposureCompValueRecieved:
        {
             exposureCompValue.text = exposureCompVal
        }

        onFrameRateCtrlValueRecieved:
        {
            skipUpdateUIFrameRate = false
            frameRateSlider.value = frameRateVal
            skipUpdateUIFrameRate = true
        }
        onMinimumFramesReceived: {
            frameRateSlider.minimumValue = minimumFps
        }
        onMaximumFramesReceived: {
            frameRateSlider.maximumValue = maximumFps
        }

        onBurstLengthValueRecieved:
        {
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLenVal - 1
            skipUpdateUIOnBurstLength = true
        }

        onAntiFlickerModeRecieved:
        {
            switch(antiFlickerVal)
            {
                case SEE3CAM_CU84.MODE_AUTO:
                    skipUpdateUIOnAntiFlickerMode = false
                    antiFlickerCombo.currentIndex = 0
                    skipUpdateUIOnAntiFlickerMode = true
                    break
                case SEE3CAM_CU84.MODE_50Hz:
                    skipUpdateUIOnAntiFlickerMode = false
                    antiFlickerCombo.currentIndex = 1
                    skipUpdateUIOnAntiFlickerMode = true
                    break
                case SEE3CAM_CU84.MODE_60Hz:
                    skipUpdateUIOnAntiFlickerMode = false
                    antiFlickerCombo.currentIndex = 2
                    skipUpdateUIOnAntiFlickerMode = true
                    break
                case SEE3CAM_CU84.MODE_DISABLE://need to check the flags
                    skipUpdateUIOnAntiFlickerMode = false
                    antiFlickerCombo.currentIndex = 3
                    skipUpdateUIOnAntiFlickerMode = true
                    break
             }
        }

        onStreamModeReceived: {
            currentStreamModeReceived(streamMode)
        }

        onFlashModeReceived: {
            currentFlashModeReceived(flashMode)
        }

        onFaceDetectModeValueReceived: {
            currentFaceDetectionModeReceived(faceDetectMode, faceDetectOverlayRect, faceDetectEmbedDataValue)
        }

        onQFactorReceived: {
            skipUpdateUIQFactor = false
            qFactorSlider.value = qFactor
            skipUpdateUIQFactor = true
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
                see3camcu84.getExposureCompensation()
            }
        }
    }

    function currentStreamModeReceived(streamMode){
        if(streamMode == SEE3CAM_CU84.MASTER_MODE){
            masterMode.checked = true
            root.checkForTriggerMode(false)
            root.captureBtnEnable(true)
            root.videoRecordBtnEnable(true)
            root.startUpdatePreviewInMasterMode()
        }
        else if(streamMode == SEE3CAM_CU84.TRIGGER_MODE){
            triggerMode.checked = true
            root.videoRecordBtnEnable(false)
            root.captureBtnEnable(false)
            root.checkForTriggerMode(true)
            root.stopUpdatePreviewInTriggerMode()
        }
    }

    function currentFlashModeReceived(flashMode){
        if(flashMode == SEE3CAM_CU84.STROBE_ON){
            strobeOn.checked = true
        }
        else if(flashMode == SEE3CAM_CU84.STROBE_OFF){
            strobeOff.checked = true
        }
    }

    function currentFaceDetectionModeReceived(faceDetectMode, faceDetectOverlayRect, faceDetectEmbedDataValue){
        if(faceDetectMode === SEE3CAM_CU84.FaceRectEnable){
            faceRectEnable.checked = true
            if(faceDetectEmbedDataValue === SEE3CAM_CU84.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }
            if(faceDetectOverlayRect === SEE3CAM_CU84.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }
        }else if(faceDetectMode === SEE3CAM_CU84.FaceRectDisable){
            faceRectDisable.checked = true
            if(faceDetectEmbedDataValue === SEE3CAM_CU84.FaceDetectEmbedDataEnable){
                faceDetectEmbedData.checked = true
            }else{
                faceDetectEmbedData.checked = false
            }
            if(faceDetectOverlayRect === SEE3CAM_CU84.FaceDetectOverlayRectEnable){
                overlayRect.checked = true
            }else{
                overlayRect.checked = false
            }
        }
    }

    function setFaceDetectionMode()
    {
        if(faceRectEnable.checked == true)
        {
            if((faceDetectEmbedData.checked == true) && (overlayRect.checked == true)){
                see3camcu84.setFaceDetection(true, true, true)
            }else if((faceDetectEmbedData.checked == true) && (overlayRect.checked == false)){
                see3camcu84.setFaceDetection(true, false, true)
            }else if((faceDetectEmbedData.checked == false) && (overlayRect.checked == true)){
                see3camcu84.setFaceDetection(true, true, false)
            }else{
                see3camcu84.setFaceDetection(true, false, false)
            }
        }else if(faceRectDisable.checked == true){
            if((faceDetectEmbedData.checked == true) && (overlayRect.checked == true)){
                see3camcu84.setFaceDetection(false, true, true)
            }else if((faceDetectEmbedData.checked == true) && (overlayRect.checked == false)){
                see3camcu84.setFaceDetection(false, false, true)
            }else if((faceDetectEmbedData.checked == false) && (overlayRect.checked == true)){
                see3camcu84.setFaceDetection(false, true, false)
            }else{
                see3camcu84.setFaceDetection(false, false, false)
            }
        }
    }

    function currentROIAutoExposureMode(roiMode, winSize)
    {
        switch(roiMode){
            case SEE3CAM_CU84.FULL_ROI:
                expRoiFull.checked = true
                expRoiFull.enabled = true

                //To disable comboBox in full roi mode
                autoExpoWinSizeCombo.enabled = false
                autoExpoWinSizeCombo.opacity = 0.1
                break
            case SEE3CAM_CU84.MANUAL_ROI:
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


    function setROIAutoExposure(){
        if(expRoiFull.checked == true){
            see3camcu84.setROIAutoExposure(SEE3CAM_CU84.FULL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);

            //To disable comboBox in full roi mode
            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.opacity = 0.1
        }else if(expRoiManual.checked == true){
            see3camcu84.setROIAutoExposure(SEE3CAM_CU84.MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)

            //To disable comboBox in manual roi mode
            autoExpoWinSizeCombo.enabled = true
            autoExpoWinSizeCombo.opacity = 1
        }
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case SEE3CAM_CU84.VERTICAL:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case SEE3CAM_CU84.HORIZONTAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case SEE3CAM_CU84.BOTH:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        default:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        }
    }

    function setWakeOnMode(mode)
    {
        switch(mode)
        {
        case SEE3CAM_CU84.ENABLE:
            wakeOnEnable.checked  = true
            wakeOnDisable.checked = false
            break;
        case SEE3CAM_CU84.DISABLE:
            wakeOnEnable.checked  = false
            wakeOnDisable.checked = true
            break;
        }
    }

    function setAntiFlickerMode()
    {
        if(antiFlickerCombo.currentIndex === 0)
            see3camcu84.setAntiFlickerMode(SEE3CAM_CU84.MODE_AUTO)
        else if(antiFlickerCombo.currentIndex === 1)
            see3camcu84.setAntiFlickerMode(SEE3CAM_CU84.MODE_50Hz)
        else if(antiFlickerCombo.currentIndex === 2)
            see3camcu84.setAntiFlickerMode(SEE3CAM_CU84.MODE_60Hz)
        else if(antiFlickerCombo.currentIndex === 3)
            see3camcu84.setAntiFlickerMode(SEE3CAM_CU84.MODE_DISABLE)
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            //To enable exposure compensation when device is in manual exposure mode in UVC
            root.enableDisableExposureCompensation(autoExposureSelect)

            expRoiManual.enabled   = true
            expRoiFull.enabled     = true

            expRoiManual.opacity   = 1
            expRoiFull.opacity     = 1

            if(expRoiManual.checked)
                autoExpoWinSizeCombo.enabled = true
            if(expRoiFull.checked)
                autoExpoWinSizeCombo.enabled = false

            exposureCompValue.enabled = true
            exposureCompValue.opacity = 1
            exposureCompSet.enabled   = true
            exposureCompSet.opacity   = 1
            exposureCompText.opacity  = 1
        }else{
            //To disable exposure compensation when device is in manual exposure mode in UVC
            root.enableDisableExposureCompensation(autoExposureSelect)

            expRoiManual.enabled   = false
            expRoiFull.enabled     = false

            expRoiManual.opacity   = 0.1
            expRoiFull.opacity     = 0.1

            autoExpoWinSizeCombo.enabled = false

            exposureCompValue.enabled = false
            exposureCompValue.opacity = 0.1
            exposureCompSet.enabled   = false
            exposureCompSet.opacity   = 0.1
            exposureCompText.opacity  = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    function enableFaceDetectEmbedData(){
        if(see3camcu84.setFaceDetection(faceRectEnable.checked, overlayRect.checked, faceDetectEmbedData.checked)){
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_CU84_Face_Detection for more details"))
            }
        }
    }

    function setMasterMode(){
        see3camcu84.setStreamMode(SEE3CAM_CU84.MASTER_MODE)
        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function setTriggerMode(){
        see3camcu84.setStreamMode(SEE3CAM_CU84.TRIGGER_MODE)
        root.stopUpdatePreviewInTriggerMode()
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    //DefaultValue
    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        defaultValue.enabled = false

        if(see3camcu84.setToDefaultValues()){
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        getCamValuesTimer.start()
        see3camcu84.getSpecialMode()
        see3camcu84.getDenoiseValue()
        see3camcu84.getAutoExpROIModeAndWindowSize()
        see3camcu84.getBurstLength()
        see3camcu84.getOrientation()
        see3camcu84.getAntiFlickerMode()
        see3camcu84.getStreamMode()
        see3camcu84.getFlashMode()
        see3camcu84.getQFactor()
        see3camcu84.getFaceDetectMode()
    }

    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
    Component.onDestruction: {
        // Stopping stillTimeOutTimer when this Component is destroyed
        vidstreamproperty.stopStillTimeOutTimer();
    }
}
