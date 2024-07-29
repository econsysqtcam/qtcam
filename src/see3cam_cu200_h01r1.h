#ifndef SEE3CAM_CU200_H01R1_H
#define SEE3CAM_CU200_H01R1_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1         0xC0
#define CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1         0x0D

#define GET_EXPOSURE_SEE3CAM_CU200_H01R1                0x13
#define SET_EXPOSURE_SEE3CAM_CU200_H01R1                0x14

#define GET_TARGET_BRIGHTNESS_SEE3CAM_CU200_H01R1       0x21
#define SET_TARGET_BRIGHTNESS_SEE3CAM_CU200_H01R1       0x22

#define GET_AUTO_EXP_ROI_SEE3CAM_CU200_H01R1            0x1F
#define SET_AUTO_EXP_ROI_SEE3CAM_CU200_H01R1            0x20

#define GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU200_H01R1  0x23
#define SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU200_H01R1  0x24

#define GET_BLACK_LEVEL_SEE3CAM_CU200_H01R1             0x07
#define SET_BLACK_LEVEL_SEE3CAM_CU200_H01R1             0x08

#define GET_CAMERA_MODE_SEE3CAM_CU200_H01R1             0x15
#define SET_CAMERA_MODE_SEE3CAM_CU200_H01R1             0x16

#define GET_STROBE_SEE3CAM_CU200_H01R1                  0x17
#define SET_STROBE_SEE3CAM_CU200_H01R1                  0x18

#define GET_ORIENTATION_SEE3CAM_CU200_H01R1             0x19
#define SET_ORIENTATION_SEE3CAM_CU200_H01R1             0x1A

#define READ_STATISTICS_SEE3CAM_CU200_H01R1             0x29

#define SET_STILL_PROPERTIES_SEE3CAM_CU200_H01R1        0x2A

#define READ_ISP_FIRMWARE_VERSION_SEE3CAM_CU200_H01R1   0x2B

#define SET_TO_DEFAULT_SEE3CAM_CU200_H01R1              0xFF

#define SAVE_CONFIGURATION_SEE3CAM_CU200_H01R1          0x42

#define RESET_COMMAND_1_SEE3CAM_CU200_H01R1             0x32
#define RESET_COMMAND_2_SEE3CAM_CU200_H01R1             0x04

#define DEFAULT_CONFIGURATION                           0x00
#define USER_DEFINED_CONFIGURATION                      0x01

#define GET_EXPOSURE_COMPENSATION_SEE3CAM_CU200         0x2C
#define SET_EXPOSURE_COMPENSATION_SEE3CAM_CU200         0x2D

#define GET_AUTO_GAIN_LIMIT_SEE3CAM_CU200               0x2E
#define SET_AUTO_GAIN_LIMIT_SEE3CAM_CU200               0x2F


#define GET_FAIL		       0x00
#define GET_SUCCESS		       0x01

#define EXPOSURE_MIN           100
#define EXPOSURE_MAX           1000000

#define EXPOSURE_LIMIT_MIN     100
#define EXPOSURE_LIMIT_MAX     1000000

class See3CAM_CU200M_H01R1 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    See3CAM_CU200M_H01R1();
    ~See3CAM_CU200M_H01R1();

    QString _title;
    QString _text;
    QString message;

    enum CAMERA_MODE {
        MASTER_MODE  = 0x00,
        TRIGGER_MODE = 0x01
    };
    Q_ENUMS(CAMERA_MODE)

    enum EXPOSURE_MODE{
        AUTO_EXPOSURE    = 0x00,
        MANUAL_EXPOSURE  = 0x01
    };
    Q_ENUMS(EXPOSURE_MODE)

    enum AUTO_EXPOSURE_FEATURE{
        CONTINIOUS_EXPOSURE  = 0x00,
        SINGLE_SHOT_EXPOUSRE = 0x01
    };
    Q_ENUMS(AUTO_EXPOSURE_FEATURE)

    enum STROBE_MODE{
        STROBE_OFF  = 0x00,
        STROBE_ON   = 0x01
    };
    Q_ENUMS(STROBE_MODE)

    enum ORIENTATION{
        NORMAL     = 0x00,
        HORIZONTAL = 0x01,
        VERTICAL   = 0x02,
        ROTATE_180 = 0x03
    };
    Q_ENUMS(ORIENTATION)

    enum SAVECONFIGURATION{
        DEFAULT      = 0x00,
        USER_DEFINED = 0x01
    };
    Q_ENUMS(SAVECONFIGURATION)


    enum autoExpRoiModes{
        FULL_ROI    = 0x00,
        MANUAL_ROI  = 0x01,
    };
    Q_ENUMS(autoExpRoiModes)


    enum ANTI_FLICKER_DETECTION{
        MODE_50Hz    = 0x00,
        MODE_60Hz    = 0x01,
        MODE_DISABLE = 0x02,
    };
    Q_ENUMS(ANTI_FLICKER_DETECTION)

signals:

    void gainModeReceived(uint mode, uint min, uint max, uint stepValue, uint autoFeature, uint manualValue);

    void currentAutoGainLimitValuesReceived(uint min, uint max, uint stepValue, uint lowerLimit, uint upperLimit);

    void exposurePropertiesReceived(uint mode, uint autoFeature, uint manualValue);

    void currentAutoExposureLimitReceived(uint lowerLimit, uint upperLimit);

    void targetBrightnessPropertiesReceived(int min, int max, int stepValue, int current);

    void roiAutoExpModeReceived(uint roiMode, uint winSize);

    void antiFlickerModeReceived(uint antiFlicker);

    void blacklevelPropertiesReceived(uint min, uint max, uint stepValue, uint current);

    void cameraModeReceived(uint cameraMode);

    void strobeModeReceived(uint strobeMode);

    void flipMirrorModeChanged(uint flipMirrorModeValues);

    void exposureCompensationReceived(uint exposure);

    void autoGainUpperLimitReceived(uint min, uint max, uint step, uint current);

    void exposureStatisticsReceived(uint exposure);
    void gainStatisticsReceived(uint gain);

    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateCommandStatus(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);
    void indicateExposureCompensationRangeFailure(QString title, QString text);

public slots:

    bool getExposure();
    bool setExposure(EXPOSURE_MODE expMode, AUTO_EXPOSURE_FEATURE autoFeature, uint manualExposure);

    bool getTargetBrightness();
    bool setTargetBrightness(uint targetBrightness);

    bool getAutoExpROIModeAndWindowSize();
    bool setROIAutoExposure(autoExpRoiModes see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode);
    bool getAntiFlickerMode();

    bool getBlackLevel();
    bool setBlackLevel(uint blackLevel);

    bool getCameraMode();
    bool setCameraMode(CAMERA_MODE camMode);

    bool getStrobeMode();
    bool setStrobeMode(STROBE_MODE strobeMode);

    bool getOrientation();
    bool setOrientation(bool horzModeSel, bool vertiModeSel);

    bool getExposureCompensation();
    bool setExposureCompensation(uint exposureCompensation);

    bool getAutoGainUpperLimit();
    bool setAutoGainUpperLimit(uint upperLimit);

    bool readStatistics();

    bool setStillProperties(bool isEnable);

    bool readISPFirmwareVersion();

    bool saveConfiguration(SAVECONFIGURATION configType);

    bool resetDevice();

    bool setToDefault();
};

#endif // SEE3CAM_CU200_H01R1_H
