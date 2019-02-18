
// Added by Sankari - 5 Oct 2018
/*
 * H264Camera.cpp -- Access the extension unit of h264 camera.
 * Copyright © 2015  e-con Systems India Pvt. Limited
 *
 * This file is part of Qtcam.
 *
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
 */

#include "h264camera.h"



H264Camera::H264Camera()
{
  gvobj = NULL;
}


H264Camera::~H264Camera()
{

}

/**
 * @brief H264Camera::initUVCExtensionUnit
 * @param vobj - pass video stream class object
 */
void H264Camera::initUVCExtensionUnit(Videostreaming *vobj){
   if(!vobj){
       return;
   }
   gvobj = vobj;
}

void H264Camera::deInitUVCExtensionUnit(){
    gvobj = NULL;
}

/**
 * @brief H264Camera::setBitrate - setting bitrate value
 * @param brightnessValue - brightness value to set
 * @return true/false
 */
bool H264Camera::setBitrate(QString bitrateValue){
    __u16 bitrate;
    bitrate =  bitrateValue.toUShort();
    return setCurrentValueCmd(V4L2_CID_XU_BITRATE, bitrate);
}


/**
 * @brief H264Camera::getBitrate
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
 * @return true/false
*/
bool H264Camera::getBitrate(uint queryType){
    __u8 outputBitrateVal;

    if(getValueCmd(V4L2_CID_XU_BITRATE, queryType, outputBitrateVal)){
	uint bitRateValue = outputBitrateVal;
        emit bitrateValueReceived(queryType, bitRateValue);
        return true;
    }
    else
        return false;
}

/**
 * @brief H264Camera::setQFactor - setting QFactor value
 * @param qFactorValue - brightness value to set
 * @return true/false
 */
bool H264Camera::setQFactor(QString qFactorValue){
    __u16 qFactor;
    qFactor = qFactorValue.toUShort();   
    return setCurrentValueCmd(V4L2_CID_XU_QFACTOR, qFactor);
}


/**
 * @brief H264Camera::getQFactor
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
* @return true/false
 */
bool H264Camera::getQFactor(uint queryType){
    __u8 outputQFactorVal;

    if(getValueCmd(V4L2_CID_XU_QFACTOR, queryType, outputQFactorVal)){
	uint qFactorValue = outputQFactorVal;	
        emit qFactorReceived(queryType, qFactorValue);
        return true;
    }
    else
        return false;
}


/**
 * @brief H264Camera::seth264Quality - setting jpeg quality value
 * @param qFactorValue - h264quality value to set
 * @return true/false
 */
bool H264Camera::setH264Quality(QString h264Quality){
    __u16 h264Qual;

    h264Qual = h264Quality.toUShort();
    return setCurrentValueCmd(V4L2_CID_XU_H264QUALITY, h264Qual);
}


/**
 * @brief H264Camera::getH264Quality
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
* @return true/false
 */
bool H264Camera::getH264Quality(uint queryType){
    __u8 outputQualityVal;

    if(getValueCmd(V4L2_CID_XU_H264QUALITY, queryType, outputQualityVal)){
        uint qualityValue = outputQualityVal;
        emit h264QualityReceived(queryType, qualityValue);
        return true;
    }
    else
        return false;
}


/**
 * @brief H264Camera::setQFactor - setting QFactor value
 * @param qFactorValue - brightness value to set
 * @return true/false
 */
bool H264Camera::setNoiseReductionValue(QString noiseReduceValue){
    __u16 noiseReduction;

    noiseReduction = noiseReduceValue.toUShort();
    return setCurrentValueCmd(V4L2_CID_XU_NOISE_REDUCION, noiseReduction);
}


/**
 * @brief H264Camera::setNoiseReductionValue
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
* @return true/false
 */
bool H264Camera::getNoiseReductionValue(uint queryType){
    __u8 outputNoiseReductionVal;

    if(getValueCmd(V4L2_CID_XU_NOISE_REDUCION, queryType, outputNoiseReductionVal)){
	uint noiseReductionValue = outputNoiseReductionVal;	
        emit noiseReductionValueReceived(queryType, noiseReductionValue);
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief H264Camera::setHDRMode - setting HDR value
 * @param hdrMode - hdr mode to set
 * @return true/false
 */
bool H264Camera::setHDRMode(QString hdrMode){
    __u16 hdr;

    hdr = hdrMode.toUShort();
    return setCurrentValueCmd(V4L2_CID_XU_HDR, hdr);
}

/**
 * @brief H264Camera::getHDRMode
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
* @return true/false
 */
bool H264Camera::getHDRMode(uint queryType){
    __u8 outputHDRVal;

    if(getValueCmd(V4L2_CID_XU_HDR, queryType, outputHDRVal)){
	uint hdrValue = outputHDRVal;	
        emit hdrModeReceived(queryType, hdrValue);
        return true;
    }
    else
        return false;
}

/**
 * @brief H264Camera::setDewarpMode - setting dewarpMode mode value
 * @param dewarpMode - dewarp mode to set
 * @return true/false
 */
bool H264Camera::setDewarpMode(QString dewarpMode){
    __u16 dewarp;

    dewarp = dewarpMode.toUShort();
    return setCurrentValueCmd(V4L2_CID_XU_DEWARPING, dewarp);
}

/**
 * @brief H264Camera::getDewarpMode
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
* @return true/false
 */
bool H264Camera::getDewarpMode(uint queryType){
    __u8 outputDewarpMode;

    if(getValueCmd(V4L2_CID_XU_HDR, queryType, outputDewarpMode)){
	uint dewarpValue = outputDewarpMode;	
        emit dewarpModeReceived(queryType, dewarpValue);
        return true;
    }
    else
        return false;
}


/**
 * @brief H264Camera::setdefault - setting default value
 * @param qFactorValue - hdr mode to set
 * @return true/false
 */
bool H264Camera::setDefault(){
     if(!gvobj){
        return false;
    }

    struct uvc_xu_control_query xquery;
    __u16 size = 0;

    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = V4L2_CID_XU_DEFAULT;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = (__u8 *)&size;

    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }

   // To set the current value to the camera
    xquery.query = UVC_SET_CUR;
    xquery.size = size;
    xquery.selector = V4L2_CID_XU_DEFAULT;
    xquery.unit = EXTENSION_UNIT_ID;   

    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }

   return true;

}



/**
 * @brief H264Camera::getFirmwareVersion - getting SDK and git version
 * @return true/false
 */
bool H264Camera::getFirmwareVersion(){
    __u8 outputValues[4];
    _title = "Firmware Version";
    if(!getFirmwareVersionValueCmd(V4L2_CID_XU_FW_VERSION, UVC_GET_CUR, 4, outputValues)){
        return false;
    }
    _text.clear();

    _text.append("Firmware Version: ");
    _text.append(QString::number(outputValues[0]).append(".").append(QString::number(outputValues[1])));
    _text.append(".");
   
    _text.append(QString::number(outputValues[2]).append(".").append(QString::number(outputValues[3])));

    emit titleTextChanged(_title, _text);
    return true;
}

/**
 * @brief H264Camera::setCurrentValueCmd - set current value from camera
 * @param controlId - control id
 * @param setVal- value to set
 * @return true/false
 */
bool H264Camera::setCurrentValueCmd(__u8 controlId, uint16_t setVal){
    if(!gvobj){
        return false;
    }

    struct uvc_xu_control_query xquery;
    __u16 size = 0;

    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = (__u8 *)&size;

    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }

   // To set the current value to the camera
    xquery.query = UVC_SET_CUR;
    xquery.size = size;
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;    
    xquery.data = (uint8_t *)&setVal;   


    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }

   return true;
}


/**
 * @brief H264Camera::getValueCmd - get Current value command multiple values
 * @param controlId - control id
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN/ UVC_GET_MAX/ UVC_GET_RES
 * @param outputValues - pointer to hold outputValues
 * @return true/false
 */
bool H264Camera::getValueCmd(__u8 controlId,  __u8 queryType, __u8 &outputValues){

    if(!gvobj){
        return false;
    }

   /*if(!outputValues){
        return false;
    }*/

    struct uvc_xu_control_query xquery;
    __u16 size = 0;
    
    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    
    xquery.data = (__u8 *)&size;


    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }
    memset(&xquery, 0, sizeof(xquery));

    // get value from camera
    xquery.query = queryType;
    xquery.size = size;
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = (unsigned char *)(&outputValues);

    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }

    __u8 *val = xquery.data;

   outputValues = *val;

    return true;
}

/**
 * @brief H264Camera::getValueCmd - get Current value command multiple values
 * @param controlId - control id
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN/ UVC_GET_MAX/ UVC_GET_RES
 * @param outputValues - pointer to hold outputValues
 * @return true/false
 */
bool H264Camera::getFirmwareVersionValueCmd(__u8 controlId,  __u8 queryType, uint numberOfValues, __u8 *outputValues){
      if(!gvobj){
        return false;
    }

    if(!outputValues){
        return false;
    }

    struct uvc_xu_control_query xquery;
    __u16 size = 0;
    
    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    
    xquery.data = (__u8 *)&size;


    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }

    memset(&xquery, 0, sizeof(xquery));

    // get value from camera
    xquery.query = queryType;
    xquery.size = size;
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = outputValues;

    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }


    __u8 *val = xquery.data;

    for(uint i=0; i<numberOfValues; i++){
        outputValues[i] = val[i];
    }
}


