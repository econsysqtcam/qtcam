/*
 * imagequalitysettings.qml -- display camera settings
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

//Added by Dhurka - 20th Oct 2016
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import cameraenum 1.0
import "../JavaScriptFiles/tempValue.js" as JS

Item {
    id: videoFilter

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

    property bool brightValueChangeProperty
    property bool contrastValueChangeProperty
    property bool saturationValueChangeProperty
    property bool panValueChangeProperty
    property bool tiltValueChangeProperty
    property bool zoomValueChangeProperty
    property bool wbValueChangeProperty
    property bool wbAutoChangeProperty
    property bool sharpValueChangeProperty
    property bool gainValueChangeProperty
    property bool gammaValueChangeProperty
    property bool hueValueChangeProperty
    property bool ledModeChangeProperty
    property bool exposureValueChangeProperty
    property bool bcklightValueChangeProperty
    property bool rawBitsValueChangeProperty
    property bool ledFreqValueChangeProperty
    property bool focusLogitechValueChangeProperty
    property bool focusValueChangeProperty
    property bool focusAutoChangeProperty
    property bool usb3speed: false
    property bool enablePowerLineFreq : true
    property bool hdrModeSelected : false
    property bool autoWhiteBalanceSelect : false
    property bool colorTempEnabled : false

    property bool powerLineComboEnable
    // Skip doing things when exposure combo index changed calls when no selection of any camera
    property bool exposureComboEnable
    property bool ledModeComboEnable
    property variant exposureOrigAscella: [10, 20, 39, 78, 156, 312, 625, 1250, 2500, 5000, 10000, 20000]
    property int expAscellaTxtFiledValue;
    property bool exposureSliderSetEnable;
    property bool exposureAutoAvailable: false
    property var menuitems:[]

    property real adjustedBrightness

    property int adjustedExposure
    property int exposureInt
    property int seconds : 0
    property int milliSeconds : 0
    property int microSeconds : 0

    // It needs some time to get exposure control correct index value recently set in image quality settings when selecting camera in UI.
    Timer {
        id: queryctrlTimer
        repeat :false
        interval: 500
        onTriggered: {
        // Adding flag to skip setting exposure auto and manual value when getting exposure value and update UI and enable back after getting all control values.
            exposureSliderSetEnable = false
            exposureComboEnable = false
            root.cameraFilterControls(true)
            exposureComboEnable = true
            exposureSliderSetEnable = true
            stop()
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

    Action {
        id: videoControl
        onTriggered: {
            videoControlFilter()
        }
    }

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
            onVisibleChanged:
            {
                root.videoCaptureChildVisible = visible
                root.imageSettingVisibleChanged();
                if(root.selectedDeviceEnumValue == CommonEnums.ECAM82_USB && visible)
                {
                    root.cameraFilterControls(true)
                }

                /*
                  Once the Image quality settings is visible, It will sets the white balance mode using
                  the "autoWhiteBalanceSelect" flag emitted from the extension settings
                */
                if(root.selectedDeviceEnumValue === CommonEnums.SEE3CAM_CU200 && visible){
                    if(autoWhiteBalanceSelect){
                        autoSelect_wb.checked = true
                    }else {
                        autoSelect_wb.checked = false
                    }
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
                        style:econSliderStyle
                        onValueChanged: {
                            if(brightValueChangeProperty) {
                                //Sending UVC value to HID
                                adjustedBrightness = (brightness_Slider.value) / 200.0;
                                root.getBrightness(adjustedBrightness)

                                root.logInfo("Brightness changed to: "+ value.toString())
                                root.changeCameraSettings(brightnessControlId,value.toString())
                            }
                        }
                    }
                    TextField {
                        id: brightness_value
                        text: brightness_Slider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        enabled: false
                        horizontalAlignment: TextInput.AlignHCenter
                        validator: IntValidator {bottom: brightness_Slider.minimumValue; top: brightness_Slider.maximumValue;}
                        opacity: 0
                        style: econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (brightness_Slider.maximumValue.toString().length > brightness_Slider.minimumValue.toString().length) ? brightness_Slider.maximumValue.toString().length : brightness_Slider.minimumValue.toString().length
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
                        style:econSliderStyle
                        opacity: enabled ? 1 : 0.1
                        onValueChanged: {
                            if(contrastValueChangeProperty) {
                                //Sending UVC value to HID
                                root.getContrast(contrast_Slider.value)

                                root.changeCameraSettings(contrastControlId,value.toString())
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
                        enabled: false
                        style: econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (contrast_Slider.maximumValue.toString().length > contrast_Slider.minimumValue.toString().length) ? contrast_Slider.maximumValue.toString().length : contrast_Slider.minimumValue.toString().length
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(saturationValueChangeProperty) {
                                //Sending UVC value to HID
                                var adjustedSaturation = (saturation_Slider.value) / 200.0;
                                root.getSaturation(adjustedSaturation)

                                root.logInfo("Saturation changed to: "+ value.toString())
                                root.changeCameraSettings(saturationControlId,value.toString())
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
                        enabled: false
                        style: econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (saturation_Slider.maximumValue.toString().length > saturation_Slider.minimumValue.toString().length) ? saturation_Slider.maximumValue.toString().length : saturation_Slider.minimumValue.toString().length
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
                        style:econSliderStyle
                        onValueChanged:  {
                            if(panValueChangeProperty) {
                                root.logInfo("Pan changed to: "+ value.toString())
                                root.changeCameraSettings(panControlId,value.toString())
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
                        enabled: false
                        style: econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (pan_Slider.maximumValue.toString().length > pan_Slider.minimumValue.toString().length) ? pan_Slider.maximumValue.toString().length : pan_Slider.minimumValue.toString().length
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
                        style:econSliderStyle
                        onValueChanged:  {
                            if(tiltValueChangeProperty) {
                                root.logInfo("Tilt changed to: "+ value.toString())
                                root.changeCameraSettings(tiltControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (tilt_Slider.maximumValue.toString().length > tilt_Slider.minimumValue.toString().length) ? tilt_Slider.maximumValue.toString().length : tilt_Slider.minimumValue.toString().length
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
                        style:econSliderStyle
                        onValueChanged:  {
                            if(zoomValueChangeProperty) {
                                root.logInfo("zoom changed to: "+ value.toString())
                                root.changeCameraSettings(zoomControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (zoom_Slider.maximumValue.toString().length > zoom_Slider.minimumValue.toString().length) ? zoom_Slider.maximumValue.toString().length : zoom_Slider.minimumValue.toString().length
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
                                if(checked)
                                {
                                    if(wbAutoChangeProperty){
                                        root.logInfo("White Balance set to Auto Mode")
                                        root.changeCameraSettings(whiteBalanceControl_auto_Id,1)
                                    }
                                    root.autoWhiteBalanceSelected(true)
                                    white_balance_Slider.opacity = 0.1
                                    white_balance_Slider.enabled = false
                                    wbAutoChangeProperty = true
                                    autoWhiteBalanceSelect = true

                                    colorTempEnabled = true
                                }
                                else
                                {
                                    if(root.selectedDeviceEnumValue === CommonEnums.SEE3CAM_CU200){
                                        if(colorTempEnabled){
                                            root.logInfo("White Balance set to Manual Mode")
                                            root.autoWhiteBalanceSelected(false)
                                            if(wbAutoChangeProperty){
                                                root.changeCameraSettings(whiteBalanceControl_auto_Id,0)
                                                // When click hardware default, selecting manual whitebalance, set to default wb.
                                                root.changeCameraSettings(whiteBalanceControlId,white_balance_Slider.value.toString())
                                            }
                                            white_balance_Slider.opacity = 1
                                            white_balance_Slider.enabled = true
                                            autoWhiteBalanceSelect = false

                                        } else {
                                            white_balance_Slider.opacity = 1
                                            white_balance_Slider.enabled = true
                                        }
                                    } else {
                                        root.logInfo("White Balance set to Manual Mode")
                                        root.autoWhiteBalanceSelected(false)
                                        if(wbAutoChangeProperty){
                                            root.changeCameraSettings(whiteBalanceControl_auto_Id,0)
                                            // When click hardware default, selecting manual whitebalance, set to default wb.
                                            root.changeCameraSettings(whiteBalanceControlId,white_balance_Slider.value.toString())
                                        }
                                        if(root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM){
                                            white_balance_Slider.opacity = 1
                                            white_balance_Slider.enabled = true
                                        }
                                        autoWhiteBalanceSelect = false
                                    }
                                }
                            }
                        }
                    }
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: white_balance_Slider
                        enabled: false
                        opacity: enabled ? 1 : 0.1
                        width: 110
                        style:econSliderStyle
                        onValueChanged: {
                            if(wbValueChangeProperty) {
                                if(!autoSelect_wb.checked) {
                                    //Sending UVC value to HID
                                    convertWhiteBalanceToColorTemperature(white_balance_Slider.value)

                                    root.logInfo("White Balance changed to: "+ value.toString())
                                    root.changeCameraSettings(whiteBalanceControlId,value.toString())
                                    root.manualWbSliderValueChanged()
                                } else {
                                    if(white_balance.opacity == 1)
                                    {
                                        white_balance_Slider.enabled = true
                                    }
                                    else{
                                        white_balance_Slider.enabled = false
                                    }

                                }
                            }
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
                        opacity:  white_balance_Slider.enabled ? 1: 0
                        enabled: false
                        style: econTextFieldStyle
                        maximumLength: (white_balance_Slider.maximumValue.toString().length > white_balance_Slider.minimumValue.toString().length) ? white_balance_Slider.maximumValue.toString().length : white_balance_Slider.minimumValue.toString().length
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
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: gamma_Slider
                        width: 110
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(gammaValueChangeProperty){
                                //Sending UVC value to HID
                                var adjustedGamma = (gamma_Slider.value) / 10.0;
                                root.getGamma(adjustedGamma)

                                root.changeCameraSettings(gammaControlId,value.toString())
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
                        enabled: false
                        style: econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (gamma_Slider.maximumValue.toString().length > gamma_Slider.minimumValue.toString().length) ? gamma_Slider.maximumValue.toString().length : gamma_Slider.minimumValue.toString().length
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(sharpValueChangeProperty) {
                                root.logInfo("Sharpness settings changed to: "+ value.toString())
                                root.changeCameraSettings(sharpnessControlId,value.toString())
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
                        enabled: false
                        style: econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (sharpness_Slider.maximumValue.toString().length > sharpness_Slider.minimumValue.toString().length) ? sharpness_Slider.maximumValue.toString().length : sharpness_Slider.minimumValue.toString().length

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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(gainValueChangeProperty) {
                                //Signal to send UVC gain value to HID
                                root.getGainValueFromUVC(gain_Slider.value)
                                root.logInfo("Gain settings changed to: "+ value.toString())
                                root.changeCameraSettings(gainControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (gain_Slider.maximumValue.toString().length > gain_Slider.minimumValue.toString().length) ? gain_Slider.maximumValue.toString().length : gain_Slider.minimumValue.toString().length
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(hueValueChangeProperty) {
                                root.logInfo("Hue settings changed to: "+ value.toString())
                                root.changeCameraSettings(hueControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (hue_Slider.maximumValue.toString().length > hue_Slider.minimumValue.toString().length) ? hue_Slider.maximumValue.toString().length : hue_Slider.minimumValue.toString().length
                    }
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
                                root.selectMenuIndex(powerLineComboControlId,currentIndex)
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
                            // Skip doing things when exposure combo index changed calls when no selection of any camera
                            if(exposureComboEnable){
                                root.selectMenuIndex(exposureAutoControlId,currentIndex)
                                //To set Auto/Manual exposure mode
                                if(currentText.toString() != "Auto Mode" && currentText.toString() != "Aperture Priority Mode") {
                                    root.changeCameraSettings(exposurecontrolId,exposure_Slider.value.toString())
                                    root.autoExposureSelected(false)
                                    JS.autoExposureSelected = false
                                    exposure_absolute.opacity = 1
                                    exposure_Slider.opacity = 1
                                    exposure_Slider.enabled = true
                                    exposure_value.opacity = 1
                                } else {
                                    root.autoExposureSelected(true)
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
                        property var exposureValueAscella
                        updateValueWhileDragging: false
                        id: exposure_Slider
                        width: 110
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            //Sending UVC value to HID
                            adjustedExposure = exposure_Slider.value * 100
                            root.getExposureUVC(adjustedExposure)

                            exposureInt = parseInt(exposure_Slider.value)

                            seconds = exposureInt / 1000000
                            milliSeconds = (exposureInt/1000) - (seconds * 1000)
                            microSeconds = exposureInt - ((seconds * 1000000) + (milliSeconds * 1000))

                            if((exposureCombo.currentText == "Manual Mode") && (root.selectedDeviceEnumValue == CommonEnums.CX3_UVC_CAM)){
                                exposureValueAscella = exposureOrigAscella[value]
                                exposure_value.text = exposureOrigAscella[value]
                                root.changeCameraSettings(exposurecontrolId, exposureValueAscella)

                            }else if(!exposureAutoAvailable){ // If a camera does not contain "exposure, auto" control, but having "exposure absolute" control, allow it change exposure value.
                                root.changeCameraSettings(exposurecontrolId,value.toString())
                            }else{
                                if((exposureCombo.currentText == "Shutter Priority Mode" || exposureCombo.currentText == "Manual Mode")) {
                                    root.getExposureComponentsUVC(seconds, milliSeconds, microSeconds)

                                    if(exposureSliderSetEnable){
                                        root.changeCameraSettings(exposurecontrolId,value.toString())
                                    }
                                }
                            }
                        }
                    }
                    TextField {
                        id: exposure_value
                        property int expLocalVal
                        property int expLocalval1
                        text: (root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM) ? exposure_Slider.value : ""
                        validator: IntValidator {bottom: (root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM) ? exposure_Slider.minimumValue : 10; top: (root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM) ? exposure_Slider.maximumValue : 20000;}
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        opacity: 0
                        style:econTextFieldStyle
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (exposure_Slider.maximumValue.toString().length > exposure_Slider.minimumValue.toString().length) ? exposure_Slider.maximumValue.toString().length : exposure_Slider.minimumValue.toString().length
                        onTextChanged: {
                             if(exposure_value.text.length > 0){
                                     if(root.selectedDeviceEnumValue == CommonEnums.CX3_UVC_CAM){
                                     maximumLength = 5
                                     expLocalval1 = text.toString()
                                     expLocalVal = expLocalval1
                                     for(var i=0; i<exposureOrigAscella.length; i++){
                                         if(expLocalVal == exposureOrigAscella[i]){
                                             expAscellaTxtFiledValue = text.toString()
                                             exposure_Slider.value = exposureOrigAscella.indexOf(expAscellaTxtFiledValue)
                                             break
                                         }
                                     }
                                 }
                            }
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
                                    root.logInfo("enable exposure auto priority")
                                    root.changeCameraSettings(exposureAutoPriorityControlId,1)
                                } else {
                                    root.logInfo("disable exposure auto priority")
                                    root.changeCameraSettings(exposureAutoPriorityControlId,0)
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(bcklightValueChangeProperty) {
                                root.logInfo("Backlight compensation settings changed to: "+ value.toString())
                                root.changeCameraSettings(backLightCompensationId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (backLight_Slider.maximumValue.toString().length > backLight_Slider.minimumValue.toString().length) ? backLight_Slider.maximumValue.toString().length : backLight_Slider.minimumValue.toString().length
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(rawBitsValueChangeProperty) {
                                root.logInfo("raw bits per pixel settings changed to: "+ value.toString())
                                root.changeCameraSettings(rawBitsControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (rawBitsSlider.maximumValue.toString().length > rawBitsSlider.minimumValue.toString().length) ? rawBitsSlider.maximumValue.toString().length : rawBitsSlider.minimumValue.toString().length
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
                                root.changeCameraSettings(ledModeComboControlId,currentIndex.toString())
                            }
                        }
                    }
                    Image {
                        source: "images/pline_dropdown.png"
                        opacity: 0
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(ledFreqValueChangeProperty) {
                                root.logInfo("led frequency settings changed to: "+ value.toString())
                                root.changeCameraSettings(ledFreqControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (ledFreqSlider.maximumValue.toString().length > ledFreqSlider.minimumValue.toString().length) ? ledFreqSlider.maximumValue.toString().length : ledFreqSlider.minimumValue.toString().length
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
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged: {
                            if(focusLogitechValueChangeProperty) {
                                root.logInfo("focus settings for logitech camera changed to: "+ value.toString())
                                root.changeCameraSettings(focusLogitechControlId,value.toString())
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
                        // Added by Sankari: 25 May 2017. To fix issue of accepting multiple number of digits[ex:00000000000000]
                        maximumLength: (focusLogitechSlider.maximumValue.toString().length > focusLogitechSlider.minimumValue.toString().length) ? focusLogitechSlider.maximumValue.toString().length : focusLogitechSlider.minimumValue.toString().length
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
                                    root.logInfo("Disable video processing")
                                    root.changeCameraSettings(disableVideoControlId,1)
                                } else {
                                    root.logInfo("Enable video processing")
                                    root.changeCameraSettings(disableVideoControlId,0)
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
                                    root.autoFocusSelected(true)
                                    root.logInfo("Focus control set in Auto Mode")
                                    focus_Slider.opacity = 0.1
                                    focus_Slider.enabled = false
                                    focus_value.opacity = 0
                                    if(focusAutoChangeProperty){
                                        root.changeCameraSettings(focusControlAutoId,1)
                                    }
                                } else {
                                    JS.autoFocusChecked = false
                                    root.autoFocusSelected(false)
                                    root.logInfo("Focus control set in Manual Mode")
                                    focus_Slider.opacity = 1
                                    focus_Slider.enabled = true
                                    focus_value.opacity = 1
                                    if(focusAutoChangeProperty){
                                        root.changeCameraSettings(focusControlAutoId,0)
                                        root.changeCameraSettings(focusControlId,focus_Slider.value)
                                    }
                                }
				focusAutoChangeProperty = true
                            }
                        }
                    }
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: focus_Slider
                        width: 110
                        style:econSliderStyle
                        opacity: enabled ? 1 : 0.1
                        onValueChanged: {
                            if(focusValueChangeProperty) {
                                if(!autoSelect_focus.checked ) {
                                    root.changeCameraSettings(focusControlId,value.toString())
                                }
                            }
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
                        maximumLength: (focus_Slider.maximumValue.toString().length > focus_Slider.minimumValue.toString().length) ? focus_Slider.maximumValue.toString().length : focus_Slider.minimumValue.toString().length
                    }

                    Image {
                        source: "images/pline_dropdown.png"
                        opacity: 0
                        //Just for layout purpose
                    }

                    Button {
                        opacity: 1
                        id: hardwareDef
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
                        onClicked: {
                            // Added by Sankari:23 Dec 2016 . To avoid double clicks event
                            hardwareDef.enabled = false
                            root.cameraFilterControls(false)
                            hardwareDef.enabled = true
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

    Connections
    {
        target: root
        // Enable Image Quality settings after capturing image
        onImageQualitySettingsEnable:
        {
            videoFilter.enabled = enableStatus
            videoFilter.opacity = enableStatus ? 1 : 0.5
        }
        onSidebarVisibleStatus:
        {
            videoFilter.visible = status;
        }

        //Added by Sushanth - Signals getting values from HID settings and set it to UVC
        onSendGainValueToUVC:{
            gain_Slider.value = gain
        }
        onGetBrightnessFromHID:{
            brightness_Slider.value = brightnessFromHID
        }
        onGetContrastFromHID:{
            contrast_Slider.value = contrastFromHID
        }
        onGetSaturationFromHID:{
            saturation_Slider.value = saturationFromHID
        }
        onGetGammaFromHID:{
            gamma_Slider.value = gammaFromHID
        }
        onGetColorTempFromHID:{
            white_balance_Slider.value = colorTempFromHID
        }
        onGetExposureStatusFromHID:{
            if(isAutoEnable)
            {
                exposureCombo.currentIndex = 0
            }
            else
            {
                exposureCombo.currentIndex = 1
                exposure_Slider.value = exposure
            }
        }
        onGetExposureFromHID:{
            exposure_Slider.value = exposureFromHID
        }

        onDisableUVCSettings:{
            white_balance_Slider.enabled = false
            white_balance_Slider.opacity = 0.1
        }
        onGetWhiteBalanceModeFromHID:{
            //To store the Enable/Disable status of White balance mode
            if(isAutoEnabled){
                autoWhiteBalanceSelect = true
            }else {
                autoWhiteBalanceSelect = false
            }

            //To store the Enable/Disable status of color temperature
            colorTempEnabled = isColorTempEnabled
        }
    }

    Connections
    {
        target: root
        onCameraSettingsTabEnable:
        {
            videoFilter.visible = status;
        }

        onDisableManualExp:
        {
            if(!isAutoExpSelected)
            {
                if(exposureCombo.currentText.toString() =="Auto Mode" || exposureCombo.currentText.toString() =="Aperture Priority Mode")
                {
                    if(exposureCombo.currentIndex == 0)
                       exposureCombo.currentIndex = 1
                    else
                       exposureCombo.currentIndex = 0
                }
            }
            else
            {
                if(exposureCombo.currentText.toString()=="Manual Mode" || exposureCombo.currentText.toString() == "Shutter Priority Mode")
                {
                    if(exposureCombo.currentIndex == 0)
                        exposureCombo.currentIndex = 1
                    else
                        exposureCombo.currentIndex = 0
                }
            }
        }
        onDisableAutoFocus:
        {
            if(isAutoFocusSelected)
            {
                autoSelect_focus.checked = true
                JS.autoFocusChecked = true
                root.autoFocusSelected(true)
                root.logInfo("Focus control set in Auto Mode")
                focus_Slider.opacity = 0.1
                focus_Slider.enabled = false
                focus_value.opacity = 0.1
            }
            else
            {
                autoSelect_focus.checked = false
                JS.autoFocusChecked = false
                root.autoFocusSelected(false)
                root.logInfo("Focus control set in Manual Mode")
                focus_Slider.opacity = 1
                focus_Slider.enabled = true
                focus_value.opacity = 1
            }
        }

        onDisableAwb:
        {
            root.cameraFilterControls(true)           //getting the control values to update awb manual slider
            if(isAwbSelected)
            {
                autoSelect_wb.checked = true
                JS.autoWhiteBalSelected = true
                root.autoWhiteBalanceSelected(true)
                root.logInfo("White Balance set to Auto Mode")
                white_balance_Slider.opacity = 0.1
                white_balance_Slider.enabled = false
            }
            else
            {
                autoSelect_wb.checked = false
                JS.autoWhiteBalSelected = false
                root.autoWhiteBalanceSelected(false)
                root.logInfo("White Balance set to Manual Mode")
                white_balance_Slider.opacity = 1
                white_balance_Slider.enabled = true
            }
        }

    }
    Connections
    {
        target:root
        onSetControlValues:
        {
            setCameraControls(controlName,controlType,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue,controlID);
        }
    }
    Connections
    {
        target:root
        onDisableImageSettings:
        {
            setOpacityFalse();
        }
    }
    Connections
    {
        target:root
        onVideoCaptureFilterChildVisible:
        {
            video_capture_filter_Child.visible = visibleStatus
        }
    }
    Connections
    {
        target:root
        onCameraControlPropertyChange:
        {
            usb3speed = false
            brightValueChangeProperty = false
            contrastValueChangeProperty = false
            saturationValueChangeProperty = false
            panValueChangeProperty = false
            tiltValueChangeProperty = false
            zoomValueChangeProperty = false
            wbValueChangeProperty = false
            wbAutoChangeProperty = false
            sharpValueChangeProperty = false
            gainValueChangeProperty = false
            gammaValueChangeProperty = false
            hueValueChangeProperty = false
            ledModeChangeProperty = false
            exposureValueChangeProperty = false
            bcklightValueChangeProperty = false
            rawBitsValueChangeProperty = false
            ledFreqValueChangeProperty = false
            focusLogitechValueChangeProperty = false
            focusValueChangeProperty = false
            focusAutoChangeProperty = false
            powerLineComboEnable = false
	    // Skip doing things when exposure combo index changed calls when no selection of any camera
            exposureComboEnable = false
            ledModeComboEnable = false
        }
    }
    Connections
    {
        target:root
        onAddAutoModeMenuItem:
        {
            if(root.selectedDeviceEnumValue == CommonEnums.CX3_UVC_CAM){
                root.cameraFilterControls(true)
                if(!usb3speed){
                    menuitems.push("Auto Mode")
                    exposureCombo.model = menuitems
                    menuitems.pop()
                }
            }
        }
        onQueryUvcControls:{
            queryctrlTimer.start()
        }

    }

    function convertWhiteBalanceToColorTemperature(whiteBalance){
        if(root.selectedDeviceEnumValue == CommonEnums.SEE3CAM_CU200){
            root.getColorTemperature(whiteBalance)
        }else if (root.selectedDeviceEnumValue == CommonEnums.SEE3CAM_50CUG){
            switch(whiteBalance)
            {
                 case 1:
                     root.getColorTemperature(2300)
                     break
                 case 2:
                     root.getColorTemperature(2800)
                     break
                 case 3:
                     root.getColorTemperature(3000)
                     break
                 case 4:
                     root.getColorTemperature(4000)
                     break
                 case 5:
                     root.getColorTemperature(6000)
                     break
                 case 6:
                     root.getColorTemperature(6500)
                     break
            }
        }
    }

    function setCameraControls(controlName,controlType,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue,controlID)
    {
        switch(controlType)
        {
        case 0:
            break;
        case 1:
            imageSettingsFirstLevel(controlName,controlType,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue,controlID);
            break;
        case 2:
            imageSettingsSecondLevel(controlName,controlType,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue,controlID);
            break;
        case 3:
            exposureAutoUIUpdate(controlName,controlID,controlMinValue,controlMaxValue,controlDefaultValue)
            break;
        case 9:
            break;
        }
    }

    //Modified by Sushanth - To check the controls with respective control IDs to make platform independant
    function imageSettingsFirstLevel(controlName,controlType,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue,controlID)
    {
        switch(controlID)
        {
            case 9963776://Brightness
                brightnessUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963777://Contrast
                contrastUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963778://Saturation
                saturationUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 10094856://Pan(Absolute)
                panUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 10094857://Tilt(Absolute)
                tiltUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 10094861://Zoom(Absolute)
                zoomUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963779://Hue
                hueUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963802://Manual White Balance(Temperature)
                whiteBalanceUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963792://Gamma
                gammaUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963795://Gain
                gainUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963803://Sharpness
                sharpnessUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 10094850://Exposure(Absolute)
                exposureAbsoluteUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 9963804://Backlight Compensation
                backLightUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case 10094860://Focus,Auto
                focusUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
        }

        switch(controlName)
        {
            case "Raw bits per pixel":
                rawBitsUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case "LED1 Frequency":
                ledFrequencyUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
            case "LED1 Mode":
                ledModeUIUpdate(controlID,controlMinValue,controlMaxValue,controlDefaultValue)
                break;
            case "Focus (absolute)":
                focusAbsoluteUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
                break;
        }
    }

    //Modified by Sushanth - To check the controls with respective control IDs to make platform independant
    function imageSettingsSecondLevel(controlName,controlType,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue,controlID)
    {
        switch(controlID)
        {
            case 9963788://White Balance, Auto
                whiteBalAutoUIUpdate(controlID,controlDefaultValue)
                break;
            case 10094860://Focus, Auto
                autoFocusUIUpdate(controlID,controlDefaultValue)
                break;
            case 10094851://Exposure (Auto,Priority)
                exposureAutoAvailable = true; // Make it true if exposure auto control is available
                if(root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM)
                {
                    exposureAutoPriorityUIUpdate(controlID,controlDefaultValue)
                }
                break;
        }

        if(controlName == "Disable video processing")
        {
            videoProcessUIUpdate(controlID,controlDefaultValue)
        }
    }
    function brightnessUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        brightness.opacity = 1
        brightness_Slider.enabled = true
        brightness_value.opacity = 1
        brightnessControlId = controlID
        brightness_Slider.minimumValue = controlMinValue
        brightness_Slider.maximumValue = controlMaxValue
        brightness_Slider.value = controlDefaultValue
        brightness_Slider.stepSize = controlStepSize
    }
    function contrastUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        contrast.opacity = 1
        contrast_Slider.enabled = true
        contrast_value.opacity = 1
        contrastControlId = controlID
        contrast_Slider.minimumValue = controlMinValue
        contrast_Slider.maximumValue = controlMaxValue
        contrast_Slider.value = controlDefaultValue
        contrast_Slider.stepSize = controlStepSize
    }
    function saturationUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        saturation.opacity = 1
        saturation_Slider.enabled = true
        saturation_value.opacity = 1
        saturationControlId = controlID
        saturation_Slider.minimumValue = controlMinValue
        saturation_Slider.maximumValue = controlMaxValue
        saturation_Slider.value = controlDefaultValue
        saturation_Slider.stepSize = controlStepSize
    }
    function panUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        pan.opacity = 1
        pan_Slider.enabled = true
        pan_value.opacity = 1
        panControlId = controlID
        pan_Slider.minimumValue = controlMinValue
        pan_Slider.maximumValue = controlMaxValue
        pan_Slider.value = controlDefaultValue
        pan_Slider.stepSize = controlStepSize
    }
    function tiltUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        tilt.opacity = 1
        tilt_Slider.enabled = true
        tilt_value.opacity = 1
        tiltControlId = controlID
        tilt_Slider.minimumValue = controlMinValue
        tilt_Slider.maximumValue = controlMaxValue
        tilt_Slider.value = controlDefaultValue
        tilt_Slider.stepSize = controlStepSize
    }
    function zoomUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        zoom.opacity = 1
        zoom_Slider.enabled = true
        zoom_value.opacity = 1
        zoomControlId = controlID
        zoom_Slider.minimumValue = controlMinValue
        zoom_Slider.maximumValue = controlMaxValue

        zoom_Slider.value = controlDefaultValue
        zoom_Slider.stepSize = controlStepSize
    }
    function hueUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        hue.opacity = 1
        hue_Slider.enabled = true
        hue_value.opacity = 1
        hueControlId = controlID
        hue_Slider.minimumValue = controlMinValue
        hue_Slider.maximumValue = controlMaxValue
        hue_Slider.value = controlDefaultValue
        hue_Slider.stepSize = controlStepSize
    }
    function ledModeUIUpdate(controlID,controlMinValue,controlMaxValue,controlDefaultValue)
    {
        ledModeText.opacity = 1
        ledModeComboControlId = controlID
        ledModeComboEnable = true
        ledModeCombo.enabled = true
        ledModeCombo.opacity = 1
        ledModeCombo.currentIndex = controlDefaultValue
    }

    //Manual White Balance
    function whiteBalanceUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        white_balance.opacity = 1

        //Added by Sushanth - To enable manual white balance when the firmware exclusively supports manual white balance alone.
        if(((autoSelect_wb.enabled == false) || ((autoSelect_wb.checked == false) && (autoSelect_wb.enabled == true ))) && (root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM))
        {
            JS.autoWhiteBalSelected = false             // manual white balance selected
            white_balance_Slider.enabled = true
            white_balance_Slider.opacity = 1
        }
        else{
            JS.autoWhiteBalSelected = true              // auto white balance selected
            white_balance_Slider.enabled = false
            white_balance_Slider.opacity = 0.1
        }

        whiteBalanceControlId = controlID
        white_balance_Slider.minimumValue = controlMinValue
        white_balance_Slider.maximumValue = controlMaxValue
        white_balance_Slider.value = controlDefaultValue
        white_balance_Slider.stepSize = controlStepSize
    }

    //Auto White Balance
    function whiteBalAutoUIUpdate(controlID,controlDefaultValue)
    {
        white_balance.opacity = 1
        autoSelect_wb.opacity = 1
        autoSelect_wb.enabled = true
        autoSelect_wb.checked = controlDefaultValue
        whiteBalanceControl_auto_Id = controlID

        if(!autoSelect_wb.checked && root.selectedDeviceEnumValue != CommonEnums.CX3_UVC_CAM) {
            white_balance_Slider.enabled = true
            JS.autoWhiteBalSelected = false             // manual white balance selected
        }else{
            JS.autoWhiteBalSelected = true              // auto white balance selected
        }
    }

    function gammaUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        gamma.opacity = 1
        gamma_Slider.enabled = true
        gamma_value.opacity = 1
        gammaControlId = controlID
        gamma_Slider.minimumValue = controlMinValue
        gamma_Slider.maximumValue = controlMaxValue
        gamma_Slider.value = controlDefaultValue
        gamma_Slider.stepSize = controlStepSize
    }

    function gainUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        gain.opacity = 1
        gain_Slider.enabled = true
        gain_value.opacity = 1
        gainControlId = controlID
        gain_Slider.minimumValue = controlMinValue
        gain_Slider.maximumValue = controlMaxValue
        gain_Slider.value = controlDefaultValue
        gain_Slider.stepSize = controlStepSize
    }

    function sharpnessUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        sharpness.opacity = 1
        sharpness_Slider.enabled = true
        sharpness_value.opacity = 1
        sharpnessControlId = controlID
        sharpness_Slider.minimumValue = controlMinValue
        sharpness_Slider.maximumValue = controlMaxValue
        sharpness_Slider.value = controlDefaultValue
        sharpness_Slider.stepSize = controlStepSize
    }
    function exposureAbsoluteUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        if(!exposureAutoAvailable){ // If a camera does not contain "exposure, auto" control, but having "exposure absolute" control, allow it change exposure value.
            exposure_Slider.opacity = 1
            exposure_Slider.enabled = true
            exposure_value.opacity = 1
            exposure_absolute.opacity = 1
        }
        exposurecontrolId = controlID
        if(root.selectedDeviceEnumValue === CommonEnums.CX3_UVC_CAM)
        {       // For ascella camera, mapped exposure values to slider values 0 to 11
            if(exposureCombo.currentText == "Auto Mode"){
                exposure_absolute.opacity = 0.1
                exposure_Slider.enabled = false
            }else{
                exposure_absolute.opacity = 1
                exposure_Slider.enabled = true
            }
            usb3speed = true
            exposure_Slider.minimumValue = 0
            exposure_Slider.maximumValue = 11
            exposure_Slider.value = exposureOrigAscella.indexOf(controlDefaultValue)
            exposure_value.text = exposureOrigAscella[exposure_Slider.value]
        }
        else{
            exposure_Slider.minimumValue = controlMinValue
            exposure_Slider.maximumValue = controlMaxValue
            exposure_Slider.value = controlDefaultValue
            exposure_Slider.stepSize = controlStepSize
        }
    }
    function focusAbsoluteUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        focusauto.opacity = 1
        focus_Slider.opacity = 1      //Edited by M.Vishnu Murali: Removed "if" condition as the control wont be enabled if device has only manual focus.
        focus_Slider.enabled = true
        focus_value.opacity = 1
        focusControlId = controlID
        focus_Slider.minimumValue = controlMinValue
        focus_Slider.maximumValue = controlMaxValue
        focus_Slider.value = controlDefaultValue
        focus_Slider.stepSize = controlStepSize
    }
    function backLightUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        backLightCompensation.opacity = 1
        backLight_Slider.enabled = true
        backLight_value.opacity = 1
        backLightCompensationId = controlID
        backLight_Slider.minimumValue = controlMinValue
        backLight_Slider.maximumValue = controlMaxValue
        backLight_Slider.value = controlDefaultValue
        backLight_Slider.stepSize = controlStepSize
    }
    function rawBitsUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        rawBits.opacity = 1
        rawBitsSlider.enabled = true
        rawBits_value.opacity = 1
        rawBitsControlId = controlID
        rawBitsSlider.minimumValue = controlMinValue
        rawBitsSlider.maximumValue = controlMaxValue
        rawBitsSlider.value = controlDefaultValue
        rawBitsSlider.stepSize = controlStepSize
    }
    function ledFrequencyUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        ledFrequency.opacity = 1
        ledFreqSlider.enabled = true
        ledFreq_value.opacity = 1
        ledFreqControlId = controlID
        ledFreqSlider.minimumValue = controlMinValue
        ledFreqSlider.maximumValue = controlMaxValue
        ledFreqSlider.value = controlDefaultValue
        ledFreqSlider.stepSize = controlStepSize
    }
    function focusUIUpdate(controlID,controlMinValue,controlMaxValue,controlStepSize,controlDefaultValue)
    {
        focusLogitech.opacity = 1
        focusLogitechSlider.enabled = true
        focusLogitech_value.opacity = 1
        focusLogitechSlider.minimumValue = controlMinValue
        focusLogitechSlider.maximumValue = controlMaxValue
        focusLogitechSlider.value = controlDefaultValue
        focusLogitechControlId = controlID
        focusLogitechSlider.stepSize = controlStepSize
    }

    function autoFocusUIUpdate(controlID,controlDefaultValue)
    {
        autoSelect_focus.opacity = 1
        autoSelect_focus.enabled = true
        autoSelect_focus.checked = controlDefaultValue
        focusControlAutoId = controlID
        if(!autoSelect_focus.checked)
        {
            JS.autoFocusChecked = false
            focus_Slider.opacity = 1
            focus_Slider.enabled = true
            focus_value.opacity = 1
        }
        else
        {
            focus_Slider.opacity = 0.1
            focus_Slider.enabled = false
            focus_value.opacity = 0.1
        }
    }
    function exposureAutoPriorityUIUpdate(controlID,controlDefaultValue)
    {
        exposureAutoPriority.opacity = 1
        exposureAutoPriorityCheck.opacity = 1
        exposureAutoPriorityCheck.enabled = true
        exposureAutoPriorityCheck.checked = controlDefaultValue
        exposureAutoPriorityControlId = controlID
    }
    function videoProcessUIUpdate(controlID,controlDefaultValue)
    {
        disableVideoProcess.opacity = 1
        disableVideoProcessCheck.opacity = 1
        disableVideoProcessCheck.enabled = true
        disableVideoProcessCheck.checked = controlDefaultValue
        disableVideoControlId = controlID
    }

    //Modified by Sushanth - To check the controls with respective control IDs to make platform independant
    function exposureAutoUIUpdate(controlName,controlID,controlMinValue,controlMaxValue,controlDefaultValue)
    {
        menuitems.push(controlName)
        //enumerateControls - To allow only when all the controls listed in menulist model
        if((controlID === 9963800) && (enumerateControls))//PowerLine Frequency
        {
            enumerateControls = false
            if(enablePowerLineFreq){
                menuitems.pop() //Control Name should be removed
                powerLine.opacity = 1
                powerLineCombo.opacity = 1
                powerLineComboEnable =  false	    // To avoid setting power line freq when get the control values
                powerLineCombo.model = menuitems
                powerLineCombo.currentIndex = controlDefaultValue;
                while(menuitems.pop()){}
                powerLineComboControlId = controlID
                powerLineComboEnable =  true
                if(!hardwareDef.enabled)
                    root.selectMenuIndex(powerLineComboControlId,controlDefaultValue)
            }
            else
            {
                menuitems.pop()
                powerLineCombo.model = menuitems
                  while(menuitems.pop()){}
                powerLineCombo.enabled = false
            }
        }//enumerateControls - To allow only when all the controls listed in menulist model
        else if((controlID === 10094849) && (enumerateControls))//Exposure, Auto
        {
            enumerateControls = false
            menuitems.pop()
            exposureAutoAvailable = true;
            if(root.selectedDeviceEnumValue == CommonEnums.CX3_UVC_CAM && !usb3speed){
                while(menuitems.pop()){}
            }
            exposure_auto.opacity = 1
            exposureCombo.opacity = 1
            exposureCombo.model = menuitems // On index changed is called for every time.
            while(menuitems.pop()){}
            exposureAutoControlId = controlID
            exposureCombo.currentIndex = controlDefaultValue

            //getting auto or manual exposure mode
            if(exposureCombo.currentText.toString()=="Manual Mode" || exposureCombo.currentText.toString()=="Shutter Priority Mode")
            {
                //Added By Sushanth
                //To disable Exposure compensation when application opens (Manual Mode)
                root.autoExposureSelected(false)

                JS.autoExposureSelected = false
                exposure_absolute.opacity = 1
                exposure_Slider.enabled = true
                exposure_Slider.opacity = 1
                exposure_value.opacity = 1
            }
            else
            {
                //Added By Sushanth
                //To Enable Exposure compensation when application opens (Auto Mode)
                root.autoExposureSelected(true)

                JS.autoExposureSelected = true
                exposure_Slider.enabled = false
                exposure_Slider.opacity = 0.1
                exposure_value.opacity = 0
            }
        }
    }

    function controlPowerLineFreq()
    {
        enablePowerLineFreq = false
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
        //powerLineCombo.model = menuitems
        //exposureCombo.model = menuitems
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

    function videoControlFilter() {
        if(!video_capture_filter_Child.visible) {
            root.logInfo("Video Capture Filter Selected");
            brightValueChangeProperty = true
            brightness_Slider.forceActiveFocus()
            contrastValueChangeProperty = true
            saturationValueChangeProperty = true
            panValueChangeProperty = true
            tiltValueChangeProperty = true
            zoomValueChangeProperty = true
            wbValueChangeProperty = true
            wbAutoChangeProperty = true
            sharpValueChangeProperty = true
            gainValueChangeProperty = true
            gammaValueChangeProperty = true
            hueValueChangeProperty = true
            ledModeChangeProperty = true
            exposureValueChangeProperty = true
            bcklightValueChangeProperty = true
            rawBitsValueChangeProperty = true
            ledFreqValueChangeProperty = true
            focusLogitechValueChangeProperty = true
            focusValueChangeProperty = true
            focusAutoChangeProperty = true
            video_capture_filter_Child.visible = true
	    // To avoid setting exposure when get the control values
	    exposureComboEnable = false
            root.enableCameraControls();
	    exposureComboEnable = true
        } else {
            video_capture_filter_Child.visible = false
            video_capture_filter_Child.focus = false
        }
    }
}
