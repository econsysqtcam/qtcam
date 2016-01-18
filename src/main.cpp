/*
 * main.cpp -- the main loop and the interface connection
 * Copyright © 2015  e-con Systems India Pvt. Limited
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
//#include <QQmlApplicationEngine>
//#include <QQmlComponent>
#include <QDateTime>
#include <QtWidgets/QWidget>
#include <QIcon>
#include <QStandardPaths>
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
#include "see3cam_cu130.h"
#include "seecam_ar0130.h"
#include "ascella.h"
#include "about.h"

//*! \mainpage Qtcam - A econ's camera product
// *
// * \section See3CAM_10CUG
// * Mono Camera
// * \ref See3CAM_10CUG
// * \section See3CAM_10CUG
// * Bayer Camera


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<Cameraproperty>("econ.camera.property",1,0,"Camproperty");
    qmlRegisterType<Videostreaming>("econ.camera.stream", 1, 0, "Videostreaming");
    qmlRegisterType<uvccamera>("econ.camera.uvcsettings", 1, 0, "Uvccamera");
    qmlRegisterType<See3CAM_11CUG>("econ.camera.see3cam11", 1, 0, "See3Cam11");    
    qmlRegisterType<See3CAM_AR130>("econ.camera.see3camar0130", 1, 0, "See3Camar0130");    
    qmlRegisterType<See3CAM_10CUG_Bayer>("econ.camera.see3cam10Bayer", 1, 0, "See3Cam10Bayer");
    qmlRegisterType<See3CAM_10CUG_Mono>("econ.camera.see3cam10Mono", 1, 0, "See3Cam10Mono");
    qmlRegisterType<See3CAM_80>("econ.camera.see3cam80", 1, 0, "See3Cam80");
    qmlRegisterType<See3CAM_CU50>("econ.camera.see3cam50", 1, 0, "See3Cam50");    
	qmlRegisterType<See3CAM_CU130>("econ.camera.see3cam130", 1, 0, "See3Cam130");
    qmlRegisterType<See3CAM_CU51>("econ.camera.see3cam51", 1, 0, "See3Cam51");
    qmlRegisterType<ASCELLA>("econ.camera.ascella", 1, 0, "Ascella");
    qmlRegisterType<See3CAM_Control>("econ.camera.see3camControl", 1, 0, "See3CamCtrl");
    qmlRegisterType<See3CAM_GPIOControl>("econ.camera.see3camGpioControl", 1, 0, "See3CamGpio");
    qmlRegisterType<AboutPage>("econ.camera.about", 1, 0, "AboutPage");

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
    viewer.rootContext()->setContextProperty("resolutionModel", &vs.resolution);
    viewer.rootContext()->setContextProperty("stillOutputFormatModel", &vs.stillOutputFormat);
    viewer.rootContext()->setContextProperty("videoOutputFormatModel", &vs.videoOutputFormat);
    viewer.rootContext()->setContextProperty("fpsAvailable", &vs.fpsList);
    viewer.rootContext()->setContextProperty("SystemPictureFolder",QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first());
    viewer.rootContext()->setContextProperty("SystemVideoFolder",QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).first());

    viewer.setMainQmlFile(QStringLiteral("/usr/share/qml/qtcam/videocapturefilter_QML/videocapturefilter_qml.qml"));
    QObject *rootObject = dynamic_cast<QObject*>(viewer.rootObject());

    QObject::connect(rootObject,SIGNAL(stopCamPreview()),rootObject,SLOT(triggerModeCapture()));

    //Setting the Window ICON
    QIcon icon("/usr/share/qml/qtcam/icon/images/icon.jpg");
    viewer.setIcon(icon);
    viewer.setTitle("Qtcam");
    viewer.showMaximized();
    return app.exec();
}
