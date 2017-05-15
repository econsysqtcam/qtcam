/*
 * videocapturesettings.qml -- display video property settings
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
import "../JavaScriptFiles/tempValue.js" as JS

Item {
    id: videoCaptureProperty

    property bool frameRateBox
    property bool colorSpace
    property bool outputSizeBox
    property int videoPinFrameInterval
    property string videoStoragePath : SystemVideoFolder
    property int imageFormatY
    property int stillPropertyY
    property string videoColorComboText : color_comp_box_VideoPin.currentText.toString()
    property string videoColorComboIndex : color_comp_box_VideoPin.currentIndex.toString()
    property string videoFrameRate : frame_rate_box.currentText.toString()
    property string videoFrameRateIndex : frame_rate_box.currentIndex.toString()

    Action {
        id: videoCap
        onTriggered: {
            videoCapProperty()
            openVideoPinPage()
        }
    }

    Action {
        id: videoLocation
        onTriggered: {
            fileDialogVideo.open()
        }
    }

    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        onAccepted: {
            close()
        }
    }

    Button {
        id: video_Capture
        y: root.stillCaptureChildVisible ? imageFormatY + 300 : stillPropertyY + 35
        opacity: 1
        action: videoCap
        activeFocusOnPress : true
        tooltip: "Video Capture Settings - \nAllows the user to set the frame rate, preview color space,\npreview resolution, the format(extension), encoder and video path\nfor video recording"
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 265
                implicitHeight: 15
                border.width: control.activeFocus ? 1 : 0
                color: control.activeFocus ? "#df643e" : video_Capture_property_Child.visible ? "#df643e" : "#222021"
                border.color: control.activeFocus ? "#df643e" : "#222021"
            }
            label: Image {
                horizontalAlignment: Text.AlignLeft
                fillMode: Image.PreserveAspectFit
                source: "images/videocapturesettings.png"
            }
        }
        ScrollView {
                id: video_Capture_property_Child
                x:10
                y:35
                width: 257
                height: 160
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

            Item {
                height: video_location.y + 85
                GridLayout {
                    id: videoCapturePropertyGrid
                    columns: 1
                    Text {
                        id: frame_rate
                        text: "Frame Rate"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                    ComboBox {
                        id: frame_rate_box
                        opacity: 1
                        model: fpsAvailable
                        currentIndex: videoPinFrameInterval
                        textRole: "display"
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            background: Image {
                                id: frameRateBox
                                source: "images/device_box.png"
                                Rectangle {
                                    width: frameRateBox.sourceSize.width - 28
                                    height: frameRateBox.sourceSize.height
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
                        onCurrentIndexChanged: {
                            if(frameRateBox) {
                                root.informPreviewFPSChanged()
                                videoPinFrameInterval = currentIndex
                                JS.videocaptureFps = currentText
                                root.updateScenePreview(output_size_box_Video.currentText.toString(), color_comp_box_VideoPin.currentIndex.toString(),currentIndex)
                            }
                        }
                        Component.onCompleted: {
                            frameRateBox = true
                        }
                        MouseArea {
                            id: mouseArea_fps
                            anchors.fill: parent
                            onPressed: {
                                mouse.accepted = false
                            }
                            onWheel: {
                            }
                        }

                    }

                    Text {
                        id: color_comp
                        text: "Color Space/Compression"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }

                    ComboBox {
                        id: color_comp_box_VideoPin
                        opacity: 1
                        textRole: "display"
                        model: resolutionModel
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            background: Image {
                                id: colorSpace
                                source: "images/device_box.png"
                                Rectangle {
                                    width: colorSpace.sourceSize.width - 28
                                    height: colorSpace.sourceSize.height
                                    color: "#222021"
                                    border.color: "white"
                                    border.width: control.activeFocus ? 3 : 1
                                    radius: control.activeFocus ? 5 : 0
                                }
                            }
                            label: Text {
                                anchors.fill: parent
                                color: "#ffffff"
                                elide: Text.ElideRight
                                text: control.currentText
                                maximumLineCount: 1
                                verticalAlignment: Text.AlignVCenter
                                font.family: "Ubuntu"
                                font.pixelSize: 14
                            }
                        }
                        onCurrentIndexChanged: {
                            if(output_size_box_Video.count > 0){
                                if(colorSpace) {
                                    root.informVideoColorSpaceChanged()
                                    root.vidFormatChanged = true
                                    JS.videoCaptureFormat = color_comp_box_VideoPin.currentIndex.toString()
                                    root.updateVideoResolution(color_comp_box_VideoPin.currentIndex.toString(),frame_rate_box.currentIndex);
                                    root.updateFPS(currentText.toString(), output_size_box_Video.currentText.toString())
                                    root.vidFormatChanged = false
                                }
                            }
                        }
                        Component.onCompleted: {
                            colorSpace = true
                        }

                        MouseArea {
                            id: mouseArea_colorSpace
                            anchors.fill: parent
                            onPressed: {
                                mouse.accepted = false
                            }
                            onWheel: {
                            }
                        }
                    }

                    Text {
                        id: output_size_Video
                        text: "Output Size"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }

                    ComboBox {
                        id: output_size_box_Video
                        opacity: 1
                        model: videoOutputFormatModel
                        textRole: "display"
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            background: Image {
                                id: resolutionSizeVideo
                                source: "images/device_box.png"
                                Rectangle {
                                    width: resolutionSizeVideo.sourceSize.width - 28
                                    height: resolutionSizeVideo.sourceSize.height
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
                            if(output_size_box_Video.count > 0){
                                JS.videoCaptureResolution = output_size_box_Video.currentText.toString();
                                if(outputSizeBox) {
                                    root.updateFPS(color_comp_box_VideoPin.currentText.toString(), currentText.toString())
                                    root.updateScenePreview(output_size_box_Video.currentText.toString(), color_comp_box_VideoPin.currentIndex.toString(),frame_rate_box.currentIndex)      
                                }
                                // Added by Sankari: 23 Dec 2016 - To inform video resolution is changed in video capture settings
                                root.informVideoResoutionChanged()
                            }
                        }
                        Component.onCompleted: {
                            outputSizeBox = true
                        }
                        MouseArea {
                            id: mouseArea_outputSize
                            anchors.fill: parent
                            onPressed: {
                                mouse.accepted = false
                            }
                            onWheel: {
                            }
                        }
                    }

                    Text {
                        id: extension
                        text: "Video Record Format"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }

                    ComboBox {
                        id: fileExtensions
                        opacity: 1
                        model: ListModel {
                            id: extensionModel
                            ListElement { text: "avi"  }
                            ListElement { text: "mkv"  }
                        }

                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            background: Image {
                                id: fileExtension
                                source: "images/device_box.png"
                                Rectangle {
                                    width: fileExtension.sourceSize.width - 28
                                    height: fileExtension.sourceSize.height
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
                            JS.videoExtension = fileExtensions.currentText
                        }
                    }

                    Text {
                        id: encoder
                        text: "Video Encoder Format"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }

                    ComboBox {
                        id: fileEncoder
                        opacity: 1
                        model: encodersModel
                        textRole: "display"
                        activeFocusOnPress: true
                        style: ComboBoxStyle {
                            background: Image {
                                id: fileEncoderImage
                                source: "images/device_box.png"
                                Rectangle {
                                    width: fileEncoderImage.sourceSize.width - 28
                                    height: fileEncoderImage.sourceSize.height
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
                            JS.videoEncoder = fileEncoder.currentIndex
                        }
                    }

                    Text {
                        id: video_location
                        text: "Video Location"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                    Row {
                        Button {
                            id: video_box
                            width: 230
                            opacity: 1
                            activeFocusOnPress: true
                            action: videoLocation
                            tooltip:""
                            style: ButtonStyle {
                                background: Image {
                                    id: videoLoc
                                    horizontalAlignment: Image.AlignHCenter
                                    verticalAlignment: Image.AlignVCenter
                                    fillMode: Image.PreserveAspectFit
                                    source: "images/still_box.png"
                                    Rectangle {
                                        width: videoLoc.sourceSize.width + 14
                                        height: videoLoc.sourceSize.height
                                        border.width: control.activeFocus ? 3 : 1
                                        color: "#222021"
                                        border.color: "#ffffff"
                                    }
                                }
                            }
                            Text {
                                id: storage_pathVideo
                                text: SystemVideoFolder
                                elide: Text.ElideLeft
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                x: 4
                                y: 4
                                width: 195
                                opacity: 1
                            }
                            Image {
                                id: open_folderVideo
                                source: "images/open_folder.png"
                                x: 205
                                y: 6
                                opacity: 1
                            }
                        }
                        FileDialog {
                            id: fileDialogVideo
                            selectFolder: true
                            title: qsTr("Select a folder")
                            onAccepted: {
                                storage_pathVideo.text = folder.toString().replace("file://", "")
                                videoStoragePath = storage_pathVideo.text
                            }
                        }

                    }
                }
            }
        }

        Keys.onReturnPressed: {
            videoCapProperty()
        }
        Keys.onSpacePressed: {

        }
        onFocusChanged: {
            video_Capture_property_Child.visible = false
        }
    }
    Connections
    {
        target: root
        onSidebarVisibleStatus:
        {
            videoCaptureProperty.visible = status

        }
    }
    Connections
    {
        target: root
        onCameraSettingsTabEnable:
        {
            videoCaptureProperty.visible = status;
        }
    }
    Connections
    {
        target:root
        onCameraControlPropertyChange:
        {
            outputSizeBox = false
            frameRateBox = false
            colorSpace = false
        }
    }

    Connections
    {
        target: root
        onVideoFrameInterval:
        {
            videoPinFrameInterval = frameInterval
            frame_rate_box.currentIndex = videoPinFrameInterval
        }
    }

    Connections
    {
        target: root
        onVideoPropertyItemEnable:
        {
            videoCaptureProperty.enabled = enableStatus
            videoCaptureProperty.opacity = enableStatus ? 1 : 0.5
        }
    }
    Connections
    {
        target: root
        onVideoChildMenuVisible:
        {
            video_Capture_property_Child.visible = visibleStatus
        }
    }
    Connections
    {
        target: root
        onSetVideoColorComboOutputIndex:
        {
            if(isColorCombo)
            {
                color_comp_box_VideoPin.currentIndex = indexValue
            }
            else
            {
                output_size_box_Video.currentIndex = indexValue
            }
        }
    }

    Connections
    {
        target: root
        onUpdateVideoMenuPosition:
        {
            imageFormatY = root.imageFormatYValue
            stillPropertyY = root.stillPropertyYValue
        }
    }

    function videoCapProperty() {
        if(!video_Capture_property_Child.visible) {
            root.logInfo("Video Capture Property selected")
            root.stillChildVisibleState(false)
            root.videoCaptureFilterChildVisible(false)
            video_Capture_property_Child.visible = true
            frame_rate_box.forceActiveFocus()
        } else {
            video_Capture_property_Child.visible = false
        }
    }

    function videoPin() {
        outputSizeBox = true
        frameRateBox = true
        colorSpace = true
    }

    function openVideoPinPage()
    {
        root.getVideoPinStatus();
    }

    function enableVideoPin(videoPinEnable)
    {
        if(videoPinEnable)
        {
            videoPin()
        }
        else
        {
            video_Capture_property_Child.visible = false
            messageDialog.title = qsTr("Disabled")
            messageDialog.text = qsTr("Video Capture Settings is disabled in trigger Mode")
            messageDialog.open()
        }
    }
}

