import econ.camera.imx900usbcam 1.0
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0


Item {
    width:268
    height:750

    property int qFactorMin: 0
    property int qFactorMax: 100

    property int sensingFrameExpMin: 15
    property int sensingFrameExpMax: 2000000

    property int levelThresholdMin: 0
    property int levelThresholdMax: 4095

    property int pixelMin: 0
    property int pixelMax: 1601664

    property int enable : 1
    property int disable: 0
    
    property int selfTrigEnable : 0
    property int selfTrigDisable: 1

    property int vidResW : 0
    property int vidResH : 0
    property int hCropPos : 0
    property int vCropPos : 0

    property int xCoord : 0
    property int yCoord : 0

    property bool skipUpdateUIQFactor           : false
    property bool skipUpdateUIOnAntiFlickerMode : false
    property bool skipUpdateUIOnAWBLockStatus   : false
    property bool skipUpdateUIOnAELockStatus    : false
    property bool skipUpdateUIOnAWBMode         : false
    property bool expSingleShotBtnClicked       : false
    property bool gainSingleShotBtnClicked      : false
    property bool skipUpdateBlackLevelMode      : false
    property bool skipUpdateUIOnBurstLength     : false
    property bool skipUpdateUIOnSwitchingDiv    : false
    property bool skipUpdateTargetBrightness    : false
    property bool skipUpdateLowerLimitSlider    : false
    property bool skipUpdateUpperLimitSlider    : false
    property bool skipUpdateGainDiv1Slider      : false
    property bool skipUpdateGainDiv2Slider      : false

    property int minExpInSeconds: 0
    property int maxExpInSeconds: 1
    property int minExpInMilliSeconds: 0
    property int maxExpInMilliSeconds: 999
    property int minExpInMicroSeconds: 0
    property int maxExpInMicroSeconds: 999


    property int minExpCompInSec: 0
    property int maxExpCompInSec: 1
    property int minExpCompInMilliSeconds: 0
    property int maxExpCompInMilliSeconds: 999
    property int minExpCompInMicroSeconds: 0
    property int maxExpCompInMicroSeconds: 999

    property int gainMinVal: 0
    property int gainMaxVal: 24

    /*property int minRoiXCoord: 0
    property int maxRoiXCoord: 255
    property int minRoiYCoord: 0
    property int maxRoiYCoord: 255*/

    property int blackLevelMin: 0
    property int blackLevelMax: 4095

    property int multiExpMin: 0
    property int multiExpMax: 2000000

    property int multiGainMin: 0
    property int multiGainMax: 48

    property int  convertedExposure : 0
    property int  secondInt : 0
    property int  milliSecondInt : 0
    property int  microSecondInt : 0

    property bool uvcAutoExposureSelected: false

    property bool isAutoExpMode : false

    Action {
        id: firmwareVersion
        onTriggered:
        {
            getFirmwareVersion()
        }
    }

    Action {
        id: triggerExposureAction
        onTriggered: {
            imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.SINGLE_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
            root.sendExposureStatusToUVC(true, 0);
        }
    }

    Action {
        id: readStatisticsCall
        onTriggered:
        {
            imx900USBCAM.readStatistics()
        }
    }

    Action {
        id: triggerGainAction
        onTriggered: {
            //imx900USBCAM.setGainMode(IMX900USBCAM.AUTO_GAIN, IMX900USBCAM.SINGLE_GAIN_TRIGGER, gainTextField.text)
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
        id: ispVersion
        onTriggered:
        {
            getISPFirmwareVersion()
        }
    }

    Timer {
        id: getCamValuesTimer
        interval: 1000
        onTriggered: {
            imx900USBCAM.getExposureMode()
            imx900USBCAM.calculateTemperature()
            imx900USBCAM.getExposureCompensation()
            imx900USBCAM.readStatistics()
            stop()
        }
    }

    Timer {
        id: getCameraValuesTimer
        interval: 2000
        onTriggered: {
            imx900USBCAM.getFastAutoExposure()
            imx900USBCAM.getSelfTrigger()
            stop()
        }
    }

    Connections
    {
        target: root
        onTakeScreenShot:
        {
           root.imageCapture(CommonEnums.BURST_SHOT);
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
        onSkipFrameCount:{
            root.setSkipFrameCount(3)
        }

        onMouseRightClicked:{
            if(enableSelfTrigger.enabled && enableSelfTrigger.checked){
                /*if(normalMode.checked == true && withOutput.checked == true){
                    imx900USBCAM.setSelfTrigger(enable, width, height, x, y, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                } else if ((normalMode.checked == true) && (withoutOutput.checked == true)){
                    imx900USBCAM.setSelfTrigger(enable, width, height, x, y, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                } else if((forcedMode.checked == true) && (withOutput.checked == true)){
                    imx900USBCAM.setSelfTrigger(enable, width, height, x, y, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                }
                else if((forcedMode.checked == true) && (withoutOutput.checked == true)){
                    imx900USBCAM.setSelfTrigger(enable, width, height, x, y, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                }*/
                vidResW = width;
                vidResH = height;
                hCropPos = x;
                vCropPos = y;
                imx900USBCAM.setSelfTrigger(selfTrigEnable, vidResW ,vidResH ,hCropPos ,vCropPos, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, 0, 0, gainSlider1.value, gainSlider2.value)
            }
            /*if(roiAutoExpManual.checked && roiAutoExpManual.enabled){
                vidResW = width;
                vidResH = height;
                xCoord = x;
                yCoord = y;
                imx900USBCAM.setROIAutoExposure(IMX900USBCAM.AE_MANUAL_ROI, vidResW, vidResH, xCoord, yCoord, autoExpoWinSizeCombo.currentText)

            }*/

        }

        //Signals getting values from UVC & set its values to the HID controls
        onSendGainValueToHID:{
            gainSlider.value = gainHid
        }

        onExposureComponentsFromUVC:{
            expInSecondsTextField.text  = seconds
            expInMilliSecTextField.text = milliSeconds
            expInMicroSecTextField.text = microSeconds
        }

        onImx900ExposureFromUVC:{
            expInSecondsTextField.text  = seconds
            expInMilliSecTextField.text = milliSeconds
            expInMicroSecTextField.text = microSeconds
        }

        onVideoResolutionChanged :{
            getCameraValuesTimer.start()
        }
        onSendResolution : {
            if(width === 2064 && height === 1552)
            {
                enableFastAutoExp.enabled = true
                enableSelfTrigger.enabled = true
                disableSelfTrigger.enabled = true
                disableFastAutoExp.enabled = true
                enableFastAutoExp.opacity = 1
                enableSelfTrigger.opacity = 1
                disableSelfTrigger.opacity = 1
                disableFastAutoExp.opacity = 1
            }
            else
            {
                enableFastAutoExp.enabled = false
                enableSelfTrigger.enabled = false
                disableSelfTrigger.enabled = false
                disableFastAutoExp.enabled = false
                enableFastAutoExp.opacity = 0.1
                enableSelfTrigger.opacity = 0.1
                disableSelfTrigger.opacity = 0.1
                disableFastAutoExp.opacity = 0.1
            }

            /*if(width === 1024  && height === 768){
                autoExpHDR.enabled = true
                outdoorHdr.enabled = true
                indoorHdr.enabled  = true
                autoExpHDR.opacity = 1
                outdoorHdr.opacity = 1
                indoorHdr.opacity  = 1
            }
            else{
                autoExpHDR.enabled = false
                outdoorHdr.enabled = false
                indoorHdr.enabled  = false
                autoExpHDR.opacity = 0.1
                outdoorHdr.opacity = 0.1
                indoorHdr.opacity  = 0.1
            }*/
        }

        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
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
            height: 4500

            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text {
                    id: camereMode
                    text: "--- Camera Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ColumnLayout{
                   ExclusiveGroup { id: cameraModeGroup }
                   spacing: 10
                   RadioButton {
                       id: master
                       style:  econRadioButtonStyle
                       text: qsTr("Master")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       tooltip: "This is a simple mode of operation for the camera without any external trigger
capability.
After selecting master mode, the application starts video streaming"
                       onClicked: {
                           setCameraControls()
                       }
                       Keys.onReturnPressed: {
                           setCameraControls()
                       }
                   }
                   RadioButton {
                       id: triggerExposure
                       style:  econRadioButtonStyle
                       text: qsTr("Trigger-Exposure")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       tooltip: "In this mode, the sensor integration time is decided by the pulse width input
(low level) to the trigger pin.
For example, If the width of the low-level trigger is 15.6ms, then the exposure
configured in the sensor is 15.6ms.
Refer Product Datasheet for more information."
                       onClicked: {
                           setCameraControls()
                       }
                       Keys.onReturnPressed: {
                           setCameraControls()
                       }
                   }
                  /* RadioButton {
                       id: triggerAcquisition
                       style:  econRadioButtonStyle
                       text: qsTr("Trigger-Acquisition")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       tooltip: "High to low pulse of the trigger pulse will initiate the capture of the image
with the exposure configured in the exposure slider."
                       onClicked: {
                           setCameraControls()
                       }
                       Keys.onReturnPressed: {
                           setCameraControls()
                       }
                   }

                   RadioButton {
                       id: triggerSoftware
                       style:  econRadioButtonStyle
                       text: qsTr("Trigger-Software ")
                       exclusiveGroup: cameraModeGroup
                       activeFocusOnPress: true
                       tooltip: "In this mode, trigger is generated internally."
                       onClicked: {
                           setCameraControls()
                       }
                       Keys.onReturnPressed: {
                           setCameraControls()
                       }
                   } */
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
                    Layout.alignment: Qt.AlignCenter
                    CheckBox {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        style: econCheckBoxStyle
                        tooltip: "This control is used to flip the preview left/right."
                        onClicked:{
                            imx900USBCAM.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        tooltip: "This control is used to flip the preview flip vertically up/down."
                        onClicked:{
                            imx900USBCAM.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)
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
                    ToolButton{
                        tooltip: "Gain – Manual :
This control is used to vary gain value from 0dB to 24dB (analog
gain)"
                        width: 200
                        opacity: 0
                    }
                }
   /*             RowLayout{
                   ExclusiveGroup { id: gainModeGroup }
                   Layout.alignment: Qt.AlignCenter
                   spacing: 40
                   RadioButton {
                       id: autoGain
                       style:  econRadioButtonStyle
                       text: qsTr("Auto")
                       exclusiveGroup: gainModeGroup
                       activeFocusOnPress: true
                       onClicked: {
                           setGainProperties()
                       }
                       Keys.onReturnPressed: {
                           setGainProperties()
                       }
                   }
                   RadioButton {
                       id: manualGain
                       style:  econRadioButtonStyle
                       text: qsTr("Manual")
                       exclusiveGroup: gainModeGroup
                       activeFocusOnPress: true
                       onClicked: {
                           setGainProperties()
                       }
                       Keys.onReturnPressed: {
                           setGainProperties()
                       }
                   }
               }

                Text
                {
                    id: autoGainModeTitle
                    text: "Auto Gain Features :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (autoGain.enabled && autoGain.checked) ? true : false
                    opacity: (autoGain.enabled && autoGain.checked) ? 0.8 : 0.1
                }

                Row{
                    spacing:10
                    ExclusiveGroup { id: autoGainFeatureGroup }
                    RadioButton {
                        exclusiveGroup: autoGainFeatureGroup
                        id: gainContinious
                        text: "Continuous"
                        activeFocusOnPress: true
                        enabled: (autoGain.enabled && autoGain.checked) ? true : false
                        opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "The camera detects the light source and sets the exposure value continuously with respect to change in scenes."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            expSingleShotBtnClicked = false
                            setGainProperties()
                        }
                        Keys.onReturnPressed: {
                            expSingleShotBtnClicked = false
                            setGainProperties()
                        }
                    }
                }

                Row{
                    spacing:25
                    RadioButton {
                        exclusiveGroup: autoGainFeatureGroup
                        id: gainSingleShot
                        text: "Single Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        enabled: (autoGain.enabled && autoGain.checked) ? true : false
                        opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The camera calculates the exposure based on the current scene just once and strict with it until next request."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            if(!gainSingleShotBtnClicked){
                                gainSingleShotBtnClicked = true
                                setGainProperties()
                            }
                        }
                        Keys.onReturnPressed: {
                            if(!gainSingleShotBtnClicked){
                                gainSingleShotBtnClicked = true
                                setGainProperties()
                            }
                        }
                    }
                    Button {
                        id: triggerGainBtn
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (gainSingleShot.enabled && gainSingleShot.checked) ? true : false
                        opacity: (gainSingleShot.enabled && gainSingleShot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        action: (gainSingleShot.enabled && gainSingleShot.checked) ? triggerGainAction : null
                        tooltip: "Allows camera to calculate the exposure value once in Single shot mode"
                        Keys.onReturnPressed: {
                            imx900USBCAM.setGainMode(IMX900USBCAM.AUTO_GAIN, IMX900USBCAM.SINGLE_GAIN_TRIGGER, gainTextField.text)
                        }
                    }
                }*/

                Text
                {
                    id: manualGainText
                    text: "Manual Gain :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    ToolButton{
                        tooltip: "Gain – Manual :
This control is used to vary gain value from 0dB to 24dB (analog
gain)"
                        width: 200
                        opacity: 0
                    }
                    enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                    opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                    //enabled: (manualGain.enabled && manualGain.checked) ? true : false
                    //opacity: (manualGain.enabled && manualGain.checked) ? 0.8 : 0.1
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
                        stepSize: 1
                        style:econSliderStyle
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                        //enabled: (manualGain.enabled && manualGain.checked) ? true : false
                        //opacity: (manualGain.enabled && manualGain.checked) ? 1 : 0.1
                        onValueChanged:  {
                            //Sending HID value to UVC
                            root.getGainValueFromHID(gainSlider.value)

                            gainTextField.text = gainSlider.value
                            imx900USBCAM.setGainMode(IMX900USBCAM.MANUAL_GAIN, 0, gainSlider.value)
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
                        enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                        opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                        //enabled: (manualGain.enabled && manualGain.checked) ? true : false
                        //opacity: (manualGain.enabled && manualGain.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainSlider.minimumValue; top: gainSlider.maximumValue}
                        Text {
                            text: "   db"
                            anchors.top: gainTextField.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            opacity: 1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainSlider.value = gainTextField.text
                            }
                        }
                    }
                }

                /*Text {
                    id: gainLimit
                    text: "--- Gain Limit ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "This Control is used to set the maximum and minimum gain range for the
Auto gain function."
                        width: 200
                        opacity: 0
                    }
                }

                Text
                {
                    id: lowerGainLimit
                    text: "Lower Limit:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (autoGain.enabled && autoGain.checked) ? true : false
                    opacity: (autoGain.enabled && autoGain.checked) ? 0.8 : 0.1
                }

                Row{
                    spacing: 35
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: gainLowerLimitSlider
                        width: 150
                        style:econSliderStyle
                        enabled: (autoGain.enabled && autoGain.checked) ? true : false
                        opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainLowerLimitTextField.text = gainLowerLimitSlider.value
                            if(skipUpdateLowerLimitSlider){
                                imx900USBCAM.setGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
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
                        enabled: (autoGain.enabled && autoGain.checked) ? true : false
                        opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainLowerLimitSlider.minimumValue; top: gainLowerLimitSlider.maximumValue}
                        Text {
                            text: "    db"
                            anchors.top: gainLowerLimitTextField.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            enabled: (autoGain.enabled && autoGain.checked) ? true : false
                            opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainLowerLimitSlider.value = gainLowerLimitTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: upperGainLimit
                    text: "Upper Limit:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (autoGain.enabled && autoGain.checked) ? true : false
                    opacity: (autoGain.enabled && autoGain.checked) ? 0.8 : 0.1
                }

                Row{
                    spacing: 35
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: gainUpperLimitSlider
                        width: 150
                        style:econSliderStyle
                        enabled: (autoGain.enabled && autoGain.checked) ? true : false
                        opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainUpperLimitTextField.text = gainUpperLimitSlider.value
                            if(skipUpdateUpperLimitSlider){
                                imx900USBCAM.setGainLimit(gainLowerLimitSlider.value, gainUpperLimitSlider.value)
                            }
                            skipUpdateUpperLimitSlider = true
                        }
                    }
                    TextField {
                        id: gainUpperLimitTextField
                        text: gainUpperLimitSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (autoGain.enabled && autoGain.checked) ? true : false
                        opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainUpperLimitSlider.minimumValue; top: gainUpperLimitSlider.maximumValue}
                        Text {
                            text: "    db"
                            anchors.top: gainUpperLimitTextField.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            enabled: (autoGain.enabled && autoGain.checked) ? true : false
                            opacity: (autoGain.enabled && autoGain.checked) ? 1 : 0.1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainUpperLimitSlider.value = gainUpperLimitTextField.text
                            }
                        }
                    }
                }*/

                Text {
                    id: exposureMode
                    text: "--- Exposure Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "This Control is used to vary the exposure time of the sensor."
                        width: 200
                        opacity: 0
                    }
                }
                RowLayout{
                   ExclusiveGroup { id: autoExpStatusGroup }
                   Layout.alignment: Qt.AlignCenter
                   spacing: 40
                   RadioButton {
                       id: autoExposure
                       style:  econRadioButtonStyle
                       text: qsTr("Auto")
                       exclusiveGroup: autoExpStatusGroup
                       tooltip: "Exposure – Auto :
    This control enables user to set exposure in automatic mode"
                       activeFocusOnPress: true
                       onClicked: {
                           /*if(!exposureContinious.checked && !exposureSingleShot.checked ){
                               imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.CONTINIOUS_EXP, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                               exposureContinious.checked = true
                           }
                           else if(exposureContinious.checked == true) {
                               imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.CONTINIOUS_EXP, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           }
                           else if(exposureSingleShot.checked == true) {
                               imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.SINGLE_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           }*/
                           imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           root.sendExposureStatusToUVC(true, 0);
                       }
                       Keys.onReturnPressed: {
                           /*if(!exposureContinious.checked && !exposureSingleShot.checked){
                               exposureContinious.checked = true
                               imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.CONTINIOUS_EXP, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           }
                           else if(exposureContinious.checked == true) {
                               imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.CONTINIOUS_EXP, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           }
                           else if(exposureSingleShot.checked == true) {
                               imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.SINGLE_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           }*/
                           imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           root.sendExposureStatusToUVC(true, 0);
                       }
                   }
                   RadioButton {
                       id: manualExposure
                       style:  econRadioButtonStyle
                       text: qsTr("Manual")
                       exclusiveGroup: autoExpStatusGroup
                       activeFocusOnPress: true
                       tooltip: " Exposure – Manual :
    This control enables the user to set the manual exposure value"
                       onClicked: {
                           imx900USBCAM.setExposureMode(IMX900USBCAM.MANUAL_EXPOSURE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           //Sending exposure value from HID to UVC
                           sendConvertedExpToUVC()
                       }
                       Keys.onReturnPressed: {
                           imx900USBCAM.setExposureMode(IMX900USBCAM.MANUAL_EXPOSURE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                           //Sending exposure value from HID to UVC
                           sendConvertedExpToUVC()
                       }
                   }
               }

                /*Text
                {
                    id: autoExpModeTitle
                    text: "Auto Exposure Features :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 0.8 : 0.1
                }

                Row{
                    spacing:10
                    ExclusiveGroup { id: autoExposureFeatureGroup }
                    RadioButton {
                        exclusiveGroup: autoExposureFeatureGroup
                        id: exposureContinious
                        text: "Continuous"
                        activeFocusOnPress: true
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        style: econRadioButtonStyle
                        ToolButton{
                            tooltip: "Auto Exposure – Continuous :
    This control enables the camera to detect the light source and set appropriate
exposure value continuously with respect to change in the scenes."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            expSingleShotBtnClicked = false
                            imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.CONTINIOUS_EXP, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            root.sendExposureStatusToUVC(true, 0);
                        }
                        Keys.onReturnPressed: {
                            expSingleShotBtnClicked = false
                            imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.CONTINIOUS_EXP, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                            root.sendExposureStatusToUVC(true, 0);
                        }
                    }
                }

                Row{
                    spacing:25
                    RadioButton {
                        exclusiveGroup: autoExposureFeatureGroup
                        id: exposureSingleShot
                        text: "Single Shot"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        ToolButton{
                            tooltip: "Auto Exposure – Single Shot:
    This control enables the camera to calculate the exposure based on the current
scene (when the trigger button is pressed) just once and maintain that exposure
until next request."
                            width: 15
                            height: 20
                            opacity: 0
                        }
                        onClicked: {
                            expSingleShotBtnClicked = true
                            root.sendExposureStatusToUVC(true, 0);
                            imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.SINGLE_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)

                        }
                        Keys.onReturnPressed: {
                            expSingleShotBtnClicked = true
                            root.sendExposureStatusToUVC(true, 0);
                             imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.SINGLE_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)

                        }
                    }
                    Button {
                        id: triggerExpBtn
                        activeFocusOnPress : true
                        text: "Trigger"
                        style: econButtonStyle
                        enabled: (exposureSingleShot.enabled && exposureSingleShot.checked) ? true : false
                        opacity: (exposureSingleShot.enabled && exposureSingleShot.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 120
                        action: (exposureSingleShot.enabled && exposureSingleShot.checked) ? triggerExposureAction : null
                        tooltip: "Auto Exposure – Single Shot:
    This control enables the camera to calculate the exposure based on the current
scene (when the trigger button is pressed) just once and maintain that exposure
until next request."
                        Keys.onReturnPressed: {
                            if(expSingleShotBtnClicked){
                                imx900USBCAM.setExposureMode(IMX900USBCAM.AUTO_EXPOSURE, IMX900USBCAM.SINGLE_EXP_TRIGGER, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text)
                                root.sendExposureStatusToUVC(true, 0)
                            }
                        }
                    }
                }*/

                Text
                {
                    id: manualExpTitle
                    text: "Manual Exposure :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                    opacity: (manualExposure.enabled && manualExposure.checked) ? 0.8 : 0.1
                    ToolButton{
                        tooltip: "Exposure – Manual:
This control enables the user to manually enter the integration time of the
sensor. For example, 15.6ms should be entered as 15ms and 600us."
                        width: 15
                        height: 20
                        opacity: 0
                    }
                }

                ColumnLayout{
                    spacing: 0
                    Layout.alignment: Qt.AlignCenter

                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 10
                        TextField
                        {
                            id: expInSecondsTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            ToolButton{
                                tooltip: "Exposure – Manual:
This control enables the user to manually enter the integration time of the
sensor. For example, 15.6ms should be entered as 15ms and 600us."
                                width: 15
                                height: 20
                                opacity: 0
                            }
                            enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                            opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1

                            // Units Name
                            Text {
                                text: "   sec"
                                anchors.top: expInSecondsTextField.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                                opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInSeconds; top: maxExpInSeconds}
                        }


                        TextField
                        {
                            id: expInMilliSecTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            ToolButton{
                                tooltip: "Exposure – Manual:
This control enables the user to manually enter the integration time of the
sensor. For example, 15.6ms should be entered as 15ms and 600us."
                                width: 15
                                height: 20
                                opacity: 0
                            }
                            enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                            opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                            // Units Name
                            Text {
                                text: "   ms"
                                anchors.top: expInMilliSecTextField.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                                opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                            }
                            validator: IntValidator {bottom: minExpInMilliSeconds; top: maxExpInMilliSeconds}
                        }

                        TextField
                        {
                            id: expInMicroSecTextField
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            ToolButton{
                                tooltip: "Exposure – Manual:
This control enables the user to manually enter the integration time of the
sensor. For example, 15.6ms should be entered as 15ms and 600us."
                                width: 15
                                height: 20
                                opacity: 0
                            }
                            enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                            opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1

                            // Unit Name
                            Text {
                                text: "    us"
                                anchors.top: expInMicroSecTextField.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                                opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInMicroSeconds ; top: maxExpInMicroSeconds}
                        }

                        Button {
                            id: manualExpSetBtn
                            activeFocusOnPress : true
                            text: "Set"
                            style: econButtonStyle
                            enabled: (manualExposure.enabled && manualExposure.checked) ? true : false
                            opacity: (manualExposure.enabled && manualExposure.checked) ? 1 : 0.1
                            implicitHeight: 25
                            implicitWidth: 50
                            onClicked: {
                                if(imx900USBCAM.setExposureMode(IMX900USBCAM.MANUAL_EXPOSURE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text) == false){
                                    imx900USBCAM.getExposureMode()
                                }
                                //Sending exposure value from HID to UVC
                                sendConvertedExpToUVC()

                            }
                            Keys.onReturnPressed: {
                                //Sending exposure value from HID to UVC
                                if(imx900USBCAM.setExposureMode(IMX900USBCAM.MANUAL_EXPOSURE, 0, expInSecondsTextField.text, expInMilliSecTextField.text, expInMicroSecTextField.text) == false){
                                    imx900USBCAM.getExposureMode()
                                }
                                sendConvertedExpToUVC()
                            }
                        }
                    }
                }

                /*Text {
                    id: exposureLimits
                    text: "--- Exposure Limits ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373

                    ToolButton{
                        tooltip: "This control is used to set the minimum and maximum exposure value for the auto exposure
function."
                        width: 200
                        opacity: 0
                    }
                }

                Text
                {
                    id: lowerExpLimit
                    text: "Lower Limit :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 0.8 : 0.1
                }

                ColumnLayout{
                    spacing: 1
                    Layout.alignment: Qt.AlignCenter
                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 20
                        TextField
                        {
                            id: lowerLimitSeconds
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "   sec"
                                anchors.top: lowerLimitSeconds.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInSeconds; top: maxExpInSeconds}
                        }

                        TextField
                        {
                            id: lowerLimitMilliSec
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "    ms"
                                anchors.top: lowerLimitMilliSec.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInMilliSeconds; top: maxExpInMilliSeconds}
                        }

                        TextField
                        {
                            id: lowerLimitMicroSec
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "    us"
                                anchors.top: lowerLimitMicroSec.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInMicroSeconds; top: maxExpInMicroSeconds}
                        }
                    }
                }


                Text
                {
                    id: upperExpLimit
                    text: "Upper Limit :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 0.8 : 0.1
                }

                ColumnLayout{
                    spacing: 1
                    Layout.alignment: Qt.AlignCenter

                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 20
                        TextField
                        {
                            id: upperLimitSeconds
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "   sec"
                                anchors.top: upperLimitSeconds.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInSeconds; top: maxExpInSeconds}
                        }

                        TextField
                        {
                            id: upperLimitMilliSec
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "    ms"
                                anchors.top: upperLimitMilliSec.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInMilliSeconds; top: maxExpInMilliSeconds}
                        }

                        TextField
                        {
                            id: upperLimitMicroSec
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "    us"
                                anchors.top: upperLimitMicroSec.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpInMicroSeconds ; top: maxExpInMicroSeconds}
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: limitSetBtn
                        activeFocusOnPress : true
                        text: "Set Limit"
                        style: econButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        onClicked: {
                            imx900USBCAM.setAutoExposureUpperLimit(upperLimitSeconds.text, upperLimitMilliSec.text, upperLimitMicroSec.text)
                            imx900USBCAM.setAutoExposureLowerLimit(lowerLimitSeconds.text, lowerLimitMilliSec.text, lowerLimitMicroSec.text)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setAutoExposureUpperLimit(upperLimitSeconds.text, upperLimitMilliSec.text, upperLimitMicroSec.text)
                            imx900USBCAM.setAutoExposureLowerLimit(lowerLimitSeconds.text, lowerLimitMilliSec.text, lowerLimitMicroSec.text)
                        }
                    }
                }*/

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
                    ToolButton{
                        tooltip: "This control is used to avoid the flicker in the preview."
                        opacity: 0
                        width: 200
                    }
                }
                RowLayout{
                    spacing: 25
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: antiFlickerGroup }
                    RadioButton {
                        id: flicker50hz
                        style:  econRadioButtonStyle
                        text: qsTr("50 Hz")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        tooltip: "Anti-flicker – 50Hz :
50Hz flicker frequency is applied in auto exposure mode"
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        onClicked: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlicker()
                            }
                             skipUpdateUIOnAntiFlickerMode = true
                        }
                        Keys.onReturnPressed: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlicker()
                            }
                             skipUpdateUIOnAntiFlickerMode = true
                        }
                    }
                    RadioButton {
                        id: flicker60Hz
                        style:  econRadioButtonStyle
                        text:   qsTr("60Hz")
                        exclusiveGroup: antiFlickerGroup
                        tooltip: "Anti-flicker – 60Hz :
60Hz flicker frequency is applied in auto exposure mode"
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        activeFocusOnPress: true
                        onClicked: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlicker()
                            }
                             skipUpdateUIOnAntiFlickerMode = true
                        }
                        Keys.onReturnPressed: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlicker()
                            }
                             skipUpdateUIOnAntiFlickerMode = true
                        }
                    }
                    RadioButton {
                        id: flickerDisable
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: antiFlickerGroup
                        tooltip: "Anti-flicker disable :
It disables Anti-flicker mode"
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        activeFocusOnPress: true
                        onClicked: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlicker()
                            }
                             skipUpdateUIOnAntiFlickerMode = true
                        }
                        Keys.onReturnPressed: {
                            if(skipUpdateUIOnAntiFlickerMode){
                                setAntiFlicker()
                            }
                             skipUpdateUIOnAntiFlickerMode = true
                        }
                    }
                }


                /*ComboBox
                {
                    id: antiFlickerCombo
                    model: ListModel
                           {
                                ListElement { text: "50 Hz" }
                                ListElement { text: "60 Hz" }
                                ListElement { text: "DISABLE" }
                           }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    ToolButton{
                        tooltip: "Disable : It disables Anti-Flicker mode

50 Hz   : 50Hz flicker frequency is applied.

60 Hz   : 60Hz flicker frequency is applied."
                        width: 200
                        opacity: 0
                    }
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnAntiFlickerMode){
                            setAntiFlicker()
                        }
                        skipUpdateUIOnAntiFlickerMode = true
                    }
                }*/

                /*Text {
                    id: blackLevelAdj
                    text: "--- Black Level ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Used to manually set the Black level adjustment for the camera.

Note: Changing this value may affect the image quality.

Kindly evaluate enough before changing this value."
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
                                imx900USBCAM.setBlackLevelAdjustment(blackLevelSlider.value)
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
                }*/

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: strobeModes
                        text: "--- Strobe Mode ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373

                    }
                }
                RowLayout {
                    spacing: 40
                    ExclusiveGroup { id: strobeModeGroup }
                    Layout.alignment: Qt.AlignCenter
                    RadioButton {
                        id: strobeOn
                        style:  econRadioButtonStyle
                        text:   qsTr("Strobe ON")
                        exclusiveGroup: strobeModeGroup
                        activeFocusOnPress: true
                        tooltip: "Strobe Mode – ON :
    The transition of high to low and low to high of strobe pulses will directly
indicate the start and stop of the sensor exposure respectively.
Refer product datasheet for more information"

                        onClicked: {
                            imx900USBCAM.setStrobeMode(IMX900USBCAM.STROBE_ON)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setStrobeMode(IMX900USBCAM.STROBE_ON)
                        }
                    }
                    RadioButton {
                        id: strobeOff
                        style:  econRadioButtonStyle
                        text: qsTr("Strobe OFF")
                        exclusiveGroup: strobeModeGroup
                        activeFocusOnPress: true
                        tooltip: "Strobe Mode – OFF :
                            The strobe mode will be disabled"
                        onClicked: {
                            imx900USBCAM.setStrobeMode(IMX900USBCAM.STROBE_OFF)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setStrobeMode(IMX900USBCAM.STROBE_OFF)
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: imgCapText
                        text: "--- Image Burst ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "Image Capture Count:
    The Image capture count can be selected from dropdown and the exact
    number of images will be captured and stored in the location specified by the
    user"
                            width: 200
                            opacity: 0
                        }
                    }
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
                        root.stillBurstLength(burstLengthCombo.currentIndex + 1)
                        if(skipUpdateUIOnBurstLength){
                            imx900USBCAM.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }

                Text
                {
                    id: brightnessTitle
                    text: "--- Target Brightness ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "This control allows the user to set the brightness of the video preview which
should be maintained in auto exposure mode."
                        opacity: 0
                        width: 200
                    }
                }
                Row{
                    spacing: 35
                    Slider {
                        id: targetBrightness
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        onValueChanged:  {
                            targetBrightnessTextField.text = targetBrightness.value
                            if(skipUpdateTargetBrightness){
                                imx900USBCAM.setTargetBrightness(targetBrightness.value)
                            }
                            skipUpdateTargetBrightness = true
                        }
                    }
                    TextField {
                        id: targetBrightnessTextField
                        text: targetBrightness.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: targetBrightness.minimumValue; top: targetBrightness.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                targetBrightness.value = targetBrightnessTextField.text
                            }
                        }
                    }
                }


                Text
                {
                    id: exposureCompensation
                    text: "Exposure Compensation :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    ToolButton{
                        tooltip: "This control lets users adjust the automatically calculated exposure value in
auto exposure mode. For instance, as the scene's lux value decreases,
exposure increases, causing a drop in frame rate. To maintain a constant frame
rate, user can reduce exposure compensation value, with gain automatically
applied over the adjusted exposure compensation value to achieve the target
brightness."
                        opacity: 0
                        width: 200
                    }
                    enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                    opacity: (autoExposure.enabled && autoExposure.checked) ? 0.8 : 0.1
                }

                ColumnLayout{
                    spacing: 1
                    Layout.alignment: Qt.AlignCenter

                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 20
                        TextField
                        {
                            id: exposureCompensationSeconds
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "   sec"
                                anchors.top: exposureCompensationSeconds.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpCompInSec; top: maxExpCompInSec}
                        }

                        TextField
                        {
                            id: exposureCompensationMilliSec
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "    ms"
                                anchors.top: exposureCompensationMilliSec.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpCompInMilliSeconds; top: maxExpCompInMilliSeconds}
                        }

                        TextField
                        {
                            id: exposureCompensationMicroSec
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            // Unit Name
                            Text {
                                text: "    us"
                                anchors.top: exposureCompensationMicroSec.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                opacity: 1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpCompInMicroSeconds ; top: maxExpCompInMicroSeconds}
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: limitSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        style: econButtonStyle
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        onClicked: {
                            if(imx900USBCAM.setExposureCompensation(exposureCompensationSeconds.text, exposureCompensationMilliSec.text, exposureCompensationMicroSec.text) == false){
                                imx900USBCAM.getExposureCompensation()
                            }
                        }
                        Keys.onReturnPressed: {
                            if(imx900USBCAM.setExposureCompensation(exposureCompensationSeconds.text, exposureCompensationMilliSec.text, exposureCompensationMicroSec.text) == false){
                                imx900USBCAM.getExposureCompensation()
                            }
                        }
                    }
                }

                /*Text
                {
                    id: roiAutoExpMode
                    text: "--- Auto Exposure ROI ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                      spacing:60
                      Layout.alignment: Qt.AlignCenter
                      ExclusiveGroup { id: roiExpogroup }
                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: roiAutoExpFull
                          text: "Full"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          tooltip: "Auto Exposure ROI – Full:
    Full-Region based exposure value will be applied to the frame."
                          enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                          opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                          // setROIAutoExposure() args:  mode, videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord, WinSize]
                          // videoresolnWidth, videoresolnHeight, mouseXCord, mouseYCord - these parameters are required only when click in preview]
                          // winSize is required only for manual mode
                          onClicked: {
                              imx900USBCAM.setROIAutoExposure(IMX900USBCAM.AE_FULL_ROI, vidResW ,vidResH , xCoord, yCoord, 0);
                          }
                          Keys.onReturnPressed: {
                              imx900USBCAM.setROIAutoExposure(IMX900USBCAM.AE_FULL_ROI, vidResW ,vidResH , xCoord, yCoord, 0);
                          }
                      }
                      RadioButton {
                          exclusiveGroup: roiExpogroup
                          id: roiAutoExpManual
                          text: "Manual"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          tooltip: "Auto Exposure ROI – Manual
User can select the region of interest to calculate the exposure value for that
particular area. This value will then be applied to the entire frame. The mouse
right click button can be used to select the auto exposure area in the preview."
                          enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                          opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                          onClicked: {
                              imx900USBCAM.setROIAutoExposure(IMX900USBCAM.AE_MANUAL_ROI, vidResW ,vidResH , xCoord, yCoord, autoExpoWinSizeCombo.currentText);
                          }
                          Keys.onReturnPressed: {
                              imx900USBCAM.setROIAutoExposure(IMX900USBCAM.AE_MANUAL_ROI, vidResW ,vidResH , xCoord, yCoord, autoExpoWinSizeCombo.currentText);
                          }
                      }
                }
                Text {
                    id: windowSizeText
                    text: "Window Size :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    ToolButton{
                        tooltip: "Auto Exposure ROI – Window size
    The window size determines the exposure region's dimensions. For a window
size of 8, the entire frame becomes the focus region. If the exposure region
extends beyond or falls short of the frame boundary, the ROI is automatically
clipped."
                        opacity: 0
                        width: 200
                    }
                    opacity: (roiAutoExpManual.enabled && roiAutoExpManual.checked) ? 1 : 0.1
                }
                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (roiAutoExpManual.enabled && roiAutoExpManual.checked) ? true : false
                    opacity: (roiAutoExpManual.enabled && roiAutoExpManual.checked) ? 1 : 0.1
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
                            imx900USBCAM.setROIAutoExposure(IMX900USBCAM.AE_MANUAL_ROI, vidResW ,vidResH, xCoord, yCoord, autoExpoWinSizeCombo.currentText)
                    }
                }*/

                Text {
                    id: readStatistics
                    text: "--- Read Statistics ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ColumnLayout{
                    spacing: 1
                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 5
                        Text {
                            id: expStatisticsText
                            text: "Exposure"
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                        }
                        TextField
                        {
                            id: secondsExpStat
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            readOnly: true

                            ToolButton{
                                tooltip: "Read Statistics – Exposure:
This control displays the current exposure value in auto exposure mode"
                                opacity: 0
                                width: 200
                            }
                            Text {
                                text: "   sec"
                                anchors.top: secondsExpStat.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                                opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpCompInSec; top: maxExpCompInSec}
                        }

                        TextField
                        {
                            id: milliSecExpStat
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            ToolButton{
                                tooltip: "Read Statistics – Exposure:
This control displays the current exposure value in auto exposure mode"
                                opacity: 0
                                width: 200
                            }
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            readOnly: true
                            Text {
                                text: "    ms"
                                anchors.top: milliSecExpStat.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                                opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpCompInMilliSeconds; top: maxExpCompInMilliSeconds}
                        }

                        TextField
                        {
                            id: microSecExpStat
                            font.pixelSize: 10
                            font.family: "Ubuntu"
                            smooth: true
                            horizontalAlignment: TextInput.AlignHCenter
                            style: econTextFieldStyle
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            readOnly: true
                            ToolButton{
                                tooltip: "Read Statistics – Exposure:
This control displays the current exposure value in auto exposure mode"
                                opacity: 0
                                width: 200
                            }
                            Text {
                                text: "    us"
                                anchors.top: microSecExpStat.bottom
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                                opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                                horizontalAlignment: Text.AlignHCenter
                            }
                            validator: IntValidator {bottom: minExpCompInMicroSeconds ; top: maxExpCompInMicroSeconds}
                        }
                    }
                }

                Row
                {
                    spacing: 35
                    Text {
                        id: gainStatisticsText
                        text: "Gain"
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
                        readOnly: true
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        ToolButton{
                            tooltip: "Read Statistics – Gain:
This control displays the current gain value in auto exposure mode"
                            opacity: 0
                            width: 200
                        }
                        enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                        opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                        Text {
                            text: "    db"
                            anchors.top: gainStatTextField.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            enabled: (autoExposure.enabled && autoExposure.checked) ? true : false
                            opacity: (autoExposure.enabled && autoExposure.checked) ? 1 : 0.1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        validator: IntValidator {bottom: gainMinVal; top: gainMaxVal}
                    }
                }
                Row{
                    Layout.alignment: Qt.AlignCenter

                    Button {
                        id: getStatisticsBtn
                        activeFocusOnPress : true
                        text: "Get Statistics"
                        style: econButtonStyle
                        action: readStatisticsCall
                        enabled: (gainStatTextField.enabled || milliSecExpStat.enabled) ? true : false
                        opacity: (gainStatTextField.enabled || milliSecExpStat.enabled) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 90
                        Keys.onReturnPressed: {
                            imx900USBCAM.readStatistics()
                        }
                    }
                }

                Text {
                    id: readTempTitle
                    text: "--- Read Temperature ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Read Sensor Temperature:
This control displays the current sensor temperature value in degrees Celsius."
                        width: 200
                        opacity: 0
                    }
                }
                Row
                {
                    spacing: 10
                    Text {
                        id: readTempText
                        text: "Temperature (°C)"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                    }
                    TextField
                    {
                        id: readTempTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        readOnly: true
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                    }
                    Button {
                        id: readTempBtn
                        activeFocusOnPress : true
                        text: "Calculate"
                        style: econButtonStyle
                        implicitHeight: 20
                        implicitWidth: 60
                        onClicked: {
                            imx900USBCAM.calculateTemperature()
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.calculateTemperature()
                        }
                    }
                }

               /* Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: hdrMode
                        text: "--- HDR Mode ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "To enable HDR, switch to 1024x768 resolution in Y8 Format."
                            width: 200
                            opacity: 0
                        }
                    }
                }
                ColumnLayout {
                    spacing: 15
                    ExclusiveGroup { id: hdrModeGroup }
                    RadioButton {
                        id: autoExpHDR
                        style:  econRadioButtonStyle
                        text:   qsTr("Adaptive Exposure Mode")
                        exclusiveGroup: hdrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setHighDynamicRange(IMX900USBCAM.AUTO_EXP_HDR)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setHighDynamicRange(IMX900USBCAM.AUTO_EXP_HDR)
                        }
                    }
                    RadioButton {
                        id: outdoorHdr
                        style:  econRadioButtonStyle
                        text: qsTr("Day HDR Mode")
                        exclusiveGroup: hdrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setHighDynamicRange(IMX900USBCAM.OUTDOOR_HDR)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setHighDynamicRange(IMX900USBCAM.OUTDOOR_HDR)

                        }
                    }
                    RadioButton {
                        id: indoorHdr
                        style:  econRadioButtonStyle
                        text: qsTr("Night HDR Mode")
                        exclusiveGroup: hdrModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setHighDynamicRange(IMX900USBCAM.INDOOR_HDR)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setHighDynamicRange(IMX900USBCAM.INDOOR_HDR)
                        }
                    }
                }*/

                /*Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: quadShutterMode
                        text: "--- Quad Shutter Mode ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "Before enabling Quad Shutter Control, switch to 1024x768 Resolution.
Refer Product datasheet before enabling this Quad Shutter Control feature."
                            width: 200
                            opacity: 0
                        }
                    }
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: quadShutterModeGroup }
                    RadioButton {
                        id: enableQuad
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: quadShutterModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setQuadShutterControl(enable)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setQuadShutterControl(enable)
                        }
                    }
                    RadioButton {
                        id: disableQuad
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: quadShutterModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setQuadShutterControl(disable)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setQuadShutterControl(disable)
                        }
                    }
                }*/

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: fastAutoExpMode
                        text: "--- Fast Auto Exposure ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "Before enabling fast auto exposure, switch to 2064x1552 Resolution in either
Y12 or Y8 format. Refer Product datasheet before enabling this Fast Auto Exposure feature"
                            width: 200
                            opacity: 0
                        }
                    }
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: fastAutoModeGroup }
                    RadioButton {
                        id: enableFastAutoExp
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: fastAutoModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setFastAutoExposure(enable)
                            imx900USBCAM.getSelfTrigger()
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setFastAutoExposure(enable)
                            imx900USBCAM.getSelfTrigger()
                        }
                    }
                    RadioButton {
                        id: disableFastAutoExp
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: fastAutoModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setFastAutoExposure(disable)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setFastAutoExposure(disable)
                        }
                    }
                }

                /*Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: dualTriggerMode
                        text: "--- Dual Trigger ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "Before enabling Dual Trigger, switch to 2064x1552 Resolution.
Select the exposure switching division from the dropdown, Gain and Gain1 values in their respective slider.
Refer Product datasheet before enabling this Dual Trigger feature"
                            width: 200
                            opacity: 0
                        }
                    }
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: dualTriggerModeGroup }
                    RadioButton {
                        id: enableDualTrigger
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: dualTriggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setDualTrigger(enable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setDualTrigger(enable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                        }
                    }
                    RadioButton {
                        id: disableDualTrigger
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: dualTriggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setDualTrigger(disable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setDualTrigger(disable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                        }
                    }
                }

                Text
                {
                    id: switchingDivision
                    text: "Switching Division :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                    opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 0.8 : 0.1
                }
                ComboBox
                {
                    id: divisionCombo
                    enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                    opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 1 : 0.1
                    model: ListModel
                           {
                                ListElement { text: "1" }
                                ListElement { text: "2" }
                                ListElement { text: "3" }
                                ListElement { text: "4" }
                                ListElement { text: "5" }
                                ListElement { text: "6" }
                                ListElement { text: "7" }
                            }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnSwitchingDiv){
                            imx900USBCAM.setDualTrigger(enable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                        }
                        skipUpdateUIOnSwitchingDiv = true
                    }
                }

                Text
                {
                    id: gainDiv1
                    text: "Gain :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                    opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 0.8 : 0.1
                }
                Row{
                    spacing: 35
                    Slider {
                        id: gainDiv1Slider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                        opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainDiv1SliderTextField.text = gainDiv1Slider.value
                            if(skipUpdateGainDiv1Slider){
                                imx900USBCAM.setDualTrigger(enable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                            }
                            skipUpdateGainDiv1Slider = true
                        }
                    }
                    TextField {
                        id: gainDiv1SliderTextField
                        text: gainDiv1Slider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                        opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainDiv1Slider.minimumValue; top: gainDiv1Slider.maximumValue}
                        Text {
                            text: "    db"
                            anchors.top: gainDiv1SliderTextField.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            opacity: 1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainDiv1Slider.value = gainDiv1SliderTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: gainDiv2
                    text: "Gain 1 :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                    opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 0.8 : 0.1
                }
                Row{
                    spacing: 35
                    Slider {
                        id: gainDiv2Slider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                        opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainDiv2SliderTextField.text = gainDiv2Slider.value
                            if(skipUpdateGainDiv2Slider){
                                imx900USBCAM.setDualTrigger(enable, divisionCombo.currentText, gainDiv1Slider.value, gainDiv2Slider.value)
                            }
                            skipUpdateGainDiv2Slider = true
                        }
                    }
                    TextField {
                        id: gainDiv2SliderTextField
                        text: gainDiv2Slider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableDualTrigger.enabled && enableDualTrigger.checked) ? true : false
                        opacity: (enableDualTrigger.enabled && enableDualTrigger.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainDiv2Slider.minimumValue; top: gainDiv2Slider.maximumValue}
                        Text {
                            text: "    db"
                            anchors.top: gainDiv2SliderTextField.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            opacity: 1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainDiv2Slider.value = gainDiv2SliderTextField.text
                            }
                        }
                    }
                }*/

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: selfTriggerMode
                        text: "--- Self Trigger ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "Before enabling Self Trigger, switch to 2064x1552 Resolution either in Y12 or
Y8. Refer Product datasheet for more information."
                            width: 200
                            opacity: 0
                        }
                    }
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: selfTriggerModeGroup }
                    RadioButton {
                        id: enableSelfTrigger
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: selfTriggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            enableSelfTriggerMode()
                        }
                        Keys.onReturnPressed:  {
                            enableSelfTriggerMode()
                        }
                    }
                    RadioButton {
                        id: disableSelfTrigger
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: selfTriggerModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            disableSelfTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            disableSelfTriggerMode()
                        }
                    }
                }

                Text
                {
                    id: hCroppingSize
                    text: "H Cropping Size :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }
                Row{
                    spacing: 35
                    Slider {
                        id: hCroppingSizeSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onValueChanged:  {
                            hCroppingSizeTextField.text = hCroppingSizeSlider.value
                        }
                    }
                    TextField {
                        id: hCroppingSizeTextField
                        text: hCroppingSizeSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: hCroppingSizeSlider.minimumValue; top: hCroppingSizeSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                hCroppingSizeSlider.value = hCroppingSizeTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: vCroppingSize
                    text: "V Cropping Size :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }
                Row{
                    spacing: 35
                    Slider {
                        id: vCroppingSizeSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onValueChanged:  {
                            vCroppingSizeTextField.text = vCroppingSizeSlider.value
                        }
                    }
                    TextField {
                        id: vCroppingSizeTextField
                        text: vCroppingSizeSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: vCroppingSizeSlider.minimumValue; top: vCroppingSizeSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                vCroppingSizeSlider.value = vCroppingSizeTextField.text
                            }
                        }
                    }
                }

                /*Text
                {
                    id: sensingFrameExpTitle
                    text: "Sensing Frame Exposure :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Row
                {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: sensingFrameExpText
                        text: "value(µs)\n[15 - 2000000]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 80
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                    }
                    TextField
                    {
                        id: sensingFrameExpTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: sensingFrameExpMin; top: sensingFrameExpMax}
                    }
                }*/

                Text
                {
                    id: brightObjTitle
                    text: "Bright Object :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Text
                {
                    id: levelThresholdTitle
                    text: "     Level Threshold :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Row
                {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: levelThresholdText1
                        text: "H Side \n[0 - 4095]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                    }
                    TextField
                    {
                        id: hSideTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: levelThresholdMin; top: levelThresholdMax}
                    }
                    Text
                    {
                        id: levelThresholdText1Unit
                        text: "Pixel Value"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                    }
                }

                Text
                {
                    id: countThresholdTitle
                    text: "     Count Threshold :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Row
                {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: countThresholdText1
                        text: "H Side \n[0 to 1601664]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                    }
                    TextField
                    {
                        id: hSideCountTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: pixelMin; top: pixelMax}
                    }
                    Text
                    {
                        id: countThresholdText1Unit
                        text: "Number of \nPixels"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                    }
                }

                Text
                {
                    id: darkObjTitle
                    text: "Dark Object :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                 }

                 Text
                 {
                    id: levelThresholdTitle1
                    text: "     Level Threshold :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                 }

                  Row
                  {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: levelThresholdText2
                        text: "L Side \n[0 - 4095]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                    }
                    TextField
                    {
                        id: lSideTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: levelThresholdMin; top: levelThresholdMax}
                    }
                    Text
                    {
                        id: levelThresholdText2Unit
                        text: "Pixel Value"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                    }
                }


                Text
                {
                    id: countThresholdTitle1
                    text: "     Count Threshold :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Row
                {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: countThresholdText2
                        text: "L Side \n[0 to 1601664]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                     }
                     TextField
                     {
                        id: lSideCountTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: pixelMin; top: pixelMax}
                     }
                     Text
                     {
                         id: countThresholdText2Unit
                         text: "Number of \nPixels"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         width: 70
                         wrapMode: Text.WordWrap
                         enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                         opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                     }
                 }

                /*Text
                {
                    id: sensingFrame
                    text: "Sensing Frame :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }
                RowLayout {
                    spacing: 20
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: sensingFrameModeGroup }
                    RadioButton {
                        id: withOutput
                        style:  econRadioButtonStyle
                        text:   qsTr("With Output")
                        exclusiveGroup: sensingFrameModeGroup
                        activeFocusOnPress: true
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onClicked: {
//                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed:  {
//                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                    }
                    RadioButton {
                        id: withoutOutput
                        style:  econRadioButtonStyle
                        text: qsTr("Without Output")
                        exclusiveGroup: sensingFrameModeGroup
                        activeFocusOnPress: true
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onClicked: {
//                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed: {
//                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                    }
                }

                Text
                {
                    id: selfTriggerForce
                    text: "Self Trigger Force ON :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: selfTriggerForceModeGroup }
                    RadioButton {
                        id: normalMode
                        style:  econRadioButtonStyle
                        text:   qsTr("Normal")
                        exclusiveGroup: selfTriggerForceModeGroup
                        activeFocusOnPress: true
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onClicked: {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                    }
                    RadioButton {
                        id: forcedMode
                        style:  econRadioButtonStyle
                        text: qsTr("Forced")
                        exclusiveGroup: selfTriggerForceModeGroup
                        activeFocusOnPress: true
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onClicked: {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                    }
                }*/

                Text
                {
                    id: gainText1
                    text: "Sensing Area Gain :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: gainSlider1
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainTextField1.text = gainSlider1.value
                        }
                    }
                    TextField
                    {
                        id: gainTextField1
                        text: gainSlider1.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainSlider1.minimumValue; top: gainSlider1.maximumValue}
                        Text {
                            text: "   db"
                            anchors.top: gainTextField1.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            opacity: 1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainSlider1.value = gainTextField1.text
                            }
                        }
                    }
                }

                Text
                {
                    id: gainText2
                    text: "Capturing Area Gain:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }
                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: gainSlider2
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        style:econSliderStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        onValueChanged:  {
                            gainTextField2.text = gainSlider2.value
                        }
                    }
                    TextField
                    {
                        id: gainTextField2
                        text: gainSlider2.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: gainSlider2.minimumValue; top: gainSlider2.maximumValue}
                        Text {
                            text: "   db"
                            anchors.top: gainTextField2.bottom
                            font.pixelSize: 14
                            font.family: "Ubuntu"
                            color: "#ffffff"
                            smooth: true
                            opacity: 1
                            horizontalAlignment: Text.AlignHCenter
                        }
                        onTextChanged: {
                            if(text.length > 0){
                                gainSlider2.value = gainTextField2.text
                            }
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter

                    Button
                    {
                        id: selfTriggerSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the current self trigger properties by clicking the Set button"
                        style: econButtonStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            imx900USBCAM.setSelfTrigger(selfTrigEnable, vidResW ,vidResH ,hCropPos ,vCropPos, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, 0, 0, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed:
                        {
                            imx900USBCAM.setSelfTrigger(selfTrigEnable, vidResW ,vidResH ,hCropPos ,vCropPos, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, 0, 0, gainSlider1.value, gainSlider2.value)
                        }
                    }
                }

                /*Text
                {
                    id: detectedPixels
                    text: "Detected Pixels :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                    opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.8 : 0.1
                }

                Row
                {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: hSidePixels
                        text: "H Side \n[0 to 1601664]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                    }
                    TextField
                    {
                        id: hSidePixelTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: pixelMin; top: pixelMax}
                    }
                    Button
                    {
                        id: hSidePixelSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed:
                        {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                    }
                }

                Row
                {
                    spacing: 9
                    Layout.alignment: Qt.AlignCenter

                    Text
                    {
                        id: lSidePixel
                        text: "L Side \n[0 to 1601664]"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        width: 70
                        wrapMode: Text.WordWrap
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 0.6 : 0.1
                    }
                    TextField
                    {
                        id: lSidePixelTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 80
                        validator: IntValidator {bottom: pixelMin; top: pixelMax}
                    }
                    Button
                    {
                        id: lSidePixelSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        tooltip: "You can set the required exposure compensation value by changing the value in the text box and click the Set button"
                        style: econButtonStyle
                        enabled: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? true : false
                        opacity: (enableSelfTrigger.enabled && enableSelfTrigger.checked) ? 1 : 0.1
                        implicitHeight: 25
                        implicitWidth: 60
                        onClicked:
                        {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                        Keys.onReturnPressed:
                        {
                            imx900USBCAM.setSelfTrigger(enable, 0, 0, 0, 0, hCroppingSizeSlider.value, vCroppingSizeSlider.value, sensingFrameExpTextField.text, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, withOutput.checked, normalMode.checked, hSidePixelTextField.text, lSidePixelTextField.text, gainSlider1.value, gainSlider2.value)
                        }
                    }
                }*/

                /*Text {
                    id: multiFrameSetTitle
                    text: "--- Multi Frame Set ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Before enabling multi-frame set, switch to 2064x1552 Resolution.
After enabling 2 frameset (or) 4 frame-set mode, provide the required
exposure and gain values in their respective text boxes."
                        width: 200
                        opacity: 0
                    }
                }

                ColumnLayout{
                    spacing: 10
                    ExclusiveGroup { id: multiFrameSetGroup }
                    RadioButton {
                        id: frameSetOff
                        exclusiveGroup: multiFrameSetGroup
                        checked: false
                        text: "Off"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            setMultipleFrameSetValues()
                        }
                        Keys.onReturnPressed: {
                            setMultipleFrameSetValues()
                        }
                    }

                    RadioButton {
                        id: frameSet2
                        exclusiveGroup: multiFrameSetGroup
                        checked: false
                        text: "2-Frame Set"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            setMultipleFrameSetValues()
                        }
                        Keys.onReturnPressed: {
                            setMultipleFrameSetValues()
                        }
                    }

                    RadioButton {
                        id: frameSet4
                        exclusiveGroup: multiFrameSetGroup
                        checked: false
                        text: "4-Frame Set"
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            setMultipleFrameSetValues()
                        }
                        Keys.onReturnPressed: {
                            setMultipleFrameSetValues()
                        }
                    }
                }

                Text {
                    id: frameSetTitle
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ColumnLayout{
                    spacing: 20

                    RowLayout{
                        spacing: 10
                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: gainLabel1
                                text: "Gain 1"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: gainStatTextField1
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   db"
                                    anchors.top: gainStatTextField1.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                                validator: IntValidator {bottom: multiGainMin; top: multiGainMax}
                            }
                        }

                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: exposure1
                                text: "Exposure 1"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: exposureTextField1
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   us"
                                    anchors.top: exposureTextField1.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                                validator: IntValidator {bottom: multiExpMin; top: multiExpMax}
                            }
                        }
                    }
                    RowLayout{
                        spacing: 10
                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: gainLabel2
                                text: "Gain 2"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: gainStatTextField2
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   db"
                                    anchors.top: gainStatTextField2.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                                validator: IntValidator {bottom: multiGainMin; top: multiGainMax}
                            }
                        }

                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: exposure2
                                text: "Exposure 2"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: exposureTextField2
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   us"
                                    anchors.top: exposureTextField2.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? true : false
                                opacity: ((frameSet2.enabled && frameSet2.checked) || (frameSet4.enabled && frameSet4.checked)) ? 1 : 0.1
                                validator: IntValidator {bottom: multiExpMin; top: multiExpMax}
                            }
                        }
                    }
                    RowLayout{
                        spacing: 10
                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: gainLabel3
                                text: "Gain 3"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: gainStatTextField3
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   db"
                                    anchors.top: gainStatTextField3.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                                validator: IntValidator {bottom: multiGainMin; top: multiGainMax}
                            }
                        }

                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: exposure3
                                text: "Exposure 3"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: exposureTextField3
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   us"
                                    anchors.top: exposureTextField3.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                                validator: IntValidator {bottom: multiExpMin; top: multiExpMax}
                            }
                        }

                    }
                    RowLayout{
                        spacing: 10

                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: gainLabel4
                                text: "Gain 4"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: gainStatTextField4
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   db"
                                    anchors.top: gainStatTextField4.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                                validator: IntValidator {bottom: multiGainMin; top: multiGainMax}
                            }
                        }

                        Row
                        {
                            spacing: 10
                            Layout.alignment: Qt.AlignLeft
                            Text {
                                id: exposure4
                                text: "Exposure 4"
                                font.pixelSize: 14
                                font.family: "Ubuntu"
                                color: "#ffffff"
                                smooth: true
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                            }
                            TextField
                            {
                                id: exposureTextField4
                                font.pixelSize: 10
                                font.family: "Ubuntu"
                                smooth: true
                                horizontalAlignment: TextInput.AlignHCenter
                                style: econTextFieldStyle
                                Text {
                                    text: "   us"
                                    anchors.top: exposureTextField4.bottom
                                    font.pixelSize: 14
                                    font.family: "Ubuntu"
                                    color: "#ffffff"
                                    smooth: true
                                    opacity: 1
                                    horizontalAlignment: Text.AlignHCenter
                                }
                                enabled: (frameSet4.enabled && frameSet4.checked) ? true : false
                                opacity: (frameSet4.enabled && frameSet4.checked) ? 1 : 0.1
                                validator: IntValidator {bottom: multiExpMin; top: multiExpMax}
                            }
                        }
                    }

                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        Button {
                            id: multiFrameSetTriggerBtn
                            activeFocusOnPress : true
                            text: "Trigger"
                            style: econButtonStyle
                            enabled: (frameSetOff.checked) ? false : true
                            opacity: (frameSetOff.checked) ? 0.1 : 1
                            implicitHeight: 25
                            implicitWidth: 80
                            onClicked: {
                                setMultipleFrameSetValues()
                            }
                            Keys.onReturnPressed: {
                                setMultipleFrameSetValues()
                            }
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: toneCtrlMode
                        text: "--- Tone Control ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "Refer Product datasheet before enabling this Tone Control feature."
                            width: 200
                            opacity: 0
                        }
                    }
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: toneCtrlModeGroup }
                    RadioButton {
                        id: enableToneCtrl
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: toneCtrlModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setToneControl(disable)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setToneControl(disable)
                        }
                    }
                    RadioButton {
                        id: disableToneCtrl
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: toneCtrlModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setToneControl(enable)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setToneControl(enable)
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: shortExpMode
                        text: "--- Short Exposure ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        ToolButton{
                            tooltip: "This control allows us to use exposure values less than 13us.
Short Exposure range (1-13us)."
                            width: 200
                            opacity: 0
                        }
                    }
                }
                RowLayout {
                    spacing: 45
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: shortExpModeGroup }
                    RadioButton {
                        id: enableShortExp
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: shortExpModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setShortExposureMode(enable, shortExpSlider.value)
                        }
                        Keys.onReturnPressed:  {
                            imx900USBCAM.setShortExposureMode(enable, shortExpSlider.value)
                        }
                    }
                    RadioButton {
                        id: disableShortExp
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: shortExpModeGroup
                        activeFocusOnPress: true
                        onClicked: {
                            imx900USBCAM.setShortExposureMode(disable, shortExpSlider.value)
                        }
                        Keys.onReturnPressed: {
                            imx900USBCAM.setShortExposureMode(disable, shortExpSlider.value)
                        }
                    }
                }

                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: shortExpSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        enabled: (enableShortExp.enabled && enableShortExp.checked) ? true : false
                        opacity: (enableShortExp.enabled && enableShortExp.checked) ? 1 : 0.1
                        onValueChanged:  {
                            shortExpTextField.text = shortExpSlider.value
                            imx900USBCAM.setShortExposureMode(enable, shortExpSlider.value)
                        }
                    }
                    TextField
                    {
                        id: shortExpTextField
                        text: shortExpSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (enableShortExp.enabled && enableShortExp.checked) ? true : false
                        opacity: (enableShortExp.enabled && enableShortExp.checked) ? 1 : 0.1
                        validator: IntValidator {bottom: shortExpSlider.minimumValue; top: shortExpSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                shortExpSlider.value = shortExpTextField.text
                            }
                        }
                    }
                }*/

               Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: defaultValue
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Default"
                        tooltip: "This Default control is used to set all the extension unit controls to their
default values."
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
                        tooltip: "To get the Unique ID that has been assigned to the See3CAM range of
cameras. The Camera serial number will be displayed along with the
firmware version."
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
                       id: ispFirmwareVersion
                       opacity: 1
                       text: "ISP Version"
                       activeFocusOnPress : true
                       tooltip: "Click th epush button to read the ISP firmware version"
                       style: econButtonStyle
                       onClicked: {
                           imx900USBCAM.readISPFirmwareVersion()
                       }
                       Keys.onReturnPressed: {
                           imx900USBCAM.readISPFirmwareVersion()
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
                           imx900USBCAM.saveConfiguration()
                       }
                       Keys.onReturnPressed: {
                           imx900USBCAM.saveConfiguration()
                       }
                   }
               }

            }//Coloumn Layout

         } // Item

    }//ScrollView

   IMX900USBCAM{
     id:imx900USBCAM

     onCurrentCameraMode: {
        getCameraControls(mode)
     }

     onCurrentFlipMode: {
         currentFlipMirrorMode(flipMirrorModeValues)
     }

     onCurrentGainProperties: {
         getGainProperties(mode, feature, min, max, current,step)
     }

     onCurrentBlackLevelValue: {
         //blackLevelSlider.value = value
     }

     onCurrentStrobeMode: {
         if(mode === IMX900USBCAM.STROBE_ON){
            strobeOn.checked = true
         }
         else if(mode  === IMX900USBCAM.STROBE_OFF){
            strobeOff.checked = true
         }
     }

     onCurrentBurstLength: {
         skipUpdateUIOnBurstLength = false
         burstLengthCombo.currentIndex = burstLength - 1
         skipUpdateUIOnBurstLength = true
     }

     onCurrentExposureProperties:{
         currentExposureMode(mode, feature, seconds, milliSeconds, microSeconds)
     }

     onCurrentTargetBrightness:{
         skipUpdateTargetBrightness = false

         targetBrightness.minimumValue = min
         targetBrightness.maximumValue = max
         targetBrightness.stepSize = step
         targetBrightness.value = value

         skipUpdateTargetBrightness = true
     }

     onCurrentGainLimit:{
         /*skipUpdateLowerLimitSlider = false
         skipUpdateUpperLimitSlider = false

         gainLowerLimitSlider.minimumValue = min
         gainLowerLimitSlider.maximumValue = max
         gainLowerLimitSlider.stepSize = step
         gainLowerLimitSlider.value = lowerLimit

         gainUpperLimitSlider.minimumValue = min
         gainUpperLimitSlider.maximumValue = max
         gainUpperLimitSlider.stepSize = step
         gainUpperLimitSlider.value = upperLimit

         skipUpdateLowerLimitSlider = true
         skipUpdateUpperLimitSlider = true*/
     }

     onCurrentExposureLowerLimit: {
        /* lowerLimitSeconds.text  = seconds
         lowerLimitMilliSec.text = milliSeconds
         lowerLimitMicroSec.text = microSeconds*/
     }

     onCurrentExposureUpperLimit: {
         /*upperLimitSeconds.text  = seconds
         upperLimitMilliSec.text = milliSeconds
         upperLimitMicroSec.text = microSeconds*/
     }

     onMultiFrameSetModeReceived:{
         /*if(mode === IMX900USBCAM.FRAME_SET_OFF)
         {
             frameSetOff.checked = true
             frameSetTitle.text = "--- Multi Frame Disabled ---"
         }
         else if(mode === IMX900USBCAM.FRAME_SET_2)
         {
             frameSet2.checked = true
             frameSetTitle.text = "--- 2-Frame Set ---"
         }
         else if(mode === IMX900USBCAM.FRAME_SET_4)
         {
             frameSetTitle.text = "--- 4-Frame Set ---"
             frameSet4.checked = true
         }*/
     }

     onFrameSet1Received:{
         //gainStatTextField1.text = gain1
         //exposureTextField1.text = exposure1
     }
     onFrameSet2Received:{
         /*gainStatTextField2.text = gain2
         exposureTextField2.text = exposure2*/
     }
     onFrameSet3Received:{
         /*gainStatTextField3.text = gain3
         exposureTextField3.text = exposure3*/
     }onFrameSet4Received:{
         /*gainStatTextField4.text = gain4
         exposureTextField4.text = exposure4*/
     }

     onCurrentAntiFlickerMode: {
         getAntiFlickerModes(mode)
     }

     onCurrentTemperature: {
         temperature = parseFloat((temperature).toFixed(2));
         readTempTextField.text = temperature.toFixed(2);
     }

     onCurrentHDRStatus: {
         /*if(hdr === IMX900USBCAM.AUTO_EXP_HDR){
             autoExpHDR.checked = true
         } else if(hdr === IMX900USBCAM.OUTDOOR_HDR){
             outdoorHdr.checked = true
         }
         else if (hdr === IMX900USBCAM.INDOOR_HDR){
             indoorHdr.checked = true
         }*/
     }

     onCurrentQuadShutterControlStatus: {
         /*if(status === enable){
             enableQuad.checked = true
         } else if(status === disable){
             disableQuad.checked = true
         }*/
     }

     onCurrentFastAutoExposureStatus: {
         if(status === enable){
             enableFastAutoExp.checked = true
         } else if(status === disable){
             disableFastAutoExp.checked = true
         }
     }

     onCurrentToneControlStatus: {
         /*if(status === disable){
             enableToneCtrl.checked = true
         } else if(status === enable){
             disableToneCtrl.checked = true
         }*/
     }

     onCurrentShortExposureStatus: {
         /*if(status === enable){
             enableShortExp.checked = true
         } else if(status === disable){
             disableShortExp.checked = true
         }

         shortExpSlider.minimumValue = min
         shortExpSlider.maximumValue = max
         shortExpSlider.value = current*/
     }
     onCurrentStatistics: {
        secondsExpStat.text = seconds
        milliSecExpStat.text = milliSeconds
        microSecExpStat.text = microSeconds

        gainStatTextField.text = gain
     }

     onCurrentDualModeProperties: {
        // getDualTriggerProperties(mode, switchingDivision, min, max, step, min1, max1, step1, gain, gain1)
     }

     onCurrentSelfTriggerMode: {
         if(mode === selfTrigEnable){
             enableSelfTrigger.checked = true
             root.stopUpdatePreviewInTriggerMode()
             root.captureBtnEnable(false)
             root.videoRecordBtnEnable(false)
             root.checkForTriggerMode(true)
             root.clearBufferInTriggerMode(true)
         } else if(mode === selfTrigDisable){
             disableSelfTrigger.checked = true
             root.captureBtnEnable(true)
             root.videoRecordBtnEnable(true)
             root.checkForTriggerMode(false)
             root.startUpdatePreviewInMasterMode()
         }
     }

     onCurrentHCropSize:{
         hCroppingSizeSlider.minimumValue = min
         hCroppingSizeSlider.maximumValue = max
         hCroppingSizeSlider.stepSize = step
         hCroppingSizeSlider.value = current
     }

     onCurrentVCropSize:{
         vCroppingSizeSlider.minimumValue = min
         vCroppingSizeSlider.maximumValue = max
         vCroppingSizeSlider.stepSize = step
         vCroppingSizeSlider.value = current
     }


     onCurrentSensingFrameExposure: {
         //sensingFrameExpTextField.text = current
     }

     onCurrentCountThreshold:{
         hSideCountTextField.text = hCount
         lSideCountTextField.text = lCount
     }

     onCurrentHighThreshold:{
         hSideTextField.text  = hLevel
         lSideTextField.text  = lLevel
     }

     onCurrentSensingFrameMode: {
        /*if(mode === disable){
            withOutput.checked = true
        } else if(mode === enable) {
            withOutput.checked = true
        }*/
     }

     onCurrentSelfTriggerForce: {
         /*if(mode === disable){
             normalMode.checked = true
         } else if(mode === enable) {
             forcedMode.checked = true
         }*/
     }
     onCurrentDetectedPixels: {
         //hSidePixelTextField.text = hPixels
         //lSidePixelTextField.text = vPixels
     }

     onCurrentGainForCapturingFrame: {
         gainSlider2.minimumValue = min
         gainSlider2.maximumValue = max
         gainSlider2.stepSize = step
         gainSlider2.value = gain
     }

     onCurrentGainForSensingFrame: {
         gainSlider1.minimumValue = min
         gainSlider1.maximumValue = max
         gainSlider1.stepSize = step
         gainSlider1.value = gain
     }

     onIndicateGainValueRangeFailure:{
         imx900USBCAM.getGainLimit()
     }

     onIndicateMultiFrameSetInvalidInput:{
         //imx900USBCAM.getMultiFrameSet()
     }

     onIndicateExposureLimitsInvalidInput:{
         //imx900USBCAM.getAutoExposureLowerLimit()
         //imx900USBCAM.getAutoExposureUpperLimit()
     }

     onTitleTextChanged: {
         messageDialog.title = _title.toString()
         messageDialog.text = _text.toString()
         messageDialog.open()
     }

     onIndicateCommandStatus:{
         messageDialog.title = title
         messageDialog.text = text
         messageDialog.open()
     }

     onCurrentExposureCompensation: {
         exposureCompensationSeconds.text  = seconds
         exposureCompensationMilliSec.text = milliSeconds
         exposureCompensationMicroSec.text = microSeconds

     }
     onRoiAutoExpModeValueReceived: {
         /*if(roiMode === IMX900USBCAM.AE_FULL_ROI){
             roiAutoExpFull.checked = true
         }else if(roiMode === IMX900USBCAM.AE_MANUAL_ROI){
             roiAutoExpManual.checked = true
         }
         autoExpoWinSizeCombo.currentIndex = winSize - 1*/
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
        id: econSliderStyle
        SliderStyle {
            groove:Row {
                spacing: 0
                y: 3
                Rectangle {
                    width: styleData.handlePosition
                    height: 4
                    color: "#dc6239"
                    radius: 5
                }
                Rectangle {
                    width: control.width - styleData.handlePosition
                    height: 4
                    color: "#dddddd"
                    radius: 5
                }
            }
            handle: Image {
                source: "images/handle.png"
                opacity: 1
            }
        }
    }

    function getCameraControls(mode){
        if(mode === IMX900USBCAM.MASTER){
            master.checked = true
        } else if(mode === IMX900USBCAM.TRIGGER_EXP){
            triggerExposure.checked = true
        } /*else if(mode === IMX900USBCAM.TRIGGER_ACQ){
            triggerAcquisition.checked = true
        } else if(mode === IMX900USBCAM.TRIGGER_SOFTWARE){
            triggerSoftware.checked = true
        }*/
    }

    function sendConvertedExpToUVC()
    {
        if(expInSecondsTextField.text <= 1)
        {
            secondInt = parseInt(expInSecondsTextField.text);
            milliSecondInt = parseInt(expInMilliSecTextField.text);
            microSecondInt = parseInt(expInMicroSecTextField.text);

            convertedExposure = (secondInt*1000000) + (milliSecondInt*1000) + microSecondInt
            convertedExposure = convertedExposure/100

            root.sendExposureStatusToUVC(false, convertedExposure)
        }
        else{
            root.sendExposureToUVC(10000)
        }
    }

    function disableSelfTriggerMode() {
        imx900USBCAM.setSelfTrigger(selfTrigDisable, vidResW ,vidResH ,hCropPos ,vCropPos, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, 0, 0, gainSlider1.value, gainSlider2.value)

        root.startUpdatePreviewInMasterMode()
        root.checkForTriggerMode(false)
        root.videoRecordBtnEnable(true)
        root.captureBtnEnable(true)
    }

    function enableSelfTriggerMode() {
        imx900USBCAM.setSelfTrigger(selfTrigEnable, vidResW ,vidResH ,hCropPos ,vCropPos, hCroppingSizeSlider.value, vCroppingSizeSlider.value, 0, hSideTextField.text, lSideTextField.text, hSideCountTextField.text, lSideCountTextField.text, 0, 0, 0, 0, gainSlider1.value, gainSlider2.value)
        //imx900USBCAM.getSelfTrigger()
        imx900USBCAM.getFastAutoExposure()

        root.stopUpdatePreviewInTriggerMode()
        root.checkForTriggerMode(true)
        root.clearBufferInTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
    }

    function setCameraControls(){
        if(master.checked == true){
            imx900USBCAM.setCameraMode(IMX900USBCAM.MASTER)
        } else if(triggerExposure.checked == true){
            imx900USBCAM.setCameraMode(IMX900USBCAM.TRIGGER_EXP)
        } /*else if(triggerAcquisition.checked == true) {
            imx900USBCAM.setCameraMode(IMX900USBCAM.TRIGGER_ACQ)
        } else if(triggerSoftware.checked == true) {
            imx900USBCAM.setCameraMode(IMX900USBCAM.TRIGGER_SOFTWARE)
        }*/
    }

    /*function getDualTriggerProperties(mode, switchingDivision, min, max, step, min1, max1, step1, gain, gain1)
    {
        if(mode === disable) {
            disableDualTrigger.checked = true
        } else if (mode === enable) {
            enableDualTrigger.checked = true
        }

        skipUpdateUIOnSwitchingDiv = false
        divisionCombo.currentIndex = switchingDivision - 1
        skipUpdateUIOnSwitchingDiv = true

        skipUpdateGainDiv1Slider = false

        gainDiv1Slider.minimumValue = min
        gainDiv1Slider.maximumValue = max
        gainDiv1Slider.stepSize = step
        gainDiv1Slider.value = gain

        skipUpdateGainDiv1Slider = true

        skipUpdateGainDiv2Slider = false

        gainDiv2Slider.minimumValue = min1
        gainDiv2Slider.maximumValue = max1
        gainDiv2Slider.stepSize = step1
        gainDiv2Slider.value = gain1

        skipUpdateGainDiv2Slider = true
    }*/

    function getAntiFlickerModes(mode) {
        switch(mode) {
            case IMX900USBCAM.MODE_50Hz:
                skipUpdateUIOnAntiFlickerMode = false
                flicker50hz.checked = true
                skipUpdateUIOnAntiFlickerMode = true
                break
            case IMX900USBCAM.MODE_60Hz:
                skipUpdateUIOnAntiFlickerMode = false
                flicker60Hz.checked = true
                skipUpdateUIOnAntiFlickerMode = true
                break
            case IMX900USBCAM.MODE_DISABLE:
                skipUpdateUIOnAntiFlickerMode = false
                flickerDisable.checked = true
                skipUpdateUIOnAntiFlickerMode = true
                break
         }
    }

    function setAntiFlicker() {
        if(flicker50hz.checked)
            imx900USBCAM.setAntiFlickerMode(IMX900USBCAM.MODE_50Hz)
        else if(flicker60Hz.checked)
            imx900USBCAM.setAntiFlickerMode(IMX900USBCAM.MODE_60Hz)
        else if(flickerDisable.checked)
            imx900USBCAM.setAntiFlickerMode(IMX900USBCAM.MODE_DISABLE)
    }

    function currentFlipMirrorMode(mode)
    {
        switch(mode)
        {
            case IMX900USBCAM.NORMAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = false
                break;
            case IMX900USBCAM.VERTICAL:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = false
                break;
            case IMX900USBCAM.HORIZONTAL:
                flipCtrlVertical.checked  = false
                flipCtrlHorizotal.checked = true
                break;
            case IMX900USBCAM.ROTATE_180:
                flipCtrlVertical.checked  = true
                flipCtrlHorizotal.checked = true
                break;
        }
    }

    function getGainProperties(mode, feature, min, max, current, step){

        /*if(mode === IMX900USBCAM.AUTO_GAIN){
            autoGain.checked   = true
        }else if(mode === IMX900USBCAM.MANUAL_GAIN){
            manualGain.checked = true
        }

        //Auto Exposure Features
        if(feature === IMX900USBCAM.CONTINIOUS_GAIN){
            gainContinious.checked   = true
        }
        else if(feature === IMX900USBCAM.SINGLE_GAIN){
            gainSingleShot.checked  = true
        }*/

        gainSlider.minimumValue = min
        gainSlider.maximumValue = max
        gainSlider.stepSize     = step
        gainSlider.value        = current
    }

    function setGainProperties(){
        /*if(autoGain.checked == true){
            if(gainContinious.checked == true) {
                imx900USBCAM.setGainMode(IMX900USBCAM.AUTO_GAIN, IMX900USBCAM.CONTINIOUS_GAIN, gainSlider.value)
            }
            else if(gainSingleShot.checked == true) {
                imx900USBCAM.setGainMode(IMX900USBCAM.AUTO_GAIN, IMX900USBCAM.SINGLE_GAIN, gainSlider.value)
            }
        }else if(manualGain.checked == true){
            imx900USBCAM.setGainMode(IMX900USBCAM.MANUAL_GAIN, 0, gainSlider.value)
        }*/
        imx900USBCAM.setGainMode(IMX900USBCAM.MANUAL_GAIN, 0, gainSlider.value)
    }


    function currentExposureMode(mode, feature, seconds, milliSeconds, microSeconds) {
        if(mode === IMX900USBCAM.AUTO_EXPOSURE){
            autoExposure.checked   = true//Auto Exposure Features
            /*if(feature === IMX900USBCAM.CONTINIOUS_EXP){
                exposureContinious.checked = true
                exposureSingleShot.checked = false
                expSingleShotBtnClicked    = false
            }
            else if(feature === IMX900USBCAM.SINGLE_EXP_TRIGGER){
                exposureSingleShot.checked = true
                exposureContinious.checked = false
                expSingleShotBtnClicked    = true
            }*/
            root.sendExposureStatusToUVC(true, 0);
        }else if(mode === IMX900USBCAM.MANUAL_EXPOSURE){
            manualExposure.checked = true
        }
        //Manual Exposure Values
        expInSecondsTextField.text  = seconds
        expInMilliSecTextField.text = milliSeconds
        expInMicroSecTextField.text = microSeconds

        convertedExposure = (seconds*1000000) + (milliSeconds*1000) + microSeconds
        convertedExposure = convertedExposure/100

        if(mode === IMX900USBCAM.MANUAL_EXPOSURE){
            root.sendExposureStatusToUVC(false, convertedExposure);
         }
    }

    /*function setMultipleFrameSetValues()
    {
        if(frameSetOff.checked == true) {

            frameSetTitle.text = "--- Multi Frame Disabled ---"
            imx900USBCAM.setMultiFrameSet(IMX900USBCAM.FRAME_SET_OFF, gainStatTextField1.text, exposureTextField1.text,  gainStatTextField2.text, exposureTextField2.text, gainStatTextField3.text, exposureTextField3.text , gainStatTextField4.text, exposureTextField4.text )
        } else if (frameSet2.checked == true) {

            frameSetTitle.text = "--- 2-Frame Set ---"
            imx900USBCAM.setMultiFrameSet(IMX900USBCAM.FRAME_SET_2, gainStatTextField1.text, exposureTextField1.text,  gainStatTextField2.text, exposureTextField2.text, gainStatTextField3.text, exposureTextField3.text , gainStatTextField4.text, exposureTextField4.text)
        } else if (frameSet4.checked == true) {

            frameSetTitle.text = "--- 4-Frame Set ---"
            imx900USBCAM.setMultiFrameSet(IMX900USBCAM.FRAME_SET_4, gainStatTextField1.text, exposureTextField1.text,  gainStatTextField2.text, exposureTextField2.text, gainStatTextField3.text, exposureTextField3.text , gainStatTextField4.text, exposureTextField4.text)
        }
    }*/

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            uvcAutoExposureSelected = true
            autoExposure.checked = true
            /*if(autoExposureHold.checked == true){
                displayMessageBox("Message", "AE hold is active")
            }*/
        }else{
            uvcAutoExposureSelected = false
            manualExposure.checked = true
        }
    }


    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        imx900USBCAM.setToDefaultValues()
        getValuesFromCamera()
    }

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function getISPFirmwareVersion(){
        messageDialog.open()
    }

    function getValuesFromCamera(){
        imx900USBCAM.getCameraMode()
        imx900USBCAM.getOrientation()

        imx900USBCAM.getGainMode()
        imx900USBCAM.getGainLimit()

        //imx900USBCAM.getAutoExposureLowerLimit()
        //imx900USBCAM.getAutoExposureUpperLimit()
        imx900USBCAM.getStrobeMode()
        imx900USBCAM.getTargetBrightness()
        imx900USBCAM.getAntiFlickerMode()
        //imx900USBCAM.getBlackLevelAdjustment()
        imx900USBCAM.getBurstLength()

       // imx900USBCAM.getHighDynamicRange()
        //imx900USBCAM.getQuadShutterControl()

        imx900USBCAM.getFastAutoExposure()

        //imx900USBCAM.getDualTrigger()
        imx900USBCAM.getSelfTrigger()
        //imx900USBCAM.getAutoExpROIModeAndWindowSize()

        //imx900USBCAM.getMultiFrameSet()
        //imx900USBCAM.getToneControl()
        //imx900USBCAM.getShortExposureMode()

        getCamValuesTimer.start()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
