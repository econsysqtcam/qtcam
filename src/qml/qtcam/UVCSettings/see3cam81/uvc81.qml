/*
 * uvc81.qml -- extension settings for cu80 cameras
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

// Added By Sankari : 17 Dec 2016
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam81 1.0
import cameraenum 1.0
import "../../JavaScriptFiles/tempValue.js" as JS

Item {
    width:268
    height:720
    id: see3cam81
    property bool outputPinFlag
    property var fpsArray // To get supported fps list
    property bool twleveFpsExist: false // Check whether 12 FPS exist
    property bool focusModeindexChange: false
    property int gainMinValue: 0
    property int gainMaxValue: 895
    property bool skipUpdateGainValues: false
    property bool skipUpdateUIOnAFWindowSize: false
    property bool skipUpdateFocusPosition: false

    Timer {
        id: getWbRGBValuesTimer
        interval: 500
        onTriggered: {
            seecam81.getWbValues(See3Cam81.GetRed)
            seecam81.getWbValues(See3Cam81.GetBlue)
            seecam81.getWbValues(See3Cam81.GetGreen)
            stop()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            getFirmwareVer()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }    

    Action {
        id: getGPIStatus
        onTriggered: {
            gpiStatus()
        }
    }

    Action{
        id:focusStatus
        onTriggered:{
            seecam81.getFocusStatus()
        }
    }

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            // If resolution is 1080p and 12 FPS is supported by camera, then change fps value to 12 and take image.
            // For that, use CommonEnums.CHANGE_FPS_SHOT.
            // otherwise, use CommonEnums.SNAP_SHOT.
            if(root.stillSettingsRootObject.stillOutputTextValue == "1920x1080"){
                fpsArray = root.availableFpslist.split(',')
                for (var index = 0; index < fpsArray.length; index++) {
                    if(fpsArray[index].indexOf("12 FPS") != -1){
                        twleveFpsExist = true
                        root.fpsIndexToChange = index
                    }
                }
                if(twleveFpsExist){
                    root.imageCapture(CommonEnums.CHANGE_FPS_SHOT);
                }else{
                    root.imageCapture(CommonEnums.SNAP_SHOT);
                }
            }else{
                root.imageCapture(CommonEnums.SNAP_SHOT);
            }
            twleveFpsExist = false
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

    ScrollView{
        id: scrollview
        x: 10
        y: 180.5
        width: 257
        height: 400
        style: ScrollViewStyle {
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
        Item {
            height: 1300
        ColumnLayout{
            y:5
            spacing:20
            Text {
                id: specialEffects
                text: "--- Special Effects ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Row{
               spacing: 55
               ExclusiveGroup { id: specialEffectsGrp }
                RadioButton {
                    exclusiveGroup: specialEffectsGrp
                    checked: false
                    id: radioNormal
                    text: "Normal"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam81.setEffectMode(See3Cam81.EFFECT_NORMAL)
                    }
                    Keys.onReturnPressed: {
                        seecam81.setEffectMode(See3Cam81.EFFECT_NORMAL)
                    }
                }
                RadioButton {
                    exclusiveGroup: specialEffectsGrp
                    checked: false
                    id: radioGreyscale
                    text: "Greyscale"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam81.setEffectMode(See3Cam81.EFFECT_GRAYSCALE)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        seecam81.setEffectMode(See3Cam81.EFFECT_GRAYSCALE)
                        defaultValue.enabled = true
                    }
                }
            }
            Text {
                id: flipText
                text: "--- Flip Controls ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Row{
                spacing: 30
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical \nFlip"
                    style: econCheckBoxStyle
                    onClicked:{
                        seecam81.setFlipMirrorMode(flipCtrlVertical.checked, mirrorCtrlHorizontal.checked)
                        if(flipCtrlVertical.checked)
                            defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        seecam81.setFlipMirrorMode(flipCtrlVertical.checked, mirrorCtrlHorizontal.checked)
                        if(flipCtrlVertical.checked)
                            defaultValue.enabled = true
                    }
                }
                CheckBox {
                    id: mirrorCtrlHorizontal
                    activeFocusOnPress : true
                    text: "Horizontal \nMirror"
                    style: econCheckBoxStyle
                    onClicked:{
                        seecam81.setFlipMirrorMode(flipCtrlVertical.checked, mirrorCtrlHorizontal.checked)
                        if(mirrorCtrlHorizontal.checked)
                            defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        seecam81.setFlipMirrorMode(flipCtrlVertical.checked, mirrorCtrlHorizontal.checked)
                        if(mirrorCtrlHorizontal.checked)
                            defaultValue.enabled = true
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

            ExclusiveGroup { id: strobesGrp }
            Row{
                RadioButton {
                    exclusiveGroup: strobesGrp
                    checked: false
                    id: strobeFlashVidoStreaming
                    text: "Flash for Video Streaming"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam81.setStrobeMode(See3Cam81.STROBE_FLASH_VS_81)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        seecam81.setStrobeMode(See3Cam81.STROBE_FLASH_VS_81)
                        defaultValue.enabled = true
                    }
                }
            }
            Row{
                RadioButton {
                    exclusiveGroup: strobesGrp
                    checked: false
                    id: strobeTorch
                    text: "Torch"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam81.setStrobeMode(See3Cam81.STROBE_TORCH_81)
                        defaultValue.enabled = true
                    }
                    Keys.onReturnPressed: {
                        seecam81.setStrobeMode(See3Cam81.STROBE_TORCH_81)
                        defaultValue.enabled = true
                    }
                }
            }
            Row{
                RadioButton {
                    exclusiveGroup: strobesGrp
                    checked: false
                    id: strobeOff
                    text: "OFF"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam81.setStrobeMode(See3Cam81.STROBE_OFF_81)
                    }
                    Keys.onReturnPressed: {
                        seecam81.setStrobeMode(See3Cam81.STROBE_OFF_81)
                    }
                }
            }

            Text {
                id: focusModeText
                text: "---Focus Modes ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            Row{
                ComboBox {

                    model: ListModel {
                        id: cbItemsModel
                        ListElement { text: "Auto Focus"  }
                        ListElement { text: "Manual Focus"  }
                        ListElement { text: "Single Trigger Focus"  }
                    }
                    id: focusModeBox
                    opacity: 1
                    activeFocusOnPress : true
                    style: ComboBoxStyle {
                        background: Image {
                            id: focusModeImg
                            source: "images/device_box.png"
                            Rectangle {
                                width: focusModeImg.sourceSize.width - 28
                                height: focusModeImg.sourceSize.height
                                color: "#222021"
                                border.color: "white"
                                border.width: control.activeFocus ? 3 : 1
                                radius: control.activeFocus ? 5 : 0
                            }
                        }
                        label: Text {
                            color: "#ffffff"
                            text: control.currentText
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    onCurrentIndexChanged: {                        
                        if(currentIndex == See3Cam81.AUTO_MODE_INDEX) {
                            doAutoModeWhenFocusModeIndexChanged()
                        } else if(currentIndex == See3Cam81.MANUAL_MODE_INDEX) {
                            doManualModeWhenFocusModeIndexChanged()
                        } else if(currentIndex == See3Cam81.SINGLE_TRIGGER_MODE_INDEX) {
                            doSingleTriggerModeWhenFocusModeIndexChanged()
                        }
                    }
                    Component.onCompleted: {
                        focusModeindexChange = true
                    }
                }
            }

            Row{
                spacing: 20
                Text {
                    id: singleShot
                    text: "Single Shot:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (focusModeBox.currentIndex == See3Cam81.SINGLE_TRIGGER_MODE_INDEX) ? 1 : 0.1
                }
                Button {
                    id: trigger
                    activeFocusOnPress : true
                    text: "Trigger"
                    style: econButtonStyle
                    enabled: (focusModeBox.currentIndex == See3Cam81.SINGLE_TRIGGER_MODE_INDEX) ? true : false
                    opacity: enabled ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 138                    
                    onClicked: {
                        trigger.enabled = false                        
                        seecam81.setFocusMode(See3Cam81.SINGLETRIGGER_FOCUS_81)
                   }
                    Keys.onReturnPressed: {
                        trigger.enabled = false                        
                        seecam81.setFocusMode(See3Cam81.SINGLETRIGGER_FOCUS_81)
                    }
                }
            }

            Row{
                spacing:25
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: focusSlider
                    enabled: (focusModeBox.currentIndex == See3Cam81.MANUAL_MODE_INDEX) ? true : false
                    opacity: enabled ? 1 : 0.1
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: 0
                    maximumValue: 1022
                    onValueChanged:  {
                        if(focusModeBox.currentIndex == See3Cam81.MANUAL_MODE_INDEX && skipUpdateFocusPosition){
                            seecam81.setFocusPosition(value);
                            seecam81.setFocusMode(See3Cam81.MANUAL_FOCUS_81)
                        }
                    }
                    Component.onCompleted: {
                        skipUpdateFocusPosition = true
                    }
                }
                TextField {
                    id: focusPosvalue
                    text: focusSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: false
                    opacity: 1
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: focusSlider.minimumValue; top: focusSlider.maximumValue}
                }
            }
            Row{
                spacing: 25
                Button {
                    id: focusStatusButton
                    activeFocusOnPress : true
                    text: "Focus Status"
                    style: econButtonStyle
                    enabled: true
                    opacity: 1
                    implicitHeight: 25
                    implicitWidth: 100
                    action: focusStatus
                    Keys.onReturnPressed: {
                        seecam81.getFocusStatus()
                    }
                }
                Button {
                    id: focusPos
                    activeFocusOnPress : true
                    text: "Focus Position"
                    style: econButtonStyle
                    enabled: true
                    opacity: 1
                    implicitHeight: 25
                    implicitWidth: 100
                    onClicked: {
                        focusPos.enabled = false
                        seecam81.getFocusPosition()
                        focusPos.enabled = true
                    }
                    Keys.onReturnPressed: {
                        focusPos.enabled = false
                        seecam81.getFocusPosition()
                        focusPos.enabled = true
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
                          centeredROIFocusMode()
                      }
                      Keys.onReturnPressed: {
                          centeredROIFocusMode()
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
                            manualROIFocusMode()
                      }
                      Keys.onReturnPressed: {
                            manualROIFocusMode()
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
                }
                activeFocusOnPress: true
                style: econComboBoxStyle
                onCurrentIndexChanged: {
                    if(skipUpdateUIOnAFWindowSize){
                        seecam81.setROIAutoFoucs(See3Cam81.AFManual, afWindowSizeCombo.currentText)
                        seecam81.setROIAutoFoucsCoordinates(0, 0, 0, 0);
                    }
                    skipUpdateUIOnAFWindowSize = true
                }
            }



            Text {
                id: gpioInputControlsText
                text: "---GPIO controls ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Row{
                Text {
                    id: gpiControls
                    text: "General purpose Input:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }
            }

            Row{
                spacing: 35
                Text {
                    id: gpi_number
                    text: "GPI Number :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }

                ComboBox {
                    id: gpi_no_box81
                    model: ListModel {
                        id: cbItems81
                        ListElement { text: "3"  }
                        ListElement { text: "6"  }
                    }
                    opacity: 1
                    activeFocusOnPress : true
                    style: ComboBoxStyle {
                        background: Image {
                            id: gpo_no_box_style81
                            source: "../../Views/images/plinefreq_box.png"
                            Rectangle {
                                width: gpo_no_box_style81.sourceSize.width - 20
                                height: gpo_no_box_style81.sourceSize.height + 1
                                color: "#222021"
                                border.color: "white"
                                border.width: control.activeFocus ? 3 : 1
                                radius: control.activeFocus ? 5 : 0
                            }
                        }
                        label: Text {
                            color: "#ffffff"
                            text: control.currentText
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    onCurrentIndexChanged: {
                        gpi_level_high.text = " "
                    }
                }
            }
            Row{
                spacing : 10
                Button {
                    id: get_gpi_selected
                    opacity: 1
                    action: getGPIStatus
                    activeFocusOnPress : true
                    tooltip: "Click to get the status of Input PIN"
                    style: ButtonStyle {
                        background: Rectangle {
                            border.width: control.activeFocus ? 3 :0
                            color: "#222021"
                            border.color: control.activeFocus ? "#ffffff" : "#222021"
                            radius: 5
                        }
                        label: Image {
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            fillMode: Image.PreserveAspectFit
                            source: "images/get_gpi_selected.png"
                        }
                    }
                    Keys.onReturnPressed: {
                        gpiStatus()
                    }
                }
                Text {
                    id: gpi_level_high
                    y:(get_gpi_selected.height - gpi_level_high.height)/2
                    text: " "
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }
            }
            Row{
                Text {
                    id: gpoControls
                    text: "General purpose Output:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }
            }
            Row{
                spacing: 25
                Text {
                    id: gpo_number
                    text: "GPO Number :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }

                ComboBox {
                    id: gpioOutputBox
                    model: ListModel {
                        id: cbItems
                        ListElement { text: "8"  }
                        ListElement { text: "9"  }
                    }
                    opacity: 1
                    activeFocusOnPress : true
                    style: ComboBoxStyle {
                        background: Image {
                            id: gpobox
                            source: "../../Views/images/plinefreq_box.png"
                            Rectangle {
                                width: gpobox.sourceSize.width - 20
                                height: gpobox.sourceSize.height + 1
                                color: "#222021"
                                border.color: "white"
                                border.width: control.activeFocus ? 3 : 1
                                radius: control.activeFocus ? 5 : 0
                            }
                        }
                        label: Text {
                            color: "#ffffff"
                            text: control.currentText
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    onCurrentIndexChanged: {
                        outputPinFlag = true
                        if(currentIndex == See3Cam81.GPIO_OUT8_INDEX){
                            seecam81.getOutputGpioLevel(See3Cam81.CAM81_OUT8)
                        }
                        else{
                            seecam81.getOutputGpioLevel(See3Cam81.CAM81_OUT9)
                        }
                    }
                }
            }
            Row {
                spacing: 60
                ExclusiveGroup { id: group }
                RadioButton {
                    exclusiveGroup: group
                    id: radioHigh81
                    style: RadioButtonStyle {
                        label: Text {
                            text: "High"
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
                    onClicked: {
                        makeGPOValuesHigh()
                    }
                    Keys.onReturnPressed: {                        
                        makeGPOValuesHigh()
                    }
                }
                RadioButton {
                    exclusiveGroup: group
                    id: radioLow81
                    style: RadioButtonStyle {
                        label: Text {
                            text: "Low"
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
                    onClicked: {
                        makeGPOValuesLow()
                    }
                    Keys.onReturnPressed: {                        
                        makeGPOValuesLow()
                    }
                }
            }

            Text {
                id: rgbGainText
                text: "---RGB Gain ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            GridLayout {
                id: wbSliders
                columns: 3
                rowSpacing: 15
                columnSpacing: 15
                Text {
                    id: redText
                    text: "Red"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1

                }
                Slider {
                    property bool wbRed: false
                    property int wbRedValue: 0
                    id: redSlider
                    updateValueWhileDragging: false
                    activeFocusOnPress : true
                    width: 120
                    minimumValue: gainMinValue
                    maximumValue: gainMaxValue
                    stepSize: 1                    
                    style: econSliderStyle
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                    enabled: JS.autoWhiteBalSelected ? false : true
                    onValueChanged: {                        
                        if(skipUpdateGainValues){
                            seecam81.setWbValues(See3Cam81.Red, redSlider.value);
                        }
                        defaultValue.enabled = true
                        skipUpdateGainValues = true
                    }
                    Component.onCompleted:
                        skipUpdateGainValues = true
                }
                TextField {
                    id: redValue
                    text: redSlider.value
                    horizontalAlignment: TextInput.AlignHCenter
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    style: econTextFieldStyle
                    enabled: false
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                    smooth: true                    
                }
                Text {
                    id: greenText
                    text: "Green"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                }
                Slider {
                    property bool wbGreen: false
                    property int wbGreenValue: 0
                    id: greenSlider
                    width: 120
                    minimumValue: gainMinValue
                    maximumValue: gainMaxValue
                    updateValueWhileDragging: false
                    activeFocusOnPress : true
                    stepSize: 1                   
                    style: econSliderStyle
                    enabled: JS.autoWhiteBalSelected ? false : true
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                    onValueChanged: {
                       if(skipUpdateGainValues){                        
                        seecam81.setWbValues(See3Cam81.Green,greenSlider.value);
                       }
                       defaultValue.enabled = true
                       skipUpdateGainValues = true
                    }
                    Component.onCompleted: {
                        skipUpdateGainValues = true
                    }
                }

                TextField {
                    id: greenValue
                    text: greenSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: false
                    smooth: true                    
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                }

                Text {
                    id: blueText
                    text: "Blue"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true                    
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                }

                Slider {
                    property bool wbBlue: false
                    property int wbBlueValue: 0
                    id: blueSlider
                    width: 120
                    updateValueWhileDragging: false
                    activeFocusOnPress : true
                    minimumValue: gainMinValue
                    maximumValue: gainMaxValue
                    stepSize: 1                   
                    style: econSliderStyle
                    enabled: JS.autoWhiteBalSelected ? false : true
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                    onValueChanged: {
                        if(skipUpdateGainValues){
                            seecam81.setWbValues(See3Cam81.Blue, blueSlider.value);
                        }
                        defaultValue.enabled = true
                        skipUpdateGainValues = true
                    }
                    Component.onCompleted: {
                        skipUpdateGainValues = true
                    }
                }

                TextField {
                    id: blueValue
                    text: blueSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: false
                    smooth: true
                    opacity: JS.autoWhiteBalSelected ? 0.1 : 1
                }
            }
            Row{
                Text {
                    id: wbHelpText
                    text: "Note: To enable this control, please enable Manual White Balance in Camera Settings"
                    horizontalAlignment: Qt.AlignLeft
                    width : 250
                    wrapMode: Text.WordWrap
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }
            }

            Row{
                Layout.alignment: Qt.AlignCenter
                Button {
                    id: defaultValue
                    activeFocusOnPress : true
                    text: "Default"
                    style: econButtonStyle
                    enabled: false
                    opacity: 1
                    implicitHeight: 35
                    implicitWidth: 100                    
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
                    id: f_wversion_selected_81
                    opacity: 1
                    action: firmwareVersion
                    activeFocusOnPress : true
                    tooltip: "Click to get the firmware version of the camera"
                    style: ButtonStyle {
                        background: Rectangle {
                            border.width: control.activeFocus ? 3 :0
                            color: "#222021"
                            border.color: control.activeFocus ? "#ffffff" : "#222021"
                            radius: 5
                        }
                        label: Image {
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            fillMode: Image.PreserveAspectFit
                            source: "images/f_wversion_selected.png"
                        }
                    }
                    Keys.onReturnPressed: {
                        getFirmwareVer()
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
        id: econCheckBoxStyle
        CheckBoxStyle {
            label: Text {
                text: control.text
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                opacity: 1
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

    See3Cam81 {
        id: seecam81
        onEffectModeValue: {
            currentEffectMode(effectMode)
        }
        onFlipMirrorModeValue:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onStrobeModeValue:{
            currentStrobeMode(strobeMode)
        }
        onFocusModeStatus: {
            switch(focusModeStatus) {
            case See3Cam81.CAM_FOCUS_FAILED:
                messageDialog.title = qsTr("Failed")
                messageDialog.text = qsTr("Failed to set the selected focus Mode")
                messageDialog.open()
                break;
            case See3Cam81.CAM_FOCUS_SUCCESS:
                messageDialog.title = qsTr("Success")
                messageDialog.text = qsTr("The selected focus Mode is set successfully")
                messageDialog.open()
                //success mode
                break;
            case See3Cam81.CAM_FOCUS_BUSY:
                messageDialog.title = qsTr("Camera busy")
                messageDialog.text = qsTr("Camera is in focus busy mode")
                messageDialog.open()
                //camera busy mode
                break;
            default:
                messageDialog.title = qsTr("Unknown mode")
                messageDialog.text = qsTr("Receive unknown mode")
                messageDialog.open()
                break;
            }
        }
        onFocusModeValue: {
            switch(focusModeValue) {
            case See3Cam81.CONTINUOUS_FOCUS_81:
                focusModeBox.currentIndex = 0
                break
            case See3Cam81.MANUAL_FOCUS_81:
                focusModeBox.currentIndex = 1
                break
            case See3Cam81.SINGLETRIGGER_FOCUS_81:
                focusModeBox.currentIndex = 2
                break
            default:
                focusModeBox.currentIndex = -1
            }
        }
        onFocusPostionValue: {
            focusSlider.value = focusPositionValue
        }
        onGpioLevel:{            
            if(level === See3Cam81.GPIO_LOW_81) {
                if(outputPinFlag){
                    radioLow81.checked = true
                }
                else{
                    gpi_level_high.text = "<b>Low</b>"                    
                }

            } else {
                if(outputPinFlag){
                    radioHigh81.checked = true
                }
                else{
                    gpi_level_high.text = "<b>High</b>"                    
                }
            }
        }
        onUpdateFrameToSkipfromCam81:{
            root.updateFrametoSkip(stillSkip)
        }
        onReturnFromSetFocusMode:{
         // After returning from set focus mode function, enable trigger button in single trigger mode
             if(focusModeBox.currentIndex == See3Cam81.SINGLE_TRIGGER_MODE_INDEX){
                 trigger.enabled = true
                 trigger.opacity = 1                 
             }             
             seecam81.getAutoFocusROIModeAndWindowSize()
        }
        onWhiteBalanceGainValue:{
            skipUpdateGainValues = false
            currentRGBGainValues(rgbColor, whiteBalanceGain)
        }
        onRoiAfMode:{
            if(roiMode == See3Cam81.AFCentered){
                afCentered.checked = true
            }else if(roiMode == See3Cam81.AFManual){
                skipUpdateUIOnAFWindowSize = false
                afManual.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
                skipUpdateFocusPosition = false
            }
        }
        onRoiAfModeSuccess:{
            afCentered.enabled = true
            afCentered.opacity = 1
            afManual.enabled = true
            afManual.opacity = 1
            if(afManual.checked){
                afWindowSizeCombo.enabled = true
                afWindowSizeCombo.opacity = 1
            }else{
                afWindowSizeCombo.enabled = false
                afWindowSizeCombo.opacity = 0.1
            }

        }
        onRoiAfModeFail:{
            afCentered.enabled = false
            afCentered.opacity = 0.1
            afManual.enabled = false
            afManual.opacity = 0.1
            afWindowSizeCombo.enabled = false
            afWindowSizeCombo.opacity = 0.1
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


    function centeredROIFocusMode(){
        seecam81.setROIAutoFoucs(See3Cam81.AFCentered, 0);
        afWindowSizeCombo.enabled = false
        afWindowSizeCombo.opacity = 0.1
    }
    function manualROIFocusMode(){
        // setROIAutoFoucs() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
        // setROIAutoFoucsCoordinates() args: videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord
        // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
        // winSize is required only for manual mode
        seecam81.setROIAutoFoucs(See3Cam81.AFManual, afWindowSizeCombo.currentText)
        seecam81.setROIAutoFoucsCoordinates(0, 0, 0, 0);
        afWindowSizeCombo.enabled = true
        afWindowSizeCombo.opacity = 1
        defaultValue.enabled = true
    }

    function getCameraValues(){
        seecam81.getEffectMode()
        seecam81.getFlipMirrorMode()
        seecam81.getStrobeMode()
        seecam81.getFocusMode()
        seecam81.getFocusPosition()
        seecam81.getWbValues(See3Cam81.GetRed)
        seecam81.getWbValues(See3Cam81.GetBlue)
        seecam81.getWbValues(See3Cam81.GetGreen)
        seecam81.getAutoFocusROIModeAndWindowSize()
    }

    function gpiStatus() {
        outputPinFlag = false
        if(gpi_no_box81.currentIndex == See3Cam81.GPIO_IN3_INDEX){
            seecam81.getInputGpioLevel(See3Cam81.CAM81_IN3) //PIN Number
        } else {
            seecam81.getInputGpioLevel(See3Cam81.CAM81_IN6)  //PIN Number
        }
    }

    function makeGPOValuesHigh(){
        outputPinFlag = true
        if(gpioOutputBox.currentIndex == See3Cam81.GPIO_OUT8_INDEX)
            seecam81.setGpioLevel(See3Cam81.CAM81_OUT8,See3Cam81.High)//Pin Number & Value
        else
            seecam81.setGpioLevel(See3Cam81.CAM81_OUT9,See3Cam81.High)//Pin Number & Value        
    }

    function makeGPOValuesLow(){
        outputPinFlag = true
        if(gpioOutputBox.currentIndex == See3Cam81.GPIO_OUT8_INDEX)
            seecam81.setGpioLevel(See3Cam81.CAM81_OUT8,See3Cam81.Low);//Pin Number & Value
        else
            seecam81.setGpioLevel(See3Cam81.CAM81_OUT9,See3Cam81.Low);//Pin Number & Value        
    }

    function setDefaultValues(){
        defaultValue.enabled = false
        focusModeindexChange = false
        seecam81.setToDefault()
        seecam81.getEffectMode()
        seecam81.getFlipMirrorMode()
        seecam81.getStrobeMode()
        seecam81.getFocusMode()
        seecam81.getFocusPosition()
        seecam81.getWbValues(See3Cam81.GetRed)
        seecam81.getWbValues(See3Cam81.GetBlue)
        seecam81.getWbValues(See3Cam81.GetGreen)
        seecam81.getAutoFocusROIModeAndWindowSize()
        focusModeindexChange = true
    }

    function currentRGBGainValues(mode, value){
        switch(mode){
        case See3Cam81.GetRed:
            skipUpdateGainValues = false
            redSlider.value = value            
            break
        case See3Cam81.GetBlue:
            skipUpdateGainValues = false
            blueSlider.value = value
            break
        case See3Cam81.GetGreen:
            skipUpdateGainValues = false
            greenSlider.value = value            
            break
        }
    }

    function currentEffectMode(mode)
    {
        switch(mode)
        {
            case See3Cam81.EFFECT_NORMAL:                
                radioNormal.checked = true
                break;
            case See3Cam81.EFFECT_GRAYSCALE:                
                radioGreyscale.checked = true
                break;
        }
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case See3Cam81.FLIP_ON_MIRROR_ON_81:
                 flipCtrlVertical.checked = true
                 mirrorCtrlHorizontal.checked = true                
                break;
            case See3Cam81.FLIP_OFF_MIRROR_ON_81:
                flipCtrlVertical.checked = false
                mirrorCtrlHorizontal.checked = true                
                break;
            case See3Cam81.FLIP_ON_MIRROR_OFF_81:
                 flipCtrlVertical.checked = true
                 mirrorCtrlHorizontal.checked = false                
                break;
            case See3Cam81.FLIP_OFF_MIRROR_OFF_81:
                flipCtrlVertical.checked = false
                mirrorCtrlHorizontal.checked = false                
                break;
        }
    }

    function currentStrobeMode(mode){
        switch(mode){
        case See3Cam81.STROBE_OFF_81:
            strobeOff.checked = true
            break;
        case See3Cam81.STROBE_FLASH_VS_81:
            strobeFlashVidoStreaming.checked = true
            break;
        case See3Cam81.STROBE_TORCH_81:
            strobeTorch.checked = true
            break;
        }
    }

    function getFirmwareVer() {
        uvccamera.getFirmWareVersion()
    }


    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function doAutoModeWhenFocusModeIndexChanged(){
         trigger.enabled = false
         trigger.opacity = 0.1
         if(focusModeindexChange){            
             seecam81.setFocusMode(See3Cam81.CONTINUOUS_FOCUS_81)
         }
     }

     function doManualModeWhenFocusModeIndexChanged(){
         trigger.enabled = false
         trigger.opacity = 0.1
         defaultValue.enabled = true
         if(focusModeindexChange){            
             seecam81.setFocusMode(See3Cam81.MANUAL_FOCUS_81)
         }
     }

    function doSingleTriggerModeWhenFocusModeIndexChanged(){         
         defaultValue.enabled = true
         trigger.enabled = true
         trigger.opacity = 1
         if(focusModeindexChange){
             seecam81.setFocusMode(See3Cam81.SINGLETRIGGER_FOCUS_81)
         }                  
    }
    Component.onCompleted:{        
        getCameraValues()
        // enable default button if any one of the following controls is selected
        if(flipCtrlVertical.checked || mirrorCtrlHorizontal.checked || radioGreyscale.checked || strobeTorch.checked
                || strobeFlashVidoStreaming.checked){
            defaultValue.enabled = true
        }
    }

    Connections{
        target: root
        onMouseRightClicked:{
            if(afCentered.enabled && afCentered.checked){
                seecam81.setROIAutoFoucs(See3Cam81.AFAuto, afWindowSizeCombo.currentText)                
            }else if(afManual.enabled && afManual.checked){
                seecam81.setROIAutoFoucs(See3Cam81.AFManual, afWindowSizeCombo.currentText)
                seecam81.setROIAutoFoucsCoordinates(width, height, x, y);
            }
        }
        onFrameSkipCount:{
            seecam81.setStillSkipCount(stillResolution, videoResolution) // for each resolution skip count will be varied
        }
        onFrameSkipCountWhenFPSChange:{
            seecam81.setStillSkipCountWhenFPSChange(fpsChange)
        }

        onStillResolutionChanged:{
            if(stillResolution == "1920x1080"){
                // Get fps list for 1080p resolution
                root.getFPSList(stillResolution, stillFormat)
            }
        }

        // based on Auto White balance is selected in Image Quality settings, Gain values will be varied in Extension settings
        onAutoWhiteBalanceSelected:{
            if(autoWhiteBalanceSelect){
                redSlider.enabled = false
                blueSlider.enabled = false
                greenSlider.enabled = false
                redSlider.opacity = 0.1
                blueSlider.opacity = 0.1
                greenSlider.opacity = 0.1
                redText.opacity = 0.1
                blueText.opacity = 0.1
                greenText.opacity = 0.1
                redValue.opacity = 0.1
                blueValue.opacity = 0.1
                greenValue.opacity = 0.1                
            }else{                
                redSlider.enabled = true
                blueSlider.enabled = true
                greenSlider.enabled = true
                redSlider.opacity = 1
                blueSlider.opacity = 1
                greenSlider.opacity = 1
                redText.opacity = 1
                blueText.opacity = 1
                greenText.opacity = 1
                redValue.opacity = 1
                blueValue.opacity = 1
                greenValue.opacity = 1
                //Added by Sankari: It is taking some time to update the manual whitebalance values in firmware. So Delay is needed
                getWbRGBValuesTimer.start()
            }
        }
        // based on Manual White balance is selected in Image Quality settings, Gain values will be varied in Extension settings
        onManualWbSliderValueChanged:{
            seecam81.getWbValues(See3Cam81.GetRed)
            seecam81.getWbValues(See3Cam81.GetBlue)
            seecam81.getWbValues(See3Cam81.GetGreen)
        }
    }
}


