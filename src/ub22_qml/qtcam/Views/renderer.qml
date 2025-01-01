import QtQuick 2.0
import QtQuick.Window 2.0
import econ.camera.qimagerenderer 1.0
import econ.camera.stream 1.0

Window {
    id:irPreview
    title: qsTr("IR Preview")
    visible: true
    width: 640
    height: 480
    color: "black"

    //Restrict window resizing
    minimumWidth: 640
    minimumHeight: 2 //To restrict menubar from minimizing


    property int irWidth: helperQml.irWidth
    property int irHeight: helperQml.irHeight
    property bool isFramesAvailable: helperQml.isFrameReceived
    property bool isMainWinMinimized : false

    flags: Qt.Window  | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint

    ImageItem{
        id: liveImageItem
         height: parent.height
         width: parent.width
         x: parent.x
         y: parent.y
         visible: isFramesAvailable? true : false
    }

    //Fps drop issue : On minimizing the main window, need to minimize ir Window.
    function setState(state) {
            if (state === Qt.WindowMinimized) {
                if(!isMainWinMinimized){
                    irPreview.showMinimized(); // Minimize the window
                }
                isMainWinMinimized = true;
            }
            else if(state === Qt.WindowNoState) {
                isMainWinMinimized = false;
            }
        }

    onWidthChanged: {
        if (width < minimumWidth)
            width = minimumWidth;

        adjustSizeAndPosition();
    }
    onHeightChanged: {
        if (height < minimumHeight)
            height = minimumHeight;

        adjustSizeAndPosition();
    }

    onWindowStateChanged: {
        //When ir window has been retrieved from minimize, need to maximize the main window too.
        if (windowState === Qt.WindowNoState) {
           if(isMainWinMinimized){
               root.windowStateChangedToNoState();
               isMainWinMinimized = false;
            }
        }
    }

    function adjustSizeAndPosition() {
        if (irWidth > 0 && irHeight > 0) {
            // Calculate the scaling factor based on the aspect ratio
            var scaleFactor = Math.min(width / irWidth, height / irHeight);

            // Scale the width and height to maintain aspect ratio
            liveImageItem.width = irWidth * scaleFactor;
            liveImageItem.height = irHeight * scaleFactor;

            // Center the image in the window
            if (liveImageItem.width < width) {
                liveImageItem.x = parent.x + (width - liveImageItem.width) / 2; // center horizontally
            } else {
                liveImageItem.x = parent.x; // align left if the image is larger than the container
            }

            if (liveImageItem.height < height) {
                liveImageItem.y = parent.y + (height - liveImageItem.height) / 2; // center vertically
            } else {
                liveImageItem.y = parent.y; // align top if the image is larger than the container
            }
        }


    }

    property int camMode

    // New: Track main window visibility
        property bool mainWindowMinimized: false

    signal closeWindow()

    Connections{
        target:root
        function onSendCameraModeToQml(cameraMode){
            camMode = cameraMode

            //If cameraMode = 2(RGB Mode) && cameraMode = 3(IR Mode) - destroy this window
            if((camMode == 2)||(camMode == 3))
            {
                irPreview.close()
            }
        }
        function onWindowCloseAfterUnplug()
        {
            irPreview.close()
        }
        function onDestroyIrWindow(){
            irPreview.close()
        }
    }

   Component.onCompleted:
    {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2

     }
}
