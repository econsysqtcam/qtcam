#ifndef NILECAM30_USB_H
#define NILECAM30_USB_H
#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_NILECAM30_USB     0x8E

#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000

#define SMILE_THRESHOLD_MIN 40
#define SMILE_THRESHOLD_MAX 75
#define DEFAULT_SMILE_THRESHOLD 40

#define GET_SPECIAL_EFFECT 0x03
#define SET_SPECIAL_EFFECT 0x04

#define SET_ORIENTATION_NILECAM30_USB    0x0E
#define GET_ORIENTATION_NILECAM30_USB    0x0D
#define SET_ORIENTATION_BOTHFLIP_ENABLE_NILECAM30_USB    0x03
#define SET_ORIENTATION_HORZFLIP_NILECAM30_USB           0x01
#define SET_ORIENTATION_VERTFLIP_NILECAM30_USB           0x02
#define SET_ORIENTATION_BOTHFLIP_DISABLE_NILECAM30_USB   0x00

#define SPECIAL_EFFECT_NORMAL       0x01
#define SPECIAL_EFFECT_BW           0x04
#define SPECIAL_EFFECT_GRAYSCALE    0x07
#define SPECIAL_EFFECT_NEGATIVE     0x08
#define SPECIAL_EFFECT_SKETCH       0x10

#define SET_BURST_LENGTH_NILECAM30_USB       0x0A
#define GET_BURST_LENGTH_NILECAM30_USB       0x09

#define GET_SCENEMODE_NILECAM30_USB          0x01
#define SET_SCENEMODE_NILECAM30_USB          0x02

#define GET_Q_FACTOR_NILECAM30_USB           0x0B
#define SET_Q_FACTOR_NILECAM30_USB           0x0C

#define SET_FRAME_RATE_NILECAM30_USB         0x16
#define GET_FRAME_RATE_NILECAM30_USB         0x15

#define SET_EXPOSURE_COMPENSATION_NILECAM30_USB  0x14
#define GET_EXPOSURE_COMPENSATION_NILECAM30_USB  0x13

#define SET_DENOISE_CONTROL  0x06
#define GET_DENOISE_CONTROL  0x05

#define SET_EXP_ROI_MODE_NILECAM30_USB       0x08
#define GET_EXP_ROI_MODE_NILECAM30_USB       0x07

#define SET_FACE_DETECT_RECT_NILECAM30_USB            0x10
#define GET_FACE_DETECT_RECT_NILECAM30_USB            0x0F
#define ENABLE_FACE_RECT_NILECAM30_USB                0x01
#define DISABLE_FACE_RECT_NILECAM30_USB               0x00
#define ENABLE_EMBED_DATA_NILECAM30_USB               0x01
#define DISABLE_EMBED_DATA_NILECAM30_USB              0x00
#define ENABLE_OVERLAY_RECT_NILECAM30_USB             0x01
#define DISABLE_OVERLAY_RECT_NILECAM30_USB            0x00

#define SET_SMILE_DETECTION_NILECAM30_USB             0x12
#define GET_SMILE_DETECTION_NILECAM30_USB             0x11
#define ENABLE_SMILE_DETECT_NILECAM30_USB             0x01
#define DISABLE_SMILE_DETECT_NILECAM30_USB            0x00
#define ENABLE_SMILE_TRIGGER_NILECAM30_USB	     0x01
#define DISABLE_SMILE_TRIGGER_NILECAM30_USB	     0x00
#define SET_TO_DEFAULT_NILECAM30_USB                  0xFF

#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_NILECAM30_USB 0x17
#define ENABLE_FACE_RECTANGLE_NILECAM30_USB              0x01
#define DISABLE_FACE_RECTANGLE_NILECAM30_USB             0x00

#define SETFLASH_STATUS_NILECAM30_USB 0x19
#define GETFLASH_STATUS_NILECAM30_USB 0x18

#define CAMERA_MCUFWVERSION_NILECAM30_USB               0x43

#define STILL_YUYV_FORMAT                             0x00
#define STILL_MJPG_FORMAT                             0x01

class NILECAM30_USB : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit NILECAM30_USB(QObject *parent = 0);

    enum specialEffects {
        EFFECT_NORMAL = 0x01,
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE = 0x07,
        EFFECT_SKETCH = 0x10,
        EFFECT_NEGATIVE = 0x08
    };

    Q_ENUMS(specialEffects)

    enum sceneModes {
        SCENE_NORMAL = 0x01,
        SCENE_DOCUMENT = 0x0C
    };
    Q_ENUMS(sceneModes)

    enum camROIAutoExpMode {
        AutoExpFace = 0x00,
        AutoExpFull = 0x01,
        AutoExpManual = 0x02,
        AutoExpDisabled = 0x03
    };
    Q_ENUMS(camROIAutoExpMode)

    enum flipMirrorControls{
        FLIP_OFF_MIRROR_OFF = 0x00,
        FLIP_ON_MIRROR_OFF  = 0x01,
        FLIP_OFF_MIRROR_ON  = 0x02,
        FLIP_ON_MIRROR_ON   = 0x03
    };
    Q_ENUMS(flipMirrorControls)

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
    void sendEffectMode(uint effectMode);
    void sendDenoiseValue(uint denoiseValue);
    void sceneModeValue(uint sceneMode);
    void roiAutoExpModeValue(uint roiMode, uint winSize);
    void burstLengthValue(uint burstLength);
    void qFactorValue(uint qFactor);
    void flipMirrorModeChanged(uint flipMirrorMode);
    void frameRateCtrlValueReceived(uint frameRateCtrlValue);
    void indicateCommandStatus(QString title, QString text);
    void indicateExposureValueRangeFailure(QString title, QString text);
    void exposureCompValueReceived(uint exposureCompensation);
    void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
    void smileDetectModeValue(uint smileDetectMode, uint smileDetectEmbedDataValue);
    void flashModeValue(uint flashMode);
    void mcuFirmwareVersionReceived(QString mcuFwVersion);
    void updateFrameToSkipfromCam(uint stillSkip);

public slots:
    bool setSpecialEffectsNileCam30USB(const specialEffects &specialEffect);
    bool getSpecialEffectsNileCam30USB();

    bool setDenoiseValueNileCam30USB(int deNoiseVal);
    bool getDenoiseValueNileCam30USB();

    bool setSceneModeNileCam30USB(const sceneModes &sceneMode);
    bool getSceneModeNileCam30USB();

    bool setExpRoiModeNileCam30USB(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getExpRoiModeNileCam30USB();

    bool setBurstLengthNileCam30USB(uint burstLength);
    bool getBurstLengthNileCam30USB();

    bool setQFactorNileCam30USB(uint qFactor);
    bool getQFactorNileCam30USB();

    bool setFlipModeNileCam30USB(bool horzModeSel, bool vertiModeSel);
    bool getFlipModeNileCam30USB();

    bool setFrameRateValueNileCam30USB(uint frameRate);
    bool getFrameRateValueNileCam30USB();

    bool setExposureCompensationNileCam30USB(unsigned int exposureCompValue);
    bool getExposureCompensationNileCam30USB();

    bool setFaceDetectionRectNileCam30USB(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectionRectNileCam30USB();

    bool setSmileDetectionStateNileCam30USB(bool enableSmileDetect, bool embedData);
    bool getSmileDetectionStateNileCam30USB();

    bool enableDisableFaceRectangle(bool enableFaceRect);

    bool setFlashNileCam30USB(flashStateValues flashMode);
    bool getFlashNileCam30USB();

    bool restoreDefaultNileCam30USB();
    bool readMCUFirmwareVersion();
    void setStillSkipCount(QString stillOutFomat);

};
#endif // NILECAM30_USB_H
