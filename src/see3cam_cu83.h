#ifndef SEE3CAM_CU83_H
#define SEE3CAM_CU83_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_SEE3CAM_CU83             0xBE

#define GET_SPECIAL_EFFECT_SEE3CAM_CU83         0x03
#define SET_SPECIAL_EFFECT_SEE3CAM_CU83         0x04

#define GET_DENOISE_CONTROL_SEE3CAM_CU83        0x05
#define SET_DENOISE_CONTROL_SEE3CAM_CU83        0x06

#define GET_AUTOEXP_ROI_MODE_SEE3CAM_CU83       0x07
#define SET_AUTOEXP_ROI_MODE_SEE3CAM_CU83       0x08

#define GET_ORIENTATION_SEE3CAM_CU83            0x0B
#define SET_ORIENTATION_SEE3CAM_CU83            0x0C

#define GET_EXPOSURE_COMPENSATION_SEE3CAM_CU83  0x11
#define SET_EXPOSURE_COMPENSATION_SEE3CAM_CU83  0x12

#define GET_FRAMERATE_CONTROL_SEE3CAM_CU83      0x13
#define SET_FRAMERATE_CONTROL_SEE3CAM_CU83      0x14

#define GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU83 0x17
#define SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU83 0x18

#define GET_BURST_LENGTH_SEE3CAM_CU83           0x19
#define SET_BURST_LENGTH_SEE3CAM_CU83           0x1A

#define GET_WAKEON_MOTION_SEE3CAM_CU83          0x1F
#define SET_WAKEON_MOTION_SEE3CAM_CU83          0x20

#define GET_FACE_DETECTION    0x22
#define SET_FACE_DETECTION    0x23

#define GET_STREAM_MODE       0x24
#define SET_STREAM_MODE       0x25

#define GET_FLASH_CONTROL     0x26
#define SET_FLASH_CONTROL     0x27

#define SET_DEFAULT_SEE3CAM_CU83        0xff

//Face Detection
#define ENABLE_FACE_RECT                0x01
#define DISABLE_FACE_RECT               0x00

#define ENABLE_EMBED_DATA               0x01
#define DISABLE_EMBED_DATA              0x00

#define ENABLE_OVERLAY_RECT             0x01
#define DISABLE_OVERLAY_RECT            0x00

#define SET_FAIL		0x00
#define SET_SUCCESS		0x01

#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define EXPOSURECOMP_MIN 50
#define EXPOSURECOMP_MAX 1000000

class SEE3CAM_CU83 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_CU83();

    enum specialModes{
        EFFECT_NORMAL      = 0x01,
        EFFECT_BLACK_WHITE = 0x04,
        EFFECT_GREYSCALE   = 0x07,
        EFFECT_NEGATIVE    = 0x08,
        EFFECT_SKETCH      = 0x10
    };
    Q_ENUMS(specialModes)

    enum autoExpRoiModes{
        FULL_ROI    = 0x01,
        MANUAL_ROI  = 0x02,
        AE_DISABLED = 0x03
    };
    Q_ENUMS(autoExpRoiModes)

    enum getOrientation{
        HORIZONTAL = 0x01,
        VERTICAL   = 0x02,
        BOTH       = 0x03
    };
    Q_ENUMS(getOrientation)

    enum antiFlickerDetection{
        MODE_AUTO    = 0x00,
        MODE_50Hz    = 0x01,
        MODE_60Hz    = 0x02,
        MODE_DISABLE = 0x03
    };
    Q_ENUMS(antiFlickerDetection)

    enum wakeOnMode{
        ENABLE    = 0x01,
        DISABLE   = 0x00
    };
    Q_ENUMS(wakeOnMode)

    enum STREAM_MODE{
        MASTER_MODE  = 0x00,
        TRIGGER_MODE = 0x01
    };Q_ENUMS(STREAM_MODE)

    enum FLASH_MODE{
        STROBE_ON  = 0x01,
        STROBE_OFF = 0x00
    };Q_ENUMS(FLASH_MODE)

    enum camFaceRectMode {
        FaceRectDisable = 0x00,
        FaceRectEnable  = 0x01
    };
    Q_ENUMS(camFaceRectMode)

    enum camFaceDetectEmbedDataMode {
        FaceDetectEmbedDataEnable  = 0x01,
        FaceDetectEmbedDataDisable = 0x00
    };
    Q_ENUMS(camFaceDetectEmbedDataMode)

    enum camFaceDetectOverlayRect {
        FaceDetectOverlayRectEnable = 0x01,
        FaceDetectOverlayRectDisable = 0x00
    };
    Q_ENUMS(camFaceDetectOverlayRect)

signals:
    void specialModeReceived(uint specialMode);
    void denoiseValueReceived(uint denoiseValue);
    void roiAutoExpModeRecieved(uint roiMode, uint winSize);
    void flipMirrorModeReceived(uint flipMode);
    void exposureCompValueRecieved(uint exposureCompVal);
    void frameRateCtrlValueRecieved(uint frameRateVal);
    void minimumFramesReceived(uint minimumFps);
    void maximumFramesReceived(uint maximumFps);
    void burstLengthValueRecieved(uint burstLenVal);
    void antiFlickerModeRecieved(uint antiFlickerVal);
    void wakeonModeReceived(uint wakeOn);

    void faceDetectModeValueReceived(uint faceDetectMode, uint faceDetectOverlayRect, uint faceDetectEmbedDataValue);
    void streamModeReceived(uint streamMode);
    void flashModeReceived(uint flashMode);

    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateCommandStatus(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);

public slots:
    bool setSpecialMode(specialModes specialMode);
    bool getSpecialMode();

    bool setDenoiseValue(uint deNoiseVal);
    bool getDenoiseValue();

    bool setROIAutoExposure(autoExpRoiModes autoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setAntiFlickerMode(antiFlickerDetection antiFlickerMode);
    bool getAntiFlickerMode();

    bool setWakeOnMotion(wakeOnMode wakeOn);
    bool getWakeOnMotion();

    bool setStreamMode(STREAM_MODE mode);
    bool getStreamMode();

    bool setFlashMode(FLASH_MODE mode);
    bool getFlashMode();

    bool getFaceDetectMode();
    bool setFaceDetection(bool enableFaceDetectRect, bool embedData, bool overlayRect);

    bool setToDefaultValues();
};
#endif // SEE3CAM_CU83_H
