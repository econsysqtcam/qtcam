#ifndef SEE3CAM_CU22_H
#define SEE3CAM_CU22_H

#include <QObject>
#include "uvccamera.h"

// See3cam_cu22 camera
#define CAMERA_CONTROL_CU22         0xA3
#define GET_SENSOR_MODE_CU22        0x01
#define SET_SENSOR_MODE_CU22        0x02
#define GET_CAMERA_MODE_CU22        0x03
#define SET_CAMERA_MODE_CU22        0x04
#define SET_SPECIAL_MODE_CU22       0x08
#define GET_SPECIAL_MODE_CU22       0x07
#define GET_ORIENTATION_CU22        0x09
#define SET_ORIENTATION_CU22        0x0A
#define SET_STROBEMODE_CU22         0x0C
#define GET_STROBEMODE_CU22         0x0B
#define SET_AE_ROI_MODE_CU22        0x06
#define GET_AE_ROI_MODE_CU22        0x05
#define SET_COLORKILLVAL_CU22       0x0E
#define GET_COLORKILLVAL_CU22       0x0D
#define SET_BURSTLENGTH_CU22        0x10
#define GET_BURSTLENGTH_CU22        0x0F
#define SET_ANTIFLICKER_CU22        0x12
#define GET_ANTIFLICKER_CU22        0x11
#define SET_DENOISE_MODE_CU22       0x16
#define GET_DENOISE_MODE_CU22       0x15
#define SET_LSCMODE_CU22            0x14
#define GET_LSCMODE_CU22            0x13
#define SET_TO_DEFAULT_CU22         0xFF


class See3CAM_CU22 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    // Initialize input and output buffers
    void initializeBuffers();

public:

    enum sensorModes {
        SENSOR_STANDARD = 0x01,
        SENSOR_HDRLFM  = 0x02,
        SENSOR_HiHDR = 0x03
    };
    Q_ENUMS(sensorModes)

    enum cameraModes {
        CAMERA_MASTER = 0x01,
        CAMERA_TRIGGER  = 0x02
    };
    Q_ENUMS(cameraModes)

    enum specialModes {
        SPECIAL_NORMAL = 0x01,
        SPECIAL_GREYSCALE  = 0x02
    };
    Q_ENUMS(specialModes)

    enum flipMirrorControls{
        SetBothFlipDisable = 0x01,
        SetHorzFlip = 0x03,
        SetVertiFlip = 0x02,
        SetBothFlipEnable = 0x04
    };
    Q_ENUMS(flipMirrorControls)

    enum camROIAutoExpMode {
        AutoExpCentered = 0x01,
        AutoExpManual = 0x02,
        AutoExpDisabled = 0x03
    };
    Q_ENUMS(camROIAutoExpMode)

    enum camAntiFlickerMode {
        AntiFlickerAuto = 0x01,
        AntiFlicker50Hz = 0x02,
        AntiFlicker60Hz = 0x03
    };
    Q_ENUMS(camAntiFlickerMode)

    enum denoiseModes {
        DenoiseEnable = 0x01,
        DenoiseDisable = 0x02
    };
    Q_ENUMS(denoiseModes)

    enum lscModes {
        LSC_WARMWHITE = 0x01,
        LSC_BRIGHTWHITE = 0x02,
        LSC_DAYLIGHT = 0x03
    };
    Q_ENUMS(lscModes)

public slots:
    bool setSensorMode(sensorModes  sensorMode);
    bool getSensorMode();

    bool setCameraMode(cameraModes  cameraMode);
    bool getCameraMode();

    bool setSpecialMode(specialModes  specialMode);
    bool getSpecialMode();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setROIAutoExposure(uint see3camAeROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setColourKill(uint colourKillValue);
    bool getColourKill();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setAntiFlickerMode(camAntiFlickerMode antiFlickerMode);
    bool getAntiFlickerMode();

    bool setDenoiseCtrlMode(denoiseModes  deNoiseMode);
    bool getDenoiseCtrlMode();

    bool setLSCMode(uint lscMode);
    bool getLSCMode();

    bool setToDefaultValues();

signals:
    void sensorModeReceived(uint sensorMode);
    void cameraModeReceived(uint cameraMode);
    void specialModeReceived(uint specialMode);
    void flipMirrorModeChanged(uint flipMirrorMode);
    void strobeModeChanged(uint strobeMode);
    void roiAutoExpModeChanged(uint roiMode, uint winSize);
    void colorKillValueChanged(uint colorKillValue);
    void burstLengthChanged(uint burstLength);
    void antiFlickerModeChanged(uint flickerMode);
    void denoiseModeChanged(uint denoiseMode);
    void lscModeChanged(uint lscMode);
    void setdefaultValueFailed();
    void exposureCompValue(uint exposureCompensation);
    void disableHDR(bool hdrstatus);
};
#endif // SEE3CAM_CU22

