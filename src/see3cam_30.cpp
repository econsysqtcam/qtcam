/*
 * seecam_30.cpp -- Handling special feature of see3cam_30 camera
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

#include "see3cam_30.h"

See3CAM_30::See3CAM_30(QObject *parent) :
    QObject(parent)
{
}


/**
 * @brief See3CAM_30::setEffectMode - setting special effects to the camera
 * return true - success /false - failure
 */
bool See3CAM_30::setEffectMode(const specialEffects &specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT; /* set special effect command  */
    g_out_packet_buf[3] = specialEffect; /* pass special effect value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_SPECIAL_EFFECT &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_30::setDenoiseValue(int deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL; /* set denoise control command  */
    g_out_packet_buf[3] = deNoiseVal; /* pass denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_DENOISE_CONTROL &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getEffectMode - get effect mode set in camera
 * return true - success /false - failure
 */
bool See3CAM_30::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT; /* get special effect command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_SPECIAL_EFFECT &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit effectModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_DENOISE_CONTROL; /* get denoise control command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_DENOISE_CONTROL &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit denoiseValueChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setAutoFocusMode - set auto focus mode in camera
 * @param afMode - enum Auto focus mode
 * return true - success /false - failure
 */
bool See3CAM_30::setAutoFocusMode(camAfMode afMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_AF_MODE_30; /* set auto focus mode command  */
    g_out_packet_buf[3] = afMode; /* pass af mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_AF_MODE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_30::getAutoFocusMode - get auto focus mode from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getAutoFocusMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_AF_MODE_30; /* get auto focus mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_AF_MODE_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit afModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_30::setQFactor(uint qFactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_30; /* set q factor command  */
    g_out_packet_buf[3] = qFactor; /* pass q factor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_30; /* get q factor command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_Q_FACTOR_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit qFactorChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_30::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_ORIENTATION_30; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_ORIENTATION_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setROIAutoFocus - Set ROI auto focus to camera
 * @param camROIAfMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_30::setROIAutoFoucs(camROIAfMode see3camAfROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

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

    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for camera see3cam_30 */
    g_out_packet_buf[2] = SET_AF_ROI_MODE_30; /* set Auto focus ROI mode command  */
    g_out_packet_buf[3] = see3camAfROIMode; /*  ROI mode which is need to set */

    if(see3camAfROIMode == AFManual){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_AF_ROI_MODE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getAutoFocusROIModeAndWindowSize - get ROI auto focus mode and window size
 * return true - success /false - failure
 */
bool See3CAM_30::getAutoFocusROIModeAndWindowSize()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_AF_ROI_MODE_30; /* get AF ROI mode command  */

    unsigned char roiMode, windowSize = 1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_AF_ROI_MODE_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
                roiMode = g_in_packet_buf[2];
                if(roiMode == AFManual){
                    windowSize = g_in_packet_buf[5];
                }
                emit roiAfModeChanged(roiMode, windowSize);
                return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_30::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_30::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

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

    g_out_packet_buf[1] = CAMERA_CONTROL_30;   /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_30; /* set exposure mode command */
    g_out_packet_buf[3] = see3camAutoexpROIMode;/* auto exposure value to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_AF_ROI_MODE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_30::getAutoExpROIModeAndWindowSize()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    unsigned char roiMode, windowSize = 1;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_30; /* get AF ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_EXP_ROI_MODE_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
                roiMode = g_in_packet_buf[2];
                if(roiMode == AFManual){
                    windowSize = g_in_packet_buf[5];
                }
                emit roiAutoExpModeChanged(roiMode, windowSize);
                return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::enableDisableAFRectangle - disable RF rectangle before capture image and enable back after capturing image
 * @param enableRFRect - true / false
 * @return true/false
 */
bool See3CAM_30::enableDisableAFRectangle(bool enableRFRect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }


    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for cam_30 */
    g_out_packet_buf[2] = SET_AF_RECT_MODE_30; /* Set AF Rect mode for cam_30 */
    if(enableRFRect)
        g_out_packet_buf[3] = ENABLE_AF_RECTANGLE_30; /* Enable AF Rectangle */
    else
        g_out_packet_buf[3] = DISABLE_AF_RECTANGLE_30; /* Disable AF Rectangle */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_AF_RECT_MODE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getAFRectMode - get AF rectangle mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getAFRectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_AF_RECT_MODE_30; /* get af rect mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_AF_RECT_MODE_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit afRectModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_30::setBurstLength(uint burstLength)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_30; /* set burst length command  */
    g_out_packet_buf[3] = burstLength; /* pass burst length value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_BURST_LENGTH_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_30; /* get burst length command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit burstLengthChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_30::setToDefault()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_30; /* set to default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_30::setOrientation - Setting orientation
 * @param see3cam30obj object of the class
 * @param horzModeSel
 * @param vertiModeSel
 * @return true/false
 */
bool See3CAM_30::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control for see3cam_30 camera */
    g_out_packet_buf[2] = SET_ORIENTATION_30; /* set flip mode for 30 camera */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_ENABLE_30; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_HORZFLIP_30; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_VERTFLIP_30; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_DISABLE_30; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_ORIENTATION_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setSceneMode - Set Scene Mode
 * @param sceneMode - scene Mode to set
 * @return true/false
 */
bool See3CAM_30::setSceneMode(sceneModes sceneMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_SCENEMODE_30; /* set scene mode  */
    g_out_packet_buf[3] = sceneMode; /* pass sceneMode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_SCENEMODE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getSceneMode - Getting scene mode
 * @return true/false
 */
bool See3CAM_30::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_SCENEMODE_30; /* get scene mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_SCENEMODE_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit sceneModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool See3CAM_30::setExposureCompensation(unsigned int exposureCompValue){
    if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure compensation value is invalid");
        return false;
    }
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_30; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool See3CAM_30::getExposureCompensation(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_30; /* get exposure compensation command  */

    unsigned int exposureCompValue;
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            exposureCompValue = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            exposureCompValueReceived(exposureCompValue);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::setFaceDetectionRect - setting face detection rectangle
 * @param enableFaceDetectRect - enable / disable face detect rectangle
 * @param embedData - Enable / Disable embed data
 * @param overlayRect - Enable / Disable overlay Rectangle
 * @return true/false
 */
bool See3CAM_30::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_RECT_30; /* set face detect Rect command */

    if(enableFaceDetectRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECT_30; /* enable face Rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECT_30; /* disable face Rect*/

    if(embedData)
        g_out_packet_buf[4] = ENABLE_EMBED_DATA_30; /* enable embed data */
    else
        g_out_packet_buf[4] = DISABLE_EMBED_DATA_30; /* disable embed data */

    if(overlayRect)
        g_out_packet_buf[5] = ENABLE_OVERLAY_RECT_30; /* enable overlay rect */
    else
        g_out_packet_buf[5] = DISABLE_OVERLAY_RECT_30; /* disable overlay rect */


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_FACE_DETECT_RECT_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getFactDetectMode - get face detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getFaceDetectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    initializeBuffers();

    uint faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect;
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_RECT_30; /* get face detect mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_FACE_DETECT_RECT_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            faceDetectMode = g_in_packet_buf[2];
            faceDetectEmbedDataValue = g_in_packet_buf[3];
            faceDetectOverlayRect = g_in_packet_buf[4];
            emit faceDetectModeValue(faceDetectMode, faceDetectEmbedDataValue, faceDetectOverlayRect);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_30::setSmileDetection - setting smile detection rectangle
 * @param enableSmileDetect - enable / disable smile detect
 * @param embedData - Enable / Disable embed data
 * @param thresholdValue - smile threshold value
 * @return true/false
 */
bool See3CAM_30::setSmileDetection(bool enableSmileDetect, bool embedData, uint thresholdValue){
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    if((SMILE_THRESHOLD_MIN > thresholdValue || SMILE_THRESHOLD_MAX < thresholdValue) && enableSmileDetect){        
        emit indicateSmileThresholdRangeFailure("Failure", "Given smile detection threshold value is invalid.");
        return false;
    }

    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECT_30; /* set smile detect command */

    if(enableSmileDetect)
        g_out_packet_buf[3] = ENABLE_SMILE_DETECT; /* enable smile detect */
    else
        g_out_packet_buf[3] = DISABLE_SMILE_DETECT; /* disable smile detect */

    g_out_packet_buf[4] = thresholdValue; // Setting threshold value

    if(embedData)
        g_out_packet_buf[5] = ENABLE_EMBED_DATA; /* enable embed data */
    else
        g_out_packet_buf[5] = DISABLE_EMBED_DATA; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set smile detection threshold");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_SMILE_DETECT_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Smile detection threshold is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::getSmileDetectMode - get smile detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_30::getSmileDetectMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint smileDetectMode, smileDetectThresholdValue, smileDetectEmbedDataValue;

    //Initialize the buffer
    initializeBuffers();

    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* Camera control id */
    g_out_packet_buf[2] = GET_SMILE_DETECT_30; /* Get smile detection */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set smile detection threshold");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_SMILE_DETECT_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            smileDetectMode = g_in_packet_buf[2];
            smileDetectThresholdValue = g_in_packet_buf[3];
            smileDetectEmbedDataValue = g_in_packet_buf[4];
            emit smileDetectModeValue(smileDetectMode, smileDetectThresholdValue, smileDetectEmbedDataValue);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_30::enableDisableOverlayRect -  disable face overlay rect while capturing still and
 *                                                enable back after capturing still
 * @param enableOverlayRect - true - to enable overlay rect / false - to disable overlay rect
 * @return true/false
 */
bool See3CAM_30::enableDisableOverlayRect(bool enableOverlayRect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* camera control id for cam_30 */
    g_out_packet_buf[2] = SET_OVERLAY_RECT_MODE_30; /* Set Overlay Rect mode for cam_30 */
    if(enableOverlayRect)
        g_out_packet_buf[3] = ENABLE_OVERLAY_RECT_30; /* Enable overlay Rectangle */
    else
        g_out_packet_buf[3] = DISABLE_OVERLAY_RECT_30; /* Disable overlay Rectangle */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_OVERLAY_RECT_MODE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
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
bool See3CAM_30::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_30; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* pass frame rate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==SET_FRAME_RATE_30 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::getFrameRatectrlValue - getting frame rate control value from camera
 * @return true/false
 */
bool See3CAM_30::getFrameRateCtrlValue()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    initializeBuffers();

    g_out_packet_buf[1] = CAMERA_CONTROL_30; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_30; /* get frame rate code */

    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_30 &&
            g_in_packet_buf[1]==GET_FRAME_RATE_30 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit frameRateChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

void See3CAM_30::initializeBuffers(){
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
