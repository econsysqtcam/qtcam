#ifndef ECAM_514USB_H
#define ECAM_514USB_H

#include <QObject>
#include <QString>
#include "uvccamera.h"


#define CAMERA_CONTROL_ID1_ECAM514USB                    0xC0
#define CAMERA_CONTROL_ID2_ECAM514USB                    0x11

#define GET_CHROMA_DENT_MODE_ECAM514USB                  0x01
#define SET_CHROMA_DENT_MODE_ECAM514USB                  0x02

#define GET_LED_STATUS_ECAM514USB                        0x03
#define SET_LED_STATUS_ECAM514USB                        0x04

#define GET_BURST_LENGTH_ECAM514USB                      0x05
#define SET_BURST_LENGTH_ECAM514USB                      0x06

#define RESET_COMMAND_1                                  0x32
#define RESET_COMMAND_2                                  0x04

#define READ_FIRMWARE_VERSION_ECAM514USB                 0x40

#define GET_64BIT_UNIQUE_ID_1                            0x41
#define GET_64BIT_UNIQUE_ID_2                            0x01

#define SAVE_CONFIGURATION_ECAM514USB                    0x42
#define SAVE_ECAM514USB                                  0x01

#define SET_RAW_STREAM_ECAM514USB                        0x46

#define HID_IMG_CAPTURE_ECAM514USB                       0x47
#define CAPTURE_ECAM514USB                               0x01

#define SET_TO_DEFAULT_ECAM514USB                        0xFF


class ECAM514_USB : public QObject
{
    Q_OBJECT

private:

    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];

    uvccamera uvc;

    void initializeBuffers();

public:

    ECAM514_USB();

    QString _title;
    QString _text;

    enum CHROMA_DENT_MODE{
        NORMAL                  = 0x01,
        CAVITY_MODE             = 0x02,
        GUM_HEALTH_MODE         = 0x03,
        EROSION_MODE            = 0x04,
        CRACK_DETECTION_MODE    = 0x05,
        PLAQUE_TARTAR_MODE      = 0x06
    };
    Q_ENUMS(CHROMA_DENT_MODE)

    enum LED_STATUS{
        LED_ON                  = 0x01,
        LED_OFF                 = 0x02
    };
    Q_ENUMS(LED_STATUS)

    enum RAW_STREAM{
        ENABLE                  = 0x01,
        DISABLE                 = 0x02
    };
    Q_ENUMS(RAW_STREAM)

signals:
    void chromaDentModeReceived(uint chromaDentMode);
    void ledStatusReceived(uint ledStatus);
    void burstLengthValueRecieved(uint burstLen);
    void indicateCommandStatus(QString title, QString text);

public slots:
    bool resetDevice();
    bool readFirmwareVersion();
    bool get64BitSerialNumber();
    bool saveConfiguration();
    bool setRawStream(RAW_STREAM rawStream);
    bool captureImage();

    bool setChromaDentMode(CHROMA_DENT_MODE chromaDentMode);
    bool getChromaDentMode();

    bool setLedStatus(LED_STATUS ledStat);
    bool getLedStatus();

    bool setBurstLength(uint burstLength);
    bool getBurstLength();

    bool setToDefaultValues();

};

#endif // ECAM_514USB_H
