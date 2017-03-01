#ifndef SEE3CAM_30_H
#define SEE3CAM_30_H
#include <QObject>
#include "uvccamera.h"
#include <QtConcurrentRun>

#define DENOISE_MIN 0
#define DENOISE_MAX 15
#define SET_COMMAND_TIMEOUT 3000

class See3CAM_30 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

public:
    explicit See3CAM_30(QObject *parent = 0);

    enum specialEffects {
        EFFECT_NORMAL = 0x01,
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE = 0x07,
        EFFECT_SKETCH = 0x10,
        EFFECT_NEGATIVE = 0x08
    };

    Q_ENUMS(specialEffects)

    enum flipMirrorControls{
        FLIP_OFF_MIRROR_OFF = 0x00,
        FLIP_ON_MIRROR_OFF  = 0x01,
        FLIP_OFF_MIRROR_ON  = 0x02,
        FLIP_ON_MIRROR_ON   = 0x03
    };
    Q_ENUMS(flipMirrorControls)

    enum camAfMode {
        Continuous = 0x01,
        OneShot = 0x02,
        AfModeDisabled = 0x03
    };
    Q_ENUMS(camAfMode)

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

    static bool setOrientationBackgrndFn(See3CAM_30 *see3cam30obj, bool horzModeSel, bool vertiModeSel);

signals:
    void effectModeChanged(uint effectMode);
    void denoiseValueChanged(uint denoiseValue);
    void afModeChanged(uint afMode);
    void qFactorChanged(uint qFactor);
    void roiAfModeChanged(uint roiMode, uint winSize);
    void roiAutoExpModeChanged(uint roiMode, uint winSize);
    void afRectModeChanged(uint afRectMode);
    void burstLengthChanged(uint burstLength);
    void flipMirrorModeChanged(uint flipMirrorMode);

public slots:
    bool setEffectMode(const specialEffects &specialEffect);    
    bool getEffectMode();

    bool setDenoiseValue(int deNoiseVal);
    bool getDenoiseValue();

    bool setAutoFocusMode(camAfMode afMode);
    bool getAutoFocusMode();

    bool setQFactor(uint qFactor);
    bool getQFactor();

    bool getOrientation();

    bool setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool getAutoFocusROIModeAndWindowSize();
    bool getAutoExpROIModeAndWindowSize();

    bool enableDisableAFRectangle(bool enableRFRect);
    bool getAFRectMode();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setToDefault();

    void setOrientation(bool horzModeSel, bool vertiModeSel);    
};
#endif // SEE3CAM_30_H
