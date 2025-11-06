#ifndef SEE3CAM_CU200_H
#define SEE3CAM_CU200_H


#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID_1      0xC0
#define CAMERA_CONTROL_ID_2      0x02

#define GET_RB_GAIN              0x03
#define SET_RB_GAIN              0x04

#define GET_COLOR_CORRECTION_MATRIX            0x05
#define SET_COLOR_CORRECTION_MATRIX            0x06

#define GET_BLACK_LEVEL          0x07
#define SET_BLACK_LEVEL          0x08

#define GET_BRIGHTNESS           0x09
#define SET_BRIGHTNESS           0x0A

#define GET_CONTRAST             0x0B
#define SET_CONTRAST             0x0C

#define GET_SATURATION           0x0D
#define SET_SATURATION           0x0E

#define GET_COLOR_TEMPERATURE    0x0F
#define SET_COLOR_TEMPERATURE    0x10

#define GET_GAMMA                0x11
#define SET_GAMMA                0x12

#define GET_EXPOSURE             0x13
#define SET_EXPOSURE             0x14

#define GET_CAMERA_MODE          0x15
#define SET_CAMERA_MODE          0x16

#define GET_STROBE               0x17
#define SET_STROBE               0x18

#define GET_ORIENTATION          0x19
#define SET_ORIENTATION          0x1A

#define GET_AUTO_EXPOSURE_ROI         0x1F
#define SET_AUTO_EXPOSURE_ROI         0x20

#define GET_TARGET_BRIGHTNESS         0x21
#define SET_TARGET_BRIGHTNESS         0x22

#define GET_ANTI_FLICKER_DETECTION    0x23
#define SET_ANTI_FLICKER_DETECTION    0x24

#define GET_WHITE_BALANCE             0x25
#define SET_WHITE_BALANCE             0x26

#define GET_WHITE_BALANCE_PRESET      0x27
#define SET_WHITE_BALANCE_PRESET      0x28

#define SET_CROSS_STILL_PROPERTIES    0x2A

#define READ_STATISTICS               0x29

#define SAVE_CONFIGURATION            0x42

#define DEFAULT_CONFIGURATION         0x00
#define USER_DEFINED_CONFIGURATION    0x01

#define SET_TO_DEFAULT                0xFF

#define RESET_COMMAND_1               0x32
#define RESET_COMMAND_2               0x04

#define READ_ISP_FIRMWARE_VERSION     0x2B

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define EXPOSURE_MIN 100
#define EXPOSURE_MAX 1000000

#define EXPOSURE_LIMIT_MIN     100
#define EXPOSURE_LIMIT_MAX     1000000

#define GET_EXPOSURE_COMPENSATION_SEE3CAM_CU200   0x2C
#define SET_EXPOSURE_COMPENSATION_SEE3CAM_CU200   0x2D

#define GET_AUTO_GAIN_LIMIT_SEE3CAM_CU200         0x2E
#define SET_AUTO_GAIN_LIMIT_SEE3CAM_CU200         0x2F

union
{
    float f;
    uint32_t u;
}fCurrentRGain, fRGainMin, fRGainMax, fRGainStepValue , fCurrentBGain , fBGainMin, fBGainMax, fBGainStepValue, fCurrentBrightness, fMinBrightness, fMaxBrightness, fBrightnessStepValue, fCurrentSaturation, fMinSaturation, fMaxSaturation, fSaturationStepValue , fCurrentGamma, fMinGamma, fMaxGamma, fGammaStepValue, fCurrentRr, fCurrentRg, fCurrentRb, fCurrentGr, fCurrentGg, fCurrentGb, fCurrentBr, fCurrentBg, fCurrentBb;


struct Range{
    float current;
    float min;
    float max;
    float step;
    float defaultValue;
};

class SEE3CAM_CU200 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    static Range denoiseStrength;
    static Range sharpnessStrength;
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_CU200();
    ~SEE3CAM_CU200();

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

    enum WHITE_BALANCE_MODE{
        AUTO_WB   = 0x00,
        MANUAL_WB = 0x01
    };
    Q_ENUMS(WHITE_BALANCE_MODE)

    enum AUTO_WB_FEATURES{
        WB_CONTINIOUS  = 0x00,
        WB_SINGLE_SHOT = 0x01
    };
    Q_ENUMS(AUTO_WB_FEATURES)

    enum MANUAL_WB_MODES{ 
        COLOR_TEMPERATURE  = 0x00,
        PRESET = 0x01,
        PRO_MODE = 0x02
    };
    Q_ENUMS(MANUAL_WB_MODES)

    enum WB_PRESET{
        HZ_2300K   = 0x00,
        A_2800K    = 0x01,
        U30_3000K  = 0x02,
        TL84_4000K = 0x03,
        CWF_4100K  = 0x04,
        D50_5000K  = 0x05,
        D60_6000K  = 0x06,
        D65_6500K  = 0x07
    };
    Q_ENUMS(WB_PRESET)

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
    void currentGainProperties(uint autoGain, float rGain, float bGain);

    void rGainPropertiesReceived(float minRGain, float maxRGain, float rGainStepValue, float currentRGain);
    void bGainPropertiesReceived(float minBGain, float maxBGain, float bGainStepValue, float currentBGain);

    void currentRrValuesReceived(float currentRr);
    void currentRgValuesReceived(float currentRg);
    void currentRbValuesReceived(float currentRb);

    void currentGrValuesReceived(float currentGr);
    void currentGgValuesReceived(float currentGg);
    void currentGbValuesReceived(float currentGb);

    void currentBrValuesReceived(float currentBr);
    void currentBgValuesReceived(float currentBg);
    void currentBbValuesReceived(float currentBb);

    void currentBlackLevelReceived(uint currentBlackLevel);
    void minBlackLevelReceived(uint minBlackLevel);
    void maxBlackLevelReceived(uint maxBlackLevel);
    void blackLevelStepValueReceived(uint blackLevelStepValue);

    void brightnessPropertiesReceived(float minBrightness, float maxBrightness, float stepValue, float currentBrightness);
    void saturationPropertiesReceived(float minSaturation, float maxSaturation, float stepValue, float currentSaturation);
    void gammaPropertiesReceived(float minGamma, float maxGamma, float stepValue, float currentGamma);

    void currentContrastReceived(float currentContrast);
    void minContrastReceived(float minContrast);
    void maxContrastReceived(float maxContrast);
    void contrastStepValueReceived(float stepValue);

    void colorTemperatureReceived(uint min, uint max, uint stepValue, uint colorTemp);

    void exposurePropertiesReceived(uint mode, uint autoFeature, uint manualValue);

    void exposureCompensationReceived(uint exposure);

    void cameraModeReceived(uint cameraMode);

    void strobeModeReceived(uint strobeMode);

    void flipMirrorModeChanged(uint flipMirrorModeValues);

    void whiteBalancePropertiesReceived(uint mode, uint autoFeature, uint manualFeature);

    void whiteBalancePresetModeReceived(uint presetMode);

    void currentAutoExposure(uint exposure);

    void currentRMatrixElements(float Rr, float Rg, float Rb);
    void currentGMatrixElements(float Gr, float Gg, float Gb);
    void currentBMatrixElements(float Br, float Bg, float Bb);

    void roiAutoExpModeReceived(uint roiMode, uint winSize);

    void targetBrightnessPropertiesReceived(float min, float max, float stepValue, float current);

    void denoiseStrengthValuesReceived(float min, float max, float step, float current);
    void sharpnessStrengthValuesReceived(float min, float max, float step, float current);

    void antiFlickerModeReceived(uint antiFlicker);

    void autoGainUpperLimitReceived(uint min, uint max, uint step, uint current);

    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateCommandStatus(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);
    void indicateExposureCompensationRangeFailure(QString title, QString text);

public slots:

    bool getRBGain();
    bool setRBGain(float RGain, float BGain);

    bool getColorCorrectionMatrix();
    bool setColorCorrectionMatrix(float Rr, float Rg, float Rb, float Gr, float Gg, float Gb, float Br, float Bg, float Bb);

    bool getBlackLevel();
    bool setBlackLevel(uint blackLevel);

    bool getTargetBrightness();
    bool setTargetBrightness(uint targetBrightness);

    bool getBrightness();
    bool setBrightness(float brightness);

    bool getContrast();
    bool setContrast(uint Contrast);

    bool getSaturation();
    bool setSaturation(float saturation);

    bool getColorTemperature();
    bool setColorTemperature(uint colorTemp);

    bool getGammaCorrection();
    bool setGammaCorrection(float gamma);

    bool getExposure();
    bool setExposure(EXPOSURE_MODE expMode, AUTO_EXPOSURE_FEATURE autoFeature, uint manualExposure);

    bool getExposureCompensation();
    bool setExposureCompensation(uint exposureCompensation);

    bool getAutoGainUpperLimit();
    bool setAutoGainUpperLimit(uint upperLimit);

    bool getAutoExpROIModeAndWindowSize();
    bool setROIAutoExposure(autoExpRoiModes see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool getCameraMode();
    bool setCameraMode(CAMERA_MODE camMode);

    bool getStrobeMode();
    bool setStrobeMode(STROBE_MODE strobeMode);

    bool getOrientation();
    bool setOrientation(bool horzModeSel, bool vertiModeSel);

    bool getWhiteBalanceMode();
    bool setWhiteBalanceMode(WHITE_BALANCE_MODE mode, AUTO_WB_FEATURES autoFeature, MANUAL_WB_MODES manualFeature, WB_PRESET presetMode);

    bool setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode);
    bool getAntiFlickerMode();

    bool getDenoiseStrength();
    bool setDenoiseStrength(float denoiseStrength);

    bool getSharpnessStrength();
    bool setSharpnessStrength(float sharpnessStrength);

    bool setPropertiesForCrossStill(bool isEnable);

    bool readStatistics();

    bool saveConfiguration(SAVECONFIGURATION configType);

    bool resetDevice();

    bool readISPFirmwareVersion();

    bool setToDefault();
};

#endif // SEE3CAM_CU200_H

