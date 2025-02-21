/*
 * ecam514_usb.qml -- extension settings for e-CAM512_USB camera
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

import econ.camera.ecam514_usb 1.0
import QtQuick 2.0
import QtQuick.Controls 1.1

import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item {
    width:268
    height:750

    property bool skipUpdateUIOnBurstLength: false

    Action {
        id: firmwareVersion
        onTriggered:
        {
            getFirmwareVersion()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }

    Action {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }

    Action {
        id: resetDevice
        onTriggered: {
            ecam514usb.resetDevice()
        }
    }

    Action {
        id: saveConfiguration
        onTriggered: {
            ecam514usb.saveConfiguration()
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
        function onSkipFrameCount(){
            root.setSkipFrameCount(3)
        }
    }
    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle

        Item {
            height:1600

            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text
                {
                     id: chroma_dent_mode
                     text: "--- Chroma Dent Mode ---"
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

                    ExclusiveGroup { id: chromaDentModeGroup }
                    RadioButton
                    {
                        id: dentModeNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: chromaDentModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.NORMAL)
                        }
                        Keys.onReturnPressed:  {
                            ecam514usb.setChromaDentMode(ECAM514_USB.NORMAL)
                        }
                    }
                    RadioButton
                    {
                        id: dentModeCavity
                        style:  econRadioButtonStyle
                        text: qsTr("Cavity")
                        exclusiveGroup: chromaDentModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.CAVITY_MODE)
                        }
                        Keys.onReturnPressed: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.CAVITY_MODE)
                        }
                    }
                    RadioButton
                    {
                        id: dentModeGumHealth
                        style:  econRadioButtonStyle
                        text: qsTr("Gum Health")
                        exclusiveGroup: chromaDentModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.GUM_HEALTH_MODE)
                        }
                        Keys.onReturnPressed: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.GUM_HEALTH_MODE)
                        }
                    }
                    RadioButton
                    {
                        id: dentModeErosion
                        style:  econRadioButtonStyle
                        text: qsTr("Erosion")
                        exclusiveGroup: chromaDentModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.EROSION_MODE)
                        }
                        Keys.onReturnPressed: {
                            ecam514usb.setChromaDentMode(ECAM514_USB.EROSION_MODE)
                        }
                    }
                    RadioButton
                    {
                        id: dentModeCrackDetect
                        style:  econRadioButtonStyle
                        text: qsTr("Crack Detection")
                        exclusiveGroup: chromaDentModeGroup
                        activeFocusOnPress: true
                        onClicked:
                        {
                            ecam514usb.setChromaDentMode(ECAM514_USB.CRACK_DETECTION_MODE)
                        }
                        Keys.onReturnPressed:
                        {
                            ecam514usb.setChromaDentMode(ECAM514_USB.CRACK_DETECTION_MODE)
                        }
                    }
                    }
                    RadioButton
                    {
                        id: dentModeplaque
                        style:  econRadioButtonStyle
                        text: qsTr("Plaque / Tartar")
                        exclusiveGroup: chromaDentModeGroup
                        activeFocusOnPress: true
                        onClicked:
                        {
                            ecam514usb.setChromaDentMode(ECAM514_USB.PLAQUE_TARTAR_MODE)
                        }
                        Keys.onReturnPressed:
                        {
                            ecam514usb.setChromaDentMode(ECAM514_USB.PLAQUE_TARTAR_MODE)
                        }
                }

                Text {
                    id: ledStatusTxt
                    text: "--- Led Status ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 50
                    ExclusiveGroup { id: ledStatusGroup }
                    Layout.alignment: Qt.AlignCenter

                    RadioButton {
                        id: ledStatOn
                        style:  econRadioButtonStyle
                        text:   qsTr("ON")
                        exclusiveGroup: ledStatusGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam514usb.setLedStatus(ECAM514_USB.LED_ON)
                        }
                        Keys.onReturnPressed: {
                            ecam514usb.setLedStatus(ECAM514_USB.LED_ON)
                        }
                    }

                    RadioButton {
                        id: ledStatOff
                        style:  econRadioButtonStyle
                        text: qsTr("OFF")
                        exclusiveGroup: ledStatusGroup
                        activeFocusOnPress: true
                        onClicked: {
                            ecam514usb.setLedStatus(ECAM514_USB.LED_OFF)
                        }
                        Keys.onReturnPressed: {
                            ecam514usb.setLedStatus(ECAM514_USB.LED_OFF)
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
                            ecam514usb.setBurstLength(burstLengthCombo.currentText)
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
                        action: setDefault
                        style: econButtonStyle
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    spacing: 10

                    Button {
                        id: firmwareVer
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Firmware Version"
                        tooltip: "Click to read Firmware version"
                        action: firmwareVersion
                        style: econButtonStyle
                    }
                    Button {
                        id: serialNum
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Serial Number"
                        tooltip: "Click to read Serial Number"
                        action: serialNumber
                        style: econButtonStyle
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    spacing: 10

                    Button {
                        id: resetDev
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Reset Device"
                        action: resetDevice
                        style: econButtonStyle
                    }
                    Button {
                        id: saveConfig
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Save Configuration"
                        action: saveConfiguration
                        style: econButtonStyle
                    }
                }
            }
        }
   }
   ECAM514_USB{
     id:ecam514usb

     onChromaDentModeReceived:
     {
         currentChromaDentMode(chromaDentMode)
     }
     onLedStatusReceived: {
         currentLedStatus(ledStatus)
     }

     onBurstLengthValueRecieved:
     {
         skipUpdateUIOnBurstLength = false
         burstLengthCombo.currentIndex = burstLen - 1
         skipUpdateUIOnBurstLength = true
     }
     //Signal for command Prompt

     onIndicateCommandStatus:{
         messageDialog.title = title
         messageDialog.text = text
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

    function currentChromaDentMode(chromaDentMode)
    {
        switch(chromaDentMode)
        {
            case ECAM514_USB.NORMAL:
                dentModeNormal.checked = true
                break
            case ECAM514_USB.CAVITY_MODE:
                dentModeCavity.checked = true
                break
            case ECAM514_USB.GUM_HEALTH_MODE:
                dentModeGumHealth.checked = true
                break
            case ECAM514_USB.EROSION_MODE:
                dentModeErosion.checked = true
                break
            case ECAM514_USB.CRACK_DETECTION_MODE:
                dentModeCrackDetect.checked = true
                break
            case ECAM514_USB.PLAQUE_TARTAR_MODE:
                dentModeplaque.checked = true
                break
        }
    }

    function currentLedStatus(ledStatus){
        switch(ledStatus){
            case ECAM514_USB.LED_ON:
                ledStatOn.checked = true
                break
            case ECAM514_USB.LED_OFF:
                ledStatOff.checked = true
                break
        }
    }

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        ecam514usb.setToDefaultValues()
        getValuesFromCamera()
    }

    function getSerialNumber(){
        ecam514usb.get64BitSerialNumber()
    }

    function getFirmwareVersion(){
        ecam514usb.readFirmwareVersion()
    }

    function getValuesFromCamera(){
        ecam514usb.getChromaDentMode()
        ecam514usb.getLedStatus()
        ecam514usb.getBurstLength()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}

