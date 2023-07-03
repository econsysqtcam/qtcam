# Add more folders to ship with the application, here
folder_01.source = qml/qtcam
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

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
    see3camcu512m.cpp \
    see3cam16cugm.cpp \
    barcode_camera.cpp \
    renderer.cpp \
    see3cam_cu210.cpp \
    ecam_512usb.cpp

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
    see3camcu512m.h \
    see3cam16cugm.h \
    barcode_camera.h \
    renderer.h \
    see3cam_cu210.h \
    ecam_512usb.h


INCLUDEPATH +=  $$PWD/v4l2headers/include \
                /usr/include \
                /usr/include/libusb-1.0

UNAME_MACHINE_32BIT = $$system(dpkg --print-architecture | grep -o "i386")
UNAME_MACHINE_64BIT = $$system(dpkg --print-architecture | grep -o "amd64")
BOARD_ARM64 = $$system(dpkg --print-architecture | grep -o "arm64")

DISTRIBUTION_NAME = $$system(lsb_release -a | grep -o "bionic")
contains(DISTRIBUTION_NAME,bionic):{
QMAKE_CXX = "g++-5"
QMAKE_CXXFLAGS += -std=c++11
}

contains(UNAME_MACHINE_64BIT, amd64):{
    message("x86_64 bit libs")
    LIBS += -lv4l2 -lv4lconvert \       
        -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -ludev \
        -lusb-1.0 \
        -lpulse \
        -lasound\
        -L/usr/lib/ -lturbojpeg \
        -L/usr/lib/x86_64-linux-gnu/ -levdev
}

contains(UNAME_MACHINE_32BIT, i386):{
    message("x86_32 bit libs")
    LIBS += -lv4l2 -lv4lconvert \
        -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -ludev \
        -lusb-1.0 \
        -lpulse \
        -lasound\
        -L/usr/lib/ -lturbojpeg \
        -L/usr/lib/i386-linux-gnu/ -levdev
}

contains(BOARD_ARM64, arm64):{
    message("Arm64 libs")
    LIBS += -lv4l2 -lv4lconvert \
        -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -ludev \
        -lusb-1.0 \
        -lpulse \
        -lasound\
        -L/usr/lib/ -lturbojpeg \
        -L/usr/lib/aarch64-linux-gnu/ -levdev
}

#QMAKE_CXX += -ggdb
QMAKE_CFLAGS_THREAD = -D__STDC_CONSTANT_MACROS      #For Ubuntu 12.04 compilation
QMAKE_CXXFLAGS_THREAD = -D__STDC_CONSTANT_MACROS    #For Ubuntu 12.04 compilation
QMAKE_CFLAGS_ISYSTEM = -I                           #For Ubuntu 20.04

OTHER_FILES += \
    qml/qtcam/Views/qtcam.qml

DISTFILES += \
    qml/qtcam/UVCSettings/see3cam_cu81/see3cam_cu81.qml \
    qml/qtcam/UVCSettings/see3cam160/see3cam_160.qml \
    qml/qtcam/UVCSettings/ecam51_USB/ecam51_usb.qml \
    qml/qtcam/UVCSettings/ecam82_USB/ecam82_usb.qml \
    qml/qtcam/UVCSettings/see3camcu27/see3camcu27.qml \
    qml/qtcam/UVCSettings/see3CamCu135mH01r1/see3CamCu135m_H01r1.qml \
    qml/qtcam/UVCSettings/see3cam135m/see3cam135m.qml \
    qml/qtcam/UVCSettings/see3camcu136m/see3cam_cu136m.qml \
    qml/qtcam/UVCSettings/see3cam27cug/see3cam27cug.qml\
    qml/qtcam/UVCSettings/see3camcu83/see3camcu83.qml\
    qml/qtcam/UVCSettings/nilecam20usb/nilecam20_usb.qml \
    qml/qtcam/UVCSettings/see3cam50cugm/see3cam_50cug_m.qml \
    qml/qtcam/UVCSettings/see3cam_cu512M/see3camcu512m.qml \
    qml/qtcam/UVCSettings/see3cam16cugm/see3cam16cugm.qml \
    qml/qtcam/UVCSettings/eCAM512USB/ecam512usb.qml \
    qml/qtcam/UVCSettings/See3CamCu210/see3camcu210.qml \
    qml/qtcam/UVCSettings/barcode/barcode_camera.qml
