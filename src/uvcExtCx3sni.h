#ifndef UVCEXTCX3SNI_H
#define UVCEXTCX3SNI_H

#include <QObject>
#include <QDebug>
#include "videostreaming.h"
#include <errno.h>

#define UVC_RC_UNDEFINED                                0x00
#define UVC_SET_CUR                                     0x01

#define UVC_GET_LEN                                     0x85
#define UVC_GET_INFO                                    0x86

#define CX3_UVC_XU_VC_LED_FLASH_CONTROL                 0x0F
#define CX3_UVC_XU_VC_AWB_PRESET_CONTROL                0x0D
#define CX3_UVC_XU_VC_AWB_LOCK_CONTROL                  0x09
#define CX3_UVC_XU_VC_AE_METERING_MODE_CONTROL          0x08
#define CX3_UVC_XU_VC_AE_LOCK_CONTROL                   0x05
#define CX3_UVC_XU_VC_FLICKER_CONTROL                   0x07
#define CX3_UVC_XU_VC_JPEG_Q_CONTROL                    0x06
#define CX3_UVC_XU_VC_CAF_SCAN_RANGE_CONTROL            0x0A
#define CX3_UVC_XU_VC_FOCUS_LENS_POSITION_CONTROL       0x0B
#define CX3_UVC_XU_VC_AF_SINGLE_CONTROL                 0x0C
#define CX3_UVC_XU_ROI_CONTROL                          0x03
#define CX3_UVC_XU_VC_CAF_LOCK_CONTROL                  0x0E
#define CX3_UVC_XU_FW_VERSION                           0x10


#define ROI_DATA_SIZE                                   20
#define MAX_WINDOW_SIZE                                 8
#define FW_DATA_SIZE                                    41
#define GET_FOCUS_ROI_MODE_INDEX                        8
#define EXTENSION_UNIT_ID                               3



class UVCExtCx3SNI: public QObject, public v4l2
{
    Q_OBJECT

private:
    /**
     * @brief UVCExtCx3SNI::setCurrentValueCmd - set current value from camera
     * @param controlId - control id
     * @param setVal[] - value(s) to set
     * @return true/false
     */
    // set the current value into the camera.
    bool setCurrentValueCmd(__u8 controlId, __u8 setVal[]);

    bool setCurrentValueCmd1(__u8 controlId, __u8 setVal);

    /**
     * @brief UVCExtCx3SNI::getValueCmd - get Current value command multiple values
     * @param controlId - control id
     * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN/ UVC_GET_MAX/ UVC_GET_RES
     * @param numberOfValues - No of values to receive
     * @param outputValues - pointer to hold outputValues
     * @return true/false
     */
    bool getValueCmd(__u8 controlId, __u8 queryType, uint numberOfValues, __u8 *outputValues);

public:
    Videostreaming *gvobj;

    QString _title;
    QString _text;

    UVCExtCx3SNI();
   ~UVCExtCx3SNI();

    enum getValueQueryType{
        UVC_GET_CUR = 0x81,
        UVC_GET_DEF = 0x87,
        UVC_GET_MIN = 0x82,
        UVC_GET_MAX = 0x83,
        UVC_GET_RES = 0x84
    };
    Q_ENUMS(getValueQueryType)

    enum autoFocusROIMode{
        ROI_CENTERED = 0x00,
        ROI_CUSTOM = 0x08
    };
    Q_ENUMS(autoFocusROIMode)

    enum  awbPresetMode{
        AWB_PRESET_CLOUDY = 0x01,
        AWB_PRESET_DAYLIGHT = 0x02,
        AWB_PRESET_FLASH = 0x03,
        AWB_PRESET_COOLWHITE_FLOURECENT = 0x04,
        AWB_PRESET_TUNGSTEN = 0x05,
        AWB_PRESET_CANDLELIGHT = 0x06,
        AWB_PRESET_HORIZON = 0x07,
        AWB_PRESET_AUTO = 0x08
    };
    Q_ENUMS(awbPresetMode)

    enum  cafScanRangeMode{
        CAF_MACRO_RANGE_MODE = 0x00,
        CAF_NORMAL_RANGE_MODE = 0x01,
        CAF_FULL_RANGE_MODE = 0x02,
        CAF_INFINITY_RANGE_MODE = 0x03,
        CAF_HYPERFOCAL_RANGE_MODE = 0x04
    };
    Q_ENUMS(cafScanRangeMode)

    enum aeMeteringMode{
        AE_OFF = 0x00,
        AE_CENTER_WEIGHTED_AVG_1 = 0x01,
        AE_ALL_BLOCK_INTEGRAL = 0x02,
        AE_SPOT1_SMALL_AREA = 0x05,
        AE_SPOT2_LARGE_AREA = 0x06
    };
    Q_ENUMS(aeMeteringMode)

    enum flickerMode{
        FLICKER_AUTO_DETECTION = 0x00,
        FLICKER_50HZ = 0x01,
        FLICKER_60HZ = 0x02,
        FLICKER_OFF = 0x03
    };
    Q_ENUMS(flickerMode)

    enum manualLensPositionMode{
        LENS_POS_INF = 0x01,
        LENS_POS_NORMAL = 0x02,
        LENS_BARCODE_POS = 0x03,
        LENS_MACRO_POS = 0x04,
        LENS_INITIAL_DAC_POS = 0x05
    };
    Q_ENUMS(manualLensPositionMode)

    enum autoFocusModes{
         AF_CONTINUOUS = 0x06,
         AF_ONESHOT = 0x01,
         AF_OFF = 0x00
    };
    Q_ENUMS(autoFocusModes)


signals:    
    bool ledBrightnessValueReceived(uint queryType, unsigned int brightnessVal);
    bool awbPresetsModeReceived(uint queryType, unsigned int awbPresetMode);
    bool awbLockValueReceived(uint queryType, unsigned int awbLockValue);
    bool autoExpModeReceived(uint queryType, unsigned int autoExpMode);
    bool autoExpLockValueReceived(uint queryType, unsigned int autoExpoureLockValue);
    bool flickerModeReceived(uint queryType, unsigned int flickerMode);
    bool jpegQValueReceived(uint queryType, unsigned int jpegQVal);
    bool cafScanRangeModeReceived(uint queryType, unsigned int scanRangeMode);
    bool manualLensPositionModeReceived(uint queryType, unsigned int lensPosMode);
    bool autoFocusModeReceived(uint queryType, unsigned int autoFocusModeValue);
    bool afLockValueReceived(uint queryType, unsigned int afLockValue);
    bool autoFocusRoiModeReceived(uint queryType, unsigned int afRoiMode);
    bool updateFrameToSkipfromCam(uint frameToSkip);
    void titleTextChanged(QString _title,QString _text);

public slots:

    bool setLedBrightness(uint brightnessValue);
    bool getLedBrightness(uint queryType);

    bool setAWBPresetControl(uint awbPresetMode);
    bool getAWBPresetMode(uint queryTypej);

    bool setAWBLock(bool locked);
    bool getAWBLock(uint queryType);

    bool setAEMeterMode(uint aeMeterMode);
    bool getAEMeterMode(uint queryType);

    bool setAELock(bool locked);
    bool getAELock(uint queryType);

    bool setFlickerMode(uint flickerMode);
    bool getFlickerMode(uint queryType);

    bool setJpegQfactor(uint jpegQValue);
    bool getJpegQfactor(uint queryType);

    bool setCAFScanRangeMode(uint cafScanRangeMode);
    bool getCAFScanRangeMode(uint queryType);

    bool setManualLensPositionMode(uint lensPosMode);
    bool getManualLensPositionMode(uint queryType);

    bool setAutoFocusMode(uint autoFocusMode);
    bool getAutoFocusMode(uint queryType);

    bool setAFLock(bool locked);
    bool getAFLock(uint queryType);

    bool setCenteredAutoFocusMode();
    bool setCustomAreaAutoFocus(int vidResolnWidth, int vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoFocusROIMode(uint queryType);

    bool getFirmwareVersion();

    void initUVCExtensionUnit(Videostreaming  *vobj);
    void deInitUVCExtensionUnit();
};

#endif // UVCEXTCX3SNI_H
