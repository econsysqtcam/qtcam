#ifndef SEE3CAM_CU210_H
#define SEE3CAM_CU210_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID1_SEE3CAM_CU210            0xC0
#define CAMERA_CONTROL_ID2_SEE3CAM_CU210            0x07

#define GET_AWB_MODE_SEE3CAM_CU210                  0x03
#define SET_AWB_MODE_SEE3CAM_CU210                  0x04

#define GET_AWB_LOCK_STATUS_SEE3CAM_CU210           0x05
#define SET_AWB_LOCK_STATUS_SEE3CAM_CU210           0x06

#define GET_EXPOSURE_METERING_MODE_SEE3CAM_CU210    0x07
#define SET_EXPOSURE_METERING_MODE_SEE3CAM_CU210    0x08

#define GET_AE_LOCK_STATUS_SEE3CAM_CU210            0x09
#define SET_AE_LOCK_STATUS_SEE3CAM_CU210            0x0A

#define GET_FLICKER_MODE_SEE3CAM_CU210              0x0B
#define SET_FLICKER_MODE_SEE3CAM_CU210              0x0C

#define GET_Q_FACTOR_VALUE_SEE3CAM_CU210            0x0D
#define SET_Q_FACTOR_VALUE_SEE3CAM_CU210            0x0E

#define ISP_FIRMWARE_VERSION_SEE3CAM_CU210          0x19

#define GET_BURST_LENGTH_SEE3CAM_CU210              0x1A
#define SET_BURST_LENGTH_SEE3CAM_CU210              0x1B

#define GET_DENOISE_SEE3CAM_CU210                   0x1C
#define SET_DENOISE_SEE3CAM_CU210                   0x1D

#define SET_DEFAULT_SEE3CAM_CU210                   0xFF



class See3CAM_CU210 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();
public:
    See3CAM_CU210();

    QString _title;
    QString _text;
    QString message;

    enum AWSMODE{
        CLOUDY                 = 0x01,
        DAYLIGHT               = 0x02,
        FLASH                  = 0x03,
        COOL_WHITE_FLUORESCENT = 0x04,
        TUNGSTEN               = 0x05,
        CANDLE_LIGHT           = 0x06,
        HORIZON                = 0x07,
        CUSTOM                 = 0x08,
        AUTO                   = 0x09
    };
    Q_ENUMS(AWSMODE)
    
    
    enum EXPOSURE_METERING_MODE{
        AUTO_EXPOSURE_OFF = 0x00,
        AVERAGE           = 0x01,
        INTEGRAL          = 0x02,
        SMALL_AREA        = 0x05,
        LARGE_AREA        = 0x06
    };
    Q_ENUMS(EXPOSURE_METERING_MODE)

    enum AWB_LOCK_STATUS{
        AWB_OFF = 0x00,
        AWB_ON  = 0x01
    };
    Q_ENUMS(AWB_LOCK_STATUS)

    enum AE_LOCK_STATUS{
        AE_OFF = 0x00,
        AE_ON  = 0x01
    };
    Q_ENUMS(AE_LOCK_STATUS)

    enum FLICKER{
        FLICKER_AUTO     = 0x00,
        FLICKER_50HZ     = 0x01,
        FLICKER_60HZ     = 0x02,
        FLICKER_DISABLE  = 0x04
    };
    Q_ENUMS(FLICKER)

    enum DENOISE{
        DISABLE   = 0x00,
        ENABLE    = 0x01
    };
    Q_ENUMS(DENOISE)

signals:
    void awbModeRecieved(uint awbMode);
    void awbLockStatusReceived(uint awbLockStatus);
    void meteringModeReceived(uint meteringMode);
    void aeLockStatusReceived(uint aeLockStatus);
    void burstLengthReceived(uint burstLength);
    void flickerModeReceived(uint flickerMode);
    void qFactorValueReceived(uint Qfactor);
    void denoiseModeReceived(uint denoise);
    void titleTextChanged(QString _title,QString _text);

public slots:
    bool setAwbMode(AWSMODE mode);
    bool getAwbMode();

    bool setAwbLockStatus(AWB_LOCK_STATUS lockStatus);
    bool getAwbLockStatus();

    bool setExposureMeteringMode(EXPOSURE_METERING_MODE  meteringMode);
    bool getExposureMeteringMode();

    bool setAeLockStatus(AE_LOCK_STATUS  lockStatus);
    bool getAeLockStatus();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setFlickerMode(FLICKER flickerMode);
    bool getFlickerMode();

    bool setQFactorValue(uint QFactor);
    bool getQFactorValue();

    bool setDenoiseMode(DENOISE denoise);
    bool getDenoiseMode();

    bool readISPFirmwareVersion();

    bool setToDefaultValues();
};

#endif // SEE3CAM_CU210_H
