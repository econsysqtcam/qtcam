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


    Button {
        id: serial_no_selected
        x: 85
        y: 220
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
        y: 280
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

    Component.onCompleted:{
        uvccamera.initExtensionUnit("See3CAM_CU40")
    }

    Component.onDestruction: {
        uvccamera.exitExtensionUnit()
    }
}
