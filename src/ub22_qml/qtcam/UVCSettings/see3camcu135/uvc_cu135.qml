import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3camcu135 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    id: see3camcu135Id
    width:268
    height:750
    property int denoiseMin: 0
    property int denoiseMax: 15
    property int qFactorMin: 10
    property int qFactorMax: 96
    property int iHDRMin: 1
    property int iHDRMax: 4
    property int frameRateMin: 1
    property int frameRateMax: 120
    property int expoCompMin: 8000
    property int expoCompMax: 1000000    
    property bool skipUpdateUIOnSetttings : false
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIDenoise : false
    property bool skipUpdateUIFrameRate: false
    property bool skipUpdateUIiHDR: false
    property bool skipUpdateUIOnBurstLength: false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool setButtonClicked: false
    property bool skipUpdateUIFlickerCtrl:false
    property int  flickerCtrl

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 1000
        onTriggered: {
            seecamcu135.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Timer {
        id:scenemodeTimer
        interval:1000
        repeat:false
        onTriggered:{
            seecamcu135.getSceneMode()
        }
    }

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            seecamcu135.getExposureCompensation()
            seecamcu135.getFrameRateCtrlValue()
            stop()
        }
    }

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

    Timer {
        id: burstShotTimer
        interval: 1000
        onTriggered: {
            root.imageCapture(CommonEnums.BURST_SHOT);
            stop()
        }
    }

    Connections
    {
        target: root
        function onTakeScreenShot()
        {
            if(seecamcu135.enableDisableFaceRectangle(false)){
		root.imageCapture(CommonEnums.BURST_SHOT);
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
        function onMouseRightClicked(x, y, width, height){
            if(autoexpManual.enabled && autoexpManual.checked){
               seecamcu135.setROIAutoExposure(See3CamCu135.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        function onAutoExposureSelected(autoExposureSelect){
            enableDisableAutoExposureControls(autoExposureSelect)
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
        function onEnableFaceRectafterBurst(){
            seecamcu135.enableDisableFaceRectangle(true)
        }

        function onBeforeRecordVideo(){
            seecamcu135.enableDisableFaceRectangle(false)
        }
        function onAfterRecordVideo(){
            seecamcu135.enableDisableFaceRectangle(true)
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
            height:1600
            ColumnLayout{
                x:2
                y:5
                spacing:20
                Text {
                id: effectMode
                text: "--- Effect Mode ---"
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
                        onClicked: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_NORMAL)
                        }
                    }
                    RadioButton {
                        id: effectBW
                        style:  econRadioButtonStyle
                        text: qsTr("Black and White")                     
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_BLACK_WHITE)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_BLACK_WHITE)
                        }
                    }
                    RadioButton {
                        id: effectNegative
                        style:  econRadioButtonStyle
                        text: qsTr("Negative")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_NEGATIVE)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_NEGATIVE)
                        }

                    }
                    RadioButton {
                        id: effectGrayscale
                        style:  econRadioButtonStyle
                        text: qsTr("Grayscale")                      
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_GREYSCALE)
                        }
                    }
                    RadioButton {
                        id: effectSketch
                        style:  econRadioButtonStyle
                        text: qsTr("Sketch")                      
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_SKETCH)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setEffectMode(See3CamCu135.EFFECT_SKETCH)
                        }
                    }
                }
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
                            seecamcu135.setSceneMode(See3CamCu135.SCENE_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setSceneMode(See3CamCu135.SCENE_NORMAL)
                        }
                    }
                    RadioButton {
                        id: sceneDoc
                        style:  econRadioButtonStyle
                        text: qsTr("Document")                     
                        exclusiveGroup: sceneInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            seecamcu135.setSceneMode(See3CamCu135.SCENE_DOCUMENT)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setSceneMode(See3CamCu135.SCENE_DOCUMENT)
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
                                seecamcu135.setDenoiseValue(deNoiseSlider.value)
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
                                seecamcu135.setQFactor(qFactorSlider.value)
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
                            seecamcu135.setiHDRMode(See3CamCu135.HdrOff, iHDRCombo.currentText)
                            iHDRCombo.enabled = false
                            iHDRCombo.opacity = 0.1
                        }

                        Keys.onReturnPressed: {
                            seecamcu135.setiHDRMode(See3CamCu135.HdrOff, iHDRCombo.currentText)
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
                            seecamcu135.setiHDRMode(See3CamCu135.HdrAuto, iHDRCombo.currentText)
                            iHDRCombo.enabled = false
                            iHDRCombo.opacity = 0.1
                        }

                        Keys.onReturnPressed: {
                            seecamcu135.setiHDRMode(See3CamCu135.HdrAuto, iHDRCombo.currentText)
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
                            seecamcu135.setiHDRMode(See3CamCu135.HdrManual, iHDRCombo.currentText)
                            iHDRCombo.enabled = true
                            iHDRCombo.opacity = 1
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setiHDRMode(See3CamCu135.HdrManual, iHDRCombo.currentText)
                            iHDRCombo.enabled = true
                            iHDRCombo.opacity = 1
                        }
                    }
                }
                Row{
                    spacing: 35

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
                                    seecamcu135.setiHDRMode(See3CamCu135.HdrManual, iHDRCombo.currentText)
                                }
                                skipUpdateUIiHDR = true
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
                            seecamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        onClicked:{
                            seecamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
                            seecamcu135.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
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
                            seecamcu135.setROIAutoExposure(See3CamCu135.AutoExpFull, 0, 0, 0, 0, 0);
                            autoExpoWinSizeCombo.enabled = false
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setROIAutoExposure(See3CamCu135.AutoExpFull, 0, 0, 0, 0, 0);
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
                            seecamcu135.setROIAutoExposure(See3CamCu135.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                            autoExpoWinSizeCombo.enabled = true
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setROIAutoExposure(See3CamCu135.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                            seecamcu135.setROIAutoExposure(See3CamCu135.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
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
                    spacing: 10
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
                            seecamcu135.setExposureCompensation(exposureCompValue.text)
                            setButtonClicked = false
                            exposureCompSet.enabled = true
                        }
                        Keys.onReturnPressed: {
                            exposureCompSet.enabled = false
                            setButtonClicked = true
                            seecamcu135.setExposureCompensation(exposureCompValue.text)
                            setButtonClicked = false
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
                                seecamcu135.setFrameRateCtrlValue(frameRateSlider.value)
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
                            seecamcu135.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: faceRectGroup
                        id:faceRectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {                            
                            seecamcu135.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                        }
                        Keys.onReturnPressed: {                            
                            seecamcu135.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
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
                            seecamcu135.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                        }
                        Keys.onReturnPressed: {
                            seecamcu135.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
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
                            seecamcu135.setSmileDetection(true, smileDetectEmbedData.checked)
                        }
                        Keys.onReturnPressed: {                            
                            seecamcu135.setSmileDetection(true, smileDetectEmbedData.checked)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: smileDetectGroup
                        id:smileDetectDisable
                        text: "Disable"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {                            
                            seecamcu135.setSmileDetection(false, smileDetectEmbedData.checked)
                        }
                        Keys.onReturnPressed: {                            
                            seecamcu135.setSmileDetection(false, smileDetectEmbedData.checked)
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
                        ListElement { text: "AUTO" }
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
                        id: f_wversion_selectedCU135
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
    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function setMasterMode(){
        root.checkForTriggerMode(false)
        seecamcu135.setStreamMode(See3CamCu135.STREAM_MASTER)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
    }

    // set to default values
    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        seecamcu135.setToDefault()
        getValuesFromCamera()
        root.keyEventFiltering = true
    }

    // Enable Face detect embed data
    function enableFaceDetectEmbedData(){
        if(seecamcu135.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
            if(faceDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by face data.Refer document See3CAM_CU135_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }
    }

    // enable smile detect embed data
    function enableSmileDetectEmbedData(){
        setButtonClicked = false
        if(seecamcu135.setSmileDetection(true, smileDetectEmbedData.checked)){
            if(smileDetectEmbedData.checked){
                messageDialog.title = qsTr("Status")
                messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document See3CAM_CU135_Face_and_Smile_Detection for more details")
                messageDialog.open()
            }
        }
    }

    // enable/disable auto exposure controls in extension settings based on selection of expoure in Image Quality Settings
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

    // Effect mode
    function currentEffectModeValue(mode)
    {
        switch(mode)
        {
            case See3CamCu135.EFFECT_NORMAL:
                effectNormal.checked = true
                break;
            case See3CamCu135.EFFECT_BLACK_WHITE:
                effectBW.checked = true
                break;
            case See3CamCu135.EFFECT_GREYSCALE:
                effectGrayscale.checked = true
                break;
            case See3CamCu135.EFFECT_NEGATIVE:
                effectNegative.checked = true
                break;
            case See3CamCu135.EFFECT_SKETCH:
                effectSketch.checked = true
                break;
        }
    }

    // scene mode
    function currentSceneModeValue(mode)
    {
        switch(mode)
        {
            case See3CamCu135.SCENE_NORMAL:
                sceneNormal.checked = true
                break;
            case See3CamCu135.SCENE_DOCUMENT:
                sceneDoc.checked = true
                break;
        }
    }

    // HDR mode
    function currentHDRMode(mode)
    {
        switch(mode)
        {
            case See3CamCu135.HdrOff:
                hdrOff.checked = true

                iHDRCombo.enabled = false
                iHDRCombo.opacity = 0.1
                break;
            case See3CamCu135.HdrAuto:
                hdrAuto.checked = true

                iHDRCombo.enabled = false
                iHDRCombo.opacity = 0.1
                break;
            case See3CamCu135.HdrManual:
                hdrManual.checked = true

                iHDRCombo.enabled = true
                iHDRCombo.opacity = 1
                break;
        }
    }

    // set trigger stream mode
    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        seecamcu135.setStreamMode(See3CamCu135.STREAM_TRIGGER)
        displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document See3CAM_CU135_Trigger_Mode"))
    }

    // Get the control values in extension settings
    function getValuesFromCamera(){
        scenemodeTimer.start()       // To call getSceneMode API after some delay
        seecamcu135.getEffectMode()
        seecamcu135.getSceneMode()
        seecamcu135.getDenoiseValue()
        seecamcu135.getQFactor()
        seecamcu135.getiHDRMode()
        seecamcu135.getStreamMode()
        seecamcu135.getBurstLength()
        seecamcu135.getFlickerDetection();
        seecamcu135.getAutoExpROIModeAndWindowSize()        
        seecamcu135.getFaceDetectMode()
        seecamcu135.getSmileDetectMode()
        seecamcu135.getOrientation()
        getexposureCompFrameRateCtrlTimer.start()
    }

    // Flip mirror modes
    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case See3CamCu135.SetBothFlipEnable:
                 flipCtrlVertical.checked = true
                 flipCtrlHorizotal.checked = true
                break;
            case See3CamCu135.SetVertiFlip:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case See3CamCu135.SetHorzFlip:
                 flipCtrlVertical.checked = false
                 flipCtrlHorizotal.checked = true
                break;
            case See3CamCu135.SetBothFlipDisable:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
        }
    }

    // current ROI auto exposure mode
    function currentROIAutoExposureMode(roiMode, winSize){
        switch(roiMode){
        case See3CamCu135.AutoExpFull:
            autoexpFull.checked = true
            autoExpoWinSizeCombo.enabled = false
            break
        case See3CamCu135.AutoExpManual:
            skipUpdateUIOnExpWindowSize = false
            autoexpManual.checked = true
            // If window size is got from camera is 0 then set window size to 1 in UI
            if(winSize == 0){
                autoExpoWinSizeCombo.currentIndex = 0
            }else
                autoExpoWinSizeCombo.currentIndex = winSize-1
            break
        case See3CamCu135.AutoExpDisabled:
            autoexpFull.enabled = false
            autoexpManual.enabled = false
            autoExpoWinSizeCombo.enabled = false
            break
        }
    }

    function setFlickerDetectionFn()
    {
        switch(flickercombo.currentIndex){
        case 0:
            flickerCtrl = See3CamCu135.MODE_AUTO
            break
        case 1:
            flickerCtrl = See3CamCu135.MODE_50Hz
            break
        case 2:
            flickerCtrl = See3CamCu135.MODE_60Hz
            break
        case 3:
            flickerCtrl = See3CamCu135.MODE_DISABLE
            break
        }
        seecamcu135.setFlickerDetection(flickerCtrl)
    }

    Uvccamera {
        id: uvccamera
        onTitleTextChanged: {
            displayMessageBox(qsTr(_title), qsTr(_text))
        }
        onSerialNumber:{
            displayMessageBox(qsTr("Serial Number"), serialNumber)
        }
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    See3CamCu135 {
        id: seecamcu135
        onSceneModeValue:{
            currentSceneModeValue(sceneMode)
        }
        onEffectModeValue:{
            currentEffectModeValue(effectMode)
        }
        onDenoiseValue:{
            skipUpdateUIDenoise = false
            deNoiseSlider.value = denoiseVal
        }
        onQFactorValue:{
            skipUpdateUIQFactor = false
            qFactorSlider.value = qFactor
        }
        onHdrModeValue:{
            currentHDRMode(hdrMode)
            if(hdrMode == See3CamCu135.HdrManual){
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
        onStreamModeValue:{
            if(streamMode == See3CamCu135.STREAM_MASTER){
                streamMaster.checked = true
                root.captureBtnEnable(true)
                root.videoRecordBtnEnable(true)
            }else if(streamMode == See3CamCu135.STREAM_TRIGGER){
                streamTrigger.checked = true
                root.captureBtnEnable(false)
                root.videoRecordBtnEnable(false)
                messageDialog.title = "Trigger Mode"
                messageDialog.text = "Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document."
                messageDialog.open()
            }

        }
        onFlickerDetectionMode:{

            skipUpdateUIFlickerCtrl = false
            if(flickerMode == See3CamCu135.MODE_AUTO){
                flickercombo.currentIndex = 0
            }else if(flickerMode == See3CamCu135.MODE_50Hz){
                flickercombo.currentIndex  = 1
            }else if(flickerMode == See3CamCu135.MODE_60Hz){
                flickercombo.currentIndex  = 2
            }else if(flickerMode == See3CamCu135.MODE_DISABLE){
                flickercombo.currentIndex  = 3
            }else{ }
            skipUpdateUIFlickerCtrl = true;
        }

        onFlipMirrorModeChanged:{
            currentFlipMirrorMode(flipMirrorMode)
        }
        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
        }
        onRoiAutoExpMode:{
            currentROIAutoExposureMode(roiMode, winSize)
        }
        onExposureCompValue:{
            exposureCompValue.text = exposureCompensation
        }
        onFrameRateCtrlValue:{
            skipUpdateUIFrameRate = false
            frameRateSlider.value = frameRateVal
        }
        onFaceDetectModeValue:{            
            if(faceDetectMode == See3CamCu135.FaceRectEnable){
                faceRectEnable.checked = true
                if(faceDetectEmbedDataValue == See3CamCu135.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }
                if(faceDetectOverlayRect == See3CamCu135.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }
            }else if(faceDetectMode == See3CamCu135.FaceRectDisable){
                faceRectDisable.checked = true
                if(faceDetectEmbedDataValue == See3CamCu135.FaceDetectEmbedDataEnable){
                    faceDetectEmbedData.checked = true
                }else{
                    faceDetectEmbedData.checked = false
                }
                if(faceDetectOverlayRect == See3CamCu135.FaceDetectOverlayRectEnable){
                    overlayRect.checked = true
                }else{
                    overlayRect.checked = false
                }
            }
        }
        onSmileDetectModeValue:{
            if(smileDetectMode == See3CamCu135.SmileDetectEnable){
                smileDetectEnable.checked = true
                if(smileDetectEmbedDataValue == See3CamCu135.SmileDetectEmbedDataEnable){
                    smileDetectEmbedData.checked = true
                }
            }else if(smileDetectMode == See3CamCu135.SmileDetectDisable){
                smileDetectDisable.checked = true
                if(smileDetectEmbedDataValue == See3CamCu135.SmileDetectEmbedDataEnable){
                    smileDetectEmbedData.checked = true
                }else{
                    smileDetectEmbedData.checked = false
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
                seecamcu135.getExposureCompensation()
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

    Component.onCompleted: {
        getValuesFromCamera()
        root.disablePowerLineFreq()
    }
}
