#ifndef BARCODE_CAMERA_H
#define BARCODE_CAMERA_H

#include "uvccamera.h"
#include <QObject>

#define CAMERA_CONTROL_BARCODE              0xB6

#define SET_SUCCESS                         0x01
#define SET_FAILURE                         0x00

#define GET_SUCCESS                         0x01
#define GET_FAILURE                         0x00

#define SET_UART_BARCODE                    0x01
#define GET_UART_BARCODE                    0x02

#define SET_RED_LED_STATUS_BARCODE          0x03
#define GET_RED_LED_STATUS_BARCODE          0x04

#define SET_WHITE_LED_STATUS_BARCODE        0x05
#define GET_WHITE_LED_STATUS_BARCODE        0x06

#define SET_QRCODE_BARCODE                  0x10
#define GET_QRCODE_BARCODE                  0x11

#define SET_DATAMATRIX_BARCODE              0x12
#define GET_DATAMATRIX_BARCODE              0x13

#define SET_CODE128_BARCODE                 0x14
#define GET_CODE128_BARCODE                 0x15

#define SET_CODE39_BARCODE                  0x16
#define GET_CODE39_BARCODE                  0x17

#define SET_CODE93_BARCODE                  0x18
#define GET_CODE93_BARCODE                  0x19

#define SET_UPCA_BARCODE                    0x1A
#define GET_UPCA_BARCODE                    0x1B

#define SET_UPCE_BARCODE                    0x1C
#define GET_UPCE_BARCODE                    0x1D

#define SET_EAN13_BARCODE                   0x1E
#define GET_EAN13_BARCODE                   0x1F

#define SET_EAN8_BARCODE                    0x20
#define GET_EAN8_BARCODE                    0x21

#define SET_ROTATE90_BARCODE                0x30
#define GET_ROTATE90_BARCODE                0x31

#define SET_ROTATE180_BARCODE               0x32
#define GET_ROTATE180_BARCODE               0x33

#define SET_ROTATE270_BARCODE               0x34
#define GET_ROTATE270_BARCODE               0x35

#define SET_GAIN_BARCODE                    0x36
#define GET_GAIN_BARCODE                    0x37

#define SET_EXPOSURE_BARCODE                0x38
#define GET_EXPOSURE_BARCODE                0x39

#define SET_EXPOSURE_VALUE_BARCODE          0x3A
#define GET_EXPOSURE_VALUE_BARCODE          0x3B

#define SET_DEFAULT_BARCODE                 0x3C

#define SAVE_SETTINGS_BARCODE               0x3D

#define READ_FIRMWARE_VERSION_BARCODE       0x00

class BarcodeCamera : public QObject
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
    explicit BarcodeCamera(QObject *parent = 0);

    enum modes
    {
        UART = 0x00,
        SPI = 0x01
    };
    Q_ENUMS(modes)

    enum status
    {
        OFF = 0x00,
        ON = 0x01
    };
    Q_ENUMS(status)

    enum exposure
    {
        AUTO = 0x00,
        MANUAL = 0x01
    };
    Q_ENUMS(exposure)

signals:
        void uartSpiStatus(uint mode);
        void redLedStatus (uint status);
        void whiteLedStatus (uint status);
        void qrCodeValue(uint qrValue);
        void dataMatrixValue(uint dataMatrixValue);
        void code128Value(uint code128);
        void code93Value(uint code93);
        void code39Value(uint code39);
        void upcaValue(uint upca);
        void upceValue(uint upce);
        void ean13Value(uint ean13);
        void ean8Value(uint ean8);
        void rotate90Value(uint rotate90);
        void rotate180Value(uint rotate180);
        void rotate270Value(uint rotate270);
        void gainValue(uint gain);
        void exposureType(uint expType);
        void exposureValue(uint expVal);
        void titleTextChanged(QString _title,QString _text);

public slots:

         bool setUartSPI(uint mode);
         bool getUartSPI();

         bool setRedLedStatus(uint redLedStatus);
         bool getRedLedStatus();

         bool setWhiteLedStatus(uint whiteLedStatus);
         bool getWhiteLedStatus();

         bool setQRcode(uint qrCode);
         bool getQRcode();

         bool setDataMatrix(uint dataMatrix);
         bool getDataMatrix();

         bool setCode128(uint code128);
         bool getCode128();

         bool setCode39(uint code39);
         bool getCode39();

         bool setCode93(uint code93);
         bool getCode93();

         bool setUPCA(uint upca);
         bool getUPCA();

         bool setUPCE(uint upce);
         bool getUPCE();

         bool setEAN13(uint ean13);
         bool getEAN13();

         bool setEAN8(uint ean8);
         bool getEAN8();

         bool setRotate90(uint rotate90);
         bool getRotate90();

         bool setRotate180(uint rotate180);
         bool getRotate180();

         bool setRotate270(uint rotate270);
         bool getRotate270();

         bool setGain(uint gain);
         bool getGain();

         bool setExposure(uint exposure);
         bool getExposure();

         bool setExposureValue(uint exposure);
         bool getExposureValue();

         bool setToDefault();

         bool readFirmwareVersion();

         bool saveSettings();
};

#endif // BARCODE_CAMERA_H
