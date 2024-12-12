import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
//import econ.camera.see3cam_cu1330m 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    width:268
    height:750

    property bool skipUpdateUIOnExpWindowSize: false

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

    // Used when selecting auto exposure in image Quality settings menu
    Timer {
        id: getAutoExpsoureControlValues
        interval: 1000
        onTriggered: {
            see3camcu1330m.getAutoExpROIModeAndWindowSize()
            stop()
        }
    }

    Connections
    {
        target: root
        function onTakeScreenShot()
        {
           root.imageCapture(CommonEnums.SNAP_SHOT);
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
               see3camcu1330m.setROIAutoExposure(See3camCU1300M.AutoExpManual, width, height, x, y, autoExpoWinSizeCombo.currentText)
            }
        }
        function onAutoFocusSelected(autoFocusSelect){
            enableDisableAutoExposureControls(autoExposureSelect)
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
                        see3camcu1330m.setFlashState(See3camCU1300M.FLASHMODE_OFF)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu1330m.setFlashState(See3camCU1300M.FLASHMODE_OFF)
                    }
                }
                RadioButton {
                    id: rdoModeStrobe
                    style:  econRadioButtonStyle
                    text: qsTr("Strobe")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu1330m.setFlashState(See3camCU1300M.FLASHMODE_STROBE)
                    }
                    Keys.onReturnPressed: {
                        see3camcu1330m.setFlashState(See3camCU1300M.FLASHMODE_STROBE)
                    }
                }
                RadioButton {
                    id: rdoModeTorch
                    style:  econRadioButtonStyle
                    text: qsTr("Torch")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu1330m.setFlashState(See3camCU1300M.FLASHMODE_TORCH)
                    }
                    Keys.onReturnPressed: {
                        see3camcu1330m.setFlashState(See3camCU1300M.FLASHMODE_TORCH)
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
                        see3camcu1330m.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu1330m.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                }
                CheckBox {
                    id: flipVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3camcu1330m.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3camcu1330m.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
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
                        see3camcu1330m.setROIAutoExposure(See3camCU1300M.AutoExpFull, 0, 0, 0, 0, 0);
                        autoExpoWinSizeCombo.enabled = false
                    }
                    Keys.onReturnPressed: {
                        see3camcu1330m.setROIAutoExposure(See3camCU1300M.AutoExpFull, 0, 0, 0, 0, 0);
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
                        see3camcu1330m.setROIAutoExposure(See3camCU1300M.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
                        autoExpoWinSizeCombo.enabled = true
                    }
                    Keys.onReturnPressed: {
                        see3camcu1330m.setROIAutoExposure(See3camCU1300M.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText);
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
                        see3camcu1330m.setROIAutoExposure(See3camCU1300M.AutoExpManual, 0, 0, 0, 0, autoExpoWinSizeCombo.currentText)
                    }
                    skipUpdateUIOnExpWindowSize = true
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
            Row{
                Button {
                    id: dummy
                    opacity: 0
                }
            }
        }
    }
    See3camCU1330M{
        id:see3camcu1330m
        onStreamModeValue:{
            if(streamMode == See3camCU1300M.MODE_MASTER){
                rdoModeMaster.checked = true
            }else if(streamMode == See3camCU1300M.MODE_TRIGGER){
                rdoModeTrigger.checked = true
            }
        }

        onFlashModeValue:{
            if(flashMode == See3camCU1300M.FLASHMODE_OFF){
                rdoModeOff.checked = true
            }else if(flashMode == See3camCU1300M.FLASHMODE_STROBE){
                rdoModeStrobe.checked = true
            }else if(flashMode == See3camCU1300M.FLASHMODE_TORCH){
                rdoModeTorch.checked = true
            }
        }

        onFlipCtrlValue:{
            if(flipValue == See3camCU1300M.FLIP_HORIZONTAL){
                 flipHorizontal.checked = true
            }else if(flipValue == See3camCU1300M.FLIP_VERTICAL){
                flipVertical.checked = true
            }else if(flipValue ==See3camCU1300M.FLIP_BOTH){
               flipHorizontal.checked = true
               flipVertical.checked = true
            }else{
               flipHorizontal.checked = false
               flipVertical.checked = false
            }
        }

        onRoiAutoExpMode:{
            currentROIAutoExposureMode(roiMode, winSize)
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

    function getSerialNumber() {
        uvccamera.getSerialNumber()
        messageDialog.open()
    }

    function getFirmwareVersion() {
        uvccamera.getFirmWareVersion()
        messageDialog.open()
    }

    // current ROI auto exposure mode
    function currentROIAutoExposureMode(roiMode, winSize){
        switch(roiMode){
        case See3camCU1300M.AutoExpFull:
            autoexpFull.checked = true
            autoExpoWinSizeCombo.enabled = false
            break
        case See3camCU1300M.AutoExpManual:
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

    function getValuesFromCamera(){
        see3camcu1330m.getFlashState()
        see3camcu1330m.getStreamMode()
        see3camcu1330m.getFlipCtrlValue()
        see3camcu1330m.getAutoExpROIModeAndWindowSize()
    }
    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        see3camcu1330m.setToDefault()
        getValuesFromCamera()
    }
    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        see3camcu1330m.setStreamMode(See3camCU1300M.MODE_TRIGGER)
    }
    function setMasterMode(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.masterEnableForMonochrome()
        see3camcu1330m.setStreamMode(See3camCU1300M.MODE_MASTER)
    }
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
        }else{
            autoexpManual.enabled = false
            autoexpFull.enabled = false
            autoExpoWinSizeCombo.enabled = false
            autoexpManual.opacity = 0.1
            autoexpFull.opacity = 0.1
        }
        getAutoExpsoureControlValues.start()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
