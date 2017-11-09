/*
 * seecam_cu130.cpp -- Handling special feature of seecamcu130 camera
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

#include "see3cam_cu130.h"

See3CAM_CU130::See3CAM_CU130(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief See3CAM_CU130::getSceneMode - get scene mode from camera
 */
bool See3CAM_CU130::getSceneMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = GET_SCENE_MODE; /* get scene mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_SCENE_MODE &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit sceneModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::getEffectMode - get effect mode from camera
 */
bool See3CAM_CU130::getEffectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = GET_SPECIAL_EFFECT; /* get special effect command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_SPECIAL_EFFECT &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit effectModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::setSceneMode - setting scene mode into the camera
 * @param sceneMode - scene mode value
 * return true/false
 */
bool See3CAM_CU130::setSceneMode(const See3CAM_CU130::sceneModes &sceneMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Camera control id */
    g_out_packet_buf[2] = SET_SCENE_MODE; /* Set scene mode command */
    g_out_packet_buf[3] = sceneMode; /* Scene mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_SCENE_MODE &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::setEffectMode - set special effect mode
 * @param specialEffect - special effect value
 * return true/false
 */
bool See3CAM_CU130::setEffectMode(const See3CAM_CU130::specialEffects &specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Camera control id */
    g_out_packet_buf[2] = SET_SPECIAL_EFFECT; /* Set special effect command */
    g_out_packet_buf[3] = specialEffect; /* special mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_SPECIAL_EFFECT &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setOrientation - Setting orientation - set horizontal/vertical/both flip
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool See3CAM_CU130::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera control for see3cam_30 camera */
    g_out_packet_buf[2] = SET_ORIENTATION_CU130; /* set flip mode for 30 camera */
    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_ENABLE_CU130; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_HORZFLIP_CU130; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SET_ORIENTATION_VERTFLIP_CU130; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SET_ORIENTATION_BOTHFLIP_DISABLE_CU130; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_ORIENTATION_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_CU130::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Camera control id */
    g_out_packet_buf[2] = GET_ORIENTATION_CU130; /* get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_ORIENTATION_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool See3CAM_CU130::setExposureCompensation(unsigned int exposureCompValue){

    if(EXPOSURECOMP_MIN > exposureCompValue || EXPOSURECOMP_MAX < exposureCompValue){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure compensation value is invalid");
        return false;
    }
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_CU130; /* set exposure compensation command */
    g_out_packet_buf[3] = (u_int8_t)((exposureCompValue & 0xFF000000) >> 24);
    g_out_packet_buf[4] = (u_int8_t)((exposureCompValue & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = (u_int8_t)((exposureCompValue & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = (u_int8_t)((exposureCompValue & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set exposure compensation value");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            emit indicateCommandStatus("Success", "Exposure compensation value is set successfully");
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool See3CAM_CU130::getExposureCompensation(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_CU130; /* get exposure compensation code */

    unsigned int exposureComp;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_EXPOSURE_COMPENSATION_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            exposureComp = (((u_int8_t)g_in_packet_buf[2]) << 24)
                                | (((u_int8_t)g_in_packet_buf[3]) << 16)
                                | (((u_int8_t)g_in_packet_buf[4]) << 8)
                                | (((u_int8_t)g_in_packet_buf[5]) << 0);
            emit exposureCompValueReceived(exposureComp);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setDenoiseValue - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool See3CAM_CU130::setDenoiseValue(int deNoiseVal)
{

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_CONTROL_CU130; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_DENOISE_CONTROL_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::getDenoiseValue - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU130::getDenoiseValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_CONTROL_CU130; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_DENOISE_CONTROL_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit denoiseValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool See3CAM_CU130::setBurstLength(uint burstLength){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_BURST_LENGTH_CU130; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_BURST_LENGTH_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::getBurstLength - get burst length from camera [ To take more number of images at a time ]
 * @return true/false
 */
bool See3CAM_CU130::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = GET_BURST_LENGTH_CU130; /* get burst length mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_BURST_LENGTH_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setQFactor - set QFactor in camera
 * @param qFactor.
 * return true - success /false - failure
 */
bool See3CAM_CU130::setQFactor(uint qFactor){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_Q_FACTOR_CU130; /* set qfactor command */
    g_out_packet_buf[3] = qFactor; /* qfactor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_Q_FACTOR_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::getQFactor - get QFactor from camera
 * return true - success /false - failure
 */
bool See3CAM_CU130::getQFactor()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = GET_Q_FACTOR_CU130; /* get qFactor value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_Q_FACTOR_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit qFactorValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_CU130::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_CU130; /* set exposure ROI command */
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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_CU130::getAutoExpROIModeAndWindowSize(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_CU130; /* get exposure ROI mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_EXP_ROI_MODE_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit roiAutoExpModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setToDefault - set the default values in camera
 * @return true/false
 */
bool See3CAM_CU130::setToDefault(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_CU130; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::setFrameRatectrlValue - setting frame rate control value
 * @param frameRate
 * @return true/false
 */
bool See3CAM_CU130::setFrameRateCtrlValue(uint frameRate)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* set camera control code */
    g_out_packet_buf[2] = SET_FRAME_RATE_CU130; /* set framerate control code */
    g_out_packet_buf[3] = frameRate; /* set framerate value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_FRAME_RATE_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::getFrameRateCtrlValue - get frameRate control value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU130::getFrameRateCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* set camera control code */
    g_out_packet_buf[2] = GET_FRAME_RATE_CU130; /* get frame rate code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_FRAME_RATE_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit frameRateCtrlValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setFaceDetectionRect - setting face detection rectangle
 * @param enableFaceDetectRect - enable / disable face detect rectangle
 * @param embedData - Enable / Disable embed data
 * @param overlayRect - Enable / Disable overlay Rectangle
 * @return true/false
 */
bool See3CAM_CU130::setFaceDetectionRect(bool enableFaceDetectRect, bool embedData, bool overlayRect){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_FACE_DETECT_CU130; /* set face detect Rect command */

    if(enableFaceDetectRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECT_CU130; /* enable face Rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECT_CU130; /* disable face Rect*/

    if(embedData)
        g_out_packet_buf[4] = ENABLE_EMBED_DATA_CU130; /* enable embed data */
    else
        g_out_packet_buf[4] = DISABLE_EMBED_DATA_CU130; /* disable embed data */

    if(overlayRect)
        g_out_packet_buf[5] = ENABLE_OVERLAY_RECT_CU130; /* enable overlay rect */
    else
        g_out_packet_buf[5] = DISABLE_OVERLAY_RECT_CU130; /* disable overlay rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_FACE_DETECT_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::getFactDetectMode - get face detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_CU130::getFaceDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = GET_FACE_DETECT_CU130; /* get face detect mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_FACE_DETECT_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit faceDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU130::setSmileDetection - setting smile detection rectangle
 * @param enableSmileDetect - enable / disable smile detect
 * @param embedData - Enable / Disable embed data
 * @param thresholdValue - smile threshold value
 * @return true/false
 */
bool See3CAM_CU130::setSmileDetection(bool enableSmileDetect, bool embedData, uint thresholdValue){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = SET_SMILE_DETECTION_CU130; /* set face detect Rect command */

    if(enableSmileDetect)
        g_out_packet_buf[3] = ENABLE_SMILE_DETECT_CU130; /* enable smile detect */
    else
        g_out_packet_buf[3] = DISABLE_SMILE_DETECT_CU130; /* disable smile detect */

    g_out_packet_buf[4] = thresholdValue; // Setting threshold value

    if(embedData)
        g_out_packet_buf[5] = ENABLE_EMBED_DATA_CU130; /* enable embed data */
    else
        g_out_packet_buf[5] = DISABLE_EMBED_DATA_CU130; /* disable embed data */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure", "Failed to set smile detection threshold");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==SET_SMILE_DETECTION_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {\
            emit indicateCommandStatus("Success", "Smile detection threshold is set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::getSmileDetectMode - get smile detect mode[ disable/enable ] from camera
 * return true - success /false - failure
 */
bool See3CAM_CU130::getSmileDetectMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* Camera control id */
    g_out_packet_buf[2] = GET_SMILE_DETECTION_CU130; /* Get smile detection */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==GET_SMILE_DETECTION_CU130 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit smileDetectModeValue(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::enableDisableFaceRectangle - enable or disable the face overlay rectangle
 * @param enableFaceRect - true/false
 * @return true/false
 */
bool See3CAM_CU130::enableDisableFaceRectangle(bool enableFaceRect){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU130; /* camera id */
    g_out_packet_buf[2] = ENABLE_DISABLE_MODE_FACE_RECTANGLE_CU130; /* pass enable/disable command */
    if(enableFaceRect)
        g_out_packet_buf[3] = ENABLE_FACE_RECTANGLE_CU130; /* enable auto focus rect */
    else
        g_out_packet_buf[3] = DISABLE_FACE_RECTANGLE_CU130; /* disable auto focus rect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU130 &&
            g_in_packet_buf[1]==ENABLE_DISABLE_MODE_FACE_RECTANGLE_CU130 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU130::initializeBuffers - Initialize input and output buffers
 */
void See3CAM_CU130::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
