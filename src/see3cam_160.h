#ifndef SEE3CAM_160_H
#define SEE3CAM_160_H

#include <QObject>
#include "uvccamera.h"

#define CAMERA_CONTROL_160          0xAD

#define SET_FAIL                    0x00
#define SET_SUCCESS                 0x01

#define GET_FAIL                    0x00
#define GET_SUCCESS                 0x01

#define GET_AF_MODE_160             0x01
#define SET_AF_MODE_160             0x02

#define GET_AWB_MODE_160            0x03
#define SET_AWB_MODE_160            0x04

#define GET_AWB_LOCKSTATUS_160      0x05
#define SET_AWB_LOCKSTATUS_160      0x06

#define GET_AE_METERING_MODE_160    0x07
#define SET_AE_METERING_MODE_160    0x08

#define GET_AE_LOCK_STATUS_160      0x09
#define SET_AE_LOCK_STATUS_160      0x0A

#define GET_FLICKER_DETECTION_160   0x0B
#define SET_FLICKER_DETECTION_160   0x0C

#define GET_Q_FACTOR_160            0x0D
#define SET_Q_FACTOR_160            0x0E

#define GET_CAF_RANGE_160           0x0F
#define SET_CAF_RANGE_160           0x10

#define GET_AF_LOCKSTATUS_160       0x11
#define SET_AF_LOCKSTATUS_160       0x12

#define GET_LENS_POSITION_160       0x13
#define SET_LENS_POSITION_160       0x14

#define GET_AF_STATUS_160           0x15
#define SET_AF_STATUS_160           0x16

#define GET_AF_ROI_MODE_160         0X17
#define SET_AF_ROI_MODE_160         0X18

#define CENTER_WEIGHTED_MODE_160    0x00
#define CUSTOM_AREA_MODE_160        0x01

#define GET_BURST_LENGTH_160        0x1A
#define SET_BURST_LENGTH_160        0x1B

#define MAX_WINDOW_SIZE             8

#define READ_FIRMWARE_VERSION_160   0x19

#define SET_TO_DEFAULT_160          0xFF

class See3CAM_160 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit See3CAM_160(QObject *parent = 0);

    enum autoFocusModes{
         AF_CONTINUOUS = 0x06,
         AF_ONESHOT = 0x01
    };
    Q_ENUMS(autoFocusModes)

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

    enum  cafScanRangeMode{
        CAF_MACRO_RANGE_MODE = 0x00,
        CAF_NORMAL_RANGE_MODE = 0x01,
        CAF_FULL_RANGE_MODE = 0x02,
        CAF_INFINITY_RANGE_MODE = 0x03,
        CAF_HYPERFOCAL_RANGE_MODE = 0x04
    };
    Q_ENUMS(cafScanRangeMode)

    enum afLockStatus {
        AF_LOCK_OFF = 0x00,
        AF_LOCK_ON = 0x01
    };
    Q_ENUMS(afLockStatus)

    enum manualLensPositionMode{
        LENS_POS_INF = 0x01,
        LENS_POS_NORMAL = 0x02,
        LENS_BARCODE_POS = 0x03,
        LENS_MACRO_POS = 0x04,
        LENS_INITIAL_DAC_POS = 0x05,
        LENS_CUSTOM_POS = 0x06
    };
    Q_ENUMS(manualLensPositionMode)

    enum afStatus {
        AF_ON = 0x00,
        AF_OFF = 0x06
    };
    Q_ENUMS(afStatus)

    enum autoFocusROIMode{
        ROI_CENTERED = 0x00,
        ROI_CUSTOM = 0x01
    };
    Q_ENUMS(autoFocusROIMode)

    QString _title;
    QString _text;

signals:
    void afModeChanged(uint afMode);
    void titleTextChanged(QString _title,QString _text);
    void qFactorValue(uint qFactor);
    void flickerModeChanged(uint flickerMode);
    void awbPresetModeChanged(uint awbMode);
    void awbLockStatusChanged(uint lockStatus);
    void aeLockStatusChanged(uint lockStatus);
    void aeMeterModeChanged(uint meterMode);
    void afStatusChanged(uint afstatus);
    void manualLensPositionModeChanged(uint lensPosMode);
    void afLockStatusChanged(uint lockStatus);
    void autoFocusRoiModeChanged(uint afRoiMode);
    void cafScanRangeModeChanged(uint scanRangeMode);
    void burstLengthValue(uint burstLength);

public slots:
    bool getAutoFocusMode();
    bool setAutoFocusMode(autoFocusModes afMode);

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

    bool getAFstatus();
    bool setAFstatus(afStatus afstatus);

    bool getManualLensPositionMode();
    bool setManualLensPositionMode(manualLensPositionMode lensPosition);

    bool getAFlockstatus();
    bool setAFlockstatus(afLockStatus lockStatus);

    bool getCAFScanRangeMode();
    bool setCAFScanRangeMode(cafScanRangeMode cafMode);

    bool getAutoFocusROIMode();
    bool setCenteredAutoFocusMode();
    bool setCustomAreaAutoFocus(int vidResolnWidth, int vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

};

#endif // SEE3CAM_160_H

