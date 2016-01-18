/*
 * videocapturefilter.qml -- display preview in screen
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
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import econ.camera.property 1.0
import econ.camera.stream 1.0
import econ.camera.uvcsettings 1.0
import "../JavaScriptFiles/tempValue.js" as JS

Rectangle {
    id: root
    signal detectCam()
    signal dispResCam()
    signal dispOutFormatCam()
    signal stopCamPreview()
    signal seeCamCu51Capture()
    property bool ret;
    property bool vidFormatChanged: false
    property string stateDisplay:"";
    property string indexvalue: ""
    property int controly : 0;
    property int controlId
    property int brightnessControlId
    property int contrastControlId
    property int saturationControlId
    property int panControlId
    property int tiltControlId
    property int zoomControlId
    property int hueControlId
    property int ledModeControlId
    property int whiteBalanceControl_auto_Id
    property int whiteBalanceControlId
    property int gammaControlId
    property int sharpnessControlId
    property int gainControlId
    property int powerLineComboControlId
    property int ledModeComboControlId
    property int exposureAutoControlId
    property int exposurecontrolId
    property int exposureAutoPriorityControlId
    property int backLightCompensationId
    property int rawBitsControlId
    property int ledFreqControlId
    property int focusLogitechControlId
    property int focusControlId
    property int focusControlAutoId
    property int disableVideoControlId
    property bool powerLineComboEnable
    property bool ledModeComboEnable
    property bool exposureComboEnable
    property bool keyEventFiltering
    property int seconds : 0


    property int controlType
    property string controlName
    property int controlMaxValue
    property int controlMinValue
    property int controlDefaultValue
    property int labelSpacing: 35
    property int sliderSpacing: 35
    property bool pressCheck: true
    property bool brightValueChangeProperty
    property bool contrastValueChangeProperty
    property bool saturationValueChangeProperty
    property bool panValueChangeProperty
    property bool tiltValueChangeProperty
    property bool zoomValueChangeProperty
    property bool wbValueChangeProperty
    property bool sharpValueChangeProperty
    property bool gainValueChangeProperty
    property bool hueValueChangeProperty
    property bool ledModeChangeProperty
    property bool exposureValueChangeProperty
    property bool bcklightValueChangeProperty
    property bool rawBitsValueChangeProperty
    property bool ledFreqValueChangeProperty
    property bool focusLogitechValueChangeProperty
    property bool focusValueChangeProperty
    property bool setResolutionEnable
    property bool m_Snap : true
    property bool stillPreview : false
    property bool frameRateBox
    property bool colorSpace
    property bool outputSizeBox
    property bool olderValue:false
    property bool webcamKeyAccept: true
    property bool stillColorSpace


    property int videoPinSize;
    property int videoPinSizeIndex;
    property int videoPinFormat;
    property int videoPinFormatIndex;
    property int videoPinFrameInterval;
    property int videoPinFrameIntervalIndex;
    property int hours
    property int minutes
    property int seconds2

    property variant videoPinRecordFormat
    property variant previewWidth
    property variant previewHeight
    property string statusText
    property string captureTime
    property string videoStoragePath : SystemVideoFolder
    property string videofileName
    property var menuitems:[]
    property variant aboutWindow
    property variant see3cam

    onSeeCamCu51Capture: {
        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
        vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
    }



    width:Screen.width
    height:Screen.height
    focus: true

    seconds2: 0
    minutes: 0
    hours: 0

    Action {
        id: cameratab
        onTriggered: {
            selectCameraSettings()
        }
    }
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
            mouseClickCapture()
        }
    }

    Action {
        id: videoRecord
        enabled: record.enabled ?  true : false
        onTriggered: {
            videoRecordBegin()
        }
    }

    Action {
        id: saveVideo
        onTriggered: {
            videoSaveVideo()
        }
    }

    Action {
        id: aboutAction
        onTriggered: {
            aboutWindow.show();
        }
    }

    Action {
        id: exitAction
        onTriggered: {
            exitDialog.visible = true
        }
    }

    Action {
        id: videoControl
        onTriggered: {
            videoControlFilter()
        }
    }

    Action {
        id: stillProperty
        onTriggered: {
            stillImageProperty()
        }
    }

    Action {
        id: videoCap
        onTriggered: {
            videoCapProperty()
            openVideoPinPage()
        }
    }

    Action {
        id: hardwareDefault
        onTriggered: {
            vidstreamproperty.cameraFilterControls(olderValue)
        }
    }

    Action {
        id: updatePreview
        onTriggered: {
            updateSettings()
        }
    }

    Action {
        id: restorePreview
        onTriggered: {
            cancelSettings()
        }
    }

    Action {
        id: videoLocation
        onTriggered: {
            fileDialogVideo.open()
        }
    }


    Timer {
        id: statusTimer
        interval: 3000
        onTriggered: {
            statusbartext.text = statusText;
            stop()
        }
    }
    Timer {
        id: captureTimer
        interval: 5000
        onTriggered: {
            captureTime = ""
            stop()
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
        if(seconds2 < 59) {
            seconds2 = seconds2+1;
        }
        else {
            seconds2 = 0
            updateMinutes()
        }
    }

    function updateTime() {
        updateSeconds()
        vidRecTimer.text = "Recording: "+ ((hours < 10) ? '0' : '') + hours+":"+((minutes < 10) ? '0' : '') + minutes +":"+((seconds2 < 10) ? '0' : '') + seconds2
        vidRecTimer.visible = true
    }

    Timer {
        id: videoTimer
        interval: 1000
        repeat: true
        property int timeText: 0
        onTriggered: {
            updateTime()
        }
    }

    MessageDialog {
        id: exitDialog
        title: "Exit Application"
        icon: StandardIcon.Question
        text: qsTr("Do you want to really close the application?")
        standardButtons:  StandardButton.No | StandardButton.Yes
        onYes: Qt.quit()
        onNo: close()
        Component.onCompleted:{
            close()
        }
    }

    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        onAccepted: {
            close()
        }
        Component.onCompleted:{
            close()
        }
    }

    MessageDialog {
        id: recordFailedDialog
        icon: StandardIcon.Critical
        onAccepted: {
            statusbartext.text = "Recording Failed..."
            videoCaptureProperty.enabled = true
            videoCaptureProperty.opacity = 1
            stillproperties.enabled = true
            stillproperties.opacity = 1
            device_box.enabled = true
            vidstreamproperty.enabled = true
            device_box.opacity = 1
            record.visible = true
            record_stop.visible = false
            uvc_settings.enabled = true
            uvc_settings.opacity = 1
            close()
            statusTimer.start()
        }
        Component.onCompleted:{
            close()
        }
    }

    Image {
        id: layer_0
        source: "images/layer_0.png"
        x: sideBarItems.visible ? 268 : 0
        y: 0
        opacity: 1
        width: sideBarItems.visible ? parent.width - 268 : parent.width
        height: parent.height - statusbar.height
    }

    ScrollView {
        id: previewer
        anchors.centerIn: layer_0
        width: vidstreamproperty.width < (sideBarItems.visible ? parent.width - 268 : parent.width) ? vidstreamproperty.width + 20 : sideBarItems.visible ? parent.width - 268 : parent.width
        height: vidstreamproperty.height < layer_0.height ? (vidstreamproperty.height) : (parent.height - statusbar.height)
        style: ScrollViewStyle {
            scrollToClickedPosition: true
            scrollBarBackground: Image {
                source: styleData.horizontal? "images/Horizontal_Scroll_base.png" : "images/Vertical_Scroll_base.png"
            }
            incrementControl: Image {
                source: styleData.horizontal? "images/Horizontal_right_arrow.png" : "images/Vertical_down_arrow.png"
            }
            decrementControl: Image {
                source: styleData.horizontal? "images/Horizontal_left_arrow.png" : "images/Vertical_top_arrow.png"
            }
        }
        Videostreaming {
            id: vidstreamproperty
            focus: true
//            onEnableCaptureAndRecord:{
//                vidstreamproperty.enabled = true
//                capture.enabled = true
//                capture.opacity = 1
//                record.enabled = true
//                record.opacity = 1
//                webcamKeyAccept = true
//                keyEventFiltering = false
//            }

//            onTitleTextChanged: {
//                messageDialog.title = _title.toString()
//                messageDialog.text = _text.toString()
//                messageDialog.visible = true
//            }
            onTitleTextChanged:{
                vidstreamproperty.enabled = true
                capture.enabled = true
                capture.opacity = 1
                record.enabled = true
                record.opacity = 1
                webcamKeyAccept = true
                keyEventFiltering = false
                messageDialog.title = _title.toString()
                messageDialog.text = _text.toString()
                messageDialog.visible = true
            }

            onNewControlAdded: {
                setControls(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,ctrlDefaultValue)
            }

            onDeviceUnplugged: {
                capture.enabled = false
                capture.opacity = 0.5
                record.enabled = false
                keyEventFiltering = true
                statusbartext.text = ""
                messageDialog.title = _title.toString()
                messageDialog.text = _text.toString()
                messageDialog.open()
                if(device_box.currentText == "e-con's 1MP Monochrome\n Camera")
                {
                    JS.enableMasterMode_10cugM()
                } else if(device_box.currentText == "e-con's 1MP Bayer RGB \nCamera") {
                    JS.enableMasterMode_10cugB()
                } else if(device_box.currentText == "See3CAM_11CUG"){
                    JS.enableMasterMode_11cug()
                } else if(device_box.currentText == "See3CAM_CU51"){
                    JS.enableMasterMode_cu51()
                }
                device_box.oldIndex = 0
                device_box.currentIndex = 0
                setOpacityFalse()
                if(record_stop.visible) {
                    statusText = statusbartext.text
                    statusbartext.text = "Saving..."
                    vidstreamproperty.recordStop()
                    videoTimer.stop()
                    vidRecTimer.text = ""
                    vidRecTimer.visible = false
                    messageDialog.title = qsTr("Saved")
                    messageDialog.text = qsTr("Video saved in the location:"+videofileName)
                    messageDialog.open()
                    videoCaptureProperty.enabled = true
                    videoCaptureProperty.opacity = 1
                    stillproperties.enabled = true
                    stillproperties.opacity = 1
                    device_box.enabled = true
                    vidstreamproperty.enabled = true
                    device_box.opacity = 1
                    record.visible = true
                    record_stop.visible = false
                    uvc_settings.enabled = true
                    uvc_settings.opacity = 1
                    statusTimer.start()
                }
            }

            onLogDebugHandle: {
                camproperty.logDebugWriter(_text.toString())
            }

            onLogCriticalHandle: {
                camproperty.logCriticalWriter(_text.toString())
            }

            onAverageFPS: {
                average_frame_value.text = fps
                if(device_box.opacity === 0.5)
                    statusbartext.text = "Recording..." + " " + "Current FPS: " + fps + " Preview Resolution: "+ vidstreamproperty.width +"x"+vidstreamproperty.height + " " + "Color Format: " + color_comp_box_VideoPin.currentText.toString()
                else {
                    statusText = "Current FPS: " + fps + " Preview Resolution: "+ vidstreamproperty.width +"x"+vidstreamproperty.height + " " + captureTime + " " + "Color Format: " + color_comp_box_VideoPin.currentText.toString()
                    statusbartext.text = statusText
                }
            }

            onDefaultFrameSize: {
                if(m_Snap) {
                    output_value.currentIndex = outputIndexValue                    
                }
                output_size_box_Video.currentIndex = outputIndexValue
                vidstreamproperty.width = defaultWidth
                vidstreamproperty.height = defaultHeight
            }

            onDefaultStillFrameSize: {                
                output_value.currentIndex = outputIndexValue                
            }

            onDefaultOutputFormat: {                                
                if(m_Snap){
                    color_comp_box.currentIndex = formatIndexValue
                }

                if(!stillPreview){
                    color_comp_box_VideoPin.currentIndex = formatIndexValue                    
                }                
            }

            onDefaultFrameInterval:{
                videoPinFrameInterval = frameInterval
                frame_rate_box.currentIndex = videoPinFrameInterval
            }

            onRcdStop: {
                recordFailedDialog.title = "Failed"
                recordFailedDialog.text = recordFail
                recordFailedDialog.open()
                vidstreamproperty.recordStop()
                videoTimer.stop()
                vidRecTimer.text = ""
                vidRecTimer.visible = false
            }
            onCaptureSaveTime: {
                captureTime =  saveTime
                captureTimer.start()
            }

            onVideoRecord: {
                videofileName = fileName
            }

            MouseArea {
                anchors.fill: parent
                onReleased:
                {
                    if(capture.visible)
                        mouseClickCapture()
                    else if(record.visible){
                        videoRecordBegin()
                    } else if(record_stop.visible){
                        videoSaveVideo()
                    }
                }
            }
        }
    }

    Image {
        id: open_sideBar
        visible: false
        source: "images/open_tab.png"
        anchors.bottom: layer_0.bottom
        anchors.bottomMargin: 50
        anchors.left: layer_0.left
        y: layer_0.height/2 + 20
        opacity: 1
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                open_sideBar.opacity = 1
            }
            onExited: {
                open_sideBar.opacity = 1
            }

            onReleased: {
                sideBarItems.visible = true
                open_sideBar.visible = false
            }
        }
    }

    //    ScrollView {
    //        id: test
    //        x:0
    //        y:0
    //        width: 267
    //        height: parent.height + 30
    //        style: ScrollViewStyle {
    //            scrollToClickedPosition: true
    //            scrollBarBackground: Image {
    //                id: scrollStyle3
    //                source: styleData.horizontal? "images/Horizontal_Scroll_base.png" : "images/Vertical_Scroll_base.png"
    //            }
    //            incrementControl: Image {
    //                id: increment3
    //                source: styleData.horizontal? "images/Horizontal_right_arrow.png" : "images/Vertical_down_arrow.png"
    //            }
    //            decrementControl: Image {
    //                id: decrement3
    //                source: styleData.horizontal? "images/Horizontal_left_arrow.png" : "images/Vertical_top_arrow.png"
    //            }
    //        }
    Item {
        id: sideBarItems
        Image {
            id: side_bar_bg
            source: "images/side_bar_bg.png"
            x: -3
            y: -3
            height: root.height+5
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

        Image {
            id: camera_box
            source: "images/camera_box.png"
            x: 18
            y: 18
            opacity: 1
        }
        Button {
            id: about
            x: 20
            y: statusbar.y - 30
            opacity: 1
            action: aboutAction
            activeFocusOnPress : true
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 :0
                    color: "#222021"
                    border.color: control.activeFocus ? "#ffffff" : "#222021"
                }
                label: Image {
                    source: "images/about.png"
                }
            }
            Keys.onReturnPressed:  {
                aboutWindow.show();
            }
        }

        Button {
            id: exit
            x: 192
            y: statusbar.y - 30
            opacity: 1
            action: exitAction
            activeFocusOnPress : true
            style: ButtonStyle {
                background: Rectangle {
                    border.width: control.activeFocus ? 1 :0
                    color: "#222021"
                    border.color: control.activeFocus ? "#ffffff" : "#222021"
                }
                label: Image {
                    source: "images/exit.png"
                }
            }
            Keys.onReturnPressed: {
                exitDialog.visible = true
            }
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
                    mouseClickCapture()
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
                    videoRecordBegin()
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
                    videoSaveVideo()
                }
            }
        }

        ComboBox {
            currentIndex: 0
            property int oldIndex: currentIndex
            property int indexZero: 1
            id: device_box
            x: 18
            y: 110
            opacity: 1
            textRole: "display"
            model: camModels
            activeFocusOnPress : true

            style: ComboBoxStyle {
                background: Image {
                    id: deviceBox
                    source: "images/device_box.png"
                    Rectangle {
                        width: deviceBox.sourceSize.width -28
                        height: deviceBox.sourceSize.height
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
            MouseArea{
                anchors.fill: parent
                onPressed: {
                    if(pressed) {
                        camproperty.checkforDevice()
                    }
                    mouse.accepted = false
                }
                onWheel: {
                }
            }
            onCurrentIndexChanged: {
                if(currentIndex.toString() != "-1" && currentIndex.toString() != "0") {
                    if(oldIndex!=currentIndex) {
                        oldIndex = currentIndex
                        m_Snap = true
                        outputSizeBox = false
                        stillColorSpace = false
                        frameRateBox = false
                        colorSpace = false
                        capture.enabled = true
                        capture.opacity = 1
                        record.enabled = true
                        record.opacity = 1
                        keyEventFiltering = false
                        vidstreamproperty.enabled = true                                                
                        webcamKeyAccept = true
                        vidstreamproperty.stopCapture()
                        vidstreamproperty.closeDevice()
                        selectCameraSettings()
                        camproperty.setCurrentDevice(currentIndex.toString(),currentText.toString())
                        vidstreamproperty.setDevice("/dev/video")
                        vidstreamproperty.displayOutputFormat()
                        vidstreamproperty.displayStillResolution()
                        vidstreamproperty.displayVideoResolution()
                        if(device_box.currentText == "CX3-UVC"){
                            vidstreamproperty.cameraFilterControls()
                        }
                        updateFPS(color_comp_box.currentText.toString(), output_value.currentText.toString())
                        brightValueChangeProperty = false
                        contrastValueChangeProperty = false
                        saturationValueChangeProperty = false
                        panValueChangeProperty = false
                        tiltValueChangeProperty = false
                        zoomValueChangeProperty = false
                        wbValueChangeProperty = false
                        sharpValueChangeProperty = false
                        gainValueChangeProperty = false
                        hueValueChangeProperty = false
                        ledModeChangeProperty = false
                        exposureValueChangeProperty = false
                        bcklightValueChangeProperty = false
                        rawBitsValueChangeProperty = false
                        ledFreqValueChangeProperty = false
                        focusLogitechValueChangeProperty = false
                        focusValueChangeProperty = false
                        powerLineComboEnable = false
                        ledModeComboEnable = false
                        exposureComboEnable =  false
                        setOpacityFalse()
                        vidstreamproperty.startAgain()                        
                        vidstreamproperty.width = output_value.currentText.toString().split("x")[0].toString()
                        vidstreamproperty.height = output_value.currentText.toString().split("x")[1].toString()
                        vidstreamproperty.lastPreviewResolution(output_value.currentText.toString(),color_comp_box.currentIndex.toString())
                        JS.stillCaptureFormat = color_comp_box.currentIndex.toString()
                        JS.stillCaptureResolution = output_value.currentText.toString()
                        JS.videoCaptureFormat = JS.stillCaptureFormat
                        JS.videoCaptureResolution = JS.stillCaptureResolution                        
                        vidstreamproperty.masterModeEnabled()
                    }
                }
                else {
                    if (currentIndex == 0) {
                        currentIndex = oldIndex
                    }
                }
            }
            Component.onCompleted: {
                camproperty.checkforDevice()
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

        Button {
            id: camera_settings
            focus: false
            smooth: true
            x: cameraColumnLayout.visible ? 0 : 7
            y: cameraColumnLayout.visible ? 149 : 153
            opacity: 1
            tooltip: "Camera Settings - This settings will have the v4l2 controls for the camera"
            style: tabButtonStyle
            action: cameratab
            activeFocusOnPress: true
            onFocusChanged: {
                if(activeFocus){
                    selectCameraSettings()
                }
            }
        }

        ColumnLayout {
            id: cameraColumnLayout
            Item {
                id: videoFilter
                Button {
                    id: video_capture_filter
                    y: 206
                    opacity: 1
                    tooltip: "Image Quality Settings - \nAllows User to adjust the control settings of the preview"
                    action: videoControl
                    activeFocusOnPress : true
                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 265
                            implicitHeight: 15
                            border.width: control.activeFocus ? 1 : 0
                            color: control.activeFocus ? "#df643e" : video_capture_filter_Child.visible ? "#df643e" : "#222021"  ///*#df643e"//*/
                            border.color: control.activeFocus ? "#df643e" : "#222021"
                        }
                        label: Image {
                            horizontalAlignment: Image.AlignLeft
                            fillMode: Image.PreserveAspectFit
                            source: "images/image_qualitysettings.png"
                        }
                    }
                    ScrollView {
                        id: video_capture_filter_Child
                        x:10
                        y: 35
                        width: 257
                        height: 160
                        visible: false
                        style: ScrollViewStyle {
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
                        Item {
                            height: focus_value.y + 85
                            GridLayout {
                                id: grid
                                columns: 3
                                rowSpacing: 15
                                columnSpacing: 8
                                Text {
                                    id: brightness
                                    text: "Brightness"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: brightness_Slider
                                    opacity: enabled ? 1 : 0.1
                                    width: 110
                                    stepSize: 1
                                    style:econSliderStyle
                                    onValueChanged:  {
                                        if(brightValueChangeProperty) {
                                            camproperty.logDebugWriter("Brightness changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(brightnessControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: brightness_value                                    
                                    text: brightness_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: brightness_Slider.minimumValue; top: brightness_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            brightness_Slider.value = brightness_value.text
                                    }
                                }
                                Text {
                                    id: contrast
                                    text: "Contrast"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: contrast_Slider
                                    width: 110
                                    stepSize: 1
                                    style:econSliderStyle
                                    opacity: enabled ? 1 : 0.1
                                    onValueChanged: {
                                        if(contrastValueChangeProperty) {
                                            camproperty.logDebugWriter("Contrast changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(contrastControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: contrast_value
                                    text: contrast_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: contrast_Slider.minimumValue; top: contrast_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            contrast_Slider.value = contrast_value.text
                                    }
                                }
                                Text {
                                    id: saturation
                                    text: "Saturation"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: saturation_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(saturationValueChangeProperty) {
                                            camproperty.logDebugWriter("Saturation changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(saturationControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: saturation_value
                                    text: saturation_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: saturation_Slider.minimumValue; top: saturation_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            saturation_Slider.value = saturation_value.text
                                    }
                                }
                                Text {
                                    id: pan
                                    text: "Pan\n(Absolute)"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: pan_Slider
                                    opacity: enabled ? 1 : 0.1
                                    width: 110
                                    stepSize: 3600
                                    style:econSliderStyle
                                    onValueChanged:  {
                                        if(panValueChangeProperty) {
                                            camproperty.logDebugWriter("Pan changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(panControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: pan_value
                                    text: pan_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: pan_Slider.minimumValue; top: pan_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            pan_Slider.value = pan_value.text
                                    }
                                }
                                Text {
                                    id: tilt
                                    text: "Tilt\n(Absolute)"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: tilt_Slider
                                    opacity: enabled ? 1 : 0.1
                                    width: 110
                                    stepSize: 3600
                                    style:econSliderStyle
                                    onValueChanged:  {
                                        if(tiltValueChangeProperty) {
                                            camproperty.logDebugWriter("Tilt changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(tiltControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: tilt_value
                                    text: tilt_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: tilt_Slider.minimumValue; top: tilt_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            tilt_Slider.value = tilt_value.text
                                    }
                                }
                                Text {
                                    id: zoom
                                    text: "Zoom\n(absolute)"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: zoom_Slider
                                    opacity: enabled ? 1 : 0.1
                                    width: 110
                                    stepSize: 1
                                    style:econSliderStyle
                                    onValueChanged:  {
                                        if(zoomValueChangeProperty) {
                                            camproperty.logDebugWriter("zoom changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(zoomControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: zoom_value
                                    text: zoom_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: zoom_Slider.minimumValue; top: zoom_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            zoom_Slider.value = zoom_value.text
                                    }
                                }

                                Column {
                                    spacing: 4
                                    Text {
                                        id: white_balance
                                        text: "White\nBalance"
                                        font.pixelSize: 12
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 0.1
                                    }
                                    CheckBox {
                                        id: autoSelect_wb
                                        opacity: 0.1
                                        activeFocusOnPress: true
                                        style: CheckBoxStyle {
                                            label: Text {
                                                id: autowb
                                                text: "Auto"
                                                font.pixelSize: 10
                                                font.family: "SegoeUI-Light"
                                                color: "#ffffff"
                                                smooth: true
                                                opacity: 1
                                            }
                                        }
                                        onCheckedChanged: {
                                            if(checked) {
                                                camproperty.logDebugWriter("White Balance set to Auto Mode")
                                                vidstreamproperty.changeSettings(whiteBalanceControl_auto_Id,1)
                                                white_balance_Slider.opacity = 0.1
                                                white_balance_Slider.enabled = false
                                            } else {
                                                camproperty.logDebugWriter("White Balance set to Manual Mode")                                                
                                                vidstreamproperty.changeSettings(whiteBalanceControl_auto_Id,0)
                                                white_balance_Slider.opacity = 1
                                                white_balance_Slider.enabled = true
                                            }
                                        }
                                    }
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: white_balance_Slider
                                    opacity: enabled ? 1 : 0.1
                                    width: 110
                                    stepSize: 1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(wbValueChangeProperty) {
                                            if(!autoSelect_wb.checked) {
                                                camproperty.logDebugWriter("White Balance changed to: "+ value.toString())
                                                vidstreamproperty.changeSettings(whiteBalanceControlId,value.toString())
                                            } else {
                                                white_balance_Slider.enabled = false
                                            }
                                        }
                                        wbValueChangeProperty = true
                                    }
                                }
                                TextField {
                                    id: wb_value
                                    text: white_balance_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: white_balance_Slider.minimumValue; top: white_balance_Slider.maximumValue;}
                                    opacity: white_balance_Slider.enabled ? 1: 0
                                    //opacity: autoSelect_wb.checked ? 0 : 1
                                    enabled: autoSelect_wb.checked ? false : true
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            white_balance_Slider.value = wb_value.text
                                    }
                                }
                                Text {
                                    id: gamma
                                    text: "Gamma"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    property bool gammaValueChangeProperty
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: gamma_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(gammaValueChangeProperty) {
                                            camproperty.logDebugWriter("Gamma settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(gammaControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: gamma_value
                                    text: gamma_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: gamma_Slider.minimumValue; top: gamma_Slider.maximumValue;}
                                    smooth: true
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            gamma_Slider.value = gamma_value.text
                                    }
                                }
                                Text {
                                    id: sharpness
                                    text: "Sharpness"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: sharpness_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(sharpValueChangeProperty) {
                                            camproperty.logDebugWriter("Sharpness settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(sharpnessControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: sharpness_value
                                    text: sharpness_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: sharpness_Slider.minimumValue; top: sharpness_Slider.maximumValue;}
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            sharpness_Slider.value = sharpness_value.text
                                    }
                                }
                                Text {
                                    id: gain
                                    text: "Gain"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity:0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: gain_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(gainValueChangeProperty) {
                                            camproperty.logDebugWriter("Gain settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(gainControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: gain_value
                                    text: gain_Slider.value
                                    validator: IntValidator {bottom: gain_Slider.minimumValue; top: gain_Slider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            gain_Slider.value = gain_value.text
                                    }
                                }
                                Text {
                                    id: hue
                                    text: "Hue"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: hue_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(hueValueChangeProperty) {
                                            camproperty.logDebugWriter("Hue settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(hueControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: hue_value
                                    text: hue_Slider.value
                                    validator: IntValidator {bottom: hue_Slider.minimumValue; top: hue_Slider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    horizontalAlignment: TextInput.AlignHCenter
                                    smooth: true
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            hue_Slider.value = hue_value.text
                                    }
                                }
//                                Text {
//                                    id: ledModeText
//                                    text: "LED1 Mode"
//                                    font.pixelSize: 12
//                                    font.family: "Ubuntu"
//                                    color: "#ffffff"
//                                    smooth: true
//                                    opacity: 0.1
//                                }
//                                Slider {
//                                    activeFocusOnPress: true
//                                    updateValueWhileDragging: false
//                                    id: ledModeSlider
//                                    width: 110
//                                    stepSize: 1
//                                    opacity: enabled ? 1 : 0.1
//                                    style:econSliderStyle
//                                    onValueChanged: {
//                                        if(ledModeChangeProperty) {
//                                            camproperty.logDebugWriter("led mode changed to: "+ value.toString())
//                                            vidstreamproperty.changeSettings(ledModeControlId,value.toString())
//                                        }
//                                    }
//                                }
//                                TextField {
//                                    id: ledMode_value
//                                    text: ledModeSlider.value
//                                    validator: IntValidator {bottom: ledModeSlider.minimumValue; top: ledModeSlider.maximumValue;}
//                                    font.pixelSize: 10
//                                    font.family: "Ubuntu"
//                                    horizontalAlignment: TextInput.AlignHCenter
//                                    smooth: true
//                                    opacity: 0
//                                    style: econTextFieldStyle
//                                    onTextChanged: {
//                                        if(text != "")
//                                            ledModeSlider.value = ledMode_value.text
//                                    }
//                                }

//                                Image {
//                                    source: "images/pline_dropdown.png"
//                                    opacity: 0
//                                    //Just for layout purpose
//                                }
                                Text {
                                    id: powerLine
                                    text: "PowerLine\nFrequency"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                ComboBox
                                {
                                    id: powerLineCombo
                                    opacity: 0
                                    model: menuitems
                                    activeFocusOnPress: true
                                    style: ComboBoxStyle {
                                        background: Image {
                                            id: deviceBox_powerLine
                                            source: "images/plinefreq_box.png"                                            
                                            Rectangle {
                                                width: deviceBox_powerLine.sourceSize.width - 20
                                                height: deviceBox_powerLine.sourceSize.height + 3
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
                                    onCurrentIndexChanged: {
                                        if(powerLineComboEnable) {
                                            vidstreamproperty.selectMenuIndex(powerLineComboControlId,currentIndex)
                                        }
                                    }
                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }
                                Text {
                                    id: exposure_auto
                                    text: "Exposure,\nAuto"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                ComboBox
                                {
                                    id: exposureCombo
                                    model: menuitems
                                    opacity: 0
                                    activeFocusOnPress: true
                                    style: ComboBoxStyle {
                                        background: Image {
                                            id: deviceBox2
                                            source: "images/plinefreq_box.png"
                                            Rectangle {
                                                width: deviceBox2.sourceSize.width - 20
                                                height: deviceBox2.sourceSize.height + 3
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
                                    onCurrentIndexChanged: {
                                        if(exposureComboEnable) {
                                            vidstreamproperty.selectMenuIndex(exposureAutoControlId,currentIndex)
                                            if(currentText.toString() == "Manual Mode") {
                                                JS.autoExposureSelected = false
                                                exposure_absolute.opacity = 1
                                                exposure_Slider.opacity = 1
                                                exposure_Slider.enabled = true
                                                exposure_value.opacity = 1
                                                exposure_value.enabled = true
                                            } else {
                                                JS.autoExposureSelected = true
                                                exposure_absolute.opacity = 0.1
                                                exposure_Slider.opacity = 0.1
                                                exposure_Slider.enabled = false
                                                exposure_value.opacity = 0
                                                exposure_value.enabled = false
                                            }
                                        }
                                    }
                                }
                                Image {
                                    //For layout purpose
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                }

                                Text {
                                    id: exposure_absolute
                                    text: "Exposure\n(Absolute)"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity:  0
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: exposure_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if((exposureCombo.currentText == "Manual Mode") || (device_box.currentText == "e-con's CX3 RDK with O\nV5680") || (device_box.currentText == "e-con's CX3 RDK with M\nT9P031") || (device_box.currentText == "See3CAM_CU40")) {
                                            camproperty.logDebugWriter("Exposure Control settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(exposurecontrolId,value.toString())
                                        }

                                    }
                                }
                                TextField {
                                    id: exposure_value
                                    text: exposure_Slider.value
                                    validator: IntValidator {bottom: exposure_Slider.minimumValue; top: exposure_Slider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    opacity: 0
                                    style:econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            exposure_Slider.value = exposure_value.text
                                    }
                                }
                                Column {
                                    spacing : 4
                                    Text {
                                        id: exposureAutoPriority
                                        text: "Exposure,\nAuto\nPriority"
                                        font.pixelSize: 12
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 0.1
                                    }
                                    CheckBox {
                                        id: exposureAutoPriorityCheck
                                        onCheckedChanged: {
                                            if(checked) {                                                
                                                camproperty.logDebugWriter("enable exposure auto priority")
                                                vidstreamproperty.changeSettings(exposureAutoPriorityControlId,1)
                                            } else {                                                
                                                camproperty.logDebugWriter("disable exposure auto priority")
                                                vidstreamproperty.changeSettings(exposureAutoPriorityControlId,0)
                                            }
                                        }
                                    }
                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }
                                Text {
                                    id: backLightCompensation
                                    text: "BackLight\nCompen\n-sation"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: backLight_Slider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(bcklightValueChangeProperty) {
                                            camproperty.logDebugWriter("Backlight compensation settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(backLightCompensationId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: backLight_value
                                    text: backLight_Slider.value
                                    validator: IntValidator {bottom: backLight_Slider.minimumValue; top: backLight_Slider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            backLight_Slider.value = backLight_value.text
                                    }
                                }
                                Text {
                                    id: rawBits
                                    text: "Raw bits\nper pixel"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: rawBitsSlider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(rawBitsValueChangeProperty) {
                                            camproperty.logDebugWriter("raw bits per pixel settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(rawBitsControlId,value.toString())                                            
                                        }
                                    }
                                }
                                TextField {
                                    id: rawBits_value
                                    text: rawBitsSlider.value
                                    validator: IntValidator {bottom: rawBitsSlider.minimumValue; top: rawBitsSlider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            rawBitsSlider.value = rawBits_value.text
                                    }
                                }
                                Text {
                                    id: ledModeText
                                    text: "LED1 Mode"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                ComboBox
                                {
                                    id: ledModeCombo
                                    opacity: 0
                                    activeFocusOnPress: true
                                    model: ListModel {
                                        id: cbItemsledModes
                                        ListElement { text: "Off"  }
                                        ListElement { text: "On"  }
                                        ListElement { text: "Blinking"  }
                                        ListElement { text: "Auto"  }
                                    }
                                    style: ComboBoxStyle {
                                        background: Image {
                                            id: ledModeComboImage
                                            source: "images/plinefreq_box.png"
                                            Rectangle {
                                                width: ledModeComboImage.sourceSize.width - 20
                                                height: ledModeComboImage.sourceSize.height + 3
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
                                    onCurrentIndexChanged: {
                                        if(ledModeComboEnable) {                                            
                                            vidstreamproperty.changeSettings(ledModeComboControlId,currentIndex.toString())
                                        }
                                    }
                                }
//                                Slider {
//                                    activeFocusOnPress: true
//                                    updateValueWhileDragging: false
//                                    id: ledModeSlider
//                                    width: 110
//                                    stepSize: 1
//                                    opacity: enabled ? 1 : 0.1
//                                    style:econSliderStyle
//                                    onValueChanged: {
//                                        if(ledModeChangeProperty) {
//                                            camproperty.logDebugWriter("led mode changed to: "+ value.toString())
//                                            vidstreamproperty.changeSettings(ledModeControlId,value.toString())
//                                        }
//                                    }
//                                }
//                                TextField {
//                                    id: ledMode_value
//                                    text: ledModeSlider.value
//                                    validator: IntValidator {bottom: ledModeSlider.minimumValue; top: ledModeSlider.maximumValue;}
//                                    font.pixelSize: 10
//                                    font.family: "Ubuntu"
//                                    horizontalAlignment: TextInput.AlignHCenter
//                                    smooth: true
//                                    opacity: 0
//                                    style: econTextFieldStyle
//                                    onTextChanged: {
//                                        if(text != "")
//                                            ledModeSlider.value = ledMode_value.text
//                                    }
//                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }

                                Text {
                                    id: ledFrequency
                                    text: "LED1\nFrequency"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: ledFreqSlider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(ledFreqValueChangeProperty) {
                                            camproperty.logDebugWriter("led frequency settings changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(ledFreqControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: ledFreq_value
                                    text: ledFreqSlider.value
                                    validator: IntValidator {bottom: ledFreqSlider.minimumValue; top: ledFreqSlider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            ledFreqSlider.value = ledFreq_value.text
                                    }
                                }
                                Text {
                                    id: focusLogitech
                                    text: "Focus"
                                    font.pixelSize: 12
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 0.1
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: focusLogitechSlider
                                    width: 110
                                    stepSize: 1
                                    opacity: enabled ? 1 : 0.1
                                    style:econSliderStyle
                                    onValueChanged: {
                                        if(focusLogitechValueChangeProperty) {
                                            camproperty.logDebugWriter("focus settings for logitech camera changed to: "+ value.toString())
                                            vidstreamproperty.changeSettings(focusLogitechControlId,value.toString())
                                        }
                                    }
                                }
                                TextField {
                                    id: focusLogitech_value
                                    text: focusLogitechSlider.value
                                    validator: IntValidator {bottom: focusLogitechSlider.minimumValue; top: focusLogitechSlider.maximumValue;}
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    opacity: 0
                                    style: econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            focusLogitechSlider.value = focusLogitech_value.text
                                    }
                                }
                                Column {
                                    spacing : 4
                                    Text {
                                        id: disableVideoProcess
                                        text: "Disable\nvideo\nProcessing"
                                        font.pixelSize: 12
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 0.1
                                    }
                                    CheckBox {
                                        id: disableVideoProcessCheck
                                        onCheckedChanged: {
                                            if(checked) {                                                
                                                camproperty.logDebugWriter("Disable video processing")
                                                vidstreamproperty.changeSettings(disableVideoControlId,1)
                                            } else {                                                
                                                camproperty.logDebugWriter("Enable video processing")
                                                vidstreamproperty.changeSettings(disableVideoControlId,0)
                                            }                                            
                                        }
                                    }
                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }

                                Column {
                                    spacing: 4
                                    Text {
                                        id: focusauto
                                        text: "Focus\nAbsolute"
                                        font.pixelSize: 12
                                        font.family: "Ubuntu"
                                        color: "#ffffff"
                                        smooth: true
                                        opacity: 0.1
                                    }
                                    CheckBox {
                                        id: autoSelect_focus                                        
                                        style: CheckBoxStyle {
                                            label: Text {
                                                id: autofocus
                                                text: "Auto"
                                                font.pixelSize: 10
                                                font.family: "SegoeUI-Light"
                                                color: "#ffffff"
                                                smooth: true
                                                opacity: 1
                                            }
                                        }
                                        onCheckedChanged: {
                                            if(checked) {
                                                JS.autoFocusChecked = true
                                                camproperty.logDebugWriter("Focus control set in Auto Mode")
                                                vidstreamproperty.changeSettings(focusControlAutoId,1)
                                                focus_Slider.opacity = 0.1
                                                focus_Slider.enabled = false
                                                focus_value.opacity = 0
                                                focus_value.enabled = false
                                            } else {
                                                JS.autoFocusChecked = false
                                                camproperty.logDebugWriter("Focus control set in Manual Mode")
                                                vidstreamproperty.changeSettings(focusControlAutoId,0)
                                                focus_Slider.opacity = 1
                                                focus_Slider.enabled = true
                                                focus_value.opacity = 1
                                                focus_value.enabled = true
                                            }
                                        }
                                    }
                                }
                                Slider {
                                    activeFocusOnPress: true
                                    updateValueWhileDragging: false
                                    id: focus_Slider
                                    width: 110
                                    stepSize: 1
                                    style:econSliderStyle
                                    opacity: enabled ? 1 : 0.1
                                    onValueChanged: {
                                        if(focusValueChangeProperty) {
                                            if(!autoSelect_focus.checked || device_box.currentText == "e-con's CX3 RDK with O\nV5680") {
                                                camproperty.logDebugWriter("Focus control settings changed to: "+ value.toString())
                                                vidstreamproperty.changeSettings(focusControlId,value.toString())
                                            } else {
                                                focus_Slider.enabled = false
                                                focus_Slider.opacity = 0.1
                                                focus_value.enabled = false
                                                focus_value.opacity = 0
                                            }
                                        }
                                        focusValueChangeProperty = true
                                    }
                                }
                                TextField {
                                    id: focus_value
                                    text: focus_Slider.value
                                    font.pixelSize: 10
                                    font.family: "Ubuntu"
                                    smooth: true
                                    horizontalAlignment: TextInput.AlignHCenter
                                    validator: IntValidator {bottom: focus_Slider.minimumValue; top: focus_Slider.maximumValue;}
                                    opacity: 0
                                    style:econTextFieldStyle
                                    onTextChanged: {
                                        if(text != "")
                                            focus_Slider.value = focus_value.text
                                    }
                                }


                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }

                                Button {
                                    opacity: 1
                                    action: hardwareDefault
                                    activeFocusOnPress : true
                                    style: ButtonStyle {
                                        background: Rectangle {
                                            border.width: control.activeFocus ? 3 :0
                                            color: "#222021"
                                            border.color: control.activeFocus ? "#ffffff" : "#222021"
                                            radius: 5
                                        }
                                        label: Image {
                                            horizontalAlignment: Image.AlignHCenter
                                            verticalAlignment: Image.AlignVCenter
                                            fillMode: Image.PreserveAspectFit
                                            source: "images/hardware_default_selected.png"
                                            sourceSize.width: 110
                                        }
                                    }
                                }
                                Image {
                                    source: "images/pline_dropdown.png"
                                    opacity: 0
                                    //Just for layout purpose
                                }
                            }
                        }

                    }
                    onFocusChanged: {
                        video_capture_filter_Child.visible = false
                    }
                    Keys.onSpacePressed: {

                    }
                    Keys.onReturnPressed: {
                        videoControlFilter()
                    }                    
                }
            }

            Item {
                id: stillproperties //Still Capture settings
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
                    y: video_capture_filter_Child.visible ? 400 : 240
                    opacity: 1
                    Item {
                        id: stillchildProperty
                        visible: false
                        x: 10
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
                                JS.stillCaptureFormat = color_comp_box.currentIndex.toString()
                                if(JS.triggerMode_11cug === 1 || JS.triggerMode_B === 1 || JS.triggerMode_M === 1 || JS.triggerMode_cu51 === 1)
                                    triggerModeCapture()

                                if(stillColorSpace) {
                                      updateStillPreview(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
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
                                if(JS.triggerMode_11cug === 1 || JS.triggerMode_B === 1 || JS.triggerMode_M === 1 || JS.triggerMode_cu51 === 1)
                                    triggerModeCapture()
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

                            model: ListModel {
                                id: cbItemsImgFormat
                                ListElement { text: "jpg"  }
                                ListElement { text: "bmp"  }
                                ListElement { text: "raw"  }
                                ListElement { text: "png"  }
                            }
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
            }

            Item {
                id: videoCaptureProperty
                Button {
                    id: video_Capture
                    y: stillchildProperty.visible ? image_format.y + 300 : still_properties.y + 35
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
                                            videoPinFrameInterval = currentIndex
                                            updateScenePreview(output_size_box_Video.currentText.toString(), color_comp_box_VideoPin.currentIndex.toString(),currentIndex)
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
                                        if(colorSpace) {                                            
                                            vidFormatChanged = true
                                            JS.videoCaptureFormat = color_comp_box_VideoPin.currentIndex.toString()                                            
                                            updateScenePreview(vidstreamproperty.width.toString() +"x"+vidstreamproperty.height.toString(), color_comp_box_VideoPin.currentIndex.toString(),frame_rate_box.currentIndex)
                                            vidstreamproperty.displayVideoResolution()
                                            updateFPS(currentText.toString(), output_size_box_Video.currentText.toString())
                                            vidFormatChanged = false
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
                                        JS.videoCaptureResolution = output_size_box_Video.currentText.toString();
                                        if(outputSizeBox) {
                                            updateFPS(color_comp_box_VideoPin.currentText.toString(), currentText.toString())
                                            updateScenePreview(output_size_box_Video.currentText.toString(), color_comp_box_VideoPin.currentIndex.toString(),frame_rate_box.currentIndex)
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
                                    text: "Video record format"
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
                                    text: "Video Encoder format"
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                }

                                ComboBox {
                                    id: fileEncoder
                                    opacity: 1
                                    model: ListModel {
                                        ListElement { text: "YUY"  }
                                        ListElement { text: "MJPG" }
                                        ListElement { text: "H264" }
                                        ListElement { text: "VP8" }
                                    }
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
            }

            Item{
                id:videoRenderer
                Text {
                    x: 20
                    y: about.y - 50
                    id: average_frame
                    text: "Current frame rate achieved"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                    width: 150
                    wrapMode: Text.WordWrap
                }
                TextField {
                    id: average_frame_value
                    x: average_frame.x + 150
                    y: about.y - 50
                    text: "0"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    textColor: "#ffffff"
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: false
                    smooth: true
                    opacity: 1
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            implicitWidth: 70
                            implicitHeight: 24
                            color: "#222021"
                            border.color: "#ffffff"
                            border.width: 1
                        }
                    }
                }
            }
        }

        Button {
            id: uvc_settings
            smooth: true
            focus: false
            activeFocusOnPress: true
            tooltip: "Extension Settings - Its a special settings available for the individual camera"
            style: ButtonStyle {
                background: Image {
                    source: (!cameraColumnLayout.visible || control.activeFocus) ? "images/toggle_selection.png" : ""
                }
                label: Text {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    text: qsTr("Extension Settings")
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                }
            }
            x: cameraColumnLayout.visible ? 134 : 130
            y: cameraColumnLayout.visible ? 153 : 149
            opacity: 1
            onFocusChanged: {
                if(activeFocus)
                    extensionTab()
            }
        }


        Component {
            id: econSliderStyle
            SliderStyle {
                groove:Row {
                    spacing: 0
                    y: 3
                    Rectangle {
                        width: styleData.handlePosition
                        height: 4
                        color: "#dc6239"
                        radius: 5
                    }
                    Rectangle {
                        width: control.width - styleData.handlePosition
                        height: 4
                        color: "#dddddd"
                        radius: 5
                    }
                }
                handle: Image {
                    source: "images/handle.png"
                    opacity: 1
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
            id: tabButtonStyle
            ButtonStyle {
                background: Image {
                    source: cameraColumnLayout.visible ? "images/toggle_selection.png" : ""
                }
                label: Text {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    text: qsTr("Camera Settings")
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                }
            }
        }

        Component {
            id: tabSkipButtonStyle
            ButtonStyle {
                background: Image {
                    source: ""
                }
                label: Text {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    text: qsTr("Camera Settings")
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                }
            }
        }

        Image {
            id: close_sideBar
            source: "images/close_tab.png"
            anchors.bottom: side_bar_bg.bottom
            anchors.bottomMargin: 50
            anchors.right: side_bar_bg.right
            anchors.rightMargin: -28
            y: side_bar_bg.height/2 + 20
            opacity: 1
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    close_sideBar.opacity = 1
                }
                onExited: {
                    close_sideBar.opacity = 1
                }

                onReleased: {
                    if(!cameraColumnLayout.visible)
                        selectCameraSettings()
                    sideBarItems.visible = false
                    open_sideBar.visible = true
                    //previewer.x = 0
                }
            }
        }
    }

    StatusBar {
        id: statusbar
        anchors.bottom: parent.bottom
        Row {
            Label {
                id: statusbartext
                color: "#ffffff"
                text: statusText;
            }
        }
        style: StatusBarStyle {
            background: Rectangle {
                implicitHeight: 16
                implicitWidth: 200
                color: "#222021"
                border.width: 1
            }
        }
    }

    Camproperty {
        id: camproperty
    }

    function mouseClickCapture() {
        m_Snap = false
        capture.enabled = false
        capture.opacity = 0.5
        keyEventFiltering = true
        vidstreamproperty.enabled = false
        if(device_box.currentText == "e-con's 1MP Monochrome\n Camera")
        {
            if(JS.masterMode_M === 1) {
                vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
            }
        } else if(device_box.currentText == "e-con's 1MP Bayer RGB \nCamera") {
            if(JS.masterMode_B === 1) {
                vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
            }
        } else if(device_box.currentText == "See3CAM_11CUG") {
            if(JS.masterMode_11cug === 1) {
                vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
            }
        } else if(device_box.currentText == "See3CAM_CU51") {
            if(JS.masterMode_cu51 === 1) {
                vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
            }
        } else if(device_box.currentText == "See3CAM_12CUNIR") {
            if(JS.masterMode_12cuinr === 1) {
                vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
            }
        } else {
            vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
            vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
        }
    }

    function setOpacityFalse(){
        brightness.opacity = 0.1
        brightness_Slider.enabled = false
        brightness_Slider.minimumValue = -65536  //To solve SliderStyle Qt component problem
        brightness_Slider.maximumValue = 65536  //To solve SliderStyle Qt component problem
        brightness_value.opacity = 0
        brightness_value.enabled = false
        contrast.opacity = 0.1
        contrast_Slider.enabled = false
        contrast_Slider.minimumValue = -65536
        contrast_Slider.maximumValue = 65536
        contrast_value.opacity = 0
        contrast_value.enabled = false
        saturation.opacity = 0.1
        saturation_Slider.enabled = false
        saturation_Slider.minimumValue = -65536
        saturation_Slider.maximumValue = 65536
        saturation_value.opacity = 0
        saturation_value.enabled = false
        pan.opacity = 0.1
        pan_Slider.enabled = false
        pan_Slider.minimumValue = -529200
        pan_Slider.maximumValue = 529200
        pan_value.opacity = 0
        pan_value.enabled = false
        tilt.opacity = 0.1
        tilt_Slider.enabled = false
        tilt_Slider.minimumValue = -432000
        tilt_Slider.maximumValue = 432000
        tilt_value.opacity = 0
        tilt_value.enabled = false
        zoom.opacity = 0.1
        zoom_Slider.enabled = false
        zoom_Slider.minimumValue = 0
        zoom_Slider.maximumValue = 317
        zoom_value.opacity = 0
        zoom_value.enabled = false
        hue.opacity = 0.1
        hue_Slider.enabled = false
        hue_Slider.minimumValue = -65536
        hue_Slider.maximumValue = 65536
        hue_value.opacity = 0
        hue_value.enabled = false
        white_balance.opacity = 0.1
        white_balance_Slider.enabled = false
        white_balance_Slider.minimumValue = -65536
        white_balance_Slider.maximumValue = 65536
        gamma.opacity = 0.1
        gamma_Slider.enabled = false
        gamma_Slider.minimumValue = -65536
        gamma_Slider.maximumValue = 65536
        gamma_value.opacity = 0
        gamma_value.enabled = false
        gain.opacity = 0.1
        gain_Slider.enabled = false
        gain_Slider.minimumValue = -65536
        gain_Slider.maximumValue = 65536
        gain_value.opacity = 0
        gain_value.enabled = false
        sharpness.opacity = 0.1
        sharpness_Slider.enabled = false
        sharpness_Slider.minimumValue = -65536
        sharpness_Slider.maximumValue = 65536
        sharpness_value.opacity = 0
        sharpness_value.enabled = false
        autoSelect_wb.opacity = 0
        powerLine.opacity = 0.1
        exposure_auto.opacity = 0.1
        exposure_absolute.opacity = 0.1
        powerLineCombo.opacity =0.1
        exposureCombo.opacity = 0.1
        menuitems = []
        powerLineCombo.model = menuitems
        exposureCombo.model = menuitems
        exposure_Slider.enabled = false
        exposure_Slider.opacity = 0.1
        exposure_Slider.minimumValue = -65536
        exposure_Slider.maximumValue = 65536
        exposure_value.opacity = 0
        exposure_value.enabled = false
        exposureAutoPriority.opacity = 0.1
        exposureAutoPriorityCheck.opacity = 0.1
        exposureAutoPriorityCheck.enabled = false
        backLightCompensation.opacity = 0.1        
        backLight_Slider.enabled = false
        backLight_Slider.minimumValue = -65536
        backLight_Slider.maximumValue = 65536
        backLight_value.opacity = 0
        backLight_value.enabled = false
        rawBits.opacity = 0.1
        rawBitsSlider.enabled = false
        rawBitsSlider.minimumValue = 0
        rawBitsSlider.maximumValue = 1
        rawBits_value.opacity = 0
        rawBits_value.enabled = false
        ledModeText.opacity = 0.1
        ledModeCombo.enabled = false
        ledModeCombo.opacity = 0.1
        ledFrequency.opacity = 0.1
        ledFreqSlider.enabled = false
        ledFreqSlider.minimumValue = -65536
        ledFreqSlider.maximumValue = 65536
        ledFreq_value.opacity = 0
        ledFreq_value.enabled = false
        focusLogitech.opacity = 0.1
        focusLogitechSlider.enabled = false
        focusLogitechSlider.minimumValue = -65536
        focusLogitechSlider.maximumValue = 655336
        focusLogitech_value.opacity = 0
        focusLogitech_value.enabled = false
        focusauto.opacity = 0.1
        focus_Slider.opacity = 0.1
        focus_Slider.enabled = false
        focus_Slider.minimumValue = -65536
        focus_Slider.maximumValue = 65536
        focus_value.opacity = 0
        focus_value.enabled = false
        autoSelect_focus.opacity = 0.1
        autoSelect_focus.enabled = false
        disableVideoProcess.opacity = 0.1
        disableVideoProcessCheck.opacity = 0.1
        disableVideoProcessCheck.enabled = false
        autoSelect_wb.opacity = 0.1
        autoSelect_wb.enabled = false
    }

    function updateScenePreview(str, format, fps) {
        m_Snap = false        
        if (!vidFormatChanged){            
            vidstreamproperty.width = str.toString().split("x")[0].toString()
            vidstreamproperty.height = str.toString().split("x")[1].toString()
        }
        vidstreamproperty.stopCapture()
        vidstreamproperty.lastPreviewResolution(str,format)
        vidstreamproperty.vidCapFormatChanged(format)
        if (!vidFormatChanged){
            vidstreamproperty.setResoultion(str)
        }
        //updateFPS(color_comp_box_VideoPin.currentText.toString(), output_size_box_Video.currentText.toString())
        vidstreamproperty.frameIntervalChanged(fps)
        vidstreamproperty.startAgain()
    }

    function updateStillPreview(str, format) {
        m_Snap = false
        stillPreview = true        
        vidstreamproperty.stopCapture()
        vidstreamproperty.vidCapFormatChanged(format)
        vidstreamproperty.displayStillResolution()
        vidstreamproperty.setStillVideoSize(str, format)
        vidstreamproperty.startAgain()

        if(JS.videoCaptureFormat !== JS.stillCaptureFormat  || JS.stillCaptureResolution !== JS.videoCaptureResolution)
        {
            vidstreamproperty.stopCapture()
            vidstreamproperty.vidCapFormatChanged(JS.videoCaptureFormat)
            vidstreamproperty.setResoultion(JS.videoCaptureResolution)
            vidstreamproperty.startAgain()
        }
    }

    function updateFPS(pix, size) {        
        vidstreamproperty.updateFrameInterval(pix, size)
    }

    function setControls(ctrlName,ctrlType,ctrlID,ctrlMinValue,ctrlMaxValue,ctrlDefaultValue) {
        controlName = ctrlName.toString()
        controlType = ctrlType;
        controlMinValue = ctrlMinValue
        controlMaxValue = ctrlMaxValue
        controlDefaultValue = ctrlDefaultValue        
        switch(controlType) {
        case 0:
            break;
        case 1:
            if(controlName === "Brightness") {
                brightness.opacity = 1
                brightness_Slider.enabled = true
                brightness_value.opacity = 1
                brightness_value.enabled = true
                brightnessControlId = ctrlID
                brightness_Slider.minimumValue = controlMinValue
                brightness_Slider.maximumValue = controlMaxValue
                brightness_Slider.value = controlDefaultValue
                brightness_Slider.stepSize = 1
            } else if(controlName === "Contrast") {
                contrast.opacity = 1
                contrast_Slider.enabled = true
                contrast_value.opacity = 1
                contrast_value.enabled = true
                contrastControlId = ctrlID
                contrast_Slider.minimumValue = controlMinValue
                contrast_Slider.maximumValue = controlMaxValue
                contrast_Slider.value = controlDefaultValue
            } else if(controlName === "Saturation") {
                saturation.opacity = 1
                saturation_Slider.enabled = true
                saturation_value.opacity = 1
                saturation_value.enabled = true
                saturationControlId = ctrlID
                saturation_Slider.minimumValue = controlMinValue
                saturation_Slider.maximumValue = controlMaxValue
                saturation_Slider.value = controlDefaultValue
            } else if(controlName === "Pan (Absolute)") {
                pan.opacity = 1
                pan_Slider.enabled = true
                pan_value.opacity = 1
                pan_value.enabled = true
                panControlId = ctrlID
                pan_Slider.minimumValue = controlMinValue
                pan_Slider.maximumValue = controlMaxValue
                pan_Slider.value = controlDefaultValue
            } else if(controlName === "Tilt (Absolute)") {
                tilt.opacity = 1
                tilt_Slider.enabled = true
                tilt_value.opacity = 1
                tilt_value.enabled = true
                tiltControlId = ctrlID
                tilt_Slider.minimumValue = controlMinValue
                tilt_Slider.maximumValue = controlMaxValue
                tilt_Slider.value = controlDefaultValue
            } else if(controlName === "Zoom, Absolute") {
                zoom.opacity = 1
                zoom_Slider.enabled = true
                zoom_value.opacity = 1
                zoom_value.enabled = true
                zoomControlId = ctrlID
                zoom_Slider.minimumValue = controlMinValue
                zoom_Slider.maximumValue = controlMaxValue
                zoom_Slider.value = controlDefaultValue
            } else if(controlName === "Hue") {
                hue.opacity = 1
                hue_Slider.enabled = true
                hue_value.opacity = 1
                hue_value.enabled = true
                hueControlId = ctrlID
                hue_Slider.minimumValue = controlMinValue
                hue_Slider.maximumValue = controlMaxValue
                hue_Slider.value = controlDefaultValue
            }else if(controlName === "LED1 Mode") {
                //Turning V4L2_CTRL_TYPE_INTEGER to combobox
                ledModeText.opacity = 1
                ledModeComboControlId = ctrlID
                ledModeComboEnable = true
                ledModeCombo.enabled = true
                ledModeCombo.opacity = 1
                ledModeCombo.currentIndex = controlDefaultValue
            } else if(controlName === "White Balance Temperature") {
                white_balance.opacity = 1                
                whiteBalanceControlId = ctrlID
                white_balance_Slider.minimumValue = controlMinValue
                white_balance_Slider.maximumValue = controlMaxValue
                white_balance_Slider.value = controlDefaultValue
            } else if(controlName === "Gamma") {
                gamma.opacity = 1
                gamma_Slider.enabled = true
                gamma_value.opacity = 1
                gamma_value.enabled = true
                gammaControlId = ctrlID
                gamma_Slider.minimumValue = controlMinValue
                gamma_Slider.maximumValue = controlMaxValue
                gamma_Slider.value = controlDefaultValue
            } else if(controlName === "Gain") {
                gain.opacity = 1
                gain_Slider.enabled = true
                gain_value.opacity = 1
                gain_value.enabled = true
                gainControlId = ctrlID
                gain_Slider.minimumValue = controlMinValue
                gain_Slider.maximumValue = controlMaxValue
                gain_Slider.value = controlDefaultValue
            } else if(controlName === "Sharpness") {
                sharpness.opacity = 1
                sharpness_Slider.enabled = true
                sharpness_value.opacity = 1
                sharpness_value.enabled = true
                sharpnessControlId = ctrlID
                sharpness_Slider.minimumValue = controlMinValue
                sharpness_Slider.maximumValue = controlMaxValue
                sharpness_Slider.value = controlDefaultValue
            } else if(controlName === "Exposure (Absolute)") {
                exposure_absolute.opacity = 1
                if((device_box.currentText === "e-con's CX3 RDK with O\nV5680") || (device_box.currentText === "e-con's CX3 RDK with M\nT9P031") || (device_box.currentText === "See3CAM_CU40") ) {                    
                    exposure_Slider.opacity = 1
                    exposure_Slider.enabled = true
                    exposure_value.opacity = 1
                    exposure_value.enabled = true
                }

                exposurecontrolId = ctrlID
                exposure_Slider.minimumValue = controlMinValue
                exposure_Slider.maximumValue = controlMaxValue
                exposure_Slider.value = controlDefaultValue
            } else if(controlName === "Focus (absolute)") {
                focusauto.opacity = 1
                if(device_box.currentText === "e-con's CX3 RDK with O\nV5680") {
                    focus_Slider.opacity = 1
                    focus_Slider.enabled = true
                    focus_value.opacity = 1
                    focus_value.enabled = true
                }
                focusControlId = ctrlID
                focus_Slider.minimumValue = controlMinValue
                focus_Slider.maximumValue = controlMaxValue
                focus_Slider.value = controlDefaultValue

            } else if(controlName === "Backlight Compensation") {
                backLightCompensation.opacity = 1
                backLight_Slider.enabled = true
                backLight_value.opacity = 1
                backLight_value.enabled = true
                backLightCompensationId = ctrlID
                backLight_Slider.minimumValue = controlMinValue
                backLight_Slider.maximumValue = controlMaxValue
                backLight_Slider.value = controlDefaultValue
            } else if(controlName === "Raw bits per pixel") {
                rawBits.opacity = 1
                rawBitsSlider.enabled = true
                rawBits_value.opacity = 1
                rawBits_value.enabled = true
                rawBitsControlId = ctrlID
                rawBitsSlider.minimumValue = controlMinValue
                rawBitsSlider.maximumValue = controlMaxValue
                rawBitsSlider.value = controlDefaultValue
            } else if(controlName === "LED1 Frequency") {
                ledFrequency.opacity = 1
                ledFreqSlider.enabled = true
                ledFreq_value.opacity = 1
                ledFreq_value.enabled = true
                ledFreqControlId = ctrlID
                ledFreqSlider.minimumValue = controlMinValue
                ledFreqSlider.maximumValue = controlMaxValue
                ledFreqSlider.value = controlDefaultValue
            } else if(controlName === "Focus") {
                focusLogitech.opacity = 1
                focusLogitechSlider.enabled = true
                focusLogitech_value.opacity = 1
                focusLogitech_value.enabled = true
                focusLogitechSlider.minimumValue = controlMinValue
                focusLogitechSlider.maximumValue = controlMaxValue
                focusLogitechSlider.value = controlDefaultValue
                focusLogitechControlId = ctrlID
            }
            break;
        case 2:
            if(controlName == "White Balance Temperature, Auto") {                
                white_balance.opacity = 1
                autoSelect_wb.opacity = 1
                autoSelect_wb.enabled = true
                autoSelect_wb.checked = controlDefaultValue
                whiteBalanceControl_auto_Id = ctrlID
                if(!autoSelect_wb.checked) {
                    white_balance_Slider.enabled = true
                }
            } else if(controlName == "Focus, Auto") {
                autoSelect_focus.opacity = 1
                autoSelect_focus.enabled = true
                autoSelect_focus.checked = controlDefaultValue
                focusControlAutoId = ctrlID
                if(!autoSelect_focus.checked) {
                    JS.autoFocusChecked = false
                    focus_Slider.opacity = 1
                    focus_Slider.enabled = true
                    focus_value.opacity = 1
                    focus_value.enabled = true
                }
            } else if(controlName == "Exposure, Auto Priority") {
                exposureAutoPriority.opacity = 1
                exposureAutoPriorityCheck.opacity = 1
                exposureAutoPriorityCheck.enabled = true
                exposureAutoPriorityCheck.checked = controlDefaultValue
                exposureAutoPriorityControlId = ctrlID
            } else if(controlName == "Disable video processing") {
                disableVideoProcess.opacity = 1
                disableVideoProcessCheck.opacity = 1
                disableVideoProcessCheck.enabled = true
                disableVideoProcessCheck.checked = controlDefaultValue
                disableVideoControlId = ctrlID
            }

            break;
        case 3:
            menuitems.push(controlName)
            if(controlName === "Power Line Frequency") {
                menuitems.pop() //Control Name should be removed
                powerLine.opacity = 1
                powerLineCombo.opacity = 1
                powerLineCombo.model = menuitems
                powerLineCombo.currentIndex = controlDefaultValue
                while(menuitems.pop()){}
                powerLineComboControlId = ctrlID
                powerLineComboEnable =  true

            }
            else if(controlName === "Exposure, Auto") {
                menuitems.pop()
                exposure_auto.opacity = 1
                exposureCombo.opacity = 1
                exposureCombo.model = menuitems
                while(menuitems.pop()){}
                exposureAutoControlId = ctrlID
                exposureComboEnable =  true
                exposureCombo.currentIndex = controlDefaultValue
                if(exposureCombo.currentText == "Manual Mode"){
                    JS.autoExposureSelected = false
                    exposure_absolute.opacity = 1
                    exposure_Slider.enabled = true
                    exposure_Slider.opacity = 1
                    exposure_value.opacity = 1
                    exposure_value.enabled = true
                }else{
                    JS.autoExposureSelected = true
                }
            }
            break;
        case 9:
            break;
        }

        //        controlId = ctrlID;
        //        var labelObject = Qt.createQmlObject('import QtQuick 2.0;
        //                        import QtQuick.Controls 1.1; Text {
        //                        text: "'+controlName+'";font.pixelSize: 10;
        //                        font.family: "Ubuntu";
        //                        color: "#ffffff";
        //                        smooth: true;
        //                        width: 80;
        //                        wrapMode:Text.WordWrap;
        //                        x: -10;
        //                        y: '+controly+';
        //                        opacity: 1
        //                        property string buttonId;
        //                        signal clicked(string buttonId);
        //                        MouseArea {
        //                            anchors.fill: parent
        //                            onClicked:parent.clicked(parent.buttonId)
        //                        }
        //                    }', ctrlRect);

        //        if(ctrlType.toString() === "1")
        //        {
        //            var sliderObject = Qt.createQmlObject('import QtQuick 2.0;
        //                            import QtQuick.Controls 1.1;   import QtQuick.Controls.Styles 1.0;
        //                Slider {
        //                x: 75
        //                y: '+controly+'
        //                minimumValue: 0
        //                maximumValue: 100
        //                stepSize: 1
        //                opacity: 1
        //                style: SliderStyle {
        //                    groove: Image {
        //                        source: "images/uvc_red_fill.png"
        //                    }
        //                    handle: Image {
        //                        source: "images/uvc_red_scroller.png"
        //                        y: -4
        //                        opacity: 1
        //                    }
        //                }onValueChanged:  {
        //console.log("Changing... i wondered")
        //}
        //            }',ctrlRect);
        //        }
        //        controly = controly + 30;
        //        //newObject.destroy(1000);

    }

    function selectCameraSettings(){
        camera_settings.forceActiveFocus()
        if(!cameraColumnLayout.visible)
        {
            see3cam.destroy()
        }
        if(device_box.currentText == "e-con's 1MP Bayer RGB \nCamera")
        {
           cbItemsImgFormat.clear();
           cbItemsImgFormat.insert(0, {text: "raw" })
           cbItemsImgFormat.insert(1, {text: "bmp" })
           cbItemsImgFormat.insert(2, {text: "jpg" })
           cbItemsImgFormat.insert(3, {text: "png" })
        }else if(device_box.currentText == "See3CAM_CU40")
        {
           cbItemsImgFormat.clear();
           cbItemsImgFormat.insert(0, {text: "jpg" })
           cbItemsImgFormat.insert(1, {text: "bmp" })
           cbItemsImgFormat.insert(2, {text: "raw" })
           cbItemsImgFormat.insert(3, {text: "png" })
           cbItemsImgFormat.insert(4, {text: "IR data(8bit BMP)" })
        }else{
           cbItemsImgFormat.clear();
           cbItemsImgFormat.insert(0, {text: "jpg" })
           cbItemsImgFormat.insert(1, {text: "bmp" })
           cbItemsImgFormat.insert(2, {text: "raw" })
           cbItemsImgFormat.insert(3, {text: "png" })
           imageFormatCombo.currentIndex = 0;
        }
        cameraColumnLayout.visible = true
        stillchildProperty.visible = false
        video_capture_filter_Child.visible = false
        video_Capture_property_Child.visible = false
    }

    function triggerModeCapture(){
        capture.enabled = false
        capture.opacity = 0.5
        record.enabled = false
        record.opacity = 0.5
        keyEventFiltering = true
        vidstreamproperty.enabled = false
        vidstreamproperty.triggerModeEnabled()
        vidstreamproperty.stopCapture()
        vidstreamproperty.vidCapFormatChanged(JS.stillCaptureFormat)
        vidstreamproperty.setResoultion(JS.stillCaptureResolution);
        vidstreamproperty.startAgain();
    }

    function masterModeCapture(){
        if(!capture.enabled || !record.enabled)  {
            capture.enabled = true
            capture.opacity = 1
            record.enabled = true
            record.opacity = 1
            vidstreamproperty.enabled = true
            keyEventFiltering = false
        }
        vidstreamproperty.masterModeEnabled()
        if(JS.videoCaptureFormat !== JS.stillCaptureFormat  || JS.stillCaptureResolution !== JS.videoCaptureResolution)
        {
            vidstreamproperty.stopCapture()
            vidstreamproperty.vidCapFormatChanged(JS.videoCaptureFormat)
            vidstreamproperty.setResoultion(JS.videoCaptureResolution);
            vidstreamproperty.startAgain();            
        }
    }

    function videoPin() {
        outputSizeBox = true
        frameRateBox = true
        colorSpace = true
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

    function videoRecordBegin() {
        seconds2 = 0
        minutes = 0
        hours = 0
        videoTimer.start()
        vidstreamproperty.recordBegin(JS.videoEncoder,JS.videoExtension, videoStoragePath)
        videoCaptureProperty.enabled = false
        videoCaptureProperty.opacity = 0.5
        stillproperties.enabled = false
        stillproperties.opacity = 0.5
        device_box.enabled = false
        device_box.opacity = 0.5
        record_stop.visible = true
        record.visible = false
        keyEventFiltering = true
        uvc_settings.enabled = false
        uvc_settings.opacity = 0.5
        selectCameraSettings()
    }

    function videoSaveVideo() {
        statusText = statusbartext.text
        statusbartext.text = "Saving..."
        vidstreamproperty.recordStop()
        videoTimer.stop()
        vidRecTimer.text = ""
        vidRecTimer.visible = false
        messageDialog.title = qsTr("Saved")
        messageDialog.text = qsTr("Video saved in the location:"+videofileName)
        messageDialog.open()
        videoCaptureProperty.enabled = true
        videoCaptureProperty.opacity = 1
        stillproperties.enabled = true
        stillproperties.opacity = 1
        device_box.enabled = true
        vidstreamproperty.enabled = true
        device_box.opacity = 1
        record.visible = true
        keyEventFiltering = false
        record_stop.visible = false
        uvc_settings.enabled = true
        uvc_settings.opacity = 1
        statusTimer.start()
    }

    function videoControlFilter() {
        if(!video_capture_filter_Child.visible) {
            camproperty.logDebugWriter("Video Capture Filter Selected")
            brightValueChangeProperty = true
            brightness_Slider.forceActiveFocus()
            contrastValueChangeProperty = true
            saturationValueChangeProperty = true
            panValueChangeProperty = true
            tiltValueChangeProperty = true
            zoomValueChangeProperty = true
            wbValueChangeProperty = true
            sharpValueChangeProperty = true
            gainValueChangeProperty = true
            hueValueChangeProperty = true
            ledModeChangeProperty = true
            exposureValueChangeProperty = true
            bcklightValueChangeProperty = true
            rawBitsValueChangeProperty = true
            ledFreqValueChangeProperty = true
            focusLogitechValueChangeProperty = true
            focusValueChangeProperty = true
            controly = 30
            stillchildProperty.visible = false
            video_capture_filter_Child.visible = true
            video_Capture_property_Child.visible = false
            vidstreamproperty.cameraFilterControls()
        } else {
            video_capture_filter_Child.visible = false
            video_capture_filter_Child.focus = false
        }
    }

    function stillImageProperty() {
        if(!stillchildProperty.visible) {
            camproperty.logDebugWriter("Still Properties Selected")
            stillchildProperty.visible = true
            color_comp_box.forceActiveFocus()
            video_capture_filter_Child.visible = false
            video_Capture_property_Child.visible = false
            controly =0
            stillColorSpace = true
        } else {
            stillchildProperty.visible = false
            stillchildProperty.focus = false
        }
    }

    function videoCapProperty() {
        if(!video_Capture_property_Child.visible) {
            camproperty.logDebugWriter("Video Capture Property selected")
            stillchildProperty.visible = false
            video_capture_filter_Child.visible = false
            video_Capture_property_Child.visible = true
            frame_rate_box.forceActiveFocus()
            controly = 0
        } else {
            video_Capture_property_Child.visible = false
        }
    }

    function openVideoPinPage() {
        if(device_box.currentText == "e-con's 1MP Monochrome\n Camera") {
            if(JS.masterMode_M === 1) {
                videoPin()
            } else {                
                video_Capture_property_Child.visible = false
                messageDialog.title = qsTr("Disabled")
                messageDialog.text = qsTr("Video Capture Settings is disabled in trigger Mode")
                messageDialog.open()                
            }
        } else if(device_box.currentText == "e-con's 1MP Bayer RGB \nCamera") {
            if(JS.masterMode_B === 1) {
                videoPin()
            } else {
                video_Capture_property_Child.visible = false
                messageDialog.title = qsTr("Disabled")
                messageDialog.text = qsTr("Video Capture Settings is disabled in trigger Mode")
                messageDialog.open()
            }
        } else if(device_box.currentText == "See3CAM_11CUG") {
            if(JS.masterMode_11cug === 1) {
                videoPin()
            } else {
                video_Capture_property_Child.visible = false
                messageDialog.title = qsTr("Disabled")
                messageDialog.text = qsTr("Video Capture Settings is disabled in trigger Mode")
                messageDialog.open()
            }
        } else if(device_box.currentText == "See3CAM_CU51") {
            if(JS.masterMode_cu51 === 1) {
                videoPin()
            } else {
                video_Capture_property_Child.visible = false
                messageDialog.title = qsTr("Disabled")
                messageDialog.text = qsTr("Video Capture Settings is disabled in trigger Mode")
                messageDialog.open()
            }
        } else if(device_box.currentText == "See3CAM_12CUNIR") {
            if(JS.masterMode_12cuinr === 1) {
                videoPin()
            } else {
                video_Capture_property_Child.visible = false
                messageDialog.title = qsTr("Disabled")
                messageDialog.text = qsTr("Video Capture Settings is disabled in trigger Mode")
                messageDialog.open()
            }
        } else {
            videoPin()
        }
    }

    function extensionTab() {
        if(cameraColumnLayout.visible) {
            cameraColumnLayout.visible = false
            JS.cameraComboIndex = device_box.currentIndex
            if(device_box.currentText == "e-con's 1MP Bayer RGB \nCamera" ) {
                see3cam = Qt.createComponent("../UVCSettings/see3cam10/uvc10_c.qml").createObject(root)
            } else if(device_box.currentText == "e-con's 1MP Monochrome\n Camera") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam10/uvc10_m.qml").createObject(root)
            } else if(device_box.currentText == "See3CAM_11CUG") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam11/uvc11.qml").createObject(root)
            } else if(device_box.currentText == "e-con's 8MP Camera") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam80/uvc80.qml").createObject(root)
            } else if(device_box.currentText == "See3CAMCU50") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam50/uvc50.qml").createObject(root)
            } else if(device_box.currentText == "See3CAM_CU130") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam130/uvc130.qml").createObject(root)
            } else if(device_box.currentText == "See3CAM_CU51") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam51/uvc51.qml").createObject(root)
            } else if(device_box.currentText == "See3CAM_12CUNIR") {
                see3cam = Qt.createComponent("../UVCSettings/see3camar0130/uvc_ar0130.qml").createObject(root)
            } else if(device_box.currentText == "See3CAM_CU40") {
                see3cam = Qt.createComponent("../UVCSettings/see3cam40/uvc40.qml").createObject(root)
            } else if(device_box.currentText == "CX3-UVC") {
                see3cam = Qt.createComponent("../UVCSettings/ascella/cx3-uvc.qml").createObject(root)
            } else {
                see3cam = Qt.createComponent("../UVCSettings/others/others.qml").createObject(root)
            }
        }
    }

    Component.onCompleted: {
        setOpacityFalse()
        camproperty.createLogger()
        camera_btn.forceActiveFocus()
        var component2 = Qt.createComponent("../about/QML_about.qml")
        aboutWindow = component2.createObject(root);
    }
    Component.onDestruction: {
        if(record_stop.visible) {
            vidstreamproperty.recordStop()
            videoTimer.stop()
            vidRecTimer.text = ""
            vidRecTimer.visible = false
        }
        JS.enableMasterMode_10cugM()
        JS.enableMasterMode_10cugB()
        JS.enableMasterMode_11cug()
        exitDialog.visible = false
    }

    Keys.onReleased: {
        if(event.key === Qt.Key_I) {
            if((!keyEventFiltering)) {
                mouseClickCapture()
            }
        } else if(event.key === Qt.Key_WebCam){
            m_Snap = false
            if(webcamKeyAccept) {
                if(device_box.currentText == "e-con's 1MP Monochrome\n Camera")
                {
                    if(JS.masterMode_M === 1) {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    } else {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.triggerModeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    }
                } else if(device_box.currentText == "e-con's 1MP Bayer RGB \nCamera") {
                    if(JS.masterMode_B === 1) {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    } else {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.triggerModeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    }
                } else if(device_box.currentText == "See3CAM_11CUG") {
                    if(JS.masterMode_11cug === 1) {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    } else {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.triggerModeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    }
                } else if(device_box.currentText == "See3CAM_CU51") {
                    if(JS.masterMode_cu51 === 1) {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    } else {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.triggerModeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    }
                } else if(device_box.currentText == "See3CAM_12CUNIR") {
                    if(JS.masterMode_12cuinr === 1) {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    } else {
                        vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                        vidstreamproperty.triggerModeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                    }
                } else {
                    vidstreamproperty.setStillVideoSize(output_value.currentText.toString(), color_comp_box.currentIndex.toString())
                    vidstreamproperty.makeShot(storage_path.text.toString(),imageFormatCombo.currentText.toString())
                }
                webcamKeyAccept = false
            }
            event.accepted = true
        }
    }

    Keys.onLeftPressed: {
        if(!cameraColumnLayout.visible)
            selectCameraSettings()
        sideBarItems.visible = false
        open_sideBar.visible = true
    }

    Keys.onRightPressed: {
        sideBarItems.visible = true
        open_sideBar.visible = false
   }
}

