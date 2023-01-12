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

    property int camMode

    Connections{
        target:root
        onSendCameraModeToQml:{
            camMode = cameraMode

            //If cameraMode = 2(RGB Mode) && cameraMode = 3(IR Mode) - destroy this window
            if((camMode == 2)||(camMode == 3))
            {
                irPreview.close()
            }
        }
    }

    Connections{
        target: helperQml
        onImageChanged:
        {
           liveImageItem.image = helperQml.image
        }
    }

    ImageItem{
        id: liveImageItem
         height: parent.height
         width: parent.width
    }

}
