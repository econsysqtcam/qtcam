import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcExtsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0
import "../../JavaScriptFiles/tempValue.js" as JS

Item {
    id: cx3_sni
    property bool skipUpdateInCamOnAWbPreset: false
    property bool skipUpdateInCamOnLedBrightness: false
    property bool skipUpdateInCamOnAEMeterMode: false
    property bool skipUpdateInCamOnFlickerMode: false
    property bool skipUpdateInCamOnJpegQValue: false
    property bool skipUpdateInCamOnCAFScanRangeMode: false
    property bool skipUpdateManualLensPositionMode: false
    property int  aeMeteringMode
    property int  flickerMode
    property int  cafScanRangeMode
    property int  manualLensPosMode

    // Update led brightness in UI for every 2 secs
    Timer {
        id: geLedStatusTimer
        interval: 2000
        repeat: true
        onTriggered: {
            uvcExtCamAccessId.getLedBrightness(UvcExtcamera.UVC_GET_CUR)
            // skipUpdateInCamOnLedBrightness is false when getting led brightness.
            // Now set skipUpdateInCamOnLedBrightness is true to set the value when moving led brightness slider
            skipUpdateInCamOnLedBrightness = true
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
            if(radiocustom.enabled && radiocustom.checked){
                uvcExtCamAccessId.setCustomAreaAutoFocus(width, height, x, y, customAreaWindowSizeCombo.currentText)
            }
        }


        onAutoWhiteBalanceSelected:{
            if(autoWhiteBalanceSelect){
                awbLock.enabled = true
                awbLock.opacity = 1
                awbPresetModeText.opacity = 1
                awbPresetsCombo.enabled = true
                awbPresetsCombo.opacity = 1
            }else{
                awbLock.enabled = false
                awbLock.opacity = 0.1
                awbPresetModeText.opacity = 0.1
                awbPresetsCombo.enabled = false
                awbPresetsCombo.opacity = 0.1
            }
        }
        onAutoFocusSelected:{
            extSettingsBasedOnAutoFocusSelectionInUVCSettings(autoFocusSelect)
        }

        onAutoExposureSelected:{
        }

        // Update led brightness in UI for every 2 secs only when extension setting extension tab visible
        onExtensionTabVisible:{
            if(visible){
                geLedStatusTimer.start()
                getMinMaxStepSizeValues()
                getValuesFromCamera(UvcExtcamera.UVC_GET_CUR)
                extSettingsBasedOnAutoFocusSelectionInUVCSettings(JS.autoFocusChecked) // Initially get the auto focus mode in UVC settings and enable/disable controls in extension unit
            }
            else{
                geLedStatusTimer.stop()
            }
        }
    }

    UvcExtcamera{
        id: uvcExtCamAccessId
        onLedBrightnessValueReceived:{
            queryForLedControl(queryType, brightnessVal)
        }

        onAwbPresetsModeReceived:{
            skipUpdateInCamOnAWbPreset = false
            awbPresetsCombo.currentIndex = awbPresetMode - 1
            if(queryType == UvcExtcamera.UVC_GET_DEF){
                uvcExtCamAccessId.setAWBPresetControl(awbPresetsCombo.currentIndex + 1)
            }
        }
        onAwbLockValueReceived:{
            if(awbLockValue)
                awbLock.checked = true
            else
                awbLock.checked = false

            if(queryType == UvcExtcamera.UVC_GET_DEF){
                uvcExtCamAccessId.setAWBLock(awbLock.checked)
            }
        }
        onAutoExpModeReceived:{
            skipUpdateInCamOnAEMeterMode = false
            switch(autoExpMode){
            case UvcExtcamera.AE_OFF:
                aeMeteringModeCombo.currentIndex = 0
                break
            case UvcExtcamera.AE_CENTER_WEIGHTED_AVG_1:
                aeMeteringModeCombo.currentIndex = 1
                break
            case UvcExtcamera.AE_ALL_BLOCK_INTEGRAL:
                aeMeteringModeCombo.currentIndex = 2
                break
            case UvcExtcamera.AE_SPOT1_SMALL_AREA:
                aeMeteringModeCombo.currentIndex = 3
                break
            case UvcExtcamera.AE_SPOT2_LARGE_AREA:
                aeMeteringModeCombo.currentIndex = 4
                break
            }

            if(queryType == UvcExtcamera.UVC_GET_DEF){
                setAutoExpMeteringMode()
            }
        }
        onAutoExpLockValueReceived:{
            if(autoExpoureLockValue)
                aeLock.checked = true
            else
                aeLock.checked = false

            if(queryType == UvcExtcamera.UVC_GET_DEF){
                uvcExtCamAccessId.setAELock(aeLock.checked)
            }
        }
        onFlickerModeReceived:{
            skipUpdateInCamOnFlickerMode = false
            switch(flickerMode){
                case UvcExtcamera.FLICKER_AUTO_DETECTION:
                    flickerModeCombo.currentIndex = 0
                    break
                case UvcExtcamera.FLICKER_50HZ:
                    flickerModeCombo.currentIndex = 1
                    break
                case UvcExtcamera.FLICKER_60HZ:
                    flickerModeCombo.currentIndex = 2
                    break
                case UvcExtcamera.FLICKER_OFF:
                    flickerModeCombo.currentIndex = 3
                    break
            }
            if(queryType == UvcExtcamera.UVC_GET_DEF){
                setFlickerModeFn()
            }
        }
        onJpegQValueReceived:{
            queryForJpegQValueControl(queryType, jpegQVal)

        }
        onCafScanRangeModeReceived:{            
            skipUpdateInCamOnCAFScanRangeMode = false
            switch(scanRangeMode){
                case UvcExtcamera.CAF_MACRO_RANGE_MODE:
                    cafScanRangeCombo.currentIndex = 0
                    break
                case UvcExtcamera.CAF_NORMAL_RANGE_MODE:
                    cafScanRangeCombo.currentIndex = 1
                    break
                case UvcExtcamera.CAF_FULL_RANGE_MODE:
                    cafScanRangeCombo.currentIndex = 2
                    break
                case UvcExtcamera.CAF_INFINITY_RANGE_MODE:
                    cafScanRangeCombo.currentIndex = 3
                    break
                case UvcExtcamera.CAF_HYPERFOCAL_RANGE_MODE:
                    cafScanRangeCombo.currentIndex = 4
                    break
            }
            if(queryType == UvcExtcamera.UVC_GET_DEF){
                setCAFScanRangeModeFn()
            }
        }
        onManualLensPositionModeReceived:{            
            skipUpdateManualLensPositionMode = false
            switch(lensPosMode){
            case UvcExtcamera.LENS_POS_INF:
                manualLensPositionCombo.currentIndex = 0
                break
            case UvcExtcamera.LENS_POS_NORMAL:
                manualLensPositionCombo.currentIndex = 1
                break
            case UvcExtcamera.LENS_BARCODE_POS:
                manualLensPositionCombo.currentIndex = 2
                break
            case UvcExtcamera.LENS_MACRO_POS:
                manualLensPositionCombo.currentIndex = 3
                break
            case UvcExtcamera.LENS_INITIAL_DAC_POS:
                manualLensPositionCombo.currentIndex = 4
                break
            }
            if(queryType == UvcExtcamera.UVC_GET_DEF){
                setManualLensFocusPosMode()
            }
        }

        onAutoFocusModeReceived:{            
            switch(autoFocusModeValue){
            case UvcExtcamera.AF_CONTINUOUS:
                radioContin.checked = true
                afOff.checked = false
                break
            case UvcExtcamera.AF_ONESHOT:
                radioOneshot.checked = true
                afOff.checked = false
                break
            case UvcExtcamera.AF_OFF:
                radioContin.enabled = false
                radioOneshot.enabled = false
                afOff.checked = true
                break
            }
            if(queryType == UvcExtcamera.UVC_GET_DEF){
                setManualLensFocusPosMode()
            }
        }
        onAfLockValueReceived:{            
            switch(afLockValue){
            case 0x00:
                afLock.checked = true
                break
            case 0x01:
                afLock.checked = false
                break;
            }
            if(queryType == UvcExtcamera.UVC_GET_DEF){
                uvcExtCamAccessId.setAFLock(afLock.checked)
            }
        }
        onAutoFocusRoiModeReceived:{            
            if(afRoiMode == UvcExtcamera.ROI_CENTERED){
                radiocenter.checked = true
                if(queryType == UvcExtcamera.UVC_GET_DEF){
                    uvcExtCamAccessId.setCenteredAutoFocusMode(root.vidstreamObj)
                }
            }
            else if(afRoiMode == UvcExtcamera.ROI_CUSTOM){
                radiocustom.checked = true
                if(queryType == UvcExtcamera.UVC_GET_DEF){
                    uvcExtCamAccessId.setCustomAreaAutoFocus(width, height, width/2, height/2, customAreaWindowSizeCombo.currentText)
                }
            }

        }

        onUpdateFrameToSkipfromCam:{
            root.updateFrametoSkip(frameToSkip)
        }

        onTitleTextChanged: {
            displayMessageBox(qsTr(_title), qsTr(_text))
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
            height:1300
            ColumnLayout{
                x:2
                y:5
                spacing:20
                Text {
                    id: ledMode
                    text: "--- Led Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Grid {
                    columns: 2
                    spacing: 50
                    ExclusiveGroup { id: ledGroup }
                    RadioButton {
                        id: ledOn
                        style:  radioButtonWordWrapStyle
                        text:   qsTr("On")
                        exclusiveGroup: ledGroup
                        activeFocusOnPress: true                       
                        onClicked: {
                            setLedStatusValue()
                        }
                        Keys.onReturnPressed: {                            
                            setLedStatusValue()
                        }
                    }
                    RadioButton {
                        id: ledOff
                        style:  radioButtonWordWrapStyle
                        text: qsTr("Off")
                        exclusiveGroup: ledGroup
                        activeFocusOnPress: true
                        onClicked: {
                            uvcExtCamAccessId.setLedBrightness(0)
                        }
                        Keys.onReturnPressed: {
                            uvcExtCamAccessId.setLedBrightness(0)
                        }
                    }
                }
                Row{
                    spacing: 20
                    Slider {
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: ledBrightnessSlider
                        width: 150
                        enabled: ledOn.checked ? true : false
                        opacity: enabled ? 1 : 0.1
                        style:econSliderStyle
                        onValueChanged:  {
                            if(skipUpdateInCamOnLedBrightness){
                                uvcExtCamAccessId.setLedBrightness(ledBrightnessSlider.value)                                
                            }
                            skipUpdateInCamOnLedBrightness = true
                        }
                    }
                    TextField {
                        id: ledBrighntessTextField
                        text: ledBrightnessSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        enabled: false
                        opacity: ledOn.checked ? 1 : 0.1
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: ledBrightnessSlider.minimumValue; top: ledBrightnessSlider.maximumValue}
                    }
                }
                Text {
                    id: awbPresetModeText
                    text: "--- AWB Presets ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                CheckBox {
                    id: awbLock
                    enabled: JS.autoWhiteBalSelected ? true : false
                    opacity: JS.autoWhiteBalSelected ? 1 : 0.1
                    activeFocusOnPress : true
                    text: "AWB Lock"
                    style: econCheckBoxStyle
                    onClicked:{
                        uvcExtCamAccessId.setAWBLock(checked)
                    }
                    Keys.onReturnPressed: {
                        uvcExtCamAccessId.setAWBLock(checked)
                    }
                }
                ComboBox
                {
                    id: awbPresetsCombo
                    enabled: JS.autoWhiteBalSelected ? true : false
                    opacity: JS.autoWhiteBalSelected ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "Cloudy" }
                        ListElement { text: "DayLight" }
                        ListElement { text: "Flash" }
                        ListElement { text: "Cool White Fluorescent" }
                        ListElement { text: "Tungsten" }
                        ListElement { text: "CandleLight" }
                        ListElement { text: "Horizon" }
                        ListElement { text: "Auto" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateInCamOnAWbPreset){
                            // combo index starts from 0. AWB preset values start from 1.  So Add 1 to set the AWB preset mode
                            uvcExtCamAccessId.setAWBPresetControl(awbPresetsCombo.currentIndex + 1)
                        }
                        skipUpdateInCamOnAWbPreset = true
                    }
                }


                Text {
                    id: aeMeteringModeText
                    text: "--- AE metering mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                CheckBox {
                    id: aeLock
                    activeFocusOnPress : true
                    text: "AE Lock"
                    style: econCheckBoxStyle
                    onClicked:{
                        uvcExtCamAccessId.setAELock(checked)
                    }
                    Keys.onReturnPressed: {
                        uvcExtCamAccessId.setAELock(checked)
                    }
                }
                ComboBox
                {
                    id: aeMeteringModeCombo
                    opacity: 1
                    enabled: true
                    model: ListModel {
                        ListElement { text: "Manual" }
                        ListElement { text: "Center-Weighted average" }
                        ListElement { text: "All block integral" }
                        ListElement { text: "Spot1(small area)" }
                        ListElement { text: "Spot2(large area)" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateInCamOnAEMeterMode){
                            setAutoExpMeteringMode()
                        }
                        skipUpdateInCamOnAEMeterMode = true
                    }
                }                
                Text {
                    id: flickerCompText
                    text: "--- Flicker Compensation ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ComboBox
                {
                    id: flickerModeCombo
                    opacity: 1
                    enabled: true
                    model: ListModel {
                        ListElement { text: "Auto detection and compensation" }
                        ListElement { text: "50Hz flickerless mode" }
                        ListElement { text: "60Hz flickerless mode" }
                        ListElement { text: "Flicker compensation off" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateInCamOnFlickerMode){
                            setFlickerModeFn()
                        }
                        skipUpdateInCamOnFlickerMode = true
                    }
                }

                Text {
                    id: jpegQValueText
                    text: "--- JPEG Q Value ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 20
                    Slider {                        
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        id: jpegQValueSlider
                        width: 150                       
                        enabled: true
                        opacity: 1
                        style:econSliderStyle
                        onValueChanged:  {                            
                            if(skipUpdateInCamOnJpegQValue){
                                uvcExtCamAccessId.setJpegQfactor(jpegQValueSlider.value)
                            }
                            skipUpdateInCamOnJpegQValue = true
                        }
                    }
                    TextField {
                        id: jpegQValueSliderTextField
                        text: jpegQValueSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        enabled: true
                        opacity: 1
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: jpegQValueSlider.minimumValue; top: jpegQValueSlider.maximumValue}
                    }
                }
                Text {
                    id: focusModeCtrlsText
                    text: "--- Focus Mode Controls ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                CheckBox {
                    id: afOff
                    activeFocusOnPress : true
                    text: "AF Off"
                    style: econCheckBoxStyle
                    onClicked:{
                        afOffSelection()
                    }
                    Keys.onReturnPressed: {
                        afOffSelection()
                    }
                }
                Text {
                    id: manualLensPositionText
                    text: "Manual focus Lens Position:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: afOff.checked ? 1 : 0.1
                }

                ComboBox
                {
                    id: manualLensPositionCombo
                    opacity: enabled ? 1 : 0.1
                    enabled: afOff.checked ? true : false
                    model: ListModel {
                        ListElement { text: "INF position(10m)" }
                        ListElement { text: "Normal Position(1m)" }
                        ListElement { text: "Bar code position(10cm)" }
                        ListElement { text: "Macro position(10cm)" }
                        ListElement { text: "Initial DAC position(for Power-Off)"}

                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateManualLensPositionMode){
                            setManualLensFocusPosMode()
                        }
                        skipUpdateManualLensPositionMode = true
                    }
                }

                CheckBox {
                    id: afLock
                    activeFocusOnPress : true
                    enabled: true
                    opacity: enabled ? 1 : 0.1
                    text: "AF Lock"
                    style: econCheckBoxStyle
                    onClicked:{
                        uvcExtCamAccessId.setAFLock(checked)
                    }
                    Keys.onReturnPressed:{
                        uvcExtCamAccessId.setAFLock(checked)
                    }
                }

                Text {
                    id: cAFScanRange
                    text: "CAF Scan Range:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    enabled: afLock.enabled ? true : false
                    opacity: enabled ? 1 : 0.1
                }

                ComboBox
                {
                    id: cafScanRangeCombo
                    enabled: afLock.enabled ? true : false
                    opacity: enabled ? 1 : 0.1
                    model: ListModel {
                        ListElement { text: "Macro Range Mode" }
                        ListElement { text: "Normal Range Mode" }
                        ListElement { text: "Full Range Mode" }
                        ListElement { text: "Infinity Range Mode" }
                        ListElement { text: "Hyperfocal Range Mode"}

                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateInCamOnCAFScanRangeMode){
                            setCAFScanRangeModeFn()
                        }
                        skipUpdateInCamOnCAFScanRangeMode = true
                    }
                }

                Text {
                    id: focusModeText
                    text: "Auto Focus Modes:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: afOff.checked ? 0.1 : 1
                }

                Row{
                    spacing:10
                    ExclusiveGroup { id: afgroup }
                    RadioButton {
                        exclusiveGroup: afgroup
                        id: radioContin
                        text: "Continuous"
                        activeFocusOnPress: true
                        style: radioButtonWordWrapStyle
                        enabled: true
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            uvcExtCamAccessId.setAutoFocusMode(UvcExtcamera.AF_CONTINUOUS);
                            uvcExtCamAccessId.getAFLock(UvcExtcamera.UVC_GET_CUR)
                        }
                        Keys.onReturnPressed: {
                            uvcExtCamAccessId.setAutoFocusMode(UvcExtcamera.AF_CONTINUOUS);
                            uvcExtCamAccessId.getAFLock(UvcExtcamera.UVC_GET_CUR)
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
                        style: radioButtonWordWrapStyle
                        enabled: true
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            uvcExtCamAccessId.setAutoFocusMode(UvcExtcamera.AF_ONESHOT)
                            uvcExtCamAccessId.getAFLock(UvcExtcamera.UVC_GET_CUR)
                        }
                        Keys.onReturnPressed: {
                            uvcExtCamAccessId.setAutoFocusMode(UvcExtcamera.AF_ONESHOT)
                            uvcExtCamAccessId.getAFLock(UvcExtcamera.UVC_GET_CUR)
                        }
                    }
                }

                Text {
                    id: autoFocusAreaText
                    text: "--- Auto Focus Area ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }                
                RowLayout{
                    spacing:25
                    ExclusiveGroup { id: groupAF }
                    RadioButton {
                        exclusiveGroup: groupAF
                        id: radiocenter
                        text: "Center Weighted"
                        activeFocusOnPress: true
                        style: radioButtonWordWrapStyle
                        enabled: true
                        opacity: enabled ? 1 : 0.1
                        onClicked:{
                           centeredModeButtonClicked()
                        }                        
                        Keys.onReturnPressed: {
                           centeredModeButtonClicked()
                        }
                    }
                    RadioButton {
                        exclusiveGroup: groupAF
                        id: radiocustom
                        text: "Custom Area"
                        activeFocusOnPress: true
                        style: radioButtonWordWrapStyle
                        enabled: true
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            customModeButtonClicked()
                        }
                        Keys.onReturnPressed: {
                            customModeButtonClicked()
                        }
                    }
                }
                Text {
                    id: customAreaWindowSize
                    text: "Window Size:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: radiocustom.checked ? 1 : 0.1
                }
                ComboBox
                {
                    id: customAreaWindowSizeCombo
                    opacity: radiocustom.checked ? 1 : 0.1
                    enabled: radiocustom.checked ? true : false
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
                    }
                }
                Text {
                    id: fwVersionText
                    text: "--- Firmware version ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: fwVersion
                        activeFocusOnPress : true
                        text: "F/W Version"
                        tooltip: "Click to see the firmware version and build date"
                        style: econButtonStyle
                        opacity: 1
                        onClicked: {
                            getFirmwareVersion()
                        }
                        Keys.onReturnPressed: {
                            getFirmwareVersion()
                        }
                    }
                }
                Text {
                    id: defaultText
                    text: "--- Default Settings ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: defaultSettings
                        activeFocusOnPress : true
                        text: "Default"
                        tooltip: "Click to set all the controls to default values"
                        style: econButtonStyle
                        opacity: 1
                        onClicked: {
                            setToDefaultValues()
                        }
                        Keys.onReturnPressed: {
                            setToDefaultValues()
                        }
                    }
                }
            }
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)        
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    function setManualLensFocusPosMode(){
        switch(manualLensPositionCombo.currentIndex){
        case 0:
            manualLensPosMode = UvcExtcamera.LENS_POS_INF
            break
        case 1:
            manualLensPosMode = UvcExtcamera.LENS_POS_NORMAL
            break
        case 2:
            manualLensPosMode = UvcExtcamera.LENS_BARCODE_POS
            break
        case 3:
            manualLensPosMode = UvcExtcamera.LENS_MACRO_POS
            break
        case 4:
            manualLensPosMode = UvcExtcamera.LENS_INITIAL_DAC_POS
            break
        }

        uvcExtCamAccessId.setManualLensPositionMode(manualLensPosMode)
    }

    function setCAFScanRangeModeFn(){
        switch(cafScanRangeCombo.currentIndex){
        case 0:
            cafScanRangeMode = UvcExtcamera.CAF_MACRO_RANGE_MODE
            break
        case 1:
            cafScanRangeMode = UvcExtcamera.CAF_NORMAL_RANGE_MODE
            break
        case 2:
            cafScanRangeMode = UvcExtcamera.CAF_FULL_RANGE_MODE
            break
        case 3:
            cafScanRangeMode = UvcExtcamera.CAF_INFINITY_RANGE_MODE
            break
        case 4:
            cafScanRangeMode = UvcExtcamera.CAF_HYPERFOCAL_RANGE_MODE
            break
        }

        uvcExtCamAccessId.setCAFScanRangeMode(cafScanRangeMode)
    }

    // set Auto Exposure metering mode
    function setAutoExpMeteringMode(){
        switch(aeMeteringModeCombo.currentIndex){
        case 0:
            aeMeteringMode = UvcExtcamera.AE_OFF
            break
        case 1:
            aeMeteringMode = UvcExtcamera.AE_CENTER_WEIGHTED_AVG_1            
            break
        case 2:
            aeMeteringMode = UvcExtcamera.AE_ALL_BLOCK_INTEGRAL
            break
        case 3:
            aeMeteringMode = UvcExtcamera.AE_SPOT1_SMALL_AREA
            break
        case 4:
            aeMeteringMode = UvcExtcamera.AE_SPOT2_LARGE_AREA
            break
        }

        uvcExtCamAccessId.setAEMeterMode(aeMeteringMode)
    }

    // setFlickerMode
    function setFlickerModeFn(){        
        switch(flickerModeCombo.currentIndex){
        case 0:
            flickerMode = UvcExtcamera.FLICKER_AUTO_DETECTION
            break
        case 1:
            flickerMode = UvcExtcamera.FLICKER_50HZ
            break
        case 2:
            flickerMode = UvcExtcamera.FLICKER_60HZ
            break
        case 3:
            flickerMode = UvcExtcamera.FLICKER_OFF
            break
        }

        uvcExtCamAccessId.setFlickerMode(flickerMode)
    }


    function getFirmwareVersion() {
        uvcExtCamAccessId.getFirmwareVersion()
        messageDialog.open()
    }

    function setToDefaultValues(){
        getValuesFromCamera(UvcExtcamera.UVC_GET_DEF)
        updateCustomAreaWindowUI()
    }

    function updateCustomAreaWindowUI(){
        if(radiocustom.enabled && radiocustom.checked){
            customAreaWindowSize.opacity = 1
            customAreaWindowSizeCombo.enabled = true
            customAreaWindowSizeCombo.opacity = 1
        }else{
            customAreaWindowSizeCombo.enabled = false
            customAreaWindowSizeCombo.opacity = 0.1
            customAreaWindowSize.opacity = 0.1
        }
    }

    // get values from camera, [default/cur]
    function getValuesFromCamera(valueToGet){
        uvcExtCamAccessId.getLedBrightness(valueToGet)
        uvcExtCamAccessId.getAWBPresetMode(valueToGet)
        uvcExtCamAccessId.getAWBLock(valueToGet)
        uvcExtCamAccessId.getAEMeterMode(valueToGet)
        uvcExtCamAccessId.getAELock(valueToGet)
        uvcExtCamAccessId.getFlickerMode(valueToGet)
        uvcExtCamAccessId.getJpegQfactor(valueToGet)
        uvcExtCamAccessId.getCAFScanRangeMode(valueToGet)
        uvcExtCamAccessId.getManualLensPositionMode(valueToGet)
        uvcExtCamAccessId.getAutoFocusMode(valueToGet)
        uvcExtCamAccessId.getAFLock(valueToGet)
        uvcExtCamAccessId.getAutoFocusROIMode(valueToGet)
    }

    // Query for led control - min, max, current, default values
    function queryForLedControl(queryType, brightnessVal){        
        if(queryType == UvcExtcamera.UVC_GET_DEF || queryType == UvcExtcamera.UVC_GET_CUR){            
            if(brightnessVal == 0){
                skipUpdateInCamOnLedBrightness = true
                ledOff.checked = true                
            }else{                
                ledOn.checked = true
                skipUpdateInCamOnLedBrightness = false
                ledBrightnessSlider.value = brightnessVal
            }
            if(queryType == UvcExtcamera.UVC_GET_DEF){ // Default value
                uvcExtCamAccessId.setLedBrightness(brightnessVal)
            }
        }else
        if(queryType == UvcExtcamera.UVC_GET_MIN){ // Minimum value            
            ledBrightnessSlider.minimumValue = brightnessVal
        }else
        if(queryType == UvcExtcamera.UVC_GET_MAX){ // Maximum value            
            ledBrightnessSlider.maximumValue = brightnessVal
        }else
        if(queryType == UvcExtcamera.UVC_GET_RES){ // step size            
            ledBrightnessSlider.stepSize  = brightnessVal
        }
    }

    // Query for jpeg Q control - min, max, current, default values
    function queryForJpegQValueControl(queryType, jpegQVal){
        skipUpdateInCamOnJpegQValue = false
        if(queryType == UvcExtcamera.UVC_GET_DEF || queryType == UvcExtcamera.UVC_GET_CUR){
            jpegQValueSlider.value = jpegQVal
            if(queryType == UvcExtcamera.UVC_GET_DEF){ // Default value
                uvcExtCamAccessId.setJpegQfactor(jpegQValueSlider.value)
            }
        }else if(queryType == UvcExtcamera.UVC_GET_MIN){ // Minimum value
            jpegQValueSlider.minimumValue = jpegQVal
        }else if(queryType == UvcExtcamera.UVC_GET_MAX){ // Maximum value
            jpegQValueSlider.maximumValue = jpegQVal
        }else if(queryType == UvcExtcamera.UVC_GET_RES){ // step size
            jpegQValueSlider.stepSize  = jpegQVal
        }

    }

    // Based on auto focus Off selection check box state, some controls will be enabled and disabled
    function afOffSelection(){
            if(afOff.checked){
            afLock.enabled = false
            cafScanRangeCombo.enabled = false
            cafScanRangeCombo.opacity = 0.1
            cAFScanRange.opacity = 0.1
            focusModeText.opacity = 0.1
            radioContin.enabled = false
            radioOneshot.enabled = false
            radiocenter.enabled = false
            radiocustom.enabled = false
            customAreaWindowSizeCombo.enabled = false
            customAreaWindowSizeCombo.opacity = 0.1
            customAreaWindowSize.opacity = 0.1
            manualLensPositionCombo.enabled = true
            manualLensPositionCombo.opacity = 1
            manualLensPositionText.opacity = 1
        }
        else{
            afLock.enabled = true
            cafScanRangeCombo.enabled = true
            cafScanRangeCombo.opacity = 1
            cAFScanRange.opacity = 1
            focusModeText.opacity = 1
            radioContin.enabled = true
            radioOneshot.enabled = true
            radiocenter.enabled = true
            radiocustom.enabled = true
            manualLensPositionCombo.enabled = false
            manualLensPositionCombo.opacity = 0.1
            manualLensPositionText.opacity = 0.1
            if(radiocustom.checked){
                customAreaWindowSizeCombo.enabled = true
                customAreaWindowSizeCombo.opacity = 1
                customAreaWindowSize.opacity = 1
            }else{
                customAreaWindowSizeCombo.enabled = false
                customAreaWindowSize.opacity = 0.1
                customAreaWindowSizeCombo.opacity = 0.1
            }
        }
    }

    // get minimum , maximum and step size values for jpeg q, led brightness control
    function getMinMaxStepSizeValues(){        
        uvcExtCamAccessId.getLedBrightness(UvcExtcamera.UVC_GET_MIN)
        uvcExtCamAccessId.getLedBrightness(UvcExtcamera.UVC_GET_MAX)
        uvcExtCamAccessId.getLedBrightness(UvcExtcamera.UVC_GET_RES)

        uvcExtCamAccessId.getJpegQfactor(UvcExtcamera.UVC_GET_MIN)
        uvcExtCamAccessId.getJpegQfactor(UvcExtcamera.UVC_GET_MAX)
        uvcExtCamAccessId.getJpegQfactor(UvcExtcamera.UVC_GET_RES)
    }

    function customModeButtonClicked(){
        customAreaWindowSize.opacity = 1
        customAreaWindowSizeCombo.opacity = 1
        customAreaWindowSizeCombo.enabled = true
        uvcExtCamAccessId.setCustomAreaAutoFocus(width, height, width/2, height/2, customAreaWindowSizeCombo.currentText)
    }

    function centeredModeButtonClicked(){
        customAreaWindowSize.opacity = 0.1
        customAreaWindowSizeCombo.opacity = 0.1
        customAreaWindowSizeCombo.enabled = false
        uvcExtCamAccessId.setCenteredAutoFocusMode();
    }

    function extSettingsBasedOnAutoFocusSelectionInUVCSettings(autoFocusChecked){
        if(autoFocusChecked){
            afOff.checked = false
            manualLensPositionCombo.enabled = false
            manualLensPositionCombo.opacity = 0.1
            manualLensPositionText.opacity = 0.1
            afLock.enabled = true
            cAFScanRange.opacity = 1
            cafScanRangeCombo.enabled = true
            cafScanRangeCombo.opacity = 1
            focusModeText.opacity = 1
            radioContin.enabled = true
            radioOneshot.enabled = true
            radiocenter.enabled = true
            radiocustom.enabled = true            
            if(radiocustom.checked){
                customAreaWindowSize.opacity = 1
                customAreaWindowSizeCombo.opacity = 1
                customAreaWindowSizeCombo.enabled = true
            }
            else{
                customAreaWindowSize.opacity = 0.1
                customAreaWindowSizeCombo.opacity = 0.1
                customAreaWindowSizeCombo.enabled = false
            }

        }else{
            afOff.checked = true
            manualLensPositionCombo.enabled = true
            manualLensPositionCombo.opacity = 1
            manualLensPositionText.opacity = 1
            afLock.enabled = false
            cAFScanRange.opacity = 0.1
            cafScanRangeCombo.enabled = false
            cafScanRangeCombo.opacity = 0.1
            focusModeText.opacity = 0.1
            radioContin.enabled = false
            radioOneshot.enabled = false
            radiocenter.enabled = false
            radiocustom.enabled = false
            customAreaWindowSize.opacity = 0.1
            customAreaWindowSizeCombo.enabled = false
            customAreaWindowSizeCombo.opacity = 0.1
        }

    }

    function getValuesBasedOnExposureSelectionInUVCSettings(){
        uvcExtCamAccessId.getAELock(UvcExtcamera.UVC_GET_CUR)
        uvcExtCamAccessId.getAEMeterMode(UvcExtcamera.UVC_GET_CUR)
    }

    function setLedStatusValue(){
        // If led brightness slider value is 0 means off state. So when clicking "on", It must be set to some minimum value.
        if(ledBrightnessSlider.value == 0)
            ledBrightnessSlider.value = 5
        uvcExtCamAccessId.setLedBrightness(ledBrightnessSlider.value)
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
                wrapMode: Text.WordWrap
            }
        }
    }

    Component {
        id: radioButtonWordWrapStyle
        RadioButtonStyle {
            label: Text {
                width:70
                text: control.text
                font.pixelSize: 14
                font.family: "Ubuntu"
                color: "#ffffff"
                smooth: true
                opacity: 1
                wrapMode: Text.WordWrap
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
                implicitHeight: 20
                border.color: "#333"
                border.width: 2
                y: 1
            }
        }
    }
    Component.onCompleted: {
        uvcExtCamAccessId.initUVCExtensionUnit(root.vidstreamObj)
    }
    Component.onDestruction:{        
        uvcExtCamAccessId.deInitUVCExtensionUnit()        
    }
}

