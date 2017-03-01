/*
 * uvc_30.qml -- extension settings for other cameras
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
import econ.camera.see3cam30 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:240
    height:800
    property int defaultNoiseVal: 8
    property int denoiseMin: 0
    property int denoiseMax: 15
    property int qFactorMin: 10
    property int qFactorMax: 96

    // Flags to prevent setting values in camera when getting the values from camera
    property bool skipUpdateUIOnSetttings : false
    property bool skipUpdateUIOnAFWindowSize: false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnBurstLength: false

   Timer {
        id: burstShotTimer
        interval: 1000
        onTriggered: {
            root.imageCapture(CommonEnums.BURST_SHOT);
            stop()
        }
    }

    // Used when selecting auto focus in image Quality settings menu
    Timer {
        id: getAutoFocusControlValues
        interval: 500
        onTriggered: {
            see3cam30.getAutoFocusROIModeAndWindowSize()
            stop()
        }
    }

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            see3cam30.getAutoExpROIModeAndWindowSize()
            stop()
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
                id: special_effects
                text: "--- Special Effects ---"
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

                ExclusiveGroup { id: effectInputGroup }
                RadioButton {
                    id: effectNormal
                    style:  econRadioButtonStyle
                    text:   qsTr("Normal")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_NORMAL)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_NORMAL)
                    }

                }
                RadioButton {
                    id: effectBW
                    style:  econRadioButtonStyle
                    text: qsTr("Black and White")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_BLACK_WHITE)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_BLACK_WHITE)
                    }
                }
                RadioButton {
                    id: effectNegative
                    style:  econRadioButtonStyle
                    text: qsTr("Negative")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_NEGATIVE)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_NEGATIVE)
                    }

                }
                RadioButton {
                    id: effectGrayscale
                    style:  econRadioButtonStyle
                    text: qsTr("Grayscale")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_GREYSCALE)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_GREYSCALE)
                    }
                }
                RadioButton {
                    id: effectSketch
                    style:  econRadioButtonStyle
                    text: qsTr("Sketch")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_SKETCH)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setEffectMode(See3Cam30.EFFECT_SKETCH)
                    }
                }

            }
            Text {
                id: autoFocusMode
                text: "--- Auto Focus Mode ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            Row{
              spacing:10
              ExclusiveGroup { id: afgroup }
              RadioButton {
                  exclusiveGroup: afgroup
                  id: radioContin
                  text: "Continuous"
                  activeFocusOnPress: true
                  style: econRadioButtonStyle
                  opacity: enabled ? 1 : 0.1
                  onClicked: {
                    see3cam30.setAutoFocusMode(See3Cam30.Continuous);
                  }
                  Keys.onReturnPressed: {
                    see3cam30.setAutoFocusMode(See3Cam30.Continuous);
                  }
              }
            }
            Row{
                spacing:25
                RadioButton {
                    exclusiveGroup: afgroup
                    id: radioOneshot
                    text: "One-Shot"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        see3cam30.setAutoFocusMode(See3Cam30.OneShot);
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setAutoFocusMode(See3Cam30.OneShot);
                    }
                }
                Button {
                    id: trigger
                    activeFocusOnPress : true
                    text: "Trigger"
                    style: econButtonStyle
                    enabled: (radioOneshot.enabled && radioOneshot.checked) ? true : false
                    opacity: (radioOneshot.enabled && radioOneshot.checked) ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 120                    
                    onClicked: {
                        trigger.enabled = false
                        see3cam30.setAutoFocusMode(See3Cam30.OneShot);
                        trigger.enabled = true
                    }
                    Keys.onReturnPressed: {
                        trigger.enabled = false
                        see3cam30.setAutoFocusMode(See3Cam30.OneShot);
                        trigger.enabled = true
                    }
                }
            }

            Text {
                id: denoiseText
                text: "--- De-Noise ---"
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
                    id: deNoiseSlider
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: denoiseMin
                    maximumValue: denoiseMax
                    onValueChanged:  {
                        deNoiseTextField.text = deNoiseSlider.value
                        if(skipUpdateUIOnSetttings){
                            see3cam30.setDenoiseValue(deNoiseSlider.value)
                        }
                        skipUpdateUIOnSetttings = true
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

            Text {
                id: flipText
                text: "--- Flip Control ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Row{
                spacing: 55
                CheckBox {
                    id: flipCtrlHorizotal
                    activeFocusOnPress : true
                    text: "Horizontal"
                    style: econCheckBoxStyle
                    onClicked:{                        
                        see3cam30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {                        
                        see3cam30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3cam30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
            }
            Text {
                id: roiAutoFocusMode
                text: "--- ROI - Auto Focus ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            Row{
                  spacing:55
                  ExclusiveGroup { id: roiAfgroup }
                  RadioButton {
                      exclusiveGroup: roiAfgroup
                      id: afCentered
                      text: "Centered"
                      activeFocusOnPress: true
                      style: econRadioButtonStyle
                      opacity: afCentered.enabled ? 1 : 0.1
                      // setROIAutoFoucs() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                      // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                      // winSize is required only for manual mode
                      onClicked: {
                            see3cam30.setROIAutoFoucs(See3Cam30.AFCentered, 0, 0, 0, 0, 0);
                            afWindowSizeCombo.enabled = false
                      }
                      Keys.onReturnPressed: {
                            see3cam30.setROIAutoFoucs(See3Cam30.AFCentered, 0, 0, 0, 0, 0);
                            afWindowSizeCombo.enabled = false
                      }
                  }
                  RadioButton {
                      exclusiveGroup: roiAfgroup
                      id: afManual
                      text: "Manual"
                      activeFocusOnPress: true
                      style: econRadioButtonStyle
                      opacity: afManual.enabled ? 1 : 0.1
                      onClicked: {
                            see3cam30.setROIAutoFoucs(See3Cam30.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                            afWindowSizeCombo.enabled = true
                      }
                      Keys.onReturnPressed: {
                            see3cam30.setROIAutoFocusMode(See3Cam30.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText);
                            afWindowSizeCombo.enabled = true
                      }
                  }
            }

            ComboBox
            {
                id: afWindowSizeCombo
                enabled: (afManual.enabled && afManual.checked) ? true : false
                opacity: (afManual.enabled && afManual.checked) ? 1 : 0.1
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
                    if(skipUpdateUIOnAFWindowSize){
                        see3cam30.setROIAutoFoucs(See3Cam30.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                    }
                    skipUpdateUIOnAFWindowSize = true
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
                          see3cam30.setROIAutoExposure(See3Cam30.AutoExpFull, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = false
                      }
                      Keys.onReturnPressed: {
                          see3cam30.setROIAutoExposure(See3Cam30.AutoExpFull, 0, 0, 0, 0, 0);
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
                          see3cam30.setROIAutoExposure(See3Cam30.AutoExpManual, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = true
                      }
                      Keys.onReturnPressed: {
                          see3cam30.setROIAutoExposure(See3Cam30.AutoExpManual, 0, 0, 0, 0, 0);
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
                        see3cam30.setROIAutoExposure(See3Cam30.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                    }
                    skipUpdateUIOnExpWindowSize = true
                }
            }
            Text {
                id: enableDisableAFRectText
                text: "--- Enable/Disable AF Rectangle ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            Row{
                spacing:75
                ExclusiveGroup { id: afRectGroup }
                RadioButton {
                    exclusiveGroup: afRectGroup
                    id: rectEnable
                    text: "Enable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        see3cam30.enableDisableAFRectangle(true)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.enableDisableAFRectangle(true)
                    }
                }
                RadioButton {
                    exclusiveGroup: afRectGroup
                    id:rectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        see3cam30.enableDisableAFRectangle(false)
                    }
                    Keys.onReturnPressed: {
                        see3cam30.enableDisableAFRectangle(false)
                    }
                }
            }
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
                        if(skipUpdateUIOnSetttings){
                            see3cam30.setQFactor(qFactorSlider.value)
                        }
                        skipUpdateUIOnSetttings = true
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
                        see3cam30.setBurstLength(burstLengthCombo.currentText)
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
                    onClicked: {
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
            Row{
                Button {
                    id: dummy
                    opacity: 0
                }
            }

        }
    }


    Component {
        id: econComboBoxStyle
        ComboBoxStyle {
            background: Image {
                id: comboBkgrnd
                source: "../../Views/images/device_box.png"
                Rectangle {
                    width: comboBkgrnd.sourceSize.width  - 28
                    height: comboBkgrnd.sourceSize.height
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

    See3Cam30 {
        id: see3cam30
        onEffectModeChanged: {
            currentEffectMode(effectMode)
        }
        onDenoiseValueChanged:{
            deNoiseSlider.value = denoiseValue
        }
        onAfModeChanged:{
            currentAfMode(afMode)
        }
        onQFactorChanged:{
              qFactorSlider.value = qFactor
        }
        onRoiAfModeChanged:{
            if(roiMode == See3Cam30.AFCentered){
                afCentered.checked = true
                afWindowSizeCombo.enabled = false
            }else if(roiMode == See3Cam30.AFManual){
                skipUpdateUIOnAFWindowSize = false
                afManual.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam30.AFDisabled){
                afCentered.enabled = false
                afManual.enabled = false
                afWindowSizeCombo.enabled = false
            }
        }
        onRoiAutoExpModeChanged:{
            if(roiMode == See3Cam30.AutoExpFull){
                autoexpFull.checked = true
                autoExpoWinSizeCombo.enabled = false
            }else if(roiMode == See3Cam30.AutoExpManual){
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam30.AutoExpDisabled){
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
            }
        }
        onBurstLengthChanged:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onAfRectModeChanged:{
            if(afRectMode == See3Cam30.AFRectEnable){
                rectEnable.checked = true
            }else if(afRectMode == See3Cam30.AFRectDisable){
                rectDisable.checked = true
            }
        }
        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
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


    Component.onCompleted:{
        skipUpdateUIOnSetttings = false
        see3cam30.getEffectMode()
        see3cam30.getDenoiseValue()
        see3cam30.getAutoFocusMode()
        see3cam30.getQFactor()
        see3cam30.getBurstLength()
        see3cam30.getAutoFocusROIModeAndWindowSize()
        see3cam30.getAutoExpROIModeAndWindowSize()
        see3cam30.getAFRectMode()
        see3cam30.getOrientation()
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case See3Cam30.FLIP_ON_MIRROR_ON:
                 flipCtrlVertical.checked = true
                 flipCtrlHorizotal.checked = true
                break;
            case See3Cam30.FLIP_OFF_MIRROR_ON:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case See3Cam30.FLIP_ON_MIRROR_OFF:
                 flipCtrlVertical.checked = false
                 flipCtrlHorizotal.checked = true
                break;
            case See3Cam30.FLIP_OFF_MIRROR_OFF:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
        }
    }

    // get serial number
    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    // get firmware version
    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
    }

    // set to default values
    function setToDefaultValues(){
        defaultValue.enabled = false
        see3cam30.setToDefault()
        see3cam30.getEffectMode()
        see3cam30.getAutoFocusMode()
        see3cam30.getDenoiseValue()
        see3cam30.getQFactor()
        see3cam30.getBurstLength()
        see3cam30.getAutoFocusROIModeAndWindowSize()
        see3cam30.getAutoExpROIModeAndWindowSize()
        see3cam30.getAFRectMode()
        see3cam30.getOrientation()
        defaultValue.enabled = true
    }

    // When auto focus is selected in Image Quality Settings, then focus modes and auto focus ROI modes will be enabled
    function enableDisableAutoFocusUIControls(autoFocusSelect){
        if(autoFocusSelect){
            radioContin.enabled = true
            radioOneshot.enabled = true
            trigger.enabled = true
            afCentered.enabled = true
            afManual.enabled = true
            if(afCentered.checked)
                afWindowSizeCombo.enabled = false
            if(afManual.checked)
                afWindowSizeCombo.enabled = true
            rectEnable.enabled = true
            rectEnable.opacity = 1
            rectDisable.enabled = true
            rectDisable.opacity = 1
            radioContin.opacity = 1
            radioOneshot.opacity = 1
            afCentered.opacity = 1
            afManual.opacity = 1
        }else{
            radioContin.enabled = false
            radioOneshot.enabled = false
            trigger.enabled = false
            afCentered.enabled = false
            afManual.enabled = false
            afWindowSizeCombo.enabled = false
            radioContin.opacity = 0.1
            radioOneshot.opacity = 0.1
            afCentered.opacity = 0.1
            afManual.opacity = 0.1
            rectEnable.enabled = false
            rectEnable.opacity = 0.1
            rectDisable.enabled = false
            rectDisable.opacity = 0.1
        }
        getAutoFocusControlValues.start()
    }

    // When auto exposure is selected in Image Quality Settings, then auto exposure ROI modes will be enabled
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
        }else{
            autoexpManual.enabled = false
            autoexpFull.enabled = false
            autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 0.1
            autoexpFull.opacity = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    // get current effect mode
    function currentEffectMode(mode)
    {
        switch(mode)
        {
            case See3Cam30.EFFECT_NORMAL:
                effectNormal.checked = true
                break;
            case See3Cam30.EFFECT_BLACK_WHITE:
                effectBW.checked = true
                break;
            case See3Cam30.EFFECT_GREYSCALE:
                effectGrayscale.checked = true
                break;
            case See3Cam30.EFFECT_NEGATIVE:
                effectNegative.checked = true
                break;
            case See3Cam30.EFFECT_SKETCH:
                effectSketch.checked = true
                break;
        }
    }

    // get current auto focus mode
    function currentAfMode(mode)
    {        
        switch(mode)
        {
            case See3Cam30.Continuous:
                radioContin.checked = true
                break;
            case See3Cam30.OneShot:
                radioOneshot.checked = true
                break;
            case See3Cam30.AfModeDisabled:
                radioContin.enabled = false
                radioOneshot.enabled = false
                trigger.enabled = false
                break;
        }
    }


    Connections{
         target: root
         onMouseRightClicked:{
             if(afCentered.enabled && afCentered.checked){
                see3cam30.setROIAutoFoucs(See3Cam30.AFCentered, width, height, x, y, afWindowSizeCombo.currentText)
             }else if(afManual.enabled && afManual.checked){
                 see3cam30.setROIAutoFoucs(See3Cam30.AFManual, width, height, x, y, afWindowSizeCombo.currentText)
             }
             if(autoexpFull.enabled && autoexpFull.checked){
                see3cam30.setROIAutoExposure(See3Cam30.AutoExpFull, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }else if(autoexpManual.enabled && autoexpManual.checked){
                see3cam30.setROIAutoExposure(See3Cam30.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }
         }
         onAutoFocusSelected:{
             enableDisableAutoFocusUIControls(autoFocusSelect)
         }
         onAutoExposureSelected:{
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         onAfterBurst:{
             console.log("after burst ........")
             if(rectEnable.checked){
                 console.log("after burst - rectEnable selected")
                see3cam30.enableDisableAFRectangle(true)
             }
         }
         onBeforeRecordVideo:{
             see3cam30.enableDisableAFRectangle(false)
         }
         onAfterRecordVideo:{
             console.log("after record video ........")
             if(rectEnable.checked){
                 console.log("after record video - rectEnable.checked ........")
                see3cam30.enableDisableAFRectangle(true)
             }
         }
         onTakeScreenShot:
         {
             see3cam30.enableDisableAFRectangle(false)
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
}
