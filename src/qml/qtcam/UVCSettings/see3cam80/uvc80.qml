/*
 * uvc80.qml -- extension settings for cu80 cameras
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
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam80 1.0
import econ.camera.see3camControl 1.0
import econ.camera.see3camGpioControl 1.0
import cameraenum 1.0

Item {
    width:268
    height:720
    id: see3cam80
    property bool outputPinFlag

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

    Action {
        id: getGPIStatus
        onTriggered: {
            gpiStatus()
        }
    }

    Action {
        id: singleTrigger
        onTriggered: {
            singleTriggerFocus()
        }
    }

    Action {
        id: positionValue
        onTriggered: {
            getPositionValue()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            getFirmwareVer()
        }
    }

    Text {
        id: focus_controls
        text: "--- Focus Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 71
        y: 187.5
        opacity: 0.50196078431373
    }

    Text {
        id: focus_mode
        text: "Focus Mode :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 19
        y: 212.5
        opacity: 1
    }
    ComboBox {
        property bool indexChange: false
        model: ListModel {
            id: cbItemsModel
            ListElement { text: "Auto Focus"  }
            ListElement { text: "Manual Focus"  }
            ListElement { text: "Single Trigger Focus"  }
        }
        id: focusModeBox
        x: 8
        y: 240
        opacity: 1
        activeFocusOnPress : true
        style: ComboBoxStyle {
            background: Image {
                id: focusModeImg
                source: "images/single_trigger_box.png"
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
            if(currentIndex == 0) {
                focus_selected.opacity = 0.1
                focus_selected.enabled = false
                focus_position.opacity = 0.1
                focus_position.enabled = false
                focus_position_base.opacity = 0.1
                focus_position_base.enabled = false
                focus_position_value.opacity = 0.1
                focus_position_value.enabled = false
                if(indexChange)
                  seecam80.setFocusMode(See3Cam80.Auto)

            } else if(currentIndex == 1) {
                focus_selected.opacity = 0.1
                focus_selected.enabled = false
                focus_position.opacity = 1
                focus_position.enabled = true
                focus_position_base.opacity =1
                focus_position_base.enabled = true
                focus_position_value.opacity = 1
                focus_position_value.enabled = true
                if(indexChange)
                 seecam80.setFocusMode(See3Cam80.Manual)
            } else if(currentIndex == 2) {
                focus_selected.opacity = 1
                focus_selected.enabled = true
                focus_position.opacity = 0.1
                focus_position.enabled = false
                focus_position_base.opacity = 0.1
                focus_position_base.enabled = false
                focus_position_value.opacity = 0.1
                focus_position_value.enabled = false
                if(indexChange)
                seecam80.setFocusMode(See3Cam80.Singletrigger)
            }
        }
        Component.onCompleted: {
            indexChange = true
        }
    }

    Image {
        id: single_trigger_dropdown
        source: "images/single_trigger_dropdown.png"
        x: 151
        y: 242
        opacity: 1
    }

    Button {
        id: focus_selected
        x: 184
        y: 238
        opacity: 1
        enabled: false
        action: singleTrigger
        activeFocusOnPress : true
        tooltip: "Click to get object Focus"
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
                source: "images/focus_selected.png"
            }
        }
        Keys.onReturnPressed: {
            singleTriggerFocus()
        }
    }

    Text {
        id: focus_position
        text: "Position :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 8
        y: 286.5
        opacity: 1
    }
    Slider {
        x: 82
        y: 286
        updateValueWhileDragging: false
        id: focus_position_base
        minimumValue: 0
        maximumValue: 1022
        stepSize: 1
        opacity: 1
        width: 135
        activeFocusOnPress : true
        style: SliderStyle {
            groove:Row {
                spacing: 0
                Rectangle {
                    width: styleData.handlePosition
                    height: 4
                    color: "#dc6239"
                    radius: 5
                }
                Rectangle {
                    width: control.width - styleData.handlePosition
                    height: 4
                    color: "#dddddd"
                    radius: 5
                }
            }
            handle: Image {
                id: focus_position_dragger
                source: "images/focus_position_dragger.png"
                y: -4
                opacity: 1
            }
        }
        onValueChanged: {
            seecam80.setFocusPosition(value);
            seecam80.setFocusMode(See3Cam80.Manual)
        }
    }

    TextField {
        id: focus_position_value
        validator: IntValidator {bottom: 0; top: 1022;}
        x: 227
        y: 286.5
        font.pixelSize: 9
        text:  focus_position_base.value
        style: TextFieldStyle {
            textColor: "black"
            background: Image {
                y: -2
                id: focus_position_box
                source: "images/focus_position_box.png"
            }
        }
        onTextChanged: {
            focus_position_base.value = focus_position_value.text
        }
    }

    Text {
        id: current_position
        text: "Current Position :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 9
        y: 322.5
        opacity: 1
    }

    Image {
        id: current_position_box
        source: "images/current_position_box.png"
        x: 135
        y: 320
        opacity: 1
    }

    Text {
        id: current_position_value
        text:  focus_position_base.value
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#211f21"
        smooth: true
        x: 143
        y: 322.5
        opacity: 1
    }
    Button {
        id: get_position_selected
        x: 102
        y: 358
        opacity: 1
        action: positionValue
        activeFocusOnPress : true
        tooltip: "Click to get the current position of the camera focus value"
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
                source: "images/get_position_selected.png"
            }
        }
        Keys.onReturnPressed: {
            getPositionValue()
        }
    }
    Text {
        id: gpio_controls
        text: "--- GPIO Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 396.5
        opacity: 0.50196078431373
    }
    Text {
        id: gpo_level
        text: "GPO Level :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 8
        y: 431.5
        opacity: 1
    }
    Row {
        x: 100
        y: 433
        spacing: 25
        ExclusiveGroup { id: groupcu80 }
        RadioButton {
            exclusiveGroup: groupcu80
            id: radioHighcu80
            text: "High"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked:{
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.High)//Pin Number & Value
            }
            Keys.onReturnPressed: {
                checked = true
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.High)//Pin Number & Value
            }
        }
        RadioButton {
            exclusiveGroup: groupcu80
            checked: false
            id: radioLowcu80
            text: "Low"
            activeFocusOnPress: true
            style: econRadioButtonStyle
            onClicked: {
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.Low)//Pin Number & Value
            }
            Keys.onReturnPressed: {
                checked = true
                outputPinFlag = true
                see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.Low)//Pin Number & Value
            }
        }
    }

    Text {
        id: gpi_number
        text: "GPI Number :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 7
        y: 470.5
        opacity: 1
    }

    ComboBox {
        id: gpi_no_boxcu80
        model: ListModel {
            id: cbItemscu80
            ListElement { text: "1"  }
            ListElement { text: "2"  }
        }
        x: 104
        y: 467
        opacity: 1
        activeFocusOnPress : true
        style: ComboBoxStyle {
            background: Image {
                id: gpo_no_box_stylecu80
                source: "images/gpi_no_box.png"
                Rectangle {
                    width: gpo_no_box_stylecu80.sourceSize.width - 28
                    height: gpo_no_box_stylecu80.sourceSize.height
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
            gpi_level_high.text = "GPI Level : "
        }
    }

    Text {
        id: gpi_level_high
        text: "GPI Level : "
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 8
        y: 510.5
        opacity: 1
    }

    Button {
        id: get_gpi_selected
        x: 132
        y: 505
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

    Image {
        id: gpi_no_drowdown
        source: "images/gpi_no_drowdown.png"
        x: 231
        y: 469
        opacity: 1
    }

    Text {
        id: flash_controls
        text: "--- Flash Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 545.5
        opacity: 0.50196078431373
    }

    Row {
        x: 47
        y: 580
        spacing: 50
        CheckBox {
            id: flash_ctrl
            text: "Flash"
            activeFocusOnPress : true
            style: econCheckBoxStyle
            onClicked:
            {
                see3camctrl.setFlashControlState(checked)
            }
            Keys.onReturnPressed: {
                if(checked)
                    checked = false
                else
                    checked = true
                see3camctrl.setFlashControlState(checked)
            }
        }
        CheckBox {
            id: torch_ctrl
            activeFocusOnPress : true
            text: "Torch"
            style: econCheckBoxStyle
            onClicked:
            {
                see3camctrl.setTorchControlState(checked)
            }
            Keys.onReturnPressed: {
                if(checked)
                    checked = false
                else
                    checked = true
                see3camctrl.setTorchControlState(checked)
            }
        }
    }
    Text {
        id: other_options
        text: "--- Other Options ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 72
        y: 606.5
        opacity: 0.50196078431373
    }

    Button {
        id: f_wversion_selected_80
        x: 85
        y: 637
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

    See3Cam80 {
        id: seecam80
        onUpdateTorchCheckBox: {
            if(torch_Check_state === "1") {
                torch_ctrl.checked = true;
            }
            else {
                torch_ctrl.checked =  false;
            }
        }
        onUpdateFlashCheckBox:  {
            if(flash_Check_state === "1") {
                flash_ctrl.checked = true;
            }
            else {
                flash_ctrl.checked =  false;
            }
        }
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
        onFocusPostionValue: {
            current_position_value.text = focusPostion
        }
        onFocusModeValue: {
            switch(focusModeValue) {
            case 1:
                focusModeBox.currentIndex = 0
                break
            case 2:
                focusModeBox.currentIndex = 1
                break
            case 3:
                focusModeBox.currentIndex = 2
                break
            default:
                focusModeBox.currentIndex = -1
            }
        }
        onFocusModeStatus: {
            switch(focusModeStatus) {
            case 0:
                messageDialog.title = qsTr("Failed")
                messageDialog.text = qsTr("Failed to set the selected focus Mode")
                messageDialog.open()
                break;
            case 1:
                //success mode
		break;
            case 2:
                //camera busy mode
		break;
            }
        }
    }

    See3CamCtrl {
        id: see3camctrl
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
    }

    See3CamGpio {
        id: see3camGpio
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
        onGpioLevel:{
            if(level === 0) {
                if(outputPinFlag)
                    radioLowcu80.checked = true
                else
                    gpi_level_high.text = "GPI Level : <b>Low</b>"
            } else {
                if(outputPinFlag)
                    radioHighcu80.checked = true
                else
                    gpi_level_high.text = "GPI Level : <b>High</b>"
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
    }

    function gpiStatus() {
        outputPinFlag = false
        if(gpi_no_boxcu80.currentIndex == 0){
            see3camGpio.getGpioLevel(See3CamGpio.IN1) //PIN Number
        } else {
            see3camGpio.getGpioLevel(See3CamGpio.IN3)  //PIN Number
        }
    }

    function singleTriggerFocus() {
       seecam80.setFocusMode(See3Cam80.Singletrigger)
    }

    function getPositionValue() {
        seecam80.getFocusPosition()
    }

    function getFirmwareVer() {
        uvccamera.getFirmWareVersion()
    }

    Component.onCompleted:{
        focusModeBox.forceActiveFocus()
        seecam80.getFlashLevel()
        seecam80.getTorchLevel()
        seecam80.getFocusMode()
        seecam80.getFocusPosition()
        outputPinFlag = true
        see3camGpio.getGpioLevel(See3CamGpio.OUT2)
    }
}
