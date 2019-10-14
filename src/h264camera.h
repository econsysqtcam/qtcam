#ifndef H264CAMERA_H
#define H264CAMERA_H

// Added by Sankari - 5 Oct 2018
#include <QObject>
#include <QDebug>
#include "videostreaming.h"
#include <errno.h>

#define UVC_RC_UNDEFINED                    0x00
#define UVC_SET_CUR                         0x01

#define UVC_GET_LEN                         0x85
#define UVC_GET_INFO                        0x86

#define V4L2_CID_XU_QFACTOR                 0x01 //0x0A046DF2
#define V4L2_CID_XU_BITRATE                 0x02 // 0x0A046DF3
#define V4L2_CID_XU_GAINTYPE                0x04
#define V4L2_CID_XU_HDR                     0x03 // 0x0A046DF4
#define V4L2_CID_XU_FW_VERSION               0x07  //0x0A046DF8
#define V4L2_CID_XU_NOISE_REDUCION          0x08  //0x0A046DF9
#define V4L2_CID_XU_DEFAULT                 0x06
#define V4L2_CID_XU_H264QUALITY             0x09
#define V4L2_CID_XU_DEWARPING       	    0x0A
#define V4L2_CID_XU_EXPOSURE_ROI_MODE            0x0C
#define V4L2_CID_XU_EXPOSURE_ROI_COORDINATES     0x0D
#define V4L2_CID_XU_EXPOSURE_ROI_WINSIZE         0x0E
#define V4L2_CID_XU_HFLIP                   0xF
#define V4L2_CID_XU_VFLIP                   0x10
#define V4L2_CID_XU_MIN                     0x00
#define V4L2_CID_XU_MAX                     0x01

#define EXTENSION_UNIT_ID                   3
#define XU_ROI_EXPOSURE_OFF                 0xC0
#define XU_ROI_EXPOSURE_ON                  0xD0
#define XU_ROI_EXPOSURE_WINDOW_SIZE         0xE0

#define EXTENSION_UNIT_ID                   3

class H264Camera: public QObject, public v4l2
{
    Q_OBJECT

private:
    /**
     * @brief UVCExtCx3SNI::setCurrentValueCmd - set current value from camera
     * @param controlId - control id
     * @param setVal[] - value(s) to set
     * @return true/false
     */
    // set the current value into the camera.
    bool setCurrentValueCmd(__u8 controlId, uint16_t setVal);


    bool setArrayOfValues(__u8 controlId, uint32_t setVal);

    /**
     * @brief UVCExtCx3SNI::getValueCmd - get Current value command multiple values
     * @param controlId - control id
     * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN/ UVC_GET_MAX/ UVC_GET_RES
     * @param outputValues - reference to hold outputValues
     * @return true/false
     */
    bool getValueCmd(__u8 controlId, __u8 queryType, __u8 &outputValues);

    /**
     * @brief UVCExtCx3SNI::getFirmwareVersionValueCmd - get firmware version 
     * @param controlId - control id
     * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN/ UVC_GET_MAX/ UVC_GET_RES
     * @param outputValues - pointer to hold outputValues
     * @return true/false
     */
    bool getFirmwareVersionValueCmd(__u8 controlId,  __u8 queryType, uint numberOfValues, __u8 *outputValues);

public:
    Videostreaming *gvobj;

    QString _title;
    QString _text;

    H264Camera();
   ~H264Camera();

    enum getValueQueryType{
        UVC_GET_CUR = 0x81,
        UVC_GET_DEF = 0x87,
        UVC_GET_MIN = 0x82,
        UVC_GET_MAX = 0x83,
        UVC_GET_RES = 0x84
    };
    Q_ENUMS(getValueQueryType)

    enum hdrModes{
        HDR_OFF = 0x00,
        HDR_1X = 0x01,
        HDR_2X = 0x02
    };
    Q_ENUMS(hdrModes) 

    enum dewarpModes{
        DEWARP_OFF = 0x00,
        DEWARP_ON = 0x01,
    };
    Q_ENUMS(dewarpModes)

    enum gainModes{
        GAIN_MIN = 0x00,
        GAIN_MAX = 0x01,
    };
    Q_ENUMS(gainModes)

    enum flipHorizontal{
        HFLIP_MIN =0x00,
        HFLIP_MAX = 0x01,
    };
    Q_ENUMS(flipHorizontal)

    enum flipVertical{
        VFLIP_MIN =0x00,
        VFLIP_MAX = 0x01,
    };
    Q_ENUMS(flipVertical)

    enum camROIAutoExpMode {
        ROI_FULL = 0x00,
        ROI_MANUAL = 0x01
    };
    Q_ENUMS(camROIAutoExpMode)


signals:    
    void bitrateValueReceived(uint queryType, uint bitrateValue);
    void qFactorReceived(uint queryType, uint qFactorValue);
    void h264QualityReceived(uint queryType, uint qualityValue);
    void hdrModeReceived(uint queryType, uint hdrValue);
    void gainModeReceived(uint queryType, uint gainValue);
    void noiseReductionValueReceived(uint queryType, uint noiseReductionValue);
    void dewarpModeReceived(uint queryType, uint dewarpValue);
    void roiModeReceived(uint queryType, uint expMode);
    void roiWindowSizeReceived(uint queryType, uint windowSize);
    void titleTextChanged(QString _title, QString _text);
    void flipHorizontalValue(uint queryType ,uint flipValue);
    void flipVerticalValue(uint queryType ,uint flipValue);
    void disableVFlipControl();
    void disableHFlipControl();

public slots:

    bool setBitrate(QString bitrateValue);
    bool getBitrate(uint queryType);

    bool setQFactor(QString qFactorValue);
    bool getQFactor(uint queryType);

    bool setH264Quality(QString h264Quality);
    bool getH264Quality(uint queryType);

    bool setHDRMode(QString hdrMode);
    bool getHDRMode(uint queryType);

    bool setGainMode(QString gainMode);
    bool getGainMode(uint queryType);

    bool setNoiseReductionValue(QString noiseReduceValue);
    bool getNoiseReductionValue(uint queryType);

    bool getDewarpMode(uint queryType);
    bool setDewarpMode(QString dewarpMode);

    bool setROIAutoExposureMode(QString autoexpROIMode);
    bool setROIExposureWindowSize(QString windowSize);
    bool setROIExposureCoordinates(uint previewRenderWidth, uint previewRenderHeight, uint videoResolutionWidth, uint videoResolutionHeight, uint xCord, uint yCord);

    bool getROIAutoExposureMode(uint queryType);
    bool getROIExposureWindowSize(uint queryType);

    bool setHorizontalFlip(bool hFlipCheck);
    bool getHorizontalFlip(uint queryType);

    bool setVerticalFlip(bool vFlipCheck);
    bool getVerticalFlip(uint queryType);

    bool getFirmwareVersion();

    bool setDefault();


    void initUVCExtensionUnit(Videostreaming  *vobj);
    void deInitUVCExtensionUnit();
};

#endif // H264CAMERA_H
