/*
 * uvccamera.h -- Filter e-con camera and common features of e-con camera
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

#ifndef UVCCAMERA_H
#define UVCCAMERA_H

#include <QtGlobal>
#include <QObject>
#include <QDebug>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include "libudev.h"
#include <libusb.h>
#include <QMap>

/* Report Numbers */
#define APPLICATION_READY 	0x12
#define READFIRMWAREVERSION	0x40
#define GETCAMERA_UNIQUEID	0x41
#define ENABLEMASTERMODE	0x50
#define ENABLETRIGGERMODE	0x51
#define ENABLE_CROPPED_VGA_MODE	0x52
#define ENABLE_BINNED_VGA_MODE	0x53

#define OS_CODE			0x70
#define LINUX_OS		0x01
#define CAPTURE_COMPLETE	0x71

#define CAMERA_CONTROL_80	0x60
#define CAMERA_CONTROL_50	0x64
#define CAMERA_CONTROL_51	0x65
#define CAMERA_CONTROL_AR0130	0x68
#define GET_FOCUS_MODE		0x01
#define SET_FOCUS_MODE		0x02
#define GET_FOCUS_POSITION	0x03
#define SET_FOCUS_POSITION	0x04
#define GET_FOCUS_STATUS	0x05
#define GET_FLASH_LEVEL		0x06
#define GET_TORCH_LEVEL		0x07
#define SET_FLASH_LEVEL		0x08
#define SET_TORCH_LEVEL		0x09

#define CONTINOUS_FOCUS		0x01
#define MANUAL_FOCUS		0x02
#define SINGLE_TRIG_FOCUS	0x03

#define FOCUS_FAILED		0x00
#define FOCUS_SUCCEEDED		0x01
#define FOCUS_BUSY		0x02

#define FLASH_OFF		0x00
#define FLASH_ON		0x01

#define SET_FAIL		0x00
#define SET_SUCCESS		0x01

#define GPIO_OPERATION		0x20
#define GPIO_GET_LEVEL		0x01
#define GPIO_SET_LEVEL		0x02

#define GPIO_LOW		0x00
#define GPIO_HIGH		0x01
#define GPIO_LEVEL_FAIL		0x00
#define GPIO_LEVEL_SUCCESS	0x01

#define WHITE_BAL_CONTROL	0x61
#define GET_WB_MODE		0x01
#define SET_WB_MODE		0x02
#define GET_WB_GAIN		0x03
#define SET_WB_GAIN		0x04
#define SET_WB_DEFAULTS		0x05
#define SET_ALL_WB_GAINS	0x06

#define WB_AUTO			0x01
#define WB_MANUAL		0x02

#define WB_RED			0x01
#define WB_GREEN		0x02
#define WB_BLUE			0x03

#define WB_FAIL			0x00
#define WB_SUCCESS		0x01


/* GPIO pins of SEE3CAM_80USB Products */

#define GPIO_80_OUT	0x14
#define GPIO_80_IN1	0x13
#define GPIO_80_IN2	0x21

/* GPIO pins of SEE3CAM_50USB Products */

#define GPIO_50_OUT1	0x18
#define GPIO_50_OUT2	0x14
#define GPIO_50_IN1	0x13
#define GPIO_50_IN2	0x16

#define BUFFER_LENGTH		65
#define TIMEOUT             2000

#define SUCCESS			1
#define FAILURE			-1

#define DEVICE_NAME_MAX				32		// Example: /dev/hidraw0
#define MANUFACTURER_NAME_MAX		64		// Example: e-con Systems
#define PRODUCT_NAME_MAX			128		// Example: e-con's 1MP Monochrome Camera
#define HID_LIST_MAX				32

/* SEE3CAM_CU130 */

#define CAMERA_CONTROL_CU130	0x69
#define GET_SCENE_MODE		0x01
#define SET_SCENE_MODE		0x02
#define GET_SPECIAL_EFFECT	0x03
#define SET_SPECIAL_EFFECT	0x04

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

/* SEE3CAM_CU51 */

#define GET_EXPOSURE_VALUE	0x01
#define SET_EXPOSURE_VALUE	0x02
#define TRIGGER_STILL_CAPTURE	0x03
#define GET_TORCH_LEVEL_51	0x04
#define SET_TORCH_LEVEL_51	0x05

#define EXP_FAIL		0x00
#define EXP_SUCCESS		0x01

#define TRIGGER_FAIL		0x00
#define TRIGGER_SUCCESS		0x01

/* Ascella camera */
#define ASCELLA_VID 0x04b4
#define ASCELLA_PID 0x00c3
#define ASCELLA_BUFLEN 64
#define ASCELLA_TIMEOUT 1000
#define ASCELLA_DEFAULT_MAXFRAMERATE 119

#define LEDSTATUS_MODE				0xF031
#define LED_BRIGHTNESS				0xF033
#define AUTOEXPOSURE				0xF022
#define AUTOFOCUSMODE				0xF041
#define SKIPFOCUSMODE				0xF040
#define COLORMODE					0xF051
#define CM_BLACKWHITE				0xF058
#define NOISE_REDUCTION				0xF059
#define SCENEMODE					0xF018
#define LIMITFRAMERATE				0xF02F

#define SCENEMODE_NORMAL			0x00
#define SCENEMODE_DOCUMENTSCANNER	0x20
#define FP_DISABLE					0x00
#define FP_APPLYMAX					0x03


class uvccamera: public QObject
{
    Q_OBJECT
public:
    uvccamera();

    QString titleText();
    void settitleText(const QString &t);

    QString comboBoxCameraText();
    void setcomboBoxCameraText(const QString &t);

    //Q_INVOKABLE void setCameraName(const QString msg);
    //QString getCameraName();

    //Modified by Nithyesh
    /*
     * Removed arg QStringList from function as it was unused.
     * Previous fn signature was
     * int uvccamera::findEconDevice(QStringList, QString)
     */
    int findEconDevice(QString);
    void getDeviceNodeName(QString);
    int closeAscellaDevice();

    static libusb_device_handle* handle;  /* handle for USB device */
    int kernelDriverDetached;

    static QMap<QString, QString> cameraMap;
    static QMap<QString, QString> serialNumberMap;
    static int hid_fd;
    static QString hidNode;

private:
    /**
     * @brief Read the Firmware Version
     *  - This function is used to read the firmware version from the device
     * @param pMajorVersion
     * Prints the Major version of the firmware
     * @param pMinorVersion1
     * Prints the Minor version1 of the firmware
     * @param pMinorVersion2
     * Prints the Minor version2 of the firmware
     * @param pMinorVersion3
     * Prints the Minor version3 of the firmware
     * @return
     * true - Successfully read firmware version
     * false - Failed to read firmware version
     */
    bool readFirmwareVersion(quint8 *pMajorVersion, quint8 *pMinorVersion1, quint16 *pMinorVersion2, quint16 *pMinorVersion3);

    bool sendOSCode();

    QString openNode;

protected:

    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    const char	*hid_device;

public:
    QString _title;
    QString _text;
    QString message;

    unsigned int getTickCount();

signals:
    void logHandle(QtMsgType,QString);
    void stopPreview(QString);
    void titleTextChanged(QString _title,QString _text);
    void serialNumber(QString serialNumber);

public slots:
    /**
     * @brief Get the firmware version of the camera
     *  - Reads the firmware version and emits the signal titleTextChanged(), "_text" variable of the signal
     * contains the firmware version.
     *  - Firmware version is of 4 digits
     */
    void getFirmWareVersion();

    /**
     * @brief getSerialNumber
     */
    void getSerialNumber();

    /**
     * @brief initExtensionUnit
     *  - Open the HID Node of the camera
     * @param cameraName - Camera Name
     * @return
     */
    bool initExtensionUnit(QString cameraName);

    /**
     * @brief Close the HIDRAW DEVICE
     *  - This function will close the last opened HID device
     * @
     */
    void exitExtensionUnit();

    /**
     * @brief initExtensionUnitAscella
     *  - Open the hid handle for Ascella
     * @return
     */
    int initExtensionUnitAscella();

    /**
     * @brief Close the hid handle
     *  - This function will close the last opened HID device
     * @
     */
    int exitExtensionUnitAscella();
};

class See3CAM_Control: public uvccamera {
    Q_OBJECT
public:

    /**
     * @brief The flashTorchState enum are used to set state(Disable/Enable) of flash/torch of the camera.
     */
    enum flashTorchState {
    flashOn = 1,
    flashOff = 0,
    torchOn = 1,
    torchOff = 0
    };

    /**
     * @brief Get the torch state
     *  - Reads the torch state from the device
     * @param torchState
     * This value will hold the state level of the torch (either 0-torch not enabled or 1-torch enabled)
     * @param cameraName
     * cameraName - Camera physical name
     * @return
     * true - read successfully the torch state level, false- fails to read the torch state level
     */
    bool getTorchState(quint8 *torchState, QString cameraName);

    /**
     * @brief   Set the Torch state of the Device.
     * @param torchState Disable/Enable Torch
     * @return
     * true - Success (Torch state is set as per the torchState).
     * @return
     * false - Failure (Torch state is not set as per the torchState).
     */
    bool setTorchState(flashTorchState torchState, QString cameraName);

    /**
     * @brief Get the flash state
     * @param Level
     * This value will hold the state level of the flash (either 0-flash not enabled or 1-flash is enabled)
     * @param cameraName
     * cameraName - Camera physical name
     * @return
     * true - read successfully the flash state level, false- fails to read the flash state level
     */
    bool getFlashState(quint8 *flashState, QString cameraName);

    /**
     * @brief   Set the Flash state of the Device.
     * @param flashState Disable/Enable Flash
     * @return
     * true - Success (Flash state is set as per the flashState).
     * @return
     * false - Failure (Torch state is not set as per the flashState).
     */
    bool setFlashState(flashTorchState flashState, QString cameraName);

    flashTorchState flashCheckBoxState;
    flashTorchState torchCheckBoxState;

signals:
    void deviceStatus(QString title, QString message);
public slots:

    void setFlashControlState(const int flashState,QString cameraName);
    void setTorchControlState(const int torchState, QString cameraName);
};


class See3CAM_GPIOControl: public QObject {
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

signals:
    void gpioLevel(unsigned char level);
    void deviceStatus(QString title, QString message);
public:

    /**
     * @brief The camGpioPin enum are used to set/read value for gpio Pin (OUT1,OUT2,OUT3,IN1,IN2,IN3) of the camera.
     */
    enum  camGpioPin{
    OUT1 = 24,
    OUT2 = 20,
    OUT3 = 21,
    IN1 = 19,
    IN2 = 22,
    IN3 = 33
    };

    Q_ENUMS(camGpioPin)
    /**
     * @brief The camGpioValue enum are used to set status (High/Low) for the Gpio pin.
     */
    enum camGpioValue{
    High = 1,
    Low = 0
    };

     Q_ENUMS(camGpioValue)

public slots:
    /**
     * @brief  Get the GPIO status(High/Low) for particular Gpio pin
     * @param gpioPinNumber  Gpio Pin(OUT1,OUT2,IN1,IN2,IN3) which need to be read
     * @param gpioValue   Status(High=1/Low=0) of the Gpio pin.
     * @return
     * true -successfully read Gpio Status for particular Gpio pin.
     * @return
     * false -failed to read Gpio Status for particular Gpio pin.
     */
    void getGpioLevel(camGpioPin gpioPinNumber);
    /**
     * @brief   Set the GPIO status(High=1/Low=0) for the particular Gpio pin.
     * @param gpioPin  pin(OUT1) for which user needs to  set the status.
     * @param gpioValue  Set Status(High/Low) for the Gpio pin.
     * @return
     * true - Successful to change the GPIO level.
     * @return
     * false - Failed to change the GPIO level.
     */
     void setGpioLevel(camGpioPin gpioPin,camGpioValue gpioValue);
};

class See3CAM_ModeControls {
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
public:
    /**
     * @brief Enable Camera in Master Mode
     *  - This function enables the camera in master mode
     * @return
     * true - On Success, false - On Failure
     */
    bool enableMasterMode();

    /**
     * @brief Enable Camera in trigger mode
     *  - This function enables the camera in trigger mode
     * @return
     * true - On Success, false - On Failure
     */
    bool enableTriggerMode();
};

#endif // UVCCAMERA_H
