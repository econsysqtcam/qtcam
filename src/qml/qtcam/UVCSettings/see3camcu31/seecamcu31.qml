import econ.camera.see3camcu31 1.0
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

    property int convergenceSpeedMin: 0
    property int convergenceSpeedMax: 255

    property int aeMaskIDValue  : 0
    property int awbMaskIDValue : 0

    property bool skipUpdateUIAutoExposure: false
    property bool uvcAutoExposureSelected: false
    property bool skipUpdateUIOnAEMaskID: false
    property bool skipUpdateUIOnAWBMaskID: false

    property int disable : 0
    property int enable  : 1

    Action {
        id: firmwareVersion
        onTriggered:
        {
            getFirmwareVersion()
        }
    }

    Action {
        id: setDefault
        onTriggered:
        {
            setToDefaultValues()
        }
    }

    Timer {
        id: getCamValuesTimer
        interval: 500
        onTriggered: {

            awbMaskIDValue = awbMaskIDCombo.currentText
            aeMaskIDValue  = aeMaskIDCombo.currentText

            see3camcu31.getAEWindowDimensions()
            see3camcu31.getAEMaskDimensions(true, aeMaskIDValue)
            see3camcu31.getAWBWindowDimensions()
            see3camcu31.getAWBMaskDimensions(true, awbMaskIDValue)
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
        onUpdateCrossStillCaptureProperty:{
            see3camcu31.setPropertiesForCrossStill()
        }
        onAutoExposureSelected:{
            enableDisableAutoExposureControls(autoExposureSelect)
        }
        onVideoResolutionChanged:{
            getCamValuesTimer.start()
        }
        onSkipFrameCount:{
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

        Item {
            height: 2500
            ColumnLayout{
                x:2
                y:5
                spacing:20

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
                        tooltip: "Horizonal flip - Flips the preview horizontally left/right"
                        onClicked:{
                            setOrientationProperties()
                        }
                        Keys.onReturnPressed: {
                            setOrientationProperties()
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        style: econCheckBoxStyle
                        tooltip: "Vertical flip - Flips the preview vertically up/down"
                        onClicked:{
                            setOrientationProperties()
                        }
                        Keys.onReturnPressed: {
                            setOrientationProperties()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: rnrModeTitle
                        text: "--- Raw Noise Reduction ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    Layout.alignment: Qt.AlignCenter
                    spacing: 25
                    ExclusiveGroup { id: rnrModeGroup }
                    RadioButton {
                        id: disableRNR
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: rnrModeGroup
                        activeFocusOnPress: true
                        tooltip: "Disables RNR"
                        onClicked: {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_DISABLE)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_DISABLE)
                        }
                    }
                    RadioButton {
                        id: enableRNR
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: rnrModeGroup
                        activeFocusOnPress: true
                        tooltip: "Enabling RAW noise reduction function reduces noise in RAW signals from each
signal line."
                        onClicked: {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_ENABLE)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setRawNoiseReductionStatus(SEE3CAM_CU31.RNR_ENABLE)
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: triggerModeTitle
                        text: "--- Trigger Mode ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                ColumnLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    ExclusiveGroup { id: triggerModeGroup }
                    RadioButton {
                        id: triggerDisable
                        style:  econRadioButtonStyle
                        text:   qsTr("Master")
                        exclusiveGroup: triggerModeGroup
                        activeFocusOnPress: true
                        tooltip: "Master - Disables trigger mode."
                        onClicked: {
                            disableTriggerMode()
                        }
                        Keys.onReturnPressed:{
                            disableTriggerMode()
                        }
                    }
                    RadioButton {
                        id: triggerReadOut
                        style:  econRadioButtonStyle
                        text: qsTr("Readout")
                        exclusiveGroup: triggerModeGroup
                        activeFocusOnPress: true
                        tooltip: "Readout trigger - The sensor starts a read operation based on the input trigger."
                        onClicked: {
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            setTriggerMode()
                        }
                    }
                    RadioButton {
                        id: triggerShutter
                        style:  econRadioButtonStyle
                        text: qsTr("Shutter")
                        exclusiveGroup: triggerModeGroup
                        activeFocusOnPress: true
                        tooltip: "Shutter trigger - The sensor starts a shutter operation based on the input trigger."
                        onClicked: {
                            setTriggerMode()
                        }
                        Keys.onReturnPressed: {
                            setTriggerMode()
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
                }
                Row
                {
                    spacing: 13
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
                        text: "Get"
                        style: econButtonStyle
                        implicitHeight: 20
                        implicitWidth: 45
                        tooltip: "This gives the internal temperature readout from the on-chip temperature sensor in the image
sensor."
                        onClicked: {
                            see3camcu31.readTemperature()
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.readTemperature()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: cameraModeText
                        text: "--- Enhancement Modes ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        Item {
                            visible: !uvcAutoExposureSelected
                            ToolButton {
                                tooltip: "Note: This control is functional only when the exposure mode is configured to Auto."
                                width: 200
                                opacity: 0
                            }
                        }
                    }
                }

                Row{
                    spacing: 20
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: cameraModeGroup }

                    RadioButton {
                        id: autoCameraMode
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "Auto - Auto mode"
                        onClicked: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.AUTO)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.AUTO)
                        }
                    }
                    RadioButton {
                        id: lowLightMode
                        style:  econRadioButtonStyle
                        text: qsTr("Low Light")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "Low Light - This mode enhances lowlight visibility"
                        onClicked: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LOW_LIGHT)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LOW_LIGHT)
                        }
                    }
                    RadioButton {
                        id: ledMode
                        style:  econRadioButtonStyle
                        text: qsTr("LED")
                        exclusiveGroup: cameraModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "LED - This mode enhances visibility of LEDs and reduces the blooming effect."
                        onClicked: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LED)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setCameraMode(SEE3CAM_CU31.LED)
                        }
                    }
                }

                Text {
                    id: autoExpControls
                    text: "AE Controls :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    opacity: 1
                    Item {
                        visible: !uvcAutoExposureSelected
                        ToolButton {
                            tooltip: "Note: The following AE controls are functional only when the exposure mode is configured to Auto."
                            width: 200
                            opacity: 0
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: autoExpStatusTitle
                        text: "--- AE HOLD ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    Layout.alignment: Qt.AlignCenter
                    spacing: 25
                    ExclusiveGroup { id: autoExpStatusGroup }
                    RadioButton {
                        id: autoExposure
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: autoExpStatusGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "Auto - Disables hold, runs Auto Exposure."
                        onClicked: {
                            setAutoExposure()
                        }
                        Keys.onReturnPressed:  {
                            setAutoExposure()
                        }
                    }
                    RadioButton {
                        id: autoExposureHold
                        style:  econRadioButtonStyle
                        text: qsTr("Hold")
                        exclusiveGroup: autoExpStatusGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "Hold - In this mode the sensor retains the current exposure control settings."
                        onClicked: {
                            setAutoExposure()
                        }
                        Keys.onReturnPressed: {
                            setAutoExposure()
                        }
                    }
                }

                Text
                {
                    id: convergenceSpeedTitle
                    text: "--- AE Convergence Speed ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                    ToolButton{
                        tooltip: "Adjust the speed at which auto exposure (AE) converges within the optimum exposure range.
Increasing the value will increase the AE convergence speed."
                        width: 200
                        opacity: 0
                    }
                }
                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: convergenceSpeedSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: convergenceSpeedMin
                        maximumValue: convergenceSpeedMax
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onValueChanged:
                        {
                            convergenceSpeedTextField.text = convergenceSpeedSlider.value
                            if(skipUpdateUIAutoExposure)
                            {
                                see3camcu31.setAEConvergenceSpeed(convergenceSpeedSlider.value)
                            }
                            skipUpdateUIAutoExposure = true
                        }
                    }
                    TextField
                    {
                        id: convergenceSpeedTextField
                        text: convergenceSpeedSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        validator: IntValidator {bottom: convergenceSpeedSlider.minimumValue; top: convergenceSpeedSlider.maximumValue}
                        onTextChanged:
                        {
                            if(text.length > 0)
                            {
                                convergenceSpeedSlider.value = convergenceSpeedTextField.text
                            }
                        }
                    }
                }

                Text
                {
                    id: antiFlickerMode
                    text: "--- Anti-Flicker Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                RowLayout{
                    spacing: 35
                    Layout.alignment: Qt.AlignCenter

                    ExclusiveGroup { id: antiFlickerGroup }
                    RadioButton {
                        id: flickerAuto
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        exclusiveGroup: antiFlickerGroup
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "Auto - Automatically detects the flicker and mitigates the flicker."
                        activeFocusOnPress: true
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                    RadioButton {
                        id: flicker50hz
                        style:  econRadioButtonStyle
                        text: qsTr("50 Hz")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "50 Hz - Mitigates flicker if it detects 50 Hz flicker only. "
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                }

                RowLayout{
                    spacing: 35
                    Layout.alignment: Qt.AlignCenter

                    RadioButton {
                        id: flicker60hz
                        style:  econRadioButtonStyle
                        text: qsTr("60 Hz")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "60 Hz - Mitigates flicker if it detects 60 Hz flicker only. "
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                    RadioButton {
                        id: flickerDisable
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: antiFlickerGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "Disable - Disable flicker mitigation."
                        onClicked: {
                            setFlickerControl()
                        }
                        Keys.onReturnPressed: {
                            setFlickerControl()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: aeWindowOverlay
                        text: "--- Window Overlay ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: aeWindowOverlayGroup }
                    RadioButton {
                        id: disableAEWindow
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: aeWindowOverlayGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "The user can enable or disable the on-screen display of the AE light metering window.
The user can check the position of the AE light metering window by displaying it in the preview.
Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the window.

Disable – Selecting this will disable the AE window overlay."
                        onClicked: {
                            see3camcu31.setAEWindowOverlay(disable)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAEWindowOverlay(disable)
                        }
                    }
                    RadioButton {
                        id: enableAEWindow
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: aeWindowOverlayGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "The user can enable or disable the on-screen display of the AE light metering window.
The user can check the position of the AE light metering window by displaying it in the preview.
Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the window.

Enable – Selecting this will activate the window overlay, allowing the on-screen display of the
light metering window for AE."
                        onClicked: {
                            see3camcu31.setAEWindowOverlay(enable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAEWindowOverlay(enable)
                        }
                    }
                }


               Row{
                   Layout.alignment: Qt.AlignCenter
                   Text {
                       id: aeWindowText
                       text: "--- Window Dimensions ---"
                       font.pixelSize: 14
                       font.family: "Ubuntu"
                       color: "#ffffff"
                       smooth: true
                       opacity: 0.50196078431373
                   }
               }

               RowLayout{

                    spacing: 15
                    Text {
                        id: aeWidth
                        text: "Width "
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The AE light metering window is an area used to calculate the AE evaluation value.
The user can set the size and position of the window.

Width – Set the width of AE light metering window."
                            width: 200
                            opacity: 0
                        }
                    }
                    TextField
                    {
                        id: aeWidthTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                    }

                    Text {
                        id: aeHeight
                        text: "Height"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1

                        ToolButton{
                            tooltip: "The AE light metering window is an area used to calculate the AE evaluation value.
The user can set the size and position of the window.

Height – Set the height of AE light metering window."
                            width: 200
                            opacity: 0
                        }
                    }

                    TextField
                    {
                        id: aeHeightTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                    }
               }

               RowLayout{
                    spacing: 15

                    Text {
                        id: aeXStart
                        text: "XStart "
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The AE light metering window is an area used to calculate the AE evaluation value.
The user can set the size and position of the window.

X Start – Set the starting position of the AE window in horizontal direction."
                            width: 200
                            opacity: 0
                        }
                    }
                    TextField
                    {
                        id: aeXStartTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                    }

                    Text {
                        id: aeYStart
                        text: "YStart"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        ToolButton{
                            tooltip: "The AE light metering window is an area used to calculate the AE evaluation value.
The user can set the size and position of the window.

Y Start – Set the starting position of the AE window in vertical direction."
                            width: 200
                            opacity: 0
                        }
                    }

                    TextField
                    {
                        id: aeYStartTextField
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                    }
               }

               Row
               {
                   Layout.alignment: Qt.AlignCenter
                   Button {
                       id: aeSetBtn
                       activeFocusOnPress : true
                       text: "Set"
                       style: econButtonStyle
                       implicitHeight: 23
                       implicitWidth: 45
                       enabled: (uvcAutoExposureSelected) ? true : false
                       opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                       onClicked: {
                           see3camcu31.setAEWindowDimensions(aeWidthTextField.text, aeHeightTextField.text, aeXStartTextField.text, aeYStartTextField.text)
                       }
                       Keys.onReturnPressed: {
                           see3camcu31.setAEWindowDimensions(aeWidthTextField.text, aeHeightTextField.text, aeXStartTextField.text, aeYStartTextField.text)
                       }
                   }
               }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: aeMaskOverlay
                        text: "--- Mask Overlay ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: aeMaskOverlayGroup }
                    RadioButton {
                        id: disableAEMask
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: aeMaskOverlayGroup
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        activeFocusOnPress: true
                        tooltip: "The user can enable or disable the on-screen display of AE light metering region
masking. The user can check the position of the AE masking region by displaying it on the
preview. Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the masking region.

Disable – Selecting this will disable the AE mask overlay."
                        onClicked: {
                            see3camcu31.setAEMaskOverlay(disable)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAEMaskOverlay(disable)
                        }
                    }
                    RadioButton {
                        id: enableAEMask
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: aeMaskOverlayGroup
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        activeFocusOnPress: true
                        tooltip: "The user can enable or disable the on-screen display of AE light metering region
masking. The user can check the position of the AE masking region by displaying it on the
preview. Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the masking region.

Enable – Selecting this will activate the AE mask overlay, allowing the on-screen display of
masking for AE."
                        onClicked: {
                            see3camcu31.setAEMaskOverlay(enable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAEMaskOverlay(enable)
                        }
                    }
                }

                Text
                {
                    id: aeMaskID
                    text: "--- Mask ID ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ComboBox
                {
                    id: aeMaskIDCombo
                    enabled: (uvcAutoExposureSelected) ? true : false
                    opacity: (uvcAutoExposureSelected) ? 1 : 0.1
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
                        if(skipUpdateUIOnAEMaskID)
                        {

                            aeMaskIDValue  = aeMaskIDCombo.currentText
                            see3camcu31.getAEMaskDimensions(false, aeMaskIDValue)

                        }
                        skipUpdateUIOnAEMaskID = true
                    }

                    ToolButton{
                        tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

Mask ID – Select the ID of the AE mask, for which mask state to be switched and the dimensions
to be set. Any combination of 1 to 5 masks may be applied to the preview concurrently."
                        width: 200
                        opacity: 0
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: aeMaskStatus
                        text: "--- Mask State ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: aeMaskStatusGroup }
                    RadioButton {
                        id: disableAEMaskStatus
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: aeMaskStatusGroup
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        activeFocusOnPress: true
                        tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

Mask State – Change the state of the AE mask for the selected mask ID.

Disable – When selected, no AE masking will be applied to the preview."
                        onClicked: {
                            see3camcu31.setAEMaskDimensions(aeMaskIDCombo.currentText, disable, aeMaskWidthTextField.text, aeMaskHeightTextField.text, aeMaskXStartTextField.text, aeMaskYStartTextField.text)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAEMaskDimensions(aeMaskIDCombo.currentText, disable, aeMaskWidthTextField.text, aeMaskHeightTextField.text, aeMaskXStartTextField.text, aeMaskYStartTextField.text)
                        }
                    }
                    RadioButton {
                        id: enableAEMaskStatus
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: aeMaskStatusGroup
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        activeFocusOnPress: true
                        tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

Mask State – Change the state of the AE mask for the selected mask ID.

Enable – When selected, AE masking will be applied to the preview, excluding that region
from calculation of AE evaluation value."
                        onClicked: {
                            see3camcu31.setAEMaskDimensions(aeMaskIDCombo.currentText, enable, aeMaskWidthTextField.text, aeMaskHeightTextField.text, aeMaskXStartTextField.text, aeMaskYStartTextField.text)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAEMaskDimensions(aeMaskIDCombo.currentText, enable, aeMaskWidthTextField.text, aeMaskHeightTextField.text, aeMaskXStartTextField.text, aeMaskYStartTextField.text)
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: aeMaskWindowText
                        text: "--- Mask Dimensions ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }

                RowLayout{

                     spacing: 15
                     Text {
                         id: aeMaskWidth
                         text: "Width "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1

                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

Width – Set the width of the AE mask for the selected mask ID."
                             width: 200
                             opacity: 0
                         }
                     }
                     TextField
                     {
                         id: aeMaskWidthTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                     }

                     Text {
                         id: aeMaskHeight
                         text: "Height"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1

                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

Height – Set the height of the AE mask for the selected mask ID."
                             width: 200
                             opacity: 0
                         }
                     }

                     TextField
                     {
                         id: aeMaskHeightTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                     }
                }

                RowLayout{
                     spacing: 15

                     Text {
                         id: aeMaskXStart
                         text: "XStart "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

X Start – Set the starting position of the AE mask for the selected mask ID in horizontal
direction."
                             width: 200
                             opacity: 0
                         }
                     }
                     TextField
                     {
                         id: aeMaskXStartTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                     }

                     Text {
                         id: aeMaskYStart
                         text: "YStart"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AE light metering region masking,
which excludes specific region(s) of the window from the calculation of AE evaluation value.

Y Start – Set the starting position of the AE mask for the selected mask ID in vertical
direction."
                             width: 200
                             opacity: 0
                         }
                     }

                     TextField
                     {
                         id: aeMaskYStartTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                         enabled: (uvcAutoExposureSelected) ? true : false
                         opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                     }
                }

                Row
                {
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: aeMaskSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        style: econButtonStyle
                        implicitHeight: 23
                        implicitWidth: 45
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        onClicked: {
                            setAEMaskDimensionsControl()
                        }
                        Keys.onReturnPressed: {
                            setAEMaskDimensionsControl()
                        }
                    }
                }

                //AWB Settings

                Text {
                    id: awbControls
                    text: "AWB Controls :"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignLeft
                    opacity: 1
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbStatusTitle
                        text: "--- AWB HOLD ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    Layout.alignment: Qt.AlignCenter
                    spacing: 25

                    ExclusiveGroup { id: awbStatusGroup }
                    RadioButton {
                        id: awbRelease
                        style:  econRadioButtonStyle
                        text:   qsTr("Auto")
                        tooltip: "Auto - Disables hold, runs Auto white balance."
                        exclusiveGroup: awbStatusGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_RELEASE)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_RELEASE)
                        }
                    }
                    RadioButton {
                        id: awbHold
                        style:  econRadioButtonStyle
                        text: qsTr("Hold")
                        exclusiveGroup: awbStatusGroup
                        activeFocusOnPress: true
                        tooltip: "Hold - In this mode the sensor retains the current white balance settings."
                        onClicked: {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_HOLD)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAWBStatus(SEE3CAM_CU31.AWB_HOLD)
                        }
                    }
                }

                //Light Metering Window Controls

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbWindowOverlay
                        text: "--- Window Overlay ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    x: 23
                    y: 235
                    Layout.alignment: Qt.AlignCenter
                    spacing: 15
                    ExclusiveGroup { id: awbWindowOverlayGroup }
                    RadioButton {
                        id: disableAWBWindow
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: awbWindowOverlayGroup
                        activeFocusOnPress: true
                        tooltip: "The user can enable or disable the on-screen display of the AWB light metering window.
The user can check the position of the AWB light metering window by displaying it in the
preview. Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the window.

Disable – Selecting this will disable the AWB window overlay."
                        onClicked: {
                            see3camcu31.setAWBWindowOverlay(disable)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAWBWindowOverlay(disable)
                        }
                    }
                    RadioButton {
                        id: enableAWBWindow
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: awbWindowOverlayGroup
                        activeFocusOnPress: true
                        tooltip: "The user can enable or disable the on-screen display of the AWB light metering window.
The user can check the position of the AWB light metering window by displaying it in the
preview. Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the window.

Enable – Selecting this will activate the window overlay, allowing the on-screen display of the
light metering window for AWB."
                        onClicked: {
                            see3camcu31.setAWBWindowOverlay(enable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAWBWindowOverlay(enable)
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbWindowText
                        text: "--- Window Dimensions ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }

                RowLayout{
                     spacing: 15
                     Text {
                         id: awbWidth
                         text: "Width "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true

                         ToolButton{
                             tooltip: "The AWB light metering window is an area used to calculate the white balance value.
The user can set the size and position of the window.

Width – Set the width of AWB light metering window."
                             width: 200
                             opacity: 0
                         }
                     }
                     TextField
                     {
                         id: awbWidthTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }

                     Text {
                         id: awbHeight
                         text: "Height"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         ToolButton{
                             tooltip: "The AWB light metering window is an area used to calculate the white balance value.
The user can set the size and position of the window.

Height – Set the height of AWB light metering window."
                             width: 200
                             opacity: 0
                         }
                     }

                     TextField
                     {
                         id: awbHeightTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }
                }

                RowLayout{
                     spacing: 15

                     Text {
                         id: awbXStart
                         text: "XStart "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         ToolButton{
                             tooltip: "The AWB light metering window is an area used to calculate the white balance value.
The user can set the size and position of the window.

X Start – Set the starting position of the AWB window in horizontal direction."
                             width: 200
                             opacity: 0
                         }
                     }
                     TextField
                     {
                         id: awbXStartTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }

                     Text {
                         id: awbYStart
                         text: "YStart"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         ToolButton{
                             tooltip: "The AWB light metering window is an area used to calculate the white balance value.
The user can set the size and position of the window.

Y Start – Set the starting position of the AWB window in vertical direction."
                             width: 200
                             opacity: 0
                         }
                     }

                     TextField
                     {
                         id: awbYStartTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }
                }

                Row
                {
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: awbSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        style: econButtonStyle
                        implicitHeight: 23
                        implicitWidth: 45
                        onClicked: {
                            see3camcu31.setAWBWindowDimensions(awbWidthTextField.text, awbHeightTextField.text, awbXStartTextField.text, awbYStartTextField.text)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAWBWindowDimensions(awbWidthTextField.text, awbHeightTextField.text, awbXStartTextField.text, awbYStartTextField.text)
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbMaskOverlay
                        text: "--- Mask Overlay ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    x: 23
                    y: 235
                    Layout.alignment: Qt.AlignCenter
                    spacing: 15
                    ExclusiveGroup { id: awbMaskOverlayGroup }
                    RadioButton {
                        id: disableAWBMask
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: awbMaskOverlayGroup
                        activeFocusOnPress: true
                        tooltip: "The user can enable or disable the on-screen display of AWB light metering region
masking. The user can check the position of the AWB masking region by displaying it on the
preview. Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the masking region.

Disable – Selecting this will disable the AWB mask overlay."
                        onClicked: {
                            see3camcu31.setAWBMaskOverlay(disable)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAWBMaskOverlay(disable)
                        }
                    }
                    RadioButton {
                        id: enableAWBMask
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: awbMaskOverlayGroup
                        activeFocusOnPress: true
                        tooltip: "The user can enable or disable the on-screen display of AWB light metering region
masking. The user can check the position of the AWB masking region by displaying it on the
preview. Since enabling this feature will be applied to both the preview and still capture, it is
recommended to disable it after configuring the masking region.

Enable – Selecting this will activate the AWB mask overlay, allowing the on-screen display of
masking for AWB."
                        onClicked: {
                            see3camcu31.setAWBMaskOverlay(enable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAWBMaskOverlay(enable)
                        }
                    }
                }


                Text
                {
                    id: awbMaskID
                    text: "--- Mask ID ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ComboBox
                {
                    id: awbMaskIDCombo
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
                        if(skipUpdateUIOnAWBMaskID)
                        {

                            awbMaskIDValue = awbMaskIDCombo.currentText
                            see3camcu31.getAWBMaskDimensions(false, awbMaskIDValue)
                        }
                        skipUpdateUIOnAWBMaskID = true
                    }
                    ToolButton{
                        tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

Mask ID – Select the ID of the AWB mask, for which mask state to be switched and the
dimensions to be set. Any combination of 1 to 5 masks may be applied to the preview
concurrently."
                        width: 200
                        opacity: 0
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbMaskStatus
                        text: "--- Mask State ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }
                RowLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    Layout.alignment: Qt.AlignCenter
                    ExclusiveGroup { id: awbMaskStatusGroup }
                    RadioButton {
                        id: disableAWBMaskStatus
                        style:  econRadioButtonStyle
                        text:   qsTr("Disable")
                        exclusiveGroup: awbMaskStatusGroup
                        activeFocusOnPress: true
                        tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

Mask State - Change the state of the AWB mask for the selected mask ID.

Disable – When selected, no AWB masking will be applied to the preview."
                        onClicked: {
                            see3camcu31.setAWBMaskDimensions(awbMaskIDCombo.currentText, disable, awbMaskWidthTextField.text, awbMaskHeightTextField.text, awbMaskXStartTextField.text, awbMaskYStartTextField.text)
                        }
                        Keys.onReturnPressed:  {
                            see3camcu31.setAWBMaskDimensions(awbMaskIDCombo.currentText, disable, awbMaskWidthTextField.text, awbMaskHeightTextField.text, awbMaskXStartTextField.text, awbMaskYStartTextField.text)
                        }
                    }
                    RadioButton {
                        id: enableAWBMaskStatus
                        style:  econRadioButtonStyle
                        text: qsTr("Enable")
                        exclusiveGroup: awbMaskStatusGroup
                        activeFocusOnPress: true
                        tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

Mask State - Change the state of the AWB mask for the selected mask ID.

Enable – When selected, AWB masking will be applied to the preview, excluding that
region from calculation of white balance value."
                        onClicked: {
                            see3camcu31.setAWBMaskDimensions(awbMaskIDCombo.currentText, enable, awbMaskWidthTextField.text, awbMaskHeightTextField.text, awbMaskXStartTextField.text, awbMaskYStartTextField.text)
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setAWBMaskDimensions(awbMaskIDCombo.currentText, enable, awbMaskWidthTextField.text, awbMaskHeightTextField.text, awbMaskXStartTextField.text, awbMaskYStartTextField.text)
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: awbMaskWindowText
                        text: "--- Mask Dimensions ---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                    }
                }

                RowLayout{

                     spacing: 15
                     Text {
                         id: awbMaskWidth
                         text: "Width "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true

                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

Width – Set the width of the AWB mask for the selected mask ID."
                             width: 200
                             opacity: 0
                         }
                     }
                     TextField
                     {
                         id: awbMaskWidthTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }

                     Text {
                         id: awbMaskHeight
                         text: "Height"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

Height – Set the height of the AWB mask for the selected mask ID."
                             width: 200
                             opacity: 0
                         }
                     }

                     TextField
                     {
                         id: awbMaskHeightTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }
                }

                RowLayout{
                     spacing: 15

                     Text {
                         id: awbMaskXStart
                         text: "XStart "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

X Start – Set the starting position of the AWB mask for the selected mask ID in horizontal
direction."
                             width: 200
                             opacity: 0
                         }
                     }
                     TextField
                     {
                         id: awbMaskXStartTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }

                     Text {
                         id: awbMaskYStart
                         text: "YStart"
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true

                         ToolButton{
                             tooltip: "The user can adjust the size and the position of the AWB light metering region masking,
which excludes specific region(s) of the window from the calculation of white balance value.

Y Start – Set the starting position of the AWB mask for the selected mask ID in vertical
direction."
                             width: 200
                             opacity: 0
                         }
                     }

                     TextField
                     {
                         id: awbMaskYStartTextField
                         font.pixelSize: 10
                         font.family: "Ubuntu"
                         smooth: true
                         horizontalAlignment: TextInput.AlignHCenter
                         style: econTextFieldStyle
                     }
                }

                Row
                {
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: awbMaskSetBtn
                        activeFocusOnPress : true
                        text: "Set"
                        style: econButtonStyle
                        implicitHeight: 23
                        implicitWidth: 45
                        onClicked: {
                            setAWBMaskDimensionsControl()
                        }
                        Keys.onReturnPressed: {
                            setAWBMaskDimensionsControl()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Text {
                        id: presetTitle
                        text: "--- User Preset---"
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        opacity: 0.50196078431373
                        Item {
                            visible: !uvcAutoExposureSelected
                            ToolButton {
                                tooltip: "The User Preset block allows user to customize controls according to their preferences
and save them for subsequent use.

Note: This control is functional only when the exposure mode is configured to Auto."
                                width: 200
                                opacity: 0
                            }
                        }
                    }
                }
                ColumnLayout {
                    x: 23
                    y: 235
                    spacing: 15
                    ExclusiveGroup { id: presetModeGroup }

                    RadioButton {
                        id: userPreset1
                        style:  econRadioButtonStyle
                        text: qsTr("User Preset 1")
                        exclusiveGroup: presetModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "The User Preset block allows user to customize controls according to their preferences
and save them for subsequent use.

User Preset 1 – Selecting this will activate the user-defined settings that are saved to User
Preset 1"
                        onClicked: {
                            see3camcu31.setPresetMode(SEE3CAM_CU31.USER_PRESET_1, SEE3CAM_CU31.PRESET_SELECT)
                            getValuesFromCamera()
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setPresetMode(SEE3CAM_CU31.USER_PRESET_1, SEE3CAM_CU31.PRESET_SELECT)
                            getValuesFromCamera()
                        }
                    }

                    RadioButton {
                        id: userPreset2
                        style:  econRadioButtonStyle
                        text: qsTr("User Preset 2")
                        exclusiveGroup: presetModeGroup
                        activeFocusOnPress: true
                        enabled: (uvcAutoExposureSelected) ? true : false
                        opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                        tooltip: "The User Preset block allows user to customize controls according to their preferences
and save them for subsequent use.

User Preset 2 – Selecting this will activate the user-defined settings that are saved to User
Preset 2"
                        onClicked: {
                            see3camcu31.setPresetMode(SEE3CAM_CU31.USER_PRESET_2, SEE3CAM_CU31.PRESET_SELECT)
                            getValuesFromCamera()
                        }
                        Keys.onReturnPressed: {
                            see3camcu31.setPresetMode(SEE3CAM_CU31.USER_PRESET_2, SEE3CAM_CU31.PRESET_SELECT)
                            getValuesFromCamera()
                        }
                    }

                    Row
                    {
                        Layout.alignment: Qt.AlignCenter
                        spacing: 30
                        RadioButton {
                            id: manualPreset
                            style:  econRadioButtonStyle
                            text:   qsTr("Manual")
                            exclusiveGroup: presetModeGroup
                            activeFocusOnPress: true
                            enabled: (uvcAutoExposureSelected) ? true : false
                            opacity: (uvcAutoExposureSelected) ? 1 : 0.1
                            tooltip: "The User Preset block allows user to customize controls according to their preferences
and save them for subsequent use.

Manual – The current settings will be applied."
                            onClicked: {
                                see3camcu31.setPresetMode(SEE3CAM_CU31.MANUAL_PRESET, SEE3CAM_CU31.PRESET_SELECT)
                                getValuesFromCamera()
                            }
                            Keys.onReturnPressed:  {
                                see3camcu31.setPresetMode(SEE3CAM_CU31.MANUAL_PRESET, SEE3CAM_CU31.PRESET_SELECT)
                                getValuesFromCamera()
                            }
                        }

                        Button {
                            id: savePreset
                            activeFocusOnPress : true
                            text: "Save"
                            style: econButtonStyle
                            implicitHeight: 20
                            implicitWidth: 40
                            enabled: (!manualPreset.checked && uvcAutoExposureSelected) ? true : false
                            opacity: (!manualPreset.checked && uvcAutoExposureSelected) ? 1 : 0.1
                            tooltip: "The User Preset block allows user to customize controls according to their preferences
and save them for subsequent use.

Save – Allows you to save the specific user-defined settings of the selected preset."
                            onClicked: {
                                savingPresetMode()
                            }
                            Keys.onReturnPressed: {
                                savingPresetMode()
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
                       tooltip: "Click to view the ISP firmware version"
                       style: econButtonStyle
                       onClicked: {
                           see3camcu31.readISPFirmwareVersion()
                       }
                   }
               }
            }
        }
    }

   SEE3CAM_CU31{
     id:see3camcu31

     onFlipMirrorModeChanged:
     {
         currentFlipMirrorMode(flipMirrorModeValues)
     }

     onCurrentRawNoiseDeductionStatus: {
         if(status == SEE3CAM_CU31.RNR_ENABLE)
         {
            enableRNR.checked = true
         }
         else if(status == SEE3CAM_CU31.RNR_DISABLE)
         {
            disableRNR.checked = true
         }
     }
     onCurrentTriggerMode:{
         if(mode == SEE3CAM_CU31.DISABLE_TRIGGER)
         {
             triggerDisable.checked = true
             root.startUpdatePreviewInMasterMode()

             root.checkForTriggerMode(false)
             root.captureBtnEnable(true)
             root.videoRecordBtnEnable(true)
         }
         else if(mode == SEE3CAM_CU31.READOUT_TRIGGER)
         {
             triggerReadOut.checked = true
             root.stopUpdatePreviewInTriggerMode()

             root.checkForTriggerMode(true)
             root.captureBtnEnable(false)
             root.videoRecordBtnEnable(false)
         }
         else if(mode == SEE3CAM_CU31.SHUTTER_TRIGGER)
         {
             triggerShutter.checked = true
             root.stopUpdatePreviewInTriggerMode()

             root.checkForTriggerMode(true)
             root.captureBtnEnable(false)
             root.videoRecordBtnEnable(false)
         }
     }
     onCurrentAEConvergenceSpeed:{
         skipUpdateUIAutoExposure = false
         convergenceSpeedSlider.value = currentSpeed
         skipUpdateUIAutoExposure = true
     }

     onCurrentAutoExposureStatus:{
         if(status == SEE3CAM_CU31.AE_RELEASE){
            autoExposure.checked = true
         }
         else if(status == SEE3CAM_CU31.AE_HOLD){
            autoExposureHold.checked = true
         }
     }

     onCurrentAutoWhiteBalanceStatus:{
         if(status === SEE3CAM_CU31.AWB_RELEASE){
            awbRelease.checked = true
         } else if(status === SEE3CAM_CU31.AWB_HOLD) {
            awbHold.checked = true
         }
     }

     onCurrentAntiFlickerModeRecieved: {
        currentAntiFlickerMode(mode)
     }

     onCurrentCameraModeReceived: {
        currentCameraMode(mode)
     }

     onCurrentAEWindowOverlayStatus: {
        currentAEWindowOverlayStatusReceived(status)
     }

     onCurrentAEMaskOverlayStatus: {
         currentAEMaskOverlayStatusReceived(status)
     }

     onCurrentAEWindowDimensions: {
         aeWidthTextField.text  = width
         aeHeightTextField.text = height
         aeXStartTextField.text = xStart
         aeYStartTextField.text = yStart
     }

     onCurrentAEMaskDimensions:{
         aeMaskWidthTextField.text = width
         aeMaskHeightTextField.text = height
         aeMaskXStartTextField.text = xStart
         aeMaskYStartTextField.text = yStart

         if(maskState === disable){
             disableAEMaskStatus.checked = true
         } else if(maskState === enable){
             enableAEMaskStatus.checked = true
         }

         if(isMaskIdNeeded){
             skipUpdateUIOnAEMaskID = false
             aeMaskIDCombo.currentIndex = maskID - 1
             skipUpdateUIOnAEMaskID = true
         }
     }


     onCurrentAWBWindowOverlayStatus: {
         currentAWBWindowOverlayStatusReceived(status)
     }

     onCurrentAWBWindowDimensions: {
         awbWidthTextField.text  = width
         awbHeightTextField.text = height
         awbXStartTextField.text = xStart
         awbYStartTextField.text = yStart
     }

     onCurrentAWBMaskOverlayStatus: {
         currentAWBMaskOverlayStatusReceived(status)
     }

     onCurrentAWBMaskDimensions:{
         awbMaskWidthTextField.text = width
         awbMaskHeightTextField.text = height
         awbMaskXStartTextField.text = xStart
         awbMaskYStartTextField.text = yStart

         if(maskState === disable){
             disableAWBMaskStatus.checked = true
         } else if(maskState === enable){
             enableAWBMaskStatus.checked = true
         }

         if(isMaskIDNeeded){
             skipUpdateUIOnAWBMaskID = false
             awbMaskIDCombo.currentIndex = maskID - 1
             skipUpdateUIOnAWBMaskID = true
         }
     }

     onCurrentPresetProperties: {
         getPresetProperties(presetMode, presetType)
     }

     onCurrentTemperature: {
        temperature = parseFloat((temperature).toFixed(2));
        readTempTextField.text = temperature.toFixed(2)
     }
     onTitleTextChanged: {
         messageDialog.title = _title.toString()
         messageDialog.text = _text.toString()
         messageDialog.open()
     }

     onIndicateFailureStatus:{
         displayMessageBox(title, text)
     }

     onIndicateWindowDimensionError:{
         displayMessageBox("Error",
         "Error: Value exceeds range and/or an odd number.
Enter a valid value in the following specified ranges,
         Width range: [" + minWidth + "   , " + maxWidth + " ]
         Height range: [ " + minHeight + " , " + maxHeight + " ]
         X Start range: [ " + minXStart + " , " + maxXStart + " ]
         Y Start range: [ " + minYStart + " , " + maxYStart + " ]
Note : Ensure the values entered are all even numbers." )
         see3camcu31.getAEWindowDimensions()
         see3camcu31.getAWBWindowDimensions()
     }

     onIndicateMaskDimensionError:{
         displayMessageBox("Error",
         "Error: Value exceeds range and/or an odd number.
Enter a valid value in the following specified ranges,
         Width range: [" + minWidth + "   , " + maxWidth + " ]
         Height range: [ " + minHeight + " , " + maxHeight + " ]
         X Start range: [ " + minXStart + " , " + maxXStart + " ]
         Y Start range: [ " + minYStart + " , " + maxYStart + " ]
Note : Ensure the values entered are all even numbers." )

         awbMaskIDValue = awbMaskIDCombo.currentText
         aeMaskIDValue  = aeMaskIDCombo.currentText

         see3camcu31.getAEMaskDimensions(true, aeMaskIDValue)
         see3camcu31.getAWBMaskDimensions(true, awbMaskIDValue)

         awbMaskIDCombo.currentIndex = awbMaskIDValue - 1
         aeMaskIDCombo.currentIndex = aeMaskIDValue - 1
     }

     onIndicateWindowWidthError:{
         displayMessageBox("Error",
         "Error: Window extends beyond frame width - The sum of Width and X Start should not exceed the frame width " + maxWidth + "." )
         see3camcu31.getAEWindowDimensions()
         see3camcu31.getAWBWindowDimensions()
     }

     onIndicateWindowHeightError:{
         displayMessageBox("Error",
         "Error: Window extends beyond frame height - The sum of Height and Y Start should not exceed the frame height " + maxHeight + "." )
         see3camcu31.getAEWindowDimensions()
         see3camcu31.getAWBWindowDimensions()
     }

     onIndicateMaskWidthError:{
         displayMessageBox("Error",
         "Error: Window extends beyond frame width - The sum of Width and X Start should not exceed the frame width " + maxWidth + "." )

         awbMaskIDValue = awbMaskIDCombo.currentText
         aeMaskIDValue  = aeMaskIDCombo.currentText

         see3camcu31.getAEMaskDimensions(true, aeMaskIDValue)
         see3camcu31.getAWBMaskDimensions(true, awbMaskIDValue)

         awbMaskIDCombo.currentIndex = awbMaskIDValue - 1
         aeMaskIDCombo.currentIndex = aeMaskIDValue - 1
     }

     onIndicateMaskHeightError:{
         displayMessageBox("Error",
         "Error: Window extends beyond frame width - The sum of Height and Y Start should not exceed the frame width " + maxHeight + "." )

         awbMaskIDValue = awbMaskIDCombo.currentText
         aeMaskIDValue  = aeMaskIDCombo.currentText

         see3camcu31.getAEMaskDimensions(true, aeMaskIDValue)
         see3camcu31.getAWBMaskDimensions(true, awbMaskIDValue)

         awbMaskIDCombo.currentIndex = awbMaskIDValue - 1
         aeMaskIDCombo.currentIndex = aeMaskIDValue - 1
     }


   }

   function getPresetProperties(mode, type)
   {
       if(mode === SEE3CAM_CU31.MANUAL_PRESET){
           manualPreset.checked = true
       } else if (mode === SEE3CAM_CU31.USER_PRESET_1) {
           userPreset1.checked = true
       } else if (mode === SEE3CAM_CU31.USER_PRESET_2) {
           userPreset2.checked = true
       }
   }

   function setOrientationProperties(){
       see3camcu31.setOrientation(flipCtrlHorizotal.checked, flipCtrlVertical.checked)

       awbMaskIDValue = awbMaskIDCombo.currentText
       aeMaskIDValue  = aeMaskIDCombo.currentText

       see3camcu31.getAEWindowDimensions()
       see3camcu31.getAEMaskDimensions(true, aeMaskIDValue)
       see3camcu31.getAWBWindowDimensions()
       see3camcu31.getAWBMaskDimensions(true, awbMaskIDValue)
   }

   function setAWBMaskDimensionsControl()
   {
       if(disableAWBMaskStatus.checked == true) {
           see3camcu31.setAWBMaskDimensions(awbMaskIDCombo.currentText, disable, awbMaskWidthTextField.text, awbMaskHeightTextField.text, awbMaskXStartTextField.text, awbMaskYStartTextField.text)
       } else if(enableAWBMaskStatus.checked == true) {
           see3camcu31.setAWBMaskDimensions(awbMaskIDCombo.currentText, enable, awbMaskWidthTextField.text, awbMaskHeightTextField.text, awbMaskXStartTextField.text, awbMaskYStartTextField.text)
       }
   }

   function setAEMaskDimensionsControl()
   {
       if(disableAEMaskStatus.checked == true) {
           see3camcu31.setAEMaskDimensions(aeMaskIDCombo.currentText, disable, aeMaskWidthTextField.text, aeMaskHeightTextField.text, aeMaskXStartTextField.text, aeMaskYStartTextField.text)
       } else if(enableAEMaskStatus.checked == true) {
           see3camcu31.setAEMaskDimensions(aeMaskIDCombo.currentText, enable, aeMaskWidthTextField.text, aeMaskHeightTextField.text, aeMaskXStartTextField.text, aeMaskYStartTextField.text)
       }
   }

   function savingPresetMode() {
        if(userPreset1.checked == true){
            see3camcu31.setPresetMode(SEE3CAM_CU31.USER_PRESET_1, SEE3CAM_CU31.PRESET_SAVE)
        } else if(userPreset2.checked == true) {
            see3camcu31.setPresetMode(SEE3CAM_CU31.USER_PRESET_2, SEE3CAM_CU31.PRESET_SAVE)
        }
        getValuesFromCamera()
   }

   function currentAEWindowOverlayStatusReceived(status){
          if(status === enable){
               enableAEWindow.checked = true
          } else if(status === disable) {
               disableAEWindow.checked = true
          }
      }

      function currentAEMaskOverlayStatusReceived(status){
          if(status === enable){
               enableAEMask.checked = true
          } else if(status === disable) {
               disableAEMask.checked = true
          }
      }

      function currentAWBWindowOverlayStatusReceived(status){
          if(status === enable){
               enableAWBWindow.checked = true
          } else if(status === disable) {
               disableAWBWindow.checked = true
          }
      }

      function currentAWBMaskOverlayStatusReceived(status){
          if(status === enable){
               enableAWBMask.checked = true
          } else if(status === disable) {
               disableAWBMask.checked = true
          }
      }

   function currentCameraMode(mode){
       if(mode === SEE3CAM_CU31.AUTO){
          autoCameraMode.checked = true
       }else if(mode === SEE3CAM_CU31.LOW_LIGHT){
          lowLightMode.checked = true
       }else if(mode === SEE3CAM_CU31.LED){
          ledMode.checked = true
       }
   }

   function currentAntiFlickerMode(mode){
       if(mode === SEE3CAM_CU31.MODE_AUTO){
          flickerAuto.checked = true
       } else if(mode === SEE3CAM_CU31.MODE_50Hz){
          flicker50hz.checked = true
       } else if(mode === SEE3CAM_CU31.MODE_60Hz){
          flicker60hz.checked = true
       } else if(mode === SEE3CAM_CU31.MODE_DISABLE){
          flickerDisable.checked = true
       }
   }

   function setFlickerControl(){
       if(flickerAuto.checked == true){
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_AUTO)
       } else if (flicker50hz.checked == true) {
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_50Hz)
       } else if(flicker60hz.checked == true) {
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_60Hz)
       } else if(flickerDisable.checked == true) {
           see3camcu31.setAntiFlickerMode(SEE3CAM_CU31.MODE_DISABLE)
       }
   }

   function disableTriggerMode(){
       see3camcu31.setTriggerMode(SEE3CAM_CU31.DISABLE_TRIGGER)

       root.startUpdatePreviewInMasterMode()

       root.checkForTriggerMode(false)
       root.captureBtnEnable(true)
       root.videoRecordBtnEnable(true)
   }

   function setTriggerMode(){
       if(triggerReadOut.checked)
       {
           see3camcu31.setTriggerMode(SEE3CAM_CU31.READOUT_TRIGGER)
       }
       else if(triggerShutter.checked)
       {
           see3camcu31.setTriggerMode(SEE3CAM_CU31.SHUTTER_TRIGGER)
       }

       root.stopUpdatePreviewInTriggerMode()

       root.checkForTriggerMode(true)
       root.captureBtnEnable(false)
       root.videoRecordBtnEnable(false)
   }

   function currentFlipMirrorMode(mode)
   {
       switch(mode)
       {
           case SEE3CAM_CU31.NORMAL:
               flipCtrlVertical.checked  = false
               flipCtrlHorizotal.checked = false
               break;
           case SEE3CAM_CU31.VERTICAL:
               flipCtrlVertical.checked  = true
               flipCtrlHorizotal.checked = false
               break;
           case SEE3CAM_CU31.HORIZONTAL:
               flipCtrlVertical.checked  = false
               flipCtrlHorizotal.checked = true
               break;
           case SEE3CAM_CU31.ROTATE_180:
               flipCtrlVertical.checked  = true
               flipCtrlHorizotal.checked = true
               break;
       }
   }

   function setAutoExposure() {
        if(autoExposure.checked == true){
            see3camcu31.setAutoExposureStatus(SEE3CAM_CU31.AE_RELEASE)
        } else if(autoExposureHold.checked == true){
            see3camcu31.setAutoExposureStatus(SEE3CAM_CU31.AE_HOLD)
        }
   }

   function enableDisableAutoExposureControls(autoExposureSelect){
       if(autoExposureSelect){
           uvcAutoExposureSelected = true
           if(autoExposureHold.checked == true){
               displayMessageBox("Message", "AE hold is active")
           }
       }else{
           uvcAutoExposureSelected = false
       }
   }

   function displayMessageBox(title, text){
       messageDialog.title = qsTr(title)
       messageDialog.text = qsTr(text)
       messageDialog.open()
   }

    Uvccamera {
        id: uvccamera
        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
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
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 50
                implicitHeight: 23
                border.color: "#333"
                border.width: 3
                y: 1
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

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        see3camcu31.setToDefaultValues()
        root.startUpdatePreviewInMasterMode()
        getValuesFromCamera()
    }

    function getSerialNumber() {
        see3camcu31.get64BitSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    function getValuesFromCamera(){
        see3camcu31.getOrientation()
        see3camcu31.getRawNoiseReductionStatus()
        see3camcu31.getTriggerMode()
        see3camcu31.getAEConvergenceSpeed()
        see3camcu31.getAutoExpStatus()
        see3camcu31.getAWBStatus()
        see3camcu31.readTemperature()
        see3camcu31.getCameraMode()
        see3camcu31.getAntiFlickerMode()

        see3camcu31.getAEWindowOverlay()
        see3camcu31.getAEMaskOverlay()
        see3camcu31.getAEMaskDimensions(true, 1)

        see3camcu31.getAWBWindowOverlay()
        see3camcu31.getAWBMaskOverlay()
        see3camcu31.getAWBMaskDimensions(true, 1)

        see3camcu31.getAEWindowDimensions()
        see3camcu31.getAWBWindowDimensions()

        see3camcu31.getPresetMode()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
