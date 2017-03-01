/*
 * cameraproperty.h -- enumerate the available cameras and filters the e-con camera connected to the device
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

#ifndef CAMERAPROPERTY_H
#define CAMERAPROPERTY_H
#include <QQuickPaintedItem>
#include <QQuickImageProvider>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QDir>
#include <QStringList>
#include <QStringListModel>
#include <QQuickView>
#include <QQmlContext>
#include <QStandardItemModel>
#include <QTimer>
#include <qtquick2applicationviewer.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include "v4l2-api.h"
#include "uvccamera.h"
#include "videostreaming.h"
#include "logger.h"
#include "libudev.h"
class Cameraproperty : public QObject, public v4l2
{
    Q_OBJECT
public:
    Cameraproperty();
    Cameraproperty(bool enableLog);
    ~Cameraproperty();
    QStringList availableCam;

    uvccamera uvccam;
    Videostreaming vidStr;
    logger log;
    void checkforEconDevice(QStringList*);

    static QStringListModel modelCam;
    static bool saveLog;

private:
    QDir qDir;
    struct v4l2_capability m_querycap;
    QMap<int, QString> cameraMap;
    QMap<int, QString> deviceNodeMap;


public slots:
    /**
     * @brief Check e-con Cameras
     *  - List all the camera devices detected in the system
     *  - All the camera device name are listed in a QStringlist variable
     *  - E-con camera will be filtered from the available list
     *  - Flitered list is displayed to the view
     */
    void checkforDevice();

    /**
     * @brief Create a log file by passing to Logger class
     *  - A Log file will be created if the user call this function
     */
    void createLogger();

    /**
     * @brief setCurrentDevice
     *  - Used to get device index and pass it to the signals
     * @param
     *  deviceIndex - Selected from the UI, passed to this function
     *  deviceName - Selected from the UI, passed to this function
     */
    void setCurrentDevice(QString,QString);

    /**
     * @brief Write the data into log file by passing the data to Logger class
     *  - Data passed will be written to the log file
     * @param
     * msgType - Message type may debug, critical, warning, fatal
     * @param
     * tmpStr - Data to be written in the file
     */
    void logWriter(QtMsgType,QString);

    /**
     * @brief Write the data into log file by passing the data to Logger class
     *  - Data passed will be written to the log file
     * @param
     * tmpStr - Only Debug message type data to be written in the file
     */
    void logDebugWriter(QString);

    /**
     * @brief Write the data into log file by passing the data to Logger class
     *  - Data passed will be written to the log file
     * @param
     * tmpStr - Only Critical message type data to be written in the file
     */
    void logCriticalWriter(QString);
    /**
     * @brief selectedDeviceEnum - This slot contains the selected camera enum
     * @param selectedCameraEnum - Camera enum name
     */
    void selectedDeviceEnum(CommonEnums::ECameraNames selectedCameraEnum);
    //Added by Dhurka - 17th Oct 2016
    /**
     * @brief openHIDDevice - This slot is used to open HID device
     * @param deviceName - device name
     */
    void openHIDDevice(QString deviceName);

    void closeLibUsbDeviceAscella();

signals:

    void setFirstCamDevice(int);
    void setCamName(QString);
    void logHandle(QtMsgType,QString);
    /**
     * @brief currentlySelectedCameraEnum - This signal is used to emit selected camera enum value to
     * qtcam.qml file
     * @param selectedDevice
     */
    void currentlySelectedCameraEnum(int selectedDevice);
    //Added by Dhurka - 18th Oct 2016
    /**
     * @brief initExtensionUnitSuccess - This signal is used to emit after HID initialization is success
     */
    void initExtensionUnitSuccess();
};


#endif // CAMERAPROPERTY_H
