/*
 * uvc50.qml -- extension settings for cu50 cameras
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
import econ.camera.see3cam50 1.0
import econ.camera.see3camControl 1.0
import econ.camera.see3camGpioControl 1.0
import cameraenum 1.0
Item {
    width:268
    height:720
    property bool outputPinFlag
    id: see3CAM_cu50Camera

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
        id: gpiStatus
        onTriggered: {
            getGpiStatus()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            getFirmwareVersion()
        }
    }

    Text {
        id: gpio_controlsCU50
        text: "--- GPIO Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 205.5
        opacity: 0.50196078431373
    }

    Text {
        id: gpo_number
        text: "GP0 Number :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 6
        y: 254.5
        opacity: 1
    }

    ComboBox {
        id: gpioOutputBox
        model: ListModel {
            id: cbItems
            ListElement { text: "1"  }
            ListElement { text: "2"  }
        }
        x: 104
        y: 251
        opacity: 1
        activeFocusOnPress : true
        style: ComboBoxStyle {
            background: Image {
                id: gpobox
                source: "images/gpo_no_box.png"
                Rectangle {
                    width: gpobox.sourceSize.width - 28
                    height: gpobox.sourceSize.height
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
            if(currentIndex == 0)
                see3camGpio.getGpioLevel(See3CamGpio.OUT1)
            else
                see3camGpio.getGpioLevel(See3CamGpio.OUT2)
        }
    }

    Image {
        id: gpo_no_drowdown
        source: "images/gpo_no_drowdown.png"
        x: 231
        y: 253
        opacity: 1
    }

    Text {
        id: gpo_levelCU50
        text: "GPO Level :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 8
        y: 318.5
        opacity: 1
    }

    Row {
        x: 100
        y: 319.5
        spacing: 15
        ExclusiveGroup { id: group }
        RadioButton {
            exclusiveGroup: group
            id: radioHighcu50
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
                outputPinFlag = true
                if(gpioOutputBox.currentIndex == 0)
                    see3camGpio.setGpioLevel(See3CamGpio.OUT1,See3CamGpio.High)//Pin Number & Value
                else
                    see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.High)//Pin Number & Value
            }
            Keys.onReturnPressed: {
                checked = true
                outputPinFlag = true
                if(gpioOutputBox.currentIndex == 0)
                    see3camGpio.setGpioLevel(See3CamGpio.OUT1,See3CamGpio.High)//Pin Number & Value
                else
                    see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.High)//Pin Number & Value

            }
        }
        RadioButton {
            exclusiveGroup: group
            id: radioLowcu50
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
                outputPinFlag = true
                if(gpioOutputBox.currentIndex == 0)
                    see3camGpio.setGpioLevel(See3CamGpio.OUT1,See3CamGpio.Low);//Pin Number & Value
                else
                    see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.Low);//Pin Number & Value
            }
            Keys.onReturnPressed: {
                checked = true
                outputPinFlag = true
                if(gpioOutputBox.currentIndex == 0)
                    see3camGpio.setGpioLevel(See3CamGpio.OUT1,See3CamGpio.Low);//Pin Number & Value
                else
                    see3camGpio.setGpioLevel(See3CamGpio.OUT2,See3CamGpio.Low);//Pin Number & Value
            }
        }
    }

    Text {
        id: gpi_numberCU50
        text: "GPI Number :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 7
        y: 372.5
        opacity: 1
    }

    ComboBox {
        model: ListModel {
            id: cbItemsgpi
            ListElement { text: "1" }
            ListElement { text: "2" }
        }
        id: gpi_no_boxCU50
        x: 104
        y: 369
        opacity: 1
        activeFocusOnPress : true
        style: ComboBoxStyle {
            background: Image {
                id: gpiboxImg
                source: "images/gpi_no_box.png"
                Rectangle {
                    width: gpiboxImg.sourceSize.width - 28
                    height: gpiboxImg.sourceSize.height
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
        text: "GPI Level :"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 8
        y: 426.5
        opacity: 1
    }

    Button {
        id: get_gpi_selected
        x: 132
        y: 422
        opacity: 1
        action: gpiStatus
        activeFocusOnPress : true
        tooltip: "Get the status of Input PIN"
        style: ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 3 :0
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
                radius: 5
            }
            label: Image {
                source: "images/get_gpi_selected.png"
            }
        }
        Keys.onReturnPressed: {
            getGpiStatus()
        }
    }

    Image {
        id: gpi_no_dropdown
        source: "images/gpi_no_dropdown.png"
        x: 231
        y: 371
        opacity: 1
    }

    Text {
        id: flash_controlsCU50
        text: "--- Flash Controls ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 472.5
        opacity: 0.50196078431373
    }
    Row {
        x: 55
        y: 504.5
        spacing: 50
        CheckBox {
            id: flash_ctrl
            text: "Flash"
            checked: true
            activeFocusOnPress : true
            style: CheckBoxStyle {
                label: Text {
                    text: "Flash"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                } background: Rectangle {
                    border.width: control.activeFocus ? 1 :0
                    color: "#222021"
                    border.color: control.activeFocus ? "#ffffff" : "#222021"
                }
            }
            onClicked: {
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
            style: CheckBoxStyle {
                label: Text {
                    text: "Torch"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                } background: Rectangle {
                    border.width: control.activeFocus ? 1 :0
                    color: "#222021"
                    border.color: control.activeFocus ? "#ffffff" : "#222021"
                }
            }
            onClicked: {
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
        id: other_optionsCU50
        text: "--- Other Options ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 72
        y: 549.5
        opacity: 0.50196078431373
    }

    Button {
        id: serial_no_selected
        x: 85
        y: 582
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

    Button {
        id: f_wversion_selectedCU50
        x: 85
        y: 637
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

    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        width: 200
        onAccepted: {
            close()
        }
        Component.onCompleted:{
            close()
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

    See3Cam50 {
        id: seecam50
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
                    radioLowcu50.checked = true
                else
                    gpi_level_high.text = "GPI Level : <b>Low</b>"
            } else {
                if(outputPinFlag)
                    radioHighcu50.checked = true
                else
                    gpi_level_high.text = "GPI Level : <b>High</b>"
            }
        }
    }

    function getGpiStatus() {
        outputPinFlag = false
        if(gpi_no_boxCU50.currentIndex == 0){
            see3camGpio.getGpioLevel(See3CamGpio.IN1)  //PIN Number
        } else {
            see3camGpio.getGpioLevel(See3CamGpio.IN2)  //PIN Number
        }
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    Component.onCompleted:{
        gpioOutputBox.forceActiveFocus()
        seecam50.getFlashLevel()
        seecam50.getTorchLevel()
        outputPinFlag = true
        see3camGpio.getGpioLevel(See3CamGpio.OUT1)
    }

}
