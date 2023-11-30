#ifndef SEE3CAM50CUG_H
#define SEE3CAM50CUG_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID_1_SEE3CAM_50CUG      0xC0
#define CAMERA_CONTROL_ID_2_SEE3CAM_50CUG      0x05

#define GET_GAIN_MODE_SEE3CAM_50CUG     0x01
#define SET_GAIN_MODE_SEE3CAM_50CUG     0x02

#define GET_RGB_GAIN_SEE3CAM_50CUG      0x03
#define SET_RGB_GAIN_SEE3CAM_50CUG      0x04

#define GET_CC_MATRIX_SEE3CAM_50CUG     0x05
#define SET_CC_MATRIX_SEE3CAM_50CUG     0x06

#define GET_BLACK_LEVEL_SEE3CAM_50CUG   0x07
#define SET_BLACK_LEVEL_SEE3CAM_50CUG   0x08

#define GET_BRIGHTNESS_SEE3CAM_50CUG    0x09
#define SET_BRIGHTNESS_SEE3CAM_50CUG    0x0A

#define GET_CONTRAST_SEE3CAM_50CUG      0x0B
#define SET_CONTRAST_SEE3CAM_50CUG      0x0C

#define GET_SATURATION_SEE3CAM_50CUG    0x0D
#define SET_SATURATION_SEE3CAM_50CUG    0x0E

#define GET_GAMMA_SEE3CAM_50CUG         0x11
#define SET_GAMMA_SEE3CAM_50CUG         0x12

#define GET_COLOR_TEMPERATURE_SEE3CAM_50CUG     0x0F
#define SET_COLOR_TEMPERATURE_SEE3CAM_50CUG     0x10

#define GET_EXPOSURE_SEE3CAM_50CUG      0x13
#define SET_EXPOSURE_SEE3CAM_50CUG      0x14

#define GET_CAMERA_MODE_SEE3CAM_50CUG   0x15
#define SET_CAMERA_MODE_SEE3CAM_50CUG   0x16

#define GET_STROBE_MODE_SEE3CAM_50CUG   0x17
#define SET_STROBE_MODE_SEE3CAM_50CUG   0x18

#define SAVE_CONFIGURATION_SEE3CAM_50CUG_M                0x42
#define SAVE_SEE3CAM_50CUG_M                              0x01

#define SET_TO_DEFAULT_SEE3CAM_50CUG    0xFF


#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define EXPOSURE_MIN 100
#define EXPOSURE_MAX 10000000

union
{
    float f;
    uint32_t u;
}frgain , fbgain , fBrightness , fSaturation , fgamma , fRr , fRg , fRb , fGr , fGg , fGb , fBr , fBg , fBb;


class SEE3CAM_50CUG : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_50CUG();
    ~SEE3CAM_50CUG();

    enum GAIN_MODE{
        MANUAL_MODE = 0x00,
        AUTO_MODE   = 0x01
    };
    Q_ENUMS(GAIN_MODE)

    enum CAMERA_MODE {
        MASTER              = 0x01,
        TRIGGER_EXP_CONTROL = 0x02,
        TRIGGER_ACQ_CONTROL = 0x03
    };
    Q_ENUMS(CAMERA_MODE)

    enum STROBE_MODE{
        STROBE_ON    = 0x00,
        STROBE_OFF   = 0x02
    };
    Q_ENUMS(STROBE_MODE)

    enum EXPOSURE_MODE{
        MANUAL_EXPOSURE   = 0x00,
        AUTO_EXPOSURE     = 0x01
    };
    Q_ENUMS(EXPOSURE_MODE)

signals:
    void gainValueReceived(uint gainValue);

    void gainRReceived(float RGain);
    void gainBReceived(float BGain);

    void valueRReceived(float Rr, float Rg, float Rb);
    void valueGReceived(float Gr, float Gg, float Gb);
    void valueBReceived(float Br, float Bg, float Bb);

    void brightnessReceived(float brightness);
    void blackLevelReceived(uint blackLevel);
    void contrastReceived(uint contrast);
    void saturationReceived(float saturation);
    void colorTemperatureReceived(uint colorTemp);
    void gammaCorrectionReceived(float gammaCorrection);
    void cameraModeReceived(uint cameraMode);
    void strobeModeReceived(uint strobeMode);
    void exposureValueReceived(uint exposure);

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
    bool setExposure(EXPOSURE_MODE expMode, uint exposure);

    bool getCameraMode();
    bool setCameraMode(CAMERA_MODE camMode);

    bool getStrobeMode();
    bool setStrobeMode(STROBE_MODE strobeMode);

    bool saveConfiguration();

    bool setToDefault();
};

#endif // SEE3CAM50CUG_H
