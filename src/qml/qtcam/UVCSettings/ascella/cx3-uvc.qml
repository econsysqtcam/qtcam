import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import econ.camera.ascella 1.0
import econ.camera.uvcsettings 1.0
import "../../JavaScriptFiles/tempValue.js" as JS
import cameraenum 1.0

Item {
    width:268
    height:720

    property string vidResln;
    property int vidWidth;
    property int vidHeight;
    property bool settingWhenUpdateUI: true

    // Added by Sankari: 12 Jan 2017
    // Not getting correct current values from camera for binned / resized mode. So delay 1sec is added
    Timer {
        id: setcurrentValuesTimer
        interval: 1000
        onTriggered: {
            ascella.setCurrentValues(JS.videoCaptureResolution)
            settingWhenUpdateUI = true
            stop()
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

    Action {
        id: triggerAction
        onTriggered: {
            ascella.setAutoFocusMode(Ascella.OneShot)
        }
    }

    Action {
        id: afAreaSet
        onTriggered: {
            ascella.setCustomAreaAutoFocus(afhori_start_box_value.text, afhori_end_box_value.text, afverti_start_box_value.text, afverti_end_box_value.text)
        }
    }

    Action {
        id: setDefault
        onTriggered: {
            ascella.setDefaultValues()
        }
    }

    Action {
        id: firmwareVersion
        onTriggered: {
            displayFirmwareVersion()
        }
    }

   Image {
        id: bg
        source: "images/bg.png"
        x: 0
        y: 0
        opacity: 0
    }

    ScrollView{
        id: cx3_scrollview
        x: 10
        y: 189.5
        width: 257
        height:450
        style: ScrollViewStyle {
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
                source: "images/scroller_downarrow.png"
            }
            decrementControl: Image {
                id: decrement
                source: "images/scroller_uparrow.png"
            }
        }

        ColumnLayout{
            x:2
            y:5
            spacing:20
            Row{
                 Text {
                     id: led_status
                     text: "                    --- LED Status ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            Row{
                spacing:15
                ExclusiveGroup { id: ledgroup }
                RadioButton {
                    exclusiveGroup: ledgroup
                    id: radioOff
                    text: "Off"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        ascella.setLEDStatusMode(Ascella.LedOff, "0x00");
                    }

                    Keys.onReturnPressed: {
                    }
                }
                RadioButton {
                    exclusiveGroup: ledgroup
                    id: radioAuto
                    text: "Auto on"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setLEDStatusMode(Ascella.LedAuto, led_value.text);
                    }

                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: ledgroup
                    id: radioManual
                    text: "Manual on"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setLEDStatusMode(Ascella.LedManual, led_value.text);
                    }

                    Keys.onReturnPressed: {

                    }
                }

            }

            Row{
                spacing:25
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: ledSlider
                    enabled: (radioAuto.checked || radioManual.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: 1
                    maximumValue: 100
                    onValueChanged:  {
                        if((radioAuto.checked || radioManual.checked)){
                            if(settingWhenUpdateUI){
                                led_value.text = ledSlider.value
                                if(radioAuto.checked){
                                    ascella.setLEDStatusMode(Ascella.LedAuto, led_value.text)}
                                else if(radioManual.checked){
                                    ascella.setLEDStatusMode(Ascella.LedManual, led_value.text)}
                            }
                        }
                    }
                }
                TextField {
                    id: led_value
                    text: ledSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: false
                    opacity: ledSlider.enabled ? 1 : 0.1
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: ledSlider.minimumValue; top: ledSlider.maximumValue}
                    onTextChanged: {
                        if(text != ""){
                            ledSlider.value = led_value.text
                        }
                    }
                }
            }

            Row{
                 Text {
                     id: autoFocusMode
                     text: "               --- Auto Focus Mode ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            Row{
                  spacing:10
                  ExclusiveGroup { id: afgroup }
                  RadioButton {
                      exclusiveGroup: afgroup
                      id: radioContin
                      text: "Continuous"
                      activeFocusOnPress: true
                      style: econRadioButtonStyle
                      enabled: JS.autoFocusChecked ? true : false
                      opacity: enabled ? 1 : 0.1
                      onClicked: {
                        ascella.setAutoFocusMode(Ascella.Continuous);
                        trigger.opacity = 0.1
                      }
                      onCheckedChanged: {
                          if(JS.autoFocusChecked && checked && settingWhenUpdateUI){
                            ascella.setAutoFocusMode(Ascella.Continuous);
                          }
                      }
                      Keys.onReturnPressed: {

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
                    style: econRadioButtonStyle
                    enabled: JS.autoFocusChecked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        ascella.setAutoFocusMode(Ascella.OneShot);
                        trigger.opacity = 1
                    }
                    Keys.onReturnPressed: {

                    }
                }
                Button {
                    id: trigger
                    activeFocusOnPress : true
                    text: "Trigger"
                    style: econcx3ButtonStyle
                    enabled: (JS.autoFocusChecked && radioOneshot.checked) ? 1 : 0
                    opacity: (JS.autoFocusChecked && radioOneshot.checked) ? 1 : 0.1
                    implicitHeight: 25
                    implicitWidth: 120
                    action: (JS.autoFocusChecked && radioOneshot.checked) ? triggerAction : null
                    Keys.onReturnPressed: {
                        ascella.setAutoFocusMode(Ascella.OneShot);
                    }
                }
            }
            Row{
                 Text {
                     id: autoFocusArea
                     text: "               --- Auto Focus Area ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            RowLayout{
                spacing:25
                ExclusiveGroup { id: groupAF }
                RadioButton {
                    exclusiveGroup: groupAF
                    id: radiocenter
                    text: "Center Weighted Auto Focus"
                    activeFocusOnPress: true
                    style: radioButtonWordWrapStyle
                    enabled: JS.autoFocusChecked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    onClicked:{
                        ascella.setCenterWeightedAutoFocus();
                    }
                    onCheckedChanged:{
                        if(JS.autoFocusChecked && checked && settingWhenUpdateUI){
                            ascella.setCenterWeightedAutoFocus();
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: groupAF
                    id: radiocustom
                    text: "Custom Weighted Auto Focus"
                    activeFocusOnPress: true
                    style: radioButtonWordWrapStyle
                    enabled: JS.autoFocusChecked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    onClicked: {
                        vidResln = JS.videoCaptureResolution
                        vidWidth = vidResln.split('x')[0]
                        vidHeight = vidResln.split('x')[1]
                        ascella.setCustomAreaAutoFocus(afhori_start_box_value.text, afhori_end_box_value.text, afverti_start_box_value.text, afverti_end_box_value.text)
                    }
                    Keys.onReturnPressed: {

                    }
                }
            }

            RowLayout{
                spacing:15
                Text {
                  id: afhori_start
                  text: "AF Window Horizontal Start Position"
                  font.pixelSize: 14
                  font.family: "Ubuntu"
                  color: "#ffffff"
                  smooth: true
                  Layout.maximumWidth : 150
                  wrapMode: Text.WordWrap
                  opacity: (radiocustom.enabled && radiocustom.checked) ? 1 : 0.1
                }
                TextField {
                    id: afhori_start_box_value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: (radiocustom.enabled && radiocustom.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    validator: IntValidator {bottom: 1; top: vidWidth;}
                    implicitWidth: 70
                    text:"1"
                    maximumLength:4
                    onTextChanged: {


                    }
                }
            }
            RowLayout{
                spacing:15
                Text {
                    id: afhori_end
                    text: "AF Window Horizontal End Position"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    Layout.maximumWidth : 150
                    wrapMode: Text.WordWrap
                    opacity: (radiocustom.enabled && radiocustom.checked) ? 1 : 0.1
                }
                TextField {
                    id: afhori_end_box_value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: (radiocustom.enabled && radiocustom.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    validator: IntValidator {bottom: 1; top: vidWidth;}
                    implicitWidth: 70
                    text:vidWidth
                    maximumLength:4
                    onTextChanged: {

                    }
                }
            }
            RowLayout{
                spacing:15
                Text {
                    id: afvertical_start
                    text: "AF Window Vertical Start Position"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (radiocustom.enabled && radiocustom.checked) ? 1 : 0.1
                    Layout.maximumWidth : 150
                    wrapMode: Text.WordWrap
                }
                TextField {
                    id: afverti_start_box_value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: (radiocustom.enabled && radiocustom.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    implicitWidth: 70
                    text:"1"
                    maximumLength:4
                    validator: IntValidator {bottom: 1; top: vidHeight;}
                    onTextChanged: {

                    }
                }
            }
            RowLayout{
                spacing:15
                Text {
                    id: afverti_end
                    text: "AF Window Vertical End Position"
                    font.pixelSize: 14
                    font.family: "Ubuntu"
                    color: "#ffffff"
                    smooth: true
                    opacity: (radiocustom.enabled && radiocustom.checked) ? 1 : 0.1
                    Layout.maximumWidth : 150
                    wrapMode: Text.WordWrap
                }
                TextField {
                    id: afverti_end_box_value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    enabled: (radiocustom.enabled && radiocustom.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    implicitWidth: 70
                    text:vidHeight
                    maximumLength:4
                    validator: IntValidator {bottom: 1; top: vidHeight;}
                    onTextChanged: {

                    }
                }
            }
            RowLayout{
                Image {
                    id: hideImage
                    source: "images/afvertiend_box.png"
                    Layout.preferredWidth:50
                    opacity: 0  // Just for layout
                }
                Button {
                    id: setFocusPosition
                    activeFocusOnPress : true
                    text: "Set"
                    tooltip: "Click to set focus position entered in text box"
                    style: econcx3ButtonStyle
                    enabled: (radiocustom.enabled && radiocustom.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    action: (radiocustom.enabled && radiocustom.checked) ? afAreaSet : null
                    onClicked: {
                        if(afhori_start_box_value.length == 0 || afhori_end_box_value.length == 0 || afverti_start_box_value.length == 0 || afverti_end_box_value.length == 0){
                            messageDialog.title = qsTr("Error")
                            messageDialog.text = qsTr("Input field(s) is(are) empty. Please check whether all horizontal and vertical focus positions are filled")
                            messageDialog.open()
                            return
                        }
                        if(ascella.setCustomAreaAutoFocus(afhori_start_box_value.text, afhori_end_box_value.text, afverti_start_box_value.text, afverti_end_box_value.text)){
                            messageDialog.title = qsTr("Success")
                            messageDialog.text = qsTr("Auto focus area positions are set successfully")
                            messageDialog.open()
                        }else{
                            messageDialog.title = qsTr("Failure")
                            messageDialog.text = qsTr("Setting Auto focus area positions is failed")
                            messageDialog.open()
                        }
                    }

                    Keys.onReturnPressed: {
                        if(afhori_start_box_value.length == 0 || afhori_end_box_value.length == 0 || afverti_start_box_value.length == 0 || afverti_end_box_value.length == 0){
                            messageDialog.title = qsTr("Error")
                            messageDialog.text = qsTr("Input field(s) is(are) empty. Please check whether all horizontal and vertical focus positions are filled")
                            messageDialog.open()
                            return
                        }

                        if(ascella.setCustomAreaAutoFocus(afhori_start_box_value.text, afhori_end_box_value.text, afverti_start_box_value.text, afverti_end_box_value.text)){
                            messageDialog.title = qsTr("Success")
                            messageDialog.text = qsTr("Auto focus area positions are set successfully")
                            messageDialog.open()
                        }
                        else{
                            messageDialog.title = qsTr("Failure")
                            messageDialog.text = qsTr("Setting Auto focus area positions is failed")
                            messageDialog.open()
                        }
                    }
                }
            }
            Row{
                 Text {
                     id: sceneMode
                     text: "                  --- Scene Mode ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            Row{
                spacing:15
                ExclusiveGroup { id: sceneModegroup }
                RadioButton {
                    exclusiveGroup: sceneModegroup
                    id: scenenormal
                    text: "Normal"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setSceneMode(Ascella.SceneNormal);
                    }
                    onCheckedChanged: {
                        if(checked && settingWhenUpdateUI){
                            ascella.setSceneMode(Ascella.SceneNormal);
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: sceneModegroup
                    id: sceneDocScan
                    text: "Document Scanner"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setSceneMode(Ascella.SceneDocScan);
                    }
                    Keys.onReturnPressed: {

                    }
                }
            }

            Row{
                 Text {
                     id: colorMode
                     text: "                  --- Color Mode ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            Row{
                spacing:15
                ExclusiveGroup { id: colorModegroup }
                RadioButton {
                    exclusiveGroup: colorModegroup
                    id: colorModeNormal
                    text: "Normal   "
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setColorMode(Ascella.ColorModeNormal, "0x00")
                    }
                    onCheckedChanged: {
                        if(checked && settingWhenUpdateUI){
                            ascella.setColorMode(Ascella.ColorModeNormal, "0x00")
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: colorModegroup
                    id: colorModemono
                    text: "Mono"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setColorMode(Ascella.ColorModeMono, "0x00")
                    }
                    Keys.onReturnPressed: {

                    }
                }
            }
            RowLayout{
                spacing:15
                RadioButton {
                    exclusiveGroup: colorModegroup
                    id: colorModeNegative
                    text: "Negative"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setColorMode(Ascella.ColorModeNegative, "0x00")
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: colorModegroup
                    id: colorModeBw
                    text: "Black and White"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        if(colorModeBwAuto.checked && settingWhenUpdateUI)
                            ascella.setColorMode(Ascella.ColorModeBlackWhite, "0x00")
                        else if(colorModeBwManual.checked && settingWhenUpdateUI)
                            ascella.setColorMode(Ascella.ColorModeBlackWhite, bwvalue.text)
                    }
                    Keys.onReturnPressed: {

                    }
                }
            }
            Row{
                 Text {
                     id: blacknwhiteGrp
                     text: "               --- Black and White ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: colorModeBw.checked ? 0.50196078431373 : 0.1
                 }
            }
            RowLayout{
                spacing:25
                ExclusiveGroup { id: colorModeBwgroup }
                RadioButton {
                    exclusiveGroup: colorModeBwgroup
                    id: colorModeBwAuto
                    text: "Auto"
                    activeFocusOnPress: true
                    enabled: colorModeBw.checked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setColorMode(Ascella.ColorModeBlackWhite, "0x00")
                    }
                    onCheckedChanged:  {
                        if(checked && settingWhenUpdateUI){
                            ascella.setColorMode(Ascella.ColorModeBlackWhite, bwvalue.text)
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: colorModeBwgroup
                    id: colorModeBwManual
                    text: "Manual"
                    activeFocusOnPress: true
                    enabled: colorModeBw.checked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setColorMode(Ascella.ColorModeBlackWhite, bwvalue.text)
                    }
                    Keys.onReturnPressed: {

                    }
                }
            }
            Row{
                spacing:25
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: bwManualSlider
                    enabled: (colorModeBw.enabled && colorModeBwManual.enabled && colorModeBwManual.checked) ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: 0
                    maximumValue: 255
                    onValueChanged:  {
                        bwvalue.text = bwManualSlider.value
                        if(colorModeBwManual.enabled && colorModeBwManual.checked){
                            if(settingWhenUpdateUI){
                                ascella.setColorMode(Ascella.ColorModeBlackWhite, bwvalue.text)
                            }
                        }
                    }
                }
                TextField {
                    id: bwvalue
                    text: bwManualSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: false
                    opacity: bwManualSlider.enabled ? 1 : 0.1
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: bwManualSlider.minimumValue; top: bwManualSlider.maximumValue}
                    onTextChanged: {
                        if(text != ""){
                            bwManualSlider.value = bwvalue.text

                        }

                    }
                }
            }

            RowLayout{
                spacing:25
                ExclusiveGroup { id: colorModegroup2 }
                RadioButton {
                    exclusiveGroup: colorModegroup2
                    id: colorModeBinned
                    text: "Binned"
                    activeFocusOnPress: true                   
                    opacity: enabled ? 1 : 0.1
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setBinnedResizedMode(Ascella.Binned)
                    }
                    onCheckedChanged:{
                        if(checked && settingWhenUpdateUI){
                            ascella.setBinnedResizedMode(Ascella.Binned)
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: colorModegroup2
                    id: colorModeResized
                    text: "Resized"
                    activeFocusOnPress: true                   
                    opacity: enabled ? 1 : 0.1
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setBinnedResizedMode(Ascella.Resized)
                    }
                    onCheckedChanged:{
                        if(checked && settingWhenUpdateUI){
                            ascella.setBinnedResizedMode(Ascella.Resized)
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
            }

            RowLayout{
                 Text {
                     id: exposureCompInAuto
                     text: "     --- Exposure Compensation in Auto mode ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                     Layout.maximumWidth: 230
                     wrapMode: Text.WordWrap
                 }
            }
            Row{
                spacing:25
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: exposureCompSlider
                    enabled: JS.autoExposureSelected ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue:0
                    maximumValue: 12
                    onValueChanged:  {
                        exposureCompTextValue.text = exposureCompSlider.value
                        if(settingWhenUpdateUI){
                            ascella.setExposureCompensation(exposureCompTextValue.text)
                        }
                    }                    
                }
                TextField {
                    id: exposureCompTextValue
                    text: exposureCompSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: false
                    opacity: exposureCompSlider.enabled ? 1 : 0.1
                    style: econTextFieldStyle
                    onTextChanged: {
                        if(text != ""){
                            exposureCompSlider.value = exposureCompTextValue.text
                        }
                    }
                }
            }

            Row{
                 Text {
                     id: noiseReductionGrp
                     text: "        --- Noise Reduction mode ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            Row{
                spacing:25
                ExclusiveGroup { id: noiseReductiongrp }
                RadioButton {
                    exclusiveGroup: noiseReductiongrp
                    id: reduceNoiseAuto
                    text: "Auto"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        ascella.setNoiseReduceMode(Ascella.NoiseReduceNormal, "0x00");
                    }
                    onCheckedChanged:{
                        if(checked && settingWhenUpdateUI){
                            ascella.setNoiseReduceMode(Ascella.NoiseReduceNormal, "0x00");
                        }
                    }
                    Keys.onReturnPressed: {
                    }
                }
                RadioButton {
                    exclusiveGroup: noiseReductiongrp
                    id: reduceNoiseFix
                    text: "Fix"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setNoiseReduceMode(Ascella.NoiseReduceFix, reduceNoiseFixvalue.text);
                    }
                    Keys.onReturnPressed: {

                    }
                }

            }

            Row{
                spacing:25
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: reduceNoiseFixSlider
                    enabled: reduceNoiseFix.checked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: 0
                    maximumValue: 10
                    onValueChanged:  {
                        reduceNoiseFixvalue.text = reduceNoiseFixSlider.value
                        if(settingWhenUpdateUI){
                            ascella.setNoiseReduceMode(Ascella.NoiseReduceFix, reduceNoiseFixvalue.text)
                        }
                    }
                }
                TextField {
                    id: reduceNoiseFixvalue
                    text: reduceNoiseFixSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    horizontalAlignment: TextInput.AlignHCenter
                    enabled: false
                    opacity: reduceNoiseFix.checked ? 1 : 0.1
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: reduceNoiseFixSlider.minimumValue; top: reduceNoiseFixSlider.maximumValue}
                    onTextChanged: {
                        if(text != ""){                            
                            reduceNoiseFixSlider.value = reduceNoiseFixvalue.text                            
                        }
                    }
                }
            }

            Row{
                 Text {
                     id: limitMaxFrameRateGrp
                     text: "     --- Limit Maximum FrameRate ---"
                     font.pixelSize: 14
                     font.family: "Ubuntu"
                     color: "#ffffff"
                     smooth: true
                     opacity: 0.50196078431373
                 }
            }
            Row{
                spacing:15
                ExclusiveGroup { id: maxFrameRategrp }
                RadioButton {
                    exclusiveGroup: maxFrameRategrp
                    id: limitMaxFrameRateDisable
                    text: "Disable"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked:{
                        ascella.setLimitMaxFrameRateMode(Ascella.Disable, "0x00");
                    }
                    onCheckedChanged:{
                        if(checked && settingWhenUpdateUI){
                           ascella.setLimitMaxFrameRateMode(Ascella.Disable, "0x00");
                        }
                    }
                    Keys.onReturnPressed: {

                    }
                }
                RadioButton {
                    exclusiveGroup: maxFrameRategrp
                    id: applyMaxFrameRate
                    text: "Maximum FrameRate"
                    activeFocusOnPress: true
                    style: econRadioButtonStyle
                    onClicked: {
                        ascella.setLimitMaxFrameRateMode(Ascella.ApplyMaxFrameRate, applyMaxFrameRatevalue.text);
                    }
                    Keys.onReturnPressed: {

                    }
                }

            }

            Row{
                spacing:25
                Slider {
                    activeFocusOnPress: true
                    updateValueWhileDragging: false
                    id: applyMaxFrameRateSlider
                    enabled: applyMaxFrameRate.checked ? 1 : 0
                    opacity: enabled ? 1 : 0.1
                    width: 150
                    stepSize: 1
                    style:econSliderStyle
                    minimumValue: 3
                    maximumValue: 119
                    onValueChanged:  {
                        if(applyMaxFrameRate.checked){
                            applyMaxFrameRatevalue.text = applyMaxFrameRateSlider.value
                            if(settingWhenUpdateUI){
                                ascella.setLimitMaxFrameRateMode(Ascella.ApplyMaxFrameRate, applyMaxFrameRatevalue.text)
                            }
                        }
                    }
                }
                TextField {
                    id: applyMaxFrameRatevalue
                    text: applyMaxFrameRateSlider.value
                    font.pixelSize: 10
                    font.family: "Ubuntu"
                    smooth: true
                    enabled: false
                    opacity: applyMaxFrameRateSlider.enabled ? 1 : 0.1
                    horizontalAlignment: TextInput.AlignHCenter
                    style: econTextFieldStyle
                    validator: IntValidator {bottom: applyMaxFrameRateSlider.minimumValue; top: applyMaxFrameRateSlider.maximumValue}
                    onTextChanged: {
                       if(text != ""){
                           applyMaxFrameRateSlider.value = applyMaxFrameRatevalue.text                           
                       }
                    }
                }
            }
            RowLayout{
                Image {
                    id: hideImage1
                    source: "images/afvertiend_box.png"
                    Layout.preferredWidth:50
                    opacity: 0  // Just for layout
                }
                Button {
                    id: defaultValue
                    opacity: 1
                    activeFocusOnPress : true
                    text: "Default"
                    tooltip: "Click to set default values in extension controls"
                    action: setDefault
                    style: econcx3ButtonStyle
                    Keys.onReturnPressed: {
                        ascella.setDefaultValues()
                    }
                }
            }
            RowLayout{
                Image {
                    id: hideImage2
                    source: "images/afvertiend_box.png"
                    Layout.preferredWidth:50
                    opacity: 0  // Just for layout
                }
                Button {
                    id: fwVersion
                    opacity: 1
                    activeFocusOnPress : true
                    text: "F/W Version"
                    tooltip: "Click to see firmware version"
                    action: firmwareVersion
                    style: econcx3ButtonStyle
                    Keys.onReturnPressed: {

                    }
                }
            }
            Row{
                Image {
                    id: hideImage3
                    source: "images/afvertiend_box.png"
                    Layout.preferredWidth:50
                    opacity: 0  // Just for layout
                }
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
        id: radioButtonWordWrapStyle
        RadioButtonStyle {
            label: Text {
                width:80
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
        id: econcx3ButtonStyle
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

    Ascella{
        id:ascella
        onDeviceStatus:{
            messageDialog.title = title.toString()
            messageDialog.text = message.toString()
            messageDialog.open()
        }
        onTitleTextChanged: {
            messageDialog.title = _title.toString()
            messageDialog.text = _text.toString()
            messageDialog.open()
        }
        /* set default values in UI */
        onLedOffEnable:{
            radioOff.checked = true
            settingWhenUpdateUI = true
            led_value.text = brightness
        }
        onAutoExposureEnable:{
            settingWhenUpdateUI = true
            if(JS.autoExposureSelected){
                exposureCompSlider.value = exposureValue
            }
        }
        onAfContinuousEnable:{
            settingWhenUpdateUI = true
                radioContin.checked = true
        }
        onNoiseReductionAutoEnable:{
            settingWhenUpdateUI = true
            reduceNoiseAuto.checked = true
        }
        onNormalSceneModeEnable:{
            settingWhenUpdateUI = true
            scenenormal.checked = true
        }
        onLimitMaxFRDisableMode:{
            settingWhenUpdateUI = true
            limitMaxFrameRateDisable.checked = true
            applyMaxFrameRatevalue.text = frameRateValue
        }
        onNormalColorModeEnable:{
            settingWhenUpdateUI = true
            colorModeNormal.checked = true
        }
        onBwColorModeAutoEnable:{
            settingWhenUpdateUI = true
            bwvalue.text = bwThresholdValue
            colorModeBwAuto.checked = true
        }

        onSetBinnResizeEnableDisable:{                
            if(mode == 1){
                colorModeBinned.enabled = true
                colorModeResized.enabled = true
            }else if(mode == 0){
                colorModeBinned.enabled = false
                colorModeResized.enabled = false
            }
        }
        onSetBinnResizeSelect:{
            settingWhenUpdateUI = true
            if(binResizeSelect == 1){
                colorModeBinned.checked = true
            }else if(binResizeSelect == 2){
                colorModeResized.checked = true
            }
        }
        /* set current values in UI */
        onSetCurrbinnResizeEnableDisable:{
            if(mode == 1){
                colorModeBinned.enabled = true
                colorModeResized.enabled = true
            }else if(mode == 0){
                colorModeBinned.enabled = false
                colorModeResized.enabled = false
            }
        }
        onSetCurbinnResizeSelect:{
            settingWhenUpdateUI = false
            if(binResizeSelect == 1){
                colorModeBinned.checked = true
            }else if(binResizeSelect == 2){
                colorModeResized.checked = true
            }
        }
        onSetCurrentLedValue:{
            settingWhenUpdateUI = false
            if(ledCurMode == Ascella.LedOff){
                radioOff.checked = true
            }else if(ledCurMode == Ascella.LedAuto){
                radioAuto.checked = true
            }else if(ledCurMode == Ascella.LedManual){
                radioManual.checked = true
            }            
            led_value.text = ledCurBrightness
        }
        onSetCurrentAfMode:{
            if(afMode == Ascella.Continuous){
                radioContin.checked = true
            }else if(afMode == Ascella.OneShot){
                radioOneshot.checked = true
            }
        }
        onSetCurrentColorMode:{
            settingWhenUpdateUI = false
            if(curColorMode == Ascella.ColorModeNormal){
                colorModeNormal.checked = true
            }else if(curColorMode == Ascella.ColorModeMono){
                colorModemono.checked = true
            }else if(curColorMode == Ascella.ColorModeNegative){
                colorModeNegative.checked = true
            }else if(curColorMode == Ascella.ColorModeBlackWhite){
                colorModeBw.checked = true
            }
        }
        onSetCurrentBwMode:{            
            settingWhenUpdateUI = false
            if(curBWMode == "0"){
                colorModeBwAuto.checked = true
            }else{
                colorModeBwManual.checked = true
            }
            bwvalue.text = CurBWthreshold
        }
        onSetCurNoiseReductionMode:{
            if(curNoiseMode == Ascella.NoiseReduceNormal){
                reduceNoiseAuto.checked = true
            }else if(curNoiseMode == Ascella.NoiseReduceFix){
                reduceNoiseFix.checked = true
            }
            settingWhenUpdateUI = false
            reduceNoiseFixvalue.text = curNoiseValue
        }
        onSetCurSceneMode:{
            if(curSceneMode == Ascella.SceneNormal){
                scenenormal.checked = true
            }else{
                sceneDocScan.checked = true
            }
        }
        onSetCurAutoExposureEnable:{
            settingWhenUpdateUI = false
            if(JS.autoExposureSelected){
                exposureCompSlider.value = exposureValue
            }
        }
        onSetCurFRMode:{
            if(curFRMode == Ascella.Disable){
                limitMaxFrameRateDisable.checked = true
            }else if(curFRMode == Ascella.ApplyMaxFrameRate){
                applyMaxFrameRate.checked = true
            }
            settingWhenUpdateUI = false
            applyMaxFrameRatevalue.text = curMaxFRLimit
        }
        onSetAfAreaCenterMode:{
                settingWhenUpdateUI = true
                radiocenter.checked = true
        }

        onSetCurrentAfAreaCenterMode:{
                settingWhenUpdateUI = false
                radiocenter.checked = true
        }

        onSetCurrentAfAreaCustomMode:{
            if(JS.autoFocusChecked){
                radiocustom.checked = true
                switch(afTextBoxNumber){
                case "0x01":
                    afhori_start_box_value.text = curPosition;
                    break;
                case "0x02":
                    afhori_end_box_value.text = curPosition;
                    break;
                case "0x03":
                    afverti_start_box_value.text = curPosition;
                    break;
                case "0x04":
                    afverti_end_box_value.text = curPosition;
                    break;
                }
            }
        }
    }

    function enableDisableAutoExposureControls(autoExposureSelect){
        if(autoExposureSelect){
            exposureCompSlider.enabled = true
            exposureCompTextValue.enabled = true
            exposureCompSlider.opacity = 1
            exposureCompTextValue.opacity = 1
        }else{
            exposureCompSlider.enabled = false
            exposureCompTextValue.enabled = false
            exposureCompSlider.opacity = 0.1
            exposureCompTextValue.opacity = 0.1
        }

    }

    function enableDisableAutoFocusUIControls(autoFocusSelect){
        if(autoFocusSelect){
            radioContin.enabled = true
            radioOneshot.enabled = true
            if(radioOneshot.enabled && radioOneshot.checked){
                trigger.enabled = true
                trigger.opacity = 1
            }
            radiocenter.enabled = true
            radiocustom.enabled = true
            radioContin.opacity = 1
            radioOneshot.opacity = 1
            radiocenter.opacity = 1
            radiocustom.opacity = 1
        }else{
            radioContin.enabled = false
            radioOneshot.enabled = false
            trigger.enabled = false
            trigger.opacity = 0.1
            radiocenter.enabled = false
            radiocustom.enabled = false
            radioContin.opacity = 0.1
            radioOneshot.opacity = 0.1
            radiocenter.opacity = 0.1
            radiocustom.opacity = 0.1
        }
    }

    function displayFirmwareVersion() {
        ascella.getFirmwareVersion()
    }

    Uvccamera {
            id: uvccamera
            onTitleTextChanged: {
                messageDialog.title = _title.toString()
                messageDialog.text = _text.toString()
                messageDialog.open()
            }
     }

    Component.onCompleted:{
        uvccamera.initExtensionUnitAscella()
        vidResln = JS.videoCaptureResolution
        vidWidth = vidResln.split('x')[0]
        vidHeight = vidResln.split('x')[1]
        ascella.setCurrentValues(JS.videoCaptureResolution)
        settingWhenUpdateUI = true
    }

    Connections{
         target: root
         onAutoFocusSelected:{
             enableDisableAutoFocusUIControls(autoFocusSelect)
         }
         onAutoExposureSelected:{
             enableDisableAutoExposureControls(autoExposureSelect)
         }
         onPreviewFPSChanged:{
             setcurrentValuesTimer.start()
         }

         // Added by Sankari: 23 Dec 2016 - If video resolution is changed, set binned/resized mode enable/disable in UI. So
         // setCurrentValues is called
         onVideoResolutionChanged:{
            ascella.setCurrentValues(JS.videoCaptureResolution)
            settingWhenUpdateUI = true
         }
    }
}
