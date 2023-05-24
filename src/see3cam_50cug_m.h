#ifndef SEE3CAM_50CUG_M_H
#define SEE3CAM_50CUG_M_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_SEE3CAM_50CUG_M                    0xBF

#define GET_CAMERA_MODE_SEE3CAM_50CUG_M                   0x01
#define SET_CAMERA_MODE_SEE3CAM_50CUG_M                   0x02

#define GET_ORIENTATION_SEE3CAM_50CUG_M                   0x03
#define SET_ORIENTATION_SEE3CAM_50CUG_M                   0x04

#define GET_GAIN_MODE_SEE3CAM_50CUG_M                     0x05
#define SET_GAIN_MODE_SEE3CAM_50CUG_M                     0x06

#define GET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M        0x07
#define SET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M        0x08

#define GET_STROBE_MODE_SEE3CAM_50CUG_M                   0x09
#define SET_STROBE_MODE_SEE3CAM_50CUG_M                   0x0A

#define GET_IMAGE_BURST_SEE3CAM_50CUG_M                   0x0B
#define SET_IMAGE_BURST_SEE3CAM_50CUG_M                   0x0C

#define SET_DEFAULT_SEE3CAM_50CUGM                        0xFF

#define SAVE_CONFIGURATION_SEE3CAM_50CUG_M                0x42
#define SAVE_SEE3CAM_50CUG_M                              0x01



#define SET_FAIL	 0x00
#define SET_SUCCESS  0x01

#define GET_FAIL     0x00
#define GET_SUCCESS	 0x01

class SEE3CAM_50CUGM : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_50CUGM();

    QString _title;
    QString _text;

    enum cameraModes {
        MASTER = 0x01,
        TRIGGER_EXP_CONTROL = 0x02,
        TRIGGER_ACQ_CONTROL = 0x03
    };
    Q_ENUMS(cameraModes)

    enum orientation{
        NORMAL     = 0x01,
        VERTICAL   = 0x02,
        HORIZONTAL = 0x03,
        ROTATE_180 = 0x04
    };
    Q_ENUMS(orientation)

    enum blackLevelAdjustment {
        AUTO  = 0x01,
        MANUAL = 0x02,
    };
    Q_ENUMS(blackLevelAdjustment)

    enum strobeMode {
        FLASH = 0x00,
        OFF   = 0x02
    };
    Q_ENUMS(strobeMode)

    signals:
        void cameraModeChanged(uint cameraMode);
        void flipMirrorModeChanged(uint flipMode);
        void strobeModeChanged(uint strobe);
        void blackLevelModeChanged(uint blacklevelMode);
        void blackLevelValueChanged(uint blackLevelValue);
        void imageBurstChanged(uint burstLength);
        void indicateCommandStatus(QString title, QString text);

    public slots:
        bool setCameraMode(cameraModes cameraMode);
        bool getCameraMode();

        bool setOrientation(bool horzModeSel, bool vertiModeSel);
        bool getOrientation();

        bool setStrobeMode(strobeMode strobe);
        bool getStrobeMode();

        bool setBlackLevelAdjustment(blackLevelAdjustment blackLevelMode, uint blackLevelValue);
        bool getBlackLevelAdjustment();

        bool setBurstLength(uint burstlength);
        bool getBurstLength();

        bool setToDefaultValues();
        bool saveConfiguration();

};
#endif // SEE3CAM_50CUG_M_H
