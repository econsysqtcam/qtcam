#ifndef SEE3CAM_130D_H
#define SEE3CAM_130D_H

#include <QObject>
#include "uvccamera.h"

#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000

#define CAMERA_CONTROL_130D                 0xA9

#define GET_SCENEMODE_130D                  0x01
#define SET_SCENEMODE_130D                  0x02

#define GET_SPECIALEFFECT_130D              0x03
#define SET_SPECIALEFFECT_130D              0x04

#define GET_DENOISE_130D					0x05
#define SET_DENOISE_130D					0x06

#define GET_AF_MODE_130D					0x07
#define SET_AF_MODE_130D					0x08

#define GET_AF_ROI_MODE_130D				0x09
#define SET_AF_ROI_MODE_130D				0x0A

#define GET_EXP_ROI_MODE_130D               0x0B
#define SET_EXP_ROI_MODE_130D               0x0C

#define GET_BURST_LENGTH_130D               0x0D
#define SET_BURST_LENGTH_130D               0x0E

#define GET_HDR_MODE_130D                   0x0F
#define SET_HDR_MODE_130D                   0x10

#define GET_Q_FACTOR_130D                   0x11
#define SET_Q_FACTOR_130D                   0x12

#define ENABLE_DISABLE_MODE_AF_RECTANGLE_130D     0x13
#define ENABLE_AF_RECTANGLE_130D            0x01
#define DISABLE_AF_RECTANGLE_130D           0x00

#define GET_STREAM_MODE_130D                 0x14
#define SET_STREAM_MODE_130D                 0x15

#define GET_AF_RECT_MODE_130D                0x16

#define SET_TO_DEFAULT_130D                  0x17

#define GET_FLIP_MODE_130D                   0x18
#define SET_FLIP_MODE_130D                   0x19

#define SET_FLIP_BOTHFLIP_ENABLE_130D    0x03
#define SET_FLIP_HORZFLIP_130D           0x01
#define SET_FLIP_VERTFLIP_130D           0x02
#define SET_FLIP_BOTHFLIP_DISABLE_130D   0x00

#define GET_FACE_DETECT_RECT                 0x1E
#define SET_FACE_DETECT_RECT                 0x1F

#define ENABLE_FACE_RECT                0x01
#define DISABLE_FACE_RECT               0x00
#define ENABLE_EMBED_DATA               0x01
#define DISABLE_EMBED_DATA              0x00
#define ENABLE_OVERLAY_RECT             0x01
#define DISABLE_OVERLAY_RECT            0x00

#define GET_EXPOSURE_COMPENSATION            0x22
#define SET_EXPOSURE_COMPENSATION            0x23

#define GET_FRAME_RATE_130D                  0x24
#define SET_FRAME_RATE_130D                  0x25

#define GET_ANTIFLICKER_130D                0x26
#define SET_ANTIFLICKER_130D                0x27

#define SAVE_CONFIGURATION_130D       0x42
#define SAVE_130D                     0x01

#define GET_64BIT_UNIQUE_ID_1      0x41
#define GET_64BIT_UNIQUE_ID_2      0x01

#define GET_AWB_130D                     0x1A

#define GET_AUTO_EXPOSURE               0x1B

#define ENABLE_DISABLE_AF_RECT               0x30

#define GET_FLASH_MODE_130              0x1C
#define SET_FLASH_MODE_130              0x1D

#define GET_AF_POSITION                 0x28

#define GET_RED_GAIN                    0x31
#define SET_RED_GAIN                    0x32

#define GET_BLUE_GAIN                   0x2B
#define SET_BLUE_GAIN                   0x2C

#define GET_GREEN_GAIN                  0x2D
#define SET_GREEN_GAIN                  0x2E

class See3CAM_130D : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit See3CAM_130D(QObject *parent = 0);
    QString _title;
    QString _text;

    enum sceneModes
    {
        SCENE_NORMAL = 0x01,
        SCENE_DOCUMENT = 0x0C
    };
    Q_ENUMS(sceneModes)

    enum specialEffects
    {
        EFFECT_NORMAL = 0x01,
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE = 0x07,
        EFFECT_NEGATIVE = 0x08,
        EFFECT_SKETCH = 0x10
    };
    Q_ENUMS(specialEffects)

    enum camAfMode
    {
        Continuous = 0x01,
        OneShot = 0x02,
        AfModeDisabled = 0x03
    };
    Q_ENUMS(camAfMode)

    enum FLASH_MODE {
            DISABLE  = 0x00,
            STROBE   = 0x01,
            TORCH    = 0x02
     };
     Q_ENUMS(FLASH_MODE)

    enum camROIAfMode
    {
        AFCentered = 0x01,
        AFManual = 0x02,
        AFDisabled = 0x03
    };
    Q_ENUMS(camROIAfMode)

    enum camROIAutoExpMode
    {
        AutoExpFace = 0x00,
        AutoExpFull = 0x01,
        AutoExpManual = 0x02,
        AutoExpDisabled = 0x03
    };
    Q_ENUMS(camROIAutoExpMode)

    enum camiHDRMode
    {
        HdrOff = 0x01,
        HdrAuto = 0x02,
        HdrManual = 0x03
    };
    Q_ENUMS(camiHDRMode)

    enum FLICKER_MODE {
         MODE_AUTO    = 0x00,
         MODE_50Hz    = 0x01,
         MODE_60Hz    = 0x02,
         MODE_DISABLE = 0x03
    };
    Q_ENUMS(FLICKER_MODE)

    enum camStreamMode
    {
        STREAM_MASTER = 0x00,
        STREAM_TRIGGER = 0x01
    };
    Q_ENUMS(camStreamMode)

    enum camFaceRectMode {
        FaceRectEnable = 0x01,
        FaceRectDisable = 0x00
    };
    Q_ENUMS(camFaceRectMode)

    enum camFaceDetectEmbedDataMode {
        FaceDetectEmbedDataEnable = 0x01,
        FaceDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camFaceDetectEmbedDataMode)

    enum camFaceDetectOverlayRect {
        FaceDetectOverlayRectEnable = 0x01,
        FaceDetectOverlayRectDisable = 0x00
    };
    Q_ENUMS(camFaceDetectOverlayRect)

    enum camAFRectMode {
        AFRectEnable = 0x01,
        AFRectDisable = 0x00
    };
    Q_ENUMS(camAFRectMode)

    enum camFlipMode {
        FLIP_BOTHFLIP_ENABLE =0x03,
        FLIP_HORZFLIP = 0x01,
        FLIP_VERTFLIP =0x02 ,
        FLIP_BOTHFLIP_DISABLE =0x00
    };
    Q_ENUMS(camFlipMode)

signals:
    void sceneModeValue(uint sceneMode);
    void effectModeValue(uint effectMode);
    void denoiseValueReceived(uint denoiseValue);
    void hDRModeValueReceived(uint hdrMode, uint hdrValue);
    void burstLengthValue(uint burstLength);
    void qFactorValue(uint qFactor);
    void afModeValue(uint afMode);
    void roiAfModeValue(uint roiMode, uint winSize);
    void roiAutoExpModeValue(uint roiMode, uint winSize);
    void autoExposureReceived(uint autoExposure);
    void afRectModeValue(uint afRectMode);
    void streamModeValue(uint streamMode);
    void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
    void exposureCompValueReceived(uint exposureCompensation);
    void frameRateCtrlValueReceived(uint frameRateCtrlValue);
    void indicateCommandStatus(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);
    void flipMirrorModeChanged(uint flipMirrorMode);
    void indicateExposureValueRangeFailure(QString title, QString text);
    void antiFlickerModeChanged(uint flickerMode);
    void autoWhiteBalanceReceived(uint autoWhiteBalance);
    void flashModeReceived(uint flashMode);
    void autoFocusPositionReceived(uint afPosition);

    void redGainMinReceived(uint minRed);
    void redGainMaxReceived(uint maxRed);
    void redGainCurrentReceived(uint currentRed);

    void blueGainMinReceived(uint minBlue);
    void blueGainMaxReceived(uint maxBlue);
    void blueGainCurrentReceived(uint currentBlue);

    void greenGainMinReceived(uint minGreen);
    void greenGainMaxReceived(uint maxGreen);
    void greenGainCurrentReceived(uint currentGreen);
    public slots:
    bool getSceneMode();
    bool setSceneMode(const sceneModes& sceneMode);

    bool getEffectMode();
    bool setEffectMode(const specialEffects& specialEffect);

    bool getDenoiseValue();
    bool setDenoiseValue(int deNoiseVal);

    bool getAutoFocusMode();
    bool setAutoFocusMode(camAfMode afMode);

    bool getAutoFocusROIModeAndWindowSize();
    bool setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool getAutoExpROIModeAndWindowSize();
    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool getBurstLength();
    bool setBurstLength(uint burstLength);

    bool getiHDRMode();
    bool setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue);

    bool getQFactor();
    bool setQFactor(uint qFactor);

    bool getAFRectMode();
    bool enableDisableAFRectangle(bool enableRFRect);

    bool getStreamMode();
    bool setStreamMode(camStreamMode streamMode);

    bool getAutoWhiteBalance();

    bool getAutoExposure();

    bool getFlashMode();
    bool setFlashMode(FLASH_MODE mode);

    bool getAutoFocusPosition();

    bool setToDefault();

    bool getFlipMode();
    bool setFlipMode(bool horzModeSel, bool vertiModeSel);

    bool getFaceDetectMode();
    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);

    bool getRedGain();
    bool setRedGain(uint redGain);

    bool getBlueGain();
    bool setBlueGain(uint blueGain);

    bool getGreenGain();
    bool setGreenGain(uint greenGain);

    bool getExposureCompensation();
    bool setExposureCompensation(unsigned int exposureCompValue);

    bool getFrameRateCtrlValue();
    bool setFrameRateCtrlValue(uint frameRate);

    bool enableDisableFaceRectangle(bool enableFaceRect);

    bool getAntiFlickerMode();
    bool setAntiFlickerMode(FLICKER_MODE flickerMode);

    bool saveConfiguration();
    bool get64BitSerialNumber();

    bool enable_disablerect(bool value);
};
#endif // SEE3CAM_130D_H

