import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam130 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    id: see3cam30Id
    width:268
    height:750
    property int denoiseMin: 0
    property int denoiseMax: 15
    property int qFactorMin: 10
    property int qFactorMax: 96
    property int frameRateMin: 1
    property int frameRateMax: 120
    property int expoCompMin: 8000
    property int expoCompMax: 1000000

    // Flags to prevent setting values in camera when getting the values from camera
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIDenoise : false
    property bool skipUpdateUIFrameRate: false
    property bool skipUpdateUIOnAFWindowSize: false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIiHDR: false
    property bool setButtonClicked: false
    property bool skipUpdateUIFlickerCtrl:false
    property int  flickerCtrl
    property bool skipUpdateUIOnRedGain: false
    property bool skipUpdateUIOnBlueGain: false
    property bool skipUpdateUIOnGreenGain: false


    Connections
    {
        target: root
        function onTakeScreenShot(isWebKeyPressed)
        {
            //To avoid capturing 2 images for single trigger signal.
            if(isWebKeyPressed){
                if(seecam130.enableDisableFaceRectangle(false)){
                    burstShotTimer.start()
                }
            }else{
                if(seecam130.enableDisableFaceRectangle(false)){
                    root.imageCapture(CommonEnums.BURST_SHOT);
                }
            }
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
    }

    Connections{
         target: root
         function onMouseRightClicked(x, Y, width, height){
             if(afManual.enabled && afManual.checked){
                 seecam130.setROIAutoFocus(See3Cam130.AF_MANUAL_ROI, width, height, x, y, afWindowSizeCombo.currentText)
             }
             if(autoexpManual.enabled && autoexpManual.checked){
                seecam130.setROIAutoExposure(See3Cam130.AE_MANUAL_ROI, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }
         }
         function onAutoFocusSelected(autoFocusSelect){
             enableDisableAutoFocusUIControls(autoFocusSelect)
         }
         function onAutoExposureSelected(autoExposureSelect){
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         function onEnableFaceRectafterBurst(){
             seecam130.enableDisableFaceRectangle(true)
         }
         function onSetHIDControls(){
             seecam130.setFrameRateCtrlValue(frameRateSlider.value)
         }
    }

    Connections{
         target: root
         function onAfterBurst(){
             if(rectEnable.checked){
                seecam130.enableDisableAFRectangle(true)
             }
         }
         function onBeforeRecordVideo(){
             seecam130.enableDisableAFRectangle(false)
             seecam130.enableDisableFaceRectangle(false)
         }
         function onAfterRecordVideo(){
             if(rectEnable.checked){
                seecam130.enableDisableAFRectangle(true)
             }
             seecam130.enableDisableFaceRectangle(true)
         }
         function onVideoResolutionChanged(){
             getexposureCompFrameRateCtrlTimer.start()
         }
         function onPreviewFPSChanged(){
             getexposureCompFrameRateCtrlTimer.start()
         }
         function onVideoColorSpaceChanged(){
             getexposureCompFrameRateCtrlTimer.start()
         }
    }

    Timer {
        id: burstShotTimer
        interval: 1000
        onTriggered: {
            root.imageCapture(CommonEnums.BURST_SHOT);
            stop()
        }
    }

    Timer {
        id: getCamValuesTimer
        interval: 500
        onTriggered: {
            seecam130.getSceneMode()
            seecam130.getEffectMode()
            seecam130.getDenoiseValue()
            seecam130.getQFactor()
            seecam130.getExposureCompensation()
            seecam130.getFrameRateCtrlValue()
            seecam130.getAutoExposure()
            seecam130.getAutoFocusPosition()
            stop()
        }
    }

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            seecam130.getExposureCompensation()
            seecam130.getFrameRateCtrlValue()
            stop()
        }
    }

    // Used when selecting auto focus in image Quality settings menu
    Timer {
        id: getAutoFocusControlValues
        interval: 500
        onTriggered: {
            seecam130.getAutoFocusROIModeAndWindowSize()
            stop()
        }
    }

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            seecam130.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Action {
        id: triggerAction
        onTriggered: {
            seecam130.setAutoFocusMode(See3Cam130.SINGLE_TRIGGER)
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

    Action {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle

        Item{
            height: 2500

            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text {
                id: scene_mode
                text: "--- Scene Mode ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
                }


                Grid {
                    columns: 2
                    spacing: 30
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: sceneInputGroup }
                    RadioButton {
                        id: sceneNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: sceneInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecam130.setSceneMode(See3Cam130.SCENE_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setSceneMode(See3Cam130.SCENE_NORMAL)
                        }
                    }
                    RadioButton {
                        id: sceneDoc
                        style:  econRadioButtonStyle
                        text: qsTr("Document")
                        exclusiveGroup: sceneInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecam130.setSceneMode(See3Cam130.SCENE_DOCUMENT)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setSceneMode(See3Cam130.SCENE_DOCUMENT)
                        }
                    }
                }

                Text {
                    id: special_effects
                    text: "--- Special Effects ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Grid {
                    columns: 2
                    spacing: 20

                    ExclusiveGroup { id: effectInputGroup }
                    RadioButton {
                        id: effectNormal
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        //checked: true
                        onClicked: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_NORMAL)
                        }
                    }
                    RadioButton {
                        id: effectBW
                        style:  econRadioButtonStyle
                        text: qsTr("Black and White")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_BLACK_WHITE)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_BLACK_WHITE)
                        }
                    }
                    RadioButton {
                        id: effectNegative
                        style:  econRadioButtonStyle
                        text: qsTr("Negative")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_NEGATIVE)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_NEGATIVE)
                        }
                    }
                    RadioButton {
                        id: effectGrayscale
                        style:  econRadioButtonStyle
                        text: qsTr("Grayscale")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_GREYSCALE)
                        }
                    }
                    RadioButton {
                        id: effectSketch
                        style:  econRadioButtonStyle
                        text: qsTr("Sketch")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_SKETCH)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setEffectMode(See3Cam130.EFFECT_SKETCH)
                        }
                    }
                }


                Text {
                    id: denoiseText
                    text: "--- De-Noise ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 35
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: deNoiseSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: denoiseMin
                        maximumValue: denoiseMax
                        onValueChanged:  {
                            deNoiseTextField.text = deNoiseSlider.value
                            if(skipUpdateUIDenoise){
                                seecam130.setDenoiseValue(deNoiseSlider.value)
                            }
                            skipUpdateUIDenoise = true
                        }
                    }
                    TextField {
                        id: deNoiseTextField
                        text: deNoiseSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: deNoiseSlider.minimumValue; top: deNoiseSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                deNoiseSlider.value = deNoiseTextField.text
                            }
                        }
                    }
                }

                Text {
                    id: autoFocusMode
                    text: "--- Auto Focus Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                      spacing:10
                      ExclusiveGroup { id: afgroup }
                      RadioButton {
                          exclusiveGroup: afgroup
                          id: radioContin
                          text: "Continuous"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          onClicked: {
                            seecam130.setAutoFocusMode(See3Cam130.CONTINIOUS_SCAN);
                          }
                          Keys.onReturnPressed: {
                            seecam130.setAutoFocusMode(See3Cam130.CONTINIOUS_SCAN);
                          }
                      }
                }
                Row{
                    spacing:25
                    RadioButton {
                        exclusiveGroup: afgroup
                        id: radioOneshot
                        text: "One-Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            seecam130.setAutoFocusMode(See3Cam130.SINGLE_TRIGGER);
                        }
                        Keys.onReturnPressed: {
                            seecam130.setAutoFocusMode(See3Cam130.SINGLE_TRIGGER);
                        }
                    }
                    Button {
                        id: trigger
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (radioOneshot.enabled && radioOneshot.checked) ? true : false
                        opacity: (radioOneshot.enabled && radioOneshot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        action: (radioOneshot.enabled && radioOneshot.checked) ? triggerAction : null
                        Keys.onReturnPressed: {
                            seecam130.setAutoFocusMode(See3Cam130.SINGLE_TRIGGER);
                        }
                    }
                }

            Text {
                    id: roiAutoFocusMode
                    text: "--- ROI - Auto Focus ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                      spacing:55
                      ExclusiveGroup { id: roiAfgroup }
                      Layout.alignment: Qt.AlignCenter
                      RadioButton {
                          exclusiveGroup: roiAfgroup
                          id: afCentered
                          text: "Centered"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: afCentered.enabled ? 1 : 0.1
                          // setROIAutoFocus() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                          // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                          // winSize is required only for manual mode
                          onClicked: {
                                seecam130.setROIAutoFocus(See3Cam130.AF_CENTERED_ROI, 0, 0, 0, 0, 0);
                                afWindowSizeCombo.enabled = false
                          }
                          Keys.onReturnPressed: {
                                seecam130.setROIAutoFocus(See3Cam130.AF_CENTERED_ROI, 0, 0, 0, 0, 0);
                                afWindowSizeCombo.enabled = false
                          }
                      }
                      RadioButton {
                          exclusiveGroup: roiAfgroup
                          id: afManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: afManual.enabled ? 1 : 0.1
                          onClicked: {
                                seecam130.setROIAutoFocus(See3Cam130.AF_MANUAL_ROI, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                                afWindowSizeCombo.enabled = true
                          }
                          Keys.onReturnPressed: {
                                seecam130.setROIAutoFocusMode(See3Cam130.AF_MANUAL_ROI, 0, 0, 0, 0, afWindowSizeCombo.currentText);
                                afWindowSizeCombo.enabled = true
                          }
                      }
                }

                ComboBox
                {
                    id: afWindowSizeCombo
                    enabled: (afManual.enabled && afManual.checked) ? true : false
                    opacity: (afManual.enabled && afManual.checked) ? 1 : 0.1
                    model: ListModel {
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
                        if(skipUpdateUIOnAFWindowSize){
                            seecam130.setROIAutoFocus(See3Cam130.AF_MANUAL_ROI, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                        }
                        skipUpdateUIOnAFWindowSize = true
                    }
                }

                Text {
                    id: roiAutoExpMode
                    text: "--- ROI - Auto Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                      spacing:38
                      ExclusiveGroup { id: roiExpogroup }
                      Layout.alignment: Qt.AlignCenter

                      RadioButton {
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
                              seecam130.setROIAutoExposure(See3Cam130.AE_CENTERED_ROI, 0, 0, 0, 0, 0);
                              autoExpoWinSizeCombo.enabled = false
                          }
                          Keys.onReturnPressed: {
                              seecam130.setROIAutoExposure(See3Cam130.AE_CENTERED_ROI, 0, 0, 0, 0, 0);
                              autoExpoWinSizeCombo.enabled = false
                          }
                      }
                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: autoexpManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1
                          onClicked: {
                              seecam130.setROIAutoExposure(See3Cam130.AE_MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                              autoExpoWinSizeCombo.enabled = true
                          }
                          Keys.onReturnPressed: {
                              seecam130.setROIAutoExposure(See3Cam130.AE_MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                              autoExpoWinSizeCombo.enabled = true
                          }
                      }
                }

                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (autoexpManual.enabled && autoexpManual.checked) ? true : false
                    opacity: (autoexpManual.enabled && autoexpManual.checked) ? 1 : 0.1
                    model: ListModel {
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
                            seecam130.setROIAutoExposure(See3Cam130.AE_MANUAL_ROI, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                        }
                        skipUpdateUIOnExpWindowSize = true
                    }
                }


                Text {
                    id: imgCapText
                    text: "--- Image Capture ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Text {
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
                    model: ListModel {
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
                            seecam130.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }


                Text {
                    id: iHdrMode
                    text: "--- iHDR ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:25
                    ExclusiveGroup { id: iHDRgroup }
                    RadioButton {
                        exclusiveGroup: iHDRgroup
                        id: hdrOff
                        text: "Off"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            seecam130.setiHDRMode(See3Cam130.HdrOff, iHDRCombo.currentText)
                            iHDRCombo.enabled = false
                            iHDRCombo.opacity = 0.1
                        }

                        Keys.onReturnPressed: {
                            seecam130.setiHDRMode(See3Cam130.HdrOff, iHDRCombo.currentText)
                            iHDRCombo.enabled = false
                            iHDRCombo.opacity = 0.1
                        }
                    }
                    RadioButton {
                        exclusiveGroup: iHDRgroup
                        id:hdrAuto
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            seecam130.setiHDRMode(See3Cam130.HdrAuto, iHDRCombo.currentText)
                            iHDRCombo.enabled = false
                            iHDRCombo.opacity = 0.1
                        }

                        Keys.onReturnPressed: {
                            seecam130.setiHDRMode(See3Cam130.HdrAuto, iHDRCombo.currentText)
                            iHDRCombo.enabled = false
                            iHDRCombo.opacity = 0.1
                        }
                    }
                    RadioButton {
                        exclusiveGroup: iHDRgroup
                        id: hdrManual
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            seecam130.setiHDRMode(See3Cam130.HdrManual, iHDRCombo.currentText)
                            iHDRCombo.enabled = true
                            iHDRCombo.opacity = 1
                        }
                        Keys.onReturnPressed: {
                            seecam130.setiHDRMode(See3Cam130.HdrManual, iHDRCombo.currentText)
                            iHDRCombo.enabled = true
                            iHDRCombo.opacity = 1
                        }
                    }
                }
                Row{
                    spacing: 35
                    ComboBox
                    {
                        id: iHDRCombo
                        opacity: 1
                        enabled: true
                        model: ListModel
                               {
                                    ListElement { text: "1" }
                                    ListElement { text: "2" }
                                    ListElement { text: "4" }
                                    ListElement { text: "8" }
                                }
                        activeFocusOnPress: true
                        style: econComboBoxStyle
                        onCurrentIndexChanged: {
                            if(skipUpdateUIiHDR){
                                setiHDRValues()
                            }
                            skipUpdateUIiHDR = true
                        }
                    }
                }


                Text {
                    id: qFactorText
                    text: "--- Q Factor ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 35
                    Slider {
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
                                seecam130.setQFactor(qFactorSlider.value)
                            }
                            skipUpdateUIQFactor = true
                        }
                    }
                    TextField {
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
                    id: enableDisableAFRectText
                    text: "--- Enable/Disable AF Rectangle ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:75
                    ExclusiveGroup { id: afRectGroup }
                    Layout.alignment: Qt.AlignCenter
                    RadioButton {
                        exclusiveGroup: afRectGroup
                        id: rectEnable
                        text: "Enable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            seecam130.enableDisableAFRectangle(true)
                        }
                        Keys.onReturnPressed: {
                            seecam130.enableDisableAFRectangle(true)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: afRectGroup
                        id:rectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            seecam130.enableDisableAFRectangle(false)
                        }
                        Keys.onReturnPressed: {
                            seecam130.enableDisableAFRectangle(false)
                        }
                    }
                }


                Text {
                    id: streamModeText
                    text: "--- Stream Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing:68
                    ExclusiveGroup { id: streamModeGroup }
                    Layout.alignment: Qt.AlignCenter
                    RadioButton {
                        exclusiveGroup: streamModeGroup
                        id: streamMaster
                        text: "Master"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            setMasterMode()
                        }
                        Keys.onReturnPressed: {
                            setMasterMode()
                        }
                    }
                    RadioButton {
                        exclusiveGroup: streamModeGroup
                        id: streamTrigger
                        text: "Trigger"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            setTriggerMode()
                        }
                    }
                }


                Text {
                    id: flipText
                    text: "--- Flip Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 55
                    Layout.alignment: Qt.AlignCenter
                    CheckBox {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        onClicked:{
                            seecam130.setFlipHorzMode(checked)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setFlipHorzMode(checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            seecam130.setFlipVertiMode(checked)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setFlipVertiMode(checked)
                        }
                    }
                }

                Text {
                    id: autoWhiteBalanceTitle
                    text: "--- Current Auto White Balance ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                        spacing: 9

                        Text {
                            id: autoWBText
                            text: "(in Kelvin)"
                            font.pixelSize: 13
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            width: 80
                            wrapMode: Text.WordWrap
                            opacity: 1
                        }
                        TextField {
                            id: autoWBTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            readOnly: true
                            horizontalAlignment: TextInput.AlignHCenter
                            opacity: 1
                            style: econTextFieldStyle
                            implicitHeight: 25
                            implicitWidth: 80
                        }
                        Button
                        {
                            id: getAutoWBBtn
                            activeFocusOnPress : true
                            text: "Get"
                            tooltip: "You can get the Current Auto White Balance value by clicking the Get button"
                            style: econButtonStyle
                            enabled: true
                            opacity: 1
                            implicitHeight: 25
                            implicitWidth: 60
                            onClicked:
                            {
                                getAutoWBBtn.enabled = false
                                setButtonClicked = true
                                seecam130.getAutoWhiteBalance()
                                getAutoWBBtn.enabled = true
                            }
                            Keys.onReturnPressed:
                            {
                                getAutoWBBtn.enabled = false
                                setButtonClicked = true
                                seecam130.getAutoWhiteBalance()
                                getAutoWBBtn.enabled = true
                            }
                        }
                }

                Text {
                    id: autoExposureTitle
                    text: "--- Current Auto Exposure ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                        spacing: 9
                        Text {
                            id: autoExpText
                            text: "(In MicroSeconds)"
                            font.pixelSize: 13
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            width: 80
                            wrapMode: Text.WordWrap
                            opacity: 1
                        }
                        TextField {
                            id: autoExpTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            readOnly: true
                            horizontalAlignment: TextInput.AlignHCenter
                            opacity: 1
                            style: econTextFieldStyle
                            implicitHeight: 25
                            implicitWidth: 80
                        }
                        Button
                        {
                            id: getAutoExpBtn
                            activeFocusOnPress : true
                            text: "Get"
                            tooltip: "You can get the Current Auto Exposure value by clicking the Get button"
                            style: econButtonStyle
                            enabled: true
                            opacity: 1
                            implicitHeight: 25
                            implicitWidth: 60
                            onClicked:
                            {
                                getAutoExpBtn.enabled = false
                                setButtonClicked = true
                                seecam130.getAutoExposure()
                                getAutoExpBtn.enabled = true
                            }
                            Keys.onReturnPressed:
                            {
                                getAutoExpBtn.enabled = false
                                setButtonClicked = true
                                seecam130.getAutoExposure()
                                getAutoExpBtn.enabled = true
                            }
                        }
                }

                Text{
                    id: flashModeText
                    x: 85
                    y: 200
                    text: "--- Flash Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                RowLayout{
                    x:25
                    y:230
                    spacing: 25
                    ExclusiveGroup { id: flashGrp }
                    Column{
                        RadioButton {
                            id: flashModeOff
                            exclusiveGroup: flashGrp
                            checked: false
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                seecam130.setFlashMode(See3Cam130.DISABLE)
                            }
                            Keys.onReturnPressed: {
                                seecam130.setFlashMode(See3Cam130.DISABLE)
                            }
                        }
                    }
                    Column{
                        RadioButton {
                            id: flashModeStrobe
                            exclusiveGroup: flashGrp
                            checked: false
                            text: "Strobe"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                seecam130.setFlashMode(See3Cam130.STROBE)
                            }
                            Keys.onReturnPressed: {
                                seecam130.setFlashMode(See3Cam130.STROBE)
                            }
                        }
                    }
                    Column{
                        RadioButton {
                            id: flashModeTorch
                            exclusiveGroup: flashGrp
                            checked: false
                            text: "Torch"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                seecam130.setFlashMode(See3Cam130.TORCH)
                            }
                            Keys.onReturnPressed: {
                                seecam130.setFlashMode(See3Cam130.TORCH)
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
                    spacing: 62
                    ExclusiveGroup { id: faceRectGroup }
                    RadioButton {
                        exclusiveGroup: faceRectGroup
                        id: faceRectEnable
                        text: "Enable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            seecam130.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: faceRectGroup
                        id:faceRectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            seecam130.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
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
                        text: "Overlay Rectangle"
                        style: econCheckBoxTextWrapModeStyle
                        enabled: faceRectEnable.checked ? true : false
                        opacity: enabled ? 1 : 0.1
                        onClicked:{
                            seecam130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        }
                        Keys.onReturnPressed: {
                            seecam130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        }
                    }
                }


                Text {
                    id: autoFocusPositionTitle
                    text: "--- Current Auto Focus Position ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                        spacing: 9
                        Text {
                            id: autoFocusPositionText
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            width: 80
                            wrapMode: Text.WordWrap
                            opacity: 1
                        }
                        TextField {
                            id: autoFocusPositionTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            readOnly: true
                            horizontalAlignment: TextInput.AlignHCenter
                            opacity: 1
                            style: econTextFieldStyle
                            implicitHeight: 25
                            implicitWidth: 80
                        }
                        Button
                        {
                            id: getFocusPosition
                            activeFocusOnPress : true
                            text: "Get"
                            tooltip: "You can get the Focus Position by clicking the Get button"
                            style: econButtonStyle
                            enabled: true
                            opacity: 1
                            implicitHeight: 25
                            implicitWidth: 60
                            onClicked:
                            {
                                getFocusPosition.enabled = false
                                setButtonClicked = true
                                seecam130.getAutoFocusPosition()
                                getFocusPosition.enabled = true
                            }
                            Keys.onReturnPressed:
                            {
                                getFocusPosition.enabled = false
                                setButtonClicked = true
                                seecam130.getAutoFocusPosition()
                                getFocusPosition.enabled = true
                            }
                        }
                }

                Text {
                        id: exposureCompTextTitle
                        text: "--- Exposure Compensation ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }
                Row{
                        spacing: 9
                        Text {
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
                        TextField {
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
                        Button {
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
                            onClicked: {
                                exposureCompSet.enabled = false
                                setButtonClicked = true
                                seecam130.setExposureCompensation(exposureCompValue.text)
                                exposureCompSet.enabled = true
                            }
                            Keys.onReturnPressed: {
                                exposureCompSet.enabled = false
                                setButtonClicked = true
                                seecam130.setExposureCompensation(exposureCompValue.text)
                                exposureCompSet.enabled = true
                            }
                        }
                    }

                Text {
                    id: frameRateText
                    text: "--- Frame Rate Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 35
                    Slider {
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
                                seecam130.setFrameRateCtrlValue(frameRateSlider.value)
                            }
                            skipUpdateUIFrameRate = true
                        }
                    }
                    TextField {
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
                    id: flickerctrlField
                    text: "-- Flicker Detection Control --"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ComboBox
                {
                    id: flickercombo
                    opacity: 1
                    enabled: true
                    model: ListModel {
                        ListElement { text: "50Hz" }
                        ListElement { text: "60Hz" }
                        ListElement { text: "DISABLE" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIFlickerCtrl){
                           setFlickerDetectionFn();
                        }
                    }
                }

                //Red Gain
                Text
                {
                    id: redGainTitle
                    text: "--- Red Gain ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 35
                    Slider {
                        id: redGainSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        stepSize: 1
                        minimumValue: redGainSlider.minimumValue
                        maximumValue: redGainSlider.maximumValue
                        onValueChanged:  {
                            redGainTextField.text = redGainSlider.value
                            if(skipUpdateUIOnRedGain){
                                seecam130.setRedGain(redGainSlider.value)
                            }
                            skipUpdateUIOnRedGain = true
                        }
                    }
                    TextField {
                        id: redGainTextField
                        text: redGainSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: redGainSlider.minimumValue; top: redGainSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                redGainSlider.value = redGainTextField.text
                            }
                        }
                    }
                }

                //Blue Gain
                Text
                {
                    id: blueGainTitle
                    text: "--- Blue Gain ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 35
                    Slider {
                        id: blueGainSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        stepSize: 1
                        minimumValue: blueGainSlider.minimumValue
                        maximumValue: blueGainSlider.maximumValue
                        onValueChanged:  {
                            blueGainTextField.text = blueGainSlider.value
                            if(skipUpdateUIOnBlueGain){
                                seecam130.setBlueGain(blueGainSlider.value)
                            }
                            skipUpdateUIOnBlueGain = true
                        }
                    }
                    TextField {
                        id: blueGainTextField
                        text: blueGainSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: blueGainSlider.minimumValue; top: blueGainSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                blueGainSlider.value = blueGainTextField.text
                            }
                        }
                    }
                }

                //Green Gain
                Text
                {
                    id: greenGainTitle
                    text: "--- Green Gain ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 35
                    Slider {
                        id: greenGainSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        stepSize: 1
                        minimumValue: greenGainSlider.minimumValue
                        maximumValue: greenGainSlider.maximumValue
                        onValueChanged:  {
                            greenGainTextField.text = greenGainSlider.value
                            if(skipUpdateUIOnGreenGain){
                                seecam130.setGreenGain(greenGainSlider.value)
                            }
                            skipUpdateUIOnGreenGain = true
                        }
                    }
                    TextField {
                        id: greenGainTextField
                        text: greenGainSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: greenGainSlider.minimumValue; top: greenGainSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                greenGainSlider.value = greenGainTextField.text
                            }
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
                        action: setDefault
                        style: econButtonStyle
                        Keys.onReturnPressed: {
                            setToDefaultValues()
                        }
                    }
                }

                Row{
                    Button {
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
                    Button {
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
                Row{
                    Button {
                        id: dummy
                        opacity: 0
                    }
                }
            }
        }
    }


    See3Cam130 {
        id: seecam130
        onSceneModeValueReceived: {
            currentSceneMode(sceneMode)
        }
        onEffectModeValueReceived: {
            currentEffectMode(effectMode)
        }
        onDenoiseValueReceived:{
            skipUpdateUIDenoise = false
            deNoiseSlider.value = denoiseValue
            skipUpdateUIDenoise = true
        }
        onAfModeValueReceived:{
            currentAfMode(afMode)
        }
        onRoiAfModeValueReceived:{
            if(roiMode == See3Cam130.AF_CENTERED_ROI){
                afCentered.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam130.AF_MANUAL_ROI){
                skipUpdateUIOnAFWindowSize = false
                afManual.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }
        }
        onRoiAutoExpModeValueReceived:{
            currentROIAutoExposureMode(roiMode, winSize)
        }
        onBurstLengthValueReceived:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onHDRModeValueReceived:{
            currentHDRMode(hdrMode)
            if(hdrMode == See3Cam130.HdrManual){
                switch(hdrValue)
                {
                    case 1:
                        iHDRCombo.currentIndex = 0
                        break
                    case 2:
                        iHDRCombo.currentIndex = 1
                        break
                    case 4:
                        iHDRCombo.currentIndex = 2
                        break
                    case 8:
                        iHDRCombo.currentIndex = 3
                        break
                 }
            }
        }
        onQFactorValueReceived:{
              skipUpdateUIQFactor = false
              qFactorSlider.value = qFactor
              skipUpdateUIQFactor = true
        }

        onStreamModeValueReceived:{
            currentStreamMode(streamMode)
        }
        onFlipModeValue:{
           updateFlipMode(flipMode, flipEnableDisableMode)
        }
        onAutoWhiteBalanceReceived:{
            autoWBTextField.text = autoWhiteBalance
        }
        onAutoExposureReceived:{
            autoExpTextField.text = autoExposure
        }
        onFlashModeReceived:{
            currentFlashMode(flashMode)
        }
        onFaceDetectModeValueReceived:{
            if(faceDetectMode == See3Cam130.FaceRectEnable){
                faceRectEnable.checked = true
                if(faceDetectEmbedDataValue == See3Cam130.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }
                if(faceDetectOverlayRect == See3Cam130.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }
            }else if(faceDetectMode == See3Cam130.FaceRectDisable){
                faceRectDisable.checked = true
                if(faceDetectEmbedDataValue == See3Cam130.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }else{
                    faceDetectEmbedData.checked = false
                }
                if(faceDetectOverlayRect == See3Cam130.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }else{
                    overlayRect.checked = false
                }
            }
        }
        onAutoFocusPositionReceived:{
            autoFocusPositionTextField.text = afPosition
        }
        onExposureCompValueReceived:{
            exposureCompValue.text = exposureCompensation
        }
        onFrameRateCtrlValueReceived:{
            skipUpdateUIFrameRate = false
            frameRateSlider.value = frameRateCtrlValue
            skipUpdateUIFrameRate = true
        }
        onFlickerDetectionModeReceived:{
            skipUpdateUIFlickerCtrl = false
            if(flickerMode == See3Cam130.MODE_50Hz){
                flickercombo.currentIndex = 0
            }else if(flickerMode == See3Cam130.MODE_60Hz){
                flickercombo.currentIndex  = 1
            }else if(flickerMode == See3Cam130.MODE_DISABLE){
                flickercombo.currentIndex  = 2
            }else{ }
            skipUpdateUIFlickerCtrl = true;
        }

        onRedGainCurrentReceived: {
            skipUpdateUIOnRedGain      = false
            redGainSlider.value        = currentRed
            skipUpdateUIOnRedGain      = true
        }

        onRedGainMinReceived: {
            redGainSlider.minimumValue = minRed
        }
        onRedGainMaxReceived: {
            redGainSlider.maximumValue = maxRed
        }

        onBlueGainCurrentReceived: {
            skipUpdateUIOnBlueGain      = false
            blueGainSlider.value        = currentBlue
            skipUpdateUIOnBlueGain      = true
        }

        onBlueGainMinReceived: {
            blueGainSlider.minimumValue = minBlue
        }
        onBlueGainMaxReceived: {
            blueGainSlider.maximumValue = maxBlue
        }

        onGreenGainCurrentReceived: {
            skipUpdateUIOnGreenGain      = false
            greenGainSlider.value        = currentGreen
            skipUpdateUIOnGreenGain      = true
        }

        onGreenGainMinReceived: {
            greenGainSlider.minimumValue = minGreen
        }
        onGreenGainMaxReceived: {
            greenGainSlider.maximumValue = maxGreen
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
                seecam130.getExposureCompensation()
            }
        }

        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
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
    }}

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

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function currentStreamMode(streamMode)
    {
        if(streamMode == See3Cam130.STREAM_MASTER){
            streamMaster.checked = true
            root.captureBtnEnable(true)
            root.videoRecordBtnEnable(true)
        }else if(streamMode == See3Cam130.STREAM_TRIGGER){
            streamTrigger.checked = true
            root.captureBtnEnable(false)
            root.videoRecordBtnEnable(false)
            displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document."))
        }
    }

    function updateFlipMode(flipMode, FlipEnableDisableMode){
        switch(flipMode){
        case See3Cam130.FlipHorizontal:
            if(FlipEnableDisableMode == See3Cam130.FlipEnable){
                flipCtrlHorizotal.checked = true
            }else{
                flipCtrlHorizotal.checked = false
            }
            break;
        case See3Cam130.FlipVertical:
            if(FlipEnableDisableMode == See3Cam130.FlipEnable){
                flipCtrlVertical.checked = true
            }else{
                flipCtrlVertical.checked = false
            }
            break;
        case See3Cam130.FlipBoth:
            if(FlipEnableDisableMode == See3Cam130.FlipEnable){
                flipCtrlHorizotal.checked = true
                flipCtrlVertical.checked = true
            }else{
                flipCtrlHorizotal.checked = false
                flipCtrlVertical.checked = false
            }
            break;
        }

    }


    function currentFlashMode(flashMode)
    {
        if(flashMode == See3Cam130.DISABLE){
            flashModeOff.checked = true
        }else if(flashMode == See3Cam130.STROBE){
            flashModeStrobe.checked = true
        }else if(flashMode == See3Cam130.TORCH){
            flashModeTorch.checked = true
        }
    }

    function saveConfigurations()
    {
        seecam130.saveConfiguration()
    }

    function setiHDRValues()
    {
        if(iHDRCombo.currentIndex == 0)
        {
            seecam130.setiHDRMode(See3Cam130.HdrManual, 1)

        }
        else if(iHDRCombo.currentIndex == 1)
        {
            seecam130.setiHDRMode(See3Cam130.HdrManual, 2)

        }
        else if(iHDRCombo.currentIndex == 2)
        {
            seecam130.setiHDRMode(See3Cam130.HdrManual, 4)

        }
        else if(iHDRCombo.currentIndex == 3)
        {
            seecam130.setiHDRMode(See3Cam130.HdrManual, 8)
        }
    }

    // current ROI auto exposure mode
    function currentROIAutoExposureMode(roiMode, winSize){
        switch(roiMode){
            case See3Cam130.AE_CENTERED_ROI:
                autoexpFull.checked = true
                autoExpoWinSizeCombo.enabled = false
                break
            case See3Cam130.AE_MANUAL_ROI:
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
        }
    }

    function setMasterMode(){
        seecam130.setStreamMode(See3Cam130.STREAM_MASTER)
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
    }

    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        seecam130.setStreamMode(See3Cam130.STREAM_TRIGGER)
        displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document See3CAM_130_Trigger_Mode"))
    }

    function getSerialNumber() {
        seecam130.get64BitSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }
    function setToDefaultValues(){
        root.checkForTriggerMode(false)

        seecam130.setToDefault()
        seecam130.getSceneMode()
        seecam130.getEffectMode()
        seecam130.getDenoiseValue()
        seecam130.getAutoFocusMode()
        seecam130.getAutoFocusROIModeAndWindowSize()
        seecam130.getAutoExpROIModeAndWindowSize()
        seecam130.getBurstLength()
        seecam130.getiHDRMode()
        seecam130.getQFactor()
        seecam130.getStreamMode()
        seecam130.getFlipMode()
        seecam130.getAutoWhiteBalance()
        seecam130.getAutoExposure()
        seecam130.getFlashMode()
        seecam130.getFaceDetectMode()
        seecam130.getAutoFocusPosition()
        seecam130.getExposureCompensation()
        seecam130.getFrameRateCtrlValue()
        seecam130.getFlickerDetection()
        seecam130.getRedGain()
        seecam130.getBlueGain()
        seecam130.getGreenGain()

        //To get preview in master mode
        root.startUpdatePreviewInMasterMode()
    }

    function currentSceneMode(mode)
    {
        switch(mode)
        {
            case See3Cam130.SCENE_NORMAL:
                sceneNormal.checked = true
                break;
            case See3Cam130.SCENE_DOCUMENT:
                sceneDoc.checked = true
                break;
        }
    }
    function currentEffectMode(mode)
    {
        switch(mode)
        {
            case See3Cam130.EFFECT_NORMAL:
                effectNormal.checked = true
                break;
            case See3Cam130.EFFECT_BLACK_WHITE:
                effectBW.checked = true
                break;
            case See3Cam130.EFFECT_GREYSCALE:
                effectGrayscale.checked = true
                break;
            case See3Cam130.EFFECT_NEGATIVE:
                effectNegative.checked = true
                break;
            case See3Cam130.EFFECT_SKETCH:
                effectSketch.checked = true
                break;
        }
    }

    function currentAfMode(mode)
    {
        switch(mode)
        {
            case See3Cam130.CONTINIOUS_SCAN:
                radioContin.checked = true
                break;
            case See3Cam130.SINGLE_TRIGGER:
                radioOneshot.checked = true
                break;
        }
    }

    function currentHDRMode(mode)
    {
        switch(mode)
        {
            case See3Cam130.HdrOff:
                hdrOff.checked = true

                iHDRCombo.enabled = false
                iHDRCombo.opacity = 0.1
                break;
            case See3Cam130.HdrAuto:
                hdrAuto.checked = true

                iHDRCombo.enabled = false
                iHDRCombo.opacity = 0.1
                break;
            case See3Cam130.HdrManual:
                hdrManual.checked = true

                iHDRCombo.enabled = true
                iHDRCombo.opacity = 1
                break;
        }
    }

    function enableFaceDetectEmbedData(){
        if(seecam130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_130_Face_and_Smile_Detection for more details"))
            }
        }
    }

    function setFlickerDetectionFn()
    {
        switch(flickercombo.currentIndex){
        case 0:
            flickerCtrl= See3Cam130.MODE_50Hz
            break
        case 1:
            flickerCtrl = See3Cam130.MODE_60Hz
            break
        case 2:
            flickerCtrl = See3Cam130.MODE_DISABLE
            break
        }
        seecam130.setFlickerDetection(flickerCtrl)
    }

    function enableDisableAutoFocusUIControls(autoFocusSelect){
        if(autoFocusSelect){
            radioContin.enabled = true
            radioOneshot.enabled = true
            trigger.enabled = true
            afCentered.enabled = true
            afManual.enabled = true
            if(afCentered.checked)
                afWindowSizeCombo.enabled = false
            if(afManual.checked)
                afWindowSizeCombo.enabled = true
            rectEnable.enabled = true
            rectEnable.opacity = 1
            rectDisable.enabled = true
            rectDisable.opacity = 1
            radioContin.opacity = 1
            radioOneshot.opacity = 1
            afCentered.opacity = 1
            afManual.opacity = 1
        }else{
            radioContin.enabled = false
            radioOneshot.enabled = false
            trigger.enabled = false
            afCentered.enabled = false
            afManual.enabled = false
            afWindowSizeCombo.enabled = false
            radioContin.opacity = 0.1
            radioOneshot.opacity = 0.1
            afCentered.opacity = 0.1
            afManual.opacity = 0.1
            rectEnable.enabled = false
            rectEnable.opacity = 0.1
            rectDisable.enabled = false
            rectDisable.opacity = 0.1
        }
        getAutoFocusControlValues.start()
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){

            //Disabling RGB Gain in Auto exposure mode
            redGainSlider.enabled    = false
            redGainSlider.opacity    = 0.1
            redGainTextField.enabled = false
            redGainTextField.opacity = 0.1

            blueGainSlider.enabled    = false
            blueGainSlider.opacity    = 0.1
            blueGainTextField.enabled = false
            blueGainTextField.opacity = 0.1

            greenGainSlider.enabled    = false
            greenGainSlider.opacity    = 0.1
            greenGainTextField.enabled = false
            greenGainTextField.opacity = 0.1

            autoexpManual.enabled = true
            autoexpFull.enabled = true
            if(autoexpManual.checked)
                autoExpoWinSizeCombo.enabled = true
            if(autoexpFull.checked)
                autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 1
            autoexpFull.opacity = 1
            exposureCompValue.enabled = true
            exposureCompValue.opacity = 1
            exposureCompSet.enabled = true
            exposureCompSet.opacity = 1
            exposureCompText.opacity = 1
        }else{

            //Enabling RGB Gain in manual exposure mode
            redGainSlider.enabled    = true
            redGainSlider.opacity    = 1
            redGainTextField.enabled = true
            redGainTextField.opacity = 1

            blueGainSlider.enabled    = true
            blueGainSlider.opacity    = 1
            blueGainTextField.enabled = true
            blueGainTextField.opacity = 1

            greenGainSlider.enabled    = true
            greenGainSlider.opacity    = 1
            greenGainTextField.enabled = true
            greenGainTextField.opacity = 1

            autoexpManual.enabled = false
            autoexpFull.enabled = false
            autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 0.1
            autoexpFull.opacity = 0.1
            exposureCompValue.enabled = false
            exposureCompValue.opacity = 0.1
            exposureCompSet.enabled = false
            exposureCompSet.opacity = 0.1
            exposureCompText.opacity = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    function getCurrentValuesFromCamera(){
        //getting valid effect mode and scene mode takes some time.
        //So In timer, after 500 ms, getting effect mode and scene mode is done
        getCamValuesTimer.start()

        seecam130.getAutoFocusMode()
        seecam130.getAutoFocusROIModeAndWindowSize()
        seecam130.getAutoExpROIModeAndWindowSize()
        seecam130.getBurstLength()
        seecam130.getiHDRMode()
        seecam130.getStreamMode()
        seecam130.getFlipMode()
        seecam130.getAutoWhiteBalance()
        seecam130.getAutoExposure()
        seecam130.getFlashMode()
        seecam130.getFaceDetectMode()
        seecam130.getAutoFocusPosition()
        seecam130.getFlickerDetection()
        seecam130.getRedGain()
        seecam130.getBlueGain()
        seecam130.getGreenGain()

        root.disablePowerLineFreq()   //Added by Navya-4th June 2019 - call to disable Power Line Frequency.
    }

    Component.onCompleted:{
        getCurrentValuesFromCamera()
    }

}

