import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0
import econ.camera.see3cam160 1.0
import "../../JavaScriptFiles/tempValue.js" as JS

Item {
    width:268
    height:750

    property int qFactorMin: 0
    property int qFactorMax: 100
    property int flickerCtrl
    property int aeMeteringMode
    property int  manualLensPosMode
    property int  cafScanRangeMode
    property int awbPresetAutoMode: 8
    property int afLockON : 0x01
    // Flags to prevent setting values in camera when getting the values from camera
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIFlickerMode:false
    property bool skipUpdateUIAWbPreset: false
    property bool skipUpdateUIAEMeterMode: false
    property bool skipUpdateUICAFScanRangeMode: false
    property bool skipUpdateManualLensPositionMode: false
    property bool skipUpdateUIOnBurstLength: false

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
        id: ispfirmwareVersion
        onTriggered:
        {
            getISPFirmwareVersion()
        }
    }

    Action {
        id: serialNumber
        onTriggered: {
            getSerialNumber()
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

        onAutoWhiteBalanceSelected:{

        }

        onAutoFocusSelected:{
            extSettingsBasedOnAutoFocusSelectionInUVCSettings(JS.autoFocusChecked)
        }

        onExtensionTabVisible:{
            if(visible)
            {
                getValuesFromCamera()
                extSettingsBasedOnAutoFocusSelectionInUVCSettings(JS.autoFocusChecked) // Initially get the auto focus mode in UVC settings and enable/disable controls in extension unit
            }
        }

    }

    ScrollView {
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle
        Item {
            height:1300
            ColumnLayout {
                x:2
                y:5
                spacing:20

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
                    enabled: true
                    opacity: 1
                    activeFocusOnPress : true
                    text: "AWB Lock"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3cam160.setAWBlockstatus(awbLock.checked)
                    }
                    Keys.onReturnPressed: {
                        see3cam160.setAWBlockstatus(awbLock.checked)
                    }
                    onCheckedChanged: {
                        if(checked)
                        {
                            awbPresetsCombo.enabled = false
                            awbPresetsCombo.opacity = 0.1
                        }
                        else
                        {
                            awbPresetsCombo.enabled = true
                            awbPresetsCombo.opacity = 1
                        }
                    }
                }
                ComboBox
                {
                    id: awbPresetsCombo
                    opacity: awbLock.checked ? 0.1 : 1
                    enabled: true
                    model: ListModel {
                        ListElement { text: "Cloudy" }
                        ListElement { text: "DayLight" }
                        ListElement { text: "Flash" }
                        ListElement { text: "Cool White Fluorescent" }
                        ListElement { text: "Tungsten" }
                        ListElement { text: "CandleLight" }
                        ListElement { text: "Horizon" }
                        ListElement { text: "Custom" }
                        ListElement { text: "Auto" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIAWbPreset){
                            // combo index starts from 0. AWB preset values start from 1.  So Add 1 to set the AWB preset mode
                            see3cam160.setAWBpresetMode(awbPresetsCombo.currentIndex + 1)
                            if(awbPresetsCombo.currentIndex == awbPresetAutoMode)               //Auto mode
                                root.disableAwb(true)
                            else
                                root.disableAwb(false)

                        }
                        skipUpdateUIAWbPreset = true
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
                        ListElement { text: "INF position" }
                        ListElement { text: "Normal Position" }
                        ListElement { text: "Bar code position" }
                        ListElement { text: "Macro position" }
                        ListElement { text: "Initial DAC position"}
                        ListElement { text: "Custom Position" }
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
                        see3cam160.setAFlockstatus(afLock.checked)
                    }
                    Keys.onReturnPressed:{
                        see3cam160.setAFlockstatus(afLock.checked)
                    }
                    onCheckedChanged: {
                        if(checked)
                        {
                            cafScanRangeCombo.enabled = false
                            cafScanRangeCombo.opacity = 0.1
                        }
                        else
                        {
                            cafScanRangeCombo.enabled = true
                            cafScanRangeCombo.opacity = 1
                        }
                    }
                }

                Text {
                    id: cAFScanRange
                    text: "CAF Scan Range:"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    opacity: afLock.checked ? 0.1 : 1
                    enabled: true
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
                        if(skipUpdateUICAFScanRangeMode){
                            setCAFScanRangeModeFn()
                        }
                        skipUpdateUICAFScanRangeMode = true
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
                            see3cam160.setAutoFocusMode(See3cam160.AF_CONTINUOUS)
                            see3cam160.getAFlockstatus()
                        }
                        Keys.onReturnPressed: {
                            see3cam160.setAutoFocusMode(See3cam160.AF_CONTINUOUS)
                            see3cam160.getAFlockstatus()
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
                            see3cam160.setAutoFocusMode(See3cam160.AF_ONESHOT)
                            see3cam160.getAFlockstatus()
                        }
                        Keys.onReturnPressed: {
                            see3cam160.setAutoFocusMode(See3cam160.AF_ONESHOT)
                            see3cam160.getAFlockstatus()
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
                                see3cam160.setQFactor(qFactorSlider.value)
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
                        see3cam160.setAElockstatus(aeLock.checked)
                    }
                    Keys.onReturnPressed: {
                        see3cam160.setAElockstatus(aeLock.checked)
                    }
                    onCheckedChanged: {
                        if(checked)
                        {
                            aeMeteringModeCombo.enabled = false
                            aeMeteringModeCombo.opacity = 0.1
                        }
                        else
                        {
                            aeMeteringModeCombo.enabled = true
                            aeMeteringModeCombo.opacity = 1
                        }
                    }
                }
                ComboBox
                {
                    id: aeMeteringModeCombo
                    opacity: aeLock.checked ? 0.1 : 1
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
                        if(skipUpdateUIAEMeterMode){
                            setAutoExpMeteringMode()
                        }
                        skipUpdateUIAEMeterMode = true
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
                            see3cam160.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
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
                        if(skipUpdateUIFlickerMode){
                            setFlickerDetectionFn();
                        }
                        skipUpdateUIFlickerMode = true
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
                    Button{
                            id: dummy
                            opacity: 0
                    }
                    Button {
                        id:serial_no_selected
                        opacity: 1
                        activeFocusOnPress : true
                        text: "SerialNo"
                        action: serialNumber
                        tooltip: "Click to view the Serial Number"
                        style: econButtonStyle
                        Keys.onReturnPressed: {
                            getSerialNumber()
                        }
                    }
                }

                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: f_wversion_selected160
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
                        id: isp_f_wversion_selected160
                        opacity: 1
                        action: ispfirmwareVersion
                        activeFocusOnPress : true
                        tooltip: "Click to view the isp firmware version of the camera"

                        style: ButtonStyle {
                            background: Rectangle {
                                implicitHeight: 44
                                implicitWidth: 104
                                border.width: 3
                                color: "#e76943"
                                border.color: control.activeFocus ? "#ffffff" : "#222021"
                                radius: 5
                            }
                            label: Text {
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font.family: "Ubuntu"
                                font.pointSize: 9
                                text: qsTr("ISP F/W Version")
                            }
                        }
                        Keys.onReturnPressed: {
                            getISPFirmwareVersion()
                        }
                    }
                }

            }
        }
    }


    function afOffSelection(){
        if(afOff.checked){
            focusModeText.opacity = 0.1
            radioContin.enabled = false
            radioOneshot.enabled = false
            manualLensPositionCombo.enabled = true
            manualLensPositionCombo.opacity = 1
            manualLensPositionText.opacity = 1
            afLock.enabled = false
            cafScanRangeCombo.enabled = false
            cafScanRangeCombo.opacity = 0.1
            cAFScanRange.opacity = 0.1
            see3cam160.setAFstatus(See3cam160.AF_ON)
            root.disableAutoFocus(false)
        }
        else{
            focusModeText.opacity = 1
            radioContin.enabled = true
            radioOneshot.enabled = true
            manualLensPositionCombo.enabled = false
            manualLensPositionCombo.opacity = 0.1
            manualLensPositionText.opacity = 0.1
            afLock.enabled = true
            cafScanRangeCombo.enabled = true
            cafScanRangeCombo.opacity = 1
            cAFScanRange.opacity = 1
            see3cam160.setAFstatus(See3cam160.AF_OFF)
            root.disableAutoFocus(true)
        }
	see3cam160.getAFlockstatus()
    }

    function setToDefaultValues(){
        see3cam160.setToDefault()
        getValuesFromCamera()
        extSettingsBasedOnAutoFocusSelectionInUVCSettings(JS.autoFocusChecked)
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }
    function getISPFirmwareVersion() {
        see3cam160.getFirmwareVersion()
        messageDialog.open()
    }
    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }


    function setFlickerDetectionFn()
    {
        switch(flickercombo.currentIndex){
        case 0:
            flickerCtrl = See3cam160.FLICKER_AUTO_DETECTION
            break
        case 1:
            flickerCtrl = See3cam160.FLICKER_50HZ
            break
        case 2:
            flickerCtrl = See3cam160.FLICKER_60HZ
            break
        case 3:
            flickerCtrl = See3cam160.FLICKER_OFF
            break
        }
        see3cam160.setFlickerMode(flickerCtrl)
    }

    // set Auto Exposure metering mode
    function setAutoExpMeteringMode(){
        switch(aeMeteringModeCombo.currentIndex){
        case 0:
            aeMeteringMode = See3cam160.AE_OFF
            break
        case 1:
            aeMeteringMode = See3cam160.AE_CENTER_WEIGHTED_AVG
            break
        case 2:
            aeMeteringMode = See3cam160.AE_ALL_BLOCK_INTEGRAL
            break
        case 3:
            aeMeteringMode = See3cam160.AE_SMALL_AREA
            break
        case 4:
            aeMeteringMode = See3cam160.AE_LARGE_AREA
            break
        }
        if(aeMeteringMode == See3cam160.AE_OFF)
            root.disableManualExp(false)
        else
            root.disableManualExp(true)

        see3cam160.setAEMeterMode(aeMeteringMode)
    }

    function setManualLensFocusPosMode(){
        switch(manualLensPositionCombo.currentIndex){
        case 0:
            manualLensPosMode = See3cam160.LENS_POS_INF
            break
        case 1:
            manualLensPosMode = See3cam160.LENS_POS_NORMAL
            break
        case 2:
            manualLensPosMode = See3cam160.LENS_BARCODE_POS
            break
        case 3:
            manualLensPosMode = See3cam160.LENS_MACRO_POS
            break
        case 4:
            manualLensPosMode = See3cam160.LENS_INITIAL_DAC_POS
            break
        case 5:
            manualLensPosMode = See3cam160.LENS_CUSTOM_POS
        }

        see3cam160.setManualLensPositionMode(manualLensPosMode)
    }


    function setCAFScanRangeModeFn(){
        switch(cafScanRangeCombo.currentIndex){
        case 0:
            cafScanRangeMode = See3cam160.CAF_MACRO_RANGE_MODE
            break
        case 1:
            cafScanRangeMode = See3cam160.CAF_NORMAL_RANGE_MODE
            break
        case 2:
            cafScanRangeMode = See3cam160.CAF_FULL_RANGE_MODE
            break
        case 3:
            cafScanRangeMode = See3cam160.CAF_INFINITY_RANGE_MODE
            break
        case 4:
            cafScanRangeMode = See3cam160.CAF_HYPERFOCAL_RANGE_MODE
            break
        }

        see3cam160.setCAFScanRangeMode(cafScanRangeMode)
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
        }
    }

    function getValuesFromCamera(){
        see3cam160.getAutoFocusMode()
        see3cam160.getAWBpresetMode()
        see3cam160.getAWBlockstatus()
        see3cam160.getAEMeterMode()
        see3cam160.getAElockstatus()
        see3cam160.getFlickerMode()
        see3cam160.getQFactor()
        see3cam160.getCAFScanRangeMode()
        see3cam160.getAFlockstatus()
        see3cam160.getManualLensPositionMode()
        see3cam160.getAFstatus()
        see3cam160.getBurstLength()
    }

    function getSerialNumber() {
        uvccamera.getUniqueId()
        messageDialog.open()
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

    Component {
        id: econcx3ButtonStyle
        ButtonStyle {
            background: Rectangle {
                implicitHeight: 38
                implicitWidth: 104
                border.width: 3
                color: "#e76943"
                border.color: "#222021"
                radius: 5
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

    See3cam160
    {
        id:see3cam160
        onAfModeChanged: {
            switch(afMode){
            case See3cam160.AF_CONTINUOUS:
                radioContin.checked = true
                break
            case See3cam160.AF_ONESHOT:
                radioOneshot.checked = true
                break
            }
        }

        onTitleTextChanged: {
            displayMessageBox(qsTr(_title), qsTr(_text))
        }

        onQFactorValue:{
            skipUpdateUIQFactor = false
            qFactorSlider.value = qFactor
            skipUpdateUIQFactor = true
        }

        onFlickerModeChanged: {
            skipUpdateUIFlickerMode = false
            if(flickerMode == See3cam160.FLICKER_AUTO_DETECTION){
                flickercombo.currentIndex = 0
            }else if(flickerMode == See3cam160.FLICKER_50HZ){
                flickercombo.currentIndex  = 1
            }else if(flickerMode == See3cam160.FLICKER_60HZ){
                flickercombo.currentIndex  = 2
            }else if(flickerMode == See3cam160.FLICKER_OFF){
                flickercombo.currentIndex  = 3
            }else{ }
            skipUpdateUIFlickerMode = true;
        }

        onAeLockStatusChanged: {
            if(!lockStatus)
            {
                aeLock.checked = false
            }
            else
            {
                aeLock.checked = true
            }
        }

        onAeMeterModeChanged: {
            skipUpdateUIAEMeterMode = false
            switch(meterMode){
            case See3cam160.AE_OFF:
                aeMeteringModeCombo.currentIndex = 0
                break
            case See3cam160.AE_CENTER_WEIGHTED_AVG:
                aeMeteringModeCombo.currentIndex = 1
                break
            case See3cam160.AE_ALL_BLOCK_INTEGRAL:
                aeMeteringModeCombo.currentIndex = 2
                break
            case See3cam160.AE_SMALL_AREA:
                aeMeteringModeCombo.currentIndex = 3
                break
            case See3cam160.AE_LARGE_AREA:
                aeMeteringModeCombo.currentIndex = 4
                break
            }
            if(aeMeteringModeCombo.currentIndex == 0)
                root.disableManualExp(false)
            else
                root.disableManualExp(true)
            skipUpdateUIAEMeterMode = true
       }

        onAfLockStatusChanged: {
            if(lockStatus==afLockON)
            {
                afLock.checked = true
            }
            else
            {
                afLock.checked = false
            }
        }

        onCafScanRangeModeChanged: {
            skipUpdateUICAFScanRangeMode = false
            switch(scanRangeMode){
            case See3cam160.CAF_MACRO_RANGE_MODE:
                cafScanRangeCombo.currentIndex = 0
                break
            case See3cam160.CAF_NORMAL_RANGE_MODE:
                cafScanRangeCombo.currentIndex = 1
                break
            case See3cam160.CAF_FULL_RANGE_MODE:
                cafScanRangeCombo.currentIndex = 2
                break
            case See3cam160.CAF_INFINITY_RANGE_MODE:
                cafScanRangeCombo.currentIndex = 3
                break
            case See3cam160.CAF_HYPERFOCAL_RANGE_MODE:
                cafScanRangeCombo.currentIndex = 4
                break
            }
            skipUpdateUICAFScanRangeMode = true
        }

        onAfStatusChanged: {
            if(afstatus)
            {
                afOff.checked = true
                root.disableAutoFocus(true)
            }
            else
            {
                afOff.checked = false
                root.disableAutoFocus(false)
            }
        }

        onManualLensPositionModeChanged: {
            skipUpdateManualLensPositionMode = false
            switch(lensPosMode){
            case See3cam160.LENS_POS_INF:
                manualLensPositionCombo.currentIndex = 0
                break
            case See3cam160.LENS_POS_NORMAL:
                manualLensPositionCombo.currentIndex = 1
                break
            case See3cam160.LENS_BARCODE_POS:
                manualLensPositionCombo.currentIndex = 2
                break
            case See3cam160.LENS_MACRO_POS:
                manualLensPositionCombo.currentIndex = 3
                break
            case See3cam160.LENS_INITIAL_DAC_POS:
                manualLensPositionCombo.currentIndex = 4
                break
            case See3cam160.LENS_CUSTOM_POS:
                manualLensPositionCombo.currentIndex = 5
                break
            }
            skipUpdateManualLensPositionMode = true
        }

        onAwbPresetModeChanged: {
            skipUpdateUIAWbPreset = false
            awbPresetsCombo.currentIndex = awbMode - 1
            if(awbPresetsCombo.currentIndex == awbPresetAutoMode)               //Auto mode
                root.disableAwb(true)
            else
                root.disableAwb(false)
            skipUpdateUIAWbPreset = true
        }

        onAwbLockStatusChanged: {
            if(!lockStatus)
            {
                awbLock.checked = false
            }
            else
            {
                awbLock.checked = true
            }
        }

        onBurstLengthValue:{
            skipUpdateUIOnBurstLength = false
            burstLengthCombo.currentIndex = burstLength - 1
            skipUpdateUIOnBurstLength = true
        }

    }

    Uvccamera {
        id: uvccamera
        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }
    }
}
