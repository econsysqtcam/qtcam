import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.see3camcu55mh 1.0
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
                        see3camcu55_mh.setStreamMode(See3camcu55MH.MODE_MASTER)
                        root.masterEnableForMonochrome()
                    }
                    Keys.onReturnPressed:  {
                        see3camcu55_mh.setStreamMode(See3camcu55MH.MODE_MASTER)
                        root.masterEnableForMonochrome()
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
                        see3camcu55_mh.setFlashMode(See3camcu55MH.MODE_OFF)
                    }
                    Keys.onReturnPressed:  {
                        see3camcu55_mh.setFlashMode(See3camcu55MH.MODE_OFF)
                    }
                }
                RadioButton {
                    id: rdoModeStrobe
                    style:  econRadioButtonStyle
                    text: qsTr("Strobe")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu55_mh.setFlashMode(See3camcu55MH.MODE_STROBE)
                    }
                    Keys.onReturnPressed: {
                        see3camcu55_mh.setFlashMode(See3camcu55MH.MODE_STROBE)
                    }
                }
                RadioButton {
                    id: rdoModeTorch
                    style:  econRadioButtonStyle
                    text: qsTr("Torch")
                    exclusiveGroup: flashModeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        see3camcu55_mh.setFlashMode(See3camcu55MH.MODE_TORCH)
                    }
                    Keys.onReturnPressed: {
                        see3camcu55_mh.setFlashMode(See3camcu55MH.MODE_TORCH)
                    }
                }
            }
            Text {
                id: rollControlText
                text: "--- Roll ---"
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
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: 0
                    maximumValue: 3
                    onValueChanged:  {
                        rollTextField.text = rollSlider.value                  
                            see3camcu55_mh.setRollCtrlValue(rollSlider.value)
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
                        if(text.length >= 0){
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
            }
            Row{
                Button {
                    id: f_wversion_selectedCU55_MH
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
    See3camcu55MH{
        id:see3camcu55_mh
        onStreamModeValue:{
            if(streamMode == See3camcu55MH.MODE_MASTER){
                rdoModeMaster.checked = true
            }else if(streamMode == See3camcu55MH.MODE_TRIGGER){
                rdoModeTrigger.checked = true
            }
        }
        onFlashModeValue:{
            if(flashMode == See3camcu55MH.MODE_OFF){
                rdoModeOff.checked = true
            }else if(flashMode == See3camcu55MH.MODE_STROBE){
                rdoModeStrobe.checked = true
            }else if(flashMode == See3camcu55MH.MODE_TORCH){
                rdoModeTorch.checked = true
            }
        }

        onRollCtrlValue:{

            if(rollValue == See3camcu55MH.FLIP_NORMAL){
                rollSlider.value = rollValue
            }else if(rollValue == See3camcu55MH.FLIP_HORIZONTAL){
                 rollSlider.value = rollValue
            }else if(rollValue == See3camcu55MH.FLIP_VERTICAL){
                rollSlider.value = rollValue
            }else if(rollValue ==See3camcu55MH.FLIP_BOTH){
                 rollSlider.value = rollValue
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
        see3camcu55_mh.getFlashMode()
        see3camcu55_mh.getStreamMode()
        see3camcu55_mh.getRollCtrlValue()
    }
    function setToDefaultValues(){
        see3camcu55_mh.setToDefault()
        getValuesFromCamera()
    }
    function setTriggerMode(){
        root.checkForTriggerMode(true)
        root.captureBtnEnable(false)
        root.videoRecordBtnEnable(false)
        see3camcu55_mh.setStreamMode(See3camcu55MH.MODE_TRIGGER)
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
    Component.onDestruction:{
         // While quitting the camera, set it to continuous mode
         see3camcu55_mh.setStreamMode(See3camcu55MH.MODE_MASTER)
    }
}
