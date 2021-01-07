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

#define GET_SMILE_DETECTION                  0x20
#define SET_SMILE_DETECTION                  0x21
#define ENABLE_SMILE_DETECT             0x01
#define DISABLE_SMILE_DETECT            0x00

#define GET_EXPOSURE_COMPENSATION            0x22
#define SET_EXPOSURE_COMPENSATION            0x23

#define GET_FRAME_RATE_130D                  0x24
#define SET_FRAME_RATE_130D                  0x25

#define GET_ANTIFLICKER_130D                0x26
#define SET_ANTIFLICKER_130D                0x27

#define ENABLE_DISABLE_AF_RECT               0x30

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

    enum camStreamMode
    {
        STREAM_MASTER = 0x00,
        STREAM_TRIGGER = 0x01
    };
    Q_ENUMS(camStreamMode)

    enum camAntiFlickerMode {
        AntiFlickerAuto = 0x00,
        AntiFlicker50Hz = 0x01,
        AntiFlicker60Hz = 0x02
    };
    Q_ENUMS(camAntiFlickerMode)


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

    enum camSmileDetectMode {
        SmileDetectEnable = 0x01,
        SmileDetectDisable = 0x00
    };
    Q_ENUMS(camSmileDetectMode)

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
    void afRectModeValue(uint afRectMode);
    void streamModeValue(uint streamMode);
    void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
    void smileDetectModeValue(uint smileDetectMode, uint smileDetectEmbedDataValue);
    void exposureCompValueReceived(uint exposureCompensation);
    void frameRateCtrlValueReceived(uint frameRateCtrlValue);
    void indicateCommandStatus(QString title, QString text);
    void flipMirrorModeChanged(uint flipMirrorMode);
    void indicateExposureValueRangeFailure(QString title, QString text);
    void antiFlickerModeChanged(uint flickerMode);
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

    bool setToDefault();

    bool getFlipMode();
    bool setFlipMode(bool horzModeSel, bool vertiModeSel);

    bool getFaceDetectMode();
    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);

    bool getSmileDetectMode();
    bool setSmileDetection(bool enableSmileDetect, bool embedData);

    bool getExposureCompensation();
    bool setExposureCompensation(unsigned int exposureCompValue);

    bool getFrameRateCtrlValue();
    bool setFrameRateCtrlValue(uint frameRate);

    bool enableDisableFaceRectangle(bool enableFaceRect);

    bool getAntiFlickerMode();
    bool setAntiFlickerMode(camAntiFlickerMode antiFlickerMode);

    bool enable_disablerect(bool value);
};
#endif // SEE3CAM_130D_H

