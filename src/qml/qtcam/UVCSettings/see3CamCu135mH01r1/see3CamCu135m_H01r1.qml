import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3CamCu135mH01r1 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:268
    height:800

    property bool skipUpdateUIOnExpWindowSize   : false
    property bool skipUpdateLowerLimitSlider    : false
    property bool skipUpdateUpperLimitSlider    : false
    property bool skipUpdateTargetBrightness    : false
    property bool skipUpdateUIOnAntiFlickerMode : false
    property bool skipUpdateGainMode            : false

    property bool skipUpdateGainFromUVC         : false


    property int brightnessMin: 0
    property int brightnessMax: 15

    property int xCoordinate: 0
    property int yCoordinate: 0

    property int gainMin: 1
    property int gainMax: 83

    property int lowerLimitMin: 100
    property int lowerLimitMax: 1000000

    property int upperLimitMin: 100
    property int upperLimitMax: 1000000

    property int flickerCtrl
    property bool setButtonClicked: false


    Action {
        id: firmwareVersion
        onTriggered:
        {
            getFirmwareVersion()
        }
    }

    Action {
        id: triggerGain
        onTriggered: {
            see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.SINGLE_SHOT, 1)
        }
    }

    Action {
        id: triggerExposure
        onTriggered: {
            see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.SINGLE_SHOT_EXPOSURE)
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
    Action {
        id: getStatisticsCall
        onTriggered:
        {
            see3camcu135mH01r1.getStatistics()
        }
    }


    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 1000
        onTriggered: {
            see3camcu135mH01r1.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }
    Timer {
        id: getCameraSettingsTimer
        interval: 500
        onTriggered: {
            see3camcu135mH01r1.getAutoExposureLowerLimit()
            see3camcu135mH01r1.getAutoExposureUpperLimit()
            see3camcu135mH01r1.getGainLimit()
           stop()
        }
    }
    Connections
    {
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
        onMouseRightClicked:{
            if(autoexpManual.enabled && autoexpManual.checked){
               see3camcu135mH01r1.setROIAutoExposure(See3CAM_CU135M_H01R1.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        onVideoResolutionChanged:{
            getCameraSettingsTimer.start()
        }
        onPreviewFPSChanged:{
            getCameraSettingsTimer.start()
        }
        onVideoColorSpaceChanged:{
            getCameraSettingsTimer.start()
            see3camcu135mH01r1.getTargetBrightness()
        }
        onSetExpCompensation:{
            see3camcu135mH01r1.setAutoExposureLowerLimit(lowerLimitTextField.text)
            see3camcu135mH01r1.setAutoExposureUpperLimit(upperLimitTextField.text)
            see3camcu135mH01r1.setGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
        }
        onSendGainValueToHID:{
           gainSlider.value = gainHid
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
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: stream_modes
                    text: "--- Stream Modes ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 2
                spacing: 15
                ExclusiveGroup { id: streamModeGroup }
                RadioButton {
                    id: rdoModeMaster
                    style:  econRadioButtonStyle
                    text:   qsTr("Master")
                    exclusiveGroup: streamModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        setMasterMode()
                    }
                    Keys.onReturnPressed:  {
                         setMasterMode()
                    }
                }
                RadioButton {
                    id: rdoModeTrigger
                    style:  econRadioButtonStyle
                    text: qsTr("Trigger")
                    exclusiveGroup: streamModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        root.checkForTriggerMode(true)
                        setTriggerMode()
                    }
                    Keys.onReturnPressed: {
                        root.checkForTriggerMode(true)
                        setTriggerMode()
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: flash_modes
                    text: "--- Flash Modes ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 2
                spacing: 15
                ExclusiveGroup { id: flashModeGroup }
                RadioButton {
                    id: rdoModeOff
                    style:  econRadioButtonStyle
                    text:   qsTr("OFF")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu135mH01r1.setFlashState(See3CAM_CU135M_H01R1.FLASHMODE_OFF)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu135mH01r1.setFlashState(See3CAM_CU135M_H01R1.FLASHMODE_OFF)
                    }
                }
                RadioButton {
                    id: rdoModeStrobe
                    style:  econRadioButtonStyle
                    text: qsTr("Strobe")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu135mH01r1.setFlashState(See3CAM_CU135M_H01R1.FLASHMODE_STROBE)
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setFlashState(See3CAM_CU135M_H01R1.FLASHMODE_STROBE)
                    }
                }
                RadioButton {
                    id: rdoModeTorch
                    style:  econRadioButtonStyle
                    text: qsTr("Torch")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu135mH01r1.setFlashState(See3CAM_CU135M_H01R1.FLASHMODE_TORCH)
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setFlashState(See3CAM_CU135M_H01R1.FLASHMODE_TORCH)
                    }
                }
            }
            Text {
                id: flipControlText
                text: "--- Flip Control ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Grid{
                columns :2
                spacing: 10
                ExclusiveGroup { id: enableflipCtrlGrp }
                CheckBox {
                    id: flipHorizontal
                    activeFocusOnPress : true
                    text: "Horizontal"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3camcu135mH01r1.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                }
                CheckBox {
                    id: flipVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3camcu135mH01r1.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
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

            ColumnLayout{
                spacing:25
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
                        see3camcu135mH01r1.setROIAutoExposure(See3CAM_CU135M_H01R1.AutoExpFull, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                        autoExpoWinSizeCombo.enabled = false
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setROIAutoExposure(See3CAM_CU135M_H01R1.AutoExpFull, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                        see3camcu135mH01r1.setROIAutoExposure(See3CAM_CU135M_H01R1.AutoExpManual, 0, 0, xCoordinate, yCoordinate, autoExpoWinSizeCombo.currentText);
                        autoExpoWinSizeCombo.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setROIAutoExposure(See3CAM_CU135M_H01R1.AutoExpManual, 0, 0, xCoordinate, yCoordinate, autoExpoWinSizeCombo.currentText);
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
                        see3camcu135mH01r1.setROIAutoExposure(See3CAM_CU135M_H01R1.AutoExpManual, 0, 0, xCoordinate, yCoordinate, autoExpoWinSizeCombo.currentText)
                    }
                    skipUpdateUIOnExpWindowSize = true
                }
            }

            Text {
                id: blackLevelAdj
                text: "--- black Level Adjustment ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            ColumnLayout{
               ExclusiveGroup { id: blackLeverAdjGroup }
               spacing: 25
               RadioButton {
                   id: enableBlackLevel
                   style:  econRadioButtonStyle
                   text: qsTr("Enable")
                   exclusiveGroup: blackLeverAdjGroup
                   activeFocusOnPress: true
                   onClicked: {
                       see3camcu135mH01r1.setBlackLevelAdjustment(See3CAM_CU135M_H01R1.ENABLE)
                   }
                   Keys.onReturnPressed: {
                       see3camcu135mH01r1.setBlackLevelAdjustment(See3CAM_CU135M_H01R1.ENABLE)
                   }
               }
               RadioButton {
                   id: disableBlackLevel
                   style:  econRadioButtonStyle
                   text: qsTr("Disable")
                   exclusiveGroup: blackLeverAdjGroup
                   activeFocusOnPress: true
                   onClicked: {
                       see3camcu135mH01r1.setBlackLevelAdjustment(See3CAM_CU135M_H01R1.DISABLE)
                   }
                   Keys.onReturnPressed: {
                       see3camcu135mH01r1.setBlackLevelAdjustment(See3CAM_CU135M_H01R1.DISABLE)
                   }
               }
           }

            //target brightness
            Text
            {
                id: brightnessText
                text: "--- Target Brightness ---"
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
                    id: targetBrightness
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    onValueChanged:  {
                        brightnessTextField.text = targetBrightness.value
                        if(skipUpdateTargetBrightness){
                            see3camcu135mH01r1.setTargetBrightness(targetBrightness.value)
                        }
                        skipUpdateTargetBrightness = true
                    }
                }
                TextField {
                    id: brightnessTextField
                    text: targetBrightness.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: targetBrightness.minimumValue; top: targetBrightness.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            targetBrightness.value = brightnessTextField.text
                        }
                    }
                }
            }


            Text {
                id: gainMode
                text: "--- Gain Mode ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }
            Grid{
               ExclusiveGroup { id: gainModeGroup }
               spacing: 25
               RadioButton {
                   id: autoGain
                   style:  econRadioButtonStyle
                   text: qsTr("Auto")
                   exclusiveGroup: gainModeGroup
                   activeFocusOnPress: true
                   onClicked: {
                       setAutoGain()
                   }
                   Keys.onReturnPressed: {
                       setAutoGain()
                   }
               }
               RadioButton {
                   id: manualGain
                   style:  econRadioButtonStyle
                   text: qsTr("Manual")
                   exclusiveGroup: gainModeGroup
                   activeFocusOnPress: true
                   onClicked: {
                       setManualGain()
                   }
                   Keys.onReturnPressed: {
                       setManualGain()
                   }
               }
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
                          see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.CONTINUOUS, 1)
                          triggerGainBtn.enabled = false
                          triggerGainBtn.opacity = 0.1
                    }
                      Keys.onReturnPressed: {
                          see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.CONTINUOUS, 1)

                          triggerGainBtn.enabled = false
                          triggerGainBtn.opacity = 0.1
                      }
                  }
            }
            Row{
                spacing:25
                RadioButton {
                    exclusiveGroup: afgroup
                    id: radioOneshot
                    text: "Single Shot"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.SINGLE_SHOT, 1)

                        triggerGainBtn.enabled = true
                        triggerGainBtn.opacity = 1
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.SINGLE_SHOT, 1)

                        triggerGainBtn.enabled = true
                        triggerGainBtn.opacity = 1
                    }
                }
                Button {
                    id: triggerGainBtn
                    activeFocusOnPress : true
                    text: "Trigger"
                    style: econButtonStyle
                    enabled: (radioOneshot.enabled && radioOneshot.checked) ? true : false
                    opacity: (radioOneshot.enabled && radioOneshot.checked) ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 120
                    action: (radioOneshot.enabled && radioOneshot.checked) ? triggerGain : null
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.SINGLE_SHOT, 1)
                    }
                }
            }

            Row
            {
                spacing: 35
                Slider
                {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: gainSlider
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: gainMin
                    maximumValue: gainMax
                    onValueChanged:  {
                        gainTextField.text = gainSlider.value
                        root.getGainValueFromHID(gainSlider.value)
                        if(skipUpdateGainMode){

                            if(manualGain.checked == true)
                            {
                                see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.MANUAL_GAIN,0,gainSlider.value)
                            }
                            else if(autoGain.checked == true)
                            {
                                if(radioContin.checked == true)
                                {
                                    see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.CONTINUOUS, gainSlider.value)
                                }
                                else if(radioOneshot.checked == true)
                                {
                                    see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.SINGLE_SHOT, gainSlider.value)
                                }
                            }
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

            Text {
                id: gainLowerLimit
                text: "--- Gain Lower Limit ---"
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
                    id: gainLowerLimitSlider
                    width: 150
                    stepSize: 25
                    style:econSliderStyle
                    onValueChanged:  {
                        gainLowerLimitTextField.text = gainLowerLimitSlider.value
                        if(skipUpdateLowerLimitSlider){
                            setButtonClicked = true
                            see3camcu135mH01r1.setGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
                        }
                        skipUpdateLowerLimitSlider = true
                    }
                }
                TextField {
                    id: gainLowerLimitTextField
                    text: gainLowerLimitSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: gainLowerLimitSlider.minimumValue; top: gainLowerLimitSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            gainLowerLimitSlider.value = gainLowerLimitTextField.text
                        }
                    }
                }
            }

            Text {
                id: gainUpperLimit
                text: "--- Gain Upper Limit ---"
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
                    id: gainUpperLimitSlider
                    width: 150
                    stepSize: 25
                    style:econSliderStyle
                    onValueChanged:  {
                        gainUpperLimitTextField.text = gainUpperLimitSlider.value
                        if(skipUpdateUpperLimitSlider){
                            setButtonClicked = true
                            see3camcu135mH01r1.setGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
                        }
                        skipUpdateUpperLimitSlider = true
                    }
                }
                TextField {
                    id: gainUpperLimitTextField
                    text: gainLowerLimitSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: gainUpperLimitSlider.minimumValue; top: gainUpperLimitSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            gainUpperLimitSlider.value = gainUpperLimitTextField.text
                        }
                    }
                }
            }

            Text {
                id: exposureMode
                text: "--- Auto Exposure Mode ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            Row{
                spacing:10
                ExclusiveGroup { id: autoExpGroup }
                RadioButton {
                    exclusiveGroup: autoExpGroup
                    id: continousExposure
                    text: "Continuous"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.CONTINIOUS_EXPOSURE)

                        //Enable flicker mode when auto exposure continious mode
                        antiFlickerCombo.enabled = true
                        antiFlickerCombo.opacity = 1
                        frequency.enabled        = true
                        frequency.opacity        = 1

                        triggerExposureBtn.enabled = false
                        triggerExposureBtn.opacity = 0.1
                      }
                      Keys.onReturnPressed: {
                        see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.CONTINIOUS_EXPOSURE)

                        //Enable flicker mode when auto exposure continious mode
                        antiFlickerCombo.enabled = true
                        antiFlickerCombo.opacity = 1
                        frequency.enabled        = true
                        frequency.opacity        = 1

                        triggerExposureBtn.enabled = false
                        triggerExposureBtn.opacity = 0.1
                      }
                  }
            }
            Row{
                spacing:25
                RadioButton {
                    exclusiveGroup: autoExpGroup
                    id: singleShotExposure
                    text: "Single shot"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.SINGLE_SHOT_EXPOSURE)

                        //Disable flicker mode when auto singleShotExposure
                        antiFlickerCombo.enabled = false
                        antiFlickerCombo.opacity = 0.1
                        frequency.enabled        = false
                        frequency.opacity        = 0.1

                        triggerExposureBtn.enabled = true
                        triggerExposureBtn.opacity = 1
                    }
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.SINGLE_SHOT_EXPOSURE)

                        //Disable flicker mode when auto singleShotExposure
                        antiFlickerCombo.enabled = false
                        antiFlickerCombo.opacity = 0.1
                        frequency.enabled        = false
                        frequency.opacity        = 0.1

                        triggerExposureBtn.enabled = true
                        triggerExposureBtn.opacity = 1
                    }
                }
                Button {
                    id: triggerExposureBtn
                    activeFocusOnPress : true
                    text: "Trigger"
                    style: econButtonStyle
                    enabled: (singleShotExposure.enabled && singleShotExposure.checked) ? true : false
                    opacity: (singleShotExposure.enabled && singleShotExposure.checked) ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 120
                    action: (singleShotExposure.enabled && singleShotExposure.checked) ? triggerExposure : null
                    Keys.onReturnPressed: {

                        //Disable flicker mode when auto singleShotExposure
                        antiFlickerCombo.enabled = false
                        antiFlickerCombo.opacity = 0.1
                        frequency.enabled        = false
                        frequency.opacity        = 0.1

                        see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.SINGLE_SHOT_EXPOSURE)
                    }
                }
            }


            Text {
                id: lowerLimitMode
                text: "--- Exposure Lower Limit ---"
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
                        id: lowerLimitModetext
                        text: "value(µs)[100 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        opacity: 1
                    }
                    TextField {
                        id: lowerLimitTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        opacity: 1
                        style: econTextFieldStyle
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: lowerLimitMin; top: lowerLimitMax}
                    }
                    Button {
                        id: lowerLimtSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required limit value by changing the
    value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: true
                        opacity: 1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked: {
                            lowerLimtSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu135mH01r1.setAutoExposureLowerLimit(lowerLimitTextField.text)
                            lowerLimtSetBtn.enabled = true
                        }
                        Keys.onReturnPressed: {
                            lowerLimtSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu135mH01r1.setAutoExposureLowerLimit(lowerLimitTextField.text)
                            lowerLimtSetBtn.enabled = true
                        }
                    }
            }

            Text {
                id: upperLimitMode
                text: "--- Exposure Upper Limit ---"
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
                        id: upperLimitModetext
                        text: "value(µs)[100 - 1000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        opacity: 1
                    }
                    TextField {
                        id: upperLimitTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        opacity: 1
                        style: econTextFieldStyle
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: lowerLimitMin; top: lowerLimitMax}
                    }
                    Button {
                        id: upperLimitSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required limit value by changing the
    value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: true
                        opacity: 1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked: {
                            upperLimitSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu135mH01r1.setAutoExposureUpperLimit(upperLimitTextField.text)
                            upperLimitSetBtn.enabled = true
                        }
                        Keys.onReturnPressed: {
                            upperLimitSetBtn.enabled = false
                            setButtonClicked = true
                            see3camcu135mH01r1.setAutoExposureUpperLimit(upperLimitTextField.text)
                            upperLimitSetBtn.enabled = true
                        }
                    }
            }

            //Anti-Flicker Mode
            Text {
                id: antiFlickerMode
                text: "--- Anti Flicker Mode ---"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                Layout.alignment: Qt.AlignCenter
                opacity: 0.50196078431373
            }

            Text {
                id: frequency
                text: "Frequency :"
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
            }
            ComboBox
            {
                id: antiFlickerCombo
                model: ListModel {
                    ListElement { text: "50 Hz" }
                    ListElement { text: "60 Hz" }
                    ListElement { text: "Disable" }
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

            Row{
                Layout.alignment: Qt.AlignCenter

                Text {
                    id: readStatistics
                    text: "--- Read Statistics ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                }
            }

            Row
            {
                spacing: 35
                Text {
                    id: exposureLabel
                    text: "Exposure value (us)"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                }
                TextField
                {
                    id: exposureStatTextField
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                }
            }

            Row
            {
                spacing: 35
                Text {
                    id: gainLabel
                    text: "Gain value (10^-2x)"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                }
                TextField
                {
                    id: gainStatTextField
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                }
            }

            Row{
                Layout.alignment: Qt.AlignCenter
                Button {
                    id: readStatisticsBtn
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Read Statistics"
                    action: getStatisticsCall
                    style: econButtonStyle
                    Keys.onReturnPressed: {
                        see3camcu135mH01r1.getStatistics()
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

        }
    }
    See3CAM_CU135M_H01R1{
        id:see3camcu135mH01r1
        onStreamModeValue:{
            if(streamMode == See3CAM_CU135M_H01R1.MODE_MASTER){
                rdoModeMaster.checked = true
            }else if(streamMode == See3CAM_CU135M_H01R1.MODE_TRIGGER){
                rdoModeTrigger.checked = true
            }
        }

        onFlashModeValue:{
            if(flashMode == See3CAM_CU135M_H01R1.FLASHMODE_OFF){
                rdoModeOff.checked = true
            }else if(flashMode == See3CAM_CU135M_H01R1.FLASHMODE_STROBE){
                rdoModeStrobe.checked = true
            }else if(flashMode == See3CAM_CU135M_H01R1.FLASHMODE_TORCH){
                rdoModeTorch.checked = true
            }
        }

        onFlipCtrlValue:{
            if(flipValue == See3CAM_CU135M_H01R1.FLIP_HORIZONTAL){
                 flipHorizontal.checked = true
            }else if(flipValue == See3CAM_CU135M_H01R1.FLIP_VERTICAL){
                flipVertical.checked = true
            }else if(flipValue ==See3CAM_CU135M_H01R1.FLIP_BOTH){
               flipHorizontal.checked = true
               flipVertical.checked = true
            }else{
               flipHorizontal.checked = false
               flipVertical.checked = false
            }
        }

        onRoiAutoExpMode:{
            currentROIAutoExposureMode(roiMode, x, y, winSize)
        }

        onGainModeReceived: {
            getCurrentGainMode(gainMode)
        }

        onAutoGainModeRecieved: {
            getAutoGainMode(autoGain)
        }

        onManualGainModeRecieved:{
            gainSlider.value = manualGain
        }

        onSliderMinimumRecieved: {
            gainUpperLimitSlider.minimumValue = sliderMin
            gainLowerLimitSlider.minimumValue = sliderMin
        }

        onSliderMaximumRecieved: {
            gainUpperLimitSlider.maximumValue = sliderMax
            gainLowerLimitSlider.maximumValue = sliderMax
        }

        onLowerGainLimitRecieved:{
            skipUpdateLowerLimitSlider = false
            gainLowerLimitSlider.value = lowerLimit
            skipUpdateLowerLimitSlider = true
        }

        onUpperGainLimitRecieved:{
            skipUpdateUpperLimitSlider = false
            gainUpperLimitSlider.value = upperLimit
            skipUpdateUpperLimitSlider = true
        }

        onBlackLevelAdjustmentChanged: {
            getBlackLevelValue(blacklevel)
        }

        onTargetBrightnessReceived: {
            skipUpdateTargetBrightness = false
            targetBrightness.value = brightness
            skipUpdateTargetBrightness = true
        }
        onBrightnessMinSliderReceived: {
            targetBrightness.minimumValue = sliderMin
        }
        onBrightnessMaxSliderReceived: {
            targetBrightness.maximumValue = sliderMax
        }

        onExposureModeRecieved: {
            getAutoExposureMode(exposureMode)
        }

        onLowerLimitExposure: {
            lowerLimitTextField.text = lowerLimit
        }

        onUpperLimitExposure: {
            upperLimitTextField.text = upperLimit
        }

        onAntiFlickerModeRecieved: {

            if(continousExposure.checked == true)
            {
                antiFlickerCombo.enabled = true
                antiFlickerCombo.opacity = 1

                frequency.enabled        = true
                frequency.opacity        = 1
            }
            else{
                antiFlickerCombo.enabled = false
                antiFlickerCombo.opacity = 0.1

                frequency.enabled        = false
                frequency.opacity        = 0.1
            }
            getAntiFlickerModes(antiFlicker)
        }

        onGainStatisticsRecieved: {
            gainStatTextField.text = gainStatistics
        }

        onExposureStatisticsRecieved: {
            exposureStatTextField.text = exposureStatistics
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
                see3camcu135mH01r1.getAutoExposureLowerLimit()
                see3camcu135mH01r1.getAutoExposureUpperLimit()
                see3camcu135mH01r1.getGainLimit()
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

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function currentROIAutoExposureMode(roiMode, xValue, yValue, winSize){

        xCoordinate = xValue
        yCoordinate = yValue

        switch(roiMode){
        case See3CAM_CU135M_H01R1.AutoExpFull:
            autoexpFull.checked = true
            autoExpoWinSizeCombo.enabled = false
            autoExpoWinSizeCombo.currentIndex = winSize-1
            break
        case See3CAM_CU135M_H01R1.AutoExpManual:
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

    function getBlackLevelValue(currentMode)
    {
        if(currentMode == See3CAM_CU135M_H01R1.ENABLE)
        {
            enableBlackLevel.checked = true
        }
        else if(currentMode == See3CAM_CU135M_H01R1.DISABLE)
        {
            disableBlackLevel.checked = true
        }
    }

    function getAutoExposureMode(exposureMode)
    {
        if(exposureMode == See3CAM_CU135M_H01R1.CONTINIOUS_EXPOSURE)
        {
            continousExposure.checked = true
            triggerExposureBtn.enabled = false
            triggerExposureBtn.opacity = 0.1
        }
        else if(exposureMode == See3CAM_CU135M_H01R1.SINGLE_SHOT_EXPOSURE)
        {
            singleShotExposure.checked = true
            triggerExposureBtn.enabled = true
            triggerExposureBtn.opacity = 1
        }
    }

    function getAntiFlickerModes(antiFlicker)
    {
        switch(antiFlicker)
        {
            case See3CAM_CU135M_H01R1.MODE_50Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 0
                skipUpdateUIOnAntiFlickerMode = true
                break
            case See3CAM_CU135M_H01R1.MODE_60Hz:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 1
                skipUpdateUIOnAntiFlickerMode = true
                break
            case See3CAM_CU135M_H01R1.MODE_DISABLE:
                skipUpdateUIOnAntiFlickerMode = false
                antiFlickerCombo.currentIndex = 2
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }

    function getAutoGainMode(autoGain)
    {
        if(autoGain == See3CAM_CU135M_H01R1.CONTINUOUS)
        {
            radioContin.checked = true
        }
        else if(autoGain == See3CAM_CU135M_H01R1.SINGLE_SHOT)
        {
            radioOneshot.checked = true
        }
    }

    function getCurrentGainMode(currentMode){
        if(currentMode == See3CAM_CU135M_H01R1.AUTO_GAIN)
        {
            autoGain.checked = true

            radioContin.enabled = true
            radioOneshot.opacity = 1
            radioOneshot.enabled = true
            radioOneshot.opacity = 1

            gainSlider.enabled = false
            gainSlider.opacity = 0.1
            gainTextField.enabled = false
            gainTextField.opacity = 0.1

            //To enable gain limit sliders in auto gain mode
            gainUpperLimitSlider.enabled = true
            gainLowerLimitSlider.opacity = 1
            gainLowerLimitTextField.enabled = true
            gainLowerLimitTextField.opacity = 1

            gainUpperLimitSlider.enabled = true
            gainUpperLimitSlider.opacity = 1
            gainUpperLimitTextField.enabled = true
            gainUpperLimitTextField.opacity = 1

            //To Enable readStatistics in auto gain mode
            gainLabel.enabled      = true
            gainStatTextField.enabled = true
            gainLabel.opacity      = 1
            gainStatTextField.opacity = 1
        }
        else if(currentMode == See3CAM_CU135M_H01R1.MANUAL_GAIN)
        {
            manualGain.checked = true

            radioContin.enabled  = false
            radioOneshot.opacity = 0.1
            radioOneshot.enabled = false
            radioOneshot.opacity = 0.1

            gainSlider.enabled    = true
            gainSlider.opacity    = 1
            gainTextField.enabled = true
            gainTextField.opacity = 1

            //To disable gain limit sliders in manual Gain mode
            gainUpperLimitSlider.enabled    = false
            gainLowerLimitSlider.opacity    = 0.1
            gainLowerLimitTextField.enabled = false
            gainLowerLimitTextField.opacity = 0.1

            gainUpperLimitSlider.enabled    = false
            gainUpperLimitSlider.opacity    = 0.1
            gainUpperLimitTextField.enabled = false
            gainUpperLimitTextField.opacity = 0.1

            //Disable readStatistics when Gain is in manual Mode
            gainLabel.enabled         = false
            gainStatTextField.enabled = false
            gainLabel.opacity         = 0.1
            gainStatTextField.opacity = 0.1
        }
    }

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        see3camcu135mH01r1.setToDefault()
        getValuesFromCamera()
    }

    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        see3camcu135mH01r1.setStreamMode(See3CAM_CU135M_H01R1.MODE_TRIGGER)
    }

    function setMasterMode(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.masterEnableForMonochrome()
        see3camcu135mH01r1.setStreamMode(See3CAM_CU135M_H01R1.MODE_MASTER)
    }

    function setAutoGain()
    {
        radioContin.enabled = true
        radioOneshot.opacity = 1
        radioOneshot.enabled = true
        radioOneshot.opacity = 1

        gainSlider.enabled = false
        gainSlider.opacity = 0.1
        gainTextField.enabled = false
        gainTextField.opacity = 0.1

        if(radioContin.checked == true)
        {
            see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.CONTINUOUS, 1)
            triggerGainBtn.enabled = false
            triggerGainBtn.opacity = 0.1
        }
        else if(radioOneshot.checked == true)
        {
            see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.AUTO_GAIN,See3CAM_CU135M_H01R1.SINGLE_SHOT, 1)
            triggerGainBtn.enabled = true
            triggerGainBtn.opacity = 1
        }

        //To Enable readStatistics in auto gain mode
        gainLabel.enabled      = true
        gainStatTextField.enabled = true
        gainLabel.opacity      = 1
        gainStatTextField.opacity = 1

        //To enable gainLimit in Auto Gain mode
        gainUpperLimitSlider.enabled = true
        gainLowerLimitSlider.opacity = 1
        gainLowerLimitTextField.enabled = true
        gainLowerLimitTextField.opacity = 1

        gainUpperLimitSlider.enabled = true
        gainUpperLimitSlider.opacity = 1
        gainUpperLimitTextField.enabled = true
        gainUpperLimitTextField.opacity = 1
    }

    function setManualGain()
    {
        radioContin.enabled = false
        radioOneshot.opacity = 0.1
        radioOneshot.enabled = false
        radioOneshot.opacity = 0.1

        gainSlider.enabled = true
        gainSlider.opacity = 1
        gainTextField.enabled = true
        gainTextField.opacity = 1

        see3camcu135mH01r1.setGainMode(See3CAM_CU135M_H01R1.MANUAL_GAIN, 0 ,gainSlider.value)

        //Disable readStatistics when Gain is in manual Mode
        gainLabel.enabled         = false
        gainStatTextField.enabled = false
        gainLabel.opacity         = 0.1
        gainStatTextField.opacity = 0.1

        //Disable single-shot gain trigger button
        triggerGainBtn.enabled = false
        triggerGainBtn.opacity = 0.1

        //To disable gainLimit in manual Gain mode
        gainUpperLimitSlider.enabled    = false
        gainLowerLimitSlider.opacity    = 0.1
        gainLowerLimitTextField.enabled = false
        gainLowerLimitTextField.opacity = 0.1

        gainUpperLimitSlider.enabled    = false
        gainUpperLimitSlider.opacity    = 0.1
        gainUpperLimitTextField.enabled = false
        gainUpperLimitTextField.opacity = 0.1
    }

    function setAutoExposure()
    {
        lowerLimitSlider.enabled    = false
        lowerLimitSlider.opacity    = 0.1
        lowerLimitTextField.enabled = false
        lowerLimitTextField.opacity = 0.1

        upperLimitSlider.enabled    = false
        upperLimitSlider.opacity    = 0.1
        upperLimitTextField.enabled = false
        upperLimitTextField.opacity = 0.1

        see3camcu135mH01r1.setExposureMode(See3CAM_CU135M_H01R1.AUTO_EXPOSURE)
    }

    function setAntiFlickerMode()
    {
        if(antiFlickerCombo.currentIndex === 0)
            see3camcu135mH01r1.setAntiFlickerMode(See3CAM_CU135M_H01R1.MODE_50Hz)
        else if(antiFlickerCombo.currentIndex === 1)
            see3camcu135mH01r1.setAntiFlickerMode(See3CAM_CU135M_H01R1.MODE_60Hz)
        else if(antiFlickerCombo.currentIndex === 2)
            see3camcu135mH01r1.setAntiFlickerMode(See3CAM_CU135M_H01R1.MODE_DISABLE)
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){

            //Enable Auto exposure modes
            continousExposure.enabled  = true
            continousExposure.opacity  = 1
            singleShotExposure.enabled = true
            singleShotExposure.opacity = 1

            //To enable trigger button when UVC auto exposure is enabled
            if(singleShotExposure.checked)
            {
                triggerExposureBtn.enabled = true
                triggerExposureBtn.opacity = 1

                //Disable flicker mode when auto singleShotExposure
                antiFlickerCombo.enabled = false
                antiFlickerCombo.opacity = 0.1
                frequency.enabled        = false
                frequency.opacity        = 0.1
            }
            else if(continousExposure.checked)
            {
                //Enable flicker mode when auto exposure continious mode
                antiFlickerCombo.enabled = true
                antiFlickerCombo.opacity = 1
                frequency.enabled        = true
                frequency.opacity        = 1
            }

            //Enable readStatistics in auto exposure mode
            exposureLabel.enabled         = true
            exposureStatTextField.enabled = true
            exposureLabel.opacity         = 1
            exposureStatTextField.opacity = 1

            //Enable exposure limit
            lowerLimitModetext.enabled    = true
            lowerLimitModetext.opacity    = 1
            lowerLimitTextField.enabled   = true
            lowerLimitTextField.opacity   = 1
            lowerLimtSetBtn.enabled       = true
            lowerLimtSetBtn.opacity       = 1

            upperLimitModetext.enabled    = true
            upperLimitModetext.opacity    = 1
            upperLimitTextField.enabled   = true
            upperLimitTextField.opacity   = 1
            upperLimitSetBtn.enabled      = true
            upperLimitSetBtn.opacity      = 1


            autoexpManual.enabled = true
            autoexpFull.enabled = true
            if(autoexpManual.checked)
                autoExpoWinSizeCombo.enabled = true
            if(autoexpFull.checked)
                autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 1
            autoexpFull.opacity = 1
        }else{

            //Disable Auto exposure modes in manual mode
            continousExposure.enabled = false
            continousExposure.opacity = 0.1
            singleShotExposure.enabled = false
            singleShotExposure.opacity = 0.1
            triggerExposureBtn.enabled = false
            triggerExposureBtn.opacity = 0.1

            //Disable flicker mode exposure is in manual mode
            antiFlickerCombo.enabled = false
            antiFlickerCombo.opacity = 0.1
            frequency.enabled        = false
            frequency.opacity        = 0.1

            //Disable exposure limit
            lowerLimitModetext.enabled    = false
            lowerLimitModetext.opacity    = 0.1
            lowerLimitTextField.enabled   = false
            lowerLimitTextField.opacity   = 0.1
            lowerLimtSetBtn.enabled       = false
            lowerLimtSetBtn.opacity       = 0.1

            upperLimitModetext.enabled    = false
            upperLimitModetext.opacity    = 0.1
            upperLimitTextField.enabled   = false
            upperLimitTextField.opacity   = 0.1
            upperLimitSetBtn.enabled      = false
            upperLimitSetBtn.opacity      = 0.1

            //Disable readStatistics in manual exposure mode
            exposureLabel.enabled         = false
            exposureStatTextField.enabled = false
            exposureLabel.opacity         = 0.1
            exposureStatTextField.opacity = 0.1

            autoexpManual.enabled          = false
            autoexpFull.enabled            = false
            autoExpoWinSizeCombo.enabled   = false
            autoexpManual.opacity          = 0.1
            autoexpFull.opacity            = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    function getValuesFromCamera(){
        see3camcu135mH01r1.getFlashState()
        see3camcu135mH01r1.getStreamMode()
        see3camcu135mH01r1.getFlipCtrlValue()
        see3camcu135mH01r1.getAutoExpROIModeAndWindowSize()
        see3camcu135mH01r1.getBlackLevelAdjustment()
        see3camcu135mH01r1.getTargetBrightness()
        see3camcu135mH01r1.getGainMode()
        see3camcu135mH01r1.getGainLimit()
        see3camcu135mH01r1.getExposureMode()
        see3camcu135mH01r1.getAutoExposureLowerLimit()
        see3camcu135mH01r1.getAutoExposureUpperLimit()
        see3camcu135mH01r1.getAntiFlickerMode()
        see3camcu135mH01r1.getStatistics()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
