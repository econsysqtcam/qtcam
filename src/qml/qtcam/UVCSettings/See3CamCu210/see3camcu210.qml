import econ.camera.see3camcu210 1.0
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

    property bool skipUpdateUIQFactor           : false
    property bool skipUpdateUIOnBurstLength     : false
    property bool skipUpdateUIOnAntiFlickerMode :false
    property bool skipUpdateUIOnAWBLockStatus   :false
    property bool skipUpdateUIOnAELockStatus    :false
    property bool skipUpdateUIOnAWBMode    :false


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

    Action {
        id: ispVersion
        onTriggered:
        {
            getISPFirmwareVersion()
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
    }

    ScrollView{

        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 500
        style: econscrollViewStyle

        Item{
            height: 900

            ColumnLayout{
                x:2
                y:5
                spacing:20

                Text
                {
                    id: awbMode
                    text: "--- AWB Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ComboBox
                {
                    id: awbModeCombo
                    opacity: 1
                    enabled: true
                    model: ListModel {
                        ListElement { text: "Cloudy" }
                        ListElement { text: "Daylight" }
                        ListElement { text: "Flash" }
                        ListElement { text: "Cool white fluorescent" }
                        ListElement { text: "Tungsten" }
                        ListElement { text: "Candlelight" }
                        ListElement { text: "Horizon" }
                        ListElement { text: "Custom" }
                        ListElement { text: "Auto" }

                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {          
                        if(skipUpdateUIOnAWBMode)
                        {
                            setAwbMode()
                        }
                        skipUpdateUIOnAWBMode = true
                    }
                }

                Text{
                    id: awbLockStatusText
                    text: "--- AWB Lock Status ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                CheckBox {
                    id: awbLockCheckBox
                    enabled: true
                    opacity: 1
                    activeFocusOnPress : true
                    text: "AWB Lock"
                    style: econCheckBoxStyle
                    Layout.alignment: Qt.AlignCenter
                    onCheckedChanged: {
                        if(skipUpdateUIOnAWBLockStatus)
                        {
                            if(checked)
                            {
                                see3camcu210.setAwbLockStatus(See3CAM_CU210.AWB_ON)
                            }
                            else
                            {
                                see3camcu210.setAwbLockStatus(See3CAM_CU210.AWB_OFF)
                            }
                        }
                        skipUpdateUIOnAWBLockStatus = true
                    }
                }
                Text
                {
                    id: exposureMeteringMode
                    text: "--- AE Metering Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                ComboBox
                {
                    id: meteringModeCombo
                    opacity: 1
                    enabled: true
                    model: ListModel {
                        ListElement { text: "Auto Exposure Off" }
                        ListElement { text: "Centre-Weighted Average" }
                        ListElement { text: "All Block Integral" }
                        ListElement { text: "Small Area" }
                        ListElement { text: "Large Area" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        setExpMeteringMode()
                    }
                }

                Text{
                    id: aeLockStatusText
                    text: "--- AE Lock Status ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }
                CheckBox {
                    id: aeLockCheckBox
                    enabled: true
                    opacity: 1
                    activeFocusOnPress : true
                    text: "AE Lock"
                    style: econCheckBoxStyle
                    Layout.alignment: Qt.AlignCenter
                    onCheckedChanged: {
                        if(skipUpdateUIOnAELockStatus)
                        {
                            if(checked)
                            {
                                see3camcu210.setAeLockStatus(See3CAM_CU210.AE_ON)
                            }
                            else
                            {
                                see3camcu210.setAeLockStatus(See3CAM_CU210.AE_OFF)
                            }
                        }
                        skipUpdateUIOnAELockStatus = true
                    }
                }

                Text
                {
                    id: antiFlickerMode
                    text: "--- Flicker Mode ---"
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
                    model: ListModel
                           {
                                ListElement { text: "AUTO" }
                                ListElement { text: "50 Hz" }
                                ListElement { text: "60 Hz" }
                                ListElement { text: "DISABLE" }
                           }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnAntiFlickerMode){
                            setFlickerMode()
                        }
                        skipUpdateUIOnAntiFlickerMode = true
                    }
                }

                Text
                {
                    id: qFactorSliderTitle
                    text: "--- QFactor Slider ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row
                {
                    spacing: 35
                    Slider
                    {
                        id: qFactorSlider
                        activeFocusOnPress: true
                        updateValueWhileDragging: false
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        minimumValue: qFactorMin
                        maximumValue: qFactorMax
                        onValueChanged:
                        {
                            qFactorTextField.text = qFactorSlider.value
                            if(skipUpdateUIQFactor)
                            {
                                see3camcu210.setQFactorValue(qFactorSlider.value)
                            }
                            skipUpdateUIQFactor = true
                        }
                    }
                    TextField
                    {
                        id: qFactorTextField
                        text: qFactorSlider.value
                        font.pixelSize: 10
                        font.family: "Ubuntu"
                        smooth: true
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: qFactorSlider.minimumValue; top: qFactorSlider.maximumValue}
                        onTextChanged:
                        {
                            if(text.length > 0)
                            {
                                qFactorSlider.value = qFactorTextField.text
                            }
                        }
                    }
                }

                Text
                {
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
                        root.stillBurstLength(burstLengthCombo.currentIndex + 1) // combobox index starts from 0
                        if(skipUpdateUIOnBurstLength){
                            see3camcu210.setBurstLength(burstLengthCombo.currentText)
                        }
                        skipUpdateUIOnBurstLength = true
                    }
                }

                Text
                {
                    id: denoiseText
                    text: "--- De-Noise ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Grid
                {
                      columns: 2
                      spacing: 20
                      ExclusiveGroup { id: denoiseGroup }

                      RadioButton
                      {
                          exclusiveGroup: denoiseGroup
                          id: denoiseEnable
                          text: "Enable"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1

                          onClicked: {
                               see3camcu210.setDenoiseMode(See3CAM_CU210.ENABLE)
                          }
                          Keys.onReturnPressed: {
                              see3camcu210.setDenoiseMode(See3CAM_CU210.ENABLE)
                          }
                      }
                      RadioButton
                      {
                          exclusiveGroup: denoiseGroup
                          id: denoiseDisable
                          text: "Disable"
                          activeFocusOnPress: true
                          style: econRadioButtonStyle
                          opacity: enabled ? 1 : 0.1

                          onClicked: {
                              see3camcu210.setDenoiseMode(See3CAM_CU210.DISABLE)
                          }
                          Keys.onReturnPressed: {
                              see3camcu210.setDenoiseMode(See3CAM_CU210.DISABLE)
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
                   Layout.alignment: Qt.AlignCenter

                   Button {
                       id: ispFirmwareVersion
                       opacity: 1
                       action: ispVersion
                       text: "ISP Version"
                       activeFocusOnPress : true
                       tooltip: "Click to view the ISP firmware version"
                       style: econButtonStyle
                       Keys.onReturnPressed: {
                           see3camcu210.readISPFirmwareVersion()
                       }
                   }
               }

            }//Coloumn Layout

         } // Item

    }//ScrollView

   See3CAM_CU210{
     id:see3camcu210

     onAwbModeRecieved:{
         skipUpdateUIOnAWBMode = false
         currentAwbMode(awbMode)
         skipUpdateUIOnAWBMode = true
     }
     onAwbLockStatusReceived: {
         skipUpdateUIOnAWBLockStatus = false
         currentAwbLockStatus(awbLockStatus)
         skipUpdateUIOnAWBLockStatus = true
     }
     onMeteringModeReceived: {
         currentExpMeteringMode(meteringMode)
     }
     onAeLockStatusReceived: {
         skipUpdateUIOnAELockStatus = false
         currentAeLockStatus(aeLockStatus)
         skipUpdateUIOnAELockStatus = true
     }
     onBurstLengthReceived: {
         skipUpdateUIOnBurstLength = false
         burstLengthCombo.currentIndex = burstLength - 1
         skipUpdateUIOnBurstLength = true
     }
     onFlickerModeReceived: {
         skipUpdateUIOnAntiFlickerMode = false
         currentFlickerValue(flickerMode)
         skipUpdateUIOnAntiFlickerMode = true
     }
     onQFactorValueReceived: {
         skipUpdateUIQFactor = false
         qFactorSlider.value = Qfactor
         skipUpdateUIQFactor = true
     }
     onDenoiseModeReceived: {
         currentDenoiseMode(denoise)
     }
     onTitleTextChanged: {
         messageDialog.title = _title.toString()
         messageDialog.text = _text.toString()
         messageDialog.open()
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

    function currentAwbMode(mode)
    {
        switch(mode)
        {
            case See3CAM_CU210.CLOUDY:
                awbModeCombo.currentIndex = 0
                break
            case See3CAM_CU210.DAYLIGHT:
                awbModeCombo.currentIndex = 1
                break
            case See3CAM_CU210.FLASH:
                awbModeCombo.currentIndex = 2
                break
            case See3CAM_CU210.COOL_WHITE_FLUORESCENT:
                awbModeCombo.currentIndex = 3
                break
            case See3CAM_CU210.TUNGSTEN:
                awbModeCombo.currentIndex = 4
                break
            case See3CAM_CU210.CANDLE_LIGHT:
                awbModeCombo.currentIndex = 5
                break
            case See3CAM_CU210.HORIZON:
                awbModeCombo.currentIndex = 6
                break
            case See3CAM_CU210.CUSTOM:
                awbModeCombo.currentIndex = 7
                break
            case See3CAM_CU210.AUTO:
                awbModeCombo.currentIndex = 8
                break
         }
    }

    function setAwbMode()
    {
        if(awbModeCombo.currentText.toString() == "Cloudy")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.CLOUDY)
        }
        else if(awbModeCombo.currentText.toString() == "Daylight")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.DAYLIGHT)
        }
        else if(awbModeCombo.currentText.toString() == "Flash")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.FLASH)
        }
        else if(awbModeCombo.currentText.toString() == "Cool white fluorescent")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.COOL_WHITE_FLUORESCENT)
        }
        else if(awbModeCombo.currentText.toString() == "Tungsten")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.TUNGSTEN)
        }
        else if(awbModeCombo.currentText.toString() == "Candlelight")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.CANDLE_LIGHT)
        }
        else if(awbModeCombo.currentText.toString() == "Horizon")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.HORIZON)
        }
        else if(awbModeCombo.currentText.toString() == "Custom")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.CUSTOM)
        }
        else if(awbModeCombo.currentText.toString() == "Auto")
        {
            see3camcu210.setAwbMode(See3CAM_CU210.AUTO)
        }
    }

    function currentAwbLockStatus(mode){
        if(mode == See3CAM_CU210.AWB_ON)
        {
           awbLockCheckBox.checked = true
        }
        else if(mode == See3CAM_CU210.AWB_OFF)
        {
           awbLockCheckBox.checked = false
        }
    }

    function currentAeLockStatus(status)
    {
        if(status == See3CAM_CU210.AE_ON)
        {
            aeLockCheckBox.checked = true
        }
        else if(status == See3CAM_CU210.AE_OFF)
        {
            aeLockCheckBox.checked = false
        }
    }

    function currentExpMeteringMode(mode)
    {
        if(mode == See3CAM_CU210.AUTO_EXPOSURE_OFF)
        {
            meteringModeCombo.currentIndex = 0
        }
        else if(mode == See3CAM_CU210.AVERAGE)
        {
            meteringModeCombo.currentIndex = 1
        }
        else if(mode == See3CAM_CU210.INTEGRAL)
        {
            meteringModeCombo.currentIndex = 2
        }
        else if(mode == See3CAM_CU210.SMALL_AREA)
        {
            meteringModeCombo.currentIndex = 3
        }
        else if(mode == See3CAM_CU210.LARGE_AREA)
        {
            meteringModeCombo.currentIndex = 4
        }
    }

    function setExpMeteringMode()
    {
        if(meteringModeCombo.currentText.toString() == "Auto Exposure Off")
        {
            see3camcu210.setExposureMeteringMode(See3CAM_CU210.AUTO_EXPOSURE_OFF)
        }
        else if(meteringModeCombo.currentText.toString() == "Centre-Weighted average Mode")
        {
            see3camcu210.setExposureMeteringMode(See3CAM_CU210.AVERAGE)
        }
        else if(meteringModeCombo.currentText.toString() == "All Block Integral Mode")
        {
            see3camcu210.setExposureMeteringMode(See3CAM_CU210.INTEGRAL)
        }
        else if(meteringModeCombo.currentText.toString() == "Small Area Mode")
        {
            see3camcu210.setExposureMeteringMode(See3CAM_CU210.SMALL_AREA)
        }
        else if(meteringModeCombo.currentText.toString() == "Large Area Mode")
        {
            see3camcu210.setExposureMeteringMode(See3CAM_CU210.LARGE_AREA)
        }
    }

    function currentFlickerValue(flickerVal)
    {
        if(flickerVal == See3CAM_CU210.FLICKER_AUTO)
        {
            flickerModeCombo.currentIndex = 0
        }
        else if(flickerVal == See3CAM_CU210.FLICKER_50HZ)
        {
            flickerModeCombo.currentIndex = 1
        }
        else if(flickerVal == See3CAM_CU210.FLICKER_60HZ)
        {
            flickerModeCombo.currentIndex = 2
        }
        else if(flickerVal == See3CAM_CU210.FLICKER_DISABLE)
        {
            flickerModeCombo.currentIndex = 3
        }
    }

    function setFlickerMode()
    {
        if(flickerModeCombo.currentIndex === 0)
            see3camcu210.setFlickerMode(See3CAM_CU210.FLICKER_AUTO)
        else if(flickerModeCombo.currentIndex === 1)
            see3camcu210.setFlickerMode(See3CAM_CU210.FLICKER_50HZ)
        else if(flickerModeCombo.currentIndex === 2)
            see3camcu210.setFlickerMode(See3CAM_CU210.FLICKER_60HZ)
        else if(flickerModeCombo.currentIndex === 3)
            see3camcu210.setFlickerMode(See3CAM_CU210.FLICKER_DISABLE)
    }

    function currentDenoiseMode(denoiseVal)
    {
        if(denoiseVal == See3CAM_CU210.ENABLE)
        {
            denoiseEnable.checked = true
        }
        else if(denoiseVal == See3CAM_CU210.DISABLE)
        {
            denoiseDisable.checked = true
        }
    }

    function setToDefaultValues(){
        root.checkForTriggerMode(false)
        root.captureBtnEnable(true)
        root.videoRecordBtnEnable(true)
        see3camcu210.setToDefaultValues()
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
        see3camcu210.readISPFirmwareVersion()
        messageDialog.open()
    }

    function getValuesFromCamera(){
        see3camcu210.getAwbMode()
        see3camcu210.getAwbLockStatus()
        see3camcu210.getExposureMeteringMode()
        see3camcu210.getAeLockStatus()
        see3camcu210.getBurstLength()
        see3camcu210.getFlickerMode()
        see3camcu210.getQFactorValue()
        see3camcu210.getDenoiseMode()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}
