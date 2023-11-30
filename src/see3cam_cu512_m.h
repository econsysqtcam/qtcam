#ifndef SEE3CAMCU512M_H
#define SEE3CAMCU512M_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID1_SEE3CAM_CU512M        0XC0
#define CAMERA_CONTROL_ID2_SEE3CAM_CU512M        0X04

#define GET_STREAM_MODE_SEE3CAM_CU512M           0x01
#define SET_STREAM_MODE_SEE3CAM_CU512M           0x02

#define GET_FLASH_MODE_SEE3CAM_CU512             0x03
#define SET_FLASH_MODE_SEE3CAM_CU512             0x04

#define GET_ORIENTATION_MODE_SEE3CAM_CU512M      0x06
#define SET_ORIENTATION_MODE_SEE3CAM_CU512M      0x07

#define SET_DEFAULT_SEE3CAM_CU512M               0x05

#define SET_FAIL		0x00
#define SET_SUCCESS		0x01


class SEE3CAM_CU512M : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_CU512M();

    enum CAMERA_MODE{
         MASTER  = 0x01,
         TRIGGER = 0x02
    };
    Q_ENUMS(CAMERA_MODE)

    enum FlashMode{
         DISABLE = 0x00,
         STROBE  = 0x01,
         TORCH   = 0x02,
     };
     Q_ENUMS(FlashMode)

    enum FLIP_MODE{
        NORMAL       = 0x00,
        HORIZONTAL   = 0x01,
        VERTICAL     = 0x02,
        ROTATE_180   = 0x03
    };
    Q_ENUMS(FLIP_MODE)

signals:
    void streamModeReceived(uint streamMode);
    void flashModeReceived(uint flashMode);
    void flipMirrorModeChanged(uint flipMirrorModeValues);

public slots:
    bool setStreamMode(CAMERA_MODE cameraMode);
    bool getStreamMode();

    bool setFlashMode(FlashMode flashMode);
    bool getFlashMode();

    bool setOrientation(bool horzModeSel, bool vertiModeSel);
    bool getOrientation();

    bool setToDefaultValues();
};
#endif // SEE3CAMCU512M_H
