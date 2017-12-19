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

#include "common_enums.h"

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
#define GET_INPUT_GPIO_LEVEL 0x04

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


#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

/* SEE3CAM_CU135 */
#define CAMERA_CONTROL_CU135             0x81
#define SET_SPECIAL_EFFECT_MODE_CU135    0x04
#define GET_SPECIAL_EFFECT_MODE_CU135    0x03
#define SET_SCENE_MODE_CU135             0x02
#define GET_SCENE_MODE_CU135             0x01
#define SET_DENOISE_CONTROL_CU135        0x06
#define GET_DENOISE_CONTROL_CU135        0x05
#define GET_Q_FACTOR_CU135               0x0D
#define SET_Q_FACTOR_CU135				 0x0E
#define SET_HDR_MODE_CU135				 0x0C
#define GET_HDR_MODE_CU135				 0x0B
#define SET_STREAM_MODE_CU135            0x11
#define GET_STREAM_MODE_CU135            0x0F
#define SET_BURST_LENGTH_CU135           0x0A
#define GET_BURST_LENGTH_CU135           0x09
#define SET_ORIENTATION_CU135            0x14
#define GET_ORIENTATION_CU135            0x13
#define SET_EXPOSURE_COMPENSATION_CU135  0x1A
#define GET_EXPOSURE_COMPENSATION_CU135  0x19
#define SET_EXP_ROI_MODE_CU135                     0x08
#define GET_EXP_ROI_MODE_CU135                     0x07
#define SET_FRAME_RATE_CU135                       0x1C
#define GET_FRAME_RATE_CU135                       0x1B
#define SET_FACE_DETECT_CU135                      0x16
#define GET_FACE_DETECT_CU135                      0x15
#define ENABLE_FACE_RECT_CU135                     0x01
#define DISABLE_FACE_RECT_CU135                    0x00
#define ENABLE_EMBED_DATA_CU135                    0x01
#define DISABLE_EMBED_DATA_CU135                   0x00
#define ENABLE_OVERLAY_RECT_CU135                  0x01
#define DISABLE_OVERLAY_RECT_CU135                 0x00
#define SET_SMILE_DETECTION_CU135                  0x18
#define GET_SMILE_DETECTION_CU135                  0x17
#define ENABLE_SMILE_DETECT_CU135                  0x01
#define DISABLE_SMILE_DETECT_CU135                 0x00
#define ENABLE_FACE_RECTANGLE_CU135               0x01
#define DISABLE_FACE_RECTANGLE_CU135              0x00
#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_CU135       0x10
#define SET_TO_DEFAULT_CU135                       0x12


/* SEE3CAM_81 */
#define CAMERA_CONTROL_81					0x7C
#define SETSPECIALEFFECT_81					0x09
#define GETSPECIALEFFECT_81					0x08
#define GETFLIPMIRRORSTATUS_81				0x0A
#define SETFLIPMIRRORSTATUS_81				0x0B
#define GET_AF_ROI_MODE_81                  0x13
#define SET_AF_ROI_MODE_81                  0x14
#define SET_AF_ROI_MODE_COORDINATES_81      0x16
#define GETSTROBE_STATUS					0x06
#define SETSTROBE_STATUS					0x07
#define GETFOCUS_STATUS                     0x05

#define GET_FAIL_CAM81                      0x01
#define GET_SUCCESS_CAM81                   0x00

/* SEE3CAM_30 */
#define CAMERA_CONTROL_30               0x80
#define GET_SCENEMODE_30                0x01
#define SET_SCENEMODE_30                0x02
#define SET_EXPOSURE_COMPENSATION_30    0x1A
#define GET_EXPOSURE_COMPENSATION_30    0x19
#define SET_FACE_DETECT_RECT_30         0x16
#define GET_FACE_DETECT_RECT_30         0x15
#define GET_AF_MODE_30					0x07
#define SET_AF_MODE_30					0x08
#define GET_Q_FACTOR_30                 0x0F
#define SET_Q_FACTOR_30                 0x10
#define SET_ORIENTATION_30              0x12
#define GET_ORIENTATION_30              0x11
#define SET_ORIENTATION_HORZFLIP_30     0x01
#define SET_ORIENTATION_VERTFLIP_30     0x02
#define SET_ORIENTATION_BOTHFLIP_ENABLE_30      0x03
#define SET_ORIENTATION_BOTHFLIP_DISABLE_30     0x00
#define GET_ORIENTATION_30              0x11
#define GET_AF_ROI_MODE_30				0x09
#define SET_AF_ROI_MODE_30				0x0A
#define SET_EXP_ROI_MODE_30             0x0C
#define GET_EXP_ROI_MODE_30             0x0B
#define GET_AF_RECT_MODE_30             0x13
#define SET_AF_RECT_MODE_30             0x14
#define ENABLE_AF_RECTANGLE_30          0x01
#define DISABLE_AF_RECTANGLE_30         0x00
#define GET_BURST_LENGTH_30             0x0D
#define SET_BURST_LENGTH_30             0x0E
#define SET_TO_DEFAULT_30               0xFF
#define ENABLE_FACE_RECT_30                0x01
#define DISABLE_FACE_RECT_30               0x00
#define ENABLE_EMBED_DATA_30               0x01
#define DISABLE_EMBED_DATA_30              0x00
#define ENABLE_OVERLAY_RECT_30             0x01
#define DISABLE_OVERLAY_RECT_30            0x00
#define SET_OVERLAY_RECT_MODE_30           0x1D
#define SET_SMILE_DETECT_30                0x18
#define GET_SMILE_DETECT_30                0x17
#define SET_FRAME_RATE_30                  0x1C
#define GET_FRAME_RATE_30                  0x1B

/* SEE3CAM_130 */
#define CAMERA_CONTROL_130               0x7B
#define GET_SCENEMODE_130				0x01
#define SET_SCENEMODE_130				0x02
#define GET_SPECIALEFFECT_130			0x03
#define SET_SPECIALEFFECT_130			0x04
#define GET_DENOISE_130					0x05
#define SET_DENOISE_130					0x06
#define GET_AF_MODE_130					0x07
#define SET_AF_MODE_130					0x08
#define GET_AF_ROI_MODE_130				0x09
#define SET_AF_ROI_MODE_130				0x0A
#define GET_EXP_ROI_MODE_130			0x0B
#define SET_EXP_ROI_MODE_130			0x0C
#define GET_BURST_LENGTH_130			0x0D
#define SET_BURST_LENGTH_130			0x0E
#define GET_HDR_MODE_130				0x0F
#define SET_HDR_MODE_130				0x10
#define GET_Q_FACTOR_130				0x11
#define SET_Q_FACTOR_130				0x12
#define ENABLE_DISABLE_MODE_AF_RECTANGLE_130     0x13
#define GET_AF_RECT_MODE                0x16
#define ENABLE_AF_RECTANGLE_130     0x01
#define DISABLE_AF_RECTANGLE_130     0x00
#define SET_TO_DEFAULT                0x17
#define MANUAL_ROI_AF_130             0x02
#define MANUAL_ROI_EXP_130			  0x02
#define SET_FLIP_MODE_130                 0x19
#define GET_FLIP_MODE_130                 0x18
#define FLIP_ENABLE                   0x01
#define FLIP_DISABLE                  0x00
#define SET_HORZ_FLIP_MODE_130             0x00
#define SET_VERT_FLIP_MODE_130             0x01

#define SET_STREAM_MODE_130               0x15
#define GET_STREAM_MODE_130               0x14

#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_130       0x26
#define ENABLE_FACE_RECTANGLE_130       0x01
#define DISABLE_FACE_RECTANGLE_130      0x00
#define SET_FACE_DETECT_RECT            0x1F
#define GET_FACE_DETECT_RECT            0x1E
#define ENABLE_FACE_RECT                0x01
#define DISABLE_FACE_RECT               0x00
#define ENABLE_EMBED_DATA               0x01
#define DISABLE_EMBED_DATA              0x00
#define ENABLE_OVERLAY_RECT             0x01
#define DISABLE_OVERLAY_RECT            0x00
#define FACE_RECT_SUCCESS               0x01
#define FACE_RECT_FAIL                  0x00

#define SET_SMILE_DETECTION             0x21
#define GET_SMILE_DETECTION             0x20
#define ENABLE_SMILE_DETECT             0x01
#define DISABLE_SMILE_DETECT            0x00
#define SMILE_DETECT_SUCCESS            0x01
#define SMILE_DETECT_FAIL               0x00

#define SET_EXPOSURE_COMPENSATION       0x23
#define GET_EXPOSURE_COMPENSATION       0x22
#define EXPOSURE_COMP_CMD_SUCCESS       0x01
#define EXPOSURE_COMP_CMD_FAIL          0x00

#define SET_FRAME_RATE_130              0x25
#define GET_FRAME_RATE_130              0x24
#define FRAMERATE_CMD_SUCCESS           0x01
#define FRAMERATE_CMD_FAIL              0x00

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

/* SEE3CAM_CU30 */
#define CAMERA_CONTROL_CU30     0x75
#define GET_SPECIAL_EFFECT 0x03
#define SET_SPECIAL_EFFECT 0x04
#define SET_ORIENTATION_CU30    0x0E
#define GET_ORIENTATION_CU30    0x0D
#define SET_ORIENTATION_BOTHFLIP_ENABLE_CU30    0x03
#define SET_ORIENTATION_HORZFLIP_CU30           0x01
#define SET_ORIENTATION_VERTFLIP_CU30           0x02
#define SET_ORIENTATION_BOTHFLIP_DISABLE_CU30   0x00

#define SPECIAL_EFFECT_NORMAL       0x01
#define SPECIAL_EFFECT_BW           0x04
#define SPECIAL_EFFECT_GRAYSCALE    0x07
#define SPECIAL_EFFECT_NEGATIVE     0x08
#define SPECIAL_EFFECT_SKETCH       0x10

#define SET_BURST_LENGTH_CU30       0x0A
#define GET_BURST_LENGTH_CU30       0x09

#define GET_SCENEMODE_CU30          0x01
#define SET_SCENEMODE_CU30          0x02

#define GET_Q_FACTOR_CU30           0x0B
#define SET_Q_FACTOR_CU30           0x0C

#define SET_FRAME_RATE_CU30         0x16
#define GET_FRAME_RATE_CU30         0x15

#define SET_EXPOSURE_COMPENSATION_CU30  0x14
#define GET_EXPOSURE_COMPENSATION_CU30  0x13

#define CU30_SUCCESS    0x01
#define CU30_FAIL       0x00

#define SET_DENOISE_CONTROL  0x06
#define GET_DENOISE_CONTROL  0x05

#define SET_EXP_ROI_MODE_CU30       0x08
#define GET_EXP_ROI_MODE_CU30       0x07

#define SET_FACE_DETECT_RECT_CU30            0x10
#define GET_FACE_DETECT_RECT_CU30            0x0F
#define ENABLE_FACE_RECT_CU30                0x01
#define DISABLE_FACE_RECT_CU30               0x00
#define ENABLE_EMBED_DATA_CU30               0x01
#define DISABLE_EMBED_DATA_CU30              0x00
#define ENABLE_OVERLAY_RECT_CU30             0x01
#define DISABLE_OVERLAY_RECT_CU30            0x00

#define SET_SMILE_DETECTION_CU30             0x12
#define GET_SMILE_DETECTION_CU30             0x11
#define ENABLE_SMILE_DETECT_CU30             0x01
#define DISABLE_SMILE_DETECT_CU30            0x00
#define SET_TO_DEFAULT_CU30                  0xFF

#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_CU30 0x17
#define ENABLE_FACE_RECTANGLE_CU30              0x01
#define DISABLE_FACE_RECTANGLE_CU30             0x00

/* SEE3CAM_CU40 */
#define CAMERA_CONTROL_CU40         0x77
#define SETFLASH_STATUS_CU40        0x02
#define GETFLASH_STATUS_CU40        0x01
#define GET_FAIL_CU40		0x01
#define GET_SUCCESS_CU40	0x00

/* See3Cam_81 */
#define SET_TO_DEFAULT_CAM81        0x0C

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

// See3cam_cu20 camera
#define CAMERA_CONTROL_CU20         0x86
#define GET_SENSOR_MODE_CU20        0x01
#define SET_SENSOR_MODE_CU20        0x02
#define GET_CAMERA_MODE_CU20        0x03
#define SET_CAMERA_MODE_CU20        0x04
#define SET_SPECIAL_MODE_CU20       0x08
#define GET_SPECIAL_MODE_CU20       0x07
#define GET_ORIENTATION_CU20        0x09
#define SET_ORIENTATION_CU20        0x0A
#define SET_STROBEMODE_CU20         0x0C
#define GET_STROBEMODE_CU20         0x0B
#define SET_AE_ROI_MODE_CU20        0x06
#define GET_AE_ROI_MODE_CU20        0x05
#define SET_COLORKILLVAL_CU20       0x0E
#define GET_COLORKILLVAL_CU20       0x0D
#define SET_BURSTLENGTH_CU20        0x10
#define GET_BURSTLENGTH_CU20        0x0F
#define SET_ANTIFLICKER_CU20        0x12
#define GET_ANTIFLICKER_CU20        0x11
#define SET_DENOISE_MODE_CU20       0x16
#define GET_DENOISE_MODE_CU20       0x15
#define SET_LSCMODE_CU20            0x14
#define GET_LSCMODE_CU20            0x13
#define SET_TO_DEFAULT_CU20         0xFF

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
    bool closeAscellaDevice();

    static libusb_device_handle* handle;  /* handle for USB device */
    int kernelDriverDetached;

    static QMap<QString, QString> cameraMap;
    static QMap<QString, QString> serialNumberMap;
    static int hid_fd;
    static QString hidNode;
    /**
     * @brief selectedDeviceEnum - This contains selected camera device Enum value
     */
    static CommonEnums::ECameraNames  selectedDeviceEnum;

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
    /**
     * @brief initCameraEnumMap - initiate camera enum map with vidpid,enum pair values
     */
    void initCameraEnumMap();

    static QString openNode;
    /**
     * @brief pidVidMap - This map contains the deviceName and Vid,Pid value
     */
    QMap<QString, QString> pidVidMap;
    /**
     * @brief cameraEnumMap - This map contains Vid,pid value and corresponding enum value
     */
    QMap<QString, CommonEnums::ECameraNames> cameraEnumMap;

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

    // Added by Sankari: To notify user about hid access
    // 07 Dec 2017
    void hidWarningReceived(QString _title,QString _text);

    void serialNumber(QString serialNumber);
    /**
     * @brief currentlySelectedCameraEnum - This signal is used to emit selected camera enum value to camera property.cpp
     */
    void currentlySelectedCameraEnum(CommonEnums::ECameraNames);

public slots:

    /**
     * @brief sendHidCmd - Sending hid command and get reply back
     * @param outBuf - Buffer that fills to send into camera
     * @param inBuf  - Buffer to get reply back
     * @param len    - Buffer length
     */
    bool sendHidCmd(unsigned char *outBuf, unsigned char *inBuf, int len);

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
    bool exitExtensionUnitAscella();
    /**
     * @brief currentlySelectedDevice - This slot contains currently selected device name
     * @param deviceName - Name of the camera device
     */
    void currentlySelectedDevice(QString deviceName);
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
     * @return
     * true - read successfully the torch state level, false- fails to read the torch state level
     */
    bool getTorchState(quint8 *torchState);

    /**
     * @brief   Set the Torch state of the Device.
     * @param torchState Disable/Enable Torch
     * @return
     * true - Success (Torch state is set as per the torchState).
     * @return
     * false - Failure (Torch state is not set as per the torchState).
     */
    bool setTorchState(flashTorchState torchState);

    /**
     * @brief Get the flash state
     * @param Level
     * This value will hold the state level of the flash (either 0-flash not enabled or 1-flash is enabled)     
     * @return
     * true - read successfully the flash state level, false- fails to read the flash state level
     */
    bool getFlashState(quint8 *flashState);

    /**
     * @brief   Set the Flash state of the Device.
     * @param flashState Disable/Enable Flash
     * @return
     * true - Success (Flash state is set as per the flashState).
     * @return
     * false - Failure (Torch state is not set as per the flashState).
     */
    bool setFlashState(flashTorchState flashState);

    flashTorchState flashCheckBoxState;
    flashTorchState torchCheckBoxState;

signals:
    void deviceStatus(QString title, QString message);
public slots:

    void setFlashControlState(const int flashState);
    void setTorchControlState(const int torchState);
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
    IN3 = 33,
    CAM81_IN3 = 0x01,
    CAM81_IN6 = 0x02,
    CAM81_OUT8 = 0x01,
    CAM81_OUT9 = 0x02,
    CAM81_OUT8_READVAL = 25,
    CAM81_OUT9_READVAL = 24,
    CAM81_IN3_READVAL = 21,
    CAM81_IN6_READVAL = 23,
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
    bool getGpioLevel(camGpioPin gpioPinNumber);

    /**
     * @brief   Set the GPIO status(High=1/Low=0) for the particular Gpio pin.
     * @param gpioPin  pin(OUT1) for which user needs to  set the status.
     * @param gpioValue  Set Status(High/Low) for the Gpio pin.
     * @return
     * true - Successful to change the GPIO level.
     * @return
     * false - Failed to change the GPIO level.
     */
     bool setGpioLevel(camGpioPin gpioPin,camGpioValue gpioValue);
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
