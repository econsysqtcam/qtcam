#ifndef SEE3CAM_135M_H
#define SEE3CAM_135M_H

#include "uvccamera.h"
#include <QObject>

#define CAMERA_CONTROL_135M                  0xB1

#define GET_STREAM_MODE_135M                 0x01
#define SET_STREAM_MODE_135M                 0x02

#define GET_FLASH_MODE_135M                  0x03
#define SET_FLASH_MODE_135M                  0x04

#define GET_ORIENTATION_135M                 0x05
#define SET_ORIENTATION_135M                 0x06

#define GET_EXP_ROI_MODE_135M                0x07
#define SET_EXP_ROI_MODE_135M                0x08

#define GET_FOCUS_ROI_MODE_135M              0x09
#define SET_FOCUS_ROI_MODE_135M              0x0A

#define SET_TO_DEFAULT_135M                  0xFF

#define GET_SUCCESS                           0x01
#define GET_FAILURE                           0x00

class See3CAM_135M : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    See3CAM_135M();
    ~See3CAM_135M();

    enum flashStateValues{
         FLASHMODE_OFF = 0x00,
         FLASHMODE_TORCH = 0x02,
         FLASHMODE_STROBE = 0x01,
     };
     Q_ENUMS(flashStateValues)

    enum streamModes {
       MODE_MASTER =0x00,
       MODE_TRIGGER =0x01
    };
    Q_ENUMS(streamModes)

    enum flipControls {
       FLIP_NORMAL =0x00,
       FLIP_HORIZONTAL =0x01,
       FLIP_VERTICAL =0x02,
       FLIP_BOTH =0x03
    };
    Q_ENUMS(flipControls)

    enum camROIAutoExpMode {
        AutoExpFull = 0x01,
        AutoExpManual = 0x02,
    };
    Q_ENUMS(camROIAutoExpMode)

    enum camROIAutoFocusMode {
        AutoFocusFull = 0x01,
        AutoFocusManual = 0x02,
    };
    Q_ENUMS(camROIAutoFocusMode)

signals:
    void flashModeValue(uint flashMode);

    void streamModeValue(uint streamMode);

    void flipCtrlValue(uint flipValue);

    void roiAutoExpMode(uint roiMode, uint winSize);

    void roiAutoFocusMode(uint roiMode, uint winSize);


public slots:

    bool setFlashState(flashStateValues flashMode);
    bool getFlashState();

    bool getStreamMode();
    bool setStreamMode(streamModes streamMode);

    bool setFlipCtrlValue(bool horzModeSel, bool vertiModeSel);
    bool getFlipCtrlValue();

    bool setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoExpROIModeAndWindowSize();

    bool setROIAutoFocus(camROIAutoFocusMode see3camAutofocusROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize);
    bool getAutoFocusROIModeAndWindowSize();

    bool setToDefault();


};

#endif // SEE3CAM_135M_H
