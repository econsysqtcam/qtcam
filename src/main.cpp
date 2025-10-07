/*
 * main.cpp -- the main loop and the interface connection
 * Copyright © 2024  e-con Systems India Pvt. Limited
 *
 * This file is part of Qtcam.
 *
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QtWidgets/QWidget>
#include <QIcon>
#include <QStandardPaths>
#include<QProcess>
#include "qtquick2applicationviewer.h"
#include "cameraproperty.h"
#include "videostreaming.h"
#include "uvccamera.h"
#include "seecam_10cug_bayer.h"
#include "seecam_10cug_m.h"
#include "seecam_11cug.h"
#include "seecam_cu50.h"
#include "seecam_cu51.h"
#include "seecam_cu80.h"
#include "seecam_81.h"
#include "see3cam_cu40.h"
#include "see3cam_cu130.h"
#include "see3cam_cu135.h"
#include "see3cam_130.h"
#include "see3cam_130A.h"
#include "see3cam_cu20.h"
#include "seecam_ar0130.h"
#include "seecam_cu30.h"
#include "see3cam_cu38.h"
#include "see3cam_cu55.h"
// Added by Sankari : 07 Feb 2017
#include "see3cam_30.h"
#include "uvcExtCx3sni.h"
#include "audioinput.h"
#include "ascella.h"
#include "keyEventReceive.h"
#include "nilecam30_usb.h"
#include "h264camera.h"
#include "about.h"
#include "common.h"
#include "common_enums.h"
#include "fscam_cu135.h"
#include "see3camcu55_mh.h"
#include "see3cam_20cug.h"
#include "see3cam_cu22.h"
#include "see3cam_130d.h"
#include "see3cam_1332.h"
#include "see3cam_24cug.h"
#include "see3cam_cu81.h"
#include "ecam51_usb.h"
#include "see3cam_160.h"
#include "see3cam_cu27.h"
#include "ecam82_usb.h"
#include "ecam83_usb.h"
#include "see3cam_cu135m_h01r1.h"
#include "see3cam_cu136m.h"
#include "barcode_camera.h"
#include "see3cam_135m.h"
#include "nilecam20_usb.h"
#include "see3cam_27cug.h"
#include "renderer.h"
#include "see3cam_cu83.h"
#include "see3cam_cu512_m.h"
#include "see3cam_16cug_m.h"
#include "see3cam_cu210.h"
#include "ecam512_usb.h"
#include "see3cam_50cug_m.h"
#include "see3cam_50cug.h"
#include "see3cam_cu84.h"
#include "see3cam_cu200.h"
#include "see3cam_cu31.h"
#include "see3cam_cu200m.h"
#include "see3cam_cu200_h01r1.h"
#include "imx900usbcam.h"
#include "ecam514_usb.h"

#include "systemHelper.h"

//*! \mainpage Qtcam - A econ's camera product
// *
// * \section See3CAM_10CUG
// * Mono Camera
// * \ref See3CAM_10CUG
// * \section See3CAM_10CUG
// * Bayer Camera
int main(int argc, char *argv[])
{
    /*Indentifying OS version*/
    QProcess process;
    process.start("bash", QStringList() << "-c" << "cat /etc/os-release | grep \"PRETTY_NAME\" "); //Reading os-release file for checking OS
    process.waitForFinished();
    char *os_name = strdup(process.readAllStandardOutput());
    bool is20_04detected =false;
    bool is22_04detected = false;

    if(strstr(os_name,"20.04"))
    {
        is20_04detected = true;
    }
    else if(strstr(os_name,"22.04"))
    {
        is22_04detected = true;
    }

    QApplication app(argc, argv);
    app.setOrganizationName("econsystems");
    app.setOrganizationDomain("econsystems.com");

    qmlRegisterType<Cameraproperty>("econ.camera.property",1,0,"Camproperty");
    qmlRegisterType<Videostreaming>("econ.camera.stream", 1, 0, "Videostreaming");
    qmlRegisterType<CamKeyEventReceive>("econ.camera.keyEvent", 1, 0, "KeyEventRecv");
    qmlRegisterType<uvccamera>("econ.camera.uvcsettings", 1, 0, "Uvccamera");
    qmlRegisterType<UVCExtCx3SNI>("econ.camera.uvcExtsettings", 1, 0, "UvcExtcamera");
    qmlRegisterType<See3CAM_11CUG>("econ.camera.see3cam11", 1, 0, "See3Cam11");
    qmlRegisterType<See3CAM_AR130>("econ.camera.see3camar0130", 1, 0, "See3Camar0130");
    qmlRegisterType<See3CAM_10CUG_Bayer>("econ.camera.see3cam10Bayer", 1, 0, "See3Cam10Bayer");
    qmlRegisterType<See3CAM_10CUG_Mono>("econ.camera.see3cam10Mono", 1, 0, "See3Cam10Mono");
    qmlRegisterType<See3CAM_80>("econ.camera.see3cam80", 1, 0, "See3Cam80");
    qmlRegisterType<FSCAM_CU135>("econ.camera.fscamcu135", 1, 0, "Fscamcu135");

    //Added by Navya - 26 Aug 2019
    qmlRegisterType<See3CAM_20CUG>("econ.camera.see3cam20cug", 1, 0, "See3cam20cug");

    // Added by Sankari : 07 Feb 2017
    qmlRegisterType<See3CAM_30>("econ.camera.see3cam30", 1, 0, "See3Cam30");

    // Added by Sankari: 17 Dec 2016
    qmlRegisterType<See3CAM_81>("econ.camera.see3cam81", 1, 0, "See3Cam81");

    qmlRegisterType<See3CAM_CU50>("econ.camera.see3cam50", 1, 0, "See3Cam50");
    qmlRegisterType<See3CAM_CU130>("econ.camera.see3camcu130", 1, 0, "See3CamCu130");

    // Added by Sankari : 22 Feb 2017
    qmlRegisterType<See3CAM_CU135>("econ.camera.see3camcu135", 1, 0, "See3CamCu135");

    // Added by Sankari: 21 Apr 2017
    qmlRegisterType<See3CAM_CU40>("econ.camera.see3camcu40", 1, 0, "See3Camcu40");

    qmlRegisterType<See3CAM_130>("econ.camera.see3cam130", 1, 0, "See3Cam130");
    qmlRegisterType<See3CAM_130A>("econ.camera.see3cam130A", 1, 0, "See3Cam130A");
    qmlRegisterType<See3CAM_CU30>("econ.camera.see3camcu30", 1, 0, "See3Camcu30");
    qmlRegisterType<See3CAM_CU38>("econ.camera.see3camcu38", 1, 0, "See3Camcu38");
    qmlRegisterType<NILECAM30_USB>("econ.camera.nilecam30usb", 1, 0, "NileCam30usb");
    qmlRegisterType<See3CAM_CU55>("econ.camera.see3camcu55", 1, 0, "See3camCu55");
    // Added by Navya :08 July 2019
    qmlRegisterType<See3CAM_CU55_MH>("econ.camera.see3camcu55mh",1,0,"See3camcu55MH");
    // Added by Sankari: 27 July 2017
    qmlRegisterType<See3CAM_CU20>("econ.camera.see3camcu20", 1, 0, "See3Camcu20");
    qmlRegisterType<See3CAM_CU22>("econ.camera.see3camcu22", 1, 0, "See3Camcu22");
    qmlRegisterType<See3CAM_CU51>("econ.camera.see3cam51", 1, 0, "See3Cam51");
    // Added by Sankari - 5 Oct 2018
    qmlRegisterType<H264Camera>("econ.camera.h264cam", 1, 0, "H264camera"); //ecam22_usb
    qmlRegisterType<ASCELLA>("econ.camera.ascella", 1, 0, "Ascella");
    qmlRegisterType<See3CAM_Control>("econ.camera.see3camControl", 1, 0, "See3CamCtrl");
    qmlRegisterType<See3CAM_GPIOControl>("econ.camera.see3camGpioControl", 1, 0, "See3CamGpio");
    qmlRegisterType<AboutPage>("econ.camera.about", 1, 0, "AboutPage");

    qmlRegisterType<AudioInput>("econ.camera.audioinput", 1, 0, "Audioinput");
    //Added by M.Vishnu Murali
    qmlRegisterType<See3CAM_130D>("econ.camera.see3cam130D", 1, 0, "See3Cam130D");
    qmlRegisterType<See3CAM_1332>("econ.camera.see3cam1332", 1, 0, "See3Cam1332");
    qmlRegisterType<See3CAM_24CUG>("econ.camera.see3cam24cug", 1, 0, "See3Cam24CUG");
    qmlRegisterType<See3CAM_CU81>("econ.camera.see3camcu81", 1, 0, "See3CamCU81");
    qmlRegisterType<ecam51_USB>("econ.camera.ecam51_usb",1,0,"Ecam51USB");

    qmlRegisterType<See3CAM_160>("econ.camera.see3cam160",1,0,"See3cam160");
    qmlRegisterType<See3CAM_CU27>("econ.camera.see3camcu27",1,0,"See3camCU27");
    qmlRegisterType<ecam82_USB>("econ.camera.ecam82_usb",1,0,"Ecam82USB");
    qmlRegisterType<See3CAM_CU135M_H01R1>("econ.camera.see3CamCu135mH01r1",1,0,"See3CAM_CU135M_H01R1");
    qmlRegisterType<See3CAM_135M>("econ.camera.see3cam_135m",1,0,"See3cam135M");
    qmlRegisterType<See3CAM_CU136M>("econ.camera.see3cam_cu136m",1,0,"See3camCU136M");
    qmlRegisterType<NILECAM20_USB>("econ.camera.nilecam20_usb",1,0,"NileCam20_USB");
    qmlRegisterType<SEE3CAM_27CUG>("econ.camera.see3cam_27cug",1,0,"See3CAM_27CUG");
    qmlRegisterType<SEE3CAM_CU83>("econ.camera.see3camcu83",1,0,"See3Cam_CU83");
    qmlRegisterType<BarcodeCamera>("econ.camera.barcode_camera",1,0,"BarcodeCamera");
    qmlRegisterType<ecam83_USB>("econ.camera.ecam83_usb",1,0,"Ecam83USB");//while exposing to qml it is must that first letter should be in caps.
    qmlRegisterType<SEE3CAM_50CUGM>("econ.camera.see3cam_50cug_m", 1, 0, "SEE3CAM_50CUGM");
    qmlRegisterType<SEE3CAM_CU512M>("econ.camera.see3camcu512m", 1, 0, "See3CAM_CU512M");
    qmlRegisterType<SEE3CAM_16CUGM>("econ.camera.see3cam16cugm", 1, 0, "See3CAM_16CUGM");
    qmlRegisterType<See3CAM_CU210>("econ.camera.see3camcu210", 1, 0, "See3CAM_CU210");
    qmlRegisterType<ECAM_512USB>("econ.camera.ecam_512usb", 1, 0, "ECAM_512USB");
    qmlRegisterType<SEE3CAM_50CUG>("econ.camera.see3cam50cug", 1, 0, "SEE3CAM_50CUG");
    qmlRegisterType<SEE3CAM_CU84>("econ.camera.see3camcu84", 1, 0, "SEE3CAM_CU84");
    qmlRegisterType<SEE3CAM_CU200>("econ.camera.see3camcu200", 1, 0, "SEE3CAM_CU200");
    qmlRegisterType<SEE3CAM_CU200M>("econ.camera.see3camcu200m", 1, 0, "SEE3CAM_CU200M");
    qmlRegisterType<See3CAM_CU200M_H01R1>("econ.camera.see3camcu200mh01r1", 1, 0, "See3CAM_CU200M_H01R1");
    qmlRegisterType<SEE3CAM_CU31>("econ.camera.see3camcu31", 1, 0, "SEE3CAM_CU31");
    qmlRegisterType<IMX900USBCAM>("econ.camera.imx900usbcam", 1, 0, "IMX900USBCAM");
    qmlRegisterType<ECAM514_USB>("econ.camera.ecam514_usb", 1, 0, "ECAM514_USB");

    //For IR Window
    qmlRegisterType<ImageItem>("econ.camera.qimagerenderer", 1, 0, "ImageItem");

    //Added by Dhurka - 13th Oct 2016
    /**
     * @brief qmlRegisterType<CommonEnums> - Access the ENUM list to QML
     */
    qmlRegisterType<CommonEnums>("cameraenum",1,0,"CommonEnums");

    QtQuick2ApplicationViewer viewer;

    //Create a object for Camera property
    Cameraproperty camProperty;

    if(argc > 1){
        if(strcmp(argv[1],"-l") == 0 || strcmp(argv[1],"--log") == 0){
            Cameraproperty camPropertyParam(true);
        }
        else{
            qDebug()<<"Usage: qtcam [OPTION]";
            qDebug()<<"-l, --log    to create log in a directory\n";
            return -1;
        }
    }

    viewer.rootContext()->setContextProperty("camModels", &camProperty.modelCam);

    Videostreaming vs;
    AudioInput audio;

    if(is20_04detected)
    {
        viewer.rootContext()->setContextProperty("is20_04detcted", QVariant(true));
        audio.is20_04 = true;
    }
    else if(is22_04detected)
    {
        viewer.rootContext()->setContextProperty("is22_04detcted", QVariant(true));
        audio.is22_04 = true;
    }
    else {
         viewer.rootContext()->setContextProperty("is20_04detcted", QVariant(false));
         viewer.rootContext()->setContextProperty("is22_04detcted", QVariant(false));
         audio.is20_04 = false;
         audio.is22_04 = false;
    }


    SystemHelper systemHelper;
    viewer.rootContext()->setContextProperty("systemHelper", &systemHelper);
    viewer.rootContext()->setContextProperty("resolutionModel", &vs.resolution);
    viewer.rootContext()->setContextProperty("stillOutputFormatModel", &vs.stillOutputFormat);
    viewer.rootContext()->setContextProperty("videoOutputFormatModel", &vs.videoOutputFormat);
    viewer.rootContext()->setContextProperty("fpsAvailable", &vs.fpsList);
    viewer.rootContext()->setContextProperty("encodersModel", &vs.encoderList);
    viewer.rootContext()->setContextProperty("audioinputDevModel", &audio.audioinputDeviceList);
    viewer.rootContext()->setContextProperty("audioSupportedFormatList", &audio.audiosupportedFmtListModel);
    viewer.rootContext()->setContextProperty("audioChannelCountModel", &audio.audioChannelCountModel);

    //Creating directory in Home for saving videos & pictures instead of saving it in default root folder
    if(is20_04detected || is22_04detected)
    {
        // Create the folder in the home directory
        QString picturesPath = QDir::homePath() + "/Pictures";
        QDir pictureDir(picturesPath);

        if (!pictureDir.exists())
        {//where the directory does not exist
            pictureDir.mkdir(picturesPath);
        }

        // Create the folder in the home directory
        QString videosPath = QDir::homePath() + "/Videos";
        QDir videosDir(videosPath);

        if (!videosDir.exists())
        {//where the directory does not exist
            videosDir.mkdir(videosPath);
        }

        viewer.rootContext()->setContextProperty("SystemPictureFolder",picturesPath);
        viewer.rootContext()->setContextProperty("SystemVideoFolder",videosPath);
    }
    else
    {
        viewer.rootContext()->setContextProperty("SystemPictureFolder",QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
        viewer.rootContext()->setContextProperty("SystemVideoFolder",QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).first());
    }



#if LAUNCHPAD
    viewer.setMainQmlFile(QStringLiteral("/usr/share/qml/qtcam/Views/qtcam.qml"));
#else
    viewer.setMainQmlFile(QStringLiteral("qml/qtcam/Views/qtcam.qml"));
#endif

    viewer.rootContext()->setContextProperty("helperQml", &vs.helperObj);

    QObject *rootObject = dynamic_cast<QObject*>(viewer.rootObject());

    QObject::connect(rootObject,SIGNAL(stopCamPreview()),rootObject,SLOT(triggerModeCapture()));


    //Setting the Window ICON
#if LAUNCHPAD
    QIcon icon("/usr/share/qml/qtcam/icon/images/icon.jpg");
#else
    QIcon icon("qml/qtcam/icon/images/icon.jpg");
#endif
    viewer.setIcon(icon);
    viewer.setTitle("Qtcam");

    //Added by Geethalakshmi : 23rd Dec 2024
    //Bug fix for fps drop issue in 27cug and cu83 on minimizing main window
    //This minimizes the irwindow on minimizing the main window.
    QQuickWindow *window = qobject_cast<QQuickWindow*>(viewer.contentItem()->window());
    viewer.rootContext()->setContextProperty("qmlWindow", window);
    if (window) {
        QObject::connect(window, &QQuickWindow::windowStateChanged, [&](Qt::WindowState state) {
            if (state == Qt::WindowMinimized) {
                QObject *rootObject = viewer.rootObject();
                if (rootObject) {
                    // Call the function defined in QML to minimize the irPreview window
                    QVariant returnedValue;
                    QMetaObject::invokeMethod(rootObject, "minimizeIrPreviewWindow", Q_RETURN_ARG(QVariant, returnedValue));
                 }
             }
              else if (state == Qt::WindowMaximized || state == Qt::WindowNoState || state == Qt::WindowFullScreen || state == Qt::WindowActive) {
                  QObject *rootObject = viewer.rootObject();
                  if (rootObject) {
                      // Call the function defined in QML to activate the irPreview window only when the main winow is in either one of these states.
                      QVariant returnedValue;
                      QMetaObject::invokeMethod(rootObject, "maxMainWindow", Q_RETURN_ARG(QVariant, returnedValue));
                    }
                }
          });
     }

    //Bug fix: There's no "close,Restore Down,minimize" button on the top of the QTCAM when restore the application
    viewer.show();
    return app.exec();
}
