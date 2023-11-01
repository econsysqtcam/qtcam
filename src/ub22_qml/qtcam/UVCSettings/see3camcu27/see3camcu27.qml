import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0
import econ.camera.see3camcu27 1.0

Item {
    width:268
    height:500

    property int qFactorMin: 0
    property int qFactorMax: 100
    property int rollMin: 0
    property int rollMax: 180
    property int flickerCtrl
    property int aeMeteringMode
    property int awbPresetAutoMode: 8
    // Flags to prevent setting values in camera when getting the values from camera
    property bool skipUpdateUIQFactor : false
    property bool skipUpdateUIRoll : false
    property bool skipUpdateUIFlickerMode:false
    property bool skipUpdateUIAWbPreset: false
    property bool skipUpdateUIAEMeterMode: false
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
        function onTakeScreenShot()
        {
            root.imageCapture(CommonEnums.BURST_SHOT);
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

        function onExtensionTabVisible(visible){
            if(visible)
            {
                getValuesFromCamera()
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
                        see3camcu27.setAWBlockstatus(awbLock.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu27.setAWBlockstatus(awbLock.checked)
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
                            see3camcu27.setAWBpresetMode(awbPresetsCombo.currentIndex + 1)
                            if(awbPresetsCombo.currentIndex == awbPresetAutoMode)               //Auto mode
                                root.disableAwb(true)
                            else
                                root.disableAwb(false)

                        }
                        skipUpdateUIAWbPreset = true
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
                                see3camcu27.setQFactor(qFactorSlider.value)
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
                        see3camcu27.setAElockstatus(aeLock.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu27.setAElockstatus(aeLock.checked)
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
                            see3camcu27.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }

                Text {
                    id: denoiseControlText
                    text: "--- Denoise Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                Row{
                    spacing: 80
                    ExclusiveGroup { id: denoiseControlGroup }
                    RadioButton {
                        id: denoiseEnable
                        style:  econRadioButtonStyle
                        text:   qsTr("Enable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu27.setDenoiseCtrlMode(See3camCU27.DenoiseEnable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu27.setDenoiseCtrlMode(See3camCU27.DenoiseEnable)
                        }
                    }
                    RadioButton {
                        id: denoiseDisable
                        style:  econRadioButtonStyle
                        text: qsTr("Disable")
                        exclusiveGroup: denoiseControlGroup
                        activeFocusOnPress: true
                        onClicked: {
                            see3camcu27.setDenoiseCtrlMode(See3camCU27.DenoiseDisable)
                        }
                        Keys.onReturnPressed: {
                            see3camcu27.setDenoiseCtrlMode(See3camCU27.DenoiseDisable)
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
                        if(skipUpdateUIFlickerMode){
                            setFlickerDetectionFn();
                        }
                        skipUpdateUIFlickerMode = true
                    }
                }

                Text {
                    id: rollValueText
                    text: "--- Roll Value ---"
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
                        id: rollSlider
                        width: 150
                        stepSize: 180
                        style:econSliderStyle
                        minimumValue: rollMin
                        maximumValue: rollMax
                        onValueChanged:  {
                            rollTextField.text = rollSlider.value
                            if(skipUpdateUIRoll){
                                see3camcu27.setRollValue(rollSlider.value)
                            }
                            skipUpdateUIRoll = true
                        }
                    }
                    TextField {
                        id: rollTextField
                        text: rollSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: rollSlider.minimumValue; top: rollSlider.maximumValue}
                        onTextChanged: {
                            if(text.length > 0){
                                rollSlider.value = rollTextField.text
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
                        id: f_wversion_selectedCU27
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
                        id: isp_f_wversion_selectedCU27
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

    function setToDefaultValues(){
        see3camcu27.setToDefault()
        getValuesFromCamera()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }
    function getISPFirmwareVersion() {
        see3camcu27.getFirmwareVersion()
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
            flickerCtrl = See3camCU27.FLICKER_AUTO_DETECTION
            break
        case 1:
            flickerCtrl = See3camCU27.FLICKER_50HZ
            break
        case 2:
            flickerCtrl = See3camCU27.FLICKER_60HZ
            break
        case 3:
            flickerCtrl = See3camCU27.FLICKER_OFF
            break
        }
        see3camcu27.setFlickerMode(flickerCtrl)
    }

    // set Auto Exposure metering mode
    function setAutoExpMeteringMode(){
        switch(aeMeteringModeCombo.currentIndex){
        case 0:
            aeMeteringMode = See3camCU27.AE_OFF
            break
        case 1:
            aeMeteringMode = See3camCU27.AE_CENTER_WEIGHTED_AVG
            break
        case 2:
            aeMeteringMode = See3camCU27.AE_ALL_BLOCK_INTEGRAL
            break
        case 3:
            aeMeteringMode = See3camCU27.AE_SMALL_AREA
            break
        case 4:
            aeMeteringMode = See3camCU27.AE_LARGE_AREA
            break
        }
        if(aeMeteringMode == See3camCU27.AE_OFF)
            root.disableManualExp(false)
        else
            root.disableManualExp(true)

        see3camcu27.setAEMeterMode(aeMeteringMode)
    }

    function getValuesFromCamera(){
        see3camcu27.getAEMeterMode()
        see3camcu27.getAWBpresetMode()
        see3camcu27.getAWBlockstatus()
        see3camcu27.getAElockstatus()
        see3camcu27.getFlickerMode()
        see3camcu27.getQFactor()
        see3camcu27.getRollValue()
        see3camcu27.getBurstLength()
        see3camcu27.getDenoiseCtrlMode()
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

    See3camCU27
    {
        id:see3camcu27

        onTitleTextChanged: {
            displayMessageBox(qsTr(_title), qsTr(_text))
        }

        onDenoiseModeChanged:{
            if(denoiseMode == See3camCU27.DenoiseEnable){
                denoiseEnable.checked = true
            }else if(denoiseMode == See3camCU27.DenoiseDisable){
                denoiseDisable.checked = true
            }
        }

        onQFactorValue:{
            skipUpdateUIQFactor = false
            qFactorSlider.value = qFactor
            skipUpdateUIQFactor = true
        }

        onRollValue:{
            skipUpdateUIRoll = false
            rollSlider.value = roll
            skipUpdateUIRoll = true
        }

        onFlickerModeChanged: {
            skipUpdateUIFlickerMode = false
            if(flickerMode == See3camCU27.FLICKER_AUTO_DETECTION){
                flickercombo.currentIndex = 0
            }else if(flickerMode == See3camCU27.FLICKER_50HZ){
                flickercombo.currentIndex  = 1
            }else if(flickerMode == See3camCU27.FLICKER_60HZ){
                flickercombo.currentIndex  = 2
            }else if(flickerMode == See3camCU27.FLICKER_OFF){
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
            case See3camCU27.AE_OFF:
                aeMeteringModeCombo.currentIndex = 0
                break
            case See3camCU27.AE_CENTER_WEIGHTED_AVG:
                aeMeteringModeCombo.currentIndex = 1
                break
            case See3camCU27.AE_ALL_BLOCK_INTEGRAL:
                aeMeteringModeCombo.currentIndex = 2
                break
            case See3camCU27.AE_SMALL_AREA:
                aeMeteringModeCombo.currentIndex = 3
                break
            case See3camCU27.AE_LARGE_AREA:
                aeMeteringModeCombo.currentIndex = 4
                break
            }
            if(aeMeteringModeCombo.currentIndex == 0)
                root.disableManualExp(false)
            else
                root.disableManualExp(true)
            skipUpdateUIAEMeterMode = true
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
