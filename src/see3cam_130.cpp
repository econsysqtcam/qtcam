/*
 * seecam_130.cpp -- Handling special feature of seecam130 camera
 * Copyright © 2024  e-con Systems India Pvt. Limited
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

#include "see3cam_130.h"

See3CAM_130::See3CAM_130(QObject *parent) :
    QObject(parent)
{
}


/**
 * @brief See3CAM_130::setSceneMode - Setting scene mode in the camera.
 * @ param - enum scene Mode
 * return true - success /false - failure
 */
bool See3CAM_130::setSceneMode(SCENE_MODES sceneMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENEMODE_130; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_SCENEMODE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getSceneMode - getting scene mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_130::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_SCENEMODE_130; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_SCENEMODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit sceneModeValueReceived(g_in_packet_buf[2]);

            return true;
        }
    }
    return false;
}




/**
 * @brief See3CAM_130::setEffectMode - Setting effect mode in the camera.
 * param - enum specialEffect
 * return true - success /false - failure
 */
bool See3CAM_130::setEffectMode(SPECIAL_EFFECTS specialEffect)
{
    // HID validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* Camera control id */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT_130; /* Set effect mode command */
    g_out_packet_buf[3] = specialEffect; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_SPECIAL_EFFECT_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getEffectMode - getting effect mode from the camera *
 * return true - success /false - failure
 */
bool See3CAM_130::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT_130; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_SPECIAL_EFFECT_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit effectModeValueReceived(g_in_packet_buf[2]);

            return true;
        }
    }
    return false;
}




/**
 * @brief See3CAM_130::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_130::setDenoiseValue(int deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_DENOISE_CONTROL &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_CONTROL; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_DENOISE_CONTROL &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit denoiseValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setAutoFocusMode - set auto focus mode in camera
 * @param afMode - enum Auto focus mode
 * return true - success /false - failure
 */
bool See3CAM_130::setAutoFocusMode(AUTO_FOCUS_MODE afMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_AF_MODE_130; /* set auto focus mode command  */
    g_out_packet_buf[3] = afMode; /* Set af mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_AF_MODE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getAutoFocusMode - get auto focus mode from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getAutoFocusMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_AF_MODE_130; /* get auto focus mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_AF_MODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit afModeValueReceived(g_in_packet_buf[2]);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setROIAutoFocus - Set ROI auto focus to camera
 * @param camROIAfMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_130::setROIAutoFocus(AUTO_FOCUS_ROI see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera control id for camera see3cam_130 */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_130; /* set Auto focus ROI mode command  */
    g_out_packet_buf[3] = see3camAfROIMode; /*  ROI mode which is need to set */

    if(see3camAfROIMode == AF_MANUAL_ROI){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_AF_ROI_MODE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getAutoFocusROIModeAndWindowSize - get ROI auto focus mode and window size
 * return true - success /false - failure
 */
bool See3CAM_130::getAutoFocusROIModeAndWindowSize(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_130; /* get AF ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_AF_ROI_MODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
                emit roiAfModeValueReceived(g_in_packet_buf[2], g_in_packet_buf[5]);
                return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_130::setROIAutoExposure(AUTO_EXPOSURE_ROI see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera control id for camera see3cam_130 */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_130; /* set Auto exposure ROI mode command  */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* ROI mode which is need to set */

    if(see3camAutoexpROIMode == AE_MANUAL_ROI){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_EXP_ROI_MODE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_130::getAutoExpROIModeAndWindowSize(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_130; /* get Auto exposure ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_EXP_ROI_MODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
                emit roiAutoExpModeValueReceived(g_in_packet_buf[2], g_in_packet_buf[5]);
                return true;
        }
    }
    return false;
}




/**
 * @brief See3CAM_130::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_130::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_130; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_BURST_LENGTH_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_130; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_BURST_LENGTH_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit burstLengthValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}





/**
 * @brief See3CAM_130::setiHDRMode - set iHDR mode in camera
 * @param iHDRMode - HDR mode
 * @param iHDRValue - value of iHDR for manual mode
 * return true - success /false - failure
 */
bool See3CAM_130::setiHDRMode(iHDR_MODE iHDRMode, uint iHDRValue){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = SET_HDR_MODE_130; /* set ihdr mode command code */
    g_out_packet_buf[3] = iHDRMode; /* iHdr Mode */
    g_out_packet_buf[4] = iHDRValue; /*iHDR manual value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==SET_HDR_MODE_130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getiHDRMode - get iHDR mode from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getiHDRMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_HDR_MODE_130; /* get hdr mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_HDR_MODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit hDRModeValueReceived(g_in_packet_buf[2], g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}





/**
 * @brief See3CAM_130::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_130::setQFactor(uint qFactor){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_130; /* set q factor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_Q_FACTOR_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_130; /* get q factor command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_Q_FACTOR_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            emit qFactorValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::enableDisableAFRectangle - disable RF rectangle before capture image and enable back after capturing image
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_130::enableDisableAFRectangle(bool enableRFRect){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_AF_RECTANGLE_130; /* enable/disabler AF Rect */

    if(enableRFRect)
        g_out_packet_buf[3] = ENABLE_AF_RECTANGLE_130; /* value to enable AF Rectangle */
    else
        g_out_packet_buf[3] = DISABLE_AF_RECTANGLE_130; /* value to disable AF Rectangle*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == ENABLE_DISABLE_MODE_AF_RECTANGLE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getAFRectMode - get AF rectangle mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getAFRectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_AF_RECT_MODE; /* get af rect mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==GET_AF_RECT_MODE &&
            g_in_packet_buf[6]==GET_SUCCESS)
        {
            emit afRectModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::enableDisableFaceRectangle - disable RF rectangle before capture image and enable back after capturing image
 * and disable before recording video and enable back.
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_130::enableDisableFaceRectangle(bool enableFaceRect){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_FACE_RECTANGLE_130; /* pass enable/disable command */
    if(enableFaceRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECTANGLE_130; /* enable auto focus rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECTANGLE_130; /* disable auto focus rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==ENABLE_DISABLE_MODE_FACE_RECTANGLE_130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setStreamMode - Setting  Streaming mode
 * @param streamMode - mode selected in UI
 * @return true/false
 */
bool See3CAM_130::setStreamMode(STREAM_MODE streamMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_STREAM_MODE_130; /* set stream mode command */
    g_out_packet_buf[3] = streamMode; /* stream mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_STREAM_MODE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getStreamMode - get Stream mode from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_STREAM_MODE_130; /* get stream mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_STREAM_MODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit streamModeValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setFlipHorzMode - Setting horizontal flip mode
 * @param horizModeSel - mode selected / unselected in UI
 * @return true/false
 */
bool See3CAM_130::setFlipHorzMode(bool horizModeSel){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FLIP_MODE_130; /* set flip mode command */
    g_out_packet_buf[3] = SET_HORZ_FLIP_MODE_130; /* set horizontal flip mode */
    if(horizModeSel){
        g_out_packet_buf[4] = FlipEnable; /* value to enable flip */
    }else{
        g_out_packet_buf[4] = FlipDisable; /* value to disable flip */
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==SET_FLIP_MODE_130 &&
            g_in_packet_buf[2]==SET_HORZ_FLIP_MODE_130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::setFlipVertMode - Setting vertical flip mode
 * @param vertiModeSel - mode selected / unselected in UI
 * @return true/false
 */
bool See3CAM_130::setFlipVertiMode(bool vertModeSel){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FLIP_MODE_130; /* set flip mode command */
    g_out_packet_buf[3] = SET_VERT_FLIP_MODE_130; /* set vertical flip mode */
    if(vertModeSel){
        g_out_packet_buf[4] = FlipEnable; /* value to enable flip */
    }else{
        g_out_packet_buf[4] = FlipDisable; /* value to disable flip */
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==SET_FLIP_MODE_130 &&
            g_in_packet_buf[2]==SET_VERT_FLIP_MODE_130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getFlipMode - getting flip mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_130::getFlipMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_FLIP_MODE_130; /* get flip mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==GET_FLIP_MODE_130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit flipModeValue(g_in_packet_buf[2], g_in_packet_buf[3]);
                return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getAutoWhiteBalance - getting auto white balance value
 * @return true/false
 */
bool See3CAM_130::getAutoWhiteBalance(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_AWB_130; /* get auto exposure code */

    unsigned int autoWhiteBalance;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_AWB_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            autoWhiteBalance = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);

            emit autoWhiteBalanceReceived(autoWhiteBalance);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::getAutoExposure - getting auto exposure value
 * @return true/false
 */
bool See3CAM_130::getAutoExposure(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_AUTO_EXPOSURE; /* get auto exposure code */

    unsigned int autoExposure;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_AUTO_EXPOSURE &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            autoExposure = (g_in_packet_buf[2] << 24) | (g_in_packet_buf[3] << 16) | (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
//            autoExposure |= (static_cast<uint32_t>(g_in_packet_buf[2]) << 24);
//                autoExposure |= (static_cast<uint32_t>(g_in_packet_buf[3]) << 16);
//                autoExposure |= (static_cast<uint32_t>(g_in_packet_buf[4]) << 8);
//                autoExposure |= static_cast<uint32_t>(g_in_packet_buf[5]);
            emit autoExposureReceived(autoExposure);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setFlashMode - setting flash mode to the camera
 * @param flashMode - Flash mode to be set
 * @return true/false
 */
bool See3CAM_130::setFlashMode(FLASH_MODE flashMode){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FLASH_MODE_130; /* Set flash mode command */
    g_out_packet_buf[3] = flashMode; /* Flash mode to be set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_FLASH_MODE_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getFlashMode - getting flash mode from the camera.
 * @return true/false
 */
bool See3CAM_130::getFlashMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_FLASH_MODE_130; /* get flash mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_FLASH_MODE_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit flashModeReceived(g_in_packet_buf[2]);

            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setFaceDetectionRect - setting face detection rectangle
 * @param enableFaceDetectRect - enable / disable face detect rectangle
 * @param embedData - Enable / Disable embed data
 * @param overlayRect - Enable / Disable overlay Rectangle
 * @return true/false
 */
bool See3CAM_130::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_RECT; /* set face detect Rect command */

    if(enableFaceDetectRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECT; /* enable face Rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECT; /* disable face Rect*/

    if(embedData)
        g_out_packet_buf[4] = ENABLE_EMBED_DATA; /* enable embed data */
    else
        g_out_packet_buf[4] = DISABLE_EMBED_DATA; /* disable embed data */

    if(overlayRect)
        g_out_packet_buf[5] = ENABLE_OVERLAY_RECT; /* enable overlay rect */
    else
        g_out_packet_buf[5] = DISABLE_OVERLAY_RECT; /* disable overlay rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_FACE_DETECT_RECT &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getFactDetectMode - get face detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getFaceDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_RECT; /* get face detect mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_FACE_DETECT_RECT &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit faceDetectModeValueReceived(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::getAutoFocusPosition - getting Auto Focus Position
 * @return true/false
 */
bool See3CAM_130::getAutoFocusPosition(){

    //HID validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_AF_POSITION; /* get auto focus position command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_AF_POSITION &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit autoFocusPositionReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool See3CAM_130::setExposureCompensation(unsigned int exposureCompValue){

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
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool See3CAM_130::getExposureCompensation(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION; /* get exposure compensation code */

    unsigned int exposureCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            exposureCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            emit exposureCompValueReceived(exposureCompValue);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setFrameRatectrlValue - setting frame rate control value
 * @param frameRate
 * @return true/false
 */
bool See3CAM_130::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_130; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==SET_FRAME_RATE_130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getFrameRateCtrlValue - get frameRate control value from camera
 * return true - success /false - failure
 */
bool See3CAM_130::getFrameRateCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_130; /* set framerate control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1]==GET_FRAME_RATE_130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit frameRateCtrlValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setFlickerDetection - setting flicker control
 * @param flickerMode - Mode we are going to set
 * @return true/false
 */

bool See3CAM_130::setFlickerDetection(FLICKER_MODE flickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_FLICKER_DETECTION_130; /* set flicker detection command */
    g_out_packet_buf[3] = flickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_FLICKER_DETECTION_130 &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getFlickerDetection - getting the flicker control we set,from the camera.
 * @return true/false
 */

bool See3CAM_130::getFlickerDetection()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_FLICKER_DETECTION_130; /* get flicker detection command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_FLICKER_DETECTION_130 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flickerDetectionModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setRedGain - setting red gain value
 * @param redGain - Value to be set
 * @return true/false
 */
bool See3CAM_130::setRedGain(uint redGain){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_RED_GAIN; /* set Red Gain command */
    g_out_packet_buf[3] = redGain; /* Red gain value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_RED_GAIN &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getRedGain - getting the red gain value from the camera.
 * @return true/false
 */
bool See3CAM_130::getRedGain()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_RED_GAIN; /* get Red Gain command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_RED_GAIN &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit redGainMinReceived(g_in_packet_buf[2]);
            emit redGainMaxReceived(g_in_packet_buf[3]);
            emit redGainCurrentReceived(g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setBlueGain - setting blue gain value
 * @param blueGain - Value to be set
 * @return true/false
 */
bool See3CAM_130::setBlueGain(uint blueGain){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_BLUE_GAIN; /* set Blue Gain command */
    g_out_packet_buf[3] = blueGain; /* Blue gain value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_BLUE_GAIN &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getBlueGain - getting the blue gain value from the camera.
 * @return true/false
 */
bool See3CAM_130::getBlueGain()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_BLUE_GAIN; /* get Blue Gain command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_BLUE_GAIN &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit blueGainMinReceived(g_in_packet_buf[2]);
            emit blueGainMaxReceived(g_in_packet_buf[3]);
            emit blueGainCurrentReceived(g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::setGreenGain - setting green gain value
 * @param greenGain - Value to be set
 * @return true/false
 */
bool See3CAM_130::setGreenGain(uint greenGain){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_GREEN_GAIN; /* Set Green Gain command */
    g_out_packet_buf[3] = greenGain; /* Green gain value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_GREEN_GAIN &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getGreenGain - getting the green gain value from the camera.
 * @return true/false
 */
bool See3CAM_130::getGreenGain()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = GET_GREEN_GAIN; /* get Green Gain command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == GET_GREEN_GAIN &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit greenGainMinReceived(g_in_packet_buf[2]);
            emit greenGainMaxReceived(g_in_packet_buf[3]);
            emit greenGainCurrentReceived(g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_130::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_130; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_130 &&
            g_in_packet_buf[1] == SET_TO_DEFAULT &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_130::saveConfiguration
 * @return true/false
 */
bool See3CAM_130::saveConfiguration()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION; /* camera id */
    g_out_packet_buf[2] = SAVE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION  &&
            g_in_packet_buf[1] == SAVE &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations saved successfully");
            return true;
        }
    }
    return false;
}




/**
 * @brief See3CAM_130::get64BitSerialNumber - To get 64-bit Serial number
 * @return true/false
 */
bool See3CAM_130::get64BitSerialNumber()
{
    QString lsb = "";
    QString msb = "";

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = GET_64BIT_UNIQUE_ID_1; 	/* Report Number */

    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        _text = tr("Device not available");
        return false;
    }
    /* Read the Unique id from the device */
    start = uvc.getTickCount();
    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0)
        {
        }
        else
        {
            if((g_in_packet_buf[0] == GET_64BIT_UNIQUE_ID_1))
            {
                lsb.sprintf("%02x%02x%02x%02x",g_in_packet_buf[1],g_in_packet_buf[2],g_in_packet_buf[3],g_in_packet_buf[4]);

                //Initialize the buffer
                memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

                //Set the Report Number
                g_out_packet_buf[1] = GET_64BIT_UNIQUE_ID_1;
                g_out_packet_buf[2] = GET_64BIT_UNIQUE_ID_2;

                ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
                if (ret < 0) {
                    _text = tr("Device not available");
                    return false;
                }
                start = uvc.getTickCount();

                while(timeout)
                {
                    /* Get a report from the device */
                    ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
                    if (ret < 0)
                    {
                    }
                    else
                    {
                        if((g_in_packet_buf[0] == GET_64BIT_UNIQUE_ID_1))
                        {
                            msb.sprintf("%02x%02x%02x%02x",g_in_packet_buf[2],g_in_packet_buf[3],g_in_packet_buf[4],g_in_packet_buf[5]);
                            timeout = false;
                        }
                    }
                }
            }
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }
    _text.clear();
    _text.append(lsb+msb);
    _title.clear();
    _title = tr("Serial Number");
    emit titleTextChanged(_title,_text);
    return true;
}




/**
 * @brief See3CAM_130::initializeBuffers - Initialize input and output buffers
 */
void See3CAM_130::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
