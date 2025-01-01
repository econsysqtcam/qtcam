import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3camcu81 1.0
import cameraenum 1.0
import QtQuick 2.0
import "../../JavaScriptFiles/tempValue.js" as JS

Item
{
    width:268
    height:720

    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIFrameRate: false
    property bool skipUpdateUIOnAntiFlickerMode:false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIDenoise: false
    property bool setButtonClicked: false

    property int denoiseMin: 0
    property int denoiseMax: 15
    property int qFactorMin: 10
    property int qFactorMax: 96
    property int frameRateMin: 3
    property int frameRateMax: 60
    property int expoCompMin: 8000
    property int expoCompMax: 1000000

    ScrollView
    {
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle
        Item{
            height: 1700
            Layout.alignment: Qt.AlignCenter

            ColumnLayout
            {
                x:2
                y:5
                spacing:20

                Row{
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                         id: cameraMode_title
                         text: "--- Camera Mode ---"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         opacity: 0.50196078431373
                    }
                }

                Grid
                {
                    columns: 2
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: cameraMode }

                    RadioButton
                    {
                        id: hdr_mode
                        style:  econRadioButtonStyle
                        text:   qsTr("HDR")
                        exclusiveGroup: cameraMode
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu81.setCameraMode(See3CamCU81.HDR_MODE)
                            see3camcu81.getExposureCompensation()
                            see3camcu81.getAntiFlickerMode()
                        }
                        Keys.onReturnPressed:  {
                            see3camcu81.setCameraMode(See3CamCU81.HDR_MODE)
                            see3camcu81.getExposureCompensation()
                            see3camcu81.getAntiFlickerMode()
                        }
                    }
                    RadioButton
                    {
                        id: linear_mode
                        style:  econRadioButtonStyle
                        text: qsTr("Linear")
                        exclusiveGroup: cameraMode
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu81.setCameraMode(See3CamCU81.LINEAR_MODE)
                            see3camcu81.getExposureCompensation()
                            see3camcu81.getAntiFlickerMode()
                        }
                        Keys.onReturnPressed: {
                            see3camcu81.setCameraMode(See3CamCU81.LINEAR_MODE)
                            see3camcu81.getExposureCompensation()
                            see3camcu81.getAntiFlickerMode()
                        }
                    }
                }
                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text
                    {
                         id: special_effects
                         text: "--- Special Effects ---"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         opacity: 0.50196078431373
                    }
                }

                Grid
                {
                    columns: 2
                    spacing: 20

                    ExclusiveGroup { id: effectInputGroup }
                    RadioButton
                    {
                        id: rdoEffectNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_NORMAL)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_NORMAL)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectBW
                        style:  econRadioButtonStyle
                        text: qsTr("Black and White")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_BLACK_WHITE)
                        }
                        Keys.onReturnPressed: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_BLACK_WHITE)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectGreyScale
                        style:  econRadioButtonStyle
                        text: qsTr("GreyScale")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_GREYSCALE)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectSketch
                        style:  econRadioButtonStyle
                        text: qsTr("Sketch")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_SKETCH)
                        }
                        Keys.onReturnPressed: {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_SKETCH)
                        }
                    }
                    RadioButton
                    {
                        id: rdoEffectNegative
                        style:  econRadioButtonStyle
                        text: qsTr("Negative")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked:
                        {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_NEGATIVE)
                        }
                        Keys.onReturnPressed:
                        {
                            see3camcu81.setEffect(See3CamCU81.EFFECT_NEGATIVE)
                        }
                    }
                }
                Text
                {
                    id: denoiseText
                    text: "--- De-Noise ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row
                {
                    spacing: 35
                    Slider
                    {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: deNoiseSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: denoiseMin
                        maximumValue: denoiseMax
                        onValueChanged:
                        {
                            deNoiseTextField.text = deNoiseSlider.value
                            if(skipUpdateUIDenoise)
                            {
                                see3camcu81.setDenoise(deNoiseSlider.value)
                            }
                            skipUpdateUIDenoise = true
                        }
                    }
                    TextField
                    {
                        id: deNoiseTextField
                        text: deNoiseSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: deNoiseSlider.minimumValue; top: deNoiseSlider.maximumValue}
                        onTextChanged:
                        {
                            if(text.length > 0)
                            {
                                deNoiseSlider.value = deNoiseTextField.text
                            }
                        }
                    }
                }
                Text
                {
                    id: roiAutoExpMode
                    text: "--- ROI - Auto Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row
                {
                      spacing:60
                      ExclusiveGroup { id: roiExpogroup }
                      Layout.alignment: Qt.AlignCenter

                      RadioButton
                      {
                          exclusiveGroup: roiExpogroup
                          id: autoexpFull
                          text: "Full"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                          // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                          // winSize is required only for manual mode
                          onClicked: {
                              setAutoExposureROI()
                          }
                          Keys.onReturnPressed: {
                              setAutoExposureROI()
                          }
                      }
                      RadioButton
                      {
                          exclusiveGroup: roiExpogroup
                          id: autoexpManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          onClicked: {
                              setAutoExposureROI()
                          }
                          Keys.onReturnPressed: {
                              setAutoExposureROI()
                          }
                      }
                }
                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (autoexpManual.enabled && autoexpManual.checked) ? true : false
                    opacity: (autoexpManual.enabled && autoexpManual.checked) ? 1 : 0.1
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
                            see3camcu81.setROIAutoExposure(See3CamCU81.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }
                Text
                {
                    id: burstLength
                    text: "Burst Length :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 1
                }
                ComboBox
                {
                    id: burstLengthCombo
                    opacity: 1
                    enabled: true
                    model: ListModel
                           {
                                ListElement { text: "1" }
                                ListElement { text: "2" }
                                ListElement { text: "3" }
                                ListElement { text: "4" }
                                ListElement { text: "5" }
                            }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        root.stillBurstLength(burstLengthCombo.currentIndex + 1) // combobox index starts from 0
                        if(skipUpdateUIOnBurstLength){
                            see3camcu81.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }
                Text
                {
                    id: qFactorText
                    text: "--- Q Factor ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row
                {
                    spacing: 35
                    Slider
                    {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: qFactorSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: qFactorMin
                        maximumValue: qFactorMax
                        onValueChanged:  {
                            qFactorTextField.text = qFactorSlider.value
                            if(skipUpdateUIQFactor){
                                see3camcu81.setQFactor(qFactorSlider.value)
                            }
                            skipUpdateUIQFactor = true
                        }
                    }
                    TextField
                    {
                        id: qFactorTextField
                        text: qFactorSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: qFactorSlider.minimumValue; top: qFactorSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                qFactorSlider.value = qFactorTextField.text
                            }
                        }
                    }
                }
                Text
                {
                    id: flipText
                    text: "--- Flip Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row
                {
                    spacing: 55
                    CheckBox
                    {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        onClicked:{
                            see3camcu81.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            see3camcu81.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox
                    {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            see3camcu81.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            see3camcu81.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                }
                Text
                {
                     id: exposureCompTextTitle
                     text: "--- Exposure Compensation ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     Layout.alignment: Qt.AlignCenter
                     opacity: 0.50196078431373
                }
                Row
                {
                        spacing: 9
                        Text
                        {
                            id: exposureCompText
                            text: "value(µs)[8000 - 1000000]"
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
                            id: exposureCompValue
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            opacity: 1
                            style: econTextFieldStyle
                            implicitHeight: 25
                            implicitWidth: 80
                            validator: IntValidator {bottom: expoCompMin; top: expoCompMax}
                        }
                        Button
                        {
                            id: exposureCompSet
                            activeFocusOnPress : true
                            text: "Set"
                            tooltip: "You can set the required exposure compensation value by changing the
        value in the text box and click the Set button"
                            style: econButtonStyle
                            enabled: true
                            opacity: 1
                            implicitHeight: 25
                            implicitWidth: 60
                            onClicked:
                            {
                                exposureCompSet.enabled = false
                                setButtonClicked = true
                                see3camcu81.setExposureCompensation(exposureCompValue.text)
                                exposureCompSet.enabled = true
                            }
                            Keys.onReturnPressed:
                            {
                                exposureCompSet.enabled = false
                                setButtonClicked = true
                                see3camcu81.setExposureCompensation(exposureCompValue.text)
                                exposureCompSet.enabled = true
                            }
                        }
                    }
                    Text
                    {
                        id: frameRateText
                        text: "--- Frame Rate Control ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    Row
                    {
                        spacing: 35
                        Slider
                        {
                            activeFocusOnPress: true
                            updateValueWhileDragging: false
                            id: frameRateSlider
                            width: 150
                            stepSize: 1
                            style:econSliderStyle
                            minimumValue: frameRateMin
                            maximumValue: frameRateMax
                            onValueChanged:  {
                                frameRateTextField.text = frameRateSlider.value
                                if(skipUpdateUIFrameRate){
                                    see3camcu81.setFrameRateCtrlValue(frameRateSlider.value)
                                }
                                skipUpdateUIFrameRate = true
                            }
                        }
                        TextField
                        {
                            id: frameRateTextField
                            text: frameRateSlider.value
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            validator: IntValidator {bottom: frameRateSlider.minimumValue; top: frameRateSlider.maximumValue}
                            onTextChanged: {
                                if(text.length > 0){
                                    frameRateSlider.value = frameRateTextField.text
                                }
                            }
                        }
                    }
                    Text {
                        id: faceDetectionText
                        text: "--- Face Detection ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    Row{
                        spacing: 40
                        ExclusiveGroup { id: faceRectGroup }
                        RadioButton {
                            exclusiveGroup: faceRectGroup
                            id: faceRectEnable
                            text: "Enable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked:{
                                see3camcu81.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                            Keys.onReturnPressed: {
                                see3camcu81.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                        }
                        RadioButton {
                            exclusiveGroup: faceRectGroup
                            id:faceRectDisable
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                see3camcu81.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                            Keys.onReturnPressed: {
                                see3camcu81.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                        }
                    }
                    Row{
                        spacing: 5
                        CheckBox {
                            id: faceDetectEmbedData
                            activeFocusOnPress : true
                            text: "Embed \nData"
                            style: econCheckBoxTextWrapModeStyle
                            enabled: faceRectEnable.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            onClicked:{
                                enableFaceDetectEmbedData()
                            }
                            Keys.onReturnPressed: {
                                enableFaceDetectEmbedData()
                            }
                        }
                        CheckBox {
                            id: overlayRect
                            activeFocusOnPress : true
                            text: "Overlay \nRectangle"
                            style: econCheckBoxTextWrapModeStyle
                            enabled: faceRectEnable.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            onClicked:{
                                see3camcu81.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                            }
                            Keys.onReturnPressed: {
                                see3camcu81.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                            }
                        }
                    }
                    Text {
                        id: getExpTitle
                        text: "--- Auto Exposure ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }
                    Row
                    {
                        spacing: 13
                        Text {
                            id: getExpText
                            text: "Exposure\n(In Microseconds)"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                        }
                        TextField
                        {
                            id: getExpTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            readOnly: true
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                        }
                        Button {
                            id: getExpBtn
                            activeFocusOnPress : true
                            text: "Get"
                            style: econButtonStyle
                            implicitHeight: 20
                            implicitWidth: 45
                            onClicked: {
                                see3camcu81.getAutoExposure()
                            }
                            Keys.onReturnPressed: {
                                see3camcu81.getAutoExposure()
                            }
                        }
                    }

                    Text {
                        id: getWbTitle
                        text: "--- Auto White Balance ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }
                    Row
                    {
                        spacing: 13
                        Text {
                            id: getWbText
                            text: "White Balance \n(In Kelvin)   "
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                        }
                        TextField
                        {
                            id: getWbTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            readOnly: true
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                        }
                        Button {
                            id: getWbBtn
                            activeFocusOnPress : true
                            text: "Get"
                            style: econButtonStyle
                            implicitHeight: 20
                            implicitWidth: 45
                            onClicked: {
                                see3camcu81.getAutoWhiteBalance()
                            }
                            Keys.onReturnPressed: {
                                see3camcu81.getAutoWhiteBalance()
                            }
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
                    Row
                    {
                          spacing:9
                          Layout.alignment: Qt.AlignCenter
                          ExclusiveGroup { id: antiFlickerModegroup }
                          RadioButton
                          {
                              exclusiveGroup: antiFlickerModegroup
                              id: antiFlickerModeOff
                              text: "Off"
                              activeFocusOnPress: true
                              style: econRadioButtonStyle
                              opacity: enabled ? 1 : 0.1
                              width : 70
                              onClicked: {
                                  see3camcu81.setAntiFlickerMode(see3camcu81.AntiFlickerDisable);
                              }
                              Keys.onReturnPressed: {
                                  see3camcu81.setAntiFlickerMode(see3camcu81.AntiFlickerDisable);
                              }
                          }
                          RadioButton
                          {
                              exclusiveGroup: antiFlickerModegroup
                              id: antiFlickerModeAuto
                              text: "Auto"
                              implicitWidth: 70
                              activeFocusOnPress: true
                              style: econRadioButtonStyle
                              opacity: enabled ? 1 : 0.1
                              onClicked: {
                                  see3camcu81.setAntiFlickerMode(see3camcu81.AntiFlickerAuto);
                              }
                              Keys.onReturnPressed: {
                                  see3camcu81.setAntiFlickerMode(see3camcu81.AntiFlickerAuto);
                              }
                          }
                          RadioButton
                          {
                              exclusiveGroup: antiFlickerModegroup
                              id: antiFlickerModeManual
                              text: "Manual"
                              activeFocusOnPress: true
                              style: econRadioButtonStyle
                              opacity: enabled ? 1 : 0.1
                              implicitWidth: 70
                              onClicked: {
                                  setAntiFlickerMode()
                              }
                              Keys.onReturnPressed: {
                                  setAntiFlickerMode()
                              }
                          }
                    }
                    Text
                    {
                        id: frequency
                        text: "Frequency :"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? 1 : 0.1
                    }
                    ComboBox
                    {
                        id: antiFlickerCombo
                        enabled: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? true : false
                        opacity: (antiFlickerModeManual.enabled && antiFlickerModeManual.checked) ? 1 : 0.1
                        model: ListModel
                               {
                                    ListElement { text: "50 Hz" }
                                    ListElement { text: "60 Hz" }
                               }
                        activeFocusOnPress: true
                        style: econComboBoxStyle
                        onCurrentIndexChanged: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlickerMode()
                            }
                            skipUpdateUIOnAntiFlickerMode = true
                        }
                    }
                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        Button
                        {
                            id: defaultValue
                            opacity: 1
                            activeFocusOnPress : true
                            text: "Default"
                            tooltip: "Click to set default values in extension controls"
                            action: setDefault
                            style: econButtonStyle
                            onClicked:{
                                setToDefaultValues()
                            }
                            Keys.onReturnPressed: {
                                setToDefaultValues()
                            }
                        }
                    }


                    Row
                    {
                        spacing: 20
                        Button
                        {
                            id: f_wversion_selected130
                            opacity: 1
                            action: firmwareVersion
                            activeFocusOnPress : true
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
                            Keys.onReturnPressed: {
                                getFirmwareVersion()
                            }
                        }
                        Button
                        {
                            id: serial_no_selected
                            opacity: 1
                            action: serialNumber
                            activeFocusOnPress : true
                            tooltip: "Click to view the Serial Number"
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
                            Keys.onReturnPressed: {
                                getSerialNumber()
                            }
                        }
                    }
                    Row{
                        Layout.alignment: Qt.AlignCenter
                        Button {
                            id: userDefinedConfig
                            opacity: 1
                            activeFocusOnPress : true
                            text: "Save Configuration"
                            style: econButtonStyle
                            tooltip: "This feature will save the current configurations and are retained after the following power cycles."
                            onClicked:{
                                setButtonClicked = true
                                see3camcu81.saveConfiguration(See3CamCU81.USER_DEFINED)
                            }
                            Keys.onReturnPressed: {
                                setButtonClicked = true
                                see3camcu81.saveConfiguration(See3CamCU81.USER_DEFINED)
                            }
                        }
                    }
                    Row{
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
                                see3camcu81.saveConfiguration(See3CamCU81.DEFAULT)
                            }
                            Keys.onReturnPressed: {
                                setButtonClicked = true
                                see3camcu81.saveConfiguration(See3CamCU81.DEFAULT)
                            }
                        }
                    }
            }//ColumnLayout

        }
    }//ScrollView
    Action
    {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }
    Action
    {
         id: firmwareVersion
         onTriggered:
         {
             getFirmwareVersion()
         }
    }

    Action
    {
         id: serialNumber
         onTriggered:
         {
                getSerialNumber()
         }
    }
    function getValuesFromCamera()
    {
        see3camcu81.getEffect()
        see3camcu81.getDenoise()
        see3camcu81.getQFactor()
        see3camcu81.getBurstLength()
        see3camcu81.getAutoExpROIModeAndWindowSize()
        see3camcu81.getFlipMode()
        see3camcu81.getExposureCompensation()
        see3camcu81.getFrameRateCtrlValue()
        see3camcu81.getAntiFlickerMode()
        see3camcu81.getAutoWhiteBalance()
        see3camcu81.getAutoExposure()
        see3camcu81.getFaceDetectMode()
        see3camcu81.getCameraMode()
    }

    function setToDefaultValues()
    {
        see3camcu81.setToDefault()
        getValuesFromCamera()
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function setAntiFlickerMode()
    {
        if(antiFlickerCombo.currentIndex === 0)
            see3camcu81.setAntiFlickerMode(See3CamCU81.AntiFlicker50Hz)
        else
            see3camcu81.setAntiFlickerMode(See3CamCU81.AntiFlicker60Hz)
    }
    function currentROIAutoExposureMode(roiMode, winSize)
    {
        switch(roiMode){
            case See3CamCU81.AutoExpFull:
                autoexpFull.checked = true
                autoExpoWinSizeCombo.enabled = false
                break
            case See3CamCU81.AutoExpManual:
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
            case See3CamCU81.AutoExpDisabled:
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
                break
        }
    }
    function enableFaceDetectEmbedData(){
        if(see3camcu81.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_130_Face_and_Smile_Detection for more details"))
            }
        }
    }
    function getFirmwareVersion()
    {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }
    function getSerialNumber()
    {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function enableDisableAutoExposureControls(autoExposureChecked){
        if(autoExposureChecked)
        {
            exposureCompSet.opacity = 1
            exposureCompValue.opacity = 1
            exposureCompText.opacity = 1
            exposureCompSet.enabled = true
            exposureCompValue.enabled = true
            exposureCompText.enabled = true

            autoexpFull.opacity   = 1
            autoexpFull.enabled   = true
            autoexpManual.opacity = 1
            autoexpManual.enabled = true

            if(autoexpManual.checked)
            {
                autoExpoWinSizeCombo.enabled = true
                autoExpoWinSizeCombo.opacity = 1
            }
            if(autoexpFull.checked)
            {
                autoExpoWinSizeCombo.enabled = false
                autoExpoWinSizeCombo.opacity = 0.1
            }
        }
        else
        {
            autoexpManual.enabled = false
            autoexpFull.enabled = false
            autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 0.1
            autoexpFull.opacity = 0.1
            autoExpoWinSizeCombo.opacity = 0.1

            exposureCompSet.opacity = 0.1
            exposureCompValue.opacity = 0.1
            exposureCompText.opacity = 0.1
            exposureCompSet.enabled = false
            exposureCompValue.enabled = false
            exposureCompText.enabled = false
        }
    }

    function setAutoExposureROI()
    {
        if(autoexpManual.checked)
        {
            autoExpoWinSizeCombo.enabled = true
            autoExpoWinSizeCombo.opacity = 1
            see3camcu81.setROIAutoExposure(See3CamCU81.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
        }
        else if(autoexpFull.checked)
        {
            see3camcu81.setROIAutoExposure(See3CamCU81.AutoExpFull, 0, 0, 0, 0, 0);
            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.opacity = 0.1
        }
    }

    See3CamCU81
    {
        id:see3camcu81
        onSendCameraModeValue:
        {
            if(cameraMode == See3CamCU81.HDR_MODE)
            {
                hdr_mode.checked = true
            }
            else if(cameraMode == See3CamCU81.LINEAR_MODE)
            {
                linear_mode.checked = true
            }
        }
        onSentEffectMode:
        {
            switch(effectMode)
            {
                case See3CamCU81.EFFECT_NORMAL:
                    rdoEffectNormal.checked = true
                    break;
                case See3CamCU81.EFFECT_BLACK_WHITE:
                    rdoEffectBW.checked = true
                    break;
                case See3CamCU81.EFFECT_GREYSCALE:
                    rdoEffectGreyScale.checked = true
                    break;
                case See3CamCU81.EFFECT_NEGATIVE:
                    rdoEffectNegative.checked = true
                    break;
                case See3CamCU81.EFFECT_SKETCH:
                    rdoEffectSketch.checked = true
                    break;
            }
        }
        onSendDenoiseValue:
        {
            skipUpdateUIDenoise = false
            deNoiseSlider.value = denoise
            skipUpdateUIDenoise = true
        }
        onRoiAutoExpModeValue:
        {
            currentROIAutoExposureMode(roiMode, winSize)
        }
        onBurstLengthValue:
        {
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
            skipUpdateUIOnBurstLength = true
        }
        onQFactorValue:
        {
            skipUpdateUIQFactor = false
            qFactorSlider.value = qFactor
            skipUpdateUIQFactor = true
        }
        onFlipMirrorModeChanged:
        {
            switch(flipMirrorMode)
            {
                case See3CamCU81.FLIP_BOTHFLIP_ENABLE:
                     flipCtrlVertical.checked = true
                     flipCtrlHorizotal.checked = true
                    break;
                case See3CamCU81.FLIP_VERTFLIP:
                    flipCtrlVertical.checked = true
                    flipCtrlHorizotal.checked = false
                    break;
                case See3CamCU81.FLIP_HORZFLIP:
                     flipCtrlVertical.checked = false
                     flipCtrlHorizotal.checked = true
                    break;
                case See3CamCU81.FLIP_BOTHFLIP_DISABLE:
                    flipCtrlVertical.checked = false
                    flipCtrlHorizotal.checked = false
                    break;
            }
        }
        onFrameRateCtrlValueReceived:
        {
            skipUpdateUIFrameRate = false
            frameRateSlider.value = frameRateCtrlValue
            skipUpdateUIFrameRate = true
        }
        onAntiFlickerModeChanged:
        {
            switch(flickerMode)
            {
                case See3CamCU81.AntiFlickerAuto:
                    antiFlickerModeAuto.checked = true
                    break
                case See3CamCU81.AntiFlicker50Hz:
                    antiFlickerModeManual.checked = true
                    skipUpdateUIOnAntiFlickerMode = false
                    antiFlickerCombo.currentIndex = 0
                    skipUpdateUIOnAntiFlickerMode = true
                    break
                case See3CamCU81.AntiFlicker60Hz:
                    antiFlickerModeManual.checked = true
                    skipUpdateUIOnAntiFlickerMode = false
                    antiFlickerCombo.currentIndex = 1
                    skipUpdateUIOnAntiFlickerMode = true
                    break
                 case See3CamCU81.AntiFlickerDisable:
                    antiFlickerModeOff.checked = true
                    break
             }
        }
        onExposureCompValueReceived:
        {
             exposureCompValue.text = exposureCompensation
        }
        onAutoExposureValueReceived:{
            getExpTextField.text = exposure
        }
        onAutoWhiteBalanceValueReceived:{
            getWbTextField.text = whiteBalance
        }
        onFaceDetectModeValueReceived:{
            if(faceDetectMode == See3CamCU81.FaceRectEnable){
                faceRectEnable.checked = true
                if(faceDetectEmbedDataValue == See3CamCU81.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }
                if(faceDetectOverlayRect == See3CamCU81.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }
            }else if(faceDetectMode == See3CamCU81.FaceRectDisable){
                faceRectDisable.checked = true
                if(faceDetectEmbedDataValue == See3CamCU81.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }else{
                    faceDetectEmbedData.checked = false
                }
                if(faceDetectOverlayRect == See3CamCU81.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }else{
                    overlayRect.checked = false
                }
            }
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
                see3camcu81.getExposureCompensation()
            }
        }
    }

    Uvccamera
    {
        id: uvccamera
        onTitleTextChanged:
        {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }
        onSerialNumber:
        {
            messageDialog.title = qsTr("Serial Number")
            messageDialog.text = serialNumber;
        }
    }
    Connections
    {
        target: root
        function onTakeScreenShot()
        {
            root.imageCapture(CommonEnums.BURST_SHOT);
        }
        function onGetVideoPinStatus()
        {
            root.enableVideoPin(true);
        }
        function onGetStillImageFormats()
        {
            var stillImageFormat = []
            stillImageFormat.push("jpg")
            stillImageFormat.push("bmp")
            stillImageFormat.push("raw")
            stillImageFormat.push("png")
            root.insertStillImageFormat(stillImageFormat);
        }
        function onExtensionTabVisible(visible)
        {
            if(visible)
            {
                getValuesFromCamera()
            }
        }
        function onMouseRightClicked(x, y, width, height){
            if(autoexpManual.enabled && autoexpManual.checked){
                see3camcu81.setROIAutoExposure(See3CamCU81.AutoExpManual,width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        function onVideoResolutionChanged(){
            see3camcu81.getFrameRateCtrlValue()
        }
        function onPreviewFPSChanged(){
            see3camcu81.getFrameRateCtrlValue()
        }
        function onVideoColorSpaceChanged(){
            see3camcu81.getFrameRateCtrlValue()
        }
        function onAutoExposureSelected(autoExposureSelect){
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        function onSkipFrameCount(){
            root.setSkipFrameCount(3)
        }
    }

    Component.onCompleted:
    {
        root.disablePowerLineFreq()
        root.checkForTriggerMode(false)
        root.startUpdatePreviewInMasterMode()
    }
    Component
    {
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
                id: burstLengthCombo_bkgrnd
                source: "../../Views/images/device_box.png"
                Rectangle {
                    width: burstLengthCombo_bkgrnd.sourceSize.width  - 28
                    height: burstLengthCombo_bkgrnd.sourceSize.height
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
        id: econCheckBoxTextWrapModeStyle
        CheckBoxStyle {
            label: Text {
                text: control.text
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                opacity: 1
                width: 100
                wrapMode: Text.WordWrap
            }
            background: Rectangle {
                color: "#222021"
                border.color: control.activeFocus ? "#ffffff" : "#222021"
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

}
