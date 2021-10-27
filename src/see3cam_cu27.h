#ifndef SEE3CAM_CU27_H
#define SEE3CAM_CU27_H

#include <QObject>
#include "uvccamera.h"

#define CAMERA_CONTROL_CU27          0xAF

#define SET_FAIL                    0x00
#define SET_SUCCESS                 0x01

#define GET_FAIL                    0x00
#define GET_SUCCESS                 0x01

#define GET_AWB_MODE_CU27            0x03
#define SET_AWB_MODE_CU27            0x04

#define GET_AWB_LOCKSTATUS_CU27      0x05
#define SET_AWB_LOCKSTATUS_CU27      0x06

#define GET_AE_METERING_MODE_CU27    0x07
#define SET_AE_METERING_MODE_CU27    0x08

#define GET_AE_LOCK_STATUS_CU27      0x09
#define SET_AE_LOCK_STATUS_CU27      0x0A

#define GET_FLICKER_DETECTION_CU27   0x0B
#define SET_FLICKER_DETECTION_CU27   0x0C

#define GET_Q_FACTOR_CU27            0x0D
#define SET_Q_FACTOR_CU27            0x0E

#define GET_BURST_LENGTH_CU27        0x1A
#define SET_BURST_LENGTH_CU27        0x1B

#define GET_DENOISE_MODE_CU27        0x1C
#define SET_DENOISE_MODE_CU27        0x1D

#define MAX_WINDOW_SIZE             8

#define READ_FIRMWARE_VERSION_CU27   0x19

#define SET_TO_DEFAULT_CU27          0xFF

class See3CAM_CU27 : public QObject
{
    Q_OBJECT
private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
//    explicit See3CAM_CU27(QObject *parent = 0);
    See3CAM_CU27();
    ~See3CAM_CU27();

    enum awbPresetMode {
        AWB_PRESET_CLOUDY = 0x01,
        AWB_PRESET_DAYLIGHT = 0x02,
        AWB_PRESET_FLASH = 0x03,
        AWB_PRESET_COOLWHITE_FLOURECENT = 0x04,
        AWB_PRESET_TUNGSTEN = 0x05,
        AWB_PRESET_CANDLELIGHT = 0x06,
        AWB_PRESET_HORIZON = 0x07,
        AWB_PRESET_CUSTOM = 0x08,
        AWB_PRESET_AUTO = 0x09
    };
    Q_ENUMS(awbPresetMode)

    enum awbLockStatus {
        AWB_LOCK_OFF = 0x00,
        AWB_LOCK_ON = 0x01
    };
    Q_ENUMS(awbLockStatus)

    enum aeMeteringMode{
        AE_OFF = 0x00,
        AE_CENTER_WEIGHTED_AVG = 0x01,
        AE_ALL_BLOCK_INTEGRAL = 0x02,
        AE_SMALL_AREA = 0x05,
        AE_LARGE_AREA = 0x06
    };
    Q_ENUMS(aeMeteringMode)

    enum aeLockStatus {
        AE_LOCK_OFF = 0x00,
        AE_LOCK_ON = 0x01
    };
    Q_ENUMS(aeLockStatus)

    enum camFlickerMode{
        FLICKER_AUTO_DETECTION = 0x00,
        FLICKER_50HZ = 0x01,
        FLICKER_60HZ = 0x02,
        FLICKER_OFF = 0x04
    };
    Q_ENUMS(camFlickerMode)

    enum denoiseModes {
        DenoiseDisable = 0x00,
        DenoiseEnable = 0x01
    };
    Q_ENUMS(denoiseModes)

    QString _title;
    QString _text;

signals:
    void titleTextChanged(QString _title,QString _text);
    void qFactorValue(uint qFactor);
    void flickerModeChanged(uint flickerMode);
    void awbPresetModeChanged(uint awbMode);
    void awbLockStatusChanged(uint lockStatus);
    void aeLockStatusChanged(uint lockStatus);
    void aeMeterModeChanged(uint meterMode);
    void burstLengthValue(uint burstLength);
    void denoiseModeChanged(uint denoiseMode);


public slots:

    bool getQFactor();
    bool setQFactor(uint qFactor);

    bool setToDefault();
    bool getFirmwareVersion();

    bool getFlickerMode();
    bool setFlickerMode(camFlickerMode FlickerMode);

    bool getAWBlockstatus();
    bool setAWBlockstatus(awbLockStatus lockStatus);

    bool getAWBpresetMode();
    bool setAWBpresetMode(awbPresetMode awbMode);

    bool getAElockstatus();
    bool setAElockstatus(aeLockStatus lockStatus);

    bool getAEMeterMode();
    bool setAEMeterMode(aeMeteringMode meterMode);

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setDenoiseCtrlMode(denoiseModes deNoiseMode);
    bool getDenoiseCtrlMode();
};

#endif // SEE3CAM_CU27_H
