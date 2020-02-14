import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import econ.camera.h264cam 1.0
import QtQuick.Layouts 1.1
import cameraenum 1.0

Item {
    id: see3camcu135Id
    width:268
    height:750

    property bool skipUpdateInCamOnQFactor: false
    property bool skipUpdateInCamOnBitrateChange: false
    property bool skipUpdateInCamOnNoiseReduceChange: false
    property bool skipUpdateInCamOnh264Quality: false
    property bool skipUpdateUIOnHDR: false
    property bool skipUpdateUIOnDewarp: false
    property int minWindowValue
    property int maxWindowValue
    property bool hFlipClick :true
    property bool vFlipClick :true

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
        onMouseRightClickedWithStreamResolution:{
            if(autoexpManual.enabled && autoexpManual.checked){
                h264camId.setROIExposureCoordinates(previewwindowWidth, previewwindowHeight, videoStreamWidth, videoStreamHeight, x, y)
            }
        }
    }

    ScrollView{
        id: scrollview
        x: 10
        y: 189.5
        width: 257
        height: 450
        style: econscrollViewStyle
        Item{
            height:1000
            ColumnLayout{
                x:5
                y:5
                spacing:20

                Text {
                    id: qFactorText
                    text: "--- MJPG Q-Factor ---"
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
                        onValueChanged:{
                            if(skipUpdateInCamOnQFactor){
                                h264camId.setQFactor(qFactorSlider.value)
                            }
                            skipUpdateInCamOnQFactor = true
                        }
                    }
                    TextField {
                        id: qFactorTextField
                        text: qFactorSlider.value
                        font.pixelSize: 12
                        font.family: "Ubuntu"
                        smooth: true
                        enabled: false
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: qFactorSlider.minimumValue; top: qFactorSlider.maximumValue}
                    }
                }

                Text {
                    id: bitrateText
                    text: "--- H.264 Bitrate ---"
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
                        id: bitrateSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        onValueChanged:  {
                            if(skipUpdateInCamOnBitrateChange){
                                h264camId.setBitrate(bitrateSlider.value)
                            }
                            skipUpdateInCamOnBitrateChange = true
                        }
                    }
                    TextField {
                        id: bitrateTextField
                        text: bitrateSlider.value
                        font.pixelSize: 12
                        font.family: "Ubuntu"
                        smooth: true
                        enabled: false
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: bitrateSlider.minimumValue; top: bitrateSlider.maximumValue}
                    }
                }

                Text {
                    id: h264QualityText
                    text: "--- H.264 Quality ---"
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
                        id: h264QualitySlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        onValueChanged:{
                            if(skipUpdateInCamOnh264Quality){
                                h264camId.setH264Quality(h264QualitySlider.value)
                            }
                            skipUpdateInCamOnh264Quality = true
                        }
                    }
                    TextField {
                        id: h264QualityTextField
                        text: h264QualitySlider.value
                        font.pixelSize: 12
                        font.family: "Ubuntu"
                        smooth: true
                        enabled: false
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: h264QualitySlider.minimumValue; top: h264QualitySlider.maximumValue}
                    }
                }

                Text {
                    id: noiseReductionText
                    text: "--- Denoise ---"
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
                        id: noiseReduceSlider
                        width: 150
                        stepSize: 1
                        style:econSliderStyle
                        onValueChanged:  {
                            if(skipUpdateInCamOnNoiseReduceChange){
                                h264camId.setNoiseReductionValue(noiseReduceSlider.value)
                            }
                            skipUpdateInCamOnNoiseReduceChange = true
                        }
                    }
                    TextField {
                        id: noiseReduceTextField
                        text: noiseReduceSlider.value
                        font.pixelSize: 12
                        font.family: "Ubuntu"
                        smooth: true
                        enabled: false
                        horizontalAlignment: TextInput.AlignHCenter
                        style: econTextFieldStyle
                        validator: IntValidator {bottom: noiseReduceSlider.minimumValue; top: noiseReduceSlider.maximumValue}
                    }
                }

                Text {
                    id: hdr
                    text: "--- HDR ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ComboBox
                {
                    id: hdrCombo
                    enabled: true
                    opacity: 1
                    model: ListModel {
                        ListElement { text: "OFF" }
                        ListElement { text: "HDR 1X" }
                        ListElement { text: "HDR 2X" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnHDR){
                            h264camId.setHDRMode(currentIndex)
                        }
                        skipUpdateUIOnHDR = true
                    }
                }

                Text {
                    id: dewarpMode
                    text: "--- Dewarp Mode ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ComboBox
                {
                    id: dewarpCombo
                    enabled: true
                    opacity: 1
                    model: ListModel {
                        ListElement { text: "OFF" }
                        ListElement { text: "ON" }
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        if(skipUpdateUIOnDewarp){
                            h264camId.setDewarpMode(currentIndex)
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
                    enabled:hdrCombo.currentIndex == 0 ? 1 : 0
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
                        tooltip: "In this mode, the full region-based exposure value will be applied to the
frame."
                        enabled:hdrCombo.currentIndex == 0 ?1 :0
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1

                        onClicked: {
                            h264camId.setROIAutoExposureMode(H264camera.ROI_FULL)

                        }
                        Keys.onReturnPressed: {
                            h264camId.setROIAutoExposureMode(H264camera.ROI_FULL)

                        }
                    }
                    RadioButton {
                        exclusiveGroup: roiExpogroup
                        id: autoexpManual
                        text: "Manual"
                        tooltip: "In this mode, you can select the ROI and at that region the exposure
value will be applied to the entire frame"
                        enabled:hdrCombo.currentIndex == 0 ?1 :0
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        opacity: enabled ? 1 : 0.1
                        onClicked: {
                            h264camId.setROIAutoExposureMode(H264camera.ROI_MANUAL)
                            
                        }
                        Keys.onReturnPressed: {
                            h264camId.setROIAutoExposureMode(H264camera.ROI_MANUAL)
                            
                        }
                    }
                }

                Text {
                    id: windowsize
                    text: "--- ROI Window Size ---"
                    font.pixelSize: 14
                    enabled:hdrCombo.currentIndex == 0 ?1 :0
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ComboBox
                {
                    id: autoExpoWinSizeCombo
                    enabled: (autoexpManual.enabled && autoexpManual.checked && hdrCombo.currentIndex == 0) ? true : false
                    opacity: (autoexpManual.enabled && autoexpManual.checked && hdrCombo.currentIndex == 0) ? 1 : 0.1
                    model: ListModel{
                        id:roiwinsize
                    }
                    activeFocusOnPress: true
                    style: econComboBoxStyle
                    onCurrentIndexChanged: {
                        h264camId.setROIExposureWindowSize(autoExpoWinSizeCombo.currentText)
                    }
                }

                Text {
                    id: gainCtrl
                    text: "--- Gain Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                ExclusiveGroup { id: gainGrp }
                Row{
                    spacing: 35
                    RadioButton {
                        exclusiveGroup: gainGrp
                        checked: false
                        id: gainLcg
                        text: "LCG"
                        tooltip: "LCG (Low conversion gain) is most suitable for scenes with high illumination. Since this mode is suitable for bright scenes it has low sensitivity and high Dynamic range."
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            h264camId.setGainMode(H264camera.GAIN_MIN)
                        }
                        Keys.onReturnPressed: {
                            h264camId.setGainMode(H264camera.GAIN_MIN)
                        }
                    }
                    RadioButton {
                        exclusiveGroup: gainGrp
                        checked: false
                        id: gainHcg
                        text: "HCG"
                        tooltip: "HCG (High conversion gain) is most suitable for low scenes. Since this mode is suitable for low light scenes it has high sensitivity and low Dynamic range."
                        activeFocusOnPress: true
                        style: econRadioButtonStyle
                        onClicked: {
                            h264camId.setGainMode(H264camera.GAIN_MAX)
                        }
                        Keys.onReturnPressed: {
                            h264camId.setGainMode(H264camera.GAIN_MAX)
                        }
                    }
                }

                Text {
                    id: flipText
                    text: "--- Flip Control ---"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.alignment: Qt.AlignCenter
                    opacity: 0.50196078431373
                }

                Row{
                    spacing: 55
                    CheckBox {
                        id: flipCtrlHorizotal
                        activeFocusOnPress : true
                        text: "Horizontal"
                        tooltip: "This control flips the preview left or right."
                        style: econCheckBoxStyle
                        opacity: enabled ? 1 :0.1
                        onClicked:{
                            h264camId.setHorizontalFlip(flipCtrlHorizotal.checked)
                        }
                        Keys.onReturnPressed: {
                            h264camId.setHorizontalFlip(flipCtrlHorizotal.checked)
                        }
                    }
                    CheckBox {
                        id: flipCtrlVertical
                        activeFocusOnPress : true
                        text: "Vertical"
                        tooltip: "This control flips the preview up or down."
                        style: econCheckBoxStyle
                        opacity: enabled ? 1 :0.1
                        onClicked:{
                              h264camId.setVerticalFlip(flipCtrlVertical.checked)
                        }
                        Keys.onReturnPressed: {
                             h264camId.setVerticalFlip(flipCtrlVertical.checked)
                        }
                    }
                }
                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: defaultValue
                        activeFocusOnPress : true
                        text: "Default"
                        tooltip: "Click here to get the Default Values of available controls"
                        style: econButtonStyle
                        opacity: 1
                        implicitHeight: 35
                        implicitWidth: 100
                        onClicked: {
                            setToDefaultValues()
                        }
                        Keys.onReturnPressed: {
                            setToDefaultValues()
                        }
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter

                    Button {
                        id: f_wversion_selected
                        opacity: 1
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
                        onClicked: {
                            getFirmwareVer()
                        }

                        Keys.onReturnPressed: {
                            getFirmwareVer()
                        }
                    }
                }
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

    H264camera{
        id: h264camId
        onBitrateValueReceived:{
            queryForBitrateControl(queryType, bitrateValue)
        }

        onQFactorReceived:{
            queryForQFactorControl(queryType, qFactorValue)
        }

        onH264QualityReceived:{
            queryForH264QualityControl(queryType, qualityValue)
        }

        onHdrModeReceived:{
            queryForHDRControl(queryType, hdrValue)
        }
        onFlipHorizontalValue:{
            queryForHFlipControl(queryType, flipValue)
        }

        onFlipVerticalValue:{
            queryForVFlipControl(queryType, flipValue)
        }
        onDisableHFlipControl:{
            flipCtrlHorizotal.enabled = false;
        }
        onDisableVFlipControl:{
            flipCtrlVertical.enabled = false;
        }
        onGainModeReceived:{
            queryForGainControl(queryType, gainValue)
        }

        onRoiModeReceived:{
            queryForRoiMode(queryType, expMode)
        }

        onRoiWindowSizeReceived:{
            queryForWindowSize(queryType, windowSize)
        }

        onDewarpModeReceived:{
            queryForDewarpControl(queryType, dewarpValue)
        }

        onNoiseReductionValueReceived:{
            queryForNoiseReductionControl(queryType, noiseReductionValue)
        }

        onTitleTextChanged: {
            displayMessageBox(qsTr(_title), qsTr(_text))
        }
    }

    Component {
        id: econComboBoxStyle
        ComboBoxStyle {
            background: Image {
                id: hdrcombo_bkgrnd
                source: "../../Views/images/device_box.png"
                Rectangle {
                    width: hdrcombo_bkgrnd.sourceSize.width  - 28
                    height: hdrcombo_bkgrnd.sourceSize.height
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

    function getFirmwareVer() {
        h264camId.getFirmwareVersion()
    }

    function setToDefaultValues(){
        h264camId.setDefault()
        getValuesFromCamera(H264camera.UVC_GET_CUR)
    }

    Component {
        id: econTextFieldStyle
        TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 50
                implicitHeight: 25
                border.color: "#333"
                border.width: 2
                y: 1
            }
        }
    }

    function displayMessageBox(title, text){
        messageDialog.title = qsTr(title)
        messageDialog.text = qsTr(text)
        messageDialog.open()
    }

    // Query for jpeg Q control - min, max, current values
    function queryForQFactorControl(queryType, jpegQVal){
        skipUpdateInCamOnQFactor = false
        switch(queryType){
        case H264camera.UVC_GET_CUR:
            qFactorSlider.value = jpegQVal
            break;
        case H264camera.UVC_GET_MIN:
            qFactorSlider.minimumValue = jpegQVal
            break;
        case H264camera.UVC_GET_MAX:
            qFactorSlider.maximumValue = jpegQVal
            break;
        case H264camera.UVC_GET_RES:
            qFactorSlider.stepSize = jpegQVal
            break;
        }
        skipUpdateInCamOnQFactor = true

    }

    function queryForH264QualityControl(queryType, h264Quality){
        skipUpdateInCamOnh264Quality = false
        switch(queryType){
        case H264camera.UVC_GET_CUR:
            h264QualitySlider.value = h264Quality
            break;
        case H264camera.UVC_GET_MIN:
            h264QualitySlider.minimumValue = h264Quality
            break;
        case H264camera.UVC_GET_MAX:
            h264QualitySlider.maximumValue = h264Quality
            break;
        case H264camera.UVC_GET_RES:
            h264QualitySlider.stepSize = h264Quality
            break;
        }
        skipUpdateInCamOnh264Quality = true
    }

    function queryForHDRControl(queryType, hdrVal){
        if(queryType == H264camera.UVC_GET_CUR){
            switch(hdrVal){
            case H264camera.HDR_OFF:
                hdrCombo.currentIndex = 0
                break
            case H264camera.HDR_1X:
                hdrCombo.currentIndex = 1
                break
            case H264camera.HDR_2X:
                hdrCombo.currentIndex = 2
                break
            }
        }
    }

    function queryForHFlipControl(queryType, HFlipVal){
        if(queryType == H264camera.UVC_GET_CUR){
            switch(HFlipVal){
            case H264camera.HFLIP_MAX:
                flipCtrlHorizotal.checked = true;
                break
            case H264camera.HFLIP_MIN:
                flipCtrlHorizotal.checked = false;
                break
            }
        }
    }

    function queryForVFlipControl(queryType, VFlipVal){
        if(queryType == H264camera.UVC_GET_CUR){
            switch(VFlipVal){
            case H264camera.VFLIP_MAX:
                flipCtrlVertical.checked = true;
                break
            case H264camera.VFLIP_MIN:
                flipCtrlVertical.checked = false;
                break
            }
        }
    }

    function queryForGainControl(queryType, gainVal){
        if(queryType == H264camera.UVC_GET_CUR){
            switch(gainVal){
            case H264camera.GAIN_MIN:
                gainLcg.checked = true
                break
            case H264camera.GAIN_MAX:
                gainHcg.checked = true
                break
            }
        }

    }


    function queryForRoiMode(queryType, expMode){
        if(queryType == H264camera.UVC_GET_CUR){
            switch(expMode){
            case H264camera.ROI_FULL:
                autoexpFull.checked = true
                break
            case H264camera.ROI_MANUAL:
                autoexpManual.checked = true
                break
            }
        }
    }


    function queryForWindowSize(queryType, windowSize){
        switch(queryType){
        case H264camera.UVC_GET_CUR:
            autoExpoWinSizeCombo.currentIndex = windowSize -1
            break;
        case H264camera.UVC_GET_MIN:
            minWindowValue = windowSize
            break;
        case H264camera.UVC_GET_MAX:
            maxWindowValue = windowSize
            break;
        case H264camera.UVC_GET_RES:
            autoExpoWinSizeCombo.currentIndex = windowSize -1
            break;
        }

        fillROIWindowSizeCombo(minWindowValue,maxWindowValue)
    }


    function queryForDewarpControl(queryType, dewarpValue){
        if(queryType == H264camera.UVC_GET_CUR){
            switch(dewarpValue){
            case H264camera.DEWARP_OFF:
                dewarpCombo.currentIndex = 0
                break
            case H264camera.DEWARP_ON:
                dewarpCombo.currentIndex = 1
                break
            }
        }
        skipUpdateUIOnDewarp = true
    }


    // Added by Navya - 07 June 2019 -- To get window Size dynamically from camera.
    function fillROIWindowSizeCombo(minWindowValue,maxWindowValue){ // need to fill based on windowsize getting from camera
        roiwinsize.clear()
        for (var i=minWindowValue; i <=maxWindowValue; i++){
            roiwinsize.append({"text": i})
        }
    }

    function queryForNoiseReductionControl(queryType, noiseReductionVal){
        skipUpdateInCamOnNoiseReduceChange = false
        switch(queryType){
        case H264camera.UVC_GET_CUR:
            noiseReduceSlider.value = noiseReductionVal
            break;
        case H264camera.UVC_GET_MIN:
            noiseReduceSlider.minimumValue = noiseReductionVal
            break;
        case H264camera.UVC_GET_MAX:
            noiseReduceSlider.maximumValue = noiseReductionVal
            break;
        case H264camera.UVC_GET_RES:
            noiseReduceSlider.stepSize = noiseReductionVal
            break;
        }

    }

    function queryForBitrateControl(queryType, bitrateVal){
        skipUpdateInCamOnBitrateChange = false
        switch(queryType){
        case H264camera.UVC_GET_CUR:
            bitrateSlider.value = bitrateVal
            break;
        case H264camera.UVC_GET_MIN:
            bitrateSlider.minimumValue = bitrateVal
            break;
        case H264camera.UVC_GET_MAX:
            bitrateSlider.maximumValue = bitrateVal
            break;
        case H264camera.UVC_GET_RES:
            bitrateSlider.stepSize = bitrateVal
            break;
        }

    }


    function getValuesFromCamera(valueToGet){

        h264camId.getBitrate(valueToGet)
        h264camId.getQFactor(valueToGet)
        h264camId.getHDRMode(valueToGet)
        h264camId.getGainMode(valueToGet)
        h264camId.getHorizontalFlip(valueToGet)
        h264camId.getVerticalFlip(valueToGet)
        h264camId.getNoiseReductionValue(valueToGet)
        h264camId.getH264Quality(valueToGet)
        h264camId.getDewarpMode(valueToGet)
        h264camId.getROIAutoExposureMode(valueToGet)
        h264camId.getROIExposureWindowSize(valueToGet)
    }

    // get minimum , maximum and step size values for bitrate, qfactor control
    function getMinMaxStepSizeValues(){
        h264camId.getBitrate(H264camera.UVC_GET_MIN)
        h264camId.getBitrate(H264camera.UVC_GET_MAX)
        h264camId.getBitrate(H264camera.UVC_GET_RES)

        h264camId.getQFactor(H264camera.UVC_GET_MIN)
        h264camId.getQFactor(H264camera.UVC_GET_MAX)
        h264camId.getQFactor(H264camera.UVC_GET_RES)

        h264camId.getH264Quality(H264camera.UVC_GET_MIN)
        h264camId.getH264Quality(H264camera.UVC_GET_MAX)
        h264camId.getH264Quality(H264camera.UVC_GET_RES)

        h264camId.getNoiseReductionValue(H264camera.UVC_GET_MIN)
        h264camId.getNoiseReductionValue(H264camera.UVC_GET_MAX)
        h264camId.getNoiseReductionValue(H264camera.UVC_GET_RES)

        h264camId.getROIExposureWindowSize(H264camera.UVC_GET_MIN)
        h264camId.getROIExposureWindowSize(H264camera.UVC_GET_MAX)
        h264camId.getROIExposureWindowSize(H264camera.UVC_GET_RES)
    }

    Component.onCompleted: {
        h264camId.initUVCExtensionUnit(root.vidstreamObj)
        getMinMaxStepSizeValues()
        getValuesFromCamera(H264camera.UVC_GET_CUR)
    }
    Component.onDestruction:{
        h264camId.deInitUVCExtensionUnit()
    }
}
