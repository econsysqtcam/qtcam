#ifndef SEE3CAM_CU200_H
#define SEE3CAM_CU200_H


#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID_1_SEE3CAM_CU200      0xC0
#define CAMERA_CONTROL_ID_2_SEE3CAM_CU200      0x02

#define GET_GAIN_MODE_SEE3CAM_CU200            0x01
#define SET_GAIN_MODE_SEE3CAM_CU200            0x02

#define GET_RB_GAIN_SEE3CAM_CU200              0x03
#define SET_RB_GAIN_SEE3CAM_CU200              0x04

#define GET_CC_MATRIX_SEE3CAM_CU200            0x05
#define SET_CC_MATRIX_SEE3CAM_CU200            0x06

#define GET_BLACK_LEVEL_SEE3CAM_CU200          0x07
#define SET_BLACK_LEVEL_SEE3CAM_CU200          0x08

#define GET_BRIGHTNESS_SEE3CAM_CU200           0x09
#define SET_BRIGHTNESS_SEE3CAM_CU200           0x0A

#define GET_CONTRAST_SEE3CAM_CU200             0x0B
#define SET_CONTRAST_SEE3CAM_CU200             0x0C

#define GET_SATURATION_SEE3CAM_CU200           0x0D
#define SET_SATURATION_SEE3CAM_CU200           0x0E

#define GET_COLOR_TEMPERATURE_SEE3CAM_CU200    0x0F
#define SET_COLOR_TEMPERATURE_SEE3CAM_CU200    0x10

#define GET_GAMMA_SEE3CAM_CU200                0x11
#define SET_GAMMA_SEE3CAM_CU200                0x12

#define GET_EXPOSURE_SEE3CAM_CU200             0x13
#define SET_EXPOSURE_SEE3CAM_CU200             0x14

#define GET_CAMERA_MODE_SEE3CAM_CU200          0x15
#define SET_CAMERA_MODE_SEE3CAM_CU200          0x16

#define GET_STROBE_MODE_SEE3CAM_CU200          0x17
#define SET_STROBE_MODE_SEE3CAM_CU200          0x18

#define GET_ORIENTATION_MODE_SEE3CAM_CU200     0x19
#define SET_ORIENTATION_MODE_SEE3CAM_CU200     0x1A

#define SAVE_CONFIGURATION_SEE3CAM_CU200       0x42
#define DEFAULT_CONFIGURATION                  0x00
#define USER_DEFINED_CONFIGURATION             0x01

#define SET_TO_DEFAULT_SEE3CAM_CU200           0xFF

#define RESET_COMMAND_1                        0x32
#define RESET_COMMAND_2                        0x04

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define EXPOSURE_MIN 100
#define EXPOSURE_MAX 1000000

union
{
    float f;
    uint32_t u;
}fCurrentRGain, fRGainMin, fRGainMax, fRGainStepValue , fCurrentBGain , fBGainMin, fBGainMax, fBGainStepValue, fCurrentBrightness, fMinBrightness, fMaxBrightness, fBrightnessStepValue, fCurrentSaturation, fMinSaturation, fMaxSaturation, fSaturationStepValue , fCurrentGamma, fMinGamma, fMaxGamma, fGammaStepValue, fCurrentRr, fCurrentRg, fCurrentRb, fCurrentGr, fCurrentGg, fCurrentGb, fCurrentBr, fCurrentBg, fCurrentBb;


class SEE3CAM_CU200 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_CU200();
    ~SEE3CAM_CU200();

    enum GAIN_MODE{
        AUTO_MODE   = 0x00,
        MANUAL_MODE = 0x01
    };
    Q_ENUMS(GAIN_MODE)

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

signals:

    void gainPropertiesReceived(uint min, uint max, uint stepValue, uint current);
    void gainModeReceived(uint mode);

    void rGainPropertiesReceived(float minRGain, float maxRGain, float rGainStepValue, float currentRGain);
    void bGainPropertiesReceived(float minBGain, float maxBGain, float bGainStepValue, float currentBGain);

    void currentBGainReceived(float currentBGain);
    void minBGainReceived(float minBGain);
    void maxBGainReceived(float maxBGain);
    void bGainStepValueReceived(float bGainStepValue);

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

    void colorTemperatureReceived(uint colorTemp);

    void exposureModeReceived(uint mode);
    void exposureValueReceived(uint exposure);

    void cameraModeReceived(uint cameraMode);

    void strobeModeReceived(uint strobeMode);

    void flipMirrorModeChanged(uint flipMirrorModeValues);

    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateCommandStatus(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);


public slots:

    bool getGainMode();
    bool setGainMode(GAIN_MODE gainMode, uint gainValue);

    bool getRBGain();
    bool setRBGain(float RGain, float BGain);

    bool getColorCorrectionMatrix();
    bool setColorCorrectionMatrix(float Rr, float Rg, float Rb, float Gr, float Gg, float Gb, float Br, float Bg, float Bb);

    bool getBlackLevel();
    bool setBlackLevel(uint blackLevel);

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
    bool setExposure(EXPOSURE_MODE expMode, uint manualExposure);

    bool getCameraMode();
    bool setCameraMode(CAMERA_MODE camMode);

    bool getStrobeMode();
    bool setStrobeMode(STROBE_MODE strobeMode);

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool saveConfiguration(SAVECONFIGURATION configType);

    bool resetDevice();

    bool setToDefault();
};

#endif // SEE3CAM_CU200_H
