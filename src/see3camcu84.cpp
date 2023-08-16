/*
 * see3camcu84.cpp -- Handling special feature of SEE3CAM_CU84 camera
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

#include "see3camcu84.h"

SEE3CAM_CU84::SEE3CAM_CU84()
{

}

/**
 * @brief SEE3CAM_CU84::setSpecialMode - Setting special mode in the camera
 * @param specialMode - What special mode need to set
 * @return true/false
 */
bool SEE3CAM_CU84::setSpecialMode(specialModes  specialMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = SET_SPECIAL_EFFECT_SEE3CAM_CU84; /* set special mode command  */
    g_out_packet_buf[4] = specialMode; /* pass special mode value */
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_SPECIAL_EFFECT_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getSpecialMode
 * @return true/false
 */
bool SEE3CAM_CU84::getSpecialMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = GET_SPECIAL_EFFECT_SEE3CAM_CU84; /* get special mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL)
        {
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84) &&
                (g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84) &&
                (g_in_packet_buf[2] == GET_SPECIAL_EFFECT_SEE3CAM_CU84) &&
                (g_in_packet_buf[7] == GET_SUCCESS)){
                emit specialModeReceived(g_in_packet_buf[3]);
                return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::setDenoiseValue(uint deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = SET_DENOISE_CONTROL_SEE3CAM_CU84; /* set denoise control code */
    g_out_packet_buf[4] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_DENOISE_CONTROL_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = GET_DENOISE_CONTROL_SEE3CAM_CU84; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == GET_DENOISE_CONTROL_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit denoiseValueReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU84::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = SET_AUTOEXP_ROI_MODE_SEE3CAM_CU84; /* set Auto exposure ROI mode command  */
    g_out_packet_buf[4] = see3camAutoexpROIMode; /* ROI mode which is need to set */

    if(see3camAutoexpROIMode == MANUAL_ROI){
        g_out_packet_buf[5] = outputXCord;
        g_out_packet_buf[6] = outputYCord;
        g_out_packet_buf[7] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_AUTOEXP_ROI_MODE_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_CU84::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::getAutoExpROIModeAndWindowSize(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = GET_AUTOEXP_ROI_MODE_SEE3CAM_CU84; /* get exposure ROI mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == GET_AUTOEXP_ROI_MODE_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit roiAutoExpMode(g_in_packet_buf[3], g_in_packet_buf[6]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU84::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = SET_BURST_LENGTH_SEE3CAM_CU84; /* set burst length command */
    g_out_packet_buf[4] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_BURST_LENGTH_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = GET_BURST_LENGTH_SEE3CAM_CU84; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == GET_BURST_LENGTH_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit burstLengthValueRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/*
 * @brief SEE3CAM_CU84::setOrientation - Setting orientation - set horizontal/vertical/Both
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_CU84::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
    g_out_packet_buf[3] = SET_ORIENTATION_SEE3CAM_CU84; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = BOTH; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[4] = HORIZONTAL; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = VERTICAL; /* vertical flip only mode */
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_ORIENTATION_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[3] = GET_ORIENTATION_SEE3CAM_CU84; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == GET_ORIENTATION_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit flipMirrorModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU84::setExposureCompensation - To set exposure compensation
 * exposureCompValue - exposure value to be set
 * @return true/false
 */
bool SEE3CAM_CU84::setExposureCompensation(unsigned int exposureCompValue)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[3] = SET_EXPOSURE_COMPENSATION_SEE3CAM_CU84;
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set Exposure Compensation Value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_EXPOSURE_COMPENSATION_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getExposureCompensation - To get exposure compensation from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::getExposureCompensation()
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
   g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
   g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
   g_out_packet_buf[3] = GET_EXPOSURE_COMPENSATION_SEE3CAM_CU84; /* get exposure compensation code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[7] == GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
           g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
           g_in_packet_buf[2] == GET_EXPOSURE_COMPENSATION_SEE3CAM_CU84 &&
           g_in_packet_buf[7] == GET_SUCCESS) {
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
 * @brief SEE3CAM_CU84::setFrameRateCtrlValue - To set the specified frame rate
 * @return true/false
 */
bool SEE3CAM_CU84::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[3] = SET_FRAMERATE_CONTROL_SEE3CAM_CU84; /* set framerate control code */
    g_out_packet_buf[4] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_FRAMERATE_CONTROL_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getFrameRateCtrlValue - To get the current frame rate from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU84::getFrameRateCtrlValue()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
   g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_lsb */
   g_out_packet_buf[3] = GET_FRAMERATE_CONTROL_SEE3CAM_CU84; /* get frame rate code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[7] == GET_FAIL) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
           g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
           g_in_packet_buf[2] == GET_FRAMERATE_CONTROL_SEE3CAM_CU84 &&
           g_in_packet_buf[7] == GET_SUCCESS) {
           emit minimumFramesReceived(g_in_packet_buf[4]); /*To get minimum frame rate supported*/
           emit maximumFramesReceived(g_in_packet_buf[5]); /*To get maximum frame rate supported*/
           emit frameRateCtrlValueRecieved(g_in_packet_buf[3]); /*To get current frame rate*/
           return true;
       }
   }
   return false;
}


/**
 * @brief SEE3CAM_CU84::setAntiFlickerMode - setting anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - auto/50hz/60hz]
 * @return true/false
 */
bool SEE3CAM_CU84::setAntiFlickerMode(antiFlickerDetection antiFlickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[3] = SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU84; /* set anti flicker command  */
    g_out_packet_buf[4] = antiFlickerMode; /* anti flicker mode to set */
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU84::getAntiFlickerMode - get current anti flicker mode from camera
 * @return - true/false
 */
bool SEE3CAM_CU84::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[3] = GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU84; /* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit antiFlickerModeRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}




/**
 * @brief SEE3CAM_CU84::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_CU84::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84; /* camera_control_id_msb */
    g_out_packet_buf[3] = SET_DEFAULT_SEE3CAM_CU84; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[7]==SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_MSB_SEE3CAM_CU84 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_LSB_SEE3CAM_CU84 &&
            g_in_packet_buf[2] == SET_DEFAULT_SEE3CAM_CU84 &&
            g_in_packet_buf[7] == SET_SUCCESS){
            return true;
        }
    }
    return false;
}

/*
 * @brief SEE3CAM_50CUGM::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_CU84::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
