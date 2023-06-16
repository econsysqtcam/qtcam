#ifndef SEE3CAMCU512M_H
#define SEE3CAMCU512M_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID1_SEE3CAM_CU512M        0XC0
#define CAMERA_CONTROL_ID2_SEE3CAM_CU512M        0X04

#define GET_FLASH_MODE_SEE3CAM_CU512             0x03
#define SET_FLASH_MODE_SEE3CAM_CU512             0x04

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

    enum FlashMode{
         DISABLE = 0x00,
         STROBE  = 0x01,
         TORCH   = 0x02,
     };
     Q_ENUMS(FlashMode)
signals:
    void flashModeReceived(uint flashMode);

public slots:

    bool setFlashMode(FlashMode flashMode);
    bool getFlashMode();

    bool setToDefaultValues();

};
#endif // SEE3CAMCU512M_H
