#ifndef See3CAM_CU135M_H01R1_H
#define See3CAM_CU135M_H01R1_H

#include "uvccamera.h"
#include <QObject>

#define CAMERA_CONTROL_ID_SEE3CAM_CU135M_H01R1_H          0xB0

#define GET_STREAM_MODE_CU1330M                 0x01
#define SET_STREAM_MODE_CU1330M                 0x02

#define GET_FLASH_MODE_CU1330M                  0x03
#define SET_FLASH_MODE_CU1330M                  0x04

#define GET_ORIENTATION_CU1330M                 0x05
#define SET_ORIENTATION_CU1330M                 0x06

#define GET_EXP_ROI_MODE_CU1330M                0x07
#define SET_EXP_ROI_MODE_CU1330M                0x08


#define GET_BLACKLEVEL_CORRECTION_SEE3CAM_CU135M_H01R1_H       0x09
#define SET_BLACKLEVEL_CORRECTION_SEE3CAM_CU135M_H01R1_H       0x0A

#define GET_TARGET_BRIGHTNESS_SEE3CAM_CU135M_H01R1_H           0x0B
#define SET_TARGET_BRIGHTNESS_SEE3CAM_CU135M_H01R1_H           0x0C

#define GET_AUTO_GAIN_MODE_SEE3CAM_CU135M_H01R1_H              0x0D
#define SET_AUTO_GAIN_MODE_SEE3CAM_CU135M_H01R1_H              0x0E

#define GET_AUTO_GAIN_LIMIT_SEE3CAM_CU135M_H01R1_H             0x0F
#define SET_AUTO_GAIN_LIMIT_SEE3CAM_CU135M_H01R1_H             0x10

#define GET_EXPOSURE_MODE_SEE3CAM_CU135M_H01R1_H               0x11
#define SET_EXPOSURE_MODE_SEE3CAM_CU135M_H01R1_H               0x12

#define GET_AUTO_EXPOSURE_LOWER_LIMIT_SEE3CAM_CU135M_H01R1_H   0x13
#define SET_AUTO_EXPOSURE_LOWER_LIMIT_SEE3CAM_CU135M_H01R1_H   0x14

#define GET_AUTO_EXPOSURE_UPPER_LIMIT_SEE3CAM_CU135M_H01R1_H   0x15
#define SET_AUTO_EXPOSURE_UPPER_LIMIT_SEE3CAM_CU135M_H01R1_H   0x16

#define GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU135M_H01R1_H      0x17
#define SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU135M_H01R1_H      0x18

#define READ_STATISTICS_SEE3CAM_CU135M_H01R1_H                 0x19

#define SET_TO_DEFAULT_SEE3CAM_CU135M_H01R1_H                  0xFF

#define GET_SUCCESS                                            0x01
#define GET_FAILURE                                            0x00

#define EXPOSURE_LIMIT_MIN     100
#define EXPOSURE_LIMIT_MAX     1000000


class See3CAM_CU135M_H01R1 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    See3CAM_CU135M_H01R1();
    ~See3CAM_CU135M_H01R1();

    uint exposureLowerLimit;
    uint exposureUpperLimit;

    enum flashStateValues{
         FLASHMODE_OFF = 0x00,
         FLASHMODE_TORCH = 0x02,
         FLASHMODE_STROBE = 0x01,
     };
     Q_ENUMS(flashStateValues)

    enum streamModes {
       MODE_MASTER  = 0x00,
       MODE_TRIGGER = 0x01
    };
    Q_ENUMS(streamModes)

    enum flipControls {
       FLIP_NORMAL =0x00,
       FLIP_HORIZONTAL =0x01,
       FLIP_VERTICAL =0x02,
       FLIP_BOTH =0x03
    };
    Q_ENUMS(flipControls)

    enum camROIAutoExpMode {
        AutoExpFull = 0x01,
        AutoExpManual = 0x02,
    };
    Q_ENUMS(camROIAutoExpMode)

    enum blackLevelAdjustment {
        ENABLE  = 0x01,
        DISABLE = 0x02,
    };
    Q_ENUMS(blackLevelAdjustment)

    enum gainMode{
        AUTO_GAIN   = 0x00,
        MANUAL_GAIN = 0x01
    };Q_ENUMS(gainMode)

    enum autoGain{
        CONTINUOUS  = 0x00,
        SINGLE_SHOT = 0x01
    };Q_ENUMS(autoGain)

    enum exposureModes{
        CONTINIOUS_EXPOSURE = 0x00,
        SINGLE_SHOT_EXPOSURE = 0x01
    };Q_ENUMS(exposureModes)

    enum antiFlickerDetection{
        MODE_50Hz     = 0x00,
        MODE_60Hz     = 0x01,
        MODE_DISABLE  = 0x02
    };
    Q_ENUMS(antiFlickerDetection)

signals:
    void flashModeValue(uint flashMode);
    void streamModeValue(uint streamMode);
    void flipCtrlValue(uint flipValue);
    void roiAutoExpMode(uint roiMode, uint winSize);

    void blackLevelAdjustmentChanged(uint blacklevel);

    void targetBrightnessReceived(uint brightness);
    void brightnessMinSliderReceived(uint sliderMin);
    void brightnessMaxSliderReceived(uint sliderMax);
    void targetBrightnessStepValue(uint stepValue);

    void antiFlickerModeRecieved(uint antiFlicker);

    void gainModeReceived(uint gainMode);
    void autoGainFeatureRecieved(uint autoGain);
    void manualGainValueRecieved(uint manualGain);
    void gainMinSliderValueReceived(uint minSlider);
    void gainMaxSliderValueReceived(uint maxSlider);
    void gainStepValueReceived(uint gainStepValue);

    void lowerGainLimitRecieved(uint lowerLimit);
    void upperGainLimitRecieved(uint upperLimit);
    void sliderMinimumRecieved(uint sliderMin);
    void sliderMaximumRecieved(uint sliderMax);
    void gainLimitStepValueReceived(uint stepValue);

    void exposureModeRecieved(uint exposureMode);

    void lowerLimitExposure(uint lowerLimit);
    void upperLimitExposure(uint upperLimit);
    void gainStatisticsRecieved(uint gainStatistics);
    void exposureStatisticsRecieved(uint exposureStatistics);

    void indicateCommandStatus(QString title, QString text);
    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateGainValueRangeFailure();

public slots:

    bool setFlashState(flashStateValues flashMode);
    bool getFlashState();

    bool getStreamMode();
    bool setStreamMode(streamModes streamMode);

    bool setFlipCtrlValue(bool horzModeSel, bool vertiModeSel);
    bool getFlipCtrlValue();

    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool getGainMode();
    bool setGainMode(gainMode gainmode,autoGain autoModes,uint manualGain);     

    bool getGainLimit();
    bool setGainLimit(uint lowerLimit, uint upperLimit);

    bool setBlackLevelAdjustment(blackLevelAdjustment blacklevel);
    bool getBlackLevelAdjustment();

    bool setTargetBrightness(uint targetBrightness);
    bool getTargetBrightness();

    bool setExposureMode(exposureModes exposureMode);
    bool getExposureMode();

    bool setAutoExposureLowerLimit(uint lowerLimit);
    bool getAutoExposureLowerLimit();

    bool setAutoExposureUpperLimit(uint upperLimit);
    bool getAutoExposureUpperLimit();

    bool setAntiFlickerMode(antiFlickerDetection antiFlickerMode);
    bool getAntiFlickerMode();

    bool getStatistics();

    bool setToDefault();
};

#endif // SEE3CAM_CU1330M_H
