import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.uvcsettings 1.0
import econ.camera.barcode_camera 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {

    width:240
    height:720

    property int gainMin: 0
    property int gainMax: 4
    property int expMin: 0
    property int expMax: 65535

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
                    id: modes
                    text: "--- Modes ---"
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
                ExclusiveGroup { id: modeGroup }
                RadioButton {
                    id: uart
                    style:  econRadioButtonStyle
                    text:   qsTr("UART")
                    exclusiveGroup: modeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setUartSPI(BarcodeCamera.UART)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setUartSPI(BarcodeCamera.UART)
                    }
                }
                RadioButton {
                    id: spi
                    style:  econRadioButtonStyle
                    text: qsTr("SPI")
                    exclusiveGroup: modeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setUartSPI(BarcodeCamera.SPI)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setUartSPI(BarcodeCamera.SPI)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: led
                    text: "--- LED ---"
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
                columns: 3
                spacing: 20
                ExclusiveGroup { id: redledGroup }
                Text {
                    id: redLed
                    text: "RED LED"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: redLedOn
                    style:  econRadioButtonStyle
                    text:   qsTr("ON")
                    exclusiveGroup: redledGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRedLedStatus(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setRedLedStatus(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: redLedOff
                    style:  econRadioButtonStyle
                    text: qsTr("OFF")
                    exclusiveGroup: redledGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRedLedStatus(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setRedLedStatus(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: whiteledGroup }
                Text {
                    id: whiteLed
                    text: "WHITE LED"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: whiteLedOn
                    style:  econRadioButtonStyle
                    text:   qsTr("ON")
                    exclusiveGroup: whiteledGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setWhiteLedStatus(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setWhiteLedStatus(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: whiteLedOff
                    style:  econRadioButtonStyle
                    text: qsTr("OFF")
                    exclusiveGroup: whiteledGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setWhiteLedStatus(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setWhiteLedStatus(BarcodeCamera.OFF)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: qrcode
                    text: "--- QRCODE ---"
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
                ExclusiveGroup { id: qrcodeGroup }
                RadioButton {
                    id: qrcodeEnable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: qrcodeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setQRcode(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setQRcode(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: qrcodeDisable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: qrcodeGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setQRcode(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setQRcode(BarcodeCamera.OFF)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: datamatrix
                    text: "--- DATA MATRIX ---"
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
                ExclusiveGroup { id: datamatrixGroup }
                RadioButton {
                    id: datamatrixEnable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: datamatrixGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setDataMatrix(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setDataMatrix(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: datamatrixDisable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: datamatrixGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setDataMatrix(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setDataMatrix(BarcodeCamera.OFF)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: code
                    text: "--- CODE ---"
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
                columns: 3
                spacing: 15
                ExclusiveGroup { id: code128Group }
                Text {
                    id: code128
                    text: "CODE 128"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: code128Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: code128Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setCode128(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setCode128(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: code128Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: code128Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setCode128(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setCode128(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: code39Group }
                Text {
                    id: code39
                    text: "CODE 39"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: code39Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: code39Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setCode39(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setCode39(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: code39Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: code39Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setCode39(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setCode39(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: code93Group }
                Text {
                    id: code93
                    text: "CODE 93"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: code93Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: code93Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setCode93(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setCode93(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: code93Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: code93Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setCode93(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setCode93(BarcodeCamera.OFF)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: upc
                    text: "--- UPC ---"
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
                columns: 3
                spacing: 15
                ExclusiveGroup { id: upcaGroup }
                Text {
                    id: upca
                    text: "UPC-A"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: upcaEnable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: upcaGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setUPCA(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setUPCA(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: upcaDisable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: upcaGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setUPCA(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setUPCA(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: upceGroup }
                Text {
                    id: upce
                    text: "UPC-E"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: upceEnable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: upceGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setUPCE(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setUPCE(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: upceDisable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: upceGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setUPCE(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setUPCE(BarcodeCamera.OFF)
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: ean
                    text: "--- EAN ---"
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
                columns: 3
                spacing: 15
                ExclusiveGroup { id: ean13Group }
                Text {
                    id: ean13
                    text: "EAN-13"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: ean13Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: ean13Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setEAN13(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setEAN13(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: ean13Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: ean13Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setEAN13(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setEAN13(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: ean8Group }
                Text {
                    id: ean8
                    text: "EAN-8"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: ean8Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: ean8Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setEAN8(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setEAN8(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: ean8Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: ean8Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setEAN8(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setEAN8(BarcodeCamera.OFF)
                    }
                }
            }

            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: rotate
                    text: "--- ROTATION ---"
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
                columns: 3
                spacing: 15
                ExclusiveGroup { id: rotate90Group }
                Text {
                    id: rotate90
                    text: "ROTATE 90"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: rotate90Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: rotate90Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRotate90(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setRotate90(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: rotate90Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: rotate90Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRotate90(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setRotate90(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: rotate180Group }
                Text {
                    id: rotate180
                    text: "ROTATE 180"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: rotate180Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: rotate180Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRotate180(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setRotate180(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: rotate180Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: rotate180Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRotate180(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setRotate180(BarcodeCamera.OFF)
                    }
                }
            }
            Grid {
                x: 23
                y: 235
                columns: 3
                spacing: 15
                ExclusiveGroup { id: rotate270Group }
                Text {
                    id: rotate270
                    text: "ROTATE 270"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: rotate270Enable
                    style:  econRadioButtonStyle
                    text:   qsTr("Enable")
                    exclusiveGroup: rotate270Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRotate270(BarcodeCamera.ON)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setRotate270(BarcodeCamera.ON)
                    }
                }
                RadioButton {
                    id: rotate270Disable
                    style:  econRadioButtonStyle
                    text: qsTr("Disable")
                    exclusiveGroup: rotate270Group
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setRotate270(BarcodeCamera.OFF)
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setRotate270(BarcodeCamera.OFF)
                    }
                }
            }
            Text
            {
                id: gainText
                text: "--- GAIN ---"
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
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: gainSlider
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: gainMin
                    maximumValue: gainMax
                    onValueChanged:  {
                        gainTextField.text = gainSlider.value
                        barcodecam.setGain(gainSlider.value)
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
                    validator: IntValidator {bottom: gainSlider.minimumValue; top: gainSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            gainSlider.value = gainTextField.text
                        }
                    }
                }
            }
            Row{
                Layout.alignment: Qt.AlignCenter
                Text {
                    id: exposure
                    text: "--- EXPOSURE ---"
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
                columns: 3
                spacing: 15
                ExclusiveGroup { id: exposureGroup }
                Text {
                    id: exposureType
                    text: "Exposure"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: 0.50196078431373
                }
                RadioButton {
                    id: autoExp
                    style:  econRadioButtonStyle
                    text:   qsTr("Auto")
                    exclusiveGroup: exposureGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setExposure(BarcodeCamera.AUTO)
                    }
                    Keys.onReturnPressed:  {
                        barcodecam.setExposure(BarcodeCamera.AUTO)
                    }
                }
                RadioButton {
                    id: manualExp
                    style:  econRadioButtonStyle
                    text: qsTr("Manual")
                    exclusiveGroup: exposureGroup
                    activeFocusOnPress: true
                    onClicked: {
                        barcodecam.setExposure(BarcodeCamera.MANUAL)
                        barcodecam.getExposureValue()
                    }
                    Keys.onReturnPressed: {
                        barcodecam.setExposure(BarcodeCamera.MANUAL)
                        barcodecam.getExposureValue()
                    }
                }
            }
            Row
            {
                spacing: 35
                Slider
                {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: expSlider
                    opacity: manualExp.checked ? 1 : 0.1
                    enabled: manualExp.checked ? true : false
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: expMin
                    maximumValue: expMax
                    onValueChanged:  {
                        expTextField.text = expSlider.value
                        barcodecam.setExposureValue(expSlider.value)
                    }
                }
                TextField
                {
                    id: expTextField
                    opacity: manualExp.checked ? 1 : 0.1
                    enabled: manualExp.checked ? true : false
                    text: expSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: expSlider.minimumValue; top: expSlider.maximumValue}
                    onTextChanged: {
                        if(text.length > 0){
                            expSlider.value = expTextField.text
                        }
                    }
                }
            }

            Row{
                Layout.alignment: Qt.AlignCenter
                Button {
                    id: saveSettings
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Save Settings"
                    tooltip: "Click to save settings in extension controls"
                    style: econButtonStyle
                    onClicked: {
                        barcodecam.saveSettings()
                    }
                    Keys.onReturnPressed: {
                        barcodecam.saveSettings()
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
                Button {
                    id:dummy
                    opacity: 0
                }
                Button {
                    id: f_wversion_selected20CUG_MH
                    opacity: 1
                    text: "Firmware Version"
                    action: firmwareVersion
                    activeFocusOnPress : true
                    tooltip: "Click to view the firmware version of the camera"
                    style: econButtonStyle
                    Keys.onReturnPressed: {
                        getFirmwareVersion()
                    }
                }
            }
            Row{
                Button {
                    id:dummy1
                    opacity: 0
                }
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

    BarcodeCamera{
        id:barcodecam

        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }

        onUartSpiStatus:{
            if(mode == BarcodeCamera.UART){
                uart.checked = true
            }else if(mode == BarcodeCamera.SPI){
                spi.checked = true
            }
        }

        onRedLedStatus: {
            if(status == BarcodeCamera.ON){
                redLedOn.checked = true
            }else if(status == BarcodeCamera.OFF){
                redLedOff.checked = true
            }
        }

        onWhiteLedStatus: {
            if(status == BarcodeCamera.ON){
                whiteLedOn.checked = true
            }else if(status == BarcodeCamera.OFF){
                whiteLedOff.checked = true
            }
        }

        onQrCodeValue: {
            if(qrValue == BarcodeCamera.ON){
                qrcodeEnable.checked = true
            }else if(qrValue == BarcodeCamera.OFF){
                qrcodeDisable.checked = true
            }
        }

        onDataMatrixValue: {
            if(dataMatrixValue == BarcodeCamera.ON){
                datamatrixEnable.checked = true
            }else if(dataMatrixValue == BarcodeCamera.OFF){
                datamatrixDisable.checked = true
            }
        }

        onCode128Value: {
            if(code128 == BarcodeCamera.ON){
                code128Enable.checked = true
            }else if(code128 == BarcodeCamera.OFF){
                code128Disable.checked = true
            }
        }

        onCode93Value: {
            if(code93 == BarcodeCamera.ON){
                code93Enable.checked = true
            }else if(code93 == BarcodeCamera.OFF){
                code93Disable.checked = true
            }
        }

        onCode39Value: {
            if(code39 == BarcodeCamera.ON){
                code39Enable.checked = true
            }else if(code39 == BarcodeCamera.OFF){
                code39Disable.checked = true
            }
        }

        onUpcaValue: {
            if(upca == BarcodeCamera.ON){
                upcaEnable.checked = true
            }else if(upca == BarcodeCamera.OFF){
                upcaDisable.checked = true
            }
        }

        onUpceValue: {
            if(upce == BarcodeCamera.ON){
                upceEnable.checked = true
            }else if(upce == BarcodeCamera.OFF){
                upceDisable.checked = true
            }
        }

        onRotate90Value: {
            if(rotate90 == BarcodeCamera.ON){
                rotate90Enable.checked = true
            }else if(rotate90 == BarcodeCamera.OFF){
                rotate90Disable.checked = true
            }
        }

        onRotate180Value: {
            if(rotate180 == BarcodeCamera.ON){
                rotate180Enable.checked = true
            }else if(rotate180 == BarcodeCamera.OFF){
                rotate180Disable.checked = true
            }
        }

        onRotate270Value: {
            if(rotate270 == BarcodeCamera.ON){
                rotate270Enable.checked = true
            }else if(rotate270 == BarcodeCamera.OFF){
                rotate270Disable.checked = true
            }
        }

        onGainValue: {
            gainSlider.value = gain
        }

        onEan13Value: {
            if(ean13 == BarcodeCamera.ON){
                ean13Enable.checked = true
            }else if(ean13 == BarcodeCamera.OFF){
                ean13Disable.checked = true
            }
        }

        onEan8Value: {
            if(ean8 == BarcodeCamera.ON){
                ean8Enable.checked = true
            }else if(ean8 == BarcodeCamera.OFF){
                ean8Disable.checked = true
            }
        }

        onExposureType: {
            if(expType == BarcodeCamera.AUTO)
            {
                autoExp.checked = true
            }
            else if(expType == BarcodeCamera.MANUAL)
            {
                manualExp.checked = true
                barcodecam.getExposureValue()
            }
        }

        onExposureValue: {
            expSlider.value = expVal
        }

    }

    function getFirmwareVersion() {
        barcodecam.readFirmwareVersion()
        messageDialog.open()
    }
    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }
    function getValuesFromCamera(){
        barcodecam.getUartSPI()
        barcodecam.getCode128()
        barcodecam.getCode39()
        barcodecam.getCode93()
        barcodecam.getDataMatrix()
        barcodecam.getEAN13()
        barcodecam.getEAN8()
        barcodecam.getExposure()
        barcodecam.getGain()
        barcodecam.getQRcode()
        barcodecam.getRedLedStatus()
        barcodecam.getWhiteLedStatus()
        barcodecam.getRotate180()
        barcodecam.getRotate270()
        barcodecam.getRotate90()
        barcodecam.getUPCE()
        barcodecam.getUPCA()
    }
    function setToDefaultValues(){
        barcodecam.setToDefault()
        getValuesFromCamera()
    }

    Component.onCompleted: {
        getValuesFromCamera();
    }
}

