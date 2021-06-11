import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import cameraenum 1.0
import econ.camera.ecam51_usb 1.0

Item
{

//    width:268
//    height:750
    property int selectedDevice;
    property bool skipUpdateUIAFStatus : false;

    Action
    {
         id: sensorID_action
         onTriggered:
         {
                getSensorID()
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
	onExtensionTabVisible:
        {
            if(visible)
            {
                ecam51USBId.rungetAFStatus()
            }
        }
    }
    ScrollView
    {
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle
        ColumnLayout
        {
            x:2
            y:5
            spacing:20
            Text
            {
                 id: focus_mode_title
                 text: "--- Focus Mode ---"
                 font.pixelSize: 14
                 font.family: "Ubuntu"
                 color: "#ffffff"
                 smooth: true
                 opacity: 0.50196078431373
                 Layout.alignment: Qt.AlignCenter
            }
            Grid
            {
                columns: 2
                spacing: 20

                ExclusiveGroup { id: afGroup }
                RadioButton
                {
                    id: manualFocus
                    style:  econRadioButtonStyle
                    text:   qsTr("Manual")
                    exclusiveGroup: afGroup
                    activeFocusOnPress: true
                    onClicked: {
                        ecam51USBId.selectFocusMode(Ecam51USB.MANUAL_FOCUS_SELECT)
                        maualFocus_grid.enabled = true
                        maualFocus_grid.opacity = 1
                        auto_focus_grid.enabled = false
                        auto_focus_grid.opacity = 0.1
                         ecam51USBId.getFocusPosition()
                      }
                    Keys.onReturnPressed:  {
                        ecam51USBId.selectFocusMode(Ecam51USB.MANUAL_FOCUS_SELECT)
                        maualFocus_grid.enabled = true
                        maualFocus_grid.opacity = 1
                        auto_focus_grid.enabled = false
                        auto_focus_grid.opacity = 0.1
                    }
                }
                RadioButton
                {
                    id: autoFocusTrigger
                    style:  econRadioButtonStyle
                    text:   qsTr("Single Trigger")
                    exclusiveGroup: afGroup
                    activeFocusOnPress: true
                    onClicked: {
                         ecam51USBId.selectFocusMode(Ecam51USB.SINGLE_TRIGGER_AUTO_FOCUS_SELECT)
                        auto_focus_grid.enabled = true
                        auto_focus_grid.opacity = 1
                        maualFocus_grid.enabled = false
                        maualFocus_grid.opacity = 0.1
                        autofocus_txt_field.text = ""
                      }
                    Keys.onReturnPressed:  {
                        ecam51USBId.selectFocusMode(Ecam51USB.SINGLE_TRIGGER_AUTO_FOCUS_SELECT)
                        auto_focus_grid.enabled = true
                        auto_focus_grid.opacity = 1
                        maualFocus_grid.enabled = false
                        maualFocus_grid.opacity = 0.1
                        autofocus_txt_field.text = ""
                    }
                }
            }
            Grid
            {
                id:auto_focus_grid
                spacing: 9
                rows:3
                Grid
                {
                    rows:2
                    spacing: 9
                    Button
                    {
                        id: autoFocus_trigger
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Trigger"
                        tooltip: "Click to give trigger"
                        style: econButtonStyle
                        onClicked:
                        {
                            ecam51USBId.selectFocusMode(Ecam51USB.SINGLE_TRIGGER_AUTO_FOCUS_SELECT)
                            autofocus_txt_field.text = ""
                        }

                        Keys.onReturnPressed:
                        {
                            ecam51USBId.selectFocusMode(Ecam51USB.SINGLE_TRIGGER_AUTO_FOCUS_SELECT)
                            autofocus_txt_field.text = ""
                        }
                    }
                    Button
                    {
                        id: autoFocus_getstatus
                        opacity: 1
                        activeFocusOnPress : true
                        text: "Get Status"
                        tooltip: "Click to get AF status"
                        style: econButtonStyle
                        onClicked:
                        {
                            ecam51USBId.rungetAFStatus()
                        }
                        Keys.onReturnPressed:
                        {
                            ecam51USBId.rungetAFStatus()
                        }
                    }
                    Text
                    {
                        id:af_status_title
                        text:"Status: "
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        color: "#ffffff"
                        smooth: true
                        Layout.alignment: Qt.AlignCenter
                        opacity: 0.50196078431373
                    }
                    TextField
                    {
                        id: autofocus_txt_field
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        opacity: 1
                        style: econTextFieldStyle
                        implicitHeight: 25
                        implicitWidth: 80

                    }
                }
            }
            Grid
            {
                id:maualFocus_grid
                rows:2
                spacing: 9
                Layout.alignment: Qt.AlignCenter
                Row
                {

                    spacing: 35
                    Slider
                    {
                        id: manualfocusSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: 0
                        maximumValue: 255
                        onValueChanged:
                        {
                            ecam51USBId.setManualFocusPosition(manualfocusSlider.value)
                            ecam51USBId.getFocusPosition()
                        }
                    }
                    TextField
                    {
                        id: manualfocusSlider_textfield
                        text: manualfocusSlider.value
                        font.pixelSize: 11
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: manualfocusSlider.minimumValue; top: manualfocusSlider.maximumValue}
                        onTextChanged:
                        {
//                            if(text.length > 0)
//                            {
//                                manualfocusSlider.value = manualfocusSlider.text
//                            }
                        }
                    }
                }
            }
                id:gpio_row
                Text
                {
                     id: gpio_status_title
                     text: "--- GPIO Status ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                     Layout.alignment: Qt.AlignCenter
                }
            Row
            {
                    Text
                    {
                         id: dio_status_text
                         text: " DIO : "
                         font.pixelSize: 14
                         font.family: "Ubuntu"
                         color: "#ffffff"
                         smooth: true
                         opacity: 0.50196078431373
                         Layout.alignment: Qt.AlignCenter
                    }

                    Text
                    {
                        id: dio_status_textfield
                        color: "#f5e3e3"
                        text: ""
                        font.pixelSize: 14
                        font.family: "Ubuntu"
                        font.bold: true
                        style: Text.Outline
                        smooth: true
                        opacity: 1
                        Layout.alignment: Qt.AlignCenter
                    }
            }
            Row
            {
                Text
                {
                     id: trigger_gpi_text
                     text: " Trigger GPI : "
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                     Layout.alignment: Qt.AlignCenter
                }

                Text
                {
                    id: trigger_gpi_textfiled
                    color: "#f5e3e3"
                    text: ""
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    font.bold: true
                    style: Text.Outline
                    smooth: true
                    opacity: 1
                    Layout.alignment: Qt.AlignCenter
                }
            }
            Row
            {
                Text
                {
                     id: fm_gpi_text
                     text: " F/M: HIGH : "
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                     Layout.alignment: Qt.AlignCenter
                }

                Text
                {
                    id: fm_gpi_textfiled
                    color: "#f5e3e3"
                    text: ""
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    font.bold: true
                    style: Text.Outline
                    smooth: true
                    opacity: 1
                    Layout.alignment: Qt.AlignCenter
                }
            }
            Button
            {
                id: gpio_update_status
                opacity: 1
                activeFocusOnPress : true
                text: "Update GPIO Status"
                tooltip: "Click to update status"
                style: econButtonStyle
                onClicked:
                {
                    ecam51USBId.getGPIStatus()
                }
                Keys.onReturnPressed:
                {
                    ecam51USBId.getGPIStatus()
                }
            }
            Text
            {
                 id: do_control_title
                 text: " --- Digital Output Controls --- "
                 font.pixelSize: 14
                 font.family: "Ubuntu"
                 color: "#ffffff"
                 smooth: true
                 opacity: 0.50196078431373
                 Layout.alignment: Qt.AlignCenter
            }
            Grid
            {
                rows :1
                spacing: 9
                Button
                {
                    id: set_do_pins
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Set"
                    tooltip: "Click to set DO pins"
                    style: econButtonStyle
                    onClicked:
                    {
                        ecam51USBId.controlGPO(1<<3,Ecam51USB.SET)
                    }
                    Keys.onReturnPressed:
                    {
                        ecam51USBId.controlGPO(1<<3,Ecam51USB.SET)
                    }
                }
                Button
                {
                    id: clear_do_pins
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Clear"
                    tooltip: "Click to clear DO pins"
                    style: econButtonStyle
                    onClicked:
                    {
                        ecam51USBId.controlGPO(1<<3,Ecam51USB.CLEAR)
                    }
                    Keys.onReturnPressed:
                    {
                        ecam51USBId.controlGPO(1<<3,Ecam51USB.CLEAR)
                    }
                }
            }

            Row
            {
                //Layout.alignment: Qt.AlignCenter
                spacing: 9
                Button
                {
                    id: f_wversion_selected
                    opacity: 1
                    activeFocusOnPress : true
                    text: "F/W Version"
                    tooltip: "Click to view the firmware version of the camera"
                    style: econButtonStyle
                    onClicked:
                    {
                        getFirmwareVer()
                    }
                    Keys.onReturnPressed:
                    {
                        getFirmwareVer()
                    }
                }//END of firmware Button
                Button
                {
                    id: sensorId
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Sensor ID"
                    tooltip: "Click to view the SensorID"
                    style: econButtonStyle
                    onClicked:
                    {
                        getSensorID()
                    }
                    Keys.onReturnPressed:
                    {
                        getSensorID()
                    }
                }

            }//END of Row
            Row
            {
                Button
                {
                    id:dummy_1
                    opacity: 0
                }
                Button
                {
                    id:dummy_2
                    opacity: 0
                }
            }
        }//END of ColumnLayout
    }//END of ScrollView
    Ecam51USB
    {
        id:ecam51USBId

        onTitleTextChanged:
        {
            displayMessageBox(qsTr(_title), qsTr(_text))
        }
        onAutoFocusStatus:
        {
            if(AFStatus===Ecam51USB.MANUAL_FOCUS_)
            {
                manualFocus.checked = true
                maualFocus_grid.enabled = true
                maualFocus_grid.opacity = 1
                auto_focus_grid.enabled = false
                auto_focus_grid.opacity = 0.1
            }
            else if((AFStatus === Ecam51USB.AF_SINGLE_TRIGGER_FOCUSED)||(AFStatus === Ecam51USB.AF_SINGLE_TRIGGER_FOCUSING))
            {
                autoFocusTrigger.checked = true
                auto_focus_grid.enabled = true
                auto_focus_grid.opacity = 1
                maualFocus_grid.enabled = false
                maualFocus_grid.opacity = 0.1
                if(skipUpdateUIAFStatus)
                {
                    autofocus_txt_field.text =(AFStatus === Ecam51USB.AF_SINGLE_TRIGGER_FOCUSED)?"Focused":"Focusing"
                }
                skipUpdateUIAFStatus = true;
            }
        }
        onManualFocusPosition:
        {
                manualfocusSlider.value= FocusPosition
        }
        onGpioStatus:
        {
            if((value&0x01) == 0x01)
            {
                dio_status_textfield.text = "HIGH"
                dio_status_textfield.color = "green"
            }
            else
            {
                dio_status_textfield.text = "LOW"
                dio_status_textfield.color = "red"
            }

            if((value&0x02)== 0x02)
            {
                trigger_gpi_textfiled.text = "HIGH"
                trigger_gpi_textfiled.color = "green"
            }
            else
            {
                trigger_gpi_textfiled.text = "LOW"
                trigger_gpi_textfiled.color = "red"
            }

            if((value & 0x04) == 0x04)
            {
                fm_gpi_textfiled.text = "HIGH"
                fm_gpi_textfiled.color = "green"
            }
            else
            {
                fm_gpi_textfiled.text = "LOW"
                fm_gpi_textfiled.color = "red"
            }
        }
    }
    function selectedDeviceEnumVal(selectedDeviceEnumValue) {
        selectedDevice = selectedDeviceEnumValue
    }

    function getSensorID()
    {
        ecam51USBId.getSensorId()
        messageDialog.open()
    }
    function getFirmwareVer()
    {
        if(selectedDevice == CommonEnums.ECAM51A_USB)
            ecam51USBId.readFirmwareVersion51aUSB()
        else
            ecam51USBId.readFirmwareVersion51bUSB()
    }
    function displayMessageBox(title, text)
    {
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }
    Component
    {
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
    Timer
    {
        id: getCamValuesTimer
        interval: 500
        onTriggered:
        {
            ecam51USBId.getGPIStatus()
            ecam51USBId.getFocusPosition()
            ecam51USBId.rungetAFStatus()
            stop()
        }
    }
    Component.onCompleted:
    {
        ecam51USBId.initUVCExtensionUnit(root.vidstreamObj)
        getCamValuesTimer.start()

    }
    Component.onDestruction:{
        ecam51USBId.deInitUVCExtensionUnit()
    }

}
