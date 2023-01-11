#ifndef SEE3CAM_27CUG_H
#define SEE3CAM_27CUG_H

#include <QObject>
#include <QString>
#include "uvccamera.h"

#define CAMERA_CONTROL_SEE3CAM_27CUG       0xBA

#define GET_STREAM_MODE_SEE3CAM_27CUG      0x01
#define SET_STREAM_MODE_SEE3CAM_27CUG      0x02

#define GET_STROBE_MODE_SEE3CAM_27CUG      0x03
#define SET_STROBE_MODE_SEE3CAM_27CUG      0x04

#define GET_CAMERA_MODE_SEE3CAM_27CUG      0x05
#define SET_CAMERA_MODE_SEE3CAM_27CUG      0x06

#define GET_ORIENTATION_SEE3CAM_27CUG      0x07
#define SET_ORIENTATION_SEE3CAM_27CUG      0x08

#define GET_BRIGHTNESS_FOR_IR              0x09
#define SET_BRIGHTNESS_FOR_IR              0x0A

#define GET_CONTRAST_FOR_IR                0x0B
#define SET_CONTRAST_FOR_IR                0x0C

#define GET_GAIN_FOR_IR                    0x0D
#define SET_GAIN_FOR_IR                    0x0E

#define GET_MINIMUM_EXPOSURE_COMPENSATION  0x0F
#define SET_MINIMUM_EXPOSURE_COMPENSATION  0x10

#define GET_MAXIMUM_EXPOSURE_COMPENSATION  0x11
#define SET_MAXIMUM_EXPOSURE_COMPENSATION  0x12

#define GET27CUG_UNIQUE_ID_1               0x41
#define GET27CUG_UNIQUE_ID_2               0x01

#define SET_DEFAULT_SEE3CAM_27CUG          0xFF

#define EXPOSURECOMP_MIN 100
#define EXPOSURECOMP_MAX 1000000

class SEE3CAM_27CUG : public QObject
{
    Q_OBJECT

private:
    unsigned char g_out_packet_buf[BUFFER_LENGTH];
    unsigned char g_in_packet_buf[BUFFER_LENGTH];
    uvccamera uvc;

    void initializeBuffers();

public:
    SEE3CAM_27CUG();

    QString _title;
    QString _text;

    enum streamModes{
        MASTER  = 0x01,
        TRIGGER = 0x02
    };
    Q_ENUMS(streamModes)


    enum strobeMode{
        STROBE_ON  = 0x02,
        STROBE_OFF = 0x01
    };
    Q_ENUMS(strobeMode)

    enum cameraMode{
        RGB_IR_ENABLE  = 0x01,
        RGB            = 0x02,
        IR             = 0x03,
    };
    Q_ENUMS(cameraMode)


    enum orientation{
        NORMAL     = 0x01,
        VERTICAL   = 0x02,
        HORIZONTAL = 0x03,
        ROTATE_180 = 0x04
    };
    Q_ENUMS(orientation)

    signals:
        void flipMirrorModeChanged(uint flipMirrorModeValues);
        void strobeModeChanged(uint strobeModeValues);
        void streamModeChanged(uint streamModeValues);
        void cameraModeChanged(uint cameraModeValues);
        void brightnessChanged(uint brightness);
        void contrastChanged(uint contrast);
        void gainChanged(uint gain);
        void minimumExposureReceive(uint minExpValue);
        void maximumExposureReceive(uint maxExpValue);
        void serialNumber(QString serialNumber);

        void indicateExposureValueRangeFailure(QString title, QString text);
        void indicateCommandStatus(QString title, QString text);
        void titleTextChanged(QString _title,QString _text);

    public slots:
        bool setOrientation(bool horzModeSel, bool vertiModeSel);
        bool getOrientation();

        bool setCameraMode(cameraMode cameraModeValues);
        bool getCameraMode();

        bool setStreamMode(streamModes streamModeValues);
        bool getStreamMode();

        bool setStrobeMode(strobeMode strobeValues);
        bool getStrobeMode();

        bool setBrightnessForIR(uint brightness);
        bool getBrightnessForIR();

        bool setContrastForIR(uint contrast);
        bool getContrastForIR();

        bool setGainForIR(uint gain);
        bool getGainForIR();

        bool setMinimumExposureCompensation(unsigned int MinValue);
        bool getMinimumExposureCompensation();

        bool setMaximumExposureCompensation(unsigned int MaxValue);
        bool getMaximumExposureCompensation();

        bool get27CugSerialNumber();

        bool setToDefaultValues();
};
#endif



