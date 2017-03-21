.pragma library
var masterMode_M=1;
var triggerMode_M=0;
var masterMode_B=1;
var triggerMode_B=0;
var masterMode_11cug=1;
var triggerMode_11cug=0;
var masterMode_12cuinr=1;
var triggerMode_12cuinr=0;
var masterMode_cu51=1;
var triggerMode_cu51=0;
var stillCaptureFormat
var stillCaptureResolution
var videoCaptureFormat
var videoCaptureResolution
var videocaptureFps
var videoExtension
var videoEncoder
var autoFocusChecked
var autoExposureSelected
var autoWhiteBalSelected

function enableTriggerMode_10cugM() {
    triggerMode_M=1;
    masterMode_M=0;   
}

function enableMasterMode_10cugM() {
    triggerMode_M=0;
    masterMode_M=1;
}

function enableTriggerMode_10cugB() {
    triggerMode_B=1;
    masterMode_B=0;
}

function enableMasterMode_10cugB() {
    triggerMode_B=0;
    masterMode_B=1;
}

function enableTriggerMode_11cug() {
    triggerMode_11cug=1;
    masterMode_11cug=0;
}

function enableMasterMode_11cug() {
    triggerMode_11cug=0;
    masterMode_11cug=1;
}

function enableTriggerMode_12cuinr() {
    triggerMode_12cuinr=1;
    masterMode_12cuinr=0;
}

function enableMasterMode_12cuinr() {
    triggerMode_12cuinr=0;
    masterMode_12cuinr=1;
}

function enableTriggerMode_cu51() {
    triggerMode_cu51=1;
    masterMode_cu51=0;
}

function enableMasterMode_cu51() {
    triggerMode_cu51=0;
    masterMode_cu51=1;
}


