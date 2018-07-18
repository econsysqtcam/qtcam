# Add more folders to ship with the application, here
folder_01.source = qml/qtcam
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QT += widgets concurrent
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
    seecam_cu80.cpp \
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
    nilecam30_usb.cpp

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
    nilecam30_usb.h


INCLUDEPATH +=  $$PWD/v4l2headers/include \
                /usr/include \
                /usr/include/libusb-1.0

UNAME_MACHINE_32BIT = $$system(dpkg --print-architecture | grep -o "i386")
UNAME_MACHINE_64BIT = $$system(dpkg --print-architecture | grep -o "amd64")
BOARD_ARM64 = $$system(dpkg --print-architecture | grep -o "arm64")


contains(UNAME_MACHINE_64BIT, amd64):{
    message("x86_64 bit libs")
    LIBS += -lv4l2 -lv4lconvert \
        -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -ludev \
        -lusb-1.0 \
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
        -L/usr/lib/ -lturbojpeg \
        -L/usr/lib/aarch64-linux-gnu/ -levdev
}

QMAKE_CXX += -ggdb
QMAKE_CFLAGS_THREAD = -D__STDC_CONSTANT_MACROS      #For Ubuntu 12.04 compilation
QMAKE_CXXFLAGS_THREAD = -D__STDC_CONSTANT_MACROS    #For Ubuntu 12.04 compilation

OTHER_FILES += \
    qml/qtcam/Views/qtcam.qml
