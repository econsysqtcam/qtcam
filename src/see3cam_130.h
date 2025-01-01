#ifndef SEE3CAM_130_H
#define SEE3CAM_130_H


#include <QObject>
#include "uvccamera.h"


#define DENOISE_MIN 0
#define DENOISE_MAX 15

#define EXPOSURECOMP_MIN 8000
#define EXPOSURECOMP_MAX 1000000


/* SEE3CAM_130 */
#define CAMERA_CONTROL_130              0x7B

#define GET_SCENEMODE_130				0x01
#define SET_SCENEMODE_130				0x02

#define GET_SPECIAL_EFFECT_130			0x03
#define SET_SPECIAL_EFFECT_130			0x04

#define GET_DENOISE_CONTROL             0x05
#define SET_DENOISE_CONTROL             0x06

#define GET_AF_MODE_130					0x07
#define SET_AF_MODE_130					0x08

#define GET_AF_ROI_MODE_130				0x09
#define SET_AF_ROI_MODE_130				0x0A

#define GET_EXP_ROI_MODE_130			0x0B
#define SET_EXP_ROI_MODE_130			0x0C

#define GET_BURST_LENGTH_130			0x0D
#define SET_BURST_LENGTH_130			0x0E

#define GET_HDR_MODE_130				0x0F
#define SET_HDR_MODE_130				0x10

#define GET_Q_FACTOR_130				0x11
#define SET_Q_FACTOR_130				0x12

#define ENABLE_DISABLE_MODE_AF_RECTANGLE_130     0x13

#define GET_STREAM_MODE_130             0x14
#define SET_STREAM_MODE_130             0x15

#define GET_AF_RECT_MODE                0x16

#define SET_TO_DEFAULT                  0x17

#define SET_FLIP_MODE_130                 0x19
#define GET_FLIP_MODE_130                 0x18

#define GET_AWB_130                     0x1A
#define GET_AUTO_EXPOSURE               0x1B

#define GET_FLASH_MODE_130              0x1C
#define SET_FLASH_MODE_130              0x1D

#define GET_FACE_DETECT_RECT            0x1E
#define SET_FACE_DETECT_RECT            0x1F

#define GET_AF_POSITION                 0x20

#define GET_EXPOSURE_COMPENSATION       0x22
#define SET_EXPOSURE_COMPENSATION       0x23

#define GET_FRAME_RATE_130              0x24
#define SET_FRAME_RATE_130              0x25

#define STILL_CAPTURE_STATUS            0x26

#define GET_FLICKER_DETECTION_130       0x27
#define SET_FLICKER_DETECTION_130       0x28

#define GET_RED_GAIN                    0x29
#define SET_RED_GAIN                    0x2A

#define GET_BLUE_GAIN                   0x2B
#define SET_BLUE_GAIN                   0x2C

#define GET_GREEN_GAIN                  0x2D
#define SET_GREEN_GAIN                  0x2E

#define SAVE_CONFIGURATION               0x42
#define SAVE                             0x01

//Face Detection
#define ENABLE_FACE_RECT                0x01
#define DISABLE_FACE_RECT               0x00

#define ENABLE_EMBED_DATA               0x01
#define DISABLE_EMBED_DATA              0x00

#define ENABLE_OVERLAY_RECT             0x01
#define DISABLE_OVERLAY_RECT            0x00


//Enable/Disable AF Rectangle
#define ENABLE_AF_RECTANGLE_130     0x01
#define DISABLE_AF_RECTANGLE_130     0x00


//Still Capture Status
#define STILL_CAPTURE_DISABLE_RECTANGLE      0x00
#define STILL_CAPTURE_ENABLE_RECTANGLE       0x01

//Status
#define GET_FAIL		0x00
#define GET_SUCCESS		0x01

#define SET_HORZ_FLIP_MODE_130             0x00
#define SET_VERT_FLIP_MODE_130             0x01

#define SET_FAIL		0x00
#define SET_SUCCESS		0x01

#define ENABLE_DISABLE_MODE_FACE_RECTANGLE_130       0x26
#define ENABLE_FACE_RECTANGLE_130       0x01
#define DISABLE_FACE_RECTANGLE_130      0x00

#define GET_64BIT_UNIQUE_ID_1      0x41
#define GET_64BIT_UNIQUE_ID_2      0x01


class See3CAM_130 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    explicit See3CAM_130(QObject *parent = 0);

    QString _title;
    QString _text;

    enum SCENE_MODES {
        SCENE_NORMAL   = 0x01,
        SCENE_DOCUMENT = 0x0C
    };
    Q_ENUMS(SCENE_MODES)

    enum SPECIAL_EFFECTS {
        EFFECT_NORMAL       = 0x01,
        EFFECT_BLACK_WHITE  = 0x04,
        EFFECT_GREYSCALE    = 0x07,
        EFFECT_NEGATIVE     = 0x08,
        EFFECT_SKETCH       = 0x10
    };
    Q_ENUMS(SPECIAL_EFFECTS)

    enum AUTO_FOCUS_MODE {
        CONTINIOUS_SCAN     = 0x01,
        SINGLE_TRIGGER      = 0x02
    };
    Q_ENUMS(AUTO_FOCUS_MODE)

    enum AUTO_FOCUS_ROI {
        AF_CENTERED_ROI = 0x01,
        AF_MANUAL_ROI   = 0x02
    };
    Q_ENUMS(AUTO_FOCUS_ROI)

    enum AUTO_EXPOSURE_ROI {
        AE_CENTERED_ROI   = 0x01,
        AE_MANUAL_ROI     = 0x02
    };
    Q_ENUMS(AUTO_EXPOSURE_ROI)

    enum iHDR_MODE {
        HdrOff    = 0x01,
        HdrAuto   = 0x02,
        HdrManual = 0x03
    };
    Q_ENUMS(iHDR_MODE)

    enum STREAM_MODE {
        STREAM_MASTER = 0x00,
        STREAM_TRIGGER = 0x01
    };
    Q_ENUMS(STREAM_MODE)

    enum camFlipMode {
            FlipHorizontal = 0x00,
            FlipVertical = 0x01,
            FlipBoth = 0x02
    };Q_ENUMS(camFlipMode)


    enum FLASH_MODE {
        DISABLE  = 0x00,
        STROBE   = 0x01,
        TORCH    = 0x02
    };
    Q_ENUMS(FLASH_MODE)

    enum FLICKER_MODE {
         MODE_AUTO    = 0x00,
         MODE_50Hz    = 0x01,
         MODE_60Hz    = 0x02,
         MODE_DISABLE = 0x03
    };
    Q_ENUMS(FLICKER_MODE)

    enum AFRectangleStatus{
        AF_RECT_DISABLE = 0x00,
        AF_RECT_ENABLE  = 0x01
    };
    Q_ENUMS(AFRectangleStatus)

    enum camFaceRectMode {
        FaceRectDisable = 0x00,
        FaceRectEnable  = 0x01
    };
    Q_ENUMS(camFaceRectMode)

    enum camFlipEnableDisableMode {
        FlipEnable = 0x01,
        FlipDisable = 0x00
    };
    Q_ENUMS(camFlipEnableDisableMode)

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

     void sceneModeValueReceived(uint sceneMode);
     void effectModeValueReceived(uint effectMode);
     void denoiseValueReceived(uint denoiseValue);
     void afModeValueReceived(uint afMode);
     void roiAfModeValueReceived(uint roiMode, uint winSize);
     void roiAutoExpModeValueReceived(uint roiMode, uint winSize);
     void burstLengthValueReceived(uint burstLength);
     void hDRModeValueReceived(uint hdrMode, uint hdrValue);
     void qFactorValueReceived(uint qFactor);

     void streamModeValueReceived(uint streamMode);
     void flipModeValue(uint flipMode, uint flipEnableDisableMode);
     void autoWhiteBalanceReceived(uint autoWhiteBalance);
     void autoExposureReceived(uint autoExposure);
     void flashModeReceived(uint flashMode);
     void faceDetectModeValueReceived(uint faceDetectMode, uint faceDetectEmbedDataValue, uint faceDetectOverlayRect);
     void autoFocusPositionReceived(uint afPosition);
     void exposureCompValueReceived(uint exposureCompensation);
     void frameRateCtrlValueReceived(uint frameRateCtrlValue);
     void flickerDetectionModeReceived(uint flickerMode);

     void redGainMinReceived(uint minRed);
     void redGainMaxReceived(uint maxRed);
     void redGainCurrentReceived(uint currentRed);

     void blueGainMinReceived(uint minBlue);
     void blueGainMaxReceived(uint maxBlue);
     void blueGainCurrentReceived(uint currentBlue);

     void greenGainMinReceived(uint minGreen);
     void greenGainMaxReceived(uint maxGreen);
     void greenGainCurrentReceived(uint currentGreen);

     void titleTextChanged(QString _title,QString _text);
     void indicateCommandStatus(QString title, QString text);
     void indicateExposureValueRangeFailure(QString title, QString text);

public slots:
    bool getSceneMode();
    bool setSceneMode(SCENE_MODES sceneMode);

    bool getEffectMode();
    bool setEffectMode(SPECIAL_EFFECTS specialEffect);

    bool setDenoiseValue(int deNoiseVal);
    bool getDenoiseValue();

    bool setAutoFocusMode(AUTO_FOCUS_MODE afMode);
    bool getAutoFocusMode();

    bool setROIAutoFocus(AUTO_FOCUS_ROI see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoFocusROIModeAndWindowSize();

    bool setROIAutoExposure(AUTO_EXPOSURE_ROI see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool getBurstLength();
    bool setBurstLength(uint burstLength);

    bool getiHDRMode();
    bool setiHDRMode(iHDR_MODE iHDRMode, uint iHDRValue);

    bool getQFactor();
    bool setQFactor(uint qFactor);

    bool enableDisableAFRectangle(bool enableRFRect);

    bool getStreamMode();
    bool setStreamMode(STREAM_MODE streamMode);

    bool setFlipHorzMode(bool horizModeSel);
    bool setFlipVertiMode(bool vertiModeSel);
    bool getFlipMode();

    bool getAutoWhiteBalance();

    bool getAutoExposure();

    bool getFlashMode();
    bool setFlashMode(FLASH_MODE mode);

    bool getFaceDetectMode();
    bool setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect);

    bool getAutoFocusPosition();

    bool getExposureCompensation();
    bool setExposureCompensation(unsigned int exposureCompValue);

    bool getFrameRateCtrlValue();
    bool setFrameRateCtrlValue(uint frameRate);

    bool getFlickerDetection();
    bool setFlickerDetection(FLICKER_MODE flickerMode);

    bool getRedGain();
    bool setRedGain(uint redGain);

    bool getBlueGain();
    bool setBlueGain(uint blueGain);

    bool getGreenGain();
    bool setGreenGain(uint greenGain);

    bool enableDisableFaceRectangle(bool enableFaceRect);

    bool setToDefault();

    bool saveConfiguration();

    bool get64BitSerialNumber();

};
#endif // SEE3CAM_130_H
