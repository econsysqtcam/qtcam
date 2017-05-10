#ifndef SEE3CAM_130_H
#define SEE3CAM_130_H
#include <QObject>
#include "uvccamera.h"
#define DENOISE_MIN 0
#define DENOISE_MAX 15
#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000
#define SMILE_THRESHOLD_MIN 40
#define SMILE_THRESHOLD_MAX 75
#define DEFAULT_SMILE_THRESHOLD 40

class See3CAM_130 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

public:
    explicit See3CAM_130(QObject *parent = 0);

    enum sceneModes {
        SCENE_NORMAL = 0x01,
        SCENE_DOCUMENT = 0x0C
    };
    Q_ENUMS(sceneModes)

    enum specialEffects {
        EFFECT_NORMAL = 0x01,        
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE = 0x07,
        EFFECT_NEGATIVE = 0x08,
        EFFECT_SKETCH = 0x10
    };

     Q_ENUMS(specialEffects)

    enum camAfMode {
        Continuous = 0x01,
        OneShot = 0x02,
        AfModeDisabled = 0x03
    };
    Q_ENUMS(camAfMode)

    enum camiHDRMode {
        HdrOff = 0x01,
        HdrAuto = 0x02,
        HdrManual = 0x03
    };
    Q_ENUMS(camiHDRMode)

    enum camROIAfMode {
        AFCentered = 0x01,
        AFManual = 0x02,
        AFDisabled = 0x03
    };
    Q_ENUMS(camROIAfMode)

    enum camROIAutoExpMode {
        AutoExpFull = 0x01,
        AutoExpManual = 0x02,
        AutoExpDisabled = 0x03
    };
    Q_ENUMS(camROIAutoExpMode)

    enum camAFRectMode {
        AFRectEnable = 0x01,
        AFRectDisable = 0x00
    };
    Q_ENUMS(camAFRectMode)

    enum camFlipMode {
        FlipHorizontal = 0x00,
        FlipVertical = 0x01,
        FlipBoth = 0x02
    };
    Q_ENUMS(camFlipMode)

    enum camFlipEnableDisableMode {
        FlipEnable = 0x01,
        FlipDisable = 0x00
    };
    Q_ENUMS(camFlipEnableDisableMode)

    enum camStreamMode {
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

    enum camSmileDetectMode {
        SmileDetectEnable = 0x01,
        SmileDetectDisable = 0x00
    };
    Q_ENUMS(camSmileDetectMode)

    enum camSmileDetectEmbedDataMode {
        smileDetectEmbedDataEnable = 0x01,
        smileDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camSmileDetectEmbedDataMode)


signals:
     void sceneModeValue(uint sceneMode);
     void effectModeValue(uint effectMode);
     void afModeValue(uint afMode);
     void hDRModeValueReceived(uint hdrMode, uint hdrValue);
     void denoiseValueReceived(uint denoiseValue);
     void burstLengthValue(uint burstLength);
     void qFactorValue(uint qFactor);
     void roiAfModeValue(uint roiMode, uint winSize);
     void roiAutoExpModeValue(uint roiMode, uint winSize);
     void afRectModeValue(uint afRectMode);
     void flipModeValue(uint flipMode, uint flipEnableDisableMode);
     void streamModeValue(uint streamMode);
     void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
     void smileDetectModeValue(uint smileDetectMode, uint smileDetectThresholdValue, uint smileDetectEmbedDataValue);
     void exposureCompValueReceived(uint exposureCompensation);
     void frameRateCtrlValueReceived(uint frameRateCtrlValue);
     void indicateCommandStatus(QString title, QString text);
     void indicateSmileThresholdRangeFailure(QString title, QString text);
     void indicateExposureValueRangeFailure(QString title, QString text);

public slots:
    bool getSceneMode();
    bool setSceneMode(const sceneModes& sceneMode);

    bool getEffectMode();
    bool setEffectMode(const specialEffects& specialEffect);

    bool setDenoiseValue(int deNoiseVal);
    bool getDenoiseValue();

    bool setAutoFocusMode(camAfMode afMode);
    bool getAutoFocusMode();

    bool setiHDRMode(camiHDRMode iHDRMode, uint iHDRValue);
    bool getiHDRMode();    

    bool setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool getAutoFocusROIModeAndWindowSize();
    bool getAutoExpROIModeAndWindowSize();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setQFactor(uint qFactor);
    bool getQFactor();

    bool enableDisableAFRectangle(bool enableRFRect);    
    bool getAFRectMode();

    bool setToDefault();

    bool setFlipHorzMode(bool horizModeSel);
    bool setFlipVertiMode(bool vertiModeSel);
    bool getFlipMode();

    bool setStreamMode(camStreamMode streamMode);
    bool getStreamMode();

    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectMode();

    bool setSmileDetection(bool enableSmileDetect, bool embedData, uint thresholdValue);
    bool getSmileDetectMode();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool enableDisableFaceRectangle(bool enableFaceRect);

};
#endif // SEE3CAM_130_H
