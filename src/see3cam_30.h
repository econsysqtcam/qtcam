#ifndef SEE3CAM_30_H
#define SEE3CAM_30_H
#include <QObject>
#include "uvccamera.h"
#include <QtConcurrentRun>

#define DENOISE_MIN 0
#define DENOISE_MAX 15
#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000
#define SMILE_THRESHOLD_MIN 40
#define SMILE_THRESHOLD_MAX 75

/* SEE3CAM_30 */
#define CAMERA_CONTROL_30               0x80
#define GET_SCENEMODE_30                0x01
#define SET_SCENEMODE_30                0x02
#define SET_EXPOSURE_COMPENSATION_30    0x1A
#define GET_EXPOSURE_COMPENSATION_30    0x19
#define SET_FACE_DETECT_RECT_30         0x16
#define GET_FACE_DETECT_RECT_30         0x15
#define GET_AF_MODE_30					0x07
#define SET_AF_MODE_30					0x08
#define GET_Q_FACTOR_30                 0x0F
#define SET_Q_FACTOR_30                 0x10
#define SET_ORIENTATION_30              0x12
#define GET_ORIENTATION_30              0x11
#define SET_ORIENTATION_HORZFLIP_30     0x01
#define SET_ORIENTATION_VERTFLIP_30     0x02
#define SET_ORIENTATION_BOTHFLIP_ENABLE_30      0x03
#define SET_ORIENTATION_BOTHFLIP_DISABLE_30     0x00
#define GET_ORIENTATION_30              0x11
#define GET_AF_ROI_MODE_30				0x09
#define SET_AF_ROI_MODE_30				0x0A
#define SET_EXP_ROI_MODE_30             0x0C
#define GET_EXP_ROI_MODE_30             0x0B
#define GET_AF_RECT_MODE_30             0x13
#define SET_AF_RECT_MODE_30             0x14
#define ENABLE_AF_RECTANGLE_30          0x01
#define DISABLE_AF_RECTANGLE_30         0x00
#define GET_BURST_LENGTH_30             0x0D
#define SET_BURST_LENGTH_30             0x0E
#define SET_TO_DEFAULT_30               0xFF
#define ENABLE_FACE_RECT_30                0x01
#define DISABLE_FACE_RECT_30               0x00
#define ENABLE_EMBED_DATA_30               0x01
#define DISABLE_EMBED_DATA_30              0x00
#define ENABLE_OVERLAY_RECT_30             0x01
#define DISABLE_OVERLAY_RECT_30            0x00
#define SET_OVERLAY_RECT_MODE_30           0x1D
#define SET_SMILE_DETECT_30                0x18
#define GET_SMILE_DETECT_30                0x17
#define SET_FRAME_RATE_30                  0x1C
#define GET_FRAME_RATE_30                  0x1B
#define SETFLASH_STATUS_30                 0x1F
#define GETFLASH_STATUS_30                 0x1E
#define SET_DENOISE_CONTROL 		  0x06
#define GET_DENOISE_CONTROL  		0x05
#define GET_SPECIAL_EFFECT 		0x03
#define SET_SPECIAL_EFFECT 		0x04
#define ENABLE_EMBED_DATA               0x01
#define DISABLE_EMBED_DATA              0x00
#define ENABLE_SMILE_DETECT             0x01
#define DISABLE_SMILE_DETECT            0x00
#define DENOISE_MIN 0
#define DENOISE_MAX 15

#define ENABLE_SMILE_TRIGGER_30 0x01
#define DISABLE_SMILE_TRIGGER_30 0x00

class See3CAM_30 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit See3CAM_30(QObject *parent = 0);

    enum sceneModes {
        SCENE_NORMAL = 0x01,
        SCENE_DOCUMENT = 0x0C
    };
    Q_ENUMS(sceneModes)

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
        SmileDetectEmbedDataEnable = 0x01,
        SmileDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camSmileDetectEmbedDataMode)

    enum flashStateValues{
         FLASHMODE_OFF = 0x00,
         FLASHMODE_TORCH = 0x01,
         FLASHMODE_STROBE = 0x02,
     };
     Q_ENUMS(flashStateValues)

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
    void sceneModeChanged(uint sceneMode);
    void exposureCompValueReceived(uint exposureCompensation);
    void indicateCommandStatus(QString title, QString text);
    void indicateSmileThresholdRangeFailure(QString title, QString text);
    void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
    void smileDetectModeValue(uint smileDetectMode, uint smileDetectEmbedDataValue);
    void indicateExposureValueRangeFailure(QString title, QString text);
    void frameRateChanged(uint frameRateCtrlMode);
    void flashModeValue(uint flashMode);


public slots:
    bool setEffectMode(const specialEffects &specialEffect);    
    bool getEffectMode();

    bool setDenoiseValue(int deNoiseVal);
    bool getDenoiseValue();

    bool setAutoFocusMode(camAfMode afMode);
    bool getAutoFocusMode();

    bool setQFactor(uint qFactor);
    bool getQFactor();

    bool setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);

    bool getAutoFocusROIModeAndWindowSize();
    bool getAutoExpROIModeAndWindowSize();

    bool enableDisableAFRectangle(bool enableRFRect);
    bool getAFRectMode();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setSceneMode(sceneModes sceneMode);
    bool getSceneMode();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectMode();

    bool setSmileDetection(bool enableSmileDetect, bool embedData);
    bool getSmileDetectMode();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool setFlashState(flashStateValues flashMode);
    bool getFlashState();

    bool enableDisableOverlayRect(bool enableOverlayRect);

    bool setToDefault();


};
#endif // SEE3CAM_30_H
