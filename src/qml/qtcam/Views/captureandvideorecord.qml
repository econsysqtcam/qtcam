/*
 * captureandrecord.qml -- take capture and video recording
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

Item {
    id: captureRecord

    property bool captureBtnEnable :capture.enabled
    property bool recordBtnEnable :record.enabled
    property bool captureBtnVisible :capture.visible
    property bool recordBtnVisible :record.visible
    property bool recordStopBtnVisible :record_stop.visible

    property int hours
    property int minutes
    property int seconds

    Timer {
        id: videoTimer
        interval: 1000
        repeat: true
        property int timeText: 0
        onTriggered: {
            updateTime()
        }
    }

    seconds: 0
    minutes: 0
    hours: 0

    Action {
        id: cameraBtn
        onTriggered: {
            captureBtnSelected()
        }
    }

    Action {
        id: videoBtn
        onTriggered: {
            videoBtnSelected()
        }
    }

    Action {
        id: photoCapture
        enabled: capture.enabled ?  true : false
        onTriggered: {
            root.mouseClickCapture()
        }
    }

    Action {
        id: videoRecord
        enabled: record.enabled ?  true : false
        onTriggered: {
            root.videoRecordBegin()
        }
    }

    Action {
        id: saveVideo
        onTriggered: {
            root.videoSaveVideo()
        }
    }

    Image {
        id: camera_box
        source: "images/camera_box.png"
        x: 18
        y: 18
        opacity: 1
    }
    Image {
        id: selection_bg
        source: "images/selection_bg.png"
        visible: false
        y: 197
        opacity: 1
    }
    Image {
        id: toggle_border
        source: "images/toggle_border.png"
        x: 5
        y: 153
        opacity: 1
    }
    Row {
        spacing: 12
        x: 45
        y: 27
        Button {
            id: camera_btn
            opacity: 1
            tooltip: "Camera Mode Selection"
            action: cameraBtn
            activeFocusOnPress : true
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 : 0
                    color: "#222021"
                    border.color: "#ffffff"
                }
                label: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    fillMode: Image.PreserveAspectFit
                    source: "images/camera_btn.png"
                }
            }
            Keys.onReturnPressed: {
                captureBtnSelected()
            }
        }
        Image {
            id: camera_selected
            source: "images/camera_selected.png"
            opacity: 1
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if((mouseX > (camera_selected.paintedWidth/2)) && !record_stop.visible) {
                        video_selected.visible = true;
                        camera_selected.visible = false;
                        capture.visible = false
                        record.visible = true
                        //Added by sankari - When this mouse area clicked, highlight video button
                        video_btn.forceActiveFocus()
                    }
                }
            }
        }
        Image {
            id: video_selected
            source: "images/video_selected.png"
            opacity: 1
            visible: false
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if((mouseX < (video_selected.paintedWidth/2)) && !record_stop.visible) {
                        video_selected.visible = false;
                        camera_selected.visible = true;
                        capture.visible = true
                        record.visible = false
                        //Added by sankari - When this mouse area clicked, highlight camera button
                        camera_btn.forceActiveFocus()
                    }
                }
            }
        }
        Button {
            id: video_btn
            opacity: 1
            tooltip: "Video Mode Selection"
            action: videoBtn
            y: 2
            activeFocusOnPress : true
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 : 0
                    color: "#222021"
                    border.color: "#ffffff"
                }
                label: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    fillMode: Image.PreserveAspectFit
                    source: "images/video_btn.png"
                }
            }
            Keys.onReturnPressed: {
                videoBtnSelected()
            }
        }
        Button {
            id: capture
            enabled: false
            visible: true
            action: photoCapture
            opacity: enabled ? 1 : 0.5
            activeFocusOnPress: true
            y: -3
            tooltip: "Click this icon/preview to capture photo"
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 : 0
                    color: "#222021"
                    border.color: "#ffffff"
                }
                label: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    fillMode: Image.PreserveAspectFit
                    source: "images/capture.png"
                }
            }
            Keys.onReturnPressed: {
                root.mouseClickCapture()
            }
        }
        Button {
            id: record
            opacity: 0.5
            enabled: false
            visible: false
            activeFocusOnPress : true
            y: -3
            action: videoRecord
            tooltip: "Click this icon/preview to start recording video"
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 : 0
                    color: "#222021"
                    border.color: "#ffffff"
                }
                label: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    fillMode: Image.PreserveAspectFit
                    source: "images/record.png"
                }
            }
            Keys.onReturnPressed: {
                root.videoRecordBegin()
            }
        }
        Button {
            id: record_stop
            opacity: 1
            visible: false
            tooltip: "Click this icon/preview to stop/save the recorded video"
            y: -3
            action: saveVideo
            activeFocusOnPress: true
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 : 0
                    color: "#222021"
                    border.color: "#ffffff"
                }
                label: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    fillMode: Image.PreserveAspectFit
                    source: "images/record_stop.png"
                }
            }
            Keys.onReturnPressed: {
                root.videoSaveVideo()
            }
        }
    }
    Text {
        id: vidRecTimer
        visible: false
        font.pixelSize: 16
        font.family: "Ubuntu"
        font.bold: true
        color: "green"
        smooth: true
        x: 18
        y: 66
        opacity: 1
    }
    Text {
        id: device_connected
        text: "Device Connected"
        font.pixelSize: 16
        font.family: "Ubuntu"
        color: "#ffffff"
        smooth: true
        x: 18
        y: vidRecTimer.visible ? 88 : 72
        opacity: 1
    }
    Component.onCompleted:
    {
        camera_btn.forceActiveFocus()
    }

    Connections
    {
        target: root
        onSidebarVisibleStatus:
        {
            captureRecord.visible = status;
        }
    }
    Connections
    {
        target: root
        onCaptureBtnEnable:
        {
            capture.enabled = enable
            capture.opacity = enable ? 1 : 0.5
        }
    }
    Connections
    {
        target: root
        onVideoRecordBtnEnable:
        {
            record.enabled = enable
            record.opacity = enable ? 1 : 0.5
        }
    }

    Connections
    {
        target: root
        onVideoRecordBtnVisible:
        {
            record.visible = visible
            record_stop.visible = !visible
        }
    }

    function captureBtnSelected() {
        if(!record_stop.visible) {
            video_selected.visible = false;
            camera_selected.visible = true;
            capture.visible = true
            record.visible = false
        }
    }

    function videoBtnSelected() {
        if(!record_stop.visible) {
            video_selected.visible = true;
            camera_selected.visible = false;
            capture.visible = false
            record.visible = true
        }
    }
    function updateHours() {
        hours = hours + 1
    }

    function updateMinutes() {
        if(minutes < 59) {
            minutes = minutes+1;
        }
        else {
            minutes = 0
            updateHours()
        }
    }

    function updateSeconds() {
        if(seconds < 59) {
            seconds = seconds+1;
        }
        else {
            seconds = 0
            updateMinutes()
        }
    }

    function updateTime() {
        updateSeconds()
        vidRecTimer.text = "Recording: "+ ((hours < 10) ? '0' : '') + hours+":"+((minutes < 10) ? '0' : '') + minutes +":"+((seconds < 10) ? '0' : '') + seconds
        vidRecTimer.visible = true
    }

    function videoTimerUpdate(timerState)
    {
        if(timerState)
        {
            seconds = 0
            minutes = 0
            hours = 0
            videoTimer.start();
        }
        else
        {
            videoTimer.stop();
            vidRecTimer.text = ""
            vidRecTimer.visible = false
        }
    }
}
