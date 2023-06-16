#ifndef SEE3CAM_16CUGM_H
#define SEE3CAM_16CUGM_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID1_SEE3CAM_16CUGM            0xC0
#define CAMERA_CONTROL_ID2_SEE3CAM_16CUGM            0x01

#define GET_SHUTTER_MODE_SEE3CAM_16CUGM              0x01
#define SET_SHUTTER_MODE_SEE3CAM_16CUGM              0x02

#define GET_STROBE_MODE_SEE3CAM_16CUGM               0x03
#define SET_STROBE_MODE_SEE3CAM_16CUGM               0x04

#define GET_ORIENTATION_MODE_SEE3CAM_16CUGM          0x05
#define SET_ORIENTATION_MODE_SEE3CAM_16CUGM          0x06

#define GET_BLACKLEVEL_MODE_SEE3CAM_16CUGM           0x07
#define SET_BLACKLEVEL_MODE_SEE3CAM_16CUGM           0x08

#define GET_HDR_MODE_SEE3CAM_16CUGM                  0x0B
#define SET_HDR_MODE_SEE3CAM_16CUGM                  0x0C

#define GET_STREAM_MODE_SEE3CAM_16CUGM               0x0D
#define SET_STREAM_MODE_SEE3CAM_16CUGM               0x0E

#define SET_DEFAULT_SEE3CAM_16CUGM                   0xFF


#define SET_FAIL		                             0x00
#define SET_SUCCESS		                             0x01


class SEE3CAM_16CUGM : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_16CUGM();

    enum SHUTTER_MODE{
         GLOBAL_SHUTTER   = 0x00,
         ROLLING_SHUTTER  = 0x01
     };
     Q_ENUMS(SHUTTER_MODE)

    enum STROBE_MODE{
         OFF   = 0x00,
         ON    = 0x01
     };
     Q_ENUMS(STROBE_MODE)

    enum getOrientation{
        NORMAL     = 0x00,
        HORIZONTAL = 0x01,
        VERTICAL   = 0x02,
        ROTATE_180       = 0x03
    };
    Q_ENUMS(getOrientation)

    enum BLACKLEVEL_CORRECTION{
         AUTO   = 0x00,
         MANUAL = 0x01
     };
     Q_ENUMS(BLACKLEVEL_CORRECTION)

    enum HDR_MODE{
         HDR_OFF   = 0x00,
         HDR_ON    = 0x01
     };
     Q_ENUMS(HDR_MODE)

    enum STREAM_MODE{
         MASTER     = 0x00,
         TRIGGER    = 0x01
     };
     Q_ENUMS(STREAM_MODE)

signals:
    void shutterModeReceived(uint shutterMode);

    void strobeModeReceived(uint strobeMode);

    void flipMirrorModeChanged(uint flipMirrorModeValues);

    void blackLevelModeReceived(uint blackLevelMode);
    void currentOffsetReceived(uint currentOffset);
    void minimumOffsetReceived(uint minOffset);
    void maximumOffsetReceived(uint maxOffset);

    void hdrModeReceived(uint hdrMode);

    void streamModeReceived(uint streamMode);

public slots:

    bool setShutterMode(SHUTTER_MODE flashMode);
    bool getShutterMode();

    bool setStrobeMode(STROBE_MODE strobeMode);
    bool getStrobeMode();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setBlackLevelCorrection(BLACKLEVEL_CORRECTION blackLevelMode, uint clampOffset);
    bool getBlackLevelCorrection();

    bool setHDRMode(HDR_MODE hdrMode);
    bool getHDRMode();

    bool setStreamMode(STREAM_MODE streamMode);
    bool getStreamMode();

    bool setToDefaultValues();

};
#endif // SEE3CAM_16CUGM_H
