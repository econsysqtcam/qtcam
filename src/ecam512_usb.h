#ifndef ECAM_512USB_H
#define ECAM_512USB_H

#include <QObject>
#include <QString>
#include "uvccamera.h"


#define CAMERA_CONTROL_ID1_ECAM512USB                    0xC0
#define CAMERA_CONTROL_ID2_ECAM512USB                    0x03

#define GET_SPECIAL_EFFECT_ECAM512USB                    0x03
#define SET_SPECIAL_EFFECT_ECAM512USB                    0x04

#define GET_DENOISE_ECAM512USB                           0x05
#define SET_DENOISE_ECAM512USB                           0x06

#define GET_AUTO_EXPOSURE_ROI_ECAM512USB                 0x07
#define SET_AUTO_EXPOSURE_ROI_ECAM512USB                 0x08

#define GET_BURST_LENGTH_ECAM512USB                      0x09
#define SET_BURST_LENGTH_ECAM512USB                      0x0A

#define GET_Q_FACTOR_VALUE_ECAM512USB                    0x0B
#define SET_Q_FACTOR_VALUE_ECAM512USB                    0x0C

#define GET_ORIENTATION_ECAM512USB                       0x0D
#define SET_ORIENTATION_ECAM512USB                       0x0E

#define GET_FACE_DETECTION_ECAM512USB                    0x0F
#define SET_FACE_DETECTION_ECAM512USB                    0x10

#define GET_SMILE_DETECTION_ECAM512USB                   0x11
#define SET_SMILE_DETECTION_ECAM512USB                   0x12

#define GET_EXPOSURE_COMPENSATION_ECAM512USB             0x13
#define SET_EXPOSURE_COMPENSATION_ECAM512USB             0x14

#define GET_FRAME_RATE_CONTROL_ECAM512USB                0x15
#define SET_FRAME_RATE_CONTROL_ECAM512USB                0x16

#define GET_FLASH_MODE_ECAM512USB                        0x18
#define SET_FLASH_MODE_ECAM512USB                        0x19

#define GET_ANTI_FLICKER_MODE_ECAM512USB                 0x1A
#define SET_ANTI_FLICKER_MODE_ECAM512USB                 0x1B

#define STORE_SETTINGS_ECAM512USB                        0x34

#define SET_TO_DEFAULT_ECAM512USB                        0xFF

#define ENABLE_FACE_DETECTION      0x01
#define DISBLE_FACE_DETECTION      0x00

#define ENABLE_SMILE_DETECTION     0x01
#define DISBLE_SMILE_DETECTION     0x00

#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000



class ECAM_512USB : public QObject
{
    Q_OBJECT

private:

    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    uvccamera uvc;

    void initializeBuffers();

public:

    ECAM_512USB();

    QString _title;
    QString _text;

    enum SPECIAL_EFFECTS{
        SPECIAL_NORMAL      = 0x01,
        SPECIAL_BLACK_WHITE = 0x04,
        SPECIAL_GREYSCALE   = 0x07,
        SPECIAL_NEGATIVE    = 0x08,
        SPECIAL_SKETCH      = 0x10
    };
    Q_ENUMS(SPECIAL_EFFECTS)

    enum autoExpRoiModes{
        FULL_ROI    = 0x01,
        MANUAL_ROI  = 0x02,
    };
    Q_ENUMS(autoExpRoiModes)

    enum ORIENTATION{
        NORMAL      = 0x00,
        HORIZONTAL  = 0x01,
        VERTICAL    = 0x02,
        ROATATE_180 = 0x03
    };
    Q_ENUMS(ORIENTATION)

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

    enum ANTI_FLICKER_DETECTION{
        MODE_AUTO    = 0x00,
        MODE_50Hz    = 0x01,
        MODE_60Hz    = 0x02,
        MODE_DISABLE = 0x03
    };
    Q_ENUMS(ANTI_FLICKER_DETECTION)

    enum FLASH_MODE{
         FLASH_DISABLE  = 0x00,
         FLASH_STROBE   = 0x01,
         FLASH_TORCH    = 0x02
     };
     Q_ENUMS(FLASH_MODE)

    enum camROIAutoExpMode {
        AutoExpFull   = 0x01,
        AutoExpManual = 0x02,
    };
    Q_ENUMS(camROIAutoExpMode)

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

signals:
    void specialModeReceived(uint specialMode);
    void denoiseValueReceived(uint denoiseValue);
    void roiAutoExpMode(uint roiMode, uint winSize);
    void burstLengthValueRecieved(uint burstLenVal);
    void qFactorValueReceived(uint Qfactor);
    void flipMirrorModeReceived(uint flipMode);
    void faceDetectModeValue(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
    void smileDetectModeValue(uint smileDetectMode, uint smileDetectEmbedDataValue);
    void flashModeReceived(uint flashMode);
    void exposureCompValueRecieved(uint exposureCompVal);
    void frameRateCtrlValueRecieved(uint frameRateVal);
    void antiFlickerModeRecieved(uint antiFlickerVal);

    void indicateExposureValueRangeFailure(QString title, QString text);
    void indicateCommandStatus(QString title, QString text);

public slots:
    bool setSpecialMode(SPECIAL_EFFECTS specialMode);
    bool getSpecialMode();

    bool setDenoiseValue(uint denoiseVal);
    bool getDenoiseValue();

    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setQFactorValue(uint QFactor);
    bool getQFactorValue();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);
    bool getFaceDetectMode();

    bool setSmileDetection(bool enableSmileDetect, bool embedData);
    bool getSmileDetectMode();

    bool setExposureCompensation(unsigned int exposureCompValue);
    bool getExposureCompensation();

    bool setFrameRateCtrlValue(uint frameRate);
    bool getFrameRateCtrlValue();

    bool setFlashMode(FLASH_MODE flashMode);
    bool getFlashMode();

    bool setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode);
    bool getAntiFlickerMode();

    bool storeSettings();

    bool setToDefaultValues();
};

#endif // ECAM_512USB_H
