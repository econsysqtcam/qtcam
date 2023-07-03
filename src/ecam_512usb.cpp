/*
 * see3cam_cu83.cpp -- Handling special feature of e-cam_512USB camera
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
#include "ecam_512usb.h"

ECAM_512USB::ECAM_512USB()
{

}



/**
 * @brief ECAM_512USB::setSpecialMode - Setting special mode in the camera
 * @param specialMode - What special mode need to set
 * @return true/false
 */
bool ECAM_512USB::setSpecialMode(SPECIAL_EFFECTS  specialMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_SPECIAL_EFFECT_ECAM512USB; /* set special effect command */
    g_out_packet_buf[4] = specialMode; /* Passing special mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_SPECIAL_EFFECT_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::getSpecialMode
 * @return true/false
 */
bool ECAM_512USB::getSpecialMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_SPECIAL_EFFECT_ECAM512USB; /* get special mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB) &&
                (g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB) &&
                (g_in_packet_buf[2] == GET_SPECIAL_EFFECT_ECAM512USB) &&
                (g_in_packet_buf[6] == GET_SUCCESS)){
            emit specialModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM_512USB::setDenoiseValue - setting denoise value to the camera
 * @param denoiseVal - denoise value
 * return true - success /false - failure
 */
bool ECAM_512USB::setDenoiseValue(uint denoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_DENOISE_ECAM512USB; /* set denoise value command */
    g_out_packet_buf[4] = denoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_DENOISE_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_DENOISE_ECAM512USB; /* get denoise value command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == GET_DENOISE_ECAM512USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit denoiseValueReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief ECAM_512USB::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool ECAM_512USB::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //((Input - InputLow) / (InputHigh - InputLow)) * (OutputHigh - OutputLow) + OutputLow // map resolution width and height -  0 to 255
    double outputLow = 0;
    double outputHigh = 255;
    double inputXLow = 0;
    double inputXHigh = vidResolnWidth-1;
    double inputXCord = xCord;
    int outputXCord = ((inputXCord - inputXLow) / (inputXHigh - inputXLow)) * (outputHigh - outputLow) + outputLow;

    double inputYLow = 0;
    double inputYHigh = vidResolnHeight-1;
    double inputYCord = yCord;
    int outputYCord = ((inputYCord - inputYLow) / (inputYHigh - inputYLow)) * (outputHigh - outputLow) + outputLow;

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_AUTO_EXPOSURE_ROI_ECAM512USB; /* set exposure ROI command */
    g_out_packet_buf[4] = see3camAutoexpROIMode; /* exposure ROI mode to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[5] = outputXCord; // x cord
        g_out_packet_buf[6] = outputYCord; // y cord
        g_out_packet_buf[7] = winSize.toUInt(); // window size
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[15] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2]  == SET_AUTO_EXPOSURE_ROI_ECAM512USB &&
            g_in_packet_buf[15] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief ECAM_512USB::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool ECAM_512USB::getAutoExpROIModeAndWindowSize(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_AUTO_EXPOSURE_ROI_ECAM512USB; /* get exposure ROI mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2]  == GET_AUTO_EXPOSURE_ROI_ECAM512USB &&
            g_in_packet_buf[15] == GET_SUCCESS) {
            emit roiAutoExpMode(g_in_packet_buf[3], g_in_packet_buf[4], g_in_packet_buf[5], g_in_packet_buf[6]);
            return true;
        }
    }
    return false;
}
/**
 * @brief ECAM_512USB::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool ECAM_512USB::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_BURST_LENGTH_ECAM512USB; /* set burst length command */
    g_out_packet_buf[4] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_BURST_LENGTH_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_BURST_LENGTH_ECAM512USB; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == GET_BURST_LENGTH_ECAM512USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit burstLengthValueRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::setQFactorValue - setting QFactor value to the camera
 * @param QFactor - To set the QFactor value to the camera
 * return true - success /false - failure
 */
bool ECAM_512USB::setQFactorValue(uint QFactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_Q_FACTOR_VALUE_ECAM512USB;
    g_out_packet_buf[4] = QFactor;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_Q_FACTOR_VALUE_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::getQFactorValue - get QFactor value from camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getQFactorValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = GET_Q_FACTOR_VALUE_ECAM512USB;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == GET_Q_FACTOR_VALUE_ECAM512USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit qFactorValueReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/*
 * @brief ECAM_512USB::setOrientation - Setting orientation - set horizontal/vertical/Both/Normal
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool ECAM_512USB::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_ORIENTATION_ECAM512USB; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = ROATATE_180; /* both flip enable (i.e) Rotate 180*/
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[4] = HORIZONTAL; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = VERTICAL; /* vertical flip only mode */
    }else if(!horzModeSel && !vertiModeSel){
        g_out_packet_buf[4] = NORMAL; /* Normal mode */
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_ORIENTATION_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM_512USB::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[2] = GET_ORIENTATION_ECAM512USB; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == GET_ORIENTATION_ECAM512USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief ECAM_512USB::setFaceDetectionRect
 * @return true/false
 */
bool ECAM_512USB::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
     u_int8_t faceDetectMode, embedDataMode;
    if(enableFaceDetectRect)
        faceDetectMode = ENABLE_FACE_DETECTION; /* enable face detect */
    else
        faceDetectMode = DISBLE_FACE_DETECTION; /* disable face detect */

//    if(embedData)
//        embedDataMode = ENABLE_EMBED_DATA_FSCAM_CU135; /* enable embed data */
//    else
//        embedDataMode = DISABLE_EMBED_DATA_FSCAM_CU135; /* disable embed data */

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_FACE_DETECTION_ECAM512USB; /* set face detect mode command */

    g_out_packet_buf[4] = faceDetectMode; /* enable/disable face Detect */
    g_out_packet_buf[5] = embedData;      /* enable/disable embed data */
    g_out_packet_buf[6] = overlayRect;    /* enable/disable overlay rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_FACE_DETECTION_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



bool ECAM_512USB::getFaceDetectMode(){
   uint faceDetectMode, faceDetectEmbedDataValue, faceOverlayRect;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
   g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
   g_out_packet_buf[3] = GET_FACE_DETECTION_ECAM512USB; /* get face detect mode command */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6] == GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
           g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
           g_in_packet_buf[2] == GET_FACE_DETECTION_ECAM512USB &&
           g_in_packet_buf[6] == GET_SUCCESS) {

           faceDetectMode           = g_in_packet_buf[3];
           faceDetectEmbedDataValue = g_in_packet_buf[4];
           faceOverlayRect          = g_in_packet_buf[5];
           emit faceDetectModeValue(faceDetectMode, faceDetectEmbedDataValue, faceOverlayRect);

           return true;
       }
   }
   return false;

}



bool ECAM_512USB::setSmileDetection(bool enableSmileDetect, bool embedData){

    u_int8_t smileDetectMode;

    if(enableSmileDetect)
        smileDetectMode = ENABLE_SMILE_DETECTION; /* enable smile detect */
    else
        smileDetectMode = DISBLE_SMILE_DETECTION; /* disable smile detect */

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
    g_out_packet_buf[3] = SET_SMILE_DETECTION_ECAM512USB; /* set smile detection command */
    g_out_packet_buf[4] = smileDetectMode; /* disable smile detect */
    g_out_packet_buf[5] = embedData; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_SMILE_DETECTION_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

bool ECAM_512USB::getSmileDetectMode(){

   uint smileDetectMode, smileDetectEmbedDataValue;

   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera id_1 */
   g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera id_2 */
   g_out_packet_buf[3] = GET_SMILE_DETECTION_ECAM512USB; /* set smile detection command */


   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6] == GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
           g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
           g_in_packet_buf[2] == GET_SMILE_DETECTION_ECAM512USB &&
           g_in_packet_buf[6] == GET_SUCCESS) {

           smileDetectMode           = g_in_packet_buf[3];
           smileDetectEmbedDataValue = g_in_packet_buf[4];

           emit smileDetectModeValue(smileDetectMode, smileDetectEmbedDataValue);
           return true;
       }
   }
   return false;
}



/**
 * @brief ECAM_512USB::setExposureCompensation - To set exposure compensation
 * exposureCompValue - exposure value to be set
 * @return true/false
 */
bool ECAM_512USB::setExposureCompensation(unsigned int exposureCompValue)
{
    if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure compensation value is invalid");
        return false;
    }


    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
    g_out_packet_buf[3] = SET_EXPOSURE_COMPENSATION_ECAM512USB;
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set Exposure Compensation Value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_EXPOSURE_COMPENSATION_ECAM512USB &&
            g_in_packet_buf[15] == SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::getExposureCompensation - To get exposure compensation from the camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getExposureCompensation()
{
   uint expComp;
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
   g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
   g_out_packet_buf[3] = GET_EXPOSURE_COMPENSATION_ECAM512USB; /* get exposure compensation code */


   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[15] == GET_FAIL){
           return false;
       }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
           g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
           g_in_packet_buf[2] == GET_EXPOSURE_COMPENSATION_ECAM512USB &&
           g_in_packet_buf[15] == GET_SUCCESS) {
           expComp = (((u_int8_t)g_in_packet_buf[3]) << 24)
                               | (((u_int8_t)g_in_packet_buf[4]) << 16)
                               | (((u_int8_t)g_in_packet_buf[5]) << 8)
                               | (((u_int8_t)g_in_packet_buf[6]) << 0);
           emit exposureCompValueRecieved(expComp);
           return true;
       }
   }
   return false;
}


/**
 * @brief ECAM_512USB::setFrameRateCtrlValue - To set the specified frame rate
 * @return true/false
 */
bool ECAM_512USB::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
    g_out_packet_buf[3] = SET_FRAME_RATE_CONTROL_ECAM512USB; /* set framerate control code */
    g_out_packet_buf[4] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_FRAME_RATE_CONTROL_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM_512USB::getFrameRateCtrlValue - To get the current frame rate from the camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getFrameRateCtrlValue()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
   g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
   g_out_packet_buf[3] = GET_FRAME_RATE_CONTROL_ECAM512USB; /* get frame rate code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
           g_in_packet_buf[1] == CAMERA_CONTROL_ID1_ECAM512USB &&
           g_in_packet_buf[2] == GET_FRAME_RATE_CONTROL_ECAM512USB &&
           g_in_packet_buf[6] == GET_SUCCESS) {
           emit frameRateCtrlValueRecieved(g_in_packet_buf[2]); /*To get current frame rate*/
           return true;
       }
   }
   return false;
}



/**
 * @brief ECAM_512USB::setFlashMode - setting flash mode to the camera
 * return true - success /false - failure
 */
bool ECAM_512USB::setFlashMode(FLASH_MODE flashMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
    g_out_packet_buf[3] = SET_FLASH_MODE_ECAM512USB;/* set flash status command code */
    g_out_packet_buf[4] = flashMode; /* set flash state [Disable/Torch/storbe] */

    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_FLASH_MODE_ECAM512USB  &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief ECAM_512USB::getFlashMode - getting flash state from the camera
 * return true - success /false - failure
 */
bool ECAM_512USB::getFlashMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
    g_out_packet_buf[3] = GET_FLASH_MODE_ECAM512USB; /* get flash mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == GET_FLASH_MODE_ECAM512USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flashModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief ECAM_512USB::setAntiFlickerMode - setting anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - Auto/50hz/60hz/Disable]
 * @return true/false
 */
bool ECAM_512USB::setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
    g_out_packet_buf[3] = SET_ANTI_FLICKER_MODE_ECAM512USB; /* anti flicker mode to set */
    g_out_packet_buf[4] = antiFlickerMode; /* anti flicker value to set */


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_ANTI_FLICKER_MODE_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM_512USB::getAntiFlickerMode - get current anti flicker mode from camera
 * @return - true/false
 */
bool ECAM_512USB::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* camera control id_2 */
    g_out_packet_buf[3] = GET_ANTI_FLICKER_MODE_ECAM512USB; /* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == GET_ANTI_FLICKER_MODE_ECAM512USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief ECAM_512USB::storeSettings - set all the values to default in camera
 * @return true/false
 */
bool ECAM_512USB::storeSettings()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* set camera control id_2 */
    g_out_packet_buf[3] = STORE_SETTINGS_ECAM512USB; /* Restore default */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure","Failed to store settings");
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == STORE_SETTINGS_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success","Settings stored successfully!");
            return true;
        }
    }
    return false;
}

/**
 * @brief ECAM_512USB::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool ECAM_512USB::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_ECAM512USB; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_ECAM512USB; /* set camera control id_2 */
    g_out_packet_buf[3] = SET_TO_DEFAULT_ECAM512USB; /* Restore default */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure","Failed to restore default values");
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_ECAM512USB &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_ECAM512USB &&
            g_in_packet_buf[2] == SET_TO_DEFAULT_ECAM512USB &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success","Default Value Restored");
            return true;
        }
    }
    return false;
}

/*
 *
 * @brief ECAM_512USB::initializeBuffers - Initialize input and output buffers
 */
void ECAM_512USB::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
