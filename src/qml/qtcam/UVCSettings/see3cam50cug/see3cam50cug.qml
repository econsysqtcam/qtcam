/*
 * see3cam_50cug.qml -- extension settings for SEE3CAM_50CUG camera
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
import econ.camera.see3cam50cug 1.0
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
            skipUpdateBrightness = false
            brightnessSlider.value = brightnessFromUVC
            see3cam50cug.setBrightness(brightnessFromUVC)
            skipUpdateBrightness = true
        }
        onGetContrastFromUVC:{
            contrastSlider.value = contrastFromUVC
        }
        onGetSaturationFromUVC:{
            skipUpdateSaturation = false
            saturationSlider.value = saturationFromUVC
            see3cam50cug.setSaturation(saturationFromUVC)
            skipUpdateSaturation = true
        }
        onGetGammaFromUVC:{
            gammaCorrectionSlider.value = gammaFromUVC
        }
        onGetColorTempFromUVC:{
            skipUpdateColorTemperature = false
            colorTempTextField.text = colorTempFromUVC

            if(colorTempFromUVC == "2300"){
               colorTempSlider.value = 0
            }else if(colorTempFromUVC == "2800"){
                colorTempSlider.value = 1
            }else if(colorTempFromUVC == "3000"){
                colorTempSlider.value = 2
            }else if(colorTempFromUVC == "4000"){
                colorTempSlider.value = 3
            }else if(colorTempFromUVC == "6000"){
                colorTempSlider.value = 4
            }else if(colorTempFromUVC == "6500"){
                colorTempSlider.value = 5
            }

            skipUpdateColorTemperature = true

            //To get color correction matrix and RB gain after color temperature is set
            see3cam50cug.getColorCorrectionMatrix()
            see3cam50cug.getRBGain()
        }
        onGetExposureFromUVC:{
            manualExpTextField.text = exposureFromUVC
        }
    }

    property int gainMin: 0
    property int gainMax: 48

    property int blackLevelMin: 0
    property int blackLevelMax: 4095

    property real gainRBMin: 0
    property real gainRBMax: 7.99

    property real brightnessMin: 0
    property real brightnessMax: 3.99

    property real saturationMin: 0
    property real saturationMax: 3.99

    property int contrastMin: 0
    property int contrastMax: 15

    property real colorCorrectionMin: 0
    property real colorCorrectionMax: 3.2

    property real adjustedRGain: 0.0
    property real adjustedBGain: 0.0
    property real adjustedBrightness: 0.0
    property real adjustedSaturation: 0.0
    property real adjustedGammaCorrection: 0.0

    property int expMin: 100
    property int expMax: 10000000

    property bool skipUpdateGainMode            : false
    property bool skipUpdateBlackLevelMode      : false
    property bool skipUpdateBrightness          : false
    property bool skipUpdateContrast            : false
    property bool skipUpdateSaturation          : false
    property bool skipUpdateColorTemperature    : false
    property bool skipUpdateGammaCorrection     : false
    property bool setButtonClicked              : false

    property bool skipUpdateRGainMode            : false
    property bool skipUpdateBGainMode            : false


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
            height: 1500

            ColumnLayout{
                x:2
                y:5
                spacing:18

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
                        tooltip: "Gain:
Used for modifying Digital Gain of B channel in steps of 0.04"
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
                        minimumValue: gainMin
                        maximumValue: gainMax
                        style:econSliderStyle
                        stepSize: 1

                        onValueChanged:  {
                            //Sending HID value to UVC
                            root.getGainValueFromHID(gainSlider.value)

                            gainTextField.text = gainSlider.value
                            if(skipUpdateGainMode){
                                see3cam50cug.setGainMode(SEE3CAM_50CUG.MANUAL_MODE, gainSlider.value)
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
                        text: "--- R Gain ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373

                        ToolButton{
                            tooltip: "R Gain :
Used for modifying Digital Gain of R channel in steps of 0.04"
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
                            minimumValue: gainRBMin
                            maximumValue: gainRBMax
                            style:econSliderStyle
                            stepSize: 0.005
                            onValueChanged:  {
                                gainRTextField.text = gainRSlider.value
                                if(skipUpdateRGainMode){
                                    adjustedRGain = parseFloat((gainRSlider.value).toFixed(3));
                                    gainRTextField.text = adjustedRGain
                                    see3cam50cug.setRBGain(adjustedRGain, gainBSlider.value)
                                }
                                skipUpdateRGainMode = true
                            }
                        }
                        TextField
                        {
                            id: gainRTextField
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
                        text: "--- B Gain ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373

                        ToolButton{
                            tooltip: "B Gain :
Used for modifying Digital Gain of B channel in steps of 0.04"
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
                            minimumValue: gainRBMin
                            maximumValue: gainRBMax
                            style:econSliderStyle
                            stepSize: 0.005
                            onValueChanged:  {
                                gainBTextField.text = gainBSlider.value
                                if(skipUpdateBGainMode){
                                    adjustedBGain = parseFloat((gainBSlider.value).toFixed(3));
                                    gainBTextField.text = adjustedBGain
                                    see3cam50cug.setRBGain(gainRSlider.value, adjustedBGain)
                                }
                                skipUpdateBGainMode = true
                            }
                        }
                        TextField
                        {
                            id: gainBTextField
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


               Text {
                   id: colorCorrectionMaxTitle
                   text: "--- Color Correction Matrix ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373

                   ToolButton{
                       tooltip: "Color Correction :
Used for applying manual 3×3 matrix to RGB channel in steps of +0.04 or -0.04

Matrix format :
Rr Rg Rb
Gr Gg Gb
Br Bg Bb

Application to RGB channel:
R_new = R_old*Rr + G_old*Rg + B_old*Rb
G_new = R_old*Gr + G_old*Gg + B_old*Gb
B_new = R_old*Br + G_old*Bg + B_old*Bb

For manual color temperature the CCM is predefined and it can be overwritten by using this control."
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
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
                   SpinBox{
                       id:spinBoxRg
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
                   SpinBox{
                       id:spinBoxRb
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
               }

               Row{
                   spacing: 10
                   Layout.alignment: Qt.AlignCenter
                   SpinBox{
                       id:spinBoxGr
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
                   SpinBox{
                       id:spinBoxGg
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
                   SpinBox{
                       id:spinBoxGb
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99
                   }
               }

               Row{
                   spacing: 10
                   Layout.alignment: Qt.AlignCenter
                   SpinBox{
                       id:spinBoxBr
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
                   SpinBox{
                       id:spinBoxBg
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
                   SpinBox{
                       id:spinBoxBb
                       decimals: 3
                       stepSize: 0.004
                       minimumValue: -3.99
                       maximumValue: 3.99

                   }
               }

               Button
               {
                   id: colorMatrixSetBtn
                   activeFocusOnPress : true
                   Layout.alignment: Qt.AlignCenter
                   text: "Set"
                   tooltip: "You can set the required color correction matrix elements by changing the value in the SpinBox and click the Set button"
                   style: econButtonStyle
                   enabled: true
                   opacity: 1
                   implicitHeight: 25
                   implicitWidth: 60
                   onClicked:
                   {
                       see3cam50cug.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)
                   }
                   Keys.onReturnPressed:
                   {
                       see3cam50cug.setColorCorrectionMatrix(spinBoxRr.value, spinBoxRg.value, spinBoxRb.value, spinBoxGr.value, spinBoxGg.value, spinBoxGb.value, spinBoxBr.value, spinBoxBg.value, spinBoxBb.value)
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
                       tooltip: "Blacklevel Adjustment:
The blacklevel (also called baseline) is the value which is subtracted from the image signal to compensated thermally generated noise.

Note: Changing the value will affect the image quality . Please use this after evaluating and checking enough.

Recommended value is : 240"
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
                       stepSize: 1

                       style:econSliderStyle
                       minimumValue: blackLevelMin
                       maximumValue: blackLevelMax
                       onValueChanged:  {
                           blackLevelTextField.text = blackLevelSlider.value
                           if(skipUpdateBlackLevelMode){
                               see3cam50cug.setBlackLevel(blackLevelSlider.value)
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
                   id: brightnessText
                   text: "--- Brightness ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "Brightness :
Used for changing brightness by modifying Y channel gain in steps of 0.04"
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
                       minimumValue: brightnessMin
                       maximumValue: brightnessMax
                       stepSize: 0.005
                       onValueChanged:  {
                           brightnessTextField.text = brightnessSlider.value
                           if(skipUpdateBrightness){
                               // Round the slider and TextField to three decimal places
                               adjustedBrightness = parseFloat((brightnessSlider.value).toFixed(3));

                               //Sending HID value to UVC
                               brightnessInt = adjustedBrightness * 200;
                               root.sendBrightnessToUVC(brightnessInt)

                               brightnessTextField.text = adjustedBrightness
                               see3cam50cug.setBrightness(adjustedBrightness)
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
                           const match = brightnessTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                           if(match){
                               brightnessTextField.text = match[1];
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
                       tooltip: "Contrast :
Used for changing contrast by modifying the strengths of S curve applied to Y channel "
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
                       minimumValue: contrastMin
                       maximumValue: contrastMax
                       stepSize: 1
                       onValueChanged:  {
                           contrastTextField.text = contrastSlider.value
                           if(skipUpdateContrast){
                               //Sending HID value to UVC
                               root.sendContrastToUVC(contrastSlider.value)

                               see3cam50cug.setContrast(contrastSlider.value)
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
                       tooltip: "Saturation :
Used for changing saturation by modifying the gain of Z curve applied to UV channel in steps of 0.04"
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
                       minimumValue: saturationMin
                       maximumValue: saturationMax
                       stepSize: 0.005
                       onValueChanged:  {
                           saturationTextField.text = saturationSlider.value
                           if(skipUpdateSaturation){
                               // Round the slider and TextField to three decimal places
                               adjustedSaturation = parseFloat((saturationSlider.value).toFixed(3));

                               //Sending HID value to UVC
                               saturationInt = adjustedSaturation * 200
                               root.sendSaturationToUVC(saturationInt)

                               saturationTextField.text = adjustedSaturation
                               see3cam50cug.setSaturation(adjustedSaturation)
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
                           const match = saturationTextField.text.match(/^(-?\d+(\.\d{0,3})?).*/);
                           if(match){
                               saturationTextField.text = match[1];
                               saturationSlider.value = saturationTextField.text
                           }
                       }
                   }
               }

               Text
               {
                   id: colorTempTitle
                   text: "--- Color Temperature ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373
                   ToolButton{
                       tooltip: "Color temperature :
Used for white balancing the scene based on the temperature set by applying predefined R and B digital gain.
Note:  For temperature other than the possible values(2300, 2800, 3000,4000, 6000, 6500) individual R and B digital gain can be manually adjusted."
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
                       maximumValue: 5
                       stepSize: 1

                       onValueChanged:  {

                           if(skipUpdateColorTemperature){

                               switch (value) {
                                   case 0:
                                       root.sendColorTemperatureToUVC(1)
                                       colorTempTextField.text = "2300"
                                       see3cam50cug.setColorTemperature(2300)

                                       break;
                                   case 1:
                                       root.sendColorTemperatureToUVC(2)
                                       colorTempTextField.text = "2800"
                                       see3cam50cug.setColorTemperature(2800)

                                       break;
                                   case 2:
                                       root.sendColorTemperatureToUVC(3)
                                       colorTempTextField.text = "3000"
                                       see3cam50cug.setColorTemperature(3000)

                                       break;
                                   case 3:
                                       root.sendColorTemperatureToUVC(4)
                                       colorTempTextField.text = "4000"
                                       see3cam50cug.setColorTemperature(4000)

                                       break;
                                   case 4:
                                       root.sendColorTemperatureToUVC(5)
                                       colorTempTextField.text = "6000"
                                       see3cam50cug.setColorTemperature(6000)

                                       break;
                                   case 5:
                                       root.sendColorTemperatureToUVC(6)
                                       colorTempTextField.text = "6500"
                                       see3cam50cug.setColorTemperature(6500)

                                       break;
                               }

                               //To get color correction matrix and RB gain after color temperature is set
                               see3cam50cug.getColorCorrectionMatrix()
                               see3cam50cug.getRBGain()
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
                       tooltip: "Gamma Correction:
Used for changing gamma by modifying strengths of gamma correction curve applied to Y channel in steps of 0.1"
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
                       minimumValue: colorCorrectionMin
                       maximumValue: colorCorrectionMax
                       stepSize: 0.1
                       onValueChanged:  {
                           gammaCorrectionTextField.text = gammaCorrectionSlider.value
                           if(skipUpdateGammaCorrection){
                               // Round the slider and TextField to three decimal places
                               adjustedGammaCorrection = parseFloat((gammaCorrectionSlider.value).toFixed(1));

                               //Sending HID value to UVC
                               gammaInt = adjustedGammaCorrection * 10
                               root.sendGammaToUVC(gammaInt)

                               gammaCorrectionTextField.text = adjustedGammaCorrection
                               see3cam50cug.setGammaCorrection(adjustedGammaCorrection)
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
                       tooltip: "Exposure:
This feature controls the integration time of the sensor. Values are expressed in micro seconds."
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
                       text: "value(µs)[100 - 10000000]"
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

                           //Sending HID value to UVC
                           exposureText = manualExpTextField.text
                           if(manualExpTextField.text <= 2000000)
                           {
                               exposureInt = manualExpTextField.text / 100
                               root.sendExposureToUVC(exposureInt)
                           }
                           else{
                               root.sendExposureToUVC(200000)
                           }
                           manualExpTextField.text = exposureText

                           see3cam50cug.setExposure(SEE3CAM_50CUG.MANUAL_EXPOSURE, manualExpTextField.text);
                           exposureSetBtn.enabled = true
                       }
                       Keys.onReturnPressed:
                       {
                           exposureSetBtn.enabled = false
                           setButtonClicked = true

                           //Sending HID value to UVC
                           exposureText = manualExpTextField.text
                           if(manualExpTextField.text <= 2000000)
                           {
                               exposureInt = manualExpTextField.text / 100
                               root.sendExposureToUVC(exposureInt)
                           }
                           else{
                               root.sendExposureToUVC(200000)
                           }
                           manualExpTextField.text = exposureText

                           see3cam50cug.setExposure(SEE3CAM_50CUG.MANUAL_EXPOSURE, manualExpTextField.text);
                           exposureSetBtn.enabled = true
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

                   ToolButton{
                       tooltip: "Master Mode :
After choosing master mode, the application starts steaming.
This is a simple mode of operation for the camera without any external trigger capability.

Trigger Mode:
1.Exposure trigger:
    In this mode, the sensor integration time is decided by the pulse width input (low level) to the trigger pin.
    For example :
    If the width of the low level trigger pulse is 15.6 ms then the exposure configured in the sensor is 15.6 ms.
    In this mode strobe function is not supported.

2.Acquisition trigger:
    High to low transition of the trigger pulse will initiate the capture of the image with the exposure configured in the exposure slider."
                       width: 200
                       opacity: 0
                   }
               }
               ColumnLayout{
                  ExclusiveGroup { id: cameraModeGroup }
                  spacing: 25
                  RadioButton {
                      id: masterMode
                      style:  econRadioButtonStyle
                      text: qsTr("Master")
                      exclusiveGroup: cameraModeGroup
                      activeFocusOnPress: true
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
                      onClicked: {
                          //enable trigger combobox
                          triggerCombo.enabled = true
                          triggerCombo.opacity = 1

                          setTriggerMode()
                      }
                      Keys.onReturnPressed: {
                          triggerCombo.enabled = true
                          triggerCombo.opacity = 1

                          setTriggerMode()
                      }
                  }
                  ComboBox
                  {
                      id: triggerCombo
                      opacity: 1
                      enabled: true
                      model: ListModel {
                          ListElement { text: "Exposure control" }
                          ListElement { text: "Acquisition control" }
                      }
                      activeFocusOnPress: true
                      style: econComboBoxStyle
                      onCurrentIndexChanged: {
                          setTriggerMode()
                      }
                  }
              }

               Text {
                   id: strobe
                   text: "--- Strobe Mode ---"
                   font.pixelSize: 14
                   font.family: "Ubuntu"
                   color: "#ffffff"
                   smooth: true
                   Layout.alignment: Qt.AlignCenter
                   opacity: 0.50196078431373

                   ToolButton{
                       tooltip: "Strobe Mode :
The transition from low to high and high to low of strobe pulses will directly indicate the start and stop of the sensor exposure.
This feature is supported in Acquisition trigger."
                       width: 200
                       opacity: 0
                   }
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
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3cam50cug.setStrobeMode(SEE3CAM_50CUG.STROBE_ON)
                       }
                       Keys.onReturnPressed: {
                           see3cam50cug.setStrobeMode(SEE3CAM_50CUG.STROBE_ON)
                       }
                   }

                   RadioButton {
                       id: strobeOff
                       exclusiveGroup: strobesGrp
                       checked: false
                       text: "Off"
                       activeFocusOnPress: true
                       style: econRadioButtonStyle
                       onClicked: {
                           see3cam50cug.setStrobeMode(SEE3CAM_50CUG.STROBE_OFF)
                       }
                       Keys.onReturnPressed: {
                           see3cam50cug.setStrobeMode(SEE3CAM_50CUG.STROBE_OFF)
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
                       tooltip: "Click to set default values in extension controls"
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
                       id: saveConfig
                       opacity: 1
                       activeFocusOnPress : true
                       text: "Save Configuration"
                       style: econButtonStyle
                       tooltip: "This feature will save the current configurations and are retained after the following power cycles."
                       onClicked:{
                           setButtonClicked = true
                           saveConfigurations()
                       }
                       Keys.onReturnPressed: {
                           setButtonClicked = true
                           saveConfigurations()
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

    SEE3CAM_50CUG{
        id:see3cam50cug

        onGainValueReceived: {
            skipUpdateGainMode = false
            gainSlider.value = gainValue
            root.getGainValueFromHID(gainValue)
            skipUpdateGainMode = true
        }

        onGainRReceived: {
            skipUpdateRGainMode = false
            RGain = parseFloat((RGain).toFixed(3));
            gainRSlider.value = RGain
            gainRTextField.text = RGain
            skipUpdateRGainMode = true
        }

        onGainBReceived: {
            skipUpdateBGainMode = false
            BGain = parseFloat((BGain).toFixed(3));
            gainBSlider.value = BGain
            gainBTextField.text = BGain
            skipUpdateBGainMode = true
        }

        onBlackLevelReceived: {
            blackLevelSlider.value = blackLevel
        }

        onBrightnessReceived: {
            skipUpdateBrightness = false
            brightness = parseFloat((brightness).toFixed(3));
            brightnessSlider.value = brightness
            brightnessTextField.text = brightness
            root.sendBrightnessToUVC(brightness)
            skipUpdateBrightness = true
        }

        onContrastReceived: {
            skipUpdateContrast = false
            contrastSlider.value = contrast
            root.sendContrastToUVC(contrast)
            skipUpdateContrast = true
        }

        onSaturationReceived: {
            skipUpdateSaturation = false
            saturation = parseFloat((saturation).toFixed(3));
            saturationSlider.value = saturation
            saturationTextField.text = saturation
            root.sendSaturationToUVC(saturation)
            skipUpdateSaturation = true
        }

        onColorTemperatureReceived: {
            skipUpdateColorTemperature = false
            colorTempTextField.text = colorTemp

            if(colorTemp == "2300") {
               colorTempSlider.value = 0
               root.sendColorTemperatureToUVC(1)
            } else if(colorTemp == "2800") {
                colorTempSlider.value = 1
                root.sendColorTemperatureToUVC(2)
            } else if(colorTemp == "3000") {
                colorTempSlider.value = 2
                root.sendColorTemperatureToUVC(3)
            } else if(colorTemp == "4000") {
                colorTempSlider.value = 3
                root.sendColorTemperatureToUVC(4)
            } else if(colorTemp == "6000") {
                colorTempSlider.value = 4
                root.sendColorTemperatureToUVC(5)
            } else if(colorTemp == "6500") {
                colorTempSlider.value = 5
                root.sendColorTemperatureToUVC(6)
            }

            skipUpdateColorTemperature = true
        }

        onGammaCorrectionReceived: {
            skipUpdateGammaCorrection = false
            gammaCorrection = parseFloat((gammaCorrection).toFixed(1));
            gammaCorrectionSlider.value = gammaCorrection
            gammaCorrectionTextField.text = gammaCorrection
            root.sendGammaToUVC(gammaCorrection)
            skipUpdateGammaCorrection = true
        }

        onCameraModeReceived:{
            getCameraModeReceived(cameraMode)
        }

        onStrobeModeReceived: {
            currentStrobeMode(strobeMode)

            //To Disable/Enable the strobe mode to the respective camera mode
            if(masterMode.checked == true)
            {
                strobeOn.enabled    = false
                strobeOn.opacity    = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
            }
            else if(triggerCombo.currentIndex == 0)
            {
                strobeOn.enabled    = false
                strobeOn.opacity    = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1
            }
            else
            {
                strobeOn.enabled    = true
                strobeOn.opacity    = 1
                strobeOff.enabled   = true
                strobeOff.opacity   = 1
            }
        }

        onExposureValueReceived: {
            manualExpTextField.text = exposure
        }

        onValueRReceived: {
            spinBoxRr.value = Rr
            spinBoxRg.value = Rg
            spinBoxRb.value = Rb
        }

        onValueGReceived: {
            spinBoxGr.value = Gr
            spinBoxGg.value = Gg
            spinBoxGb.value = Gb
        }

        onValueBReceived: {
            spinBoxBr.value = Br
            spinBoxBg.value = Bg
            spinBoxBb.value = Bb
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
                see3cam50cug.getExposure()
            }
        }
    }

    function getCameraModeReceived(mode)
    {
        if(mode == SEE3CAM_50CUG.MASTER)
        {
            masterMode.checked = true
            triggerCombo.enabled = false
            triggerCombo.opacity = 0.1

            strobeOn.enabled = false
            strobeOn.opacity = 0.1
            strobeOff.enabled   = false
            strobeOff.opacity   = 0.1
        }
        else
        {
            triggerCombo.enabled = true
            triggerCombo.opacity = 1
            triggerMode.checked = true

            root.checkForTriggerMode(true)
            root.videoRecordBtnEnable(false)
            root.captureBtnEnable(false)

            if(mode == SEE3CAM_50CUG.TRIGGER_EXP_CONTROL)
            {
                strobeOn.enabled = false
                strobeOn.opacity = 0.1
                strobeOff.enabled   = false
                strobeOff.opacity   = 0.1

                triggerCombo.currentIndex = 0
            }
            else if(mode == SEE3CAM_50CUG.TRIGGER_ACQ_CONTROL)
            {
                triggerCombo.currentIndex = 1

                strobeOn.enabled = true
                strobeOn.opacity = 1
                strobeOff.enabled   = true
                strobeOff.opacity   = 1
            }
        }
    }

    function setMasterMode()
    {
        //disable strobe mode when the camera is in Master mode
        strobeOn.enabled    = false
        strobeOn.opacity    = 0.1
        strobeOff.enabled   = false
        strobeOff.opacity   = 0.1

        //disable trigger combobox
        triggerCombo.enabled = false
        triggerCombo.opacity = 0.1

        see3cam50cug.setCameraMode(SEE3CAM_50CUG.MASTER)

        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function setTriggerMode()
    {
        if(triggerCombo.currentText.toString() == "Exposure control")
        {
           setTriggerExpControl()
        }
        else if(triggerCombo.currentText.toString() == "Acquisition control")
        {
           setTriggerAcqControl()
        }
    }

    function setTriggerExpControl()
    {
        root.checkForTriggerMode(true)
        root.videoRecordBtnEnable(false)
        root.captureBtnEnable(false)

        //disable strobe mode when the camera is in Master mode
        strobeOn.enabled    = false
        strobeOn.opacity    = 0.1
        strobeOff.enabled   = false
        strobeOff.opacity   = 0.1

        see3cam50cug.setCameraMode(SEE3CAM_50CUG.TRIGGER_EXP_CONTROL)
    }

    function setTriggerAcqControl()
    {
        root.checkForTriggerMode(true)
        root.videoRecordBtnEnable(false)
        root.captureBtnEnable(false)

        //enable strobe mode when the camera is in trigger mode
        strobeOn.enabled = true
        strobeOn.opacity = 1
        strobeOff.enabled   = true
        strobeOff.opacity   = 1

        see3cam50cug.setCameraMode(SEE3CAM_50CUG.TRIGGER_ACQ_CONTROL)
    }

    function currentStrobeMode(mode){
        switch(mode){
            case SEE3CAM_50CUG.STROBE_OFF:
                strobeOff.checked   = true
                strobeOn.checked = false
                break;
            case SEE3CAM_50CUG.STROBE_ON:
                strobeOff.checked   = false
                strobeOn.checked = true
                break;
        }
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function saveConfigurations()
    {
        see3cam50cug.saveConfiguration()
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

        if(see3cam50cug.setToDefault())
        {
            getCurrentValuesFromCamera()
        }
        defaultValue.enabled = true
    }

    function getCurrentValuesFromCamera(){
        see3cam50cug.getGainMode()
        see3cam50cug.getRBGain()
        see3cam50cug.getColorCorrectionMatrix()
        see3cam50cug.getBlackLevel()
        see3cam50cug.getBrightness()
        see3cam50cug.getContrast()
        see3cam50cug.getSaturation()
        see3cam50cug.getColorTemperature()
        see3cam50cug.getGammaCorrection()
        see3cam50cug.getExposure()
        see3cam50cug.getCameraMode()
        see3cam50cug.getStrobeMode()
    }
    Component.onCompleted: {
        getCurrentValuesFromCamera()
    }
}
