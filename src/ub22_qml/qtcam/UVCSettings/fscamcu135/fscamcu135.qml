import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.fscamcu135 1.0
import econ.camera.stream 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0
import "../../JavaScriptFiles/tempValue.js" as JS

Item {
    id: fscamcu135Id
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

    property int stillFormatCurrentIndex
    property int stillResolutionCurrentIndex
    property bool skipUpdateUIOnSetttings : false
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIDenoise : false
    property bool skipUpdateUIFrameRate: false
    property bool skipUpdateUIiHDR: false
    property bool skipUpdateUIOnExpWindowSize: false
    property bool setButtonClicked: false
    property bool skipUpdateUIFlickerCtrl:false
    property int  flickerCtrl


    // Used when selecting auto exposure in image Quality settings menu

    Timer {
        id: getexposureCompFrameRateCtrlTimer
        interval: 500
        onTriggered: {
            fscamcu135.getExposureCompensation()
            fscamcu135.getFrameRateCtrlValue()
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
        id: enableSettings
        interval: 3000
        onTriggered: {
            root.enableAllSettingsTab()
            stop()
        }
    }

    Timer {
        id:scenemodeTimer
        interval:1000
        repeat:false
        onTriggered:{
            fscamcu135.getSceneMode()
        }
    }

    Timer {
        id: snapShotTimer
        interval: 1000
        onTriggered: {
            root.imageCapture(CommonEnums.SNAP_SHOT);
            stop()
        }
    }

    Timer{
        id:setMaster
        interval:1000
        repeat: false
        onTriggered: {
            setMasterOnDemandMode()
        }
    }
    Videostreaming{
        id:videostream
    }

    Connections
    {
        target: root
        function onTakeScreenShot()
        {
            if(streamSwTrigger.checked || streamHwTrigger.checked){
                root.disableSaveImage()
                enableSettings.start()
            }
            else{
            root.imageCapture(CommonEnums.SNAP_SHOT);}

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
        function onMouseRightClicked(x, Y, width, height){
            if(autoexpManual.enabled && autoexpManual.checked){
                fscamcu135.setROIAutoExposure(Fscamcu135.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
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

        }
        function onBeforeRecordVideo(){

        }
        function onAfterRecordVideo(){

        }
        function onQueryFrame(retriveframe, InFailureCase){
            if((streamMasterOnDemand.checked || streamHwTrigger.checked)){
                if(retriveframe){
                    if(!fscamcu135.grabStillFrame(frameIndexCombo.currentIndex, stillFormatCurrentIndex+1, stillResolutionCurrentIndex+1)){
                        if(!InFailureCase){
                            displayMessageBox("Failure", "Image is not available in given index")
                            root.switchToCamFrameSettings(false)   // To get the preview back after failure case
                        }
                    }
                }else{

                    fscamcu135.grabPreviewFrame()
                }
            }
        }

        function onStillFormatChanged(stillFormatcurrentIndex, stillResolncurrentIndex){
            stillFormatCurrentIndex = stillFormatcurrentIndex
            stillResolutionCurrentIndex = stillResolncurrentIndex        
            fscamcu135.setStillResolution(stillFormatCurrentIndex+1, stillResolutionCurrentIndex+1);
            fscamcu135.getNumberOfFramesCanStore(stillFormatCurrentIndex+1, stillResolutionCurrentIndex+1)

        }

        function onStillResolutionChanged(stillresolutionIndex, stillFormatIndex){
            stillResolutionCurrentIndex = stillresolutionIndex          
            stillFormatCurrentIndex = stillFormatIndex
            fscamcu135.setStillResolution(stillFormatCurrentIndex+1, stillResolutionCurrentIndex+1);
            fscamcu135.getNumberOfFramesCanStore(stillFormatCurrentIndex+1, stillResolutionCurrentIndex+1)
        }

        function onCaptureFrameTimeout(){
            if(streamMasterOnDemand.checked){
                root.enableAllSettingsTab()
                fscamcu135.grabPreviewFrame()
            }
        }
        function onExtensionTabVisible(visible){
            if(visible){               
                frameIndexCombo.currentIndex = 0
            }
        }
        function onSkipFrameCount(){
            root.setSkipFrameCount(3)
        }
    }

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle
        Item{
            height:1900
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
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_NORMAL)
                        }
                    }
                    RadioButton {
                        id: effectBW
                        style:  econRadioButtonStyle
                        text: qsTr("Black and White")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_BLACK_WHITE)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_BLACK_WHITE)
                        }
                    }
                    RadioButton {
                        id: effectNegative
                        style:  econRadioButtonStyle
                        text: qsTr("Negative")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_NEGATIVE)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_NEGATIVE)
                        }

                    }
                    RadioButton {
                        id: effectGrayscale
                        style:  econRadioButtonStyle
                        text: qsTr("Grayscale")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_GREYSCALE)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_GREYSCALE)
                        }
                    }
                    RadioButton {
                        id: effectSketch
                        style:  econRadioButtonStyle
                        text: qsTr("Sketch")
                        exclusiveGroup: effectInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_SKETCH)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setEffectMode(Fscamcu135.EFFECT_SKETCH)
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
                            fscamcu135.setSceneMode(Fscamcu135.SCENE_NORMAL)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setSceneMode(Fscamcu135.SCENE_NORMAL)
                        }
                    }
                    RadioButton {
                        id: sceneDoc
                        style:  econRadioButtonStyle
                        text: qsTr("Document")
                        exclusiveGroup: sceneInputGroup
                        activeFocusOnPress: true
                        onClicked: {
                            fscamcu135.setSceneMode(Fscamcu135.SCENE_DOCUMENT)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setSceneMode(Fscamcu135.SCENE_DOCUMENT)
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
                                fscamcu135.setDenoiseValue(deNoiseSlider.value)
                            }
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
                                fscamcu135.setQFactor(qFactorSlider.value)
                            }
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
                            fscamcu135.setiHDRMode(Fscamcu135.HdrOff, 0)
                        }

                        Keys.onReturnPressed: {
                            fscamcu135.setiHDRMode(Fscamcu135.HdrOff, 0)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: iHDRgroup
                        id:hdrAuto
                        text: "Auto"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            fscamcu135.setiHDRMode(Fscamcu135.HdrAuto, 0)
                        }

                        Keys.onReturnPressed: {
                            fscamcu135.setiHDRMode(Fscamcu135.HdrAuto, 0)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: iHDRgroup
                        id: hdrManual
                        text: "Manual"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked:{
                            fscamcu135.setiHDRMode(Fscamcu135.HdrManual, iHDRSlider.value)
                        }
                        Keys.onReturnPressed: {
                            fscamcu135.setiHDRMode(Fscamcu135.HdrManual, iHDRSlider.value)
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
                                fscamcu135.setiHDRMode(Fscamcu135.HdrManual, iHDRSlider.value)
                            }
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
                    Column{
                        spacing:20
                        ExclusiveGroup { id: streamModeGroup }
                        RadioButton {
                            exclusiveGroup: streamModeGroup
                            id: streamMasterOnDemand
                            checked:false
                            text: "Master"                    // We need to send "queryNext" hid command every time. It gives one frame at a time
                            activeFocusOnPress: true          // grabPreviewFrame
                            style: econRadioButtonStyle
                            onClicked:{
                               root.checkForTriggerMode(false)
                               root.skipFrameInPreview(true)
                               setMasterOnDemandMode()
                               fscamcu135.grabPreviewFrame()

                            }
                            Keys.onReturnPressed: {
                                root.skipFrameInPreview(true)
                                setMasterOnDemandMode()
                                fscamcu135.grabPreviewFrame()
                            }
                        }
                        Row{
                            spacing : 10
                            RadioButton {
                                exclusiveGroup: streamModeGroup  // We need to send commands for storeframe and grab frame
                                id: streamSwTrigger
                                text: "Software Trigger Mode"
                                activeFocusOnPress: true
                                style: econRadioButtonStyle
                                onClicked: {           
                                    root.checkForTriggerMode(true)
                                    root.captureBtnEnable(false)
                                    root.videoRecordBtnEnable(false)
                                    fscamcu135.setStreamMode(Fscamcu135.STREAM_SOFTWARE_TRIGGER)
                                    root.skipFrameInPreview(false)

                                }
                                Keys.onReturnPressed: {
                                    root.captureBtnEnable(false)
                                    root.videoRecordBtnEnable(false)
                                    fscamcu135.setStreamMode(Fscamcu135.STREAM_SOFTWARE_TRIGGER)
                                    root.skipFrameInPreview(false)

                                }
                            }
                            Button {
                                id: swTrigger
                                activeFocusOnPress : true
                                text: "Grab"
                                style: econButtonStyle
                                opacity: streamSwTrigger.checked ? 1 : 0.1
                                implicitHeight: 20
                                implicitWidth: 60
                                onClicked: {
                                    fscamcu135.storePreviewFrame()
                                    fscamcu135.grabPreviewFrame()

                                }
                                Keys.onReturnPressed: {
                                    fscamcu135.storePreviewFrame()
                                    fscamcu135.grabPreviewFrame()
                                }
                            }
                        }
                            RadioButton {
                                exclusiveGroup: streamModeGroup
                                id: streamHwTrigger
                                text: "Hardware Trigger Mode"
                                activeFocusOnPress: true
                                style: econRadioButtonStyle
                                onClicked: {                            
                                    root.checkForTriggerMode(true)
                                    root.captureBtnEnable(false)
                                    root.videoRecordBtnEnable(false)
                                    fscamcu135.setStreamMode(Fscamcu135.STREAM_HARDWARE_TRIGGER)
                                    root.skipFrameInPreview(false)
                                }
                                Keys.onReturnPressed: {
                                    root.captureBtnEnable(false)
                                    root.videoRecordBtnEnable(false)
                                    fscamcu135.setStreamMode(Fscamcu135.STREAM_HARDWARE_TRIGGER)
                                    root.skipFrameInPreview(false)
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
                                fscamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            }
                        }
                        CheckBox {
                            id: flipCtrlVertical
                            activeFocusOnPress : true
                            text: "Vertical"
                            style: econCheckBoxStyle
                            onClicked:{
                                fscamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                            }
                        }
                    }
                    Text {
                        id: imgCapText
                        text: "--- Still Image Capture ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }
                    Text {
                        id: storeFrameText
                        text: "Store Frame:"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                    Button {
                        id: storeFrame
                        activeFocusOnPress : true
                        text: "Store"
                        tooltip: "This device supports on-board still store and retrieve option. Each resolution and format support certain number of still frames can store into the camera."
                        style: econButtonStyle
                        enabled: streamMasterOnDemand.checked ? true : false
                        opacity: enabled ? 1 : 0.1
                        implicitHeight: 30
                        implicitWidth: 100
                        onClicked: {                          
                            storeImageInCamera()
                        }
                        Keys.onReturnPressed: {
                           storeImageInCamera()
                        }
                    }
                    Text {
                        id: retrieveFrameText
                        text: "Retrieve Frame:"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }

                    Text {
                        id: frameIndex
                        text: "Frame index :"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 1
                    }
                  Row{
                        spacing: 10

                        ComboBox
                        {
                            id: frameIndexCombo
                            enabled: streamMasterOnDemand.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            model: ListModel {
                                id: frameIndexModel
                            }
                            activeFocusOnPress: true

                            style: ComboBoxStyle {
                                background: Image {
                                    id: frameIndexBg
                                    source: "../../Views/images/smallsizeCombobox.png"
                                    Rectangle {
                                        width: frameIndexBg.sourceSize.width - 20
                                        height: frameIndexBg.sourceSize.height + 3
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

                            }
                        }
                        Button {
                            id: retrieveFrame
                            activeFocusOnPress : true
                            text: "Retrieve"
                            enabled: streamMasterOnDemand.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            style: econButtonStyle
                            implicitHeight: 25
                            implicitWidth: 100
                            onClicked: {
                                root.retrieveFrameFromStorageCamera()
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
                            id: autoexpFull
                            text: "Full"
                            checked:true
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            opacity: enabled ? 1 : 0.1
                            // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                            // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                            // winSize is required only for manual mode
                            onClicked: {
                                fscamcu135.setROIAutoExposure(Fscamcu135.AutoExpFull, 0, 0, 0, 0, 0);
                                autoExpoWinSizeCombo.enabled = false
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setROIAutoExposure(Fscamcu135.AutoExpFull, 0, 0, 0, 0, 0);
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
                                fscamcu135.setROIAutoExposure(Fscamcu135.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                                autoExpoWinSizeCombo.enabled = true
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setROIAutoExposure(Fscamcu135.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                                fscamcu135.setROIAutoExposure(Fscamcu135.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
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
                                fscamcu135.setExposureCompensation(exposureCompValue.text)
                                setButtonClicked = false
                                exposureCompSet.enabled = true
                            }
                            Keys.onReturnPressed: {
                                exposureCompSet.enabled = false
                                setButtonClicked = true
                                fscamcu135.setExposureCompensation(exposureCompValue.text)
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
                                    fscamcu135.setFrameRateCtrlValue(frameRateSlider.value)
                                }
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
                                fscamcu135.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setFaceDetectionRect(true, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                        }
                        RadioButton {
                            exclusiveGroup: faceRectGroup
                            id:faceRectDisable
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                fscamcu135.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setFaceDetectionRect(false, faceDetectEmbedData.checked, overlayRect.checked)
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
                                fscamcu135.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, checked)
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
                                fscamcu135.setSmileDetection(true, smileDetectEmbedData.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setSmileDetection(true, smileDetectEmbedData.checked)
                            }
                        }
                        RadioButton {
                            exclusiveGroup: smileDetectGroup
                            id:smileDetectDisable
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {
                                fscamcu135.setSmileDetection(false, smileDetectEmbedData.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setSmileDetection(false, smileDetectEmbedData.checked)
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

                    Text {
                        id: ledCtrl
                        text: "--- LED Control ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }

                    Row{
                        spacing: 62
                        ExclusiveGroup { id:ledControlGroup  }
                        RadioButton {
                            exclusiveGroup: ledControlGroup
                            id: ledctrlenable
                            text: "Enable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked:{

                                fscamcu135.setLedControl(true,powerOn.checked,streaming.checked,triggerCtrl.checked)
                            }
                            Keys.onReturnPressed: {
                                 fscamcu135.setLedControl(true,powerOn.checked,streaming.checked,triggerCtrl.checked)
                            }
                        }
                        RadioButton {
                            exclusiveGroup: ledControlGroup
                            id: ledctrldisable
                            text: "Disable"
                            activeFocusOnPress: true
                            style: econRadioButtonStyle
                            onClicked: {

                                    fscamcu135.setLedControl(false,powerOn.checked,streaming.checked,triggerCtrl.checked)
                            }
                            Keys.onReturnPressed: {
                                     fscamcu135.setLedControl(false,powerOn.checked,streaming.checked,triggerCtrl.checked)
                            }
                        }
                    }

                    Grid{
                        columns :2
                        spacing: 10
                        ExclusiveGroup { id: enableledGrp }
                        CheckBox {
                            id: powerOn
                            activeFocusOnPress : true
                            text: "PowerON"
                            style: econCheckBoxStyle
                            enabled: ledctrlenable.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            onClicked:{
                                fscamcu135.setLedControl(ledctrlenable.checked,powerOn.checked,streaming.checked,triggerCtrl.checked)
                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setLedControl(ledctrlenable.checked,powerOn.checked,streaming.checked,triggerCtrl.checked)

                            }
                        }
                        CheckBox {
                            id: streaming
                            activeFocusOnPress : true
                            text: "Streaming"
                            style: econCheckBoxStyle
                            enabled: ledctrlenable.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            onClicked:{
                                  fscamcu135.setLedControl(ledctrlenable.checked,powerOn.checked,streaming.checked,triggerCtrl.checked)

                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setLedControl(ledctrlenable.checked,powerOn.checked,streaming.checked,triggerCtrl.checked)

                            }
                        }
                        CheckBox {
                            id: triggerCtrl
                            activeFocusOnPress : true
                            text: "Trigger ACK"
                            style: econCheckBoxStyle
                            enabled: ledctrlenable.checked ? true : false
                            opacity: enabled ? 1 : 0.1
                            onClicked:{
                                fscamcu135.setLedControl(ledctrlenable.checked,powerOn.checked,streaming.checked,triggerCtrl.checked)

                            }
                            Keys.onReturnPressed: {
                                fscamcu135.setLedControl(ledctrlenable.checked,powerOn.checked,streaming.checked,triggerCtrl.checked)

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

      function setMasterOnDemandMode(){
            root.captureBtnEnable(true)
            root.videoRecordBtnEnable(true)
            root.startUpdatePreviewInMasterMode()
            fscamcu135.setStreamMode(Fscamcu135.STREAM_MASTER_ONDEMAND)

            fscamcu135.grabPreviewFrame()

        }

        function displayMessageBox(title, text){
            messageDialog.title = qsTr(title)
            messageDialog.text = qsTr(text)
            messageDialog.open()

        }

        // set to default values
        function setToDefaultValues(){
            root.checkForTriggerMode(false)
            fscamcu135.setToDefault()
           //Added by Navya:28-Feb-2019 -To get a popup window
            messageDialog.title=qsTr("Restore Default")
            messageDialog.text=qsTr("When we click Extension Settings default option, all the controls will be updated to their default values except stream mode. The stream mode control can be changed manually")
            messageDialog.open()
            getValuesFromCamera()

            // Added by Sankari: 24 Apr 2017. To get preview in master mode
            root.startUpdatePreviewInMasterMode()
        }

        // Enable Face detect embed data
        function enableFaceDetectEmbedData(){
            if(fscamcu135.setFaceDetectionRect(faceRectEnable.checked, faceDetectEmbedData.checked, overlayRect.checked)){
                if(faceDetectEmbedData.checked){
                    messageDialog.title = qsTr("Status")
                    messageDialog.text = qsTr("The last part of the frame will be replaced by face data.Refer document FSCAM_CU135_Face_and_Smile_Detection_Application_Note for more details")
                    messageDialog.open()
                }
            }
        }

        // enable smile detect embed data
        function enableSmileDetectEmbedData(){
            if(fscamcu135.setSmileDetection(true, smileDetectEmbedData.checked)){
                if(smileDetectEmbedData.checked){
                    messageDialog.title = qsTr("Status")
                    messageDialog.text = qsTr("The last part of the frame will be replaced by smile data.Refer document FSCAM_CU135_Face_and_Smile_Detection_Application_Note for more details")
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
            fscamcu135.getAutoExpROIModeAndWindowSize()
        }

        // Effect mode
        function currentEffectModeValue(mode)
        {

            switch(mode)
            {
            case Fscamcu135.EFFECT_NORMAL:
                effectNormal.checked = true
                break;
            case Fscamcu135.EFFECT_BLACK_WHITE:
                effectBW.checked = true
                break;
            case Fscamcu135.EFFECT_GREYSCALE:
                effectGrayscale.checked = true
                break;
            case Fscamcu135.EFFECT_NEGATIVE:
                effectNegative.checked = true
                break;
            case Fscamcu135.EFFECT_SKETCH:
                effectSketch.checked = true
                break;
            }
        }

        // scene mode
        function currentSceneModeValue(mode)
        {

            switch(mode)
            {
            case Fscamcu135.SCENE_NORMAL:
                sceneNormal.checked = true
                break;
            case Fscamcu135.SCENE_DOCUMENT:
                sceneDoc.checked = true
                break;
            }
        }

        // HDR mode
        function currentHDRMode(mode)
        {
            switch(mode)
            {
            case Fscamcu135.HdrOff:
                hdrOff.checked = true
                break;
            case Fscamcu135.HdrAuto:
                hdrAuto.checked = true
                break;
            case Fscamcu135.HdrManual:
                hdrManual.checked = true
                break;
            }
        }

        // set trigger stream mode
        function setStreamModes(){
            root.stopUpdatePreviewInTriggerMode()
            fscamcu135.setStreamMode(Fscamcu135.STREAM_TRIGGER)
            displayMessageBox(qsTr("Trigger Mode"), qsTr("Frames will be out only when external hardware pulses are given to PIN 5 of CN3. Refer the document See3CAM_CU135_Trigger_Mode"))
        }

        // Get the control values in extension settings
        function getValuesFromCamera(){
            scenemodeTimer.start()
            fscamcu135.getEffectMode()
            fscamcu135.getSceneMode()
            fscamcu135.getDenoiseValue()
            fscamcu135.getQFactor()
            fscamcu135.getiHDRMode()
            fscamcu135.getStreamMode()
            fscamcu135.getAutoExpROIModeAndWindowSize()
            fscamcu135.getFaceDetectMode()
            fscamcu135.getSmileDetectMode()
            fscamcu135.getOrientation()
            fscamcu135.getLedControl()
            fscamcu135.getFlickerDetection()
            getexposureCompFrameRateCtrlTimer.start()

        }

        // Flip mirror modes
        function currentFlipMirrorMode(mode)
        {
            switch(mode)
            {
            case Fscamcu135.SetBothFlipEnable:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = true
                break;
            case Fscamcu135.SetVertiFlip:
                flipCtrlVertical.checked = true
                flipCtrlHorizotal.checked = false
                break;
            case Fscamcu135.SetHorzFlip:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = true
                break;
            case Fscamcu135.SetBothFlipDisable:
                flipCtrlVertical.checked = false
                flipCtrlHorizotal.checked = false
                break;
            }
        }

        function setFlickerDetectionFn()
        {
            switch(flickercombo.currentIndex){
            case 0:
                flickerCtrl= Fscamcu135.MODE_AUTO
                break
            case 1:
                flickerCtrl = Fscamcu135.MODE_50Hz
                break
            case 2:
                flickerCtrl = Fscamcu135.MODE_60Hz
                break
            case 3:
                flickerCtrl = Fscamcu135.MODE_DISABLE
                break
            }
            fscamcu135.setFlickerDetection(flickerCtrl)
        }

        // Added by Sankari: Added message box to intimate user about success/failure while storing image in the camera
        function storeImageInCamera(){
            if(fscamcu135.storeStillFrame(stillFormatCurrentIndex+1, stillResolutionCurrentIndex+1)){
                messageDialog.title = qsTr("Success")
                messageDialog.text = qsTr("Image is saved successfully")
                messageDialog.open()
            }else{
                messageDialog.title = qsTr("Failure")
                messageDialog.text = qsTr("Failed to save image")
                messageDialog.open()
            }

        }

        // current ROI auto exposure mode
        function currentROIAutoExposureMode(roiMode, winSize){
            switch(roiMode){           
            case Fscamcu135.AutoExpFull:
                autoexpFull.checked = true
                autoExpoWinSizeCombo.enabled = false
                break
            case Fscamcu135.AutoExpManual:
                skipUpdateUIOnExpWindowSize = false
                autoexpManual.checked = true
                // If window size is got from camera is 0 then set window size to 1 in UI
                if(winSize == 0){
                    autoExpoWinSizeCombo.currentIndex = 0
                }else
                    autoExpoWinSizeCombo.currentIndex = winSize-1
                break
            case Fscamcu135.AutoExpDisabled:
                autoexpFull.enabled = false
                autoexpManual.enabled = false
                autoExpoWinSizeCombo.enabled = false
                break
            }
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

        function fillFrameIndexCombo(numOfImages){ // need to fill based on resolution
            frameIndexModel.clear()
            for (var i=1; i <=numOfImages; i++){
                frameIndexModel.append({"text": i})
            }
        }


        Fscamcu135 {
            id: fscamcu135
            onSceneModeValue:{

                currentSceneModeValue(sceneMode)
            }
            onEffectModeValue:{
                currentEffectModeValue(effectMode)
            }
            onDenoiseValue:{
                skipUpdateUIDenoise = false
                deNoiseSlider.value = denoiseVal
                skipUpdateUIDenoise = true
            }
            onQFactorValue:{
                skipUpdateUIQFactor = false
                qFactorSlider.value = qFactor
                skipUpdateUIQFactor = true
            }
            onHdrModeValue:{
                currentHDRMode(hdrMode)
                if(hdrMode == Fscamcu135.HdrManual){
                    iHDRSlider.value = hdrValue
                }
                 skipUpdateUIiHDR = true
            }
            onStreamModeValue:{
                if(streamMode == Fscamcu135.STREAM_MASTER_ONDEMAND){
                    streamMasterOnDemand.checked = true
                    root.checkForTriggerMode(false)
                    root.captureBtnEnable(true)
                    root.videoRecordBtnEnable(true)
                    root.startUpdatePreviewInMasterMode()
                 //   root.enableTimerforGrabPreviewFrame(true)
                }else if(streamMode == Fscamcu135.STREAM_SOFTWARE_TRIGGER){
                    streamSwTrigger.checked = true
                    root.videoRecordBtnEnable(false)
                    root.captureBtnEnable(false)
                    root.checkForTriggerMode(true)
                    root.stopUpdatePreviewInTriggerMode()
                }else if(streamMode == Fscamcu135.STREAM_HARDWARE_TRIGGER){
                    streamHwTrigger.checked = true
                    root.videoRecordBtnEnable(false)
                    root.captureBtnEnable(false)
                    root.checkForTriggerMode(true)
                    root.stopUpdatePreviewInTriggerMode()
                }
            }
            onFlickerDetectionMode:{
                skipUpdateUIFlickerCtrl = false
                if(flickerMode == Fscamcu135.MODE_AUTO){
                    flickercombo.currentIndex = 0
                }else if(flickerMode == Fscamcu135.MODE_50Hz){
                    flickercombo.currentIndex  = 1
                }else if(flickerMode == Fscamcu135.MODE_60Hz){
                    flickercombo.currentIndex  = 2
                }else if(flickerMode == Fscamcu135.MODE_DISABLE){
                    flickercombo.currentIndex  = 3
                }else{ }
                skipUpdateUIFlickerCtrl = true;
            }

            onFlipMirrorModeChanged:{
                currentFlipMirrorMode(flipMirrorMode)
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
                 skipUpdateUIFrameRate = true
            }
            onFaceDetectModeValue:{
                if(faceDetectMode == Fscamcu135.FaceRectEnable){
                    faceRectEnable.checked = true
                    if(faceDetectEmbedDataValue == Fscamcu135.FaceDetectEmbedDataEnable){
                        faceDetectEmbedData.checked = true
                    }
                    if(faceDetectOverlayRect == Fscamcu135.FaceDetectOverlayRectEnable){
                        overlayRect.checked = true
                    }
                }else if(faceDetectMode == Fscamcu135.FaceRectDisable){
                    faceRectDisable.checked = true
                    if(faceDetectEmbedDataValue == Fscamcu135.FaceDetectEmbedDataEnable){
                        faceDetectEmbedData.checked = true
                    }else{
                        faceDetectEmbedData.checked = false
                    }
                    if(faceDetectOverlayRect == Fscamcu135.FaceDetectOverlayRectEnable){
                        overlayRect.checked = true
                    }else{
                        overlayRect.checked = false
                    }
                }
            }
            onLedControlStatus: {
                if(ledstatus == Fscamcu135.LedControlEnable){
                    ledctrlenable.checked = true
                    if(powerctl == Fscamcu135.PowerOnControlEnable){
                        powerOn.checked = true 
                    }
                    if(stream == Fscamcu135.StreamingControlEnable){
                        streaming.checked = true
                    }
                    if(trigger == Fscamcu135.TriggerACKControlEnable){

                        triggerCtrl.checked = true
                    }
                }else if(ledstatus == Fscamcu135.LedControlDisable){

                    ledctrldisable.checked = true
                    powerOn.checked = false
                    streaming.checked = false
                    triggerCtrl.checked = false
                }
            }

            onSmileDetectModeValue:{
                if(smileDetectMode == Fscamcu135.SmileDetectEnable){
                    smileDetectEnable.checked = true
                    if(smileDetectEmbedDataValue == Fscamcu135.SmileDetectEmbedDataEnable){
                        smileDetectEmbedData.checked = true
                    }
                }else if(smileDetectMode == Fscamcu135.SmileDetectDisable){
                    smileDetectDisable.checked = true
                    if(smileDetectEmbedDataValue == Fscamcu135.SmileDetectEmbedDataEnable){
                        smileDetectEmbedData.checked = true
                    }else{
                        smileDetectEmbedData.checked = false
                    }
                }
            }
            onIndicateCommandStatus:{
                displayMessageBox(title, text)
            }

            onIndicateExposureValueRangeFailure:{
                if(setButtonClicked){
                    displayMessageBox(title, text)
                    setButtonClicked = false
                    fscamcu135.getExposureCompensation()
                }
            }

            onIndicateSmileThresholdRangeFailure:{
                displayMessageBox(title, text)
                fscamcu135.getSmileDetectMode()
            }
            onNumberOfframesStoreCapacity:{
                fillFrameIndexCombo(numberOfFrames)
            }

            onGrabStillFrameStatus:{
                if(grabStatus){ // If grab still frame command is success, the save the frame in PC
                    root.imageCapture(CommonEnums.STORECAM_RETRIEVE_SHOT)
                }else{ // If grab still frame command is failure, then ignore
                    
                    root.switchToCamFrameSettings(false)
                    fscamcu135.grabPreviewFrame()
                }
            }

            onStoreStillFrameSucceess:{
                fscamcu135.grabPreviewFrame()
            }

            onStillsettingsReceived:{
                fscamcu135.getNumberOfFramesCanStore(stillformat, stillResolution)
                root.changeStillSettings(stillformat, stillResolution)
            }
        // Added by Sankari: Mar 21, 2019. To set number of frames to skip in preview[ex: in FSCAM_CU135]
            onUpdatePreviewFrameToSkip:{
                root.updatePreviewFrameskip(previewSkip)
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
            fscamcu135.setPreviewSkipCount()
            setMasterOnDemandMode()
            getValuesFromCamera()
            fscamcu135.getStillResolution()
            root.enableTimerforGrabPreviewFrame(true)
            root.disablePowerLineFreq()

        }
        Component.onDestruction:{
             // When quit stroage camera usage, set continuous mode
             fscamcu135.setStreamMode(Fscamcu135.STREAM_MASTER_CONTINUOUS)
        }
    }


