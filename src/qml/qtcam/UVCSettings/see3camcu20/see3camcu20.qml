/*
 * uvc_cu30.qml -- extension settings for other cameras
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
import econ.camera.see3camcu20 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {

    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnColorKillVal: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIOnAntiFlickerMode:false
    property bool skipUpdateUIOnLSCMode: false

    width:240
    height:720
    property int colorValMin: 0
    property int colorValMax: 5

    // Delay is needed to get correct auto exposure ROI mode
    Timer{
        id:autoExpROITimer
        interval: 2000
        onTriggered: {
            see3camcu20.getAutoExpROIModeAndWindowSize()
        }
    }

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            root.imageCapture(CommonEnums.BURST_SHOT);
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
        onMouseRightClicked:{
            if(autoexpManual.enabled && autoexpManual.checked){
               see3camcu20.setROIAutoExposure(See3Camcu20.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }

        onAutoExposureSelected:{
            if(autoExposureSelect){
                autoexpManual.enabled = true
                autoexpCentered.enabled = true
                if(autoexpManual.checked)
                    autoExpoWinSizeCombo.enabled = true
                if(autoexpCentered.checked)
                    autoExpoWinSizeCombo.enabled = false
                autoexpManual.opacity = 1
                autoexpCentered.opacity = 1
            }else{
                autoexpManual.enabled = false
                autoexpCentered.enabled = false
                autoExpoWinSizeCombo.enabled = false
                autoexpManual.opacity = 0.1
                autoexpCentered.opacity = 0.1
            }

            autoExpROITimer.start()
        }
    }

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle
        Item {
            height: 1300
            ColumnLayout{
                x:2
                y:5
                spacing:20
                Text {
                    id: sensor_mode
                    text: "--- Sensor Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 50
                    ExclusiveGroup { id: sensorInputGroup }
                    RadioButton {
                        id: sensorStandard
                        style:  econRadioButtonStyle
                        text:   qsTr("Standard")
                        exclusiveGroup: sensorInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            defaultValue.enabled = true
                            see3camcu20.setSensorMode(See3Camcu20.SENSOR_STANDARD)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu20.setSensorMode(See3Camcu20.SENSOR_STANDARD)
                        }
                    }
                    RadioButton {
                        id: sensorHdrDlo
                        style:  econRadioButtonStyle
                        text: qsTr("HDR")
                        exclusiveGroup: sensorInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu20.setSensorMode(See3Camcu20.SENSOR_HDR_DLO)
                        }
                        Keys.onReturnPressed: {
                            see3camcu20.setSensorMode(See3Camcu20.SENSOR_HDR_DLO)
                        }
                    }
                }
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
                Row{
                    spacing: 60
                    ExclusiveGroup { id: cameraModeGroup }
                    RadioButton {
                        id: cameraModeMaster
                        style:  econRadioButtonStyle
                        text:   qsTr("Master")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu20.setCameraMode(See3Camcu20.CAMERA_MASTER)
                        }
                        Keys.onReturnPressed: {
                            see3camcu20.setCameraMode(See3Camcu20.CAMERA_MASTER)
                        }
                    }
                    RadioButton {
                        id: cameraModeSlave
                        style:  econRadioButtonStyle
                        text: qsTr("Trigger")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            defaultValue.enabled = true
                            see3camcu20.setCameraMode(See3Camcu20.CAMERA_SLAVE)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu20.setCameraMode(See3Camcu20.CAMERA_SLAVE)
                        }
                    }
                }
                Text {
                    id: specialMode
                    text: "--- Special Effect ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 60
                    ExclusiveGroup { id: specialModeGroup }
                    RadioButton {
                        id: specialModeNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: specialModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu20.setSpecialMode(See3Camcu20.SPECIAL_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            see3camcu20.setSpecialMode(See3Camcu20.SPECIAL_NORMAL)
                        }
                    }
                    RadioButton {
                        id: specialModeGreyscale
                        style:  econRadioButtonStyle
                        text: qsTr("Greyscale")
                        exclusiveGroup: specialModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            defaultValue.enabled = true
                            see3camcu20.setSpecialMode(See3Camcu20.SPECIAL_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu20.setSpecialMode(See3Camcu20.SPECIAL_GREYSCALE)
                        }
                    }
                }
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
                            see3camcu20.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu20.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            defaultValue.enabled = true
                            see3camcu20.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            defaultValue.enabled = true
                            see3camcu20.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
                Column{
                    spacing: 15
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
                                defaultValue.enabled = true
                                see3camcu20.setStrobeMode(See3Camcu20.STROBE_FLASH_VIDEO)
                            }
                            Keys.onReturnPressed: {
                                defaultValue.enabled = true
                                see3camcu20.setStrobeMode(See3Camcu20.STROBE_FLASH_VIDEO)
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
                                see3camcu20.setStrobeMode(See3Camcu20.STROBE_TORCH)
                            }
                            Keys.onReturnPressed: {                                
                                see3camcu20.setStrobeMode(See3Camcu20.STROBE_TORCH)
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
                                see3camcu20.setStrobeMode(See3Camcu20.STROBE_OFF)
                            }
                            Keys.onReturnPressed: {
                                see3camcu20.setStrobeMode(See3Camcu20.STROBE_OFF)
                            }
                        }
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
                      spacing:60
                      ExclusiveGroup { id: roiExpogroup }
                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: autoexpCentered
                          text: "Centered"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                          // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                          // winSize is required only for manual mode
                          onClicked: {
                              see3camcu20.setROIAutoExposure(See3Camcu20.AutoExpCentered, 0, 0, 0, 0, 0);
                              autoExpoWinSizeCombo.enabled = false
                          }
                          Keys.onReturnPressed: {
                              see3camcu20.setROIAutoExposure(See3Camcu20.AutoExpCentered, 0, 0, 0, 0, 0);
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
                              see3camcu20.setROIAutoExposure(See3Camcu20.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                              autoExpoWinSizeCombo.enabled = true                              
                          }
                          Keys.onReturnPressed: {
                              see3camcu20.setROIAutoExposure(See3Camcu20.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                              autoExpoWinSizeCombo.enabled = true                              
                          }
                      }
                }
                Text {
                    id: windowSizeText
                    text: "Window Size :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (autoexpManual.enabled && autoexpManual.checked) ? 1 : 0.1
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
                            see3camcu20.setROIAutoExposure(See3Camcu20.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }

                Text {
                    id: colourKillValStatusText
                    text: "--- Colour Kill ---"
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
                        id: colourKillValSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: colorValMin
                        maximumValue: colorValMax
                        onValueChanged:  {                            
                            colorKillTextField.text = colourKillValSlider.value
                            if(skipUpdateUIOnColorKillVal){
                                see3camcu20.setColourKill(colourKillValSlider.value)
                            }
                            skipUpdateUIOnColorKillVal = true
                        }
                    }
                    TextField {
                        id: colorKillTextField
                        text: colourKillValSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: colourKillValSlider.minimumValue; top: colourKillValSlider.maximumValue}
                        onTextChanged: {                            
                            if(text.length > 0){
                                colourKillValSlider.value = colorKillTextField.text
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
                            see3camcu20.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }
                Text {
                    id: denoiseControlText
                    text: "--- Denoise Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 80
                    ExclusiveGroup { id: denoiseControlGroup }
                    RadioButton {
                        id: denoiseEnable
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu20.setDenoiseCtrlMode(See3Camcu20.DenoiseEnable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu20.setDenoiseCtrlMode(See3Camcu20.DenoiseEnable)
                        }
                    }
                    RadioButton {
                        id: denoiseDisable
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {                            
                            see3camcu20.setDenoiseCtrlMode(See3Camcu20.DenoiseDisable)
                        }
                        Keys.onReturnPressed: {                            
                            see3camcu20.setDenoiseCtrlMode(See3Camcu20.DenoiseDisable)
                        }
                    }
                }

                Text {
                    id: lscModeText
                    text: "--- Lens Correction Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 90
                    ExclusiveGroup { id: lscModeControlGroup }
                    RadioButton {
                        id: lscModeAuto
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: lscModeControlGroup
                        activeFocusOnPress: true
                        onClicked: {                            
                            see3camcu20.setLSCMode(See3Camcu20.LSC_AUTO)
                        }
                        Keys.onReturnPressed: {
                            see3camcu20.setLSCMode(See3Camcu20.LSC_AUTO)
                        }
                    }
                    RadioButton {
                        id: lscModeManual
                        style:  econRadioButtonStyle
                        text: qsTr("Manual")
                        exclusiveGroup: lscModeControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            setLSCModesBasedOnComboIndexSelection()
                        }
                        Keys.onReturnPressed: {
                            setLSCModesBasedOnComboIndexSelection()
                        }
                    }
                }
                Text {
                    id: lscModeManualModeText
                    text: "Light Source:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (lscModeManual.enabled && lscModeManual.checked) ? 1 : 0.1
                }
                ComboBox
                {
                    id: lscModeCombo
                    enabled: (lscModeManual.enabled && lscModeManual.checked) ? true : false
                    opacity: (lscModeManual.enabled && lscModeManual.checked) ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "DayLight" }
                        ListElement { text: "CWF Light" }
                        ListElement { text: "A Light" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {                        
                        if(skipUpdateUIOnLSCMode){
                            setLSCModesBasedOnComboIndexSelection()
                        }
                        skipUpdateUIOnLSCMode = true
                    }
                }

                Text {
                    id: antiFlickerMode
                    text: "--- Anti Flicker Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                      spacing:90
                      ExclusiveGroup { id: antiFlickerModegroup }
                      RadioButton {
                          exclusiveGroup: antiFlickerModegroup
                          id: antiFlickerModeAuto
                          text: "Auto"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          onClicked: {
                              see3camcu20.setAntiFlickerMode(See3Camcu20.AntiFlickerAuto);
                          }
                          Keys.onReturnPressed: {
                              see3camcu20.setAntiFlickerMode(See3Camcu20.AntiFlickerAuto);
                          }
                      }
                      RadioButton {
                          exclusiveGroup: antiFlickerModegroup
                          id: antiFlickerModeManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          onClicked: {
                              setAntiFlickerMode()
                          }
                          Keys.onReturnPressed: {
                              setAntiFlickerMode()
                          }
                      }
                }

                Text {
                    id: frequency
                    text: "Frequency :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? 1 : 0.1
                }
                ComboBox
                {
                    id: antiFlickerCombo
                    enabled: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? true : false
                    opacity: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "50 Hz" }
                        ListElement { text: "60 Hz" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {                        
                        if(skipUpdateUIOnAntiFlickerMode){
                            setAntiFlickerMode()
                        }
                        skipUpdateUIOnAntiFlickerMode = true
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



    See3Camcu20 {
        id: see3camcu20
        onDenoiseModeChanged:{
            if(denoiseMode == See3Camcu20.DenoiseEnable){
                denoiseEnable.checked = true
            }else if(denoiseMode == See3Camcu20.DenoiseDisable){
                denoiseDisable.checked = true
            }
        }

        onSensorModeReceived:{
            if(sensorMode == See3Camcu20.SENSOR_STANDARD){
                sensorStandard.checked = true
            }else if(sensorMode == See3Camcu20.SENSOR_HDR_DLO){
                sensorHdrDlo.checked = true
            }
        }
        onCameraModeReceived:{
            if(cameraMode == See3Camcu20.CAMERA_MASTER){
                cameraModeMaster.checked = true
            }else if(cameraMode == See3Camcu20.CAMERA_SLAVE){
                cameraModeSlave.checked = true
            }
        }
        onSpecialModeReceived:{
            if(specialMode == See3Camcu20.SPECIAL_NORMAL){
                specialModeNormal.checked = true
            }else if(specialMode == See3Camcu20.SPECIAL_GREYSCALE){
                specialModeGreyscale.checked = true
            }
        }
        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onStrobeModeChanged:{
            currentStrobeMode(strobeMode)
        }
        onRoiAutoExpModeChanged:{           
            if(roiMode == See3Camcu20.AutoExpCentered){
                autoexpCentered.checked = true
            }else if(roiMode == See3Camcu20.AutoExpManual){
                skipUpdateUIOnExpWindowSize = false                
                autoexpManual.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize - 1
                skipUpdateUIOnExpWindowSize = true
            }
            else if(roiMode == See3Camcu20.AutoExpDisabled){
                autoexpCentered.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
            }
        }
        onColorKillValueChanged:{
            skipUpdateUIOnColorKillVal = false
            colourKillValSlider.value = colorKillValue
            skipUpdateUIOnColorKillVal = true
        }
        onBurstLengthChanged:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onAntiFlickerModeChanged:{
            currentAntiFlickerMode(flickerMode)
        }
        onLscModeChanged:{
            currentLSCMode(lscMode)
        }
        onSetdefaultValueFailed:{
            displayMessageBox(qsTr("Failure"), qsTr("Setting default value is failed"))
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function currentLSCMode(Mode){
        skipUpdateUIOnLSCMode = false
        switch(Mode){
        case See3Camcu20.LSC_AUTO:
            lscModeAuto.checked = true
            break;
        case See3Camcu20.LSC_DAYLIGHT:
            lscModeManual.checked = true
            lscModeCombo.currentIndex  = 0
            break;
        case See3Camcu20.LSC_CWFLIGHT:
            lscModeManual.checked = true
            lscModeCombo.currentIndex  = 1
            break;
        case See3Camcu20.LSC_ALIGHT:
            lscModeManual.checked = true
            lscModeCombo.currentIndex  = 2
            break;
        }
        skipUpdateUIOnLSCMode = true
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function setLSCModesBasedOnComboIndexSelection(){
        if(lscModeCombo.currentIndex == 0)
        {
            see3camcu20.setLSCMode(See3Camcu20.LSC_DAYLIGHT)
        }else if(lscModeCombo.currentIndex == 1)
        {
            see3camcu20.setLSCMode(See3Camcu20.LSC_CWFLIGHT)
        }else if(lscModeCombo.currentIndex == 2)
        {
            see3camcu20.setLSCMode(See3Camcu20.LSC_ALIGHT)
        }
    }

    function currentAntiFlickerMode(flickerMode){ 
        switch(flickerMode){
            case See3Camcu20.AntiFlickerAuto:
                antiFlickerModeAuto.checked = true
                break
            case See3Camcu20.AntiFlicker50Hz:
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case See3Camcu20.AntiFlicker60Hz:
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }

    // set anti flicker mode in the camera based on anti flicker index in combobox
    function setAntiFlickerMode(){        
        if(antiFlickerCombo.currentIndex == 0)
          see3camcu20.setAntiFlickerMode(See3Camcu20.AntiFlicker50Hz)
        else
          see3camcu20.setAntiFlickerMode(See3Camcu20.AntiFlicker60Hz)
    }

    // selecting strobe mode
    function currentStrobeMode(mode){
        switch(mode){
        case See3Camcu20.STROBE_OFF:
            strobeOff.checked = true
            break;
        case See3Camcu20.STROBE_FLASH_VIDEO:
            strobeFlashVidoStreaming.checked = true
            break;      
        case See3Camcu20.STROBE_TORCH:
            strobeTorch.checked = true
            break;
        }
    }

    // Flip mirror modes
    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case See3Camcu20.SetBothFlipEnable:
                 flipCtrlVertical.checked = true
                 flipCtrlHorizotal.checked = true
                break;
            case See3Camcu20.SetVertiFlip:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case See3Camcu20.SetHorzFlip:
                 flipCtrlVertical.checked = false
                 flipCtrlHorizotal.checked = true
                break;
            case See3Camcu20.SetBothFlipDisable:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
        }
    }

    function setToDefaultValues(){
        defaultValue.enabled = false
        if(see3camcu20.setToDefaultValues()){
            see3camcu20.getAutoExpROIModeAndWindowSize()
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        see3camcu20.getSensorMode()
        see3camcu20.getCameraMode()
        see3camcu20.getSpecialMode()
        see3camcu20.getOrientation()
        see3camcu20.getStrobeMode()
        see3camcu20.getColourKill()
        see3camcu20.getBurstLength()
        see3camcu20.getAntiFlickerMode()
        see3camcu20.getDenoiseCtrlMode()
        see3camcu20.getLSCMode()
    }

    Component.onCompleted:{
        autoExpROITimer.start()
        getCurrentValuesFromCamera()
    }
}



