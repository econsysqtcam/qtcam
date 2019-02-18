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
#define V4L2_CID_XU_DEWARPING		    0x0A
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
        HDR_2X = 0x02,
	HDR_NIGHTMODE = 0x03
    };
    Q_ENUMS(hdrModes) 

    enum dewarpModes{
        DEWARP_OFF = 0x00,
        DEWARP_ON = 0x01,
    };
    Q_ENUMS(dewarpModes)  


signals:    
    void bitrateValueReceived(uint queryType, uint bitrateValue);
    void qFactorReceived(uint queryType, uint qFactorValue);
    void h264QualityReceived(uint queryType, uint qualityValue);
    void hdrModeReceived(uint queryType, uint hdrValue);
    void noiseReductionValueReceived(uint queryType, uint noiseReductionValue);
    void dewarpModeReceived(uint queryType, uint dewarpValue);
    void titleTextChanged(QString _title, QString _text);

public slots:

    bool setBitrate(QString bitrateValue);
    bool getBitrate(uint queryType);

    bool setQFactor(QString qFactorValue);
    bool getQFactor(uint queryType);

    bool setH264Quality(QString h264Quality);
    bool getH264Quality(uint queryType);

    bool setHDRMode(QString hdrMode);
    bool getHDRMode(uint queryType);

    bool setNoiseReductionValue(QString noiseReduceValue);
    bool getNoiseReductionValue(uint queryType);

    bool getDewarpMode(uint queryType);
    bool setDewarpMode(QString dewarpMode);

    bool getFirmwareVersion();

    bool setDefault();


    void initUVCExtensionUnit(Videostreaming  *vobj);
    void deInitUVCExtensionUnit();
};

#endif // H264CAMERA_H
