/*
 * audiocapturesettings.qml -- display video property settings
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

//Added by Dhurka - 24th Oct 2016
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1

import econ.camera.audioinput 1.0


Item {
    id: audioCaptureProperty

    property bool cameraSelect : false
    property int stillPropertyY
    property int imageFormatY

    Action {
        id: displayAudioProperties
        onTriggered: {
            displayAudioSettings()
        }
    }

    Button {
        id: audio_Capture
        y: root.stillCaptureChildVisible ? imageFormatY + 300 + 35 :  (root.videoSettingsChildVisible? imageFormatY + 270: stillPropertyY +  70 )
        action: displayAudioProperties
        activeFocusOnPress : true
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 265
                implicitHeight: 15
                border.width: control.activeFocus ? 1 : 0
                color: control.activeFocus ? "#df643e" : audio_Capture_property_Child.visible ? "#df643e" : "#222021"
                border.color: control.activeFocus ? "#df643e" : "#222021"
            }
            label: Image {
                horizontalAlignment: Text.AlignLeft
                fillMode: Image.PreserveAspectFit
                source: "images/audiocapturesettings.png"
            }
        }
        ScrollView {
                id: audio_Capture_property_Child
                x:10
                y:35
                width: 257
                height: 200
                visible: false
                style: ScrollViewStyle {
                    scrollToClickedPosition: true
                    handle: Image {
                        id: vidCapScrollHandle
                        source: "images/scroller.png"
                    }
                    scrollBarBackground: Image {
                        id: vidCapScrollStyle
                        source: "images/Scroller_bg.png"
                    }
                    incrementControl: Image {
                        id: vidCapIncrement
                        source: "images/down_arrow.png"
                    }
                    decrementControl: Image {
                        id: vidCapDecrement
                        source: "images/up_arrow.png"
                    }
                }
                onVisibleChanged:
                {
                    root.audioCaptureChildVisible = visible
                    root.audioPropertyYValue = audio_Capture.y
                }

            Item {
                height:  250

                GridLayout {
                    id: audioCapturePropertyGrid
                    columns: 1
                    rowSpacing: 10
                    Text {
                        id: devices
                        text: "Input Devices"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }

                    ComboBox {
                        id: audioDevicesList
                        opacity: 1
                        model: audioinputDevModel
                        textRole: "display"
                        width: 120
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            label: Text {
                                anchors.fill: parent
                                color: "#ffffff"
                                text: control.currentText
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                font.family: "Ubuntu"
                                font.pixelSize: 14
                                wrapMode: Text.WordWrap
                            }
                            background: Image {
                                id: deviceImage
                                source: "images/device_box.png"
                                Rectangle {
                                    width: deviceImage.sourceSize.width - 28
                                    height: control.currentText.height
                                    color: "#222021"
                                    border.color: "white"
                                    border.width: control.activeFocus ? 3 : 1
                                    radius: control.activeFocus ? 5 : 0
                                }
                            }
                        }
                        onCurrentIndexChanged: {
                              if(currentIndex != 0){
                                  sampleFmtList.enabled = true
                                  sampleFmtList.opacity = 1
                                  sampleFmt.opacity = 1
                                  channelCountList.enabled = true
                                  channelCountList.opacity = 1
                                  channelCount.opacity = 1
                                  muteSelection.enabled = true
                                  muteSelection.opacity = 1
                                  if(!muteSelection.checked){
                                      volume.opacity = 1
                                      volume_Slider.enabled = true
                                      volume_value.enabled = true
                                      volume_value.opacity = 1
                                  }


                                  audioSettings.updateSupportedInfo(audioDevicesList.currentText, false, volume_Slider.value)
                                  root.audioDeviceSelected(currentIndex);
                                  root.setSampleRate(sampleFmtList.currentText)
                                  root.setChannelCount(channelCountList.currentText)

                              }
                              if(currentIndex == 0){
                                  sampleFmtList.enabled = false
                                  sampleFmtList.opacity = 0.1
                                  sampleFmt.opacity = 0.1
                                  channelCountList.enabled = false
                                  channelCountList.opacity = 0.1
                                  channelCount.opacity = 0.1
                                  muteSelection.enabled = false
                                  muteSelection.opacity = 0.1
                                  volume_Slider.enabled = false
                                  volume.enabled = false
                                  volume.opacity = 0.1
                                  volume_value.enabled = false
                                  volume_value.opacity = 0.1
                                  root.audioDeviceSelected(currentIndex);
                              }
                        }
                    }

                    Text {
                        id: sampleFmt
                        text: "Sample Rate"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                    ComboBox {
                        id: sampleFmtList
                        opacity: 1                        
                        model: audioSupportedFormatList
                        textRole: "display"
                        width: 120
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            label: Text {
                                anchors.fill: parent
                                color: "#ffffff"
                                text: control.currentText
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                font.family: "Ubuntu"
                                font.pixelSize: 14
                                wrapMode: Text.WordWrap
                            }
                            background: Image {
                                id: sampleFmtIg
                                source: "images/device_box.png"
                                Rectangle {
                                    width: sampleFmtIg.sourceSize.width - 28
                                    height: control.currentText.height
                                    color: "#222021"
                                    border.color: "white"
                                    border.width: control.activeFocus ? 3 : 1
                                    radius: control.activeFocus ? 5 : 0
                                }
                            }
                        }
                        onCurrentIndexChanged: {
                            if(currentText.length != 0){
                                root.setSampleRate(currentText)
                            }

                        }
                    }

                    Text {
                        id: channelCount
                        text: "Channels"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                    ComboBox {
                        id: channelCountList
                        opacity: 1
                        model: audioChannelCountModel
                        textRole: "display"
                        width: 120
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            label: Text {
                                anchors.fill: parent
                                color: "#ffffff"
                                text: control.currentText
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                font.family: "Ubuntu"
                                font.pixelSize: 14
                                wrapMode: Text.WordWrap
                            }
                            background: Image {
                                id: channelCountImg
                                source: "images/device_box.png"
                                Rectangle {
                                    width: channelCountImg.sourceSize.width - 28
                                    height: control.currentText.height
                                    color: "#222021"
                                    border.color: "white"
                                    border.width: control.activeFocus ? 3 : 1
                                    radius: control.activeFocus ? 5 : 0
                                }
                            }
                        }
                        onCurrentIndexChanged: {                            
                            if(currentText.length != 0){
                                root.setChannelCount(currentText)
                            }
                        }
                    }

                    CheckBox {
                        id: muteSelection
                        activeFocusOnPress : true
                        checked: false
                        style: CheckBoxStyle {
                            label: Text {
                                text: "Mute"
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
			    volume_value.enabled = false
                            if(checked){
                                audioSettings.updateSupportedInfo(audioDevicesList.currentText, true, 0) // 0 - mute
				volume.opacity = 0.1
				volume_Slider.enabled = false
				volume_Slider.opacity = 0.1
				volume_value.opacity = 0.1								
                            }else{
                                audioSettings.updateSupportedInfo(audioDevicesList.currentText, true, volume_Slider.value)
				volume.opacity = 1
				volume_Slider.enabled = true
				volume_Slider.opacity = 1
				volume_value.opacity = 1			
                            }
                        }
                        Keys.onReturnPressed: {
			    volume_value.enabled = false

                            if(checked){
                                audioSettings.updateSupportedInfo(audioDevicesList.currentText, true, 0) // 0 - mute
				volume.opacity = 0.1
				volume_Slider.enabled = false
				volume_Slider.opacity = 0.1
				volume_value.opacity = 0.1

                            }else{
                                audioSettings.updateSupportedInfo(audioDevicesList.currentText, true, volume_Slider.value)
				volume.opacity = 1
				volume_Slider.enabled = true
				volume_Slider.opacity = 1
				volume_value.opacity = 1

                            }
                        }
                    }
                    Row{
                        spacing: 10
                        Text {
                            id: volume
                            text: "Volume"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            opacity: muteSelection.checked ? 0.1 : 1
                        }
                        Slider {
                            activeFocusOnPress: true
                            updateValueWhileDragging: false
                            id: volume_Slider
                            width: 120
                            style:econSliderStyle
                            enabled: muteSelection.checked ? 0 : 1
                            opacity: enabled ? 1 : 0.1
                            stepSize: 1
                            minimumValue: 1
                            maximumValue: 100
                            onValueChanged: {
                                if(audioDevicesList.currentText.length != 0){
                                    audioSettings.updateSupportedInfo(audioDevicesList.currentText, true, volume_Slider.value)
                                }
                            }
                        }
                        TextField {
                            id: volume_value
                            text: volume_Slider.value
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            validator: IntValidator {bottom: volume_Slider.minimumValue; top: volume_Slider.maximumValue;}
                            enabled: false
                            opacity: 1
                            style: econTextFieldStyle
                        }
                    }

                }
            }
        }

        Keys.onReturnPressed: {
        }
        Keys.onSpacePressed: {

        }
        onFocusChanged: {
            audio_Capture_property_Child.visible = false
        }
    }

    function displayAudioSettings(){
        if(audio_Capture_property_Child.visible){
            audio_Capture_property_Child.visible = false
        }else{
            audio_Capture_property_Child.visible = true
        }
//        audioSettings.updateSupportedInfo(audioDevicesList.currentText, false, volume_Slider.value)
//        root.audioDeviceSelected(audioDevicesList.currentIndex);
//        root.setSampleRate(sampleFmtList.currentText)
//        root.setChannelCount(channelCountList.currentText)
//        muteSelection.checked = false
    }

    Connections{
        target: root
        onCameraSelected:{
            cameraSelect = true

//            audioSettings.updateSupportedInfo(audioDevicesList.currentText, false, volume_Slider.value)
//            root.audioDeviceSelected(audioDevicesList.currentIndex);
//            root.setSampleRate(sampleFmtList.currentText)
//            root.setChannelCount(channelCountList.currentText)
//            muteSelection.checked = false
        }

        onUpdateAudioMenuPosition:
        {
            imageFormatY = root.imageFormatYValue
            stillPropertyY = root.stillPropertyYValue
        }

        onSidebarVisibleStatus:
        {
            audioCaptureProperty.visible = status
        }

        onCameraSettingsTabEnable:
        {
            audioCaptureProperty.visible = status;
        }

        onAudioPropertyItemEnable:
        {
            audioCaptureProperty.enabled = enableStatus
            audioCaptureProperty.opacity = enableStatus ? 1 : 0.5
        }

    }

    Audioinput{
        id: audioSettings
        onVolumeChanged: {
            volume_Slider.value = volume
        }
    }

   Component.onCompleted: {
       cameraSelect = false
       audioDevicesList.currentIndex = -1
       root.audioPropertyYValue = audio_Capture.y
    }
}

