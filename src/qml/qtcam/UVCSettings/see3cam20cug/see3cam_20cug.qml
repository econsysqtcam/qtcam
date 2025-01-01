import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3cam20cug 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {

    width:240
    height:720

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
                        root.stopUpdatePreviewInTriggerMode()
                        setTriggerMode()
                    }
                    Keys.onReturnPressed: {
                        root.checkForTriggerMode(true)
                        root.stopUpdatePreviewInTriggerMode()
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
                        see3cam20cug.setFlashMode(See3cam20cug.MODE_OFF)
                    }
                    Keys.onReturnPressed:  {
                        see3cam20cug.setFlashMode(See3cam20cug.MODE_OFF)
                    }
                }
                RadioButton {
                    id: rdoModeOn
                    style:  econRadioButtonStyle
                    text: qsTr("ON")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3cam20cug.setFlashMode(See3cam20cug.MODE_ON)
                    }
                    Keys.onReturnPressed: {
                        see3cam20cug.setFlashMode(See3cam20cug.MODE_ON)
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
                        see3cam20cug.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3cam20cug.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                }
                CheckBox {
                    id: flipVertical
                    activeFocusOnPress : true
                    text: "Vertical"
                    style: econCheckBoxStyle
                    onClicked:{
                        see3cam20cug.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
                    }
                    Keys.onReturnPressed: {
                        see3cam20cug.setFlipCtrlValue(flipHorizontal.checked,flipVertical.checked)
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
                    id: f_wversion_selected20CUG_MH
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
                            source: "images/images/f_wversion_selected.png"
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
                            source: "images/images/serial_no_selected.png"
                        }
                    }
                    Keys.onReturnPressed: {
                        getSerialNumber()
                    }
                }
            }
        }
    }
    See3cam20cug{
        id:see3cam20cug
        onStreamModeValue:{
            if(streamMode == See3cam20cug.MODE_MASTER){
                rdoModeMaster.checked = true
                root.checkForTriggerMode(false)
                root.captureBtnEnable(true)
                root.videoRecordBtnEnable(true)
                root.startUpdatePreviewInMasterMode()
            }else if(streamMode == See3cam20cug.MODE_TRIGGER){
                rdoModeTrigger.checked = true
                root.videoRecordBtnEnable(false)
                root.captureBtnEnable(false)
                root.checkForTriggerMode(true)
                root.stopUpdatePreviewInTriggerMode()
            }
        }
        onFlashModeValue:{
            if(flashMode == See3cam20cug.MODE_OFF){
                rdoModeOff.checked = true
            }else if(flashMode == See3cam20cug.MODE_ON){
                rdoModeOn.checked = true
            }
        }

        onFlipCtrlValue:{

            if(flipValue == See3cam20cug.FLIP_NORMAL){
                flipHorizontal.checked = false
                flipVertical.checked = false
            }else if(flipValue == See3cam20cug.FLIP_HORIZONTAL){
                 flipHorizontal.checked = true
            }else if(flipValue == See3cam20cug.FLIP_VERTICAL){
                flipVertical.checked = true
            }else if(flipValue ==See3cam20cug.FLIP_BOTH){
               flipHorizontal.checked = true
               flipVertical.checked = true
            }
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
                source: "images/images/scroller.png"
            }
            scrollBarBackground: Image {
                id: scrollStyle
                source: "images/images/Scroller_bg.png"
            }
            incrementControl: Image {
                id: increment
                source: "images/images/down_arrow.png"
            }
            decrementControl: Image {
                id: decrement
                source: "images/images/up_arrow.png"
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
    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }
    function getValuesFromCamera(){
        see3cam20cug.getFlashMode()
        see3cam20cug.getStreamMode()
        see3cam20cug.getFlipCtrlValue()
    }
    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        see3cam20cug.setToDefault()
        getValuesFromCamera()
    }
    function setTriggerMode(){
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        root.checkForTriggerMode(true)
        root.stopUpdatePreviewInTriggerMode()
        see3cam20cug.setStreamMode(See3cam20cug.MODE_TRIGGER)
    }
    function setMasterMode(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        root.startUpdatePreviewInMasterMode()
        see3cam20cug.setStreamMode(See3cam20cug.MODE_MASTER)
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
    Component.onDestruction:{
         // While quitting the camera, set it to continuous mode
         see3cam20cug.setStreamMode(See3cam20cug.MODE_STREAM)
    }
}

