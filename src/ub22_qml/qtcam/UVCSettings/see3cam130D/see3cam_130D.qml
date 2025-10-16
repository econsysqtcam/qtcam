import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam130D 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {

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
    property bool skipUpdateUIOnAntiFlickerMode:false
    property int  flickerCtrl

    property bool skipUpdateUIOnRedGain: false
    property bool skipUpdateUIOnBlueGain: false
    property bool skipUpdateUIOnGreenGain: false

    Connections
    {
        target: root
        function onTakeScreenShot(){
            if(seecam130D.enableDisableFaceRectangle(false)){
                seecam130D.enable_disablerect(false)
                burstShotTimer.start()
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
        function onAfterBurst()
        {
            seecam130D.enable_disablerect(true)
        }
        function onSkipFrameCount(){
            root.setSkipFrameCount(3)
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
            seecam130D.getSceneMode()
            seecam130D.getEffectMode()
            seecam130D.getDenoiseValue()
            seecam130D.getQFactor()
            seecam130D.getExposureCompensation()
            seecam130D.getFrameRateCtrlValue()
            stop()
        }
    }

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            seecam130D.getExposureCompensation()
            seecam130D.getFrameRateCtrlValue()
            stop()
        }
    }

    // Used when selecting auto focus in image Quality settings menu
    Timer {
        id: getAutoFocusControlValues
        interval: 500
        onTriggered: {
            seecam130D.getAutoFocusROIModeAndWindowSize()
            stop()
        }
    }

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            seecam130D.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Action {
        id: triggerAction
        onTriggered: {
            seecam130D.setAutoFocusMode(See3Cam130D.OneShot)
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
        height: 450
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
                        seecam130D.setSceneMode(See3Cam130D.SCENE_NORMAL)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setSceneMode(See3Cam130D.SCENE_NORMAL)
                    }
                }
                RadioButton {
                    id: sceneDoc
                    style:  econRadioButtonStyle
                    text: qsTr("Document")
                    exclusiveGroup: sceneInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        seecam130D.setSceneMode(See3Cam130D.SCENE_DOCUMENT)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setSceneMode(See3Cam130D.SCENE_DOCUMENT)
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
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_NORMAL)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_NORMAL)
                    }

                }
                RadioButton {
                    id: effectBW
                    style:  econRadioButtonStyle
                    text: qsTr("Black and White")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_BLACK_WHITE)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_BLACK_WHITE)
                    }
                }
                RadioButton {
                    id: effectNegative
                    style:  econRadioButtonStyle
                    text: qsTr("Negative")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_NEGATIVE)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_NEGATIVE)
                    }

                }
                RadioButton {
                    id: effectGrayscale
                    style:  econRadioButtonStyle
                    text: qsTr("Grayscale")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_GREYSCALE)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_GREYSCALE)
                    }
                }
                RadioButton {
                    id: effectSketch
                    style:  econRadioButtonStyle
                    text: qsTr("Sketch")
                    exclusiveGroup: effectInputGroup
                    activeFocusOnPress: true
                    onClicked: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_SKETCH)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setEffectMode(See3Cam130D.EFFECT_SKETCH)
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
                        seecam130D.setAutoFocusMode(See3Cam130D.Continuous);
                      }
                      Keys.onReturnPressed: {
                        seecam130D.setAutoFocusMode(See3Cam130D.Continuous);
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
                        seecam130D.setAutoFocusMode(See3Cam130D.OneShot);
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setAutoFocusMode(See3Cam130D.OneShot);
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
                        seecam130D.setAutoFocusMode(See3Cam130D.OneShot);
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
                        seecam130D.enableDisableAFRectangle(true)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.enableDisableAFRectangle(true)
                    }
                }
                RadioButton {
                    exclusiveGroup: afRectGroup
                    id:rectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam130D.enableDisableAFRectangle(false)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.enableDisableAFRectangle(false)
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
                        seecam130D.setROIAutoFoucs(See3Cam130D.AFCentered, 0, 0, 0, 0, 0);
                        afWindowSizeCombo.enabled = false
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setROIAutoFoucs(See3Cam130D.AFCentered, 0, 0, 0, 0, 0);
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
                        seecam130D.setROIAutoFoucs(See3Cam130D.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText)
                        afWindowSizeCombo.enabled = true
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setROIAutoFoucs(See3Cam130D.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText);
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
                        seecam130D.setROIAutoFoucs(See3Cam130D.AFManual, 0, 0, 0, 0, afWindowSizeCombo.currentText)
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
                        seecam130D.setiHDRMode(See3Cam130D.HdrOff, 0)
                    }

                    Keys.onReturnPressed: {
                        seecam130D.setiHDRMode(See3Cam130D.HdrOff, 0)
                    }
                }
                RadioButton {
                    exclusiveGroup: iHDRgroup
                    id:hdrAuto
                    text: "Auto"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam130D.setiHDRMode(See3Cam130D.HdrAuto, 0)
                    }

                    Keys.onReturnPressed: {
                        seecam130D.setiHDRMode(See3Cam130D.HdrAuto, 0)
                    }
                }
                RadioButton {
                    exclusiveGroup: iHDRgroup
                    id: hdrManual
                    text: "Manual"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        seecam130D.setiHDRMode(See3Cam130D.HdrManual, iHDRSlider.value)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setiHDRMode(See3Cam130D.HdrManual, iHDRSlider.value)
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
                            seecam130D.setiHDRMode(See3Cam130D.HdrManual, iHDRSlider.value)
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
                            seecam130D.setDenoiseValue(deNoiseSlider.value)
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
                  spacing:38
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
                          seecam130D.setROIAutoExposure(See3Cam130D.AutoExpFull, 0, 0, 0, 0, 0);
                          autoExpoWinSizeCombo.enabled = false
                      }
                      Keys.onReturnPressed: {
                          seecam130D.setROIAutoExposure(See3Cam130D.AutoExpFull, 0, 0, 0, 0, 0);
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
                          seecam130D.setROIAutoExposure(See3Cam130D.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                          autoExpoWinSizeCombo.enabled = true
                      }
                      Keys.onReturnPressed: {
                          seecam130D.setROIAutoExposure(See3Cam130D.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                        seecam130D.setROIAutoExposure(See3Cam130D.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
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
                        seecam130D.setExposureCompensation(exposureCompValue.text)
                        exposureCompSet.enabled = true
                    }
                    Keys.onReturnPressed: {
                        exposureCompSet.enabled = false
                        setButtonClicked = true
                        seecam130D.setExposureCompensation(exposureCompValue.text)
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
                            seecam130D.setQFactor(qFactorSlider.value)
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
                        seecam130D.setBurstLength(burstLengthCombo.currentText)
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
                        seecam130D.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
                CheckBox {
                    id: flipCtrlVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        seecam130D.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setFlipMode(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                    }
                }
            }

            Text {
                id: autoWhiteBalanceTitle
                text: "--- Auto White Balance ---"
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
                            seecam130D.getAutoWhiteBalance()
                            getAutoWBBtn.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            getAutoWBBtn.enabled = false
                            setButtonClicked = true
                            seecam130D.getAutoWhiteBalance()
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
                            seecam130D.getAutoExposure()
                            getAutoExpBtn.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            getAutoExpBtn.enabled = false
                            setButtonClicked = true
                            seecam130D.getAutoExposure()
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
                            seecam130D.setFlashMode(See3Cam130D.DISABLE)
                        }
                        Keys.onReturnPressed: {
                            seecam130D.setFlashMode(See3Cam130D.DISABLE)
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
                            seecam130D.setFlashMode(See3Cam130D.STROBE)
                        }
                        Keys.onReturnPressed: {
                            seecam130D.setFlashMode(See3Cam130D.STROBE)
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
                            seecam130D.setFlashMode(See3Cam130D.TORCH)
                        }
                        Keys.onReturnPressed: {
                            seecam130D.setFlashMode(See3Cam130D.TORCH)
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
                        seecam130D.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                }
                RadioButton {
                    exclusiveGroup: faceRectGroup
                    id:faceRectDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        seecam130D.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
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
                        seecam130D.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
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
                            seecam130D.getAutoFocusPosition()
                            getFocusPosition.enabled = true
                        }
                        Keys.onReturnPressed:
                        {
                            getFocusPosition.enabled = false
                            setButtonClicked = true
                            seecam130D.getAutoFocusPosition()
                            getFocusPosition.enabled = true
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
                            seecam130D.setFrameRateCtrlValue(frameRateSlider.value)
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
            RowLayout{
                spacing: 15
                Layout.alignment: Qt.AlignCenter

                ExclusiveGroup { id: antiFlickerGroup }

                RadioButton
                {
                    exclusiveGroup: antiFlickerGroup
                    id: antiFlickerModeOff
                    text: "Off"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    tooltip: "It disables Anti Flicker Mode."
                    width : 70
                    onClicked: {
                        seecam130D.setAntiFlickerMode(See3Cam130D.MODE_DISABLE)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setAntiFlickerMode(See3Cam130D.MODE_DISABLE)
                    }
                }
                RadioButton {
                    id: antiFlickerModeAuto
                    style:  econRadioButtonStyle
                    text:   qsTr("Auto")
                    exclusiveGroup: antiFlickerGroup
                    tooltip: "The device will automatically switch between the flicker frequencies, if the flicker is detected in preview."
                    activeFocusOnPress: true
                    width : 70
                    onClicked: {
                        seecam130D.setAntiFlickerMode(See3Cam130D.MODE_AUTO)
                    }
                    Keys.onReturnPressed: {
                        seecam130D.setAntiFlickerMode(See3Cam130D.MODE_AUTO)
                    }
                }
                RadioButton
                {
                    id: antiFlickerModeManual
                    text: "Manual"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    exclusiveGroup: antiFlickerGroup
                    tooltip: "The flicker frequency has to be choosed manually to avoid the flickering in the preview."
                    //implicitWidth: 70
                    width : 70
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
                            seecam130D.setRedGain(redGainSlider.value)
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
                            seecam130D.setBlueGain(blueGainSlider.value)
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
                            seecam130D.setGreenGain(greenGainSlider.value)
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
                Layout.alignment: Qt.AlignCenter
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
                    tooltip: "This feature will save the current configurations and are retained after the
following power cycles."
                    onClicked:{
                        setButtonClicked = true
                        seecam130D.saveConfiguration()
                    }
                    Keys.onReturnPressed: {
                        setButtonClicked = true
                        seecam130D.saveConfiguration()
                    }
                }
            }
        }
    }


    See3Cam130D {
        id: seecam130D
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
            if(faceDetectMode == See3Cam130D.FaceRectEnable){
                faceRectEnable.checked = true
                if(faceDetectEmbedDataValue == See3Cam130D.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }
                if(faceDetectOverlayRect == See3Cam130D.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }
            }else if(faceDetectMode == See3Cam130D.FaceRectDisable){
                faceRectDisable.checked = true
                if(faceDetectEmbedDataValue == See3Cam130D.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }else{
                    faceDetectEmbedData.checked = false
                }
                if(faceDetectOverlayRect == See3Cam130D.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }else{
                    overlayRect.checked = false
                }
            }
        }

        onExposureCompValueReceived:{
            exposureCompValue.text = exposureCompensation
        }

        onHDRModeValueReceived:{
            defaultHDRMode(hdrMode)
            if(hdrMode == See3Cam130D.HdrManual){
                iHDRSlider.value = hdrValue
            }
        }
        onQFactorValue:{
              skipUpdateUIQFactor = false
              qFactorSlider.value = qFactor
              skipUpdateUIQFactor = true
        }
        onRoiAfModeValue:{
            if(roiMode == See3Cam130D.AFCentered){
                afCentered.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam130D.AFManual){
                skipUpdateUIOnAFWindowSize = false
                afManual.checked = true
                afWindowSizeCombo.currentIndex = winSize-1
            }else if(roiMode == See3Cam130D.AFDisabled){
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
            currentROIAutoExposureMode(roiMode, winSize)
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onAfRectModeValue:{
            if(afRectMode == See3Cam130D.AFRectEnable){
                rectEnable.checked = true

            }else if(afRectMode == See3Cam130D.AFRectDisable){
                rectDisable.checked = true
            }

        }
        onFlipMirrorModeChanged:{
           currentFlipMirrorMode(flipMirrorMode)
        }
        onStreamModeValue:{
            if(streamMode == See3Cam130D.STREAM_MASTER){
                streamMaster.checked = true
                root.captureBtnEnable(true)
                root.videoRecordBtnEnable(true)
                root.checkForTriggerMode(false)
                root.startUpdatePreviewInMasterMode()
            }else if(streamMode == See3Cam130D.STREAM_TRIGGER){
                streamTrigger.checked = true
                root.captureBtnEnable(false)
                root.videoRecordBtnEnable(false)
                root.checkForTriggerMode(true)
                root.stopUpdatePreviewInTriggerMode()
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
                seecam130D.getExposureCompensation()
            }
        }
        onAntiFlickerModeChanged:{
            currentAntiFlickerMode(flickerMode)
        }
        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
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
        onAutoFocusPositionReceived:{
            autoFocusPositionTextField.text = afPosition
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
        seecam130D.getAutoFocusMode()
        seecam130D.getiHDRMode()
        seecam130D.getBurstLength()
        seecam130D.getAutoFocusROIModeAndWindowSize()
        seecam130D.getAutoExpROIModeAndWindowSize()
        seecam130D.getAFRectMode()
        seecam130D.getFlipMode()
        seecam130D.getAutoWhiteBalance()
        seecam130D.getAutoExposure()
        seecam130D.getStreamMode()
        seecam130D.getFaceDetectMode()
        seecam130D.getFlashMode()
        seecam130D.getAntiFlickerMode()
        seecam130D.getAutoFocusPosition()
        seecam130D.getRedGain()
        seecam130D.getBlueGain()
        seecam130D.getGreenGain()
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case See3Cam130D.FLIP_BOTHFLIP_ENABLE:
                 flipCtrlVertical.checked = true
                 flipCtrlHorizotal.checked = true
                break;
            case See3Cam130D.FLIP_VERTFLIP:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case See3Cam130D.FLIP_HORZFLIP:
                 flipCtrlVertical.checked = false
                 flipCtrlHorizotal.checked = true
                break;
            case See3Cam130D.FLIP_BOTHFLIP_DISABLE:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
        }
    }

    function updateFlipMode(flipMode, FlipEnableDisableMode){
        switch(flipMode){
        case See3Cam130D.FlipHorizontal:
            if(FlipEnableDisableMode == See3Cam130D.FlipEnable){
                flipCtrlHorizotal.checked = true
            }else{
                flipCtrlHorizotal.checked = false
            }
            break;
        case See3Cam130D.FlipVertical:
            if(FlipEnableDisableMode == See3Cam130D.FlipEnable){
                flipCtrlVertical.checked = true
            }else{
                flipCtrlVertical.checked = false
            }
            break;
        case See3Cam130D.FlipBoth:
            if(FlipEnableDisableMode == See3Cam130D.FlipEnable){
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
        if(flashMode == See3Cam130D.DISABLE){
            flashModeOff.checked = true
        }else if(flashMode == See3Cam130D.STROBE){
            flashModeStrobe.checked = true
        }else if(flashMode == See3Cam130D.TORCH){
            flashModeTorch.checked = true
        }
     }

    // current ROI auto exposure mode
    function currentROIAutoExposureMode(roiMode, winSize){
        switch(roiMode){
            case See3Cam130D.AutoExpFull:
                autoexpFull.checked = true
                autoExpoWinSizeCombo.enabled = false
                break
            case See3Cam130D.AutoExpManual:
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
            case See3Cam130D.AutoExpDisabled:
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
                break
        }
    }

    function setMasterMode(){
        seecam130D.setStreamMode(See3Cam130D.STREAM_MASTER)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.checkForTriggerMode(false)
        root.startUpdatePreviewInMasterMode()
    }

    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        root.stopUpdatePreviewInTriggerMode()
        seecam130D.setStreamMode(See3Cam130D.STREAM_TRIGGER)
        displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document See3CAM_130D_Trigger_Mode"))
    }

    function getSerialNumber() {
        seecam130D.get64BitSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }
    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        seecam130D.setToDefault()
        seecam130D.getSceneMode()
        seecam130D.getEffectMode()
        seecam130D.getAutoFocusMode()
        seecam130D.getiHDRMode()
        seecam130D.getDenoiseValue()
        seecam130D.getQFactor()
        seecam130D.getBurstLength()
        seecam130D.getAutoFocusROIModeAndWindowSize()
        seecam130D.getAutoExpROIModeAndWindowSize()
        seecam130D.getAFRectMode()
        seecam130D.getFlipMode()
        seecam130D.getAutoWhiteBalance()
        seecam130D.getAutoExposure()
        seecam130D.getStreamMode()
        root.startUpdatePreviewInMasterMode()
        seecam130D.getFaceDetectMode()
        seecam130D.getExposureCompensation()
        seecam130D.getFrameRateCtrlValue()
        seecam130D.getAntiFlickerMode()
        seecam130D.getFlashMode()
        seecam130D.getAutoFocusPosition()
        seecam130D.getRedGain()
        seecam130D.getBlueGain()
        seecam130D.getGreenGain()
    }

    function defaultSceneMode(mode)
    {
        switch(mode)
        {
            case See3Cam130D.SCENE_NORMAL:
                sceneNormal.checked = true
                break;
            case See3Cam130D.SCENE_DOCUMENT:
                sceneDoc.checked = true
                break;
        }
    }
    function defaultEffectMode(mode)
    {
        switch(mode)
        {
            case See3Cam130D.EFFECT_NORMAL:
                effectNormal.checked = true
                break;
            case See3Cam130D.EFFECT_BLACK_WHITE:
                effectBW.checked = true
                break;
            case See3Cam130D.EFFECT_GREYSCALE:
                effectGrayscale.checked = true
                break;
            case See3Cam130D.EFFECT_NEGATIVE:
                effectNegative.checked = true
                break;
            case See3Cam130D.EFFECT_SKETCH:
                effectSketch.checked = true
                break;
        }
    }

    function defaultAfMode(mode)
    {
        switch(mode)
        {
            case See3Cam130D.Continuous:
                radioContin.checked = true
                break;
            case See3Cam130D.OneShot:
                radioOneshot.checked = true
                break;
            case See3Cam130D.AfModeDisabled:
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
            case See3Cam130D.HdrOff:
                hdrOff.checked = true
                break;
            case See3Cam130D.HdrAuto:
                hdrAuto.checked = true
                break;
            case See3Cam130D.HdrManual:
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
        if(seecam130D.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                displayMessageBox(qsTr("Status"),qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_130D_Face_Detection for more details"))
            }
        }
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

    function currentAntiFlickerMode(flickerMode){
        switch(flickerMode)
        {
            case See3Cam130D.MODE_AUTO:
                antiFlickerModeAuto.checked = true
                break
            case See3Cam130D.MODE_50Hz:
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case See3Cam130D.MODE_60Hz:
                antiFlickerModeManual.checked = true
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
             case See3Cam130D.MODE_DISABLE:
                antiFlickerModeOff.checked = true
                break
         }
    }

    function setAntiFlickerMode(){
        switch(antiFlickerCombo.currentIndex){
        case 0:
            flickerCtrl= See3Cam130D.MODE_50Hz
            break
        case 1:
            flickerCtrl = See3Cam130D.MODE_60Hz
            break
        }
        seecam130D.setAntiFlickerMode(flickerCtrl)
    }

    Connections{
         target: root
         function onMouseRightClicked(x, Y, width, height){
             if(afManual.enabled && afManual.checked){
                 seecam130D.setROIAutoFoucs(See3Cam130D.AFManual, width, height, x, y, afWindowSizeCombo.currentText)
             }
             if(autoexpManual.enabled && autoexpManual.checked){
                seecam130D.setROIAutoExposure(See3Cam130D.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
             }
         }
         function onAutoFocusSelected(autoFocusSelect){
             enableDisableAutoFocusUIControls(autoFocusSelect)
         }
         function onAutoExposureSelected(autoExposureSelect){
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         function onEnableFaceRectafterBurst(){
             if(rectEnable.checked)     //Added by M.Vishnu Murali: Inorder to avoid enabling Rectangle eventhough it is disabled by user.
                seecam130D.enableDisableFaceRectangle(true)
         }
    }

    Connections{
         target: root
         function onAfterBurst(){
             if(rectEnable.checked){
                seecam130D.enableDisableAFRectangle(true)
             }
         }
         function onBeforeRecordVideo(){
             seecam130D.enableDisableAFRectangle(false)
             seecam130D.enableDisableFaceRectangle(false)
         }
         function onAfterRecordVideo(){
             if(rectEnable.checked){
                seecam130D.enableDisableAFRectangle(true)
                seecam130D.enableDisableFaceRectangle(true)
             }
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

}
