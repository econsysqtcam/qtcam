/*
 * stillcapturesettings.qml -- display still property settings
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

//Added by Dhurka - 21st Oct 2016
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import "../JavaScriptFiles/tempValue.js" as JS

Item {
    id: stillproperties

    property bool stillColorSpace
    property string stillColorComboIndexValue : color_comp_box.currentIndex.toString()
    property string stillOutputTextValue : output_value.currentText.toString()
    property string stillStoragePath : storage_path.text.toString()
    property string stillImageFormatComboText : imageFormatCombo.currentText.toString()
    property string stillClorComboValue : color_comp_box.currentText.toString()
    property string captureTime
    property var stillImageFormat: ["jpg","bmp","raw","png"]

    Timer {
        id: captureTimer
        interval: 5000
        onTriggered: {
            captureTime = ""
            stop()
        }
    }

    Action {
        id: stillProperty
        onTriggered: {
            stillImageProperty()
        }
    }

    Button {
        id: still_properties // still_capture_settings
        tooltip: "Still Capture Settings - \nAllows the user to set the image color space, image resolution,\nimage path, and image format(extension) for photo capture"
        action: stillProperty
        activeFocusOnPress : true
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 265
                implicitHeight: 15
                border.width: control.activeFocus ? 1 : 0
                color: control.activeFocus ? "#df643e" : stillchildProperty.visible ? "#df643e" : "#222021"  ///*#df643e"//*/
                border.color: control.activeFocus ? "#df643e" : "#222021"
            }
            label: Image {
                horizontalAlignment: Text.AlignLeft
                fillMode: Image.PreserveAspectFit
                source: "images/stillcapturesettings.png"
            }
        }
        y: root.videoCaptureChildVisible ? 400 : 240
        opacity: 1
        Item {
            id: stillchildProperty
            visible: false
            x: 10
            onVisibleChanged:
            {
                root.stillCaptureChildVisible = visible
                root.imageFormatYValue = image_format.y
                root.stillPropertyYValue = still_properties.y
            }

            Text {
                id: color_compression
                text: "Color Space/ Compression :"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                x: 0
                y: 40
                opacity: 1
            }
            ComboBox {
                id: color_comp_box
                opacity: 1
                model: resolutionModel
                textRole: "display"
                smooth: true
                x: 0
                y: 60
                activeFocusOnPress: true
                style: ComboBoxStyle {
                    background: Image {
                        id: resolutionSize
                        source: "images/device_box.png"
                        Rectangle {
                            width: resolutionSize.sourceSize.width - 28
                            height: resolutionSize.sourceSize.height
                            color: "#222021"
                            border.color: "white"
                            border.width: control.activeFocus ? 3 : 1
                            radius: control.activeFocus ? 5 : 0
                        }
                    }
                    label: Text {
                        anchors.fill: parent
                        color: "#ffffff"
                        text: control.currentText
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Ubuntu"
                        font.pixelSize: 14
                    }
                }
                onCurrentIndexChanged: {
                    if(color_comp_box.count > 0){
                        JS.stillCaptureFormat = color_comp_box.currentIndex.toString()
                        if(JS.triggerMode_11cug === 1 || JS.triggerMode_B === 1 || JS.triggerMode_M === 1 || JS.triggerMode_cu51 === 1 || JS.triggerMode_12cuinr === 1)
                            root.triggerModeCapture()
                        if(stillColorSpace) {
                              root.updateStillPreview(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        }
                    }
                }
                Component.onCompleted:
                {
                    stillColorSpace = true
                }
            }
            Image {
                id: color_comp_dropdown
                source: "images/output_dropdown.png"
                x: 204
                y: 62
                opacity: 1
            }
            Text {
                id: output_size
                text: "Output Size :"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                x: 0
                y: 95
                opacity: 1
            }
            ComboBox {
                id: output_value
                opacity: 1
                model:stillOutputFormatModel
                textRole: "display"
                smooth: true
                x: 0
                y: 115
                activeFocusOnPress: true
                style: ComboBoxStyle {
                    background: Image {
                        id: resolutionSize2
                        source: "images/device_box.png"
                        Rectangle {
                            width: resolutionSize2.sourceSize.width - 28
                            height: resolutionSize2.sourceSize.height
                            color: "#222021"
                            border.color: "white"
                            border.width: control.activeFocus ? 3 : 1
                            radius: control.activeFocus ? 5 : 0
                        }
                    }
                    label: Text {
                        anchors.fill: parent
                        color: "#ffffff"
                        text: control.currentText
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Ubuntu"
                        font.pixelSize: 14
                    }
                }
                onCurrentIndexChanged: {
                    JS.stillCaptureResolution = output_value.currentText.toString()
                    if(JS.triggerMode_11cug === 1 || JS.triggerMode_B === 1 || JS.triggerMode_M === 1 || JS.triggerMode_cu51 === 1 || JS.triggerMode_12cuinr == 1)
                        root.triggerModeCapture()

                    root.informStillResolutionIndexChanged(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                }
            }

            Text {
                id: image_location
                text: "Image Location :"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                x: 0
                y: 155
                opacity: 1
            }
            Action {
                id: stillFileDialog
                onTriggered: {
                    fileDialog.open()
                }
            }

            Button {
                id: still_box
                x: 0
                y: 175
                width:230
                opacity: 1
                action: stillFileDialog
                activeFocusOnPress : true
                style: ButtonStyle {
                    background: Image {
                        id: stillLoc
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        fillMode: Image.PreserveAspectFit
                        source: "images/still_box.png"
                        Rectangle {
                            width: stillLoc.sourceSize.width + 15
                            height: stillLoc.sourceSize.height
                            border.width: control.activeFocus ? 3 : 1
                            color: "#222021"
                            border.color: "#ffffff"
                        }
                    }
                }
            }
            FileDialog {
                id: fileDialog
                selectFolder: true
                title: qsTr("Select a folder")
                onAccepted: {
                    storage_path.text = folder.toString().replace("file://", "")
                }
            }
            Text {
                id: storage_path
                text: SystemPictureFolder
                elide: Text.ElideLeft
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                x: 4
                y: 180
                width: 185
                opacity: 1
            }
            Image {
                id: open_folder
                source: "images/open_folder.png"
                x: 205
                y: 182
                opacity: 1
            }
            Text {
                id: image_format
                text: "Image Format :"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                x: 0
                y: image_location.y + 60
                opacity: 1
            }

            ComboBox {
                id: imageFormatCombo
                property bool checkIndex : false
                opacity: 1
                activeFocusOnPress: true
                model: stillImageFormat
                x: 0
                y: image_format.y + 20
                style: ComboBoxStyle {
                    background: Image {
                        id: imageFormatBox
                        source: "images/device_box.png"
                        Rectangle {
                            width: imageFormatBox.sourceSize.width - 28
                            height: imageFormatBox.sourceSize.height
                            color: "#222021"
                            border.color: "white"
                            border.width: control.activeFocus ? 3 : 1
                            radius: control.activeFocus ? 5 : 0
                        }
                    }
                    label:  Text{
                        anchors.fill: parent
                        color: "#ffffff"
                        text: control.currentText
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Ubuntu"
                        font.pixelSize: 14
                    }
                }
            }
        }

        Keys.onReturnPressed: {
            stillImageProperty()
        }
        Keys.onSpacePressed: {

        }
        onFocusChanged: {
            stillchildProperty.visible = false
        }
    }
    Connections
    {
        target: root
        onSidebarVisibleStatus:
        {
            stillproperties.visible = status;
        }
    }
    Connections
    {
        target: root
        onCameraSettingsTabEnable:
        {
            stillproperties.visible = status;
        }
    }
    Connections
    {
        target:root
        onCameraControlPropertyChange:
        {
            stillColorSpace = false
        }
    }
    Connections
    {
        target:root
        onStillChildVisibleState:
        {
            stillchildProperty.visible = visibleStatus;
        }
    }
    Connections
    {
        target: root
        onStillPropertyItemEnable:
        {
            stillproperties.enabled = enableStatus
            stillproperties.opacity = enableStatus ? 1 : 0.5
        }
    }
    Connections
    {
        target: root
        onInsertStillImageFormat:
        {
            stillImageFormat = []
            stillImageFormat = stillFormat
            imageFormatCombo.currentIndex = 0
        }
    }

    Component.onCompleted:
    {
        root.imageFormatYValue = image_format.y
        root.stillPropertyYValue = still_properties.y
    }

    Connections
    {
        target: root
        onSetColorComboOutputIndex:
        {
            if(isColorCombo)
            {
                color_comp_box.currentIndex = indexValue
            }
            else
            {
                output_value.currentIndex = indexValue
            }
        }
    }

    Connections
    {
        target: root
        onGetStillPropertyPositionValues:
        {
            root.imageFormatYValue = image_format.y
            root.stillPropertyYValue = still_properties.y
        }
    }

    function stillImageProperty() {
        if(!stillchildProperty.visible) {
            root.logInfo("Still Properties Selected")
            stillchildProperty.visible = true
            color_comp_box.forceActiveFocus()
            root.videoCaptureFilterChildVisible(false)
            stillColorSpace = true
        } else {
            stillchildProperty.visible = false
            stillchildProperty.focus = false
        }
    }
    function startCaptureTimer(captureSaveTimeValue)
    {
        captureTime =  captureSaveTimeValue
        captureTimer.start()
    }
}
