/*
 * see3camcu200.qml -- extension settings for SEE3CAM_CU200 camera
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
import econ.camera.see3camcu200 1.0
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item{

    Action {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }

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

    Connections{
        target: root

        onTakeScreenShot:
        {
            root.imageCapture(CommonEnums.SNAP_SHOT);
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

        //Signals getting values from UVC & set its values to the HID controls
        onSendGainValueToHID:{
            gainSlider.value = gainHid
        }
        onGetBrightnessFromUVC:{
            brightnessSlider.value = brightnessFromUVC
        }
        onGetContrastFromUVC:{
            contrastSlider.value = contrastFromUVC
        }
        onGetSaturationFromUVC:{
            saturationSlider.value = saturationFromUVC
        }
        onGetGammaFromUVC:{
            gammaCorrectionSlider.value = gammaFromUVC
        }
        onGetColorTempFromUVC:{
            skipUpdateColorTemperature = false
            colorTempTextField.text = colorTempFromUVC

            if(colorTempFromUVC == "2300")
            {
               colorTempSlider.value = 0
            }
            else if(colorTempFromUVC == "2800")
            {
                colorTempSlider.value = 1
            }
            else if(colorTempFromUVC == "3000")
            {
                colorTempSlider.value = 2
            }
            else if(colorTempFromUVC == "4000")
            {
                colorTempSlider.value = 3
            }
            else if(colorTempFromUVC == "4100")
            {
                colorTempSlider.value = 4
            }
            else if(colorTempFromUVC == "6000")
            {
                colorTempSlider.value = 5
            }
            else if(colorTempFromUVC == "6500")
            {
                colorTempSlider.value = 6
            }

            skipUpdateColorTemperature = true

            //To get color correction matrix and RB gain after color temperature is set
            see3camcu200.getColorCorrectionMatrix()
            see3camcu200.getRBGain()
        }
        onGetExposureFromUVC:{
            manualExpTextField.text = exposureFromUVC
        }
        onGetFlipMode:{ //To get the Flip status when 2 or more cameras are connected
            sendFlipStatus()
        }
    }

    property real adjustedRGain: 0.0
    property real adjustedBGain: 0.0
    property real adjustedBrightness: 0.0
    property real adjustedSaturation: 0.0
    property real adjustedGammaCorrection: 0.0
    property real spinBoxMin: -3.99
    property real spinBoxMax: 3.99
    property real spinBoxStepSize: 0.005

    property int expMin: 100
    property int expMax: 1000000

    property bool skipUpdateGainMode            : false
    property bool skipUpdateBlackLevelMode      : false
    property bool skipUpdateBrightness          : false
    property bool skipUpdateContrast            : false
    property bool skipUpdateSaturation          : false
    property bool skipUpdateColorTemperature    : false
    property bool skipUpdateGammaCorrection     : false
    property bool setButtonClicked              : false

    property bool skipUpdateRGainMode           : false
    property bool skipUpdateBGainMode           : false


    property int brightnessInt: 0
    property int saturationInt: 0
    property int gammaInt: 0
    property int exposureInt: 0
    property int exposureText: 0

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle
        Item{
            height: 1600

            ColumnLayout{
                x:2
                y:5
                spacing:18

                Text
                {
                    id: brightnessText
                    text: "--- Brightness ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the Y channel gain in steps of 0.005 by changing the brightness of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: brightnessSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            brightnessTextField.text = brightnessSlider.value
                            if(skipUpdateBrightness){
                                // Round the slider and TextField to three decimal places
                                adjustedBrightness = parseFloat((brightnessSlider.value).toFixed(3));

                                //Sending HID value to UVC
                                brightnessInt = adjustedBrightness * 200;
                                root.sendBrightnessToUVC(brightnessInt)

                                brightnessTextField.text = adjustedBrightness
                                see3camcu200.setBrightness(adjustedBrightness)
                            }
                            skipUpdateBrightness = true
                        }
                    }
                    TextField {
                        id: brightnessTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: brightnessSlider.minimumValue; top: brightnessSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                brightnessSlider.value = brightnessTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: contrastTitle
                    text: "--- Contrast ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the strengths of S curve applied to Y channel by changing the contrast of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: contrastSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            contrastTextField.text = contrastSlider.value
                            if(skipUpdateContrast){
                                //Sending HID value to UVC
                                root.sendContrastToUVC(contrastSlider.value)

                                see3camcu200.setContrast(contrastSlider.value)
                            }
                            skipUpdateContrast = true
                        }
                    }
                    TextField {
                        id: contrastTextField
                        text: contrastSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: contrastSlider.minimumValue; top: contrastSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                contrastSlider.value = contrastTextField.text
                            }
                        }
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
                                see3camcu200.setBlackLevel(blackLevelSlider.value)
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

                Text
                {
                    id: gammaCorrectionText
                    text: "--- Gamma Correction ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the strength of gamma correction curve applied to Y channel in steps of 0.1 by changing the gamma of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: gammaCorrectionSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        onValueChanged:  {
                            if(skipUpdateGammaCorrection){
                                gammaCorrectionTextField.text = gammaCorrectionSlider.value
                                // Round the slider and TextField to three decimal places
                                adjustedGammaCorrection = parseFloat((gammaCorrectionSlider.value).toFixed(1));

                                //Sending HID value to UVC
                                gammaInt = adjustedGammaCorrection * 10
                                root.sendGammaToUVC(gammaInt)

                                gammaCorrectionTextField.text = adjustedGammaCorrection
                                see3camcu200.setGammaCorrection(adjustedGammaCorrection)
                            }
                            skipUpdateGammaCorrection = true
                        }
                    }
                    TextField {
                        id: gammaCorrectionTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        readOnly:true
                        validator: IntValidator {bottom: gammaCorrectionSlider.minimumValue; top: gammaCorrectionSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                gammaCorrectionSlider.value = gammaCorrectionTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: exposureMode
                    text: "--- Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "It controls the integration time of the sensor. It changes in the range of 100µs to 1s. It is expressed in microseconds(µs)."
                        opacity: 0
                        width: 200
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
                        opacity: 1
                    }
                    TextField
                    {
                        id: manualExpTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        opacity: 1
                        style: econTextFieldStyle
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: expMin; top: expMax}
                    }
                    Button
                    {
                        id: exposureSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: true
                        opacity: 1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            exposureSetBtn.enabled = false
                            setButtonClicked = true

                            see3camcu200.setExposure(SEE3CAM_CU200.MANUAL_EXPOSURE, manualExpTextField.text);

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

                            see3camcu200.setExposure(SEE3CAM_CU200.MANUAL_EXPOSURE, manualExpTextField.text);
                            exposureSetBtn.enabled = true
                        }
                    }
                }

                //                Text {
                //                    id: gainModeTitle
                //                    text: "--- Gain Mode ---"
                //                    font.pixelSize: 14
                //                    font.family: "Ubuntu"
                //                    color: "#ffffff"
                //                    smooth: true
                //                    Layout.alignment: Qt.AlignCenter
                //                    opacity: 0.50196078431373
                //                }
                //                Row{
                //                    spacing: 30
                //                    Layout.alignment: Qt.AlignCenter
                //                    ExclusiveGroup { id: gainModeGroup }
                //                    RadioButton {
                //                        id: autoGain
                //                        exclusiveGroup: gainModeGroup
                //                        checked: false
                //                        text: "Auto"
                //                        activeFocusOnPress: true
                //                        style: econRadioButtonStyle
                //                        onClicked: {
                //                            see3camcu200.setGainMode(SEE3CAM_CU200.AUTO_MODE, 0)
                //                        }
                //                        Keys.onReturnPressed: {
                //                            see3camcu200.setGainMode(SEE3CAM_CU200.AUTO_MODE, 0)
                //                        }
                //                    }

                //                    RadioButton {
                //                        id: manualGain
                //                        exclusiveGroup: gainModeGroup
                //                        checked: false
                //                        text: "Manual"
                //                        activeFocusOnPress: true
                //                        style: econRadioButtonStyle
                //                        onClicked: {
                //                            see3camcu200.setGainMode(SEE3CAM_CU200.MANUAL_MODE, gainSlider.value)
                //                        }
                //                        Keys.onReturnPressed: {
                //                            see3camcu200.setGainMode(SEE3CAM_CU200.MANUAL_MODE, gainSlider.value)
                //                        }
                //                    }
                //                }


                Text{
                     id: manualGainSlider
                     text: "--- Gain ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     Layout.alignment: Qt.AlignCenter
                     opacity: 0.50196078431373

                     ToolButton{
                         tooltip: "It is used to modify the gain value of the sensor."
                         width: 200
                         opacity: 0
                     }
                }
                Row
                {
                     spacing: 35
                     Slider
                     {
                         id: gainSlider
                         activeFocusOnPress: true
                         updateValueWhileDragging: false
                         width: 150
                         style:econSliderStyle
 //                        opacity: (manualGain.enabled && manualGain.checked) ? 1 : 0.1
 //                        enabled: (manualGain.enabled && manualGain.checked) ? 1 : 0.1
                         onValueChanged:  {
                             //Sending HID value to UVC
                             root.getGainValueFromHID(gainSlider.value)

                             gainTextField.text = gainSlider.value
                             if(skipUpdateGainMode){
                                 see3camcu200.setGainMode(SEE3CAM_CU200.MANUAL_MODE, gainSlider.value)
                             }
                             skipUpdateGainMode = true
                         }
                     }
                     TextField
                     {
                         id: gainTextField
                         text: gainSlider.value
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
 //                        opacity: (manualGain.enabled && manualGain.checked) ? 1 : 0.1
 //                        enabled: (manualGain.enabled && manualGain.checked) ? 1 : 0.1
                         style: econTextFieldStyle
                         validator: IntValidator {bottom: gainSlider.minimumValue; top: gainSlider.maximumValue}
                         onTextChanged: {
                             if(text.length > 0){
                                 gainSlider.value = gainTextField.text
                             }
                         }
                     }
                 }

                Text{
                     id: gainRTitle
                     text: "--- Red Gain ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     Layout.alignment: Qt.AlignCenter
                     opacity: 0.50196078431373

                     ToolButton{
                         tooltip: "Modifies the digital gain of R channel in steps of 0.005."
                         width: 200
                         opacity: 0
                     }
                }
                Row
                {
                     spacing: 35
                     Slider
                     {
                         id: gainRSlider
                         activeFocusOnPress: true
                         updateValueWhileDragging: false
                         width: 150
                         style:econSliderStyle
                         onValueChanged:  {
                             gainRTextField.text = gainRSlider.value
                             if(skipUpdateRGainMode){
                                 adjustedRGain = parseFloat((gainRSlider.value).toFixed(3));
                                 gainRTextField.text = adjustedRGain
                                 see3camcu200.setRBGain(adjustedRGain, gainBSlider.value)
                             }
                             skipUpdateRGainMode = true
                         }
                     }
                     TextField
                     {
                         id: gainRTextField
                         text:gainRSlider.value
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         validator: IntValidator {bottom: gainRSlider.minimumValue; top: gainRSlider.maximumValue}
                         onTextChanged: {
                             if(text.length > 0){
                                 gainRSlider.value = gainRTextField.text
                             }
                         }
                     }
                 }

                Text{
                     id: gainBTitle
                     text: "--- Blue Gain ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     Layout.alignment: Qt.AlignCenter
                     opacity: 0.50196078431373

                     ToolButton{
                         tooltip: "Modifies the digital gain of B channel in steps of 0.005."
                         width: 200
                         opacity: 0
                     }
                }

                Row
                {
                     spacing: 35
                     Slider
                     {
                         id: gainBSlider
                         activeFocusOnPress: true
                         updateValueWhileDragging: false
                         width: 150
                         style:econSliderStyle
                         onValueChanged:  {
                             gainBTextField.text = gainBSlider.value
                             if(skipUpdateBGainMode){
                                 adjustedBGain = parseFloat((gainBSlider.value).toFixed(3));
                                 gainBTextField.text = adjustedBGain
                                 see3camcu200.setRBGain(gainRSlider.value, adjustedBGain)
                             }
                             skipUpdateBGainMode = true
                         }
                     }
                     TextField
                     {
                         id: gainBTextField
                         text:gainBSlider.value
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         validator: IntValidator {bottom: gainBSlider.minimumValue; top: gainBSlider.maximumValue}
                         onTextChanged: {
                             if(text.length > 0){
                                 gainBSlider.value = gainBTextField.text
                             }
                         }

                     }
                 }

                Text
                {
                    id: saturationTitle
                    text: "--- Saturation ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Modifies the gain of Z curve applied to UV channel in steps of 0.005 by changing the saturation of the frame."
                        width: 200
                        opacity: 0
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: saturationSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle

                        onValueChanged:  {
                            saturationTextField.text = saturationSlider.value
                            if(skipUpdateSaturation){
                                // Round the slider and TextField to three decimal places
                                adjustedSaturation = parseFloat((saturationSlider.value).toFixed(3));

                                //Sending HID value to UVC
                                saturationInt = adjustedSaturation * 200
                                root.sendSaturationToUVC(saturationInt)

                                saturationTextField.text = adjustedSaturation
                                see3camcu200.setSaturation(adjustedSaturation)
                            }
                            skipUpdateSaturation = true
                        }
                    }
                    TextField {
                        id: saturationTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: saturationSlider.minimumValue; top: saturationSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                saturationSlider.value = saturationTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: colorTempTitle
                    text: "--- Colour Temperature ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "It is used for white balancing the scene based on the temperature set by applying predefined R and B digital gain.
 Note:
 For temperature other than the possible values (2300, 2800, 3000, 4000, 4100, 6000, 6500), individual R and B digital gain can be manually adjusted."
                        width: 200
                        opacity: 0
                    }
                }

                Row{
                    spacing: 35
                    Slider {
                        id: colorTempSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle

                        minimumValue: 0
                        maximumValue: 6
                        stepSize: 1

                        onValueChanged:  {

                            if(skipUpdateColorTemperature){

                                switch (value) {
                                    case 0:
                                        root.sendColorTemperatureToUVC(1)
                                        colorTempTextField.text = "2300"
                                        see3camcu200.setColorTemperature(2300)

                                        break;
                                    case 1:
                                        root.sendColorTemperatureToUVC(2)
                                        colorTempTextField.text = "2800"
                                        see3camcu200.setColorTemperature(2800)

                                        break;
                                    case 2:
                                        root.sendColorTemperatureToUVC(3)
                                        colorTempTextField.text = "3000"
                                        see3camcu200.setColorTemperature(3000)

                                        break;
                                    case 3:
                                        root.sendColorTemperatureToUVC(4)
                                        colorTempTextField.text = "4000"
                                        see3camcu200.setColorTemperature(4000)

                                        break;
                                    case 4:
                                        root.sendColorTemperatureToUVC(5)
                                        colorTempTextField.text = "4100"
                                        see3camcu200.setColorTemperature(4100)

                                        break;
                                    case 5:
                                        root.sendColorTemperatureToUVC(6)
                                        colorTempTextField.text = "6000"
                                        see3camcu200.setColorTemperature(6000)

                                        break;
                                    case 6:
                                        root.sendColorTemperatureToUVC(7)
                                        colorTempTextField.text = "6500"
                                        see3camcu200.setColorTemperature(6500)

                                        break;
                                }

                                //To get color correction matrix and RB gain after color temperature is set
                                see3camcu200.getColorCorrectionMatrix()
                                see3camcu200.getRBGain()
                            }

                            skipUpdateColorTemperature = true
                        }
                    }
                    TextField {
                        id: colorTempTextField
                        text: colorTempSlider.value
                        readOnly: true
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle

                    }
                }

                Text {
                    id: colorCorrectionMaxTitle
                    text: "--- Colour Correction Matrix ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373

                    ToolButton{
                        tooltip: "It is used for applying manual 3x3 matrix to RGB channel in steps of 0.005.
 For Manual Colour Temperature, the Colour Correction Matrix is predefined and it can overwritten by using this control."
                        width: 200
                        opacity: 0
                    }
                }


                Row{
                    spacing: 10
                    Layout.alignment: Qt.AlignCenter
                    SpinBox{
                        id:spinBoxRr
                        decimals: 3
                    }
                    SpinBox{
                        id:spinBoxRg
                        decimals: 3
                    }
                    SpinBox{
                        id:spinBoxRb
                        decimals: 3
                    }
                }

                Row{
                    spacing: 10
                    Layout.alignment: Qt.AlignCenter
                    SpinBox{
                        id:spinBoxGr
                        decimals: 3
                    }
                    SpinBox{
                        id:spinBoxGg
                        decimals: 3
                    }
                    SpinBox{
                        id:spinBoxGb
                        decimals: 3
                    }
                }

                Row{
                    spacing: 10
                    Layout.alignment: Qt.AlignCenter
                    SpinBox{
                        id:spinBoxBr
                        decimals: 3
                    }
                    SpinBox{
                        id:spinBoxBg
                        decimals: 3
                    }
                    SpinBox{
                        id:spinBoxBb
                        decimals: 3
                    }
                }

                Button
                {
                    id: colorMatrixSetBtn
                    activeFocusOnPress : true
                    Layout.alignment: Qt.AlignCenter
                    text: "Set"
                    tooltip: "You can set the required colour correction matrix elements by changing the value in the SpinBox and click the Set button"
                    style: econButtonStyle
                    enabled: true
                    opacity: 1
                    implicitHeight: 25
                    implicitWidth: 60
                    onClicked:
                    {
                        see3camcu200.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)
                    }
                    Keys.onReturnPressed:
                    {
                        see3camcu200.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)
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
                    CheckBox {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        tooltip: "Flips the preview left/right."
                        onClicked:{
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
                        }
                        Keys.onReturnPressed: {
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        tooltip: "Flips the preview vertically up/down."
                        onClicked:{
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
                        }
                        Keys.onReturnPressed: {
                            see3camcu200.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            sendFlipStatus()
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
                      tooltip: " After choosing master mode, the application starts video streaming. This is a simple mode of operation for the camera without any external trigger capability."
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
                      tooltip: "In Trigger mode, Frames will be out only when external hardware pulses are given to PIN1 of CN4."
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
                       tooltip: "When this control is enabled, the LED is ON for each frame’s exposure time while Video Streaming."
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_ON)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_ON)
                       }
                   }

                   RadioButton {
                       id: strobeOff
                       exclusiveGroup: strobesGrp
                       checked: false
                       text: "Off"
                       tooltip: "It disables strobe control."
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_OFF)
                       }
                       Keys.onReturnPressed: {
                           see3camcu200.setStrobeMode(SEE3CAM_CU200.STROBE_OFF)
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
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.USER_DEFINED)
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.USER_DEFINED)
                       }
                   }
                   Button {
                       id: resetDevice
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Device Reset"
                       style: econButtonStyle
                       onClicked:{
                           see3camcu200.resetDevice()
                       }
                       Keys.onReturnPressed: {
                           see3camcu200.resetDevice()
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
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.DEFAULT)
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           see3camcu200.saveConfiguration(SEE3CAM_CU200.DEFAULT)
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

    SEE3CAM_CU200{
        id:see3camcu200

        onGainModeReceived: {
//            if(mode == SEE3CAM_CU200.AUTO_MODE)
//            {
//                autoGain.checked = true
//            }
//            else if(mode == SEE3CAM_CU200.MANUAL_MODE)
//            {
//                manualGain.checked = true
//            }
        }

        onGainPropertiesReceived:{
            skipUpdateGainMode = false
            gainSlider.minimumValue = min
            gainSlider.value        = current
            gainSlider.stepSize     = stepValue
            gainSlider.maximumValue = max
            skipUpdateGainMode = true
        }

        onRGainPropertiesReceived: {
            skipUpdateRGainMode = false
            gainRSlider.minimumValue = minRGain
            gainRSlider.maximumValue = maxRGain
            gainRSlider.stepSize = rGainStepValue

            currentRGain = parseFloat((currentRGain).toFixed(3));
            gainRSlider.value   = currentRGain
            gainRTextField.text = currentRGain

            skipUpdateRGainMode = true
        }

        onBGainPropertiesReceived: {
            skipUpdateBGainMode = false
            gainBSlider.minimumValue = minBGain
            gainBSlider.maximumValue = maxBGain
            gainBSlider.stepSize = bGainStepValue

            currentBGain = parseFloat((currentBGain).toFixed(3));
            gainBSlider.value   = currentBGain
            gainBTextField.text = currentBGain

            skipUpdateBGainMode = true
        }


        onCurrentBlackLevelReceived: {
            blackLevelSlider.value = currentBlackLevel
        }
        onMinBlackLevelReceived:{
            blackLevelSlider.minimumValue = minBlackLevel
        }
        onMaxBlackLevelReceived:{
            blackLevelSlider.maximumValue = maxBlackLevel
        }
        onBlackLevelStepValueReceived:{
            blackLevelSlider.stepSize = blackLevelStepValue
        }

        onBrightnessPropertiesReceived: {
            skipUpdateBrightness = false

            brightnessSlider.minimumValue = minBrightness
            brightnessSlider.maximumValue = maxBrightness
            brightnessSlider.stepSize = stepValue

            currentBrightness = parseFloat((currentBrightness).toFixed(3));
            brightnessSlider.value = currentBrightness
            brightnessTextField.text = currentBrightness

            adjustedBrightness = parseFloat((brightnessSlider.value).toFixed(3));

            //Sending HID value to UVC
            brightnessInt = adjustedBrightness * 200;
            root.sendBrightnessToUVC(brightnessInt)

            skipUpdateBrightness = true
        }

        onSaturationPropertiesReceived: {
            skipUpdateSaturation = false
            saturationSlider.minimumValue = minSaturation
            saturationSlider.maximumValue = maxSaturation
            saturationSlider.stepSize = stepValue

            currentSaturation = parseFloat((currentSaturation).toFixed(3));
            saturationSlider.value = currentSaturation
            saturationTextField.text = currentSaturation

            adjustedSaturation = parseFloat((saturationSlider.value).toFixed(3));

            //Sending HID value to UVC
            saturationInt = adjustedSaturation * 200
            root.sendSaturationToUVC(saturationInt)

            skipUpdateSaturation = true
        }


        onGammaPropertiesReceived: {
            skipUpdateGammaCorrection = false

            gammaCorrectionSlider.minimumValue = minGamma
            gammaCorrectionSlider.maximumValue = maxGamma
            gammaCorrectionSlider.stepSize = stepValue

            currentGamma = parseFloat((currentGamma).toFixed(1));
            gammaCorrectionSlider.value = currentGamma
            gammaCorrectionTextField.text = currentGamma

            adjustedGammaCorrection = parseFloat((gammaCorrectionSlider.value).toFixed(1));

            //Sending HID value to UVC
            gammaInt = adjustedGammaCorrection * 10
            root.sendGammaToUVC(gammaInt)

            skipUpdateGammaCorrection = true
        }


        onCurrentContrastReceived: {
            skipUpdateContrast = false
            contrastSlider.value = currentContrast
            root.sendContrastToUVC(contrastSlider.value)
            skipUpdateContrast = true
        }
        onMinContrastReceived: {
            contrastSlider.minimumValue = minContrast
        }
        onMaxContrastReceived: {
            contrastSlider.maximumValue = maxContrast
        }
        onContrastStepValueReceived: {
            contrastSlider.stepSize = stepValue
        }

        onColorTemperatureReceived: {
            skipUpdateColorTemperature = false
            colorTempTextField.text = colorTemp

            if(colorTemp == "2300")
            {
               colorTempSlider.value = 0
               root.sendColorTemperatureToUVC(1)
            }
            else if(colorTemp == "2800")
            {
                colorTempSlider.value = 1
                root.sendColorTemperatureToUVC(2)
            }
            else if(colorTemp == "3000")
            {
                colorTempSlider.value = 2
                root.sendColorTemperatureToUVC(3)
            }
            else if(colorTemp == "4000")
            {
                colorTempSlider.value = 3
                root.sendColorTemperatureToUVC(4)
            }
            else if(colorTemp == "4100")
            {
                colorTempSlider.value = 4
                root.sendColorTemperatureToUVC(5)
            }
            else if(colorTemp == "6000")
            {
                colorTempSlider.value = 5
                root.sendColorTemperatureToUVC(6)
            }
            else if(colorTemp == "6500")
            {
                colorTempSlider.value = 6
                root.sendColorTemperatureToUVC(7)
            }

            skipUpdateColorTemperature = true
        }

        onCameraModeReceived:{
            getCameraModeReceived(cameraMode)
        }

        onStrobeModeReceived: {
            currentStrobeMode(strobeMode)
        }

        onExposureValueReceived: {
            manualExpTextField.text = exposure

            if(manualExpTextField.text <= 1000000)
            {
                exposureInt = manualExpTextField.text / 100
                root.sendExposureToUVC(exposureInt)
            }
            else{
                root.sendExposureToUVC(100000)
            }
        }


        onCurrentRrValuesReceived: {

            spinBoxRr.minimumValue = spinBoxMin
            spinBoxRr.maximumValue = spinBoxMax
            spinBoxRr.stepSize     = spinBoxStepSize
            spinBoxRr.value        = currentRr
        }
        onCurrentRgValuesReceived: {
            spinBoxRg.minimumValue = spinBoxMin
            spinBoxRg.maximumValue = spinBoxMax
            spinBoxRg.stepSize     = spinBoxStepSize
            spinBoxRg.value        = currentRg
        }
        onCurrentRbValuesReceived: {
            spinBoxRb.minimumValue = spinBoxMin
            spinBoxRb.maximumValue = spinBoxMax
            spinBoxRb.stepSize     = spinBoxStepSize
            spinBoxRb.value        = currentRb
        }


        onCurrentGrValuesReceived:{
            spinBoxGr.minimumValue = spinBoxMin
            spinBoxGr.maximumValue = spinBoxMax
            spinBoxGr.stepSize     = spinBoxStepSize
            spinBoxGr.value        = currentGr
        }
        onCurrentGgValuesReceived:{
            spinBoxGg.minimumValue = spinBoxMin
            spinBoxGg.maximumValue = spinBoxMax
            spinBoxGg.stepSize     = spinBoxStepSize
            spinBoxGg.value        = currentGg
        }
        onCurrentGbValuesReceived:{
            spinBoxGb.minimumValue = spinBoxMin
            spinBoxGb.maximumValue = spinBoxMax
            spinBoxGb.stepSize     = spinBoxStepSize
            spinBoxGb.value        = currentGb
        }


        onCurrentBrValuesReceived:{
            spinBoxBr.minimumValue = spinBoxMin
            spinBoxBr.maximumValue = spinBoxMax
            spinBoxBr.stepSize     = spinBoxStepSize
            spinBoxBr.value        = currentBr
        }
        onCurrentBgValuesReceived:{
            spinBoxBg.minimumValue = spinBoxMin
            spinBoxBg.maximumValue = spinBoxMax
            spinBoxBg.stepSize     = spinBoxStepSize
            spinBoxBg.value        = currentBg
        }
        onCurrentBbValuesReceived:{
            spinBoxBb.minimumValue = spinBoxMin
            spinBoxBb.maximumValue = spinBoxMax
            spinBoxBb.stepSize     = spinBoxStepSize
            spinBoxBb.value        = currentBb
        }

        onFlipMirrorModeChanged:
        {
            currentFlipMirrorMode(flipMirrorModeValues)
        }

        onIndicateCommandStatus:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
            }
        }

        onIndicateExposureValueRangeFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
                see3camcu200.getExposure()
            }
        }
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case SEE3CAM_CU200.NORMAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = false
                root.getFlipStatus(false, false)
                break;
            case SEE3CAM_CU200.VERTICAL:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = false
                root.getFlipStatus(false, true)
                break;
            case SEE3CAM_CU200.HORIZONTAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = true
                root.getFlipStatus(true, false)
                break;
            case SEE3CAM_CU200.ROTATE_180:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = true
                root.getFlipStatus(false, false)
                break;
        }
    }

    function sendFlipStatus()
    {
        if((flipCtrlVertical.checked) && (flipCtrlHorizotal.checked))
        {
            root.getFlipStatus(true, true)
        }
        else if(!(flipCtrlVertical.checked) && (flipCtrlHorizotal.checked))
        {
            root.getFlipStatus(true, false)
        }
        else if((flipCtrlVertical.checked) && !(flipCtrlHorizotal.checked))
        {
            root.getFlipStatus(false, true)
        }
        else if(!(flipCtrlVertical.checked) && !(flipCtrlHorizotal.checked))
        {
            root.getFlipStatus(false, false)
        }
    }


    function getCameraModeReceived(mode)
    {
        if(mode == SEE3CAM_CU200.MASTER_MODE)
        {
            masterMode.checked = true

            root.startUpdatePreviewInMasterMode()
            root.checkForTriggerMode(false)
            root.videoRecordBtnEnable(true)
            root.captureBtnEnable(true)
        }
        else if(mode = SEE3CAM_CU200.TRIGGER_MODE)
        {
            triggerMode.checked = true

            root.stopUpdatePreviewInTriggerMode()
            root.checkForTriggerMode(true)
            root.videoRecordBtnEnable(false)
            root.captureBtnEnable(false)
        }
    }


    function setMasterMode()
    {
        see3camcu200.setCameraMode(SEE3CAM_CU200.MASTER_MODE)

        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function setTriggerMode()
    {
        see3camcu200.setCameraMode(SEE3CAM_CU200.TRIGGER_MODE)

        root.stopUpdatePreviewInTriggerMode()
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function currentStrobeMode(mode){
        switch(mode){
            case SEE3CAM_CU200.STROBE_OFF:
                strobeOff.checked   = true
                strobeOn.checked = false
                break;
            case SEE3CAM_CU200.STROBE_ON:
                strobeOff.checked   = false
                strobeOn.checked = true
                break;
        }
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function setToDefaultValues(){
        defaultValue.enabled = false //To avoid multiple clicks over Default button
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)

        if(see3camcu200.setToDefault())
        {
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        see3camcu200.getGainMode()
        see3camcu200.getRBGain()
        see3camcu200.getColorCorrectionMatrix()
        see3camcu200.getBlackLevel()
        see3camcu200.getBrightness()
        see3camcu200.getContrast()
        see3camcu200.getSaturation()
        see3camcu200.getColorTemperature()
        see3camcu200.getGammaCorrection()
        see3camcu200.getExposure()
        see3camcu200.getCameraMode()
        see3camcu200.getStrobeMode()
        see3camcu200.getOrientation()
    }
    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
}
