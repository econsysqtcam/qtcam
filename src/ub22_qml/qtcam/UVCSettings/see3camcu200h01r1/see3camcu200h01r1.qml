/*
 * see3camcu200h01r1.qml -- extension settings for See3CAM_CU200M_H01R1 camera
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

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.see3camcu200mh01r1 1.0
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item{

    Action {
        id: setDefault
        onTriggered: {
            setToDefaultValues()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            getFirmwareVersion()
        }
    }

    Action {
        id: ispVersion
        onTriggered:{
            getISPFirmwareVersion()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
        }
    }

    Action {
        id: triggerGainAction
        onTriggered: {
            see3camcu200mh01r1.setGainMode(See3CAM_CU200M_H01R1.AUTO_GAIN,See3CAM_CU200M_H01R1.GAIN_SINGLE_SHOT, 1)
        }
    }

    Action {
        id: triggerExposureAction
        onTriggered: {
            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
        }
    }

    Action {
        id: readStatisticsCall
        onTriggered:{
            see3camcu200mh01r1.readStatistics()
        }
    }

    Timer {
        id: getCameraSettingsTimer
        interval: 500
        onTriggered: {
            see3camcu200mh01r1.getExposureCompensation()
            see3camcu200mh01r1.getTargetBrightness()
            see3camcu200mh01r1.readStatistics()
            stop()
        }
    }

    Connections{
        target: root

        function onTakeScreenShot(){
            root.imageCapture(CommonEnums.SNAP_SHOT);
        }
        function onGetVideoPinStatus(){
            root.enableVideoPin(true);
        }
        function onGetStillImageFormats(){
            var stillImageFormat = []
            stillImageFormat.push("jpg")
            stillImageFormat.push("bmp")
            stillImageFormat.push("raw")
            stillImageFormat.push("png")
            root.insertStillImageFormat(stillImageFormat);
        }
        function onGetExposureFromUVC(exposureFromUVC){
            manualExpTextField.text = exposureFromUVC
        }
        function onUpdateCrossStillCaptureProperty(){
            if(isEnable){
                see3camcu200mh01r1.setStillProperties(true)
            } else{
                see3camcu200mh01r1.setStillProperties(false)
            }
        }
        function onAutoExposureSelected(autoExposureSelect){
            enableDisableAutoExposureControls(autoExposureSelect)
        }

        function onMouseRightClicked(x, y, width, height){
            if(expRoiManual.enabled && expRoiManual.checked){
                see3camcu200mh01r1.setROIAutoExposure(See3CAM_CU200M_H01R1.MANUAL_ROI, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        function onVideoResolutionChanged(){
            getCameraSettingsTimer.start()
        }
        function onPreviewFPSChanged(){
            getCameraSettingsTimer.start()
        }
        function onVideoColorSpaceChanged(){
            getCameraSettingsTimer.start()
        }

        function onSkipFrameCount(){
            root.setSkipFrameCount(3)
        }
    }

    property int exposureMin: 100
    property int exposureMax: 1000000

    property bool skipUpdateBlackLevelMode      : false
    property bool setButtonClicked              : false
    property bool expSingleShotBtnClicked       : false

    property bool skipUpdateTargetBrightness    : false
    property bool skipUpdateUIOnExpWindowSize   : false
    property bool skipUpdateUIOnAntiFlickerMode : false
    property bool skipUpdateExposureUpperLimit  : false
    property bool skipUpdateExposureCompensation : false

    property int exposureInt: 0
    property int exposureText: 0

    property bool uvcAutoExpSelected  : false

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle
        Item{
            height: 1650

            ColumnLayout{
                x:2
                y:5
                spacing:18
                Text {
                    id: expModeTitle
                    text: "--- Exposure Mode---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 30
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: expModeGroup }
                    RadioButton {
                        id: autoExposure
                        exclusiveGroup: expModeGroup
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "This control enables user to set exposure in automatic mode."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setExposureProperties()
                        }
                        Keys.onReturnPressed: {
                            setExposureProperties()
                        }
                    }

                    RadioButton {
                        id: manualExposure
                        exclusiveGroup: expModeGroup
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "This control enables user to set the manual exposure value."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            setExposureProperties()
                        }
                        Keys.onReturnPressed: {
                            setExposureProperties()
                        }
                    }
                }


                Text {
                    id: upperLimitMode
                    text: "--- Auto Gain Upper Limit ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "You can set the required limit by changing the value in the slider"
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: gainUpperLimitSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainUpperLimitTextField.text = gainUpperLimitSlider.value
                            if(skipUpdateExposureUpperLimit){
                                see3camcu200mh01r1.setAutoGainUpperLimit(gainUpperLimitSlider.value)
                            }
                            skipUpdateExposureUpperLimit = true
                        }
                    }
                    TextField {
                        id: gainUpperLimitTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainUpperLimitSlider.minimumValue; top: gainUpperLimitSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                gainUpperLimitSlider.value = gainUpperLimitTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: expCompTitle
                    text: "--- Exposure Compensation ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        width: 200
                        opacity: 0
                    }
                }

                Row
                {
                    spacing: 9
                    Text
                    {
                        id: expCompLabel
                        text: "value(µs)[100 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                    }
                    TextField
                    {
                        id: expCompensationTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: exposureMin; top: exposureMax}
                    }
                    Button
                    {
                        id: expCompSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            expCompSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu200mh01r1.setExposureCompensation(expCompensationTextField.text);
                            expCompSetBtn.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            expCompSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu200mh01r1.setExposureCompensation(expCompensationTextField.text);
                            expCompSetBtn.enabled = true
                        }
                    }
                }

                Text {
                    id: autoExpModeTitle
                    text: "--- Auto Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:10
                    ExclusiveGroup { id: autoExposureFeatureGroup }
                    RadioButton {
                        exclusiveGroup: autoExposureFeatureGroup
                        id: exposureContinious
                        text: "Continuous"
                        activeFocusOnPress: true
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "The camera detects the light source and sets the exposure value continuously with respect to change in scenes."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            expSingleShotBtnClicked = false
                            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.CONTINIOUS_EXPOSURE, manualExpTextField.text)
                        }
                        Keys.onReturnPressed: {
                            expSingleShotBtnClicked = false
                            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.CONTINIOUS_EXPOSURE, manualExpTextField.text)
                        }
                    }
                }

                Row{
                    spacing:25
                    RadioButton {
                        exclusiveGroup: autoExposureFeatureGroup
                        id: exposureSingleShot
                        text: "Single Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The camera calculates the exposure based on the current scene just once and strict with it until next request."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            if(!expSingleShotBtnClicked){
                                expSingleShotBtnClicked = true
                                see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
                            }
                        }
                        Keys.onReturnPressed: {
                            if(!expSingleShotBtnClicked){
                                expSingleShotBtnClicked = true
                                see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
                            }
                        }
                    }
                    Button {
                        id: triggerExpBtn
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (exposureSingleShot.enabled && exposureSingleShot.checked) ? true : false
                        opacity: (exposureSingleShot.enabled && exposureSingleShot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        action: (exposureSingleShot.enabled && exposureSingleShot.checked) ? triggerExposureAction : null
                        tooltip: "Allows camera to calculate the exposure value once in Single shot mode"
                        Keys.onReturnPressed: {
                            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
                        }
                    }
                }

                Text {
                    id: manualExpModeTitle
                    text: "--- Manual Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "It controls the integration time of the sensor. It changes in the range of 100µs to 1s. It is expressed in microseconds(µs)."
                        width: 200
                        opacity: 0
                    }
                }

                Row
                {
                    spacing: 9
                    Text
                    {
                        id: manualExpTitle
                        text: "value(µs)[100 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                    }
                    TextField
                    {
                        id: manualExpTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: exposureMin; top: exposureMax}
                    }
                    Button
                    {
                        id: exposureSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            exposureSetBtn.enabled = false
                            setButtonClicked = true

                            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.MANUAL_EXPOSURE, 0, manualExpTextField.text);

                            //Sending HID value to UVC
                            exposureText = manualExpTextField.text
                            if(manualExpTextField.text <= 1000000)
                            {
                                exposureInt = manualExpTextField.text / 100
                                root.sendExposureToUVC(exposureInt)
                            }
                            else{
                                root.sendExposureToUVC(100000)
                            }
                            manualExpTextField.text = exposureText

                            exposureSetBtn.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            exposureSetBtn.enabled = false
                            setButtonClicked = true

                            //Sending HID value to UVC
                            root.sendExposureToUVC(manualExpTextField.text)

                            manualExpTextField.text = exposureText

                            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.MANUAL_EXPOSURE, 0, manualExpTextField.text);
                            exposureSetBtn.enabled = true
                        }
                    }
                }

                Text
                {
                    id: brightnessTitle
                    text: "--- Target Brightness ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Target Brightness control allows to set the brightness of the image preview should maintained in auto exposure and auto gain modes."
                        opacity: 0
                        width: 200
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: targetBrightness
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            targetBrightnessTextField.text = targetBrightness.value
                            if(skipUpdateTargetBrightness){
                                see3camcu200mh01r1.setTargetBrightness(targetBrightness.value)
                            }
                            skipUpdateTargetBrightness = true
                        }
                    }
                    TextField {
                        id: targetBrightnessTextField
                        text: targetBrightness.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: targetBrightness.minimumValue; top: targetBrightness.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                targetBrightness.value = targetBrightnessTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: roiAutoExpMode
                    text: "--- Auto Exposure ROI ---"
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
                      ExclusiveGroup { id: roiExpogroup }
                      Layout.alignment: Qt.AlignCenter

                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: expRoiFull
                          text: "Full"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          ToolButton{
                              tooltip: "Full region-based exposure value will be applied to the frame."
                              width: 15
                              height: 20
                              opacity: 0
                          }
                          onClicked: {
                              setROIAutoExposure()
                          }
                          Keys.onReturnPressed: {
                              setROIAutoExposure()
                          }
                      }
                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: expRoiManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          ToolButton{
                              tooltip: "Users can select the region of interest to calculate the exposure value for that particular area. This value will then be applied to the entire frame.
The mouse click button can be used to select the auto exposure area in the preview."
                              width: 15
                              height: 20
                              opacity: 0
                          }
                          onClicked: {
                              setROIAutoExposure()
                          }
                          Keys.onReturnPressed: {
                              setROIAutoExposure()
                          }
                      }
                }
                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (expRoiManual.checked && expRoiManual.enabled) ? true : false
                    opacity: (expRoiManual.checked && expRoiManual.enabled) ? 1 : 0.1
                    ToolButton{
                        tooltip: "Window size defines the size of the exposure region. For window size 8, the entire frame will be the focus region.
If the exposure region exceeds the frame boundary, the ROI will be clipped automatically."
                        width: 200
                        opacity: 0
                    }
                    model: ListModel
                           {
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
                            see3camcu200mh01r1.setROIAutoExposure(See3CAM_CU200M_H01R1.MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }

                Text
                {
                    id: antiFlickerMode
                    text: "--- Anti Flicker Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ComboBox
                {
                    id: antiFlickerCombo
                    model: ListModel
                           {
                                ListElement { text: "50 Hz" }
                                ListElement { text: "60 Hz" }
                                ListElement { text: "DISABLE" }
                           }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    ToolButton{
                        tooltip: "Disable : It disables Anti-Flicker mode
50 Hz   : 50Hz flicker frequency is applied.
60 Hz   : 60Hz flicker frequency is applied."
                        width: 200
                        opacity: 0
                    }
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnAntiFlickerMode){
                            setAntiFlicker()
                        }
                        skipUpdateUIOnAntiFlickerMode = true
                    }
                }


                Text {
                    id: blackLevelAdj
                    text: "--- Black Level ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Black level is the value which is subtracted from the image signal to compensate the thermally generated noise.
 Note: Changing the value will affect image quality."
                        opacity: 0
                        width: 200
                    }
                }
                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: blackLevelSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            blackLevelTextField.text = blackLevelSlider.value
                            if(skipUpdateBlackLevelMode){
                                see3camcu200mh01r1.setBlackLevel(blackLevelSlider.value)
                            }
                            skipUpdateBlackLevelMode = true
                        }
                    }

                    TextField
                    {
                        id: blackLevelTextField
                        text: blackLevelSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: blackLevelSlider.minimumValue; top: blackLevelSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                blackLevelSlider.value = blackLevelTextField.text
                            }
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
                  spacing: 25
                  Layout.alignment: Qt.AlignCenter
                  ExclusiveGroup { id: cameraModeGroup }

                  RadioButton {
                      id: masterMode
                      style:  econRadioButtonStyle
                      text: qsTr("Master")
                      exclusiveGroup: cameraModeGroup
                      activeFocusOnPress: true
                      ToolButton{
                          tooltip: "After choosing master mode, the application starts video streaming. This is a simple mode of operation for the camera without any external trigger capability."
                          width: 15
                          height: 20
                          opacity: 0
                      }
                      onClicked: {
                          setMasterMode()
                      }
                      Keys.onReturnPressed: {
                          setMasterMode()
                      }
                  }
                  RadioButton {
                      id: triggerMode
                      style:  econRadioButtonStyle
                      text: qsTr("Trigger")
                      exclusiveGroup: cameraModeGroup
                      activeFocusOnPress: true
                      ToolButton{
                          tooltip: "In Trigger mode, Frames will be out only when external hardware pulses are given to PIN1 of CN4."
                          width: 15
                          height: 20
                          opacity: 0
                      }
                      onClicked: {
                          setTriggerMode()
                      }
                      Keys.onReturnPressed: {
                          setTriggerMode()
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
               Row{
                   spacing: 30
                   Layout.alignment: Qt.AlignCenter
                   ExclusiveGroup { id: strobesGrp }
                   RadioButton {
                       id: strobeOn
                       exclusiveGroup: strobesGrp
                       checked: false
                       text: "On"
                       ToolButton{
                           tooltip: "When this control is enabled, the LED is ON for each frame’s exposure time while Video Streaming."
                           width: 15
                           height: 20
                           opacity: 0
                       }
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3camcu200mh01r1.setStrobeMode(See3CAM_CU200M_H01R1.STROBE_ON)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.setStrobeMode(See3CAM_CU200M_H01R1.STROBE_ON)
                       }
                   }

                   RadioButton {
                       id: strobeOff
                       exclusiveGroup: strobesGrp
                       checked: false
                       text: "Off"
                       ToolButton{
                           tooltip: "It disables strobe control."
                           width: 15
                           height: 20
                           opacity: 0
                       }
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3camcu200mh01r1.setStrobeMode(See3CAM_CU200M_H01R1.STROBE_OFF)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.setStrobeMode(See3CAM_CU200M_H01R1.STROBE_OFF)
                       }
                   }
               }

               Text {
                   id: flipMode
                   text: "--- Orientation Mode ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
               }
               Row{
                   spacing: 40
                   Layout.alignment: Qt.AlignCenter
                   CheckBox {
                       id: flipCtrlHorizotal
                       activeFocusOnPress : true
                       text: "Horizontal"
                       style: econCheckBoxStyle
                       ToolButton{
                           tooltip: "Flips the preview left/right."
                           width: 15
                           height: 20
                           opacity: 0
                       }
                       onClicked:{
                           see3camcu200mh01r1.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                       }
                   }
                   CheckBox {
                       id: flipCtrlVertical
                       activeFocusOnPress : true
                       text: "Vertical"
                       style: econCheckBoxStyle
                       ToolButton{
                           tooltip: "Flips the preview vertically up/down."
                           width: 15
                           height: 20
                           opacity: 0
                       }
                       onClicked:{
                           see3camcu200mh01r1.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                       }
                   }
               }

               Text {
                   id: readStatisticsTitle
                   text: "--- Read Statistics ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "To Read current Gain, Exposure and CCM in auto mode"
                       width: 200
                       opacity: 0
                   }
               }


               Row
               {
                   spacing: 9
                   Layout.alignment: Qt.AlignCenter
                   Text
                   {
                       id: exposureStatisticsTitle
                       text: "Exposure(µs)"
                       font.pixelSize: 14
                       font.family: "Ubuntu"
                       color: "#ffffff"
                       smooth: true
                       width: 80
                       wrapMode: Text.WordWrap
                       ToolButton{
                           tooltip: "This control displays the calculated current exposure value in Auto exposure mode."
                           width: 200
                           opacity: 0
                       }
                   }
                   TextField
                   {
                       id: expStatTextField
                       font.pixelSize: 10
                       font.family: "Ubuntu"
                       readOnly: true
                       smooth: true
                       horizontalAlignment: TextInput.AlignHCenter
                       style: econTextFieldStyle
                       enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                       opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                       implicitHeight: 25
                       implicitWidth: 80
                   }
               }

               Row
               {
                   spacing: 35
                   Layout.alignment: Qt.AlignCenter
                   Text {
                       id: gainLabel
                       text: "Gain "
                       font.pixelSize: 14
                       font.family: "Ubuntu"
                       color: "#ffffff"
                       smooth: true
                       ToolButton{
                           tooltip: "This control displays the calculated current gain value in Auto gain mode."
                           width: 200
                           opacity: 0
                       }
                   }
                   TextField
                   {
                       id: gainStatTextField
                       font.pixelSize: 10
                       font.family: "Ubuntu"
                       readOnly: true
                       smooth: true
                       horizontalAlignment: TextInput.AlignHCenter
                       style: econTextFieldStyle
                       enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                       opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                   }
               }

               Row{
                   Layout.alignment: Qt.AlignCenter
                   Button {
                       id: readStatisticsBtn
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Read Statistics"
                       action: readStatisticsCall
                       style: econButtonStyle
                       implicitHeight: 30
                       implicitWidth: 100
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.readStatistics()
                       }
                   }
               }

               Text {
                   id: userDefinedTitle
                   text: "--- User Defined Configuration ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "This control provides the functionality to securely store all currently configured settings in the device's memory.
Following the configuration save, it is recommended to initiate a device reset to seamlessly load the saved configuration as the default."
                       width: 200
                       opacity: 0
                   }
               }

               Row{
                   Layout.alignment: Qt.AlignCenter
                   spacing: 8

                   Button {
                       id: userDefinedConfig
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Save Configuration"
                       style: econButtonStyle
                       onClicked:{
                           setButtonClicked = true
                           see3camcu200mh01r1.saveConfiguration(See3CAM_CU200M_H01R1.USER_DEFINED)
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           see3camcu200mh01r1.saveConfiguration(See3CAM_CU200M_H01R1.USER_DEFINED)
                       }
                   }
                   Button {
                       id: resetDevice
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Device Reset"
                       style: econButtonStyle
                       onClicked:{
                           see3camcu200mh01r1.resetDevice()
                       }
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.resetDevice()
                       }
                   }
               }


               ColumnLayout{
                   Layout.alignment: Qt.AlignCenter
                   Button {
                       id: defaultConfig
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Default Configuration"
                       tooltip: "This control is designed to restore the factory-provided default configuration.
Upon activation, the device will undergo an automatic reset to seamlessly load and apply the default configuration settings."
                       style: econButtonStyle
                       onClicked:{
                           setButtonClicked = true
                           see3camcu200mh01r1.saveConfiguration(See3CAM_CU200M_H01R1.DEFAULT)
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           see3camcu200mh01r1.saveConfiguration(See3CAM_CU200M_H01R1.DEFAULT)
                       }
                   }
               }

               Row{
                   Button {
                       id: f_wversion_selected130
                       opacity: 1
                       activeFocusOnPress : true
                       action: firmwareVersion
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
                       action: serialNumber
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

               Row{
                   Layout.alignment: Qt.AlignCenter

                   Button {
                       id: ispFirmwareVersion
                       opacity: 1
                       action: ispVersion
                       text: "ISP Version"
                       activeFocusOnPress : true
                       tooltip: "Click to view the ISP firmware version"
                       style: econButtonStyle
                       Keys.onReturnPressed: {
                           see3camcu200mh01r1.readISPFirmwareVersion()
                       }
                   }
               }

               Row{
                   Layout.alignment: Qt.AlignCenter
                   Button {
                       id: defaultValue
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Default"
                       tooltip: "Default button is used to set all the extension unit controls to their default values."
                       style: econButtonStyle
                       action: setDefault
                       onClicked:{
                           setToDefaultValues()
                       }
                       Keys.onReturnPressed: {
                           setToDefaultValues()
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

    See3CAM_CU200M_H01R1{
        id:see3camcu200mh01r1

        onExposurePropertiesReceived: {
            currentExposureMode(mode, autoFeature, manualValue)
        }

        onTargetBrightnessPropertiesReceived: {
            skipUpdateTargetBrightness = false

            targetBrightness.minimumValue = min
            targetBrightness.maximumValue = max
            targetBrightness.stepSize = stepValue
            targetBrightness.value = current

            skipUpdateTargetBrightness = true
        }

        onAutoGainUpperLimitReceived:{
            skipUpdateExposureUpperLimit = false

            gainUpperLimitSlider.minimumValue = min
            gainUpperLimitSlider.maximumValue = max
            gainUpperLimitSlider.stepSize     = step
            gainUpperLimitSlider.value        = current

            skipUpdateExposureUpperLimit = true
        }

        onExposureCompensationReceived:{
            skipUpdateExposureCompensation = false
            expCompensationTextField.text = exposure
            skipUpdateExposureCompensation = true
        }

        onRoiAutoExpModeReceived: {
            currentROIAutoExposureMode(roiMode, winSize)
        }

        onAntiFlickerModeReceived: {
            getAntiFlickerModes(antiFlicker)
        }

        onBlacklevelPropertiesReceived: {
            blackLevelSlider.minimumValue = min
            blackLevelSlider.maximumValue = max
            blackLevelSlider.stepSize = stepValue
            blackLevelSlider.value = current
        }

        onCameraModeReceived:{
            getCameraModeReceived(cameraMode)
        }

        onStrobeModeReceived: {
            currentStrobeMode(strobeMode)
        }

        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorModeValues)
        }

        //For Read Statistics
        onExposureStatisticsReceived: {
            expStatTextField.text = exposure
        }

        onGainStatisticsReceived: {
            gainStatTextField.text = gain
        }

        onIndicateCommandStatus:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
            }
        }

        onIndicateExposureValueRangeFailure:{
            if(setButtonClicked){
                setButtonClicked = false

                displayMessageBox(title, text)
                see3camcu200mh01r1.getExposure()
                see3camcu200mh01r1.getAutoExposureLimit(expLowerLimitTextField.text, expUpperLimitTextField.text)
            }
        }

        onIndicateExposureCompensationRangeFailure:{
            if(setButtonClicked){
                setButtonClicked = false

                displayMessageBox(title, text)
                see3camcu200mh01r1.getExposureCompensation()
            }
        }

        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }
    }

    function setROIAutoExposure(){
        if(expRoiFull.checked == true){
            see3camcu200mh01r1.setROIAutoExposure(See3CAM_CU200M_H01R1.FULL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)

            //To disable comboBox in full roi mode
            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.opacity = 0.1
        }else if(expRoiManual.checked == true){
            see3camcu200mh01r1.setROIAutoExposure(See3CAM_CU200M_H01R1.MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)

            //To disable comboBox in full roi mode
            autoExpoWinSizeCombo.enabled = true
            autoExpoWinSizeCombo.opacity = 1
        }
    }

    function currentFlipMirrorMode(mode) {
        switch(mode) {
            case See3CAM_CU200M_H01R1.NORMAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = false
                root.getFlipStatus(false, false)
                break;
            case See3CAM_CU200M_H01R1.VERTICAL:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = false
                root.getFlipStatus(false, true)
                break;
            case See3CAM_CU200M_H01R1.HORIZONTAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = true
                root.getFlipStatus(true, false)
                break;
            case See3CAM_CU200M_H01R1.ROTATE_180:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = true
                root.getFlipStatus(false, false)
                break;
        }
    }

    function currentROIAutoExposureMode(roiMode, winSize) {
        switch(roiMode) {
            case See3CAM_CU200M_H01R1.FULL_ROI:
                expRoiFull.checked = true
                expRoiFull.enabled = true

                //To disable comboBox in full roi mode
                autoExpoWinSizeCombo.enabled = false
                autoExpoWinSizeCombo.opacity = 0.1
                break
            case See3CAM_CU200M_H01R1.MANUAL_ROI:
                skipUpdateUIOnExpWindowSize = false

                expRoiManual.enabled = true
                expRoiManual.checked = true

                //To enable comboBox in manual mode
                autoExpoWinSizeCombo.enabled = true
                autoExpoWinSizeCombo.opacity = 1
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
        }
    }

    function getAntiFlickerModes(antiFlicker) {
        switch(antiFlicker) {
            case See3CAM_CU200M_H01R1.MODE_50Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case See3CAM_CU200M_H01R1.MODE_60Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
            case See3CAM_CU200M_H01R1.MODE_DISABLE:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 2
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }


    function getCameraModeReceived(mode) {
        if(mode == See3CAM_CU200M_H01R1.MASTER_MODE) {
            masterMode.checked = true

            root.startUpdatePreviewInMasterMode()
            root.checkForTriggerMode(false)
            root.videoRecordBtnEnable(true)
            root.captureBtnEnable(true)
        }
        else if(mode = See3CAM_CU200M_H01R1.TRIGGER_MODE) {
            triggerMode.checked = true

            root.stopUpdatePreviewInTriggerMode()
            root.checkForTriggerMode(true)
            root.videoRecordBtnEnable(false)
            root.captureBtnEnable(false)
        }
    }

    function setMasterMode() {
        see3camcu200mh01r1.setCameraMode(See3CAM_CU200M_H01R1.MASTER_MODE)

        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function setTriggerMode() {
        see3camcu200mh01r1.setCameraMode(See3CAM_CU200M_H01R1.TRIGGER_MODE)

        root.stopUpdatePreviewInTriggerMode()
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function currentStrobeMode(mode) {
        switch(mode) {
            case See3CAM_CU200M_H01R1.STROBE_OFF:
                strobeOff.checked = true
                strobeOn.checked  = false
                break;
            case See3CAM_CU200M_H01R1.STROBE_ON:
                strobeOff.checked = false
                strobeOn.checked  = true
                break;
        }
    }

     function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){

            uvcAutoExpSelected = autoExposureSelect

            manualExposure.checked = false
            autoExposure.checked   = true

            expRoiManual.enabled   = true
            expRoiFull.enabled     = true

            expRoiManual.opacity   = 1
            expRoiFull.opacity     = 1

            if(expRoiManual.checked){
                autoExpoWinSizeCombo.enabled = true
                autoExpoWinSizeCombo.opacity = 1
            }else if(expRoiFull.checked){
                autoExpoWinSizeCombo.enabled = false
                autoExpoWinSizeCombo.opacity = 0.1
            }
        }else{
            uvcAutoExpSelected = autoExposureSelect

            manualExposure.checked = true
            autoExposure.checked   = false

            expRoiManual.enabled   = false
            expRoiFull.enabled     = false

            expRoiManual.opacity   = 0.1
            expRoiFull.opacity     = 0.1

            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.opacity = 0.1
        }
    }

    function currentExposureMode(mode, autoFeature, manualValue) {
        if(mode === See3CAM_CU200M_H01R1.AUTO_EXPOSURE){
            autoExposure.checked   = true
            manualExposure.checked = false
        }else if(mode === See3CAM_CU200M_H01R1.MANUAL_EXPOSURE){
            manualExposure.checked = true
            autoExposure.checked   = false
        }

        //Auto Exposure Features
        if(autoFeature === See3CAM_CU200M_H01R1.CONTINIOUS_EXPOSURE){
            exposureContinious.checked = true
            expSingleShotBtnClicked    = false
        }
        else if(autoFeature === See3CAM_CU200M_H01R1.SINGLE_SHOT_EXPOUSRE){
            exposureSingleShot.checked = true
            expSingleShotBtnClicked    = true
        }

        //Manual Exposure Values
        manualExpTextField.text = manualValue

        if(manualExpTextField.text <= 1000000){
            exposureInt = manualExpTextField.text / 100
            root.sendExposureToUVC(exposureInt)
        }
        else{
            root.sendExposureToUVC(100000)
        }
    }

    function sendExposureStatus(){
        if(autoExposure.checked == true){
            root.sendExposureStatusToUVC(true, 0)
        }else if(manualExposure.checked == true){
            root.sendExposureStatusToUVC(false, (manualExpTextField.text / 100))
        }
    }

    function setExposureProperties() {
        if(autoExposure.checked == true){
            root.sendExposureStatusToUVC(true, 0)
            if(exposureContinious.checked == true) {
                see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.CONTINIOUS_EXPOSURE, manualExpTextField.text)
            }
            else if(exposureSingleShot.checked == true) {
                see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.AUTO_EXPOSURE,See3CAM_CU200M_H01R1.SINGLE_SHOT_EXPOUSRE, manualExpTextField.text)
            }
        }else if(manualExposure.checked == true){
            root.sendExposureStatusToUVC(false, (manualExpTextField.text / 100))
            see3camcu200mh01r1.setExposure(See3CAM_CU200M_H01R1.MANUAL_EXPOSURE, 0, manualExpTextField.text)
        }
    }

    function setAntiFlicker() {
        if(antiFlickerCombo.currentIndex === 0)
            see3camcu200mh01r1.setAntiFlickerMode(See3CAM_CU200M_H01R1.MODE_50Hz)
        else if(antiFlickerCombo.currentIndex === 1)
            see3camcu200mh01r1.setAntiFlickerMode(See3CAM_CU200M_H01R1.MODE_60Hz)
        else if(antiFlickerCombo.currentIndex === 2)
            see3camcu200mh01r1.setAntiFlickerMode(See3CAM_CU200M_H01R1.MODE_DISABLE)
    }


    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function displayMessageBox(title, text) {
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function getISPFirmwareVersion(){
        see3camcu200mh01r1.readISPFirmwareVersion()
        messageDialog.open()
    }

    function setToDefaultValues() {
        defaultValue.enabled = false
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)

        if(see3camcu200mh01r1.setToDefault()) {
            getCurrentValuesFromCamera()
            //To send Exposure properties when set default is clicked
            sendExposureStatus()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera() {
        getCameraSettingsTimer.start()
        see3camcu200mh01r1.getBlackLevel()
        see3camcu200mh01r1.getExposure()
        see3camcu200mh01r1.getCameraMode()
        see3camcu200mh01r1.getStrobeMode()
        see3camcu200mh01r1.getOrientation()
        see3camcu200mh01r1.getAutoExpROIModeAndWindowSize()
        see3camcu200mh01r1.getAntiFlickerMode()
        see3camcu200mh01r1.getAutoGainUpperLimit()
    }
    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
}
