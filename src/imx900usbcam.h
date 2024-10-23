#ifndef IMX900USBCAM_H
#define IMX900USBCAM_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_ID_1_IMX900                  0xC0
#define CAMERA_ID_2_IMX900                  0x10

#define GET_CAMERA_MODE_IMX900              0x01
#define SET_CAMERA_MODE_IMX900              0x02

#define GET_ORIENTATION_MODE_IMX900         0x03
#define SET_ORIENTATION_MODE_IMX900         0x04

#define GET_GAIN_IMX900                     0x05
#define SET_GAIN_IMX900                     0x06

#define GET_BLACK_LEVEL_IMX900              0x07
#define SET_BLACK_LEVEL_IMX900              0x08

#define GET_STROBE_MODE_IMX900              0x09
#define SET_STROBE_MODE_IMX900              0x0A

#define GET_IMAGE_BURST_IMX900              0x0B
#define SET_IMAGE_BURST_IMX900              0x0C

#define GET_EXPOSURE_IMX900                 0x0D
#define SET_EXPOSURE_IMX900                 0x0E

#define GET_TARGET_BRIGHTNESS_IMX900        0x0F
#define SET_TARGET_BRIGHTNESS_IMX900        0x10

#define GET_AUTO_GAIN_LIMIT_IMX900          0x11
#define SET_AUTO_GAIN_LIMIT_IMX900          0x12

#define GET_EXPOSURE_LOWER_LIMIT_IMX900     0x13
#define SET_EXPOSURE_LOWER_LIMIT_IMX900     0x14

#define GET_EXPOSURE_UPPER_LIMIT_IMX900     0x15
#define SET_EXPOSURE_UPPER_LIMIT_IMX900     0x16

#define GET_ANTI_FLICKER_DETECTION_IMX900   0x17
#define SET_ANTI_FLICKER_DETECTION_IMX900   0x18

#define READ_STATISTICS_IMX900              0x19

#define READ_TEMPERATURE_IMX900             0X1A

#define GET_HDR_IMX900                      0x1B
#define SET_HDR_IMX900                      0x1C

#define GET_QUAD_SHUTTER_CONTROL_IMX900     0x1D
#define SET_QUAD_SHUTTER_CONTROL_IMX900     0x1E

#define GET_FAST_AUTO_EXPOSURE_IMX900       0x1F
#define SET_FAST_AUTO_EXPOSURE_IMX900       0x20

#define GET_DUAL_TRIGGER_IMX900             0x21
#define SET_DUAL_TRIGGER_IMX900             0x22

#define GET_SELF_TRIGGER_IMX900             0x23
#define SET_SELF_TRIGGER_IMX900             0x24

#define GET_MULTI_FRAME_SET_IMX900          0x25
#define SET_MULTI_FRAME_SET_IMX900          0x26

#define GET_TONE_CONTROL_IMX900             0x27
#define SET_TONE_CONTROL_IMX900             0x28

#define GET_SHORT_EXPOUSRE_IMX900           0x29
#define SET_SHORT_EXPOUSRE_IMX900           0x2A

#define SET_TO_DEFAULT_IMX900               0xFF

#define SAVE_CONFIGURATION_IMX900USBCAM     0x42
#define SAVE_IMX900USBCAM                   0x01


#define GAIN_MIN            0
#define GAIN_MAX            48

#define EXPOSURE_MIN        15
#define EXPOSURE_MAX        2000000


class IMX900USBCAM : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    uvccamera uvc;

    void initializeBuffers();

public:
    explicit IMX900USBCAM(QObject *parent = nullptr);

    uint lowerLimitSec, lowerLimitMilliSec, lowerLimitMicroSec;
    uint upperLimitSec, upperLimitMilliSec, upperLimitMicroSec;

    enum CAMERA_MODE{
        MASTER           = 0x01,
        TRIGGER_EXP      = 0x02,
        TRIGGER_ACQ      = 0x03,
        TRIGGER_SOFTWARE = 0x04
    };Q_ENUMS(CAMERA_MODE)

    enum ORIENTATION{
        NORMAL     = 0x01,
        VERTICAL   = 0x02,
        HORIZONTAL = 0x03,
        ROTATE_180 = 0x04
    };Q_ENUMS(ORIENTATION)

    enum GAIN_MODE{
        AUTO_GAIN = 0x00,
        MANUAL_GAIN = 0x01
    };Q_ENUMS(GAIN_MODE)


    enum AUTO_GAIN_FEATURES{
        CONTINIOUS_GAIN     = 0x00,
        SINGLE_GAIN         = 0x01,
        SINGLE_GAIN_TRIGGER = 0x02
    };Q_ENUMS(AUTO_GAIN_FEATURES)

    enum STROBE_MODE{
        STROBE_OFF = 0x00,
        STROBE_ON  = 0x01
    };Q_ENUMS(STROBE_MODE)


    enum EXPOSURE_MODE{
        AUTO_EXPOSURE   = 0x00,
        MANUAL_EXPOSURE = 0x01
    };Q_ENUMS(EXPOSURE_MODE)


    enum AUTO_EXPOSURE_FEATURES{
        CONTINIOUS_EXP     = 0x00,
        SINGLE_EXP         = 0x01,
        SINGLE_EXP_TRIGGER = 0x02
    };Q_ENUMS(AUTO_EXPOSURE_FEATURES)

    enum ANTI_FLICKER_DETECTION{
        MODE_50Hz    = 0x00,
        MODE_60Hz    = 0x01,
        MODE_DISABLE = 0x02,
    };
    Q_ENUMS(ANTI_FLICKER_DETECTION)

    enum MULTI_FRAME_SET{
        FRAME_SET_OFF = 0x00,
        FRAME_SET_2   = 0x01,
        FRAME_SET_4   = 0x02
    };Q_ENUMS(MULTI_FRAME_SET)

signals:
    void currentCameraMode(uint mode);
    void currentFlipMode(uint flipMirrorModeValues);
    void currentBlackLevelValue(uint value);
    void currentStrobeMode(uint mode);
    void currentBurstLength(uint burstLength);
    void currentGainProperties(uint mode, uint feature, uint min, uint max, uint current, uint step);
    void currentExposureProperties(uint mode, uint feature, uint seconds, uint milliSeconds, uint microSeconds);
    void currentTargetBrightness(uint min, uint max, uint step, uint value);
    void currentGainLimit(uint min, uint max, uint step, uint lowerLimit, uint upperLimit);
    void currentExposureLowerLimit(uint seconds, uint milliSeconds, uint microSeconds);
    void currentExposureUpperLimit(uint seconds, uint milliSeconds, uint microSeconds);
    void currentAntiFlickerMode(uint mode);
    void currentStatistics(uint seconds, uint milliSeconds, uint microSeconds, uint gain);
    void currentTemperature(uint temperature);
    void currentHDRStatus(uint status);
    void currentQuadShutterControlStatus(uint status);
    void currentFastAutoExposureStatus(uint status);
    void currentToneControlStatus(uint status);
    void currentShortExposureStatus(uint status, uint min, uint max, uint current);

    void multiFrameSetModeReceived(uint mode);
    void frameSet1Received(uint gain1, uint exposure1);
    void frameSet2Received(uint gain2, uint exposure2);
    void frameSet3Received(uint gain3, uint exposure3);
    void frameSet4Received(uint gain4, uint exposure4);

    void currentDualModeProperties(uint mode, uint switchingDivision, uint min, uint max, uint step, uint min1, uint max1, uint step1, uint gain, uint gain1);

    void currentSelfTriggerMode(uint mode);
    void currentHCropSize(uint min, uint max, uint step, uint current);
    void currentVCropSize(uint min, uint max, uint step, uint current);
    void currentSensingFrameExposure(uint current);
    void currentHighThreshold(uint hLevel, uint lLevel);
    void currentCountThreshold(uint hCount, uint lCount);
    void currentSensingFrameMode(uint mode);
    void currentSelfTriggerForce(uint mode);
    void currentDetectedPixels(uint hPixels, uint vPixels);
    void currentGainForSensingFrame(uint min, uint max, uint step, uint gain);
    void currentGainForCapturingFrame(uint min, uint max, uint step, uint gain);

    void indicateFailureStatus(QString title, QString text);
    void indicateCommandStatus(QString title, QString text);
    void indicateExposureLimitsInvalidInput(QString title, QString text);
    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateMultiFrameSetInvalidInput(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);
    void indicateGainValueRangeFailure();

public slots:

    bool getCameraMode();
    bool setCameraMode(CAMERA_MODE cameraMode);

    bool getOrientation();
    bool setOrientation(bool horzModeSel, bool vertiModeSel);

    bool getGainMode();
    bool setGainMode(GAIN_MODE gainmode, AUTO_GAIN_FEATURES autoModes, uint manualGain);

    bool getGainLimit();
    bool setGainLimit(uint lowerLimit, uint upperLimit);

    bool getBlackLevelAdjustment();
    bool setBlackLevelAdjustment(uint blackLevelValue);

    bool getStrobeMode();
    bool setStrobeMode(STROBE_MODE strobe);

    bool getBurstLength();
    bool setBurstLength(uint burstlength);

    bool getExposureMode();
    bool setExposureMode(EXPOSURE_MODE gainmode, AUTO_EXPOSURE_FEATURES autoModes, uint seconds, uint milliSeconds, uint microSeconds);

    bool getTargetBrightness();
    bool setTargetBrightness(uint targetBrightness);

    bool getAutoExposureLowerLimit();
    bool setAutoExposureLowerLimit(uint seconds, uint milliSeconds, uint microSeconds);

    bool getAutoExposureUpperLimit();
    bool setAutoExposureUpperLimit(uint seconds, uint milliSeconds, uint microSeconds);

    bool getAntiFlickerMode();
    bool setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode);

    bool readStatistics();

    bool calculateTemperature();

    bool getHighDynamicRange();
    bool setHighDynamicRange(uint status);

    bool getQuadShutterControl();
    bool setQuadShutterControl(uint status);

    bool getFastAutoExposure();
    bool setFastAutoExposure(uint status);

    bool getMultiFrameSet();
    bool setMultiFrameSet(MULTI_FRAME_SET multiFrameSet, uint gain1, uint exposure1, uint gain2, uint exposure2, uint gain3, uint exposure3, uint gain4, uint exposure4);

    bool getToneControl();
    bool setToneControl(uint status);

    bool getShortExposureMode();
    bool setShortExposureMode(uint status, uint value);

    bool getDualTrigger();
    bool setDualTrigger(uint mode, uint division, uint gain, uint gain1);

    bool getSelfTrigger();
    bool setSelfTrigger(uint mode, uint vidResolnWidth, uint vidResolnHeight, uint hCropPos, uint vCropPos, uint hCropSize, uint vCropSize, uint exposure, uint hLevel, uint lLevel, uint hCount, uint lCount, uint sensingMode, uint forceMode, uint hDetPixels, uint vDetPixels, uint sensingGain, uint capturingGain);

    bool setToDefaultValues();

    bool saveConfiguration();
};

#endif // IMX900USBCAM_H
