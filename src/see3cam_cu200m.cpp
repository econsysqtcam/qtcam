/*
 * SEE3CAM_CU200M.cpp -- Handling special feature of SEE3CAM_CU200M camera
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

#include "see3cam_cu200m.h"


SEE3CAM_CU200M::SEE3CAM_CU200M()
{

}


SEE3CAM_CU200M::~SEE3CAM_CU200M()
{

}


/**
 * @brief SEE3CAM_CU200M::getCameraMode - Getting Camera mode from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200M::getStreamMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_STREAM_MODE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure", "Get Stream Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_STREAM_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit streamModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200M::setStreamMode - Setting Stream Mode to the camera
 * streamMode - Type of Camera Mode from UI
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200M::setStreamMode(STREAM_MODE mode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_STREAM_MODE;
    g_out_packet_buf[4] = mode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure", "Set Stream Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_STREAM_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200M::getFlashMode - Getting flash mode from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200M::getFlashMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_FLASH_MODE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure", "Get Flash Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_FLASH_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flashModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200M::setFlashMode - Setting Flash Mode to the camera
 * @param flashMode - Flash mode to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200M::setFlashMode(FLASH_MODE mode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_FLASH_MODE;
    g_out_packet_buf[4] = mode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure", "Set Flash Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_FLASH_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200M::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
*/
bool SEE3CAM_CU200M::getOrientation()
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
    g_out_packet_buf[3] = GET_ORIENTATION; /* get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            emit indicateFailureStatus("Failure", "Get Orientation Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_ORIENTATION &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU200M::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_CU200M::setOrientation(bool horzModeSel, bool vertiModeSel)
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
    g_out_packet_buf[3] = SET_ORIENTATION; /* set orientation command  */

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
            emit indicateFailureStatus("Failure", "Set Orientation Mode Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_ORIENTATION &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU200M::resetDevice - To Reset the device
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200M::resetDevice()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = RESET_COMMAND_1;
    g_out_packet_buf[2] = RESET_COMMAND_2;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure", "Reset Device Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == RESET_COMMAND_1 &&
            g_in_packet_buf[1] == RESET_COMMAND_2 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU200M::setToDefault - set all the values to default in camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200M::setToDefault(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = RESTORE_DEFAULT; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateFailureStatus("Failure", "Set Default Failed. Retry!");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == RESTORE_DEFAULT &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU200M::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_CU200M::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
