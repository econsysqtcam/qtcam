/*
 * nilecam30_usb.cpp -- Handling special feature of nilecam30_usb camera
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

#include "nilecam30_usb.h"

NILECAM30_USB::NILECAM30_USB(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief NILECAM30_USB::setSpecialEffectsNileCam - setting special effects to the camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::setSpecialEffectsNileCam30USB(const specialEffects &specialEffect)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT; /* set special effect code */
    g_out_packet_buf[3] = specialEffect; /* set special effect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_SPECIAL_EFFECT &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::setDenoiseValueNileCam - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool NILECAM30_USB::setDenoiseValueNileCam30USB(int deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_DENOISE_CONTROL &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getSpecialEffectsNileCam - get effect mode set in camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getSpecialEffectsNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_SPECIAL_EFFECT &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit sendEffectMode(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getDenoiseValueNileCam - get denoise value from camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getDenoiseValueNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_CONTROL; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_DENOISE_CONTROL &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit sendDenoiseValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getSceneModeNileCam - getting scene mode from the camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getSceneModeNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = GET_SCENEMODE_NILECAM30_USB; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_SCENEMODE_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit sceneModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::setSceneModeNileCam - setting scene mode to the camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::setSceneModeNileCam30USB(const sceneModes &sceneMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENEMODE_NILECAM30_USB; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_SCENEMODE_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::setExpRoiModeNileCam - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool NILECAM30_USB::setExpRoiModeNileCam30USB(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_NILECAM30_USB; /* set exposure ROI command */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* exposure ROI mode to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt(); // window size
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::getExpRoiModeNileCam - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool NILECAM30_USB::getExpRoiModeNileCam30USB(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_NILECAM30_USB; /* get exposure ROI mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]== GET_EXP_ROI_MODE_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit roiAutoExpModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_130::setBurstLengthNileCam - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool NILECAM30_USB::setBurstLengthNileCam30USB(uint burstLength){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_NILECAM30_USB; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]== SET_BURST_LENGTH_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getBurstLengthNileCam - get burst length from camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getBurstLengthNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_NILECAM30_USB; /* get burst length mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::setQFactorNileCam - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool NILECAM30_USB::setQFactorNileCam30USB(uint qFactor){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_NILECAM30_USB; /* set qfactor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_Q_FACTOR_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getQFactorNileCam - get QFactor from camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getQFactorNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_NILECAM30_USB; /* get qFactor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]== GET_Q_FACTOR_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit qFactorValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::setFlipModeNileCam - Setting orientation
*/
bool NILECAM30_USB::setFlipModeNileCam30USB(bool horzModeSel, bool vertiModeSel)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera control for see3cam_30 camera */
    g_out_packet_buf[2] = SET_ORIENTATION_NILECAM30_USB; /* set flip mode for 30 camera */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_ENABLE_NILECAM30_USB ; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_HORZFLIP_NILECAM30_USB; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_VERTFLIP_NILECAM30_USB ; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_DISABLE_NILECAM30_USB ; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_ORIENTATION_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}




/**
 * @brief NILECAM30_USB::getFlipModeNileCam - getting flip mode from the camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getFlipModeNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera control id */
    g_out_packet_buf[2] = GET_ORIENTATION_NILECAM30_USB; /* Get orientation command */


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_ORIENTATION_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::setFrameRatectrlValue - setting frame rate control value
 * @param frameRate
 * @return true/false
 */
bool NILECAM30_USB::setFrameRateValueNileCam30USB(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_NILECAM30_USB; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_FRAME_RATE_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getFrameRateValueNileCam - get frameRate control value from camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getFrameRateValueNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_NILECAM30_USB; /* get frame rate code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_FRAME_RATE_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit frameRateCtrlValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::setExposureCompensationNileCam - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool NILECAM30_USB::setExposureCompensationNileCam30USB(unsigned int exposureCompValue){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_NILECAM30_USB; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_NILECAM30_USB && g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_130::getExposureCompensationNileCam - getting exposure compensation
 * @return true/false
 */
bool NILECAM30_USB::getExposureCompensationNileCam30USB(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_NILECAM30_USB; /* get exposure compensation code */

    unsigned int exposureCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]== GET_EXPOSURE_COMPENSATION_NILECAM30_USB && g_in_packet_buf[6]==GET_SUCCESS) {\
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
 * @brief NILECAM30_USB::setFaceDetectionRectNileCam - setting face detection rectangle
 * @param enableFaceDetectRect - enable / disable face detect rectangle
 * @param embedData - Enable / Disable embed data
 * @param overlayRect - Enable / Disable overlay Rectangle
 * @return true/false
 */
bool NILECAM30_USB::setFaceDetectionRectNileCam30USB(bool enableFaceDetectRect, bool embedData, bool overlayRect){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_RECT_NILECAM30_USB; /* set face detect Rect command */

    if(enableFaceDetectRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECT_NILECAM30_USB; /* enable face Rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECT_NILECAM30_USB; /* disable face Rect*/

    if(embedData)
        g_out_packet_buf[4] = ENABLE_EMBED_DATA_NILECAM30_USB; /* enable embed data */
    else
        g_out_packet_buf[4] = DISABLE_EMBED_DATA_NILECAM30_USB; /* disable embed data */

    if(overlayRect)
        g_out_packet_buf[5] = ENABLE_OVERLAY_RECT_NILECAM30_USB; /* enable overlay rect */
    else
        g_out_packet_buf[5] = DISABLE_OVERLAY_RECT_NILECAM30_USB; /* disable overlay rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_FACE_DETECT_RECT_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::getFactDetectMode - get face detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getFaceDetectionRectNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_RECT_NILECAM30_USB; /* get face detect mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1] == GET_FACE_DETECT_RECT_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit faceDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::setSmileDetectionStateNileCam - setting smile detection rectangle
 * @param enableSmileDetect - enable / disable smile detect
 * @param embedData - Enable / Disable embed data
 * @param thresholdValue - smile threshold value
 * @return true/false
 */
bool NILECAM30_USB::setSmileDetectionStateNileCam30USB(bool enableSmileDetect, bool embedData, uint thresholdValue,  bool smileTrigger){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    if((SMILE_THRESHOLD_MIN > thresholdValue || SMILE_THRESHOLD_MAX < thresholdValue) && enableSmileDetect){
        emit indicateSmileThresholdRangeFailure("Failure", "Given smile detection threshold value is invalid.");
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION_NILECAM30_USB; /* set face detect Rect command */

    if(enableSmileDetect)
        g_out_packet_buf[3] = ENABLE_SMILE_DETECT_NILECAM30_USB; /* enable smile detect */
    else
        g_out_packet_buf[3] = DISABLE_SMILE_DETECT_NILECAM30_USB; /* disable smile detect */

    g_out_packet_buf[4] = thresholdValue; // Setting threshold value

    if(embedData)
        g_out_packet_buf[5] = ENABLE_EMBED_DATA_NILECAM30_USB; /* enable embed data */
    else
        g_out_packet_buf[5] = DISABLE_EMBED_DATA_NILECAM30_USB; /* disable embed data */

    if(smileTrigger)
        g_out_packet_buf[6] = ENABLE_SMILE_TRIGGER_NILECAM30_USB; /* enable smile trigger  */
    else
        g_out_packet_buf[6] = DISABLE_SMILE_TRIGGER_NILECAM30_USB; /* disable smile trigger */
    
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set smile detection threshold");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_SMILE_DETECTION_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Smile detection threshold is set successfully");
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::getSmileDetectionStateNileCam - get smile detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getSmileDetectionStateNileCam30USB()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* Camera control id */
    g_out_packet_buf[2] = GET_SMILE_DETECTION_NILECAM30_USB; /* Get smile detection */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GET_SMILE_DETECTION_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit smileDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4], g_in_packet_buf[5]);
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::restoreDefaultNileCam - set all the values to default in camera
 * @return true/false
 */
bool NILECAM30_USB::restoreDefaultNileCam30USB(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_NILECAM30_USB; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::enableDisableFaceRectangle - disable RF rectangle before capture image and enable back after capturing image
 * and disable before recording video and enable back.
 * @param enableRFRect - true / false
 * @return true/false
 */
bool NILECAM30_USB::enableDisableFaceRectangle(bool enableFaceRect){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_FACE_RECTANGLE_NILECAM30_USB; /* pass enable/disable command */
    if(enableFaceRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECTANGLE_NILECAM30_USB; /* enable auto focus rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECTANGLE_NILECAM30_USB; /* disable auto focus rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==ENABLE_DISABLE_MODE_FACE_RECTANGLE_NILECAM30_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief NILECAM30_USB::setFlashNileCam - setting flash mode to the camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::setFlashNileCam30USB(flashStateValues flashMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* set camera control code */
    g_out_packet_buf[2] = SETFLASH_STATUS_NILECAM30_USB ; /* set flash status command code */
    g_out_packet_buf[3] = flashMode; /* set flash state [off/torch/storbe] */

    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==SETFLASH_STATUS_NILECAM30_USB  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM30_USB::getFlashNileCam - getting flash state from the camera
 * return true - success /false - failure
 */
bool NILECAM30_USB::getFlashNileCam30USB()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM30_USB; /* camera control id */
    g_out_packet_buf[2] = GETFLASH_STATUS_NILECAM30_USB; /* get strobe mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM30_USB &&
            g_in_packet_buf[1]==GETFLASH_STATUS_NILECAM30_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flashModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

bool NILECAM30_USB::readMCUFirmwareVersion(){
    QString mcuFwVersion;

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
	
    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_MCUFWVERSION_NILECAM30_USB; /* get mcu fw version */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if(g_in_packet_buf[0] == CAMERA_MCUFWVERSION_NILECAM30_USB){
	    mcuFwVersion.append(g_in_packet_buf[1]);	    
	    mcuFwVersion.append(g_in_packet_buf[2]);
	    mcuFwVersion.append(g_in_packet_buf[3]);
	    mcuFwVersion.append(g_in_packet_buf[4]);
	    mcuFwVersion.append(g_in_packet_buf[5]);
	    mcuFwVersion.append(g_in_packet_buf[6]);
	    mcuFwVersion.append(g_in_packet_buf[7]);
	    emit mcuFirmwareVersionReceived(mcuFwVersion);	   
            return true;
        }
    }
    return false;
}

void NILECAM30_USB::setStillSkipCount(QString stillOutFomat){
    uint frameToSkip;

    frameToSkip = 1;

    // send frame to skip values to qtcam.qml
    emit updateFrameToSkipfromCam(frameToSkip);
}



/**
 * @brief NILECAM30_USB::initializeBuffers - Initialize input and output buffers
 */
void NILECAM30_USB::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
