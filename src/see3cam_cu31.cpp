/*
 * see3cam_cu31.cpp -- Handling special feature of SEE3CAM_CU31 camera
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
#include "see3cam_cu31.h"

SEE3CAM_CU31::SEE3CAM_CU31()
{

}



SEE3CAM_CU31::~SEE3CAM_CU31()
{

}


/*
 * @brief SEE3CAM_CU31::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_CU31::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2; /* camera id_2 */
    g_out_packet_buf[3] = SET_ORIENTATION_SEE3CAM_CU31; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = ROTATE_180; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[4] = HORIZONTAL; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[4] = VERTICAL; /* vertical flip only mode */
    }else
        g_out_packet_buf[4] = NORMAL; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Orientation Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_ORIENTATION_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/*
 * @brief SEE3CAM_CU31::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
*/
bool SEE3CAM_CU31::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2; /* camera id_2 */
    g_out_packet_buf[3] = GET_ORIENTATION_SEE3CAM_CU31; /* get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Orientation Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_ORIENTATION_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setRawNoiseReductionStatus - Enable/Disable Raw Noise Reduction in the camera
 * @param status - Mode to set (Enable / Disable)
 * @return true/false
 */
bool SEE3CAM_CU31::setRawNoiseReductionStatus(RNR_STATUS  status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_RNR_STATUS;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Raw Noise Reduction Status Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_RNR_STATUS &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getRawNoiseReductionStatus - To get RNR status
 * @return true/false
 */
bool SEE3CAM_CU31::getRawNoiseReductionStatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_RNR_STATUS;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Raw Noise Reduction Status Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_RNR_STATUS) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentRawNoiseDeductionStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setTriggerMode - To set the Trigger mode to the camera
 * @param mode - Disable/ ReadOut/ Shutter modes
 * @return true/false
 */
bool SEE3CAM_CU31::setTriggerMode(TRIGGER_MODE  mode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_TRIGGER_MODE;
    g_out_packet_buf[4] = mode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Trigger Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_TRIGGER_MODE &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getTriggerMode - To get trigger mode from the camera
 * @return true/false
 */
bool SEE3CAM_CU31::getTriggerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_TRIGGER_MODE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Trigger Mode Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_TRIGGER_MODE) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentTriggerMode(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAEConvergenceSpeed - To set Convergence speed to the camera
 * @param convergenceSpeed - The value to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setAEConvergenceSpeed(uint convergenceSpeed)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AE_CONVERGENCE_SPEED;
    g_out_packet_buf[4] = convergenceSpeed;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Auto Exposure Convergence Speed Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AE_CONVERGENCE_SPEED &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAEConvergenceSpeed - To get convergence speed from the camera
 * @return true/false
 */
bool SEE3CAM_CU31::getAEConvergenceSpeed()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AE_CONVERGENCE_SPEED;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Auto Exposure Convergence Speed Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AE_CONVERGENCE_SPEED) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAEConvergenceSpeed(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAutoExposureStatus
 * @param exposureStatus - The value to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setAutoExposureStatus(AUTO_EXPOSURE exposureStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AE_HOLD;
    g_out_packet_buf[4] = exposureStatus;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Auto Exposure Status Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AE_HOLD &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAutoExpStatus
 * @return true/false
 */
bool SEE3CAM_CU31::getAutoExpStatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AE_HOLD;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Auto Exposure Status Failed. Retry!");
            return false;
        }else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AE_HOLD) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAutoExposureStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAWBStatus
 * @param exposureStatus - The value to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setAWBStatus(AUTO_WHITE_BALANCE status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AWB_STATUS;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Auto White Balance Status Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AWB_STATUS &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAWBStatus
 * @return true/false
 */
bool SEE3CAM_CU31::getAWBStatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AWB_STATUS;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Auto White Balance Status Failed. Retry!");
            return false;
        }else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AWB_STATUS) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAutoWhiteBalanceStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::readTeperature() - To read the temperature in the camera
 * @return true/false
 */
bool SEE3CAM_CU31::readTemperature(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    float temperature;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = READ_TEMPERATURE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Read Temperature Failed. Retry!");
            return false;
        } else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == READ_TEMPERATURE) &&
            g_in_packet_buf[15] == SET_SUCCESS) {

            uint32_t combinedValue = (g_in_packet_buf[6] << 24) | (g_in_packet_buf[5] << 16) | (g_in_packet_buf[4] << 8) | g_in_packet_buf[3];
            temperature = *reinterpret_cast<float*>(&combinedValue);

            emit currentTemperature(temperature);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAntiFlickerMode - setting anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - Auto/50hz/60hz/Disable]
 * @return true/false
 */
bool SEE3CAM_CU31::setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_ANTI_FLICKER_MODE;
    g_out_packet_buf[4] = antiFlickerMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Anti-Flicker Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_ANTI_FLICKER_MODE &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAntiFlickerMode - get current anti flicker mode from camera
 * @return - true/false
 */
bool SEE3CAM_CU31::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_ANTI_FLICKER_MODE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Set Anti-Flicker Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_ANTI_FLICKER_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentAntiFlickerModeRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setCameraMode - setting camera mode to the camera
 * @param cameraMode - mode to set [value of enum  - Auto/Low Light/ LED]
 * @return true/false
 */
bool SEE3CAM_CU31::setCameraMode(CAMERA_MODE cameraMode){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_CAMERA_MODE_SEE3CAM_CU31;
    g_out_packet_buf[4] = cameraMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Camera Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_CAMERA_MODE_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getCameraMode - get current camera mode from the camera
 * @return - true/false
 */
bool SEE3CAM_CU31::getCameraMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_CAMERA_MODE_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure","Get Camera Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_CAMERA_MODE_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit currentCameraModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAEWindowOverlay - To get AE Light metering window overlay status
 * @return true/false
 */
bool SEE3CAM_CU31::getAEWindowOverlay()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AE_OVERLAY_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Auto Exposure LMW Overlay Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AE_OVERLAY_SEE3CAM_CU31) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAEWindowOverlayStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_CU31::setAEWindowOverlay
 * @param status - The Status to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setAEWindowOverlay(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AE_OVERLAY_SEE3CAM_CU31;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Auto Exposure LMW Overlay Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AE_OVERLAY_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAEWindowDimensions - To get AE Light Metering Window Dimensions from the camera
 * @return true/false
 */
bool SEE3CAM_CU31::getAEWindowDimensions()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AE_DIMENSIONS_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get AE Light Metering Window Dimensions Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AE_DIMENSIONS_SEE3CAM_CU31) &&
            (g_in_packet_buf[15] == GET_SUCCESS)){

            uint width, height, xStart, yStart;

            width  = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            height = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            xStart = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            yStart = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            emit currentAEWindowDimensions(width, height, xStart, yStart);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAEWindowDimensions - To set the window dimensions
 * @return true/false
 */
bool SEE3CAM_CU31::setAEWindowDimensions(uint width, uint height, uint xStart, uint yStart)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AE_DIMENSIONS_SEE3CAM_CU31;

    g_out_packet_buf[4] = ((width & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((width & 0x00FF) >> 0);

    g_out_packet_buf[6] = ((height & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((height & 0x00FF) >> 0);

    g_out_packet_buf[8] = ((xStart & 0xFF00) >> 8);
    g_out_packet_buf[9] = ((xStart & 0x00FF) >> 0);

    g_out_packet_buf[10] = ((yStart & 0xFF00) >> 8);
    g_out_packet_buf[11] = ((yStart & 0x00FF) >> 0);

    uint minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == SET_FAIL) {
            if(g_in_packet_buf[32] == DIMENSION_FAILURE){
                minWidth  = (g_in_packet_buf[16] << 8) | (g_in_packet_buf[17] << 0);
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);
                minHeight = (g_in_packet_buf[20] << 8) | (g_in_packet_buf[21] << 0);
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                minXStart  = (g_in_packet_buf[24] << 8) | (g_in_packet_buf[25] << 0);
                maxXStart  = (g_in_packet_buf[26] << 8) | (g_in_packet_buf[27] << 0);
                minYStart = (g_in_packet_buf[28] << 8) | (g_in_packet_buf[29] << 0);
                maxYStart = (g_in_packet_buf[30] << 8) | (g_in_packet_buf[31] << 0);

                emit indicateWindowDimensionError(minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart);
                return false;
            }
            else if(g_in_packet_buf[32] == WIDTH_FAILURE){
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);

                emit indicateWindowWidthError(maxWidth);
                return false;
            }
            else if(g_in_packet_buf[32] == HEIGHT_FAILURE){
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                emit indicateWindowHeightError(maxHeight);
                return false;
            }
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AE_DIMENSIONS_SEE3CAM_CU31 &&
            g_in_packet_buf[15] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAEMaskOverlay - To get AE Light metering window Mask overlay status
 * @return true/false
 */
bool SEE3CAM_CU31::getAEMaskOverlay()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AE_MASK_OVERLAY_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Auto Exposure Convergence Speed Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AE_MASK_OVERLAY_SEE3CAM_CU31) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAEMaskOverlayStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAEMaskOverlay
 * @param status - The Status to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setAEMaskOverlay(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AE_MASK_OVERLAY_SEE3CAM_CU31;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Auto Exposure Status Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AE_MASK_OVERLAY_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAEMaskDimensions - To get AE Light metering Window Mask dimensions
 * @return true/false
 */
bool SEE3CAM_CU31::getAEMaskDimensions(bool isMaskIDNeeded, uint maskID)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint width, height, xStart, yStart;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AE_MASK_DIMENSIONS_SEE3CAM_CU31;
    g_out_packet_buf[4] = maskID; //Setting respective Mask ID

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == GET_FAIL){
            emit indicateFailureStatus("Failure","Set AE Light metering Window Mask dimensions Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AE_MASK_DIMENSIONS_SEE3CAM_CU31) &&
            (g_in_packet_buf[15] == GET_SUCCESS)){

            width  = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            height = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            xStart = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            yStart = (g_in_packet_buf[11] << 8) | (g_in_packet_buf[12] << 0);

            emit currentAEMaskDimensions(isMaskIDNeeded, maskID, g_in_packet_buf[4], width, height, xStart, yStart);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAEWindowDimensions - To set AE LMW Mask dimensions
 * @return true/false
 */
bool SEE3CAM_CU31::setAEMaskDimensions(uint maskID, uint maskStatus, uint width, uint height, uint xStart, uint yStart)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AE_MASK_DIMENSIONS_SEE3CAM_CU31;

    g_out_packet_buf[4] = maskID;
    g_out_packet_buf[5] = maskStatus;

    g_out_packet_buf[6] = ((width & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((width & 0x00FF) >> 0);

    g_out_packet_buf[8] = ((height & 0xFF00) >> 8);
    g_out_packet_buf[9] = ((height & 0x00FF) >> 0);

    g_out_packet_buf[10] = ((xStart & 0xFF00) >> 8);
    g_out_packet_buf[11] = ((xStart & 0x00FF) >> 0);

    g_out_packet_buf[12] = ((yStart & 0xFF00) >> 8);
    g_out_packet_buf[13] = ((yStart & 0x00FF) >> 0);

    uint minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == SET_FAIL) {
            if(g_in_packet_buf[32] == DIMENSION_FAILURE){

                minWidth  = (g_in_packet_buf[16] << 8) | (g_in_packet_buf[17] << 0);
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);
                minHeight = (g_in_packet_buf[20] << 8) | (g_in_packet_buf[21] << 0);
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                minXStart = (g_in_packet_buf[24] << 8) | (g_in_packet_buf[25] << 0);
                maxXStart = (g_in_packet_buf[26] << 8) | (g_in_packet_buf[27] << 0);
                minYStart = (g_in_packet_buf[28] << 8) | (g_in_packet_buf[29] << 0);
                maxYStart = (g_in_packet_buf[30] << 8) | (g_in_packet_buf[31] << 0);

                emit indicateMaskDimensionError(minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart);

                return false;
            }
            else if(g_in_packet_buf[32] == WIDTH_FAILURE){
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);

                emit indicateMaskWidthError(maxWidth);
                return false;
            }
            else if(g_in_packet_buf[32] == HEIGHT_FAILURE){
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                emit indicateMaskHeightError(maxHeight);
                return false;
             }
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AE_MASK_DIMENSIONS_SEE3CAM_CU31 &&
            g_in_packet_buf[15] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAWBWindowOverlay - To get AWB Light Metering Window Overlay status
 * @return true/false
 */
bool SEE3CAM_CU31::getAWBWindowOverlay()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AWB_OVERLAY_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get AWB Light Metering Window Overlay Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AWB_OVERLAY_SEE3CAM_CU31) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAWBWindowOverlayStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAWBWindowOverlay - To set AWB LMW Overlay Status
 * @param status - To set Enable/Disable
 * @return true/false
 */
bool SEE3CAM_CU31::setAWBWindowOverlay(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AWB_OVERLAY_SEE3CAM_CU31;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set AWB LMW Overlay Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AWB_OVERLAY_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAWBWindowDimensions - To get AWB Window Dimension from the camera
 * @return true/false
 */
bool SEE3CAM_CU31::getAWBWindowDimensions()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AWB_WINDOW_DIMENSIONS_SEE3CAM_CU31;

    uint width, height, xStart, yStart;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get AWB Window Dimension Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AWB_WINDOW_DIMENSIONS_SEE3CAM_CU31) &&
            (g_in_packet_buf[15] == GET_SUCCESS)){

            width  = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            height = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            xStart = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            yStart = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            emit currentAWBWindowDimensions(width, height, xStart, yStart);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAWBWindowDimensions - To set AWB window dimensions
 * @return true/false
 */
bool SEE3CAM_CU31::setAWBWindowDimensions(uint width, uint height, uint xStart, uint yStart)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AWB_WINDOW_DIMENSIONS_SEE3CAM_CU31;

    g_out_packet_buf[4] = ((width & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((width & 0x00FF) >> 0);

    g_out_packet_buf[6] = ((height & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((height & 0x00FF) >> 0);

    g_out_packet_buf[8] = ((xStart & 0xFF00) >> 8);
    g_out_packet_buf[9] = ((xStart & 0x00FF) >> 0);

    g_out_packet_buf[10] = ((yStart & 0xFF00) >> 8);
    g_out_packet_buf[11] = ((yStart & 0x00FF) >> 0);

    uint minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == SET_FAIL) {
            if(g_in_packet_buf[32] == DIMENSION_FAILURE){
                minWidth  = (g_in_packet_buf[16] << 8) | (g_in_packet_buf[17] << 0);
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);
                minHeight = (g_in_packet_buf[20] << 8) | (g_in_packet_buf[21] << 0);
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                minXStart  = (g_in_packet_buf[24] << 8) | (g_in_packet_buf[25] << 0);
                maxXStart  = (g_in_packet_buf[26] << 8) | (g_in_packet_buf[27] << 0);
                minYStart = (g_in_packet_buf[28] << 8) | (g_in_packet_buf[29] << 0);
                maxYStart = (g_in_packet_buf[30] << 8) | (g_in_packet_buf[31] << 0);

                emit indicateWindowDimensionError(minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart);

                return false;
            }
            else if(g_in_packet_buf[32] == WIDTH_FAILURE){
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);

                emit indicateWindowWidthError(maxWidth);
                return false;
            }
            else if(g_in_packet_buf[32] == HEIGHT_FAILURE){
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                emit indicateWindowHeightError(maxHeight);
                return false;
            }
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AWB_WINDOW_DIMENSIONS_SEE3CAM_CU31 &&
            g_in_packet_buf[15] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAWBMaskOverlay - To get AE Light Metering Window Overlay status
 * @return true/false
 */
bool SEE3CAM_CU31::getAWBMaskOverlay()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AWB_MASK_OVERLAY_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get AE Light Metering Window Overlay Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AWB_MASK_OVERLAY_SEE3CAM_CU31) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentAWBMaskOverlayStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAWBMaskOverlay
 * @param exposureStatus - The value to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setAWBMaskOverlay(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AWB_MASK_OVERLAY_SEE3CAM_CU31;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set AE Light Metering Window Overlay Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AWB_MASK_OVERLAY_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getAWBMaskDimensions - To get AWB Light Metering Window Mask Dimensions
 * @return true/false
 */
bool SEE3CAM_CU31::getAWBMaskDimensions(bool isMaskIDNeeded, uint maskID)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint width, height, xStart, yStart;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AWB_MASK_DIMENSIONS_SEE3CAM_CU31;
    g_out_packet_buf[4] = maskID; //Setting respective Mask ID

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[15] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get AWB Light Metering Window Mask Dimensions Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_AWB_MASK_DIMENSIONS_SEE3CAM_CU31) &&
            (g_in_packet_buf[15] == GET_SUCCESS)){

            width  = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            height = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            xStart = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            yStart = (g_in_packet_buf[11] << 8) | (g_in_packet_buf[12] << 0);

            emit currentAWBMaskDimensions(isMaskIDNeeded, maskID, g_in_packet_buf[4], width, height, xStart, yStart);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setAWBMaskDimensions - To set the AWB window dimensions
 * @return true/false
 */
bool SEE3CAM_CU31::setAWBMaskDimensions(uint maskID, uint maskStatus, uint width, uint height, uint xStart, uint yStart)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AWB_MASK_DIMENSIONS_SEE3CAM_CU31;

    g_out_packet_buf[4] = maskID;
    g_out_packet_buf[5] = maskStatus;

    g_out_packet_buf[6] = ((width & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((width & 0x00FF) >> 0);

    g_out_packet_buf[8] = ((height & 0xFF00) >> 8);
    g_out_packet_buf[9] = ((height & 0x00FF) >> 0);

    g_out_packet_buf[10] = ((xStart & 0xFF00) >> 8);
    g_out_packet_buf[11] = ((xStart & 0x00FF) >> 0);

    g_out_packet_buf[12] = ((yStart & 0xFF00) >> 8);
    g_out_packet_buf[13] = ((yStart & 0x00FF) >> 0);

    uint minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[15] == SET_FAIL) {
            if(g_in_packet_buf[32] == DIMENSION_FAILURE){
                minWidth  = (g_in_packet_buf[16] << 8) | (g_in_packet_buf[17] << 0);
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);
                minHeight = (g_in_packet_buf[20] << 8) | (g_in_packet_buf[21] << 0);
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                minXStart = (g_in_packet_buf[24] << 8) | (g_in_packet_buf[25] << 0);
                maxXStart = (g_in_packet_buf[26] << 8) | (g_in_packet_buf[27] << 0);
                minYStart = (g_in_packet_buf[28] << 8) | (g_in_packet_buf[29] << 0);
                maxYStart = (g_in_packet_buf[30] << 8) | (g_in_packet_buf[31] << 0);

                emit indicateMaskDimensionError(minWidth, maxWidth, minHeight, maxHeight, minXStart, maxXStart, minYStart, maxYStart);

                return false;
            }
            else if(g_in_packet_buf[32] == WIDTH_FAILURE){
                maxWidth  = (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);

                emit indicateMaskWidthError(maxWidth);
                return false;
            }
            else if(g_in_packet_buf[32] == HEIGHT_FAILURE){
                maxHeight = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);

                emit indicateMaskHeightError(maxHeight);
                return false;
            }
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AWB_MASK_DIMENSIONS_SEE3CAM_CU31 &&
            g_in_packet_buf[15] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getPresetMode - To get Preset Mode from the camera
 * @return true/false
 */
bool SEE3CAM_CU31::getPresetMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_USER_PRESET_MODE_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Preset Mode Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_USER_PRESET_MODE_SEE3CAM_CU31) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentPresetProperties(g_in_packet_buf[3], g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setPresetMode
 * @param mode - Preset Mode to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setPresetMode(PRESET_MODE mode, PRESET_BUTTON button)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_USER_PRESET_MODE_SEE3CAM_CU31;
    g_out_packet_buf[4] = mode;
    g_out_packet_buf[5] = button;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Preset Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_USER_PRESET_MODE_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::getEmbeddedData - To get Embedded data from the camera
 * @return true/false
 */
bool SEE3CAM_CU31::getEmbeddedData()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_EMBEDDED_DATA_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL){
            emit indicateFailureStatus("Failure","Get Embedded Data Failed. Retry!");
            return false;
        }
        else if((g_in_packet_buf[0] == CAMERA_CONTROL_ID_1) &&
            (g_in_packet_buf[1] == CAMERA_CONTROL_ID_2) &&
            (g_in_packet_buf[2] == GET_EMBEDDED_DATA_SEE3CAM_CU31) &&
            (g_in_packet_buf[6] == GET_SUCCESS)){
            emit currentEmbedDataStatus(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setEmbeddedData
 * @param exposureStatus - The value to be set
 * @return true/false
 */
bool SEE3CAM_CU31::setEmbeddedData(uint status)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_EMBEDDED_DATA_SEE3CAM_CU31;
    g_out_packet_buf[4] = status;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure","Set Embedded Data Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_EMBEDDED_DATA_SEE3CAM_CU31 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setPropertiesForCrossStill - To set still properties while taking still in cross resolution
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU31::setPropertiesForCrossStill(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SET_CROSS_STILL_PROPERTIES_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if(g_in_packet_buf[6] == SET_FAIL){
            emit indicateFailureStatus("Failure","Setting Still Properties Failed. Retry!");
            return false;
        }else if((g_in_packet_buf[0] == SET_CROSS_STILL_PROPERTIES_SEE3CAM_CU31) && (g_in_packet_buf[6] == SUCCESS)){
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_CU31::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_DEFAULT;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL){
            emit indicateFailureStatus("Failure","Set Default Failed. Retry!");
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_DEFAULT &&
            g_in_packet_buf[6] == SET_SUCCESS){
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU31::get64BitSerialNumber - To get 64-bit Serial number
 * @return true/false
 */
bool SEE3CAM_CU31::get64BitSerialNumber()
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
            if(g_in_packet_buf[0] == GET_64BIT_UNIQUE_ID_1)
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
 * @brief SEE3CAM_CU31::readISPFirmwareVersion - To read the firmware version of ISP
 * @return true/false
 */
bool SEE3CAM_CU31::readISPFirmwareVersion()
{
    _title = tr("ISP Version");

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    g_out_packet_buf[1] = ISP_FIRMWARE_VERSION_SEE3CAM_CU31;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_SUCCESS) {

        } else if(g_in_packet_buf[6] == SET_FAIL) {
            return false;
        }
    }

    _text.clear();
    _text.append("Version: ");
    _text.append(QString::number(g_in_packet_buf[1]).append(".").append(QString::number(g_in_packet_buf[2])).append(".").append(QString::number(g_in_packet_buf[3])).append(".").append(QString::number(g_in_packet_buf[4])));
    emit titleTextChanged(_title,_text);
    return true;
}


/*
 * @brief SEE3CAM_CU31::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_CU31::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
