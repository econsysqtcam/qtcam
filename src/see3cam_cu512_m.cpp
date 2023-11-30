/*
 * see3camcu512m.cpp -- Handling special feature of SEE3CAM_CU512M camera
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
#include "see3cam_cu512_m.h"

SEE3CAM_CU512M::SEE3CAM_CU512M()
{

}

/*
 * @brief SEE3CAM_CU512M::setStreamMode - set Master/Trigger mode to the camera
 * @param - streamMode - To switch between master and trigger mode
 * return true/false
*/
bool SEE3CAM_CU512M::setStreamMode(CAMERA_MODE streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M;
    g_out_packet_buf[3] = SET_STREAM_MODE_SEE3CAM_CU512M;
    g_out_packet_buf[4] = streamMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2] == SET_STREAM_MODE_SEE3CAM_CU512M &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU512M::getStreamMode - To get status of stream mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU512M::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M;
    g_out_packet_buf[3] = GET_STREAM_MODE_SEE3CAM_CU512M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2] == GET_STREAM_MODE_SEE3CAM_CU512M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit streamModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/*
 * @brief SEE3CAM_CU512M::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_CU512M::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M;
    g_out_packet_buf[3] = SET_ORIENTATION_MODE_SEE3CAM_CU512M;

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
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2] == SET_ORIENTATION_MODE_SEE3CAM_CU512M &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU512M::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU512M::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M;
    g_out_packet_buf[3] = GET_ORIENTATION_MODE_SEE3CAM_CU512M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2] == GET_ORIENTATION_MODE_SEE3CAM_CU512M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU512M::setFlashMode - setting flashMode to the camera
 * @param flashMode - Type of flash mode
 * return true - success /false - failure
 */
bool SEE3CAM_CU512M::setFlashMode(FlashMode flashMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M; /* camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M; /* camera control id_2 */
    g_out_packet_buf[3] = SET_FLASH_MODE_SEE3CAM_CU512; /* set flash mode */
    g_out_packet_buf[4] = flashMode; /* set flash mode*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2]==SET_FLASH_MODE_SEE3CAM_CU512 &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU512M::getFlashMode - get flash mode from camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU512M::getFlashMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_FLASH_MODE_SEE3CAM_CU512; /* get flash mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2] == GET_FLASH_MODE_SEE3CAM_CU512 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flashModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU512M::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_CU512M::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU512M; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU512M; /* camera id_2 */
    g_out_packet_buf[3] = SET_DEFAULT_SEE3CAM_CU512M; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU512M &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU512M &&
            g_in_packet_buf[2] == SET_DEFAULT_SEE3CAM_CU512M &&
            g_in_packet_buf[6] == SET_SUCCESS){
            return true;
        }
    }
    return false;
}

/*
 *
 * @brief SEE3CAM_CU512M::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_CU512M::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
