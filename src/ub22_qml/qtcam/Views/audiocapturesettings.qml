/*
 * audiocapturesettings.qml -- display video property settings
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
    property bool skipSettingVolume: false
    property bool skipSettingMuteState: false


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
        onClicked:{
          root.minimizeWindow()
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
                        onPressedChanged:{
                            root.minimizeWindow()
                        }
                        MouseArea{
                            anchors.fill: parent
                            onPressed: {
                                if(pressed) {
                                    root.enumerateAudioSettings()
                                }
                                mouse.accepted = false
                            }
                            onWheel: {
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


                                  audioSettings.updateSupportedInfo(currentIndex)
                                  root.audioDeviceSelected(currentIndex);
                                  root.setSampleRate(sampleFmtList.currentText)
                                  root.setChannelCount(channelCountList.currentText)
                                  skipSettingVolume = false

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
                        onPressedChanged:{
                            root.minimizeWindow()
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
                        onPressedChanged:{
                            root.minimizeWindow()
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
                            doMuteEnableDisable()
                            root.minimizeWindow()
                        }
                        Keys.onReturnPressed: {
                            doMuteEnableDisable()
                        }
                        onCheckedChanged: {
                            doMuteEnableDisable()
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
                            stepSize: 17
                            minimumValue: 1
                            maximumValue: 100
                            onValueChanged: {
                                root.minimizeWindow()
                                if(audioDevicesList.currentText.length != 0){
                                    if(!skipSettingVolume){
                                        audioSettings.setVolume(volume_Slider.value)
                                        skipSettingVolume = false
                                    }
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

    function doMuteEnableDisable()
    {
        volume_value.enabled = false
        if(!skipSettingMuteState){ // To skip setting of mute state when getting mute state from camera.
            audioSettings.setMuteState(muteSelection.checked);
        }
        skipSettingMuteState = false;
        if(muteSelection.checked){           
            volume.opacity = 0.1
            volume_Slider.enabled = false
            volume_Slider.opacity = 0.1
            volume_value.opacity = 0.1
        }else{            
            volume.opacity = 1
            volume_Slider.enabled = true
            volume_Slider.opacity = 1
            volume_value.opacity = 1
        }
    }
    function displayAudioSettings(){
        if(audio_Capture_property_Child.visible){
            audio_Capture_property_Child.visible = false
        }else{
            audio_Capture_property_Child.visible = true
        }
    }

    Connections{
        target: root
        function onCameraSelected(){
            cameraSelect = true
        }

       function onCameraDeviceUnplugged()
        {
            audioDevicesList.currentIndex = 0
            audio_Capture_property_Child.visible = false
        }

	      // Added by Sankari : Mar 7 - disable audio settings
        function onDisableAudioSettings(disableSettings){
            if(disableSettings){
                audioCaptureProperty.enabled = false
                audioCaptureProperty.opacity = 0.1
            }
            else{
                audioCaptureProperty.opacity = 1
                audioCaptureProperty.enabled = true
            }
        }

        function onUpdateAudioMenuPosition()
        {
            imageFormatY = root.imageFormatYValue
            stillPropertyY = root.stillPropertyYValue
        }

        function onSidebarVisibleStatus(status)
        {
            audioCaptureProperty.visible = status
        }

        function onCameraSettingsTabEnable(status)
        {
            audioCaptureProperty.visible = status;
        }

        function onAudioPropertyItemEnable(enableStatus)
        {
            audioCaptureProperty.enabled = enableStatus
            audioCaptureProperty.opacity = enableStatus ? 1 : 0.5
        }

    }

    Audioinput{
        id: audioSettings
        onVolumeChanged: {
            skipSettingVolume = true
            volume_Slider.value = volume
        }       
        onMuteStateChanged:{
            skipSettingMuteState = true
            if(muteState == 0){
                muteSelection.checked = true
            }else{
                muteSelection.checked = false
            }
        }
    }

   Component.onCompleted: {
       cameraSelect = false
       audioDevicesList.currentIndex = -1
       root.audioPropertyYValue = audio_Capture.y
    }
}

