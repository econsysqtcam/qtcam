
linux {
    message("Checking Linux distribution...")

    # Check for the Linux distro and set appropriate flags
    distro = $$system(lsb_release -is)
    version = $$system(lsb_release -rs)

    message("Detected $$distro $$version ($$QMAKE_HOST.arch)")
} else {
    error("Unsupported platform")
}

# Ubuntu 18.04 defaults to Qt 5.9 so make your code fail to compile if it uses deprecated APIs.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050900    # disables all the APIs deprecated before Qt 5.9.0

contains(DEFINES, UBUNTU_22_04) {
    folder_01.source  = ub22_qml/qtcam
    folder_01.target  = qml
    DEPLOYMENTFOLDERS = folder_01
}
else:{
    folder_01.source  = qml/qtcam
    folder_01.target  = qml
    DEPLOYMENTFOLDERS = folder_01
}


QT += widgets concurrent multimedia
TARGET = Qtcam

CONFIG += release

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    logger.cpp \
    about.cpp \
    cameraproperty.cpp \
    videostreaming.cpp \
    uvccamera.cpp \
    v4l2-api.cpp \
    seecam_10cug_m.cpp \
    seecam_10cug_bayer.cpp \
    seecam_11cug.cpp \
    seecam_cu50.cpp \
    seecam_ar0130.cpp \
    videoencoder.cpp \
    seecam_cu51.cpp \
    see3cam_cu130.cpp \
    h264decoder.cpp \
    ascella.cpp \
    seecam_cu30.cpp \
    see3cam_130.cpp \
    see3cam_30.cpp \
    seecam_81.cpp \
    see3cam_cu135.cpp \
    see3cam_cu40.cpp\
    see3cam_cu20.cpp\
    uvcExtCx3sni.cpp \
    keyEventReceive.cpp \
    see3cam_27cug.cpp\
    see3cam_cu83.cpp \
    nilecam20_usb.cpp \
    nilecam30_usb.cpp \
    h264camera.cpp \
    see3cam_cu55.cpp \
    see3cam_130A.cpp \
    audioinput.cpp \
    see3cam_cu38.cpp \
    alsa.cpp\
    fscam_cu135.cpp \
    see3camcu55_mh.cpp \
    see3cam_20cug.cpp \
    see3cam_cu22.cpp \
    see3cam_130d.cpp \
    see3cam_24cug.cpp \
    see3cam_cu81.cpp \
    seecam_cu80.cpp \
    see3cam_160.cpp \
    ecam51_usb.cpp \
    ecam82_usb.cpp \
    see3cam_cu27.cpp \
    see3cam_1332.cpp \
    ecam83_usb.cpp \
    see3cam_135m.cpp \
    see3cam_cu136m.cpp \
    see3cam_50cug_m.cpp \
    see3cam_cu135m_h01r1.cpp \
    barcode_camera.cpp \
    renderer.cpp \
    see3cam_cu210.cpp \
    see3cam_cu200.cpp \
    ecam512_usb.cpp \
    see3cam_16cug_m.cpp \
    see3cam_50cug.cpp \
    see3cam_cu512_m.cpp \
    see3cam_cu84.cpp \
    see3cam_cu200m.cpp \
    see3cam_cu31.cpp \
    see3cam_cu200_h01r1.cpp \
    imx900usbcam.cpp

# Installation path
# target.path =
target.path = /usr/

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()


HEADERS += \
    logger.h \
    about.h \
    cameraproperty.h \
    videostreaming.h \
    uvccamera.h \
    v4l2-api.h \
    seecam_10cug_m.h \
    seecam_10cug_bayer.h \
    seecam_11cug.h \
    seecam_cu80.h \
    seecam_cu50.h \
    seecam_ar0130.h \
    videoencoder.h \
    seecam_cu51.h \
    see3cam_cu130.h \
    h264decoder.h \
    ascella.h \
    seecam_cu30.h \
    common.h \
    see3cam_130.h \
    common_enums.h \
    see3cam_30.h \
    seecam_81.h \
    see3cam_cu135.h \
    see3cam_cu40.h\
    see3cam_cu20.h\
    uvcExtCx3sni.h \
    keyEventReceive.h \
    see3cam_27cug.h\
    see3cam_cu83.h\
    nilecam30_usb.h \
    nilecam20_usb.h \
    h264camera.h \
    see3cam_cu55.h \
    see3cam_130A.h \
    audioinput.h \
    see3cam_cu38.h \
    alsa.h\
    fscam_cu135.h\
    see3camcu55_mh.h \
    see3cam_20cug.h  \
    see3cam_cu22.h \
    see3cam_130d.h \
    see3cam_24cug.h \
    see3cam_cu81.h \
    see3cam_160.h \
    ecam51_usb.h \
    ecam82_usb.h \
    see3cam_cu27.h \
    see3cam_1332.h \
    ecam83_usb.h \
    see3cam_135m.h \
    see3cam_cu136m.h \
    see3cam_cu135m_h01r1.h \
    see3cam_50cug_m.h \
    barcode_camera.h \
    renderer.h \
    see3cam_cu210.h \
    see3cam_cu200.h \
    ecam512_usb.h \
    see3cam_16cug_m.h \
    see3cam_50cug.h \
    see3cam_cu84.h \
    see3cam_cu512_m.h \
    see3cam_cu31.h \
    see3cam_cu200m.h \
    see3cam_cu200_h01r1.h \
    imx900usbcam.h

message("Adding additional include paths...")
INCLUDEPATH +=  $$PWD/v4l2headers/include \
                /usr/include/ffmpeg \
                /usr/include/libusb-1.0 \
                /usr/include/libevdev-1.0 \
                /usr/include/pulse \
                /usr/include/

DISTRIBUTION_NAME = $$system(lsb_release -a | grep -o "bionic")
contains(DISTRIBUTION_NAME, bionic):{
    message("Detected Bionic")
    QMAKE_CXX = "g++-5"
    QMAKE_CXXFLAGS += -std=c++11
}

message("Adding LIBS from system dependencies...")
LIBS += -lv4l2 -lv4lconvert \
        -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -ludev \
        -lusb-1.0 \
        -lpulse \
        -lasound \
        -lturbojpeg \
        -levdev

#QMAKE_CXX += -ggdb
QMAKE_CFLAGS_THREAD = -D__STDC_CONSTANT_MACROS      #For Ubuntu 12.04 compilation
QMAKE_CXXFLAGS_THREAD = -D__STDC_CONSTANT_MACROS    #For Ubuntu 12.04 compilation
QMAKE_CFLAGS_ISYSTEM = -I                           #For Ubuntu 20.04


message("Adding QML source files from $$PWD/ub22_qml")

OTHER_FILES += \
    ub22_qml/qtcam/Views/qtcam.qml \
    ub22_qml/qtcam/Views/aboutview.qml \
    ub22_qml/qtcam/Views/audiocapturesettings.qml \
    ub22_qml/qtcam/Views/captureandvideorecord.qml \
    ub22_qml/qtcam/Views/imagequalitysettings.qml \
    ub22_qml/qtcam/Views/renderer.qml \
    ub22_qml/qtcam/Views/statusbar.qml \
    ub22_qml/qtcam/Views/stillcapturesettings.qml \
    ub22_qml/qtcam/Views/videocapturesettings.qml

DISTFILES += \
    ub22_qml/qtcam/UVCSettings/ascella/cx3-uvc.qml \
    ub22_qml/qtcam/UVCSettings/barcode/barcode_camera.qml \
    ub22_qml/qtcam/UVCSettings/cx3SNI/uvcExtCX3SNI.qml \
    ub22_qml/qtcam/UVCSettings/eCAM512USB/ecam512usb.qml \
    ub22_qml/qtcam/UVCSettings/ecam51_USB/ecam51_usb.qml \
    ub22_qml/qtcam/UVCSettings/ecam82_USB/ecam82_usb.qml \
    ub22_qml/qtcam/UVCSettings/ecam83_USB/ecam83_usb.qml \
    ub22_qml/qtcam/UVCSettings/fscamcu135/fscamcu135.qml \
    ub22_qml/qtcam/UVCSettings/h264cam/h264camExt.qml \
    ub22_qml/qtcam/UVCSettings/nilecam20usb/nilecam20_usb.qml \
    ub22_qml/qtcam/UVCSettings/nilecam30usb/nilecam30_usb.qml \
    ub22_qml/qtcam/UVCSettings/see3cam11/uvc11.qml \
    ub22_qml/qtcam/UVCSettings/see3cam130/uvc_130.qml \
    ub22_qml/qtcam/UVCSettings/see3cam130A/uvc_130A.qml \
    ub22_qml/qtcam/UVCSettings/see3cam130D/see3cam_130D.qml \
    ub22_qml/qtcam/UVCSettings/see3cam1332/see3cam_1332.qml \
    ub22_qml/qtcam/UVCSettings/see3cam135m/see3cam135m.qml \
    ub22_qml/qtcam/UVCSettings/see3cam160/see3cam_160.qml \
    ub22_qml/qtcam/UVCSettings/see3cam16cugm/see3cam16cugm.qml \
    ub22_qml/qtcam/UVCSettings/see3cam20cug/see3cam_20cug.qml \
    ub22_qml/qtcam/UVCSettings/see3cam24cug/see3cam_24cug.qml \
    ub22_qml/qtcam/UVCSettings/see3cam27cug/see3cam27cug.qml\
    ub22_qml/qtcam/UVCSettings/see3cam30/uvc_30.qml \
    ub22_qml/qtcam/UVCSettings/see3cam40/uvc40.qml \
    ub22_qml/qtcam/UVCSettings/see3cam50/uvc50.qml \
    ub22_qml/qtcam/UVCSettings/see3cam50cug/see3cam50cug.qml \
    ub22_qml/qtcam/UVCSettings/see3cam50cugm/see3cam_50cug_m.qml \
    ub22_qml/qtcam/UVCSettings/see3cam51/uvc51.qml \
    ub22_qml/qtcam/UVCSettings/see3cam80/uvc80.qml \
    ub22_qml/qtcam/UVCSettings/see3cam81/uvc81.qml \
    ub22_qml/qtcam/UVCSettings/see3cam_cu512M/see3camcu512m.qml \
    ub22_qml/qtcam/UVCSettings/see3cam_cu81/see3cam_cu81.qml \
    ub22_qml/qtcam/UVCSettings/see3camar0130/uvc_ar0130.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu130/uvc_cu130.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu135/uvc_cu135.qml \
    ub22_qml/qtcam/UVCSettings/see3CamCu135mH01r1/see3CamCu135m_H01r1.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu136m/see3cam_cu136m.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu20/see3camcu20.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu200/see3camcu200.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu200m/see3camcu200m.qml \
    ub22_qml/qtcam/UVCSettings/See3CamCu210/see3camcu210.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu22/see3camcu22.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu27/see3camcu27.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu30/uvc_cu30.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu38/uvc_cu38.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu55/see3camcu55.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu55_MH/see3camcu55_mh.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu83/see3camcu83.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu84/see3camcu84.qml \
    ub22_qml/qtcam/UVCSettings/see3camcu31/seecamcu31.qml \
    ub22_qml/qtcam/UVCSettings/imx900USBCam/imx900USBCam.qml \
    ub22_qml/qtcam/UVCSettings/others/others.qml

### todo: clean up QML files. There is no need for distro specific QML
##    OTHER_FILES += qml/qtcam/Views/qtcam.qml
##
##    DISTFILES += \
##        qml/qtcam/UVCSettings/ascella/cx3-uvc.qml \
##        qml/qtcam/UVCSettings/barcode/barcode_camera.qml \
##        qml/qtcam/UVCSettings/cx3SNI/uvcExtCX3SNI.qml \
##        qml/qtcam/UVCSettings/eCAM512USB/ecam512usb.qml \
##        qml/qtcam/UVCSettings/ecam51_USB/ecam51_usb.qml \
##        qml/qtcam/UVCSettings/ecam82_USB/ecam82_usb.qml \
##        qml/qtcam/UVCSettings/ecam83_USB/ecam83_usb.qml \
##        qml/qtcam/UVCSettings/fscamcu135/fscamcu135.qml \
##        qml/qtcam/UVCSettings/h264cam/h264camExt.qml \
##        qml/qtcam/UVCSettings/nilecam20usb/nilecam20_usb.qml \
##        qml/qtcam/UVCSettings/nilecam30usb/nilecam30_usb.qml \
##        qml/qtcam/UVCSettings/see3cam11/uvc11.qml \
##        qml/qtcam/UVCSettings/see3cam130/uvc_130.qml \
##        qml/qtcam/UVCSettings/see3cam130A/uvc_130A.qml \
##        qml/qtcam/UVCSettings/see3cam130D/see3cam_130D.qml \
##        qml/qtcam/UVCSettings/see3cam1332/see3cam_1332.qml \
##        qml/qtcam/UVCSettings/see3cam135m/see3cam135m.qml \
##        qml/qtcam/UVCSettings/see3cam160/see3cam_160.qml \
##        qml/qtcam/UVCSettings/see3cam16cugm/see3cam16cugm.qml \
##        qml/qtcam/UVCSettings/see3cam20cug/see3cam_20cug.qml \
##        qml/qtcam/UVCSettings/see3cam24cug/see3cam_24cug.qml \
##        qml/qtcam/UVCSettings/see3cam27cug/see3cam27cug.qml\
##        qml/qtcam/UVCSettings/see3cam30/uvc_30.qml \
##        qml/qtcam/UVCSettings/see3cam40/uvc40.qml \
##        qml/qtcam/UVCSettings/see3cam50/uvc50.qml \
##        qml/qtcam/UVCSettings/see3cam50cug/see3cam50cug.qml \
##        qml/qtcam/UVCSettings/see3cam50cugm/see3cam_50cug_m.qml \
##        qml/qtcam/UVCSettings/see3cam51/uvc51.qml \
##        qml/qtcam/UVCSettings/see3cam80/uvc80.qml \
##        qml/qtcam/UVCSettings/see3cam81/uvc81.qml \
##        qml/qtcam/UVCSettings/see3cam_cu512M/see3camcu512m.qml \
##        qml/qtcam/UVCSettings/see3cam_cu81/see3cam_cu81.qml \
##        qml/qtcam/UVCSettings/see3camar0130/uvc_ar0130.qml \
##        qml/qtcam/UVCSettings/see3camcu130/uvc_cu130.qml \
##        qml/qtcam/UVCSettings/see3camcu135/uvc_cu135.qml \
##        qml/qtcam/UVCSettings/see3CamCu135mH01r1/see3CamCu135m_H01r1.qml \
##        qml/qtcam/UVCSettings/see3camcu136m/see3cam_cu136m.qml \
##        qml/qtcam/UVCSettings/see3camcu20/see3camcu20.qml \
##        qml/qtcam/UVCSettings/see3camcu200/see3camcu200.qml \
##        qml/qtcam/UVCSettings/See3CamCu210/see3camcu210.qml \
##        qml/qtcam/UVCSettings/see3camcu22/see3camcu22.qml \
##        qml/qtcam/UVCSettings/see3camcu27/see3camcu27.qml \
##        qml/qtcam/UVCSettings/see3camcu30/uvc_cu30.qml \
##        qml/qtcam/UVCSettings/see3camcu38/uvc_cu38.qml \
##        qml/qtcam/UVCSettings/see3camcu55/see3camcu55.qml \
##        qml/qtcam/UVCSettings/see3camcu55_MH/see3camcu55_mh.qml \
##        qml/qtcam/UVCSettings/see3camcu83/see3camcu83.qml \
##        qml/qtcam/UVCSettings/see3camcu84/see3camcu84.qml \
##        qml/qtcam/UVCSettings/see3camcu31/seecamcu31.qml \
##        qml/qtcam/UVCSettings/see3camcu200m/see3camcu200m.qml \
##        qml/qtcam/UVCSettings/imx900USBCam/imx900USBCam.qml \
##        qml/qtcam/UVCSettings/others/others.qml
