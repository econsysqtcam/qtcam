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

    onWidthChanged: {
        if (width < minimumWidth)
            width = minimumWidth;
    }
    onHeightChanged: {
        if (height < minimumHeight)
            height = minimumHeight;
    }
    property int camMode

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

        function onMinimizeSecondaryWindow() {
            irPreview.flags ^= Qt.WindowMinimized
        }
    }

    Connections{
        target: helperQml
        function onImageChanged()
        {
           liveImageItem.image = helperQml.image
        }
    }

    ImageItem{
        id: liveImageItem
         height: parent.height
         width: parent.width
    }

    Component.onCompleted:
    {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }
}
