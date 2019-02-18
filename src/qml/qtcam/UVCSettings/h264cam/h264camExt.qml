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
    }

    ScrollView{
            id: scrollview
            x: 10
            y: 189.5
            width: 257
            height: 450
            style: econscrollViewStyle
            Item{
                height:650
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
			ListElement { text: "Night Mode" }
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
                        skipUpdateUIOnDewarp = true
                    }
                }


                Row{
                    Layout.alignment: Qt.AlignCenter
                    Button {
                        id: defaultValue
                        activeFocusOnPress : true
                        text: "Default"
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
            case H264camera.HDR_NIGHTMODE:
                hdrCombo.currentIndex = 3
               break
            }
        }
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
        h264camId.getNoiseReductionValue(valueToGet)
        h264camId.getH264Quality(valueToGet)
	h264camId.getDewarpMode(valueToGet)
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
