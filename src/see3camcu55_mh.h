#ifndef SEE3CAMCU55_MH
#define SEE3CAMCU55_MH
#include "uvccamera.h"
#include <QObject>

#define CAMERA_CONTROL_SEE3CAM_CU55_MH  0x9F
#define SET_SUCCESS                     0x01
#define SET_FAILURE                     0x00

#define SET_STREAM_MODE                 0x02
#define GET_STREAM_MODE                 0x01

#define GET_SUCCESS                     0x01
#define GET_FAILURE                     0x00

#define SET_FLASH_MODE                 0x04
#define GET_FLASH_MODE                 0x03

#define SET_ROLL_CONTROL                0x07
#define GET_ROLL_CONTROL                0x06

#define SET_DEFAULT                     0x05

#define READ_FIRMWARE_VERSION           0x40

#define READ_UNIQUE_ID                  0x41

class See3CAM_CU55_MH : public QObject
{
    Q_OBJECT

 private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;
    QString _title;
    QString _text;

    void initializeBuffers();
public :
     explicit See3CAM_CU55_MH(QObject *parent = 0);

    enum streamModes {
       MODE_MASTER =0x00,
       MODE_TRIGGER =0x01
    };

    Q_ENUMS(streamModes)


    enum flashModes {
       MODE_OFF =0x00,
       MODE_STROBE =0x01,
       MODE_TORCH =0x02
    };

    Q_ENUMS(flashModes)

    enum rollControl {
       FLIP_NORMAL =0x01,
       FLIP_HORIZONTAL =0x02,
       FLIP_VERTICAL =0x03,
       FLIP_BOTH =0x04
    };
    Q_ENUMS(rollControl)

signals:
        void streamModeValue(uint streamMode);
        void flashModeValue (uint flashMode);
        void rollCtrlValue (uint rollValue);
        void titleTextChanged(QString _title,QString _text);

public slots:

         bool setStreamMode(streamModes streamMode);
         bool getStreamMode();

         bool setFlashMode(flashModes flashMode);
         bool getFlashMode();

         bool setRollCtrlValue(uint rollValue);
         bool getRollCtrlValue();

         bool setToDefault();
      //   bool readFirmwareVersion(uint *pMajorVersion, uint *pMinorVersion1, uint *sdkVersion, uint *svnVersion);
};
#endif // SEE3CAMCU55_MH

