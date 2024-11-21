#ifndef SEE3CAM_CU31_H
#define SEE3CAM_CU31_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID_1        0xC0
#define CAMERA_CONTROL_ID_2        0x0A

#define GET_ORIENTATION_SEE3CAM_CU31            0x01
#define SET_ORIENTATION_SEE3CAM_CU31            0x02

#define GET_RNR_STATUS             0x03
#define SET_RNR_STATUS             0x04

#define GET_TRIGGER_MODE           0x05
#define SET_TRIGGER_MODE           0x06

#define GET_AE_CONVERGENCE_SPEED   0x07
#define SET_AE_CONVERGENCE_SPEED   0x08

#define GET_AE_HOLD                0x09
#define SET_AE_HOLD                0x0A

#define GET_AWB_STATUS             0x0B
#define SET_AWB_STATUS             0x0C

#define READ_TEMPERATURE           0x0D

#define SET_DEFAULT                0xFF

#define GET_64BIT_UNIQUE_ID_1      0x41
#define GET_64BIT_UNIQUE_ID_2      0x01

#define GET_ANTI_FLICKER_MODE      0x0E
#define SET_ANTI_FLICKER_MODE      0x0F

#define GET_CAMERA_MODE_SEE3CAM_CU31      0x10
#define SET_CAMERA_MODE_SEE3CAM_CU31      0x11

#define SET_CROSS_STILL_PROPERTIES_SEE3CAM_CU31     0x44

#define DIMENSION_FAILURE   0x01
#define WIDTH_FAILURE       0x02
#define HEIGHT_FAILURE      0x03

//Second Phase Controls
#define GET_AE_OVERLAY_SEE3CAM_CU31              0x12
#define SET_AE_OVERLAY_SEE3CAM_CU31              0x13

#define GET_AE_DIMENSIONS_SEE3CAM_CU31           0x14
#define SET_AE_DIMENSIONS_SEE3CAM_CU31           0x15

#define GET_AE_MASK_OVERLAY_SEE3CAM_CU31         0x16
#define SET_AE_MASK_OVERLAY_SEE3CAM_CU31         0x17

#define GET_AE_MASK_DIMENSIONS_SEE3CAM_CU31      0x18
#define SET_AE_MASK_DIMENSIONS_SEE3CAM_CU31      0x19

#define GET_AWB_OVERLAY_SEE3CAM_CU31             0x1A
#define SET_AWB_OVERLAY_SEE3CAM_CU31             0x1B

#define GET_AWB_WINDOW_DIMENSIONS_SEE3CAM_CU31   0x1C
#define SET_AWB_WINDOW_DIMENSIONS_SEE3CAM_CU31   0x1D

#define GET_AWB_MASK_OVERLAY_SEE3CAM_CU31        0x1E
#define SET_AWB_MASK_OVERLAY_SEE3CAM_CU31        0x1F

#define GET_AWB_MASK_DIMENSIONS_SEE3CAM_CU31     0x20
#define SET_AWB_MASK_DIMENSIONS_SEE3CAM_CU31     0x21

#define GET_USER_PRESET_MODE_SEE3CAM_CU31        0x22
#define SET_USER_PRESET_MODE_SEE3CAM_CU31        0x23

#define GET_EMBEDDED_DATA_SEE3CAM_CU31           0x24
#define SET_EMBEDDED_DATA_SEE3CAM_CU31           0x25

#define ISP_FIRMWARE_VERSION_SEE3CAM_CU31        0x45


class SEE3CAM_CU31 : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_CU31();
    ~SEE3CAM_CU31();

    QString _title;
    QString _text;

    enum ORIENTATION{
        NORMAL     = 0x00,
        HORIZONTAL = 0x01,
        VERTICAL   = 0x02,
        ROTATE_180 = 0x03
    };
    Q_ENUMS(ORIENTATION)

    enum RNR_STATUS{
        RNR_ENABLE  = 0x01,
        RNR_DISABLE = 0x00
    };
    Q_ENUMS(RNR_STATUS)

    enum TRIGGER_MODE{
        DISABLE_TRIGGER  = 0x00,
        READOUT_TRIGGER  = 0x01,
        SHUTTER_TRIGGER  = 0x02
    };
    Q_ENUMS(TRIGGER_MODE)

    enum AUTO_EXPOSURE{
        AE_RELEASE = 0x00,
        AE_HOLD    = 0x01
    };
    Q_ENUMS(AUTO_EXPOSURE)

    enum AUTO_WHITE_BALANCE{
        AWB_RELEASE = 0x00,
        AWB_HOLD    = 0x01
    };
    Q_ENUMS(AUTO_WHITE_BALANCE)

    enum ANTI_FLICKER_DETECTION{
        MODE_AUTO    = 0x00,
        MODE_50Hz    = 0x01,
        MODE_60Hz    = 0x02,
        MODE_DISABLE = 0x03
    };
    Q_ENUMS(ANTI_FLICKER_DETECTION)

    enum CAMERA_MODE{
        AUTO      = 0x00,
        LOW_LIGHT = 0x01,
        LED       = 0x02
    };Q_ENUMS(CAMERA_MODE)

    enum PRESET_MODE{
        MANUAL_PRESET  = 0x00,
        USER_PRESET_1  = 0x01,
        USER_PRESET_2  = 0x02
    };Q_ENUMS(PRESET_MODE)

    enum PRESET_BUTTON{
        PRESET_SAVE   = 0x01,
        PRESET_SELECT = 0x00
    };Q_ENUMS(PRESET_BUTTON)

signals:
    void flipMirrorModeChanged(uint flipMirrorModeValues);
    void currentRawNoiseDeductionStatus(uint status);
    void currentTriggerMode(uint mode);
    void currentAEConvergenceSpeed(uint currentSpeed);
    void currentAutoExposureStatus(uint status);
    void currentAutoWhiteBalanceStatus(uint status);
    void currentTemperature(float temperature);
    void currentAntiFlickerModeRecieved(uint mode);
    void currentCameraModeReceived(uint mode);
    void indicateFailureStatus(QString title, QString text);
    void titleTextChanged(QString _title,QString _text);

    void currentAEWindowOverlayStatus(uint status);
    void currentAEWindowDimensions(uint width, uint height, uint xStart, uint yStart);
    void currentAEMaskOverlayStatus(uint status);
    void currentAEMaskDimensions(bool isMaskIdNeeded, uint maskID, uint maskState, uint width, uint height, uint xStart, uint yStart);

    void currentAWBWindowOverlayStatus(uint status);
    void currentAWBWindowDimensions(uint width, uint height, uint xStart, uint yStart);
    void currentAWBMaskOverlayStatus(uint status);
    void currentAWBMaskDimensions(bool isMaskIDNeeded, uint maskID, uint maskState, uint width, uint height, uint xStart, uint yStart);

    void currentPresetProperties(uint presetMode, uint presetType);
    void currentEmbedDataStatus(uint status);

    void indicateWindowDimensionError(uint minWidth, uint maxWidth, uint minHeight, uint maxHeight, uint minXStart, uint maxXStart, uint minYStart, uint maxYStart);
    void indicateMaskDimensionError(uint minWidth, uint maxWidth, uint minHeight, uint maxHeight, uint minXStart, uint maxXStart, uint minYStart, uint maxYStart);

public slots:
    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool getRawNoiseReductionStatus();
    bool setRawNoiseReductionStatus(RNR_STATUS status);

    bool getTriggerMode();
    bool setTriggerMode(TRIGGER_MODE mode);

    bool getAEConvergenceSpeed();
    bool setAEConvergenceSpeed(uint value);

    bool getAutoExpStatus();
    bool setAutoExposureStatus(AUTO_EXPOSURE exposureStatus);

    bool getAWBStatus();
    bool setAWBStatus(AUTO_WHITE_BALANCE whileBalanceStatus);

    bool setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode);
    bool getAntiFlickerMode();

    bool setCameraMode(CAMERA_MODE cameraMode);
    bool getCameraMode();

    bool readTemperature();

    //AUTO EXPOSURE
    bool getAEWindowOverlay();
    bool setAEWindowOverlay(uint status);

    bool getAEWindowDimensions();
    bool setAEWindowDimensions(uint width, uint height, uint xStart, uint yStart);

    bool getAEMaskOverlay();
    bool setAEMaskOverlay(uint status);

    bool getAEMaskDimensions(bool isMaskIDNeeded);
    bool setAEMaskDimensions(uint maskID, uint maskStatus, uint width, uint height, uint xStart, uint yStart);

    //AUTO WHITE BALANCE
    bool getAWBWindowOverlay();
    bool setAWBWindowOverlay(uint status);

    bool getAWBWindowDimensions();
    bool setAWBWindowDimensions(uint width, uint height, uint xStart, uint yStart);

    bool getAWBMaskOverlay();
    bool setAWBMaskOverlay(uint status);

    bool getAWBMaskDimensions(bool isMaskIDNeeded);
    bool setAWBMaskDimensions(uint maskID, uint maskStatus, uint width, uint height, uint xStart, uint yStart);

    //OTHER CONTROLS
    bool getEmbeddedData();
    bool setEmbeddedData(uint status);

    bool getPresetMode();
    bool setPresetMode(PRESET_MODE mode, PRESET_BUTTON button);

    bool setPropertiesForCrossStill();

    bool setToDefaultValues();

    bool get64BitSerialNumber();

    bool readISPFirmwareVersion();

};

#endif // SEE3CAM_CU31_H
