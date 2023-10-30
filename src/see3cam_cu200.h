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

#define SAVE_CONFIGURATION_SEE3CAM_CU200       0x42
#define SAVE_SEE3CAM_CU200                     0x01

#define SET_TO_DEFAULT_SEE3CAM_CU200           0xFF

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define EXPOSURE_MIN 100
#define EXPOSURE_MAX 1000000

#define BUFFER_LENGTH_SEE3CAM_CU200		256

union
{
    float f;
    uint32_t u;
}fCurrentRGain, fRGainMin, fRGainMax, fRGainStepValue , fCurrentBGain , fBGainMin, fBGainMax, fBGainStepValue, fCurrentBrightness, fMinBrightness, fMaxBrightness, fBrightnessStepValue, fCurrentSaturation, fMinSaturation, fMaxSaturation, fSaturationStepValue , fCurrentGamma, fMinGamma, fMaxGamma, fGammaStepValue, fCurrentRr, fMinRr, fMaxRr, fStepRr, fCurrentRg, fMinRg, fMaxRg, fStepRg, fCurrentRb, fMinRb, fMaxRb, fStepRb, fCurrentGr, fMinGr, fMaxGr, fStepGr, fCurrentGg, fMinGg, fMaxGg, fStepGg, fCurrentGb, fMinGb, fMaxGb, fStepGb, fCurrentBr, fMinBr, fMaxBr, fStepBr, fCurrentBg, fMinBg, fMaxBg, fStepBg, fCurrentBb, fMinBb, fMaxBb, fStepBb;

//fCurrentRr, fMinRr, fMaxRr, fCurrentRg, fMinRg, fMaxRg, fCurrentRb, fMinRb, fMaxRb,

//fCurrentGr, fMinGr, fMaxGr, fCurrentGg, fMinGg, fMaxGg, fCurrentGb, fMinGb, fMaxGb,

//fCurrentBr, fMinBr, fMaxBr, fCurrentBg, fMinBg, fMaxBg, fCurrentBb, fMinBb, fMaxBb,


class SEE3CAM_CU200 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH_SEE3CAM_CU200];
    unsigned char g_in_packet_buf[BUFFER_LENGTH_SEE3CAM_CU200];
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

signals:

    void currentGainReceived(uint gainValue);
    void minGainReceived(uint minGain);
    void maxGainReceived(uint maxGain);
    void gainStepValueReceived(uint gainStep);

    void currentRGainReceived(float currentRGain);
    void minRGainReceived(float minRGain);
    void maxRGainReceived(float maxRGain);
    void rGainStepValueReceived(float rGainStepValue);

    void currentBGainReceived(float currentBGain);
    void minBGainReceived(float minBGain);
    void maxBGainReceived(float maxBGain);
    void bGainStepValueReceived(float bGainStepValue);

    void currentRrValuesReceived(float current, float min, float max, float stepSize);
    void currentRgValuesReceived(float current, float min, float max, float stepSize);
    void currentRbValuesReceived(float current, float min, float max, float stepSize);

    void currentGrValuesReceived(float current, float min, float max, float stepSize);
    void currentGgValuesReceived(float current, float min, float max, float stepSize);
    void currentGbValuesReceived(float current, float min, float max, float stepSize);

    void currentBrValuesReceived(float current, float min, float max, float stepSize);
    void currentBgValuesReceived(float current, float min, float max, float stepSize);
    void currentBbValuesReceived(float current, float min, float max, float stepSize);

    void currentBlackLevelReceived(uint currentBlackLevel);
    void minBlackLevelReceived(uint minBlackLevel);
    void maxBlackLevelReceived(uint maxBlackLevel);
    void blackLevelStepValueReceived(uint blackLevelStepValue);

    void currentBrightnessReceived(float currentBrightness);
    void minBrightnessReceived(float minBrightness);
    void maxBrightnessReceived(float maxBrightness);
    void brightnessStepValueReceived(float stepValue);

    void currentContrastReceived(float currentContrast);
    void minContrastReceived(float minContrast);
    void maxContrastReceived(float maxContrast);
    void contrastStepValueReceived(float stepValue);

    void currentSaturationReceived(float currentSaturation);
    void minSaturationReceived(float minSaturation);
    void maxSaturationReceived(float maxSaturation);
    void saturationStepValueReceived(float stepValue);

    void colorTemperatureReceived(uint colorTemp);

    void currentGammaCorrectionReceived(float currentGamma);
    void minGammaCorrectionReceived(float minGamma);
    void maxGammaCorrectionReceived(float maxGamma);
    void gammaCorrectionStepValueReceived(float stepValue);

    void exposureValueReceived(uint exposure);

    void cameraModeReceived(uint cameraMode);

    void strobeModeReceived(uint strobeMode);

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

    bool saveConfiguration();

    bool setToDefault();
};

#endif // SEE3CAM_CU200_H
