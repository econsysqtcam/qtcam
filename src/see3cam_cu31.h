#ifndef SEE3CAM_CU31_H
#define SEE3CAM_CU31_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_ID_1_SEE3CAM_CU31        0xC0
#define CAMERA_CONTROL_ID_2_SEE3CAM_CU31        0x0A

#define GET_ORIENTATION_SEE3CAM_CU31            0x01
#define SET_ORIENTATION_SEE3CAM_CU31            0x02

#define GET_RNR_STATUS_SEE3CAM_CU31             0x03
#define SET_RNR_STATUS_SEE3CAM_CU31             0x04

#define GET_TRIGGER_MODE_SEE3CAM_CU31           0x05
#define SET_TRIGGER_MODE_SEE3CAM_CU31           0x06

#define GET_AE_CONVERGENCE_SPEED_SEE3CAM_CU31   0x07
#define SET_AE_CONVERGENCE_SPEED_SEE3CAM_CU31   0x08

#define GET_AE_HOLD_SEE3CAM_CU31                0x09
#define SET_AE_HOLD_SEE3CAM_CU31                0x0A

#define GET_AWB_STATUS_SEE3CAM_CU31             0x0B
#define SET_AWB_STATUS_SEE3CAM_CU31             0x0C

#define READ_TEMPERATURE_SEE3CAM_CU31           0x0D

#define SET_DEFAULT_SEE3CAM_CU31                0xFF

#define GET_64BIT_UNIQUE_ID_1                   0x41
#define GET_64BIT_UNIQUE_ID_2                   0x01

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

signals:
    void flipMirrorModeChanged(uint flipMirrorModeValues);
    void currentRawNoiseDeductionStatus(uint status);
    void currentTriggerMode(uint mode);
    void currentAEConvergenceSpeed(uint currentSpeed);
    void currentAutoExposureStatus(uint status);
    void currentAutoWhiteBalanceStatus(uint status);
    void currentTemperature(float temperature);
    void titleTextChanged(QString _title,QString _text);

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

    bool readTemperature();

    bool setToDefaultValues();

    bool get64BitSerialNumber();

};

#endif // SEE3CAM_CU31_H
