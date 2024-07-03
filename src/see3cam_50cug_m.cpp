/*
 * see3cam_50cug_m.cpp -- Handling special feature of nilecam30_usb camera
 * Copyright © 2024 e-con Systems India Pvt. Limited
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

#include "see3cam_50cug_m.h"

SEE3CAM_50CUGM::SEE3CAM_50CUGM()
{

}


/*
 * @brief SEE3CAM_50CUGM::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_50CUGM::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SET_ORIENTATION_SEE3CAM_50CUG_M; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = ROTATE_180; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = HORIZONTAL; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = VERTICAL; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = NORMAL; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_ORIENTATION_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_ORIENTATION_SEE3CAM_50CUG_M; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_ORIENTATION_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/*
 * @brief SEE3CAM_50CUGM::setCameraMode - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - cameraModes - To switch between master and trigger mode
 * return true/false
*/
bool SEE3CAM_50CUGM::setCameraMode(cameraModes cameraMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SET_CAMERA_MODE_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = cameraMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]== SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_CAMERA_MODE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getStrobeMode - To get status of camera mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getCameraMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_CAMERA_MODE_SEE3CAM_50CUG_M; /* get camera mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_CAMERA_MODE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit cameraModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/*
 * @brief SEE3CAM_50CUGM::setStrobeMode - Setting Strobe mode to the camera
 * @param - strobe - To switch between OFF, Flash and Torch modes
 * return true/false
*/
bool SEE3CAM_50CUGM::setStrobeMode(strobeMode strobe)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_STROBE_MODE_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = strobe;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_STROBE_MODE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getStrobeMode - To get status of strobe mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getStrobeMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = GET_STROBE_MODE_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_STROBE_MODE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setBlackLevelAdjustment - set black level value in camera
 * @param blacklevel - set blacklevel to enable or disable
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::setBlackLevelAdjustment(uint blackLevelValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = ((blackLevelValue & 0xFF00) >> 8);
    g_out_packet_buf[4] = ((blackLevelValue & 0x00FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getBlackLevelAdjustment - get black level adjustment from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getBlackLevelAdjustment()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint blackLevelValue;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = GET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_BLACK_LEVEL_ADJUSTMENT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            blackLevelValue = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);
            emit blackLevelValueChanged(blackLevelValue);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::setBurstLength(uint burstLength){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SET_IMAGE_BURST_SEE3CAM_50CUG_M; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1]==SET_IMAGE_BURST_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_IMAGE_BURST_SEE3CAM_50CUG_M; /* get burst length mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1]==GET_IMAGE_BURST_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit imageBurstChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_50CUGM::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_50CUGM::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULT_SEE3CAM_50CUGM; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1]==SET_DEFAULT_SEE3CAM_50CUGM &&
            g_in_packet_buf[6]==SET_SUCCESS){
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::saveConfiguration
 * @return true/false
 */
bool SEE3CAM_50CUGM::saveConfiguration()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SAVE_SEE3CAM_50CUG_M; /* set to default command */


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION_SEE3CAM_50CUG_M  &&
            g_in_packet_buf[1]==SAVE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6]==SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations saved successfully");
            return true;
        }
    }
    return false;
}

/*
 * @brief SEE3CAM_50CUGM::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_50CUGM::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
