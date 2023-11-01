#ifndef SEE3CAM_50CUG_M_H
#define SEE3CAM_50CUG_M_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_SEE3CAM_50CUG_M                    0xBF

#define GET_CAMERA_MODE_SEE3CAM_50CUG_M                   0x01
#define SET_CAMERA_MODE_SEE3CAM_50CUG_M                   0x02

#define GET_ORIENTATION_SEE3CAM_50CUG_M                   0x03
#define SET_ORIENTATION_SEE3CAM_50CUG_M                   0x04

#define GET_GAIN_MODE_SEE3CAM_50CUG_M                     0x05
#define SET_GAIN_MODE_SEE3CAM_50CUG_M                     0x06

#define GET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M        0x07
#define SET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M        0x08

#define GET_STROBE_MODE_SEE3CAM_50CUG_M                   0x09
#define SET_STROBE_MODE_SEE3CAM_50CUG_M                   0x0A

#define GET_IMAGE_BURST_SEE3CAM_50CUG_M                   0x0B
#define SET_IMAGE_BURST_SEE3CAM_50CUG_M                   0x0C

#define GET_EXPOSURE_SEE3CAM_50CUG_M                      0x0D
#define SET_EXPOSURE_SEE3CAM_50CUG_M                      0x0E

#define GET_TARGET_BRIGHTNESS_SEE3CAM_50CUG_M             0x0F
#define SET_TARGET_BRIGHTNESS_SEE3CAM_50CUG_M             0x10

#define GET_AUTO_GAIN_LIMIT_SEE3CAM_50CUG_M               0x11
#define SET_AUTO_GAIN_LIMIT_SEE3CAM_50CUG_M               0x12

#define GET_AUTOEXP_LOWER_LIMIT_SEE3CAM_50CUG_M           0x13
#define SET_AUTOEXP_LOWER_LIMIT_SEE3CAM_50CUG_M           0x14

#define GET_AUTOEXP_UPPER_LIMIT_SEE3CAM_50CUG_M           0x15
#define SET_AUTOEXP_UPPER_LIMIT_SEE3CAM_50CUG_M           0x16

#define GET_ANTI_FLICKER_DETECTION_SEE3CAM_50CUG_M        0x17
#define SET_ANTI_FLICKER_DETECTION_SEE3CAM_50CUG_M        0x18

#define READ_STATISTICS_SEE3CAM_50CUG_M                   0x19

#define READ_TEMPERATURE_SEE3CAM_50CUG_M                  0x1A

#define GET_MULTI_EXP_TRIGGER_SEE3CAM_50CUG_M             0x1B
#define SET_MULTI_EXP_TRIGGER_SEE3CAM_50CUG_M             0x1C

#define GET_MULTI_FRAMESET_SEE3CAM_50CUG_M                0x1D
#define SET_MULTI_FRAMESET_SEE3CAM_50CUG_M                0x1E

#define GET_ULTRA_SHORT_EXPOSURE_SEE3CAM_50CUG_M          0x1F
#define SET_ULTRA_SHORT_EXPOSURE_SEE3CAM_50CUG_M          0x20

#define GET_SOFTWARE_TRIGGER_SEE3CAM_50CUG_M              0x21
#define SET_SOFTWARE_TRIGGER_SEE3CAM_50CUG_M              0x22

#define SET_DEFAULT_SEE3CAM_50CUGM                        0xFF

#define SAVE_CONFIGURATION_SEE3CAM_50CUG_M                0x42
#define SAVE_SEE3CAM_50CUG_M                              0x01

#define SOFTWARE_TRIGGER_MIN     0
#define SOFTWARE_TRIGGER_MAX     255

#define MULTI_EXPOSURE_TRIGGER_MIN    2
#define MULTI_EXPOSURE_TRIGGER_MAX    4095

#define GAIN_MIN            0
#define GAIN_MAX            48

#define SECONDS_MIN         0
#define SECONDS_MAX         14

#define MILLISECONDS_MIN    0
#define MILLISECONDS_MAX    999

#define MICROSECONDS_MIN    0
#define MICROSECONDS_MAX    999

#define SET_FAIL	 0x00
#define SET_SUCCESS  0x01

#define GET_FAIL     0x00
#define GET_SUCCESS	 0x01


class SEE3CAM_50CUGM : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_50CUGM();

    QString _title;
    QString _text;

    uint triggerMin, triggerMax;
    uint ultraShortExpMin, ultraShortExpMax;

    uint lowerLimitSec, lowerLimitMilliSec, lowerLimitMicroSec;
    uint upperLimitSec, upperLimitMilliSec, upperLimitMicroSec;

    enum cameraModes {
        MASTER = 0x01,
        TRIGGER_EXP_CONTROL      = 0x02,
        TRIGGER_ACQ_CONTROL      = 0x03,
        TRIGGER_SOFTWARE_CONTROL = 0x04
    };
    Q_ENUMS(cameraModes)

    enum orientation{
        NORMAL     = 0x01,
        VERTICAL   = 0x02,
        HORIZONTAL = 0x03,
        ROTATE_180 = 0x04
    };
    Q_ENUMS(orientation)

    enum blackLevelAdjustment {
        AUTO  = 0x01,
        MANUAL = 0x02,
    };
    Q_ENUMS(blackLevelAdjustment)

    enum strobeMode {
        FLASH = 0x00,
        OFF   = 0x02
    };
    Q_ENUMS(strobeMode)

    //Gain mode
    enum GAIN_MODE {
        AUTO_GAIN_MODE   = 0x00,
        MANUAL_GAIN_MODE = 0x01
    };Q_ENUMS(GAIN_MODE)

    enum AUTO_GAIN_FEATURES {
        CONTINIOUS_GAIN          = 0x00,
        SINGLE_SHOT_GAIN         = 0x01,
        SINGLE_SHOT_GAIN_TRIGGER = 0x02
    };Q_ENUMS(AUTO_GAIN_FEATURES)

    //Exposure mode
    enum EXPOSURE_MODE {
        AUTO_EXPOSURE_MODE   = 0x00,
        MANUAL_EXPOSURE_MODE = 0x01
    };Q_ENUMS(EXPOSURE_MODE)

    enum AUTO_EXPOSURE_FEATURES {
        CONTINIOUS_EXPOSURE     = 0x00,
        SINGLE_SHOT_EXPOSURE    = 0x01,
        SINGLE_SHOT_EXP_TRIGGER = 0x02
    };Q_ENUMS(AUTO_EXPOSURE_FEATURES)

    //Multiple Exposure Trigger
    enum MET_MODE{
        MET_DISABLE = 0x00,
        MET_ENABLE  = 0x01
    };Q_ENUMS(MET_MODE)

    enum MULTIPLE_FRAME_SET{
        FRAME_SET_OFF = 0x00,
        FRAME_SET_2   = 0x01,
        FRAME_SET_4   = 0x02
    };Q_ENUMS(MULTIPLE_FRAME_SET)

    //Ultra Short Exposure
    enum ULTRA_SHORT_EXPOSURE{
        EXP_DISABLE = 0x00,
        EXP_ENABLE  = 0x01
    };Q_ENUMS(ULTRA_SHORT_EXPOSURE)

    enum ANTI_FLICKER_DETECTION{
        MODE_50Hz     = 0x00,
        MODE_60Hz     = 0x01,
        MODE_DISABLE  = 0x02
    };
    Q_ENUMS(ANTI_FLICKER_DETECTION)

    signals:
        void cameraModeChanged(uint cameraMode);
        void flipMirrorModeChanged(uint flipMode);
        void strobeModeChanged(uint strobe);
        void blackLevelValueChanged(uint blackLevelValue);
        void imageBurstChanged(uint burstLength);

        void gainModeReceived(uint gainMode);
        void autoGainFeatureReceived(uint autoGainFeature);
        void manualGainValuesReceived(uint current, uint min, uint max, uint stepValue);

        void exposureModeReceived(uint exposureMode);
        void autoExposureFeaturesReceived(uint autoExpFeature);
        void manualExposureValuesReceived(uint seconds, uint milliSeconds, uint microSeconds);

        void currentBrightnessReceived(uint current);
        void minBrightnessReceived(uint min);
        void maxBrightnessReceived(uint max);
        void brightnessStepValueReceived(uint stepValue);

        void lowerGainLimitReceived(uint lowerLimit);
        void upperGainLimitReceived(uint upperLimit);
        void sliderMinimumReceived(uint min);
        void sliderMaximumReceived(uint max);
        void gainLimitStepValueReceived(uint stepValue);

        void lowerLimitExposure(uint seconds, uint milliSeconds, uint microSeconds);
        void upperLimitExposure(uint seconds, uint milliSeconds, uint microSeconds);

        void antiFlickerModeReceived(uint antiFlicker);

        void gainStatisticsReceived(uint gainStatistics);
        void exposureStatisticsReceived(uint seconds, uint milliSeconds, uint microSeconds);

        void temperatureReceived(uint temperature);

        void multiExposureModeReceived(uint mode);
        void multiExposureValueReceived(uint count);

        void multiFrameSetModeReceived(uint mode);
        void frameSet1Received(uint gainVal1, uint sec1, uint milliSec1, uint microSec1);
        void frameSet2Received(uint gainVal2, uint sec2, uint milliSec2, uint microSec2);
        void frameSet3Received(uint gainVal3, uint sec3, uint milliSec3, uint microSec3);
        void frameSet4Received(uint gainVal4, uint sec4, uint milliSec4, uint microSec4);

        void ultraShortExposureModeReceived(uint mode);
        void minUltraShortExposureReceived(uint min);
        void maxUltraShortExposureReceived(uint max);
        void currentUltraShortExpValueReceived(uint current);

        void currentTriggerValueReceived(uint triggerValue);

        void indicateCommandStatus(QString title, QString text);
        void indicateExposureValueRangeFailure(QString title, QString text);
        void indicateExposureLimitsInvalidInput(QString title, QString text);
        void indicateGainValueRangeFailure();
        void indicateSoftwareTriggerFailure(QString title, QString text);
        void indicateMultipleExposureTriggerFailure(QString title, QString text);
        void indicateMultiFrameSetInvalidInput(QString title, QString text);

    public slots:
        bool setCameraMode(cameraModes cameraMode);
        bool getCameraMode();

        bool setOrientation(bool horzModeSel, bool vertiModeSel);
        bool getOrientation();

        bool getGainMode();
        bool setGainMode(GAIN_MODE gainMode, AUTO_GAIN_FEATURES autoGain, uint manualValues);

        bool setStrobeMode(strobeMode strobe);
        bool getStrobeMode();

        bool setBlackLevelAdjustment(uint blackLevelValue);
        bool getBlackLevelAdjustment();

        bool setBurstLength(uint burstlength);
        bool getBurstLength();

        bool getExposure();
        bool setExposure(EXPOSURE_MODE expMode, AUTO_EXPOSURE_FEATURES autoExposure, uint seconds, uint milliSeconds, uint microSeconds);

        bool getTargetBrightness();
        bool setTargetBrightness(uint brightness);

        bool getAutoGainLimit();
        bool setAutoGainLimit(uint lowerLimit, uint upperLimit);

        bool getAutoExposureLowerLimit();
        bool setAutoExposureLowerLimit(uint seconds, uint milliSeconds, uint microSeconds);

        bool getAutoExposureUpperLimit();
        bool setAutoExposureUpperLimit(uint seconds, uint milliSeconds, uint microSeconds);

        bool setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode);
        bool getAntiFlickerMode();

        bool readStatistics();

        bool readTemperature();

        bool getMultiExposureTrigger();
        bool setMultiExposureTrigger(MET_MODE metMode, uint exposureCount);

        bool getMultiFrameSet();
        bool setMultiFrameSet(MULTIPLE_FRAME_SET multiFrameSet, uint gain1, uint second1, uint milliSecond1, uint microSecond1, uint gain2, uint second2, uint milliSecond2, uint microSecond2,  uint gain3,uint second3, uint milliSecond3, uint microSecond3, uint gain4, uint second4, uint milliSecond4, uint microSecond4);

        bool getUltraShortExposure();
        bool setUltraShortExposure(ULTRA_SHORT_EXPOSURE expMode, uint ultraShortExposure);

        bool getSoftwareTrigger();
        bool setSoftwareTrigger(uint triggerValue);

        bool setToDefaultValues();
        bool saveConfiguration();
};
#endif // SEE3CAM_50CUG_M_H
