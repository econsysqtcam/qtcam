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
#include "common_enums.h"
#include <stddef.h>
#include <dirent.h>
#include <string.h>
#include <libevdev-1.0/libevdev/libevdev.h>

QStringListModel Cameraproperty::modelCam;
bool Cameraproperty::saveLog;
int Cameraproperty::event_fd;

Cameraproperty::Cameraproperty()
{
    connect(this,SIGNAL(setFirstCamDevice(int)),&vidStr,SLOT(getFirstDevice(int)));
    connect(this,SIGNAL(setCamName(QString)),&vidStr,SLOT(getCameraName(QString)));
    //Added by Dhurka - 13th Oct 2016
    /**
     * @brief connect - This signal is used to send the currently selected device to uvccamera.cpp filr
     */
    connect(this,SIGNAL(setCamName(QString)),&uvccam,SLOT(currentlySelectedDevice(QString)));
    connect(this,SIGNAL(logHandle(QtMsgType,QString)),this,SLOT(logWriter(QtMsgType,QString)));
    connect(&uvccam,SIGNAL(logHandle(QtMsgType,QString)),this,SLOT(logWriter(QtMsgType,QString)));
    //Added by Dhurka - 13th Oct 2016
    /**
     * @brief connect - This signal is used to send the currently selected camera enum to videostreaming.cpp
     */
    connect(&uvccam,SIGNAL(currentlySelectedCameraEnum(CommonEnums::ECameraNames)),&vidStr,SLOT(selectedCameraEnum(CommonEnums::ECameraNames)));
    //Added by Dhurka - 13th Oct 2016
    /**
     * @brief connect - This signal is used to send the selected camera enum value to QML for commparision instead of camera name
     */
    connect(&uvccam,SIGNAL(currentlySelectedCameraEnum(CommonEnums::ECameraNames)),this,SLOT(selectedDeviceEnum(CommonEnums::ECameraNames)));
    // Added by Sankari: To notify user about hid access 
    // 07 Dec 2017
    connect(&uvccam,SIGNAL(hidWarningReceived(QString, QString)),this,SLOT(notifyUser(QString, QString)));
}

Cameraproperty::Cameraproperty(bool enableLog) {
	saveLog	= enableLog;
}

Cameraproperty::~Cameraproperty() {

}
//Added by Dhurka - 13th Oct 2016
/**
 * @brief Cameraproperty::selectedDeviceEnum - This is used to send the Camera enum name to QML
 * @param selectedCameraEnum - Selected enum value
 */
void Cameraproperty::selectedDeviceEnum(CommonEnums::ECameraNames selectedCameraEnum)
{
    emit currentlySelectedCameraEnum((int)selectedCameraEnum);
}



//Modified by Sankari: 3 Mar 2018
/**
 * @brief Cameraproperty::openEventNode - Open respective event node in the path  /dev/input/eventX/ for selected camera
 * @param Businfo - pci bus information using [querycap.bus_info] in v4l2 query capability
 */
void Cameraproperty::openEventNode(QString businfo){ //open device event path file Ex: /dev/input/eventX/

    int deviceEventCount = 0;

    DIR *dir;
    struct dirent *ent;

    // open directory name /dev/input
    if ((dir = opendir ("/dev/input")) != NULL) {

        /* read all the files and directories /dev/input directory */
        while ((ent = readdir (dir)) != NULL) {

            // If file "event" is present , open /dev/input/eventX, open and read the physical location. If physicallocation and businfo of
            // selected camera matches, we can conclude that this event tracking file for currently selected camera.
            if(strstr(ent->d_name, "event")){

                event_fd = ::open("/dev/input/event" +QString::number(deviceEventCount).toLatin1(), O_RDONLY);
                if(event_fd >= 0){
                    struct libevdev *dev;

                    int rc = libevdev_new_from_fd(event_fd, &dev); //  allocate the device structure event file
                    if (rc < 0){
                        fprintf(stderr, "error: %d %s\n", -rc, strerror(-rc));
                        libevdev_free(dev);
                    }

                    const char *chdevPhyLocation;
                    QString devPhyLocation;

                    chdevPhyLocation = libevdev_get_phys(dev); // get physical location of the device

                    devPhyLocation   = QString::fromUtf8(chdevPhyLocation).toLower();

                    libevdev_free(dev); // free the device event file

                    int pos = devPhyLocation.indexOf(businfo);
                    if (pos >= 0)
                    {  // camera device is present
                        break;
                    }else{
			// close event file opened not related to camera device
                        ::close(event_fd);
                    }
                }else{
                    qDebug()<<"Cant open the file /dev/input/event<"<<deviceEventCount;
                    qDebug()<<"Please make sure you are launching the application with sudo";
                }

                deviceEventCount++;
            }

        }
        closedir (dir);

    } else {
        /* could not open directory */
        perror ("Opening /dev/input directory is failed");
    }
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
                    //Removed the commented code by Dhurka - 13th Oct 2016
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
    availableCam.prepend("----Select Camera Device----");
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
    //Modified by Dhurka - Aligned the braces - 13th Oct 2016
    if(deviceName == "----Select camera----")
    {
        emit setFirstCamDevice(-1);
    }
    else
    {
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
//Added by Dhurka - 17th Oct 2016
void Cameraproperty::openHIDDevice(QString deviceName)
{
    uvccam.exitExtensionUnit();
    deviceName.remove(QRegExp("[\n\t\r]"));
    bool hidInit = uvccam.initExtensionUnit(deviceName);    
    if(hidInit)
    {
        emit initExtensionUnitSuccess();
    }
}

void Cameraproperty::closeLibUsbDeviceAscella(){
    uvccam.exitExtensionUnitAscella();
}

// Added by Sankari: To notify user about warning from uvccamera.cpp
// 07 Dec 2017
void Cameraproperty::notifyUser(QString title, QString text){
    emit notifyUserInfo(title, text);
}
