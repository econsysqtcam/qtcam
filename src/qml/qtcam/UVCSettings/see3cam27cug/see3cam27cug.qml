/*
 * see3cam27cug.qml -- extension settings for other cameras
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
import econ.camera.see3cam_27cug 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:240
    height:720
    property int stillFormatCurrentIndex
    property int stillResolutionCurrentIndex

    property int brightnessMin: 0
    property int brightnessMax: 8

    property int contrastMin: 0
    property int contrastMax: 31

    property int gainMin: 0
    property int gainMax: 100

    property int expoCompMin: 100
    property int expoCompMax: 1000000

    property variant secondWindow
    property bool setButtonClicked: false

    Timer {
        id: getexposureCompTimer
        interval: 500
        onTriggered: {
            see3cam27cug.getMinimumExposureCompensation()
            see3cam27cug.getMaximumExposureCompensation()
            stop()
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
        onAutoExposureSelected:
        {
            enableDisableAutoExposureControls(autoExposureSelect)
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
        onVideoResolutionChanged:{
            getexposureCompTimer.start()
        }
        onPreviewFPSChanged:{
            getexposureCompTimer.start()
        }
        onVideoColorSpaceChanged:{
            getexposureCompTimer.start()
        }
        onSetExpCompensation:{
            see3cam27cug.setMaximumExposureCompensation(maxExpTextField.text)
            see3cam27cug.setMinimumExposureCompensation(minExpTextField.text)
        }
        onSkipFrameCount:{
            root.setSkipFrameCount(5)
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

                //stream mode
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

                //camera mode
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
                RowLayout{
                   ExclusiveGroup { id: cameraModeGroup }
                   spacing: 25
                   RadioButton {
                       id: irMode
                       style:  econRadioButtonStyle
                       text: qsTr("IR")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       onClicked: {
                           root.camModeEnabled(See3CAM_27CUG.IR)
                           setIrMode()
                           if(triggerMode.checked == true)
                           {
                               root.checkForTriggerMode(true)
                               root.captureBtnEnable(false)
                               root.videoRecordBtnEnable(false)
                           }
                       }
                       Keys.onReturnPressed: {
                           root.camModeEnabled(See3CAM_27CUG.IR)
                           setIrMode()
                           if(triggerMode.checked == true)
                           {
                               root.checkForTriggerMode(true)
                               root.captureBtnEnable(false)
                               root.videoRecordBtnEnable(false)
                           }
                       }
                   }
                   RadioButton {
                       id: rgbMode
                       style:  econRadioButtonStyle
                       text: qsTr("RGB")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       onClicked: {
                          root.camModeEnabled(See3CAM_27CUG.RGB)
                          setRgbMode()
                           if(triggerMode.checked == true)
                           {
                               root.checkForTriggerMode(true)
                               root.captureBtnEnable(false)
                               root.videoRecordBtnEnable(false)
                           }
                       }
                       Keys.onReturnPressed: {
                          root.camModeEnabled(See3CAM_27CUG.RGB)
                          setRgbMode()
                           if(triggerMode.checked == true)
                           {
                               root.checkForTriggerMode(true)
                               root.captureBtnEnable(false)
                               root.videoRecordBtnEnable(false)
                           }
                       }
                   }
                   RadioButton {
                       id: irrgbMode
                       style:  econRadioButtonStyle
                       text: qsTr("IR-RGB")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       onClicked: {
                           root.camModeEnabled(See3CAM_27CUG.RGB_IR_ENABLE)
                           setIrRgbMode()

                           //function to create IR window
                           root.irPreviewWindow()
                           videoRecordBtnEnable(false)
                           if(triggerMode.checked == true)
                           {
                               root.checkForTriggerMode(true)
                               root.captureBtnEnable(false)
                               root.videoRecordBtnEnable(false)
                           }
                       }
                       Keys.onReturnPressed: {
                           root.camModeEnabled(See3CAM_27CUG.RGB_IR_ENABLE)
                           setIrRgbMode()

                           root.irPreviewWindow()
                           videoRecordBtnEnable(false)
                           if(triggerMode.checked == true)
                           {
                               root.checkForTriggerMode(true)
                               root.captureBtnEnable(false)
                               root.videoRecordBtnEnable(false)
                           }
                       }
                   }
               }

                //Strobe mode
                Text {
                    id: strobeMode
                    text: "--- Strobe Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 60
                    ExclusiveGroup { id: strobeModeGroup }

                    RadioButton {
                        id: strobeON
                        style:  econRadioButtonStyle
                        text:   qsTr("Strobe ON")
                        exclusiveGroup: strobeModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam27cug.setStrobeMode(See3CAM_27CUG.STROBE_ON)
                        }
                        Keys.onReturnPressed: {
                            see3cam27cug.setStrobeMode(See3CAM_27CUG.STROBE_ON)
                        }
                    }
                    RadioButton {
                        id: strobeOFF
                        style:  econRadioButtonStyle
                        text:   qsTr("Strobe OFF")
                        exclusiveGroup: strobeModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3cam27cug.setStrobeMode(See3CAM_27CUG.STROBE_OFF)
                        }
                        Keys.onReturnPressed: {
                            see3cam27cug.setStrobeMode(See3CAM_27CUG.STROBE_OFF)
                        }
                    }
                }

                //Flip Mode
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
                            see3cam27cug.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3cam27cug.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            defaultValue.enabled = true
                            see3cam27cug.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3cam27cug.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }

                }

                //Brightness For IR
                Text
                {
                    id: brightnessText
                    text: "--- Brightness For IR ---"
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
                        id: brightnessSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: brightnessMin
                        maximumValue: brightnessMax
                        onValueChanged:  {
                            brightnessTextField.text = brightnessSlider.value
                            see3cam27cug.setBrightnessForIR(brightnessSlider.value)
                        }
                    }
                    TextField
                    {
                        id: brightnessTextField
                        text: brightnessSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: brightnessSlider.minimumValue; top: brightnessSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                brightnessSlider.value = brightnessTextField.text
                            }
                        }
                    }
                }

                //Contrast For IR
                Text
                {
                    id: contrastText
                    text: "--- Contrast For IR ---"
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
                        id: contrastSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: contrastMin
                        maximumValue: contrastMax
                        onValueChanged:  {
                             contrastTextField.text = contrastSlider.value
                             see3cam27cug.setContrastForIR(contrastSlider.value)
                        }
                    }
                    TextField
                    {
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

                //Gain For IR
                Text
                {
                    id: gainText
                    text: "--- Gain For IR ---"
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
                        id: gainSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: gainMin
                        maximumValue: gainMax
                        onValueChanged:  {
                            gainTextField.text = gainSlider.value
                            see3cam27cug.setGainForIR(gainSlider.value)
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

                //Minimum Exposure Compensation
                Text {
                        id: minExposureCompTextTitle
                        text: "--- Min Exposure Compensation ---"
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
                            id: minExposureCompText
                            text: "value(µs)[100 - 1000000]"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            width: 80
                            wrapMode: Text.WordWrap
                            opacity: 1
                        }
                        TextField {
                            id: minExpTextField
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
                            id: minExpSetButton
                            activeFocusOnPress : true
                            text: "Set"
                            tooltip: "You can set the required exposure compensation value by changing the
        value in the text box and click the Set button"
                            style: econButtonStyle
                            enabled: true
                            opacity: 1
                            implicitHeight: 25
                            implicitWidth: 60
                            onClicked: {
                                minExpSetButton.enabled = false
                                setButtonClicked = true
                                see3cam27cug.setMinimumExposureCompensation(minExpTextField.text)
                                minExpSetButton.enabled = true
                            }
                            Keys.onReturnPressed: {
                                minExpSetButton.enabled = false
                                setButtonClicked = true
                                see3cam27cug.setMinimumExposureCompensation(minExpTextField.text)
                                minExpSetButton.enabled = true
                            }
                        }
                    }

                //Maximum Exposure Compensation
                Text {
                        id: maxExposureCompTextTitle
                        text: "--- Max Exposure Compensation ---"
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
                            id: maxExposureCompText
                            text: "value(µs)[100 - 1000000]"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            width: 80
                            wrapMode: Text.WordWrap
                            opacity: 1
                        }
                        TextField {
                            id: maxExpTextField
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
                            id: maxExposureCompSet
                            activeFocusOnPress : true
                            text: "Set"
                            tooltip: "You can set the required exposure compensation value by changing the
        value in the text box and click the Set button"
                            style: econButtonStyle
                            enabled: true
                            opacity: 1
                            implicitHeight: 25
                            implicitWidth: 60
                            onClicked: {
                                maxExposureCompSet.enabled = false
                                setButtonClicked = true
                                see3cam27cug.setMaximumExposureCompensation(maxExpTextField.text)
                                maxExposureCompSet.enabled = true
                            }
                            Keys.onReturnPressed: {
                                maxExposureCompSet.enabled = false
                                setButtonClicked = true
                                see3cam27cug.setMaximumExposureCompensation(maxExpTextField.text)
                                maxExposureCompSet.enabled = true
                            }
                        }
                    }
                //Default Button
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
    See3CAM_27CUG{
        id: see3cam27cug

        onFlipMirrorModeChanged:
        {
            currentFlipMirrorMode(flipMirrorModeValues)
        }

        onStrobeModeChanged:
        {
            currentStrobeMode(strobeModeValues)
        }

        onStreamModeChanged:
        {
            if(streamModeValues == See3CAM_27CUG.MASTER)
            {
                masterMode.checked = true            
                root.checkForTriggerMode(false)
                root.captureBtnEnable(true)
                if(irrgbMode.checked == false){
                    root.videoRecordBtnEnable(true)
                }
                root.startUpdatePreviewInMasterMode()
            }
            else if(streamModeValues == See3CAM_27CUG.TRIGGER)
            {
                triggerMode.checked = true                
                root.videoRecordBtnEnable(false)
                root.captureBtnEnable(false)
                root.checkForTriggerMode(true)
            }
        }

        onCameraModeChanged:
        {
            if(cameraModeValues == See3CAM_27CUG.IR)
            {
                irMode.checked = true

            }
            else if(cameraModeValues == See3CAM_27CUG.RGB)
            {
                rgbMode.checked = true
            }
            else if(cameraModeValues == See3CAM_27CUG.RGB_IR_ENABLE)
            {
                root.videoRecordBtnEnable(false)
                irrgbMode.checked = true
                root.irPreviewWindow()
            }
        }

        onBrightnessChanged:{
            brightnessSlider.value = brightness
        }

        onContrastChanged: {
            contrastSlider.value = contrast
        }

        onGainChanged:{
            gainSlider.value = gain
        }

        onMinimumExposureReceive:{
                minExpTextField.text = minExpValue
        }

        onMaximumExposureReceive:{
                maxExpTextField.text = maxExpValue
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
                see3cam27cug.getMinimumExposureCompensation()
                see3cam27cug.getMaximumExposureCompensation()
            }
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function getSerialNumber() {
        see3cam27cug.get27CugSerialNumber()
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function contrastUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        contrast.opacity = 1
        contrast_Slider.enabled = true
        contrast_value.opacity = 1
        contrastControlId = controlID
        contrast_Slider.minimumValue = controlMinValue
        contrast_Slider.maximumValue = controlMaxValue
        contrast_Slider.value = controlDefaultValue
        contrast_Slider.stepSize = controlStepSize
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case See3CAM_27CUG.NORMAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        case See3CAM_27CUG.VERTICAL:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case See3CAM_27CUG.HORIZONTAL:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case See3CAM_27CUG.ROTATE_180:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        }
    }

    function currentStrobeMode(mode){
        switch(mode){
            case See3CAM_27CUG.STROBE_OFF:
                defaultValue.enabled = true
                strobeOFF.checked    = true
                strobeON.checked     = false
                break;
            case See3CAM_27CUG.STROBE_ON:
                defaultValue.enabled = true
                strobeON.checked     = true
                strobeOFF.checked    = false
                break;
        }
    }

    function setMasterMode(){
        defaultValue.enabled = true
        see3cam27cug.setStreamMode(See3CAM_27CUG.MASTER)
        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)

        if(irrgbMode.checked == false)
        {
            root.videoRecordBtnEnable(true)
        }
        root.captureBtnEnable(true)

        //To enable preview in master mode
        root.clearBufferInTriggerMode(false)
    }

    function setTriggerMode(){
        defaultValue.enabled = true
        see3cam27cug.setStreamMode(See3CAM_27CUG.TRIGGER)
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function setRgbMode(){
        defaultValue.enabled = true
        see3cam27cug.setCameraMode(See3CAM_27CUG.RGB)
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)  
        root.startUpdatePreviewInMasterMode()

        //To disable IR controls in RGB mode
        disableIrControls()
    }

    function setIrMode(){
        defaultValue.enabled = true
        see3cam27cug.setCameraMode(See3CAM_27CUG.IR)
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()

        //To enable IR controls in IR mode
        enableIrControls()
    }

    function setIrRgbMode(){
        defaultValue.enabled = true
        see3cam27cug.setCameraMode(See3CAM_27CUG.RGB_IR_ENABLE)
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(false)
        root.startUpdatePreviewInMasterMode()

        //To enable IR controls in IR-RGB mode
        enableIrControls()
    }

    function enableIrControls()
    {
        brightnessSlider.enabled = true
        brightnessSlider.opacity = 1
        brightnessTextField.enabled = true
        brightnessTextField.opacity = 1

        contrastSlider.enabled = true
        contrastSlider.opacity = 1
        contrastTextField.enabled = true
        contrastTextField.opacity = 1

        gainSlider.enabled = true
        gainSlider.opacity = 1
        gainTextField.enabled = true
        gainTextField.opacity = 1
    }

    function disableIrControls()
    {
        brightnessSlider.enabled = false
        brightnessSlider.opacity = 0.1
        brightnessTextField.enabled = false
        brightnessTextField.opacity = 0.1

        contrastSlider.enabled = false
        contrastSlider.opacity = 0.1
        contrastTextField.enabled = false
        contrastTextField.opacity = 0.1

        gainSlider.enabled = false
        gainSlider.opacity = 0.1
        gainTextField.enabled = false
        gainTextField.opacity = 0.1
    }

    function setToDefaultValues(){
        defaultValue.enabled = false //To avoid multiple clicks over Default button
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(false)
        root.irPreviewWindow()

        root.camModeEnabled(See3CAM_27CUG.RGB_IR_ENABLE)

        if(see3cam27cug.setToDefaultValues())
        {
            getCurrentValuesFromCamera()
        }

        defaultValue.enabled = true
        root.startUpdatePreviewInMasterMode()
    }

    function getCurrentValuesFromCamera(){
        see3cam27cug.getCameraMode()
        see3cam27cug.getOrientation()
        see3cam27cug.getStreamMode()
        see3cam27cug.getStrobeMode()
        see3cam27cug.getBrightnessForIR()
        see3cam27cug.getContrastForIR()
        see3cam27cug.getGainForIR()
        see3cam27cug.getMinimumExposureCompensation()
        see3cam27cug.getMaximumExposureCompensation()
        if(triggerMode.checked == true)
        {
            root.checkForTriggerMode(true)
            root.captureBtnEnable(false)
            root.videoRecordBtnEnable(false)
        }

        if(rgbMode.checked == true)
        {
            //To disable IR controls in RGB mode
            disableIrControls()
        }
        else
        {
            //To enable IR controls in IR mode
            enableIrControls()
        }
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            //To enable exposure compensation when device is in manual exposure mode in UVC
            root.enableDisableExposureCompensation(autoExposureSelect)

            maxExposureCompTextTitle.enabled = true
            maxExposureCompText.enabled = true
            maxExpTextField.enabled = true
            maxExposureCompSet.enabled = true

            maxExposureCompTextTitle.opacity = 1
            maxExposureCompText.opacity = 1
            maxExpTextField.opacity = 1
            maxExposureCompSet.opacity = 1

            minExposureCompTextTitle.enabled = true
            minExposureCompText.enabled = true
            minExpTextField.enabled = true
            minExpSetButton.enabled = true

            minExposureCompTextTitle.opacity = 1
            minExposureCompText.opacity = 1
            minExpTextField.opacity = 1
            minExpSetButton.opacity = 1

        }else{
            //To disable exposure compensation when device is in manual exposure mode in UVC
            root.enableDisableExposureCompensation(autoExposureSelect)

            maxExposureCompTextTitle.enabled = false
            maxExposureCompText.enabled = false
            maxExpTextField.enabled = false
            maxExposureCompSet.enabled = false

            maxExposureCompTextTitle.opacity = 0.1
            maxExposureCompText.opacity = 0.1
            maxExpTextField.opacity = 0.1
            maxExposureCompSet.opacity = 0.1

            minExposureCompTextTitle.enabled = false
            minExposureCompText.enabled = false
            minExpTextField.enabled = false
            minExpSetButton.enabled = false

            minExposureCompTextTitle.opacity = 0.1
            minExposureCompText.opacity = 0.1
            minExpTextField.opacity = 0.1
            minExpSetButton.opacity = 0.1
        }
    }

    Component.onCompleted: {
        getCurrentValuesFromCamera()
        root.enableTimerToRetrieveFrame()
    }
    Component.onDestruction: {
        // Stopping stillTimeOutTimer when this Component is destroyed
        vidstreamproperty.stopStillTimeOutTimer();
    }
}
