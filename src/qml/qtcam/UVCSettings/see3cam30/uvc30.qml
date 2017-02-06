/*
 * futudent.qml -- extension settings for other cameras
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
    height:720
    property int defaultNoiseVal: 8
    property int denoiseMin: 0
    property int denoiseMax: 15

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
    id: firmwareVersion
    onTriggered:
        {
            getFirmwareVersion()
        }
    }   

    Text {
        id: special_effects
        text: "--- Special Effects ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 73
        y: 205.5
        opacity: 0.50196078431373
    }    

    Grid {
        x: 23
        y: 235

        columns: 2
        spacing: 15

        ExclusiveGroup { id: sceneInputGroup }
        RadioButton {
            id: rdoEffectNormal
            style:  econRadioButtonStyle
            text:   qsTr("Normal")
            exclusiveGroup: sceneInputGroup
            activeFocusOnPress: true
            onClicked: {
                see3camcu30.setEffectMode(See3Cam30.EFFECT_NORMAL)
            }
        }
        RadioButton {
            id: rdoEffectBW
            style:  econRadioButtonStyle
            text: qsTr("Black and White")
            exclusiveGroup: sceneInputGroup
            activeFocusOnPress: true
            onClicked: {
                defaultValue.enabled = true
                see3camcu30.setEffectMode(See3Cam30.EFFECT_BLACK_WHITE)
            }
        }
        RadioButton {
            id: rdoEffectGreyScale
            style:  econRadioButtonStyle
            text: qsTr("GreyScale")
            exclusiveGroup: sceneInputGroup
            activeFocusOnPress: true
            onClicked: {
                defaultValue.enabled = true
                see3camcu30.setEffectMode(See3Cam30.EFFECT_GREYSCALE)
            }
        }
        RadioButton {
            id: rdoEffectSketch
            style:  econRadioButtonStyle
            text: qsTr("Sketch")
            exclusiveGroup: sceneInputGroup
            activeFocusOnPress: true
            onClicked: {
                defaultValue.enabled = true
                see3camcu30.setEffectMode(See3Cam30.EFFECT_SKETCH)
            }
        }
        RadioButton {
            id: rdoEffectNegative
            style:  econRadioButtonStyle
            text: qsTr("Negative")
            exclusiveGroup: sceneInputGroup
            activeFocusOnPress: true
            onClicked: {
                defaultValue.enabled = true
                see3camcu30.setEffectMode(See3Cam30.EFFECT_NEGATIVE)
            }
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
    Button {
        id: defaultValue
        opacity: 1
        x: 20
        y: 400
        activeFocusOnPress : true
        text: "Default"
        tooltip: "Click to set default values"
        style: econButtonStyle
        onClicked: {            
            defaultValue.enabled = false
            see3camcu30.setEffectMode(See3Cam30.EFFECT_NORMAL)
            rdoEffectNormal.checked = true
            see3camcu30.setDenoiseValue(defaultNoiseVal)
            deNoiseSlider.value = defaultNoiseVal
        }
        Keys.onReturnPressed: {
            defaultValue.enabled = false
            see3camcu30.setEffectMode(See3Cam30.EFFECT_NORMAL)
            rdoEffectNormal.checked = true
            see3camcu30.setDenoiseValue(defaultNoiseVal)
            deNoiseSlider.value = defaultNoiseVal
        }
    }
    Button {
        id: firmwareversion
        opacity: 1
        x: defaultValue.x + defaultValue.width + 20
        y: 400
        activeFocusOnPress : true
        text: "F/W version"
        tooltip: "Click to view firmware version"
        style: econButtonStyle
        onClicked: {
            getFirmwareVersion()
        }
        Keys.onReturnPressed: {
            getFirmwareVersion()
        }
    }

    Text {
        x: 53
        y: 330
        id: denoiseText
        text: "--- De-Noise Control ---"
        font.pixelSize: 14
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        opacity: 0.50196078431373
    }
    Slider {
        x: 23
        y: 360
        activeFocusOnPress: true
        updateValueWhileDragging: false
        id: deNoiseSlider
        width: 150
        stepSize: 1
        style:econSliderStyle
        minimumValue: denoiseMin
        maximumValue: denoiseMax
        onValueChanged:  {
            defaultValue.enabled = true
            deNoiseTextField.text = deNoiseSlider.value
            see3camcu30.setDenoiseValue(deNoiseSlider.value)
        }
    }
    TextField {
        x: deNoiseSlider.x + deNoiseSlider.width + 10
        y: 360
        id: deNoiseTextField
        text: deNoiseSlider.value
        font.pixelSize: 10
        font.family: "Ubuntu"
        smooth: true
        horizontalAlignment: TextInput.AlignHCenter
        style: econTextFieldStyle
        validator: IntValidator {bottom: deNoiseSlider.minimumValue; top: deNoiseSlider.maximumValue}
        onTextChanged: {
            if(text != ""){
                defaultValue.enabled = true
                deNoiseSlider.value = deNoiseTextField.text
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


    See3Cam30 {
        id: see3camcu30
        onSendEffectMode:{
            switch(effectMode){
            case 0x01:
                rdoEffectNormal.checked = true
                break;
            case 0x04:
                rdoEffectBW.checked = true
                break;
            case 0x07:
                rdoEffectGreyScale.checked = true
                break;
            case 0x10:
                rdoEffectSketch.checked = true
                break;
            case 0x08:
                rdoEffectNegative.checked = true
                break;
            }

        }
        onSendDenoiseValue:{
            deNoiseSlider.value = denoiseValue
        }
    }


    Component.onCompleted:{        
        see3camcu30.getEffectMode()
        see3camcu30.getDenoiseValue()        
        defaultValue.enabled = true
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
    }
}



