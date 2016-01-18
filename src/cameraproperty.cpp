/*
 * cameraproperty.cpp -- enumerate the available cameras and filters the e-con camera connected to the device
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

#include "cameraproperty.h"
#include <QDebug>

#define PRODUCTID "2560"
QStringListModel Cameraproperty::modelCam;
bool Cameraproperty::saveLog;

Cameraproperty::Cameraproperty()
{
    connect(this,SIGNAL(setFirstCamDevice(int)),&vidStr,SLOT(getFirstDevice(int)));
    connect(this,SIGNAL(setCamName(QString)),&vidStr,SLOT(getCameraName(QString)));
    connect(this,SIGNAL(logHandle(QtMsgType,QString)),this,SLOT(logWriter(QtMsgType,QString)));
    connect(&uvccam,SIGNAL(logHandle(QtMsgType,QString)),this,SLOT(logWriter(QtMsgType,QString)));
}

Cameraproperty::Cameraproperty(bool enableLog) {
	saveLog	= enableLog;
}

Cameraproperty::~Cameraproperty() {

}

void Cameraproperty::checkforDevice() {
    int deviceBeginNumber,deviceEndNumber;
    cameraMap.clear();
    deviceNodeMap.clear();
    int deviceIndex = 1;
    availableCam.clear();
    if(qDir.cd("/sys/class/video4linux/")) {
        QStringList filters,list;
        filters << "video*";
        qDir.setNameFilters(filters);
        list << qDir.entryList(filters,QDir::Dirs ,QDir::Name);
        qSort(list.begin(), list.end());        
        deviceBeginNumber = list.value(0).mid(5).toInt();   //Fetching all values after "video"
        deviceEndNumber = list.value(list.count()-1).mid(5).toInt();
        for(int qDevCount=deviceBeginNumber;qDevCount<=deviceEndNumber;qDevCount++) {
            QString qTempStr = qDir.path().append("/video" + QString::number(qDevCount));
            if(open("/dev/video" +QString::number(qDevCount),false)) {
                if (querycap(m_querycap)) {
                    QString cameraName = (char*)m_querycap.card;
                    if(cameraName.length()>22){
                        cameraName.insert(22,"\n");
                    }
                    //cameraMap.insert(qDevCount,cameraName);
                    cameraMap.insert(qDevCount,QString::number(deviceIndex,10));
                    deviceNodeMap.insert(deviceIndex,(char*)m_querycap.bus_info);
                    availableCam.append(cameraName);
                    deviceIndex++;
                    close();
                } else {
                    emit logWriter(QtCriticalMsg, "Cannot open device: /dev/video"+qDevCount);
                    return void();
                }
            } else {
                emit logHandle(QtCriticalMsg, qTempStr+"Device opening failed"+qDevCount);
            }
        }
    } else {
        emit logHandle(QtCriticalMsg,"/sys/class/video4linux/ path is Not available");
    }
    emit logHandle(QtDebugMsg,"Camera devices Connected to System: "+ availableCam.join(", "));
    //Modified by Nithyesh
    /*
     * Removed arg availableCam from function as it was unused.
     * Previous fn call was like
     * uvccam.findEconDevice(&availableCam,"video4linux");
     */
    uvccam.findEconDevice("video4linux");
    availableCam.prepend("----Select camera Device----");
    modelCam.setStringList(availableCam);
    //Modified by Nithyesh
    /*
     * Removed arg availableCam from function as it was unused.
     * Previous fn call was like
     * uvccam.findEconDevice(&availableCam,"hidraw");
     */
    uvccam.findEconDevice("hidraw");
}

void Cameraproperty::setCurrentDevice(QString deviceIndex,QString deviceName) {

    if(deviceIndex.isEmpty() || deviceName.isEmpty())
    {
        emit setFirstCamDevice(-1);
        return;
    }

    emit logHandle(QtDebugMsg,"Selected Device is: "+deviceName);
    if(deviceName == "----Select camera----")
        emit setFirstCamDevice(-1);
    else {
        emit setFirstCamDevice(cameraMap.key(deviceIndex));
        emit setCamName(deviceName);
        uvccam.getDeviceNodeName(deviceNodeMap.value(deviceIndex.toInt()));
    }
}

void Cameraproperty::createLogger() {
    if (saveLog){
	log.close();
	log.logFileCreation();
    }
}

void Cameraproperty::logWriter(QtMsgType msgType,QString tmpStr) {
    log.logHandler(msgType,tmpStr);
}

void Cameraproperty::logDebugWriter(QString tmpStr) {
    log.logHandler(QtDebugMsg,tmpStr);
}

void Cameraproperty::logCriticalWriter(QString tmpStr) {
    log.logHandler(QtCriticalMsg,tmpStr);
}
