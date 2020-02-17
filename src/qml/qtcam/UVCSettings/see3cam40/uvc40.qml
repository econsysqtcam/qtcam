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
import econ.camera.see3camControl 1.0
import econ.camera.see3camcu40 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:268
    height:720
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
            stillImageFormat.push("IR data(8bit BMP)")
            root.insertStillImageFormat(stillImageFormat);
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
                activeFocusOnPress: true
                style: econRadioButtonStyle
                onClicked: {
                    see3camcu40.setFlashState(See3Camcu40.FLASHMODE_STROBE)
                }
                Keys.onReturnPressed: {
                    see3camcu40.setFlashState(See3Camcu40.FLASHMODE_STROBE)
                }
            }
        }
        Column{
            RadioButton {
                exclusiveGroup: flashGrp
                checked: false
                id: flashModeTorch
                text: "Torch"
                activeFocusOnPress: true
                style: econRadioButtonStyle
                onClicked: {
                    see3camcu40.setFlashState(See3Camcu40.FLASHMODE_TORCH)
                }
                Keys.onReturnPressed: {
                    see3camcu40.setFlashState(See3Camcu40.FLASHMODE_TORCH)
                }
            }
        }
        Column{
            RadioButton {
                exclusiveGroup: flashGrp
                checked: false
                id: flashModeOff
                text: "OFF"
                activeFocusOnPress: true
                style: econRadioButtonStyle
                onClicked: {
                    see3camcu40.setFlashState(See3Camcu40.FLASHMODE_OFF)
                }
                Keys.onReturnPressed: {
                    see3camcu40.setFlashState(See3Camcu40.FLASHMODE_OFF)
                }
            }
        }
    }



    Button {
        id: serial_no_selected
        x: 85
        y: 270
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
        id: f_wversion_selectedCU40
        x: 85
        y: 320
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

    See3Camcu40{
        id: see3camcu40
        onFlashModeValue:{
            currentFlashMode(flashMode)
        }
    }

    function currentFlashMode(mode){
        switch(mode){
        case See3Camcu40.FLASHMODE_TORCH:
            flashModeTorch.checked = true
            break;
        case See3Camcu40.FLASHMODE_STROBE:
            flashModeStrobe.checked = true
            break;
        case See3Camcu40.FLASHMODE_OFF:
            flashModeOff.checked = true
            break;
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

    See3CamCtrl {
        id: see3camctrl
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
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
    Component.onCompleted: {
        see3camcu40.getFlashState();
    }
}
