import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3camcu130 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item {
    width:268
    height:720

    property int expoCompMin: 8000
    property int expoCompMax: 1000000

    property int denoiseMin: 0
    property int denoiseMax: 15

    property int qFactorMin: 10
    property int qFactorMax: 96

    property int frameRateMin: 1
    property int frameRateMax: 120

    property bool setButtonClicked: false

    // Added by Sankari: 05 Apr 2017 - To skip setting denoise value when getting the denoise value.
    property bool skipUpdateUIDenoise : false

    // Added by Sankari: 12 Apr 2017 - To skip setting burst length when getting the burst length value.
    property bool skipUpdateUIOnBurstLength: false

    // Added by Sankari: 12 Apr 2017 - To skip setting qfactor when getting the qfactor value.
    property bool skipUpdateUIQFactor : false

    // Added by Sankari: 12 Apr 2017 - To skip setting exposure window size when getting the qfactor value.
    property bool skipUpdateUIOnExpWindowSize: false

    // Added by Sankari: 12 Apr 2017 - To skip setting frame rate when getting the framerate value.
    property bool skipUpdateUIFrameRate: false

    Connections
    {
        target: root
        onTakeScreenShot:
        {
            seecamcu130.enableDisableFaceRectangle(false)
            burstShotTimer.start()
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
        onMouseRightClicked:{
            if(autoexpManual.enabled && autoexpManual.checked){
                seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
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
        onEnableFaceRectafterBurst:{
            seecamcu130.enableDisableFaceRectangle(true)
        }

        onBeforeRecordVideo:{
            seecamcu130.enableDisableFaceRectangle(false)
        }
        onAfterRecordVideo:{
            seecamcu130.enableDisableFaceRectangle(true)
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

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 500
        onTriggered: {
            seecamcu130.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            seecamcu130.getExposureCompensation()
            seecamcu130.getFrameRateCtrlValue()
            stop()
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
        onTriggered:
        {
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
        style: uvc130_scrollViewStyle
        Item{
            height:1300
            ColumnLayout{
                x:2
                y:5
                spacing:20
                Row{
                    Text {
                        id: scene_mode
                        text: "--- Scene Mode ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                    Layout.alignment: Qt.AlignCenter
                }
                Row{
                    Grid {
                        columns: 2
                        columnSpacing: 40
                        rowSpacing: 15
                        ExclusiveGroup { id: sceneInputGroup }
                        RadioButton {
                            id: scene1Button
                            style:  econRadioButtonStyle
                            text:   qsTr("Normal")
                            exclusiveGroup: sceneInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setSceneMode(See3CamCu130.SCENE_NORMAL)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setSceneMode(See3CamCu130.SCENE_NORMAL)
                            }
                        }
                        RadioButton {
                            id: scene2Button
                            style:  econRadioButtonStyle
                            text: qsTr("Document")
                            exclusiveGroup: sceneInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setSceneMode(See3CamCu130.SCENE_DOCUMENT)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setSceneMode(See3CamCu130.SCENE_DOCUMENT)
                            }
                        }
                    }
                }
                Row{
                    Text {
                        id: effect_mode
                        text: "--- Effect Mode ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                    Layout.alignment: Qt.AlignCenter
                }
                Row{

                    Grid {
                        columns: 2
                        columnSpacing: 40
                        rowSpacing: 15
                        ExclusiveGroup { id: effectInputGroup }
                        RadioButton {
                            id: effect1Button
                            style:  econRadioButtonStyle
                            text: qsTr("Normal")
                            exclusiveGroup: effectInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_NORMAL)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_NORMAL)
                            }
                        }

                        RadioButton {
                            id: effect2Button
                            style:  econRadioButtonStyle
                            text: qsTr("Black & White")
                            exclusiveGroup: effectInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_BLACK_WHITE)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_BLACK_WHITE)
                            }
                        }

                        RadioButton {
                            id: effect3Button
                            style:  econRadioButtonStyle
                            text: qsTr("Grey")
                            exclusiveGroup: effectInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_GRAY)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_GRAY)
                            }
                        }

                        RadioButton {
                            id: effect4Button
                            style:  econRadioButtonStyle
                            text: qsTr("Negative")
                            exclusiveGroup: effectInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_NEGATIVE)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_NEGATIVE)
                            }
                        }
                        RadioButton {
                            id: effect5Button
                            style:  econRadioButtonStyle
                            text: qsTr("Sketch")
                            exclusiveGroup: effectInputGroup
                            activeFocusOnPress: true
                            onClicked: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_SKETCH)
                            }
                            Keys.onReturnPressed: {
                                seecamcu130.setEffectMode(See3CamCu130.EFFECT_SKETCH)
                            }
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
                    CheckBox {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        onClicked:{
                            seecamcu130.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            seecamcu130.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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

                    // Added by Sankari 13th Sep 2017 : Added Face ROI mode
                    RadioButton {
                        exclusiveGroup: roiExpogroup
                        id: autoexpFace
                        text: "Face"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                        // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                        // winSize is required only for manual mode
                        onClicked: {
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpFace, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpFace, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                    }

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
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpFull, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpFull, 0, 0, 0, 0, 0);
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
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpManual, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = true
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpManual, 0, 0, 0, 0, 0);
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
                            seecamcu130.setROIAutoExposure(See3CamCu130.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
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
                            seecamcu130.setExposureCompensation(exposureCompValue.text)
                            exposureCompSet.enabled = true
                        }
                        Keys.onReturnPressed: {
                            exposureCompSet.enabled = false
                            setButtonClicked = true
                            seecamcu130.setExposureCompensation(exposureCompValue.text)
                            exposureCompSet.enabled = true
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
                                seecamcu130.setDenoiseValue(deNoiseSlider.value)
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
                            seecamcu130.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
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
                                seecamcu130.setQFactor(qFactorSlider.value)
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
                            seecamcu130.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: faceRectGroup
                        id:faceRectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            seecamcu130.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
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
                            seecamcu130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
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
                            seecamcu130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: smileDetectGroup
                        id:smileDetectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            seecamcu130.setSmileDetection(false, smileDetectEmbedData.checked, smileThreshold.text)
                        }
                        Keys.onReturnPressed: {
                            seecamcu130.setSmileDetection(false, smileDetectEmbedData.checked, smileThreshold.text)
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
                            seecamcu130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
                            smileThresholdSet.enabled = true
                        }
                        Keys.onReturnPressed: {
                            smileThresholdSet.enabled = false
                            setButtonClicked = true
                            seecamcu130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)
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
                                seecamcu130.setFrameRateCtrlValue(frameRateSlider.value)
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
                            seecamcu130.setToDefault()
                        }
                    }
                }
                Row{
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

                    Button {
                        id: f_wversion_selectedCU130
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
                }

            }
        }
    }


    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        onAccepted: {
            close()
        }
        Component.onCompleted: close()
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
        id: uvc130_scrollViewStyle

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

    See3CamCu130 {
        id: seecamcu130
        onSceneModeValue: {
            defaultSceneMode(sceneMode)
        }
        onEffectModeValue: {
            defaultEffectMode(effectMode)
        }
        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onExposureCompValueReceived:{
            exposureCompValue.text = exposureCompensation
        }
        onIndicateCommandStatus:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
            }
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }

        onQFactorValue:{
            skipUpdateUIQFactor = false
            qFactorSlider.value = qFactor
            skipUpdateUIQFactor = true
        }

        onIndicateExposureValueRangeFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
                seecamcu130.getExposureCompensation()
            }
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

        onRoiAutoExpModeValue:{
            currentROIAutoExposureMode(roiMode, winSize)
        }
        onFaceDetectModeValue:{
            if(faceDetectMode == See3CamCu130.FaceRectEnable){
                faceRectEnable.checked = true
                if(faceDetectEmbedDataValue == See3CamCu130.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }
                if(faceDetectOverlayRect == See3CamCu130.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }
            }else if(faceDetectMode == See3CamCu130.FaceRectDisable){
                faceRectDisable.checked = true
                if(faceDetectEmbedDataValue == See3CamCu130.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }else{
                    faceDetectEmbedData.checked = false
                }
                if(faceDetectOverlayRect == See3CamCu130.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }else{
                    overlayRect.checked = false
                }
            }
        }
        onSmileDetectModeValue:{
            smileThreshold.text = smileDetectThresholdValue
            if(smileDetectMode == See3CamCu130.SmileDetectEnable){
                smileDetectEnable.checked = true
                if(smileDetectEmbedDataValue == See3CamCu130.smileDetectEmbedDataEnable){
                    smileDetectEmbedData.checked = true
                }
            }else if(smileDetectMode == See3CamCu130.SmileDetectDisable){
                smileDetectDisable.checked = true
                if(smileDetectEmbedDataValue == See3CamCu130.smileDetectEmbedDataEnable){
                    smileDetectEmbedData.checked = true
                }else{
                    smileDetectEmbedData.checked = false
                }
            }
        }

        onIndicateSmileThresholdRangeFailure:{
            if(setButtonClicked){
                displayMessageBox(title, text)
                setButtonClicked = false
                seecamcu130.getSmileDetectMode()
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

    Component.onCompleted:{
        getValuesFromCamera()
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    // Enable Face detect embed data
    function enableFaceDetectEmbedData(){
        if(seecamcu130.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_CU130_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }
    }

    // enable smile detect embed data
    function enableSmileDetectEmbedData(){
        setButtonClicked = false
        seecamcu130.getSmileDetectMode()
        if(seecamcu130.setSmileDetection(true, smileDetectEmbedData.checked, smileThreshold.text)){
            if(smileDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document See3CAM_CU130_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }
    }

    // current ROI auto exposure mode
    function currentROIAutoExposureMode(roiMode, winSize){       
        switch(roiMode){
        case See3CamCu130.AutoExpFace:
            autoexpFace.checked = true
            autoExpoWinSizeCombo.enabled = false
            break
        case See3CamCu130.AutoExpFull:
            autoexpFull.checked = true            
            autoExpoWinSizeCombo.enabled = false
            break
        case See3CamCu130.AutoExpManual:
            skipUpdateUIOnExpWindowSize = false
            autoexpManual.checked = true
            // If window size is got from camera is 0 then set window size to 1 in UI
            if(winSize == 0){
                autoExpoWinSizeCombo.currentIndex = 0
            }else
                autoExpoWinSizeCombo.currentIndex = winSize-1
            break
        case See3CamCu130.AutoExpDisabled:
            autoexpFace.enabled = false
            autoexpFull.enabled = false
            autoexpManual.enabled = false
            autoExpoWinSizeCombo.enabled = false
            break
        }
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            autoexpManual.enabled = true
            autoexpFull.enabled = true
            autoexpFace.enabled = true
            if(autoexpManual.checked)
                autoExpoWinSizeCombo.enabled = true
            if(autoexpFull.checked || autoexpFace.checked)
                autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 1
            autoexpFull.opacity = 1
            autoexpFace.opacity = 1
            exposureCompValue.enabled = true
            exposureCompValue.opacity = 1
            exposureCompSet.enabled = true
            exposureCompSet.opacity = 1
            exposureCompText.opacity = 1
        }else{
            autoexpManual.enabled = false
            autoexpFull.enabled = false
            autoexpFace.enabled = false
            autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 0.1
            autoexpFull.opacity = 0.1
            autoexpFace.opacity = 0.1
            exposureCompValue.enabled = false
            exposureCompValue.opacity = 0.1
            exposureCompSet.enabled = false
            exposureCompSet.opacity = 0.1
            exposureCompText.opacity = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
        case See3CamCu130.FLIP_ON_MIRROR_ON:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = true
            break;
        case See3CamCu130.FLIP_OFF_MIRROR_ON:
            flipCtrlVertical.checked = true
            flipCtrlHorizotal.checked = false
            break;
        case See3CamCu130.FLIP_ON_MIRROR_OFF:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = true
            break;
        case See3CamCu130.FLIP_OFF_MIRROR_OFF:
            flipCtrlVertical.checked = false
            flipCtrlHorizotal.checked = false
            break;
        }
    }

    // by Sankari: 28 June 2017
    // Reduced scene modes
    function defaultSceneMode(mode)
    {
        switch(mode)
        {
        case See3CamCu130.SCENE_NORMAL :
            scene1Button.checked = true
            break;
        case See3CamCu130.SCENE_DOCUMENT :
            scene2Button.checked = true
            break;
        }
    }

    // by Sankari: 28 June 2017
    // Reduced effect modes
    function defaultEffectMode(mode)
    {
        switch(mode)
        {
        case See3CamCu130.EFFECT_NORMAL :
            effect1Button.checked = true
            break;
        case See3CamCu130.EFFECT_BLACK_WHITE :
            effect2Button.checked = true
            break;
        case See3CamCu130.EFFECT_GRAY :
            effect3Button.checked = true
            break;
        case See3CamCu130.EFFECT_NEGATIVE :
            effect4Button.checked = true
            break;
        case See3CamCu130.EFFECT_SKETCH :
            effect5Button.checked = true
            break;
        }
    }

    function setToDefaultValues(){
        seecamcu130.setToDefault()
        getValuesFromCamera()
    }

    function getValuesFromCamera(){
        seecamcu130.getSceneMode()
        seecamcu130.getEffectMode()
        seecamcu130.getOrientation()
        seecamcu130.getExposureCompensation()
        seecamcu130.getDenoiseValue()
        seecamcu130.getBurstLength()
        seecamcu130.getQFactor()
        seecamcu130.getAutoExpROIModeAndWindowSize()
        seecamcu130.getFrameRateCtrlValue()
        seecamcu130.getFaceDetectMode()
        seecamcu130.getSmileDetectMode()
    }
}
