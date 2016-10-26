#ifndef SEE3CAM_130_H
#define SEE3CAM_130_H
#include <QObject>
#include "uvccamera.h"
#define DENOISE_MIN 0
#define DENOISE_MAX 15

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

signals:
     void sendSceneMode(uint sceneMode);
     void sendEffectMode(uint effectMode);
     void sendAfMode(uint afMode);
     void sendHDRMode(uint hdrMode, uint hdrValue);
     void sendDenoiseValue(uint denoiseValue);
     void sendBurstLength(uint burstLength);
     void sendqFactor(uint qFactor);
     void sendROIAfMode(uint roiMode, uint winSize);
     void sendROIAutoExpMode(uint roiMode, uint winSize);
     void sendAfRectMode(uint afRectMode);
     void sendFlipMode(uint flipMode, uint flipEnableDisableMode);

public slots:
    bool getSceneMode();
    bool getEffectMode();
    bool setSceneMode(const sceneModes& sceneMode);
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
};
#endif // SEE3CAM_130_H
