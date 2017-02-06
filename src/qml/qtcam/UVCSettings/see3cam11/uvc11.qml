/*
 * uvc11.qml -- extension settings for 11cug cameras
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
import QtQuick.Layouts 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam11 1.0
import "../../JavaScriptFiles/tempValue.js" as JS
import cameraenum 1.0
Item {
    width:268
    height:720
    id: see3CAM_11CUGCamera
    property bool masterMode
    property bool triggerMode

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            if(JS.masterMode_11cug === 1)
            {
                root.imageCapture(CommonEnums.SNAP_SHOT);
            }
            else
            {
                if(isWebKeyPressed)
                {
                    root.imageCapture(CommonEnums.TRIGGER_SHOT);
                }
            }
        }
        onGetVideoPinStatus:
        {
            var videoPin = JS.masterMode_11cug === 1 ? true : false
            root.enableVideoPin(videoPin);
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
        onCameraDeviceUnplugged:
        {
            JS.enableMasterMode_11cug()
        }
        onSetMasterMode:
        {
            enableMasterMode();
        }
    }

    Timer {
        id: masterModeTimer
        interval: 1000
        onTriggered: {
            masterModeCapture()
            stop()
        }
    }

    Action {
        id: masterModeAction
        onTriggered: {
            enableMasterMode()
        }
    }

    Action {
        id: triggerModeAction
        onTriggered: {
            enableTriggerMode()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            displayFirmwareVersion()
        }
    }

    Action {
        id: croppedMode
        onTriggered: {
            croppedModeDisplay()
        }
    }

    Action {
        id: binnedMode
        onTriggered: {
            binnedModeDisplay()
        }
    }

    Action {
        id: defValue
        onTriggered: {
            defaultValue()
        }
    }

    Button {
        id: mastermmode_selected
        x: 19
        y: 207
        opacity: 1
        action: masterModeAction
        activeFocusOnPress : true
        tooltip: "Set Camera in Master Mode"
        text: "Master Mode"
        style: econ11CUGButtonStyle
        Keys.onReturnPressed: {
             enableMasterMode()
        }
    }

    Button {
        id: trigger_mode_selected
        x: 145
        y: 207
        opacity: 1
        action: triggerModeAction
        activeFocusOnPress : true
        tooltip: "Set Camera in Trigger Mode"
        text: "Trigger Mode"
        style: econ11CUGButtonStyle
        Keys.onReturnPressed: {
             enableTriggerMode()
        }
    }
    Button {
        id: f_wversion_selected
        x: 79
        y: 270
        opacity: 1
        action: firmwareVersion
        activeFocusOnPress : true
        text: "F/W Version"
        tooltip: "Click to see the firmware version of the camera"
        style: econ11CUGButtonStyle
        Keys.onReturnPressed: {
            displayFirmwareVersion()
        }
    }
    Button {
        id: vga60fps_selected
        x: 19
        y: 332
        opacity: 1
        action: croppedMode
        activeFocusOnPress : true
        tooltip: "Set the camera in cropped mode"
        text: "VGA 60 fps<br>(Cropped)"
        style:econ11CUGButtonStyle
        Keys.onReturnPressed: {
            croppedModeDisplay()
        }
    }
    Button {
        id: vga30fps_selected
        x: 145
        y: 332
        opacity: 1
        action: binnedMode
        activeFocusOnPress : true
        tooltip: "Set the camera in binned mode"
        text: "VGA 30 fps<br>(Binned)"
        style: econ11CUGButtonStyle
        Keys.onReturnPressed: {
            binnedModeDisplay()
        }
    }
    Text {
        id: uvc_whitebalance
        text: "---White Balance---"
        font.pixelSize: 16
        font.family: "SegoeUI-Light"
        color: "#ffffff"
        smooth: true
        x: 60
        y: 411
        opacity: 0.50196078431373
    }

    CheckBox {
        id: autoSelect_Unselect
        x: 22
        y: 570
        property bool wbBox: false
        activeFocusOnPress : true
        style: CheckBoxStyle {
            label: Text {
                text: "Auto"
                font.pixelSize: 13
                font.family: "SegoeUI-Light"
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
            enableWbAutoMode()
        }
        Keys.onReturnPressed: {
            if(checked)
                checked = false
            else
                checked = true
            enableWbAutoMode()
        }

        Component.onCompleted: {
            wbBox = seecam11.getWbMode()
            if(!wbBox)
            {
                messageDialog.title = qsTr("Failure")
                messageDialog.text = qsTr("Unable to get the whitebalance mode")
                messageDialog.open()
            }
        }
    }
    GridLayout {
        id: wbSliders
        columns: 3
        rowSpacing: 15
        columnSpacing: 20
        x:5
        y: 440
        Text {
            id: uvc_red
            text: "Red"
            font.pixelSize: 14
            font.family: "Ubuntu"
            color: "#ffffff"
            smooth: true
            opacity: 1
        }
        Slider {
            property bool wbRed: false
            property int wbRedValue: 0
            id: redSlider
            updateValueWhileDragging: false
            activeFocusOnPress : true
            width: 135
            minimumValue: 0
            maximumValue: 255
            stepSize: 1
            enabled: autoSelect_Unselect.checked ? false : true
            opacity: autoSelect_Unselect.checked ? 0.1 : 1
            style: econSliderStyle
            onValueChanged: {
                wbRed = seecam11.setWbValues(1,redSlider.value);
                if(!wbRed){
                    messageDialog.title = qsTr("Failure")
                    messageDialog.text = qsTr("Unable to set Values")
                    messageDialog.open()
                }
            }
            onEnabledChanged: {
                wbRed = seecam11.setWbValues(1,redSlider.value);
                if(!wbRed){
                    messageDialog.title = qsTr("Failure")
                    messageDialog.text = qsTr("Unable to set Values")
                    messageDialog.open()
                }
            }
            Component.onCompleted: {
                redSlider.value = seecam11.getWbValues(See3Cam11.Red);
            }
        }

        TextField {
            id: uvc_red_value
            text: redSlider.value
            font.pixelSize: 14
            font.family: "Ubuntu"
            style: econTextFieldStyle
            enabled: false
            smooth: true
            opacity: 1
        }
        Text {
            id: uvc_green
            text: "Green"
            font.pixelSize: 14
            font.family: "Ubuntu"
            color: "#ffffff"
            smooth: true
            opacity: 1
        }
        Slider {
            property bool wbGreen: false
            property int wbGreenValue: 0
            id: greenSlider
            width: 135
            minimumValue: 0
            maximumValue: 255
            updateValueWhileDragging: false
            activeFocusOnPress : true
            stepSize: 1
            enabled: autoSelect_Unselect.checked ? false : true
            opacity: autoSelect_Unselect.checked ? 0.1 : 1
            style: econSliderStyle
            onValueChanged: {
                wbGreen = seecam11.setWbValues(2,greenSlider.value);
                if(!wbGreen){
                    messageDialog.title = qsTr("Failure")
                    messageDialog.text = qsTr("Unable to set Values")
                    messageDialog.open()
                }
            }
            onEnabledChanged: {
                wbGreen = seecam11.setWbValues(2,greenSlider.value);
                if(!wbGreen){
                    messageDialog.title = qsTr("Failure")
                    messageDialog.text = qsTr("Unable to set Values")
                    messageDialog.open()
                }
            }
            Component.onCompleted: {
                greenSlider.value = seecam11.getWbValues(See3Cam11.Green);
            }
        }

        TextField {
            id: uvc_green_value
            text: greenSlider.value
            font.pixelSize: 14
            font.family: "Ubuntu"
            style: econTextFieldStyle
            enabled: false
            smooth: true
            opacity: 1
        }

        Text {
            id: uvc_blue
            text: "Blue"
            font.pixelSize: 14
            font.family: "Ubuntu"
            color: "#ffffff"
            smooth: true
            opacity: 1
        }

        Slider {
            property bool wbBlue: false
            property int wbBlueValue: 0
            id: blueSlider
            width: 135
            updateValueWhileDragging: false
            activeFocusOnPress : true
            minimumValue: 0
            maximumValue: 255
            stepSize: 1
            enabled: autoSelect_Unselect.checked ? false : true
            opacity: autoSelect_Unselect.checked ? 0.1 : 1
            style: econSliderStyle
            onValueChanged: {
                wbBlue = seecam11.setWbValues(3,blueSlider.value);
                if(!wbBlue){
                    messageDialog.title = qsTr("Failure")
                    messageDialog.text = qsTr("Unable to set Values")
                    messageDialog.open()
                }
            }
            onEnabledChanged: {
                wbBlue = seecam11.setWbValues(3,blueSlider.value);
                if(!wbBlue){
                    messageDialog.title = qsTr("Failure")
                    messageDialog.text = qsTr("Unable to set Values")
                    messageDialog.open()
                }
            }
            Component.onCompleted: {
                blueSlider.value = seecam11.getWbValues(See3Cam11.Blue);
            }
        }

        TextField {
            id: uvc_blue_value
            text: blueSlider.value
            font.pixelSize: 14
            font.family: "Ubuntu"
            style: econTextFieldStyle
            enabled: false
            smooth: true
            opacity: 1
        }
    }

    Button {
        id: default_selected
        x: 180
        y: 563
        opacity: 1
        action: defValue
        activeFocusOnPress : true
        tooltip: "Set the Extension setting white balance control to default values"
        style: ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 3 :0
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
                radius: 5
            }
            label: Image {
                source: "images/default_selected.png"
            }
        }
        Keys.onReturnPressed: {
            defaultValue()
        }
    }

    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        onAccepted: {
            close()
        }
        Component.onCompleted: close()
    }
    Uvccamera {
        id: uvccamera
        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }
    }

    See3Cam11 {
        id: seecam11
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
        onDefaultValue: {
            if(autoMode === 1) {
                autoSelect_Unselect.checked = true
            }
            else {
                autoSelect_Unselect.checked = false
            }
            redSlider.value = redValue
            greenSlider.value = greenValue
            blueSlider.value = blueValue
        }
        onAuto_manualMode: {
            if(cbValue === 1) {
                autoSelect_Unselect.checked = true              
            }
            else {
                autoSelect_Unselect.checked = false               
            }
        }
    }

    function enableMasterMode() {
       masterModeCapture();
        masterMode = seecam11.enableMasterMode()
        if(masterMode) {
            masterModeTimer.start()
            JS.enableMasterMode_11cug()
            if(!vga60fps_selected.enabled) {
                vga60fps_selected.enabled = true
                vga60fps_selected.opacity = 1
                vga30fps_selected.enabled = true
                vga30fps_selected.opacity = 1
            }

        }
        else {
            messageDialog.title = "Failure"
            messageDialog.text = "Master Mode enabling failed"
            messageDialog.open()
        }
    }

    function enableTriggerMode() {
        triggerModeCapture()
        triggerMode = seecam11.enableTriggerMode()
        if(triggerMode) {
            JS.enableTriggerMode_11cug()
            vga60fps_selected.enabled = false
            vga60fps_selected.opacity = 0.2
            vga30fps_selected.enabled = false
            vga30fps_selected.opacity = 0.2
        }
        else {
            messageDialog.title = "Failure"
            messageDialog.text = "Trigger Mode enabling failed"
            messageDialog.open()
        }
    }

    function displayFirmwareVersion() {
        uvccamera.getFirmWareVersion()
    }

    function croppedModeDisplay() {
         seecam11.setCroppedVGAMode()
    }

    function binnedModeDisplay() {
        seecam11.setBinnedVGAMode()
    }

    function defaultValue() {
        seecam11.defaultValues()
    }

    function enableWbAutoMode() {
        if(autoSelect_Unselect.checked) {
            seecam11.setWbMode(See3Cam11.Auto)
        } else {
            seecam11.setWbMode(See3Cam11.Manual)
        }

    }

    Component {
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 40
                implicitHeight: 20
                border.color: "#333"
                border.width: 2
                y: -1
            }
        }
    }

    Component {
        id: econ11CUGButtonStyle
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

    Component.onCompleted:{        
        mastermmode_selected.forceActiveFocus()
        if(JS.triggerMode_11cug === 1) {
            vga60fps_selected.enabled = false
            vga60fps_selected.opacity = 0.2
            vga30fps_selected.enabled = false
            vga30fps_selected.opacity = 0.2
        }
    }
}
