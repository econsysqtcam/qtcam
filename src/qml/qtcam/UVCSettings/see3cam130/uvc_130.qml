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
    property int defaultSmileThreshold: 40
    property int iHDRMin: 1
    property int iHDRMax: 4   
    // Flags to prevent setting values in camera when getting the values from camera    
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIDenoise : false
    property bool skipUpdateUIFrameRate: false
    property bool skipUpdateUIOnAFWindowSize: false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIiHDR: false
    property bool setButtonClicked: false

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            seecam130.enableDisableAFRectangle(false)
            seecam130.enableDisableFaceRectangle(false)
            burstShotTimer.start()
            // In trigger mode, if frames are not coming then after 3 seconds enable all settings
            if(streamTrigger.checked){
                enableSettings.start()
            }
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
    }

    Timer {
        id: burstShotTimer
        interval: 1000
        onTriggered: {
            root.imageCapture(CommonEnums.BURST_SHOT);
            // Added by Sankari: 20 Apr 2017
            // Disable saving image when trigger mode is selected initially and capturing image then switch to master mode
            if(streamTrigger.checked)
                root.disableSaveImage()
            stop()
        }
    }

    Timer {
        id: enableSettings
        interval: 3000
        onTriggered: {
            root.enableAllSettingsTab()
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
            seecam130.setAutoFocusMode(See3Cam130.OneShot)
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
                spacing: 20

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
                        seecam130.setAutoFocusMode(See3Cam130.Continuous);
                      }
                      Keys.onReturnPressed: {
                        seecam130.setAutoFocusMode(See3Cam130.Continuous);
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
                        seecam130.setAutoFocusMode(See3Cam130.OneShot);
                    }
                    Keys.onReturnPressed: {
                        seecam130.setAutoFocusMode(See3Cam130.OneShot);
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
                        seecam130.setAutoFocusMode(See3Cam130.OneShot);
                    }
                }
            }
	    Text {
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
                  RadioButton {
                      exclusiveGroup: roiAfgroup
                      id: afCentered
                      text: "Centered"
                      activeFocusOnPress: true
                      style: econRadioButtonStyle
                      opacity: afCentered.enabled ? 1 : 0.1
                      // setROIAutoFoucs() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                      // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                      // winSize is required only for manual mode
                      onClicked: {
                            seecam130.setROIAutoFoucs(See3Cam130.AFCentered, 0, 0, 0, 0, 0);
                            afWindowSizeCombo.enabled = false
                      }                      
                      Keys.onReturnPressed: {
                            seecam130.setROIAutoFoucs(See3Cam130.AFCentered, 0, 0, 0, 0, 0);
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
                            seecam130.setROIAutoFoucs(See3Cam130.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                            afWindowSizeCombo.enabled = true
                      }
                      Keys.onReturnPressed: {
                            seecam130.setROIAutoFocusMode(See3Cam130.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText);
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
                        seecam130.setROIAutoFoucs(See3Cam130.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                    }
                    skipUpdateUIOnAFWindowSize = true
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
                        seecam130.setiHDRMode(See3Cam130.HdrOff, 0)
                    }

                    Keys.onReturnPressed: {
                        seecam130.setiHDRMode(See3Cam130.HdrOff, 0)
                    }
                }
                RadioButton {
                    exclusiveGroup: iHDRgroup
                    id:hdrAuto
                    text: "Auto"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam130.setiHDRMode(See3Cam130.HdrAuto, 0)
                    }

                    Keys.onReturnPressed: {
                        seecam130.setiHDRMode(See3Cam130.HdrAuto, 0)
                    }
                }
                RadioButton {
                    exclusiveGroup: iHDRgroup
                    id: hdrManual
                    text: "Manual"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        seecam130.setiHDRMode(See3Cam130.HdrManual, iHDRSlider.value)
                    }
                    Keys.onReturnPressed: {
                        seecam130.setiHDRMode(See3Cam130.HdrManual, iHDRSlider.value)
                    }
                }
            }
            Row{
                spacing: 35
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: iHDRSlider
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: iHDRMin
                    maximumValue: iHDRMax
                    enabled: (hdrManual.enabled && hdrManual.checked) ? true : false
                    opacity: enabled ? 1 : 0.1
                    onValueChanged:  {
                        iHDRTextField.text = iHDRSlider.value
                        if(skipUpdateUIiHDR){
                            seecam130.setiHDRMode(See3Cam130.HdrManual, iHDRSlider.value)
                        }
                        skipUpdateUIiHDR = true

                    }
                }
                TextField {
                    id: iHDRTextField
                    text: iHDRSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: (hdrManual.enabled && hdrManual.checked) ? true : false
                    opacity: enabled ? 1 : 0.1
                    validator: IntValidator {bottom: iHDRSlider.minimumValue; top: iHDRSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            iHDRSlider.value = iHDRTextField.text
                        }
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
                RadioButton {
                    exclusiveGroup: streamModeGroup
                    id: streamMaster
                    text: "Master"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        root.startUpdatePreviewInMasterMode()
                        seecam130.setStreamMode(See3Cam130.STREAM_MASTER)
                    }
                    Keys.onReturnPressed: {
                        root.startUpdatePreviewInMasterMode()
                        seecam130.setStreamMode(See3Cam130.STREAM_MASTER)
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
                    onFocusChanged: {
                        // Disable saving image when focus is changed from trigger mode to master mode
                        // or changing to any other camera if it is m_saveImage flag set as true to avoid saving image.
                       root.disableSaveImage()
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
                  spacing:90
                  ExclusiveGroup { id: roiExpogroup }
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
                          seecam130.setROIAutoExposure(See3Cam130.AutoExpFull, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = false
                      }
                      Keys.onReturnPressed: {
                          seecam130.setROIAutoExposure(See3Cam130.AutoExpFull, 0, 0, 0, 0, 0);
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
                          seecam130.setROIAutoExposure(See3Cam130.AutoExpManual, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = true
                      }
                      Keys.onReturnPressed: {
                          seecam130.setROIAutoExposure(See3Cam130.AutoExpManual, 0, 0, 0, 0, 0);
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
                        seecam130.setROIAutoExposure(See3Cam130.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                    }
                    skipUpdateUIOnExpWindowSize = true
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
                id: smileDetectionText
                text: "--- Smile Detection ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Row{
                spacing: 62
                ExclusiveGroup { id: smileDetectGroup }
                RadioButton {
                    exclusiveGroup: smileDetectGroup
                    id: smileDetectEnable
                    text: "Enable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        seecam130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                    }
                    Keys.onReturnPressed: {
                        seecam130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                    }
                }
                RadioButton {
                    exclusiveGroup: smileDetectGroup
                    id:smileDetectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam130.setSmileDetection(false, smileDetectEmbedData.checked, smileThreshold.text)
                    }
                    Keys.onReturnPressed: {
                        seecam130.setSmileDetection(false, smileDetectEmbedData.checked, smileThreshold.text)
                    }
                }
            }
            Row{
                spacing: 14
                Text {
                    id: smileThresholdText
                    text: "Threshold value[40-75]"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    width: 80
                    wrapMode: Text.WordWrap
                    opacity: (smileDetectEnable.enabled && smileDetectEnable.checked) ? 1 : 0.1
                }
                TextField {                    
                    id: smileThreshold
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: (smileDetectEnable.enabled && smileDetectEnable.checked) ? true : false
                    opacity: (smileDetectEnable.enabled && smileDetectEnable.checked) ? 1 : 0.1
                    style: econTextFieldStyle
                    implicitHeight: 25
                    implicitWidth: 70
                    validator: IntValidator {bottom: 40; top: 75}
                }
                Button {
                    id: smileThresholdSet
                    activeFocusOnPress : true
                    text: "Set"
                    style: econButtonStyle
                    enabled: (smileDetectEnable.enabled && smileDetectEnable.checked) ? true : false
                    opacity: (smileDetectEnable.enabled && smileDetectEnable.checked) ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 60
                    onClicked: {
                        smileThresholdSet.enabled = false
                        setButtonClicked = true
                        seecam130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                        smileThresholdSet.enabled = true
                    }
                    Keys.onReturnPressed: {
                        smileThresholdSet.enabled = false
                        setButtonClicked = true
                        seecam130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                        smileThresholdSet.enabled = true
                    }
                }
            }
            Row{
                spacing: 5
                CheckBox {
                    id: smileDetectEmbedData
                    activeFocusOnPress : true
                    text: "Embed Data"
                    style: econCheckBoxStyle
                    enabled: smileDetectEnable.checked ? true : false
                    opacity: enabled ? 1 : 0.1
                    onClicked:{
                        enableSmileDetectEmbedData()
                    }
                    Keys.onReturnPressed: {
                        enableSmileDetectEmbedData()
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
               // Layout.alignment: Qt.AlignCenter
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
                Button {
                    id: dummy
                    opacity: 0
                }
            }
        }
    }


    See3Cam130 {
        id: seecam130
        onSceneModeValue: {
            defaultSceneMode(sceneMode)
        }
        onEffectModeValue: {
            defaultEffectMode(effectMode)
        }
        onDenoiseValueReceived:{
            skipUpdateUIDenoise = false
            deNoiseSlider.value = denoiseValue
	    skipUpdateUIDenoise = true
        }
        onFrameRateCtrlValueReceived:{
            skipUpdateUIFrameRate = false
            frameRateSlider.value = frameRateCtrlValue
	    skipUpdateUIFrameRate = true
        }

        onAfModeValue:{
            defaultAfMode(afMode)
        }
        onFaceDetectModeValue:{
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
        onSmileDetectModeValue:{
            smileThreshold.text = smileDetectThresholdValue
            if(smileDetectMode == See3Cam130.SmileDetectEnable){
                smileDetectEnable.checked = true
                if(smileDetectEmbedDataValue == See3Cam130.smileDetectEmbedDataEnable){
                    smileDetectEmbedData.checked = true
                }
            }else if(smileDetectMode == See3Cam130.SmileDetectDisable){
                smileDetectDisable.checked = true
                if(smileDetectEmbedDataValue == See3Cam130.smileDetectEmbedDataEnable){
                    smileDetectEmbedData.checked = true
                }else{
                    smileDetectEmbedData.checked = false
                }
            }
        }

        onExposureCompValueReceived:{
            exposureCompValue.text = exposureCompensation
        }

        onHDRModeValueReceived:{
            defaultHDRMode(hdrMode)
            if(hdrMode == See3Cam130.HdrManual){
                iHDRSlider.value = hdrValue
            }
        }
        onQFactorValue:{
              skipUpdateUIQFactor = false
              qFactorSlider.value = qFactor
	      skipUpdateUIQFactor = true
        }
        onRoiAfModeValue:{
            if(roiMode == See3Cam130.AFCentered){
                afCentered.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam130.AFManual){
                skipUpdateUIOnAFWindowSize = false
                afManual.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam130.AFDisabled){
                rectEnable.enabled = false
                rectDisable.enabled = false
                rectEnable.opacity = 0.1
                rectDisable.opacity = 0.1
                afCentered.enabled = false
                afManual.enabled = false
                afWindowSizeCombo.enabled = false
            }
        }
        onRoiAutoExpModeValue:{
            if(roiMode == See3Cam130.AutoExpFull){                
                autoexpFull.checked = true                
                autoExpoWinSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam130.AutoExpManual){
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                autoExpoWinSizeCombo.currentIndex = winSize-1
            }
            else if(roiMode == See3Cam130.AutoExpDisabled){
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
            }
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onAfRectModeValue:{            
            if(afRectMode == See3Cam130.AFRectEnable){                
                rectEnable.checked = true

            }else if(afRectMode == See3Cam130.AFRectDisable){            
                rectDisable.checked = true
            }

        }
        onFlipModeValue:{
           updateFlipMode(flipMode, flipEnableDisableMode)
        }
        onStreamModeValue:{
            if(streamMode == See3Cam130.STREAM_MASTER){
                streamMaster.checked = true

            }else if(streamMode == See3Cam130.STREAM_TRIGGER){
                streamTrigger.checked = true
                displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document."))
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
                seecam130.getExposureCompensation()
            }
        }

        onIndicateSmileThresholdRangeFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
                seecam130.getSmileDetectMode()
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

    Component.onCompleted:{
        //getting valid effect mode and scene mode takes some time.
        //So In timer, after 500 ms, getting effect mode and scene mode is done
        getCamValuesTimer.start()
        seecam130.getAutoFocusMode()
        seecam130.getiHDRMode()      
        seecam130.getBurstLength()
        seecam130.getAutoFocusROIModeAndWindowSize()
        seecam130.getAutoExpROIModeAndWindowSize()
        seecam130.getAFRectMode()
        seecam130.getFlipMode()
        seecam130.getStreamMode()
        seecam130.getFaceDetectMode()
        seecam130.getSmileDetectMode()

    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
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

    function setTriggerMode(){
        root.stopUpdatePreviewInTriggerMode()
        seecam130.setStreamMode(See3Cam130.STREAM_TRIGGER)
        displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document See3CAM_130_Trigger_Mode"))
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }
    function setToDefaultValues(){
        seecam130.setToDefault()
        seecam130.getSceneMode()
        seecam130.getEffectMode()
        seecam130.getAutoFocusMode()
        seecam130.getiHDRMode()
        seecam130.getDenoiseValue()
        seecam130.getQFactor()
        seecam130.getBurstLength()
        seecam130.getAutoFocusROIModeAndWindowSize()
        seecam130.getAutoExpROIModeAndWindowSize()
        seecam130.getAFRectMode()
        seecam130.getFlipMode()
        seecam130.getStreamMode()
        // Added by Sankari: 17 Apr 2017. To get preview in master mode
        root.startUpdatePreviewInMasterMode()
        seecam130.getFaceDetectMode()
        seecam130.getSmileDetectMode()
        seecam130.getExposureCompensation()        
        seecam130.getFrameRateCtrlValue()
    }

    function defaultSceneMode(mode)
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
    function defaultEffectMode(mode)
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

    function defaultAfMode(mode)
    {
        switch(mode)
        {
            case See3Cam130.Continuous:
                radioContin.checked = true
                break;
            case See3Cam130.OneShot:
                radioOneshot.checked = true
                break;
            case See3Cam130.AfModeDisabled:
                radioContin.enabled = false
                radioOneshot.enabled = false
                trigger.enabled = false
                break;
        }
    }

    function defaultHDRMode(mode)
    {
        switch(mode)
        {
            case See3Cam130.HdrOff:
                hdrOff.checked = true
                break;
            case See3Cam130.HdrAuto:
                hdrAuto.checked = true
                break;
            case See3Cam130.HdrManual:
                hdrManual.checked = true
                break;
        }
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

    function enableFaceDetectEmbedData(){        
        if(seecam130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){            
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_130_Face_and_Smile_Detection for more details"))
            }
        }
    }

    function enableSmileDetectEmbedData(){
        setButtonClicked = false
	seecam130.getSmileDetectMode()
        if(seecam130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)){
            if(smileDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document See3CAM_130_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
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

    Connections{
         target: root
         onMouseRightClicked:{
             if(afManual.enabled && afManual.checked){
                 seecam130.setROIAutoFoucs(See3Cam130.AFManual, width, height, x, y, afWindowSizeCombo.currentText)
             }
             if(autoexpManual.enabled && autoexpManual.checked){
                seecam130.setROIAutoExposure(See3Cam130.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }
         }
         onAutoFocusSelected:{
             enableDisableAutoFocusUIControls(autoFocusSelect)
         }
         onAutoExposureSelected:{
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         onEnableFaceRectafterBurst:{
             seecam130.enableDisableFaceRectangle(true)
         }
    }

    Connections{
         target: root
         onAfterBurst:{
             if(rectEnable.checked){
                seecam130.enableDisableAFRectangle(true)
             }
         }
         onBeforeRecordVideo:{
             seecam130.enableDisableAFRectangle(false)
             seecam130.enableDisableFaceRectangle(false)
         }
         onAfterRecordVideo:{
             if(rectEnable.checked){
                seecam130.enableDisableAFRectangle(true)
             }
             seecam130.enableDisableFaceRectangle(true)
         }
         onVideoResolutionChanged:{
             getexposureCompFrameRateCtrlTimer.start()
         }
         onPreviewFPSChanged:{
             getexposureCompFrameRateCtrlTimer.start()
         }
         onVideoColorSpaceChanged:{
             getexposureCompFrameRateCtrlTimer.start()
         }
    }

}

