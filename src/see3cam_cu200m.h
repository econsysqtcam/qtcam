#ifndef SEE3CAM_CU200_M_H
#define SEE3CAM_CU200_M_H

#include <QObject>
#include <QString>
#include "uvccamera.h"


#define CAMERA_CONTROL_ID_1      0xC0
#define CAMERA_CONTROL_ID_2      0x0C

#define GET_STREAM_MODE          0x01
#define SET_STREAM_MODE          0x02

#define GET_FLASH_MODE           0x03
#define SET_FLASH_MODE           0x04

#define GET_ORIENTATION          0x05
#define SET_ORIENTATION          0x06

#define RESET_COMMAND_1          0x32
#define RESET_COMMAND_2          0x04

#define RESTORE_DEFAULT          0XFF


class SEE3CAM_CU200M : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_CU200M();
    ~SEE3CAM_CU200M();

    enum ORIENTATION{
        NORMAL     = 0x00,
        HORIZONTAL = 0x01,
        VERTICAL   = 0x02,
        ROTATE_180 = 0x03
    };
    Q_ENUMS(ORIENTATION)

    enum STREAM_MODE{
        MASTER_MODE = 0x00,
        TRIGGER_MODE = 0x01
    };
    Q_ENUMS(STREAM_MODE)

    enum FLASH_MODE{
        DISABLE = 0x00,
        STROBE  = 0x01,
        TORCH   = 0x02
    };
    Q_ENUMS(FLASH_MODE)


signals:

    void flipMirrorModeChanged(uint mode);
    void streamModeReceived(uint mode);
    void flashModeReceived(uint mode);

    void indicateFailureStatus(QString title, QString text);

public slots:

    bool getStreamMode();
    bool setStreamMode(STREAM_MODE mode);

    bool getFlashMode();
    bool setFlashMode(FLASH_MODE mode);

    bool getOrientation();
    bool setOrientation(bool horzModeSel, bool vertiModeSel);

    bool resetDevice();

    bool setToDefault();

};

#endif // SEE3CAM_CU200_M_H
