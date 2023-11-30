/*
 * see3cam16cugm.cpp -- Handling special feature of SEE3CAM_16CUGM camera
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
#include "see3cam_16cug_m.h"

SEE3CAM_16CUGM::SEE3CAM_16CUGM()
{

}


/**
 * @brief SEE3CAM_16CUGM::setShutterMode - setting shutter mode to the camera
 * @param shutterMode - Type of shutter mode
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::setShutterMode(SHUTTER_MODE shutterMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[3] = SET_SHUTTER_MODE_SEE3CAM_16CUGM; /* set shutter mode */
    g_out_packet_buf[4] = shutterMode; /* set shutter mode*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == SET_SHUTTER_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_16CUGM::getShutterMode - get shutter mode from camera
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::getShutterMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_SHUTTER_MODE_SEE3CAM_16CUGM; /* get shutter mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == GET_SHUTTER_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit shutterModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_16CUGM::setStrobeMode - setting strobe mode to the camera
 * @param strobeMode - Type of strobe mode
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::setStrobeMode(STROBE_MODE strobeMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[3] = SET_STROBE_MODE_SEE3CAM_16CUGM; /* set strobe mode */
    g_out_packet_buf[4] = strobeMode; /* set strobe mode*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == SET_STROBE_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_16CUGM::getStrobeMode - get strobe mode from camera
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::getStrobeMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_STROBE_MODE_SEE3CAM_16CUGM; /* get strobe mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == GET_STROBE_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_16CUGM::setBlackLevelCorrection - Correcting blacklevel in the camera
 * @param blackLevelMode - Type of blacklevel mode
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::setBlackLevelCorrection(BLACKLEVEL_CORRECTION blackLevelMode, uint clampOffset)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[3] = SET_BLACKLEVEL_MODE_SEE3CAM_16CUGM;  /* set Black level */
    g_out_packet_buf[4] = blackLevelMode;

    if(g_out_packet_buf[4] == 0x01)
    {
        g_out_packet_buf[5] = clampOffset;
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2]== SET_BLACKLEVEL_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6]== SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_16CUGM::getBlackLevelCorrection - get black level from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::getBlackLevelCorrection()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_BLACKLEVEL_MODE_SEE3CAM_16CUGM; /* get black level values */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == GET_BLACKLEVEL_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[10] == GET_SUCCESS) {

            emit minimumOffsetReceived(g_in_packet_buf[5]);
            emit maximumOffsetReceived(g_in_packet_buf[6]);
            emit blackLevelModeReceived(g_in_packet_buf[3]);  //Auto / Manual Mode
            emit currentOffsetReceived(g_in_packet_buf[4]);
            return true;
        }
    }
    return false;
}



/*
 * @brief SEE3CAM_16CUGM::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_16CUGM::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* camera id_2 */
    g_out_packet_buf[3] = SET_ORIENTATION_MODE_SEE3CAM_16CUGM; /* set orientation command  */

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
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == SET_ORIENTATION_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_16CUGM::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* camera id_2 */
    g_out_packet_buf[3] = GET_ORIENTATION_MODE_SEE3CAM_16CUGM; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == GET_ORIENTATION_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_16CUGM::setHDRMode - setting HDR mode to the camera
 * @param hdrMode - Type of HDR mode
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::setHDRMode(HDR_MODE hdrMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[3] = SET_HDR_MODE_SEE3CAM_16CUGM; /* set HDR mode */
    g_out_packet_buf[4] = hdrMode; /* set HDR mode*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2]== SET_HDR_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6]== SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_16CUGM::getHDRMode - get HDR mode from camera
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::getHDRMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_HDR_MODE_SEE3CAM_16CUGM; /* get HDR mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2]==GET_HDR_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit hdrModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_16CUGM::setStreamMode - setting stream mode to the camera
 * @param streamMode - Type of strobe mode
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::setStreamMode(STREAM_MODE streamMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control code */
    g_out_packet_buf[3] = SET_STREAM_MODE_SEE3CAM_16CUGM; /* set stream mode */
    g_out_packet_buf[4] = streamMode; /* set stream mode*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2]== SET_STREAM_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6]== SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_16CUGM::getStreamMode - get stream mode from camera
 * return true - success /false - failure
 */
bool SEE3CAM_16CUGM::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_STREAM_MODE_SEE3CAM_16CUGM; /* get stream mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == GET_STREAM_MODE_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit streamModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_16CUGM::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_16CUGM::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_16CUGM; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_16CUGM; /* set camera control id_2 */
    g_out_packet_buf[3] = SET_DEFAULT_SEE3CAM_16CUGM; /* Restore default */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_16CUGM &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_16CUGM &&
            g_in_packet_buf[2] == SET_DEFAULT_SEE3CAM_16CUGM &&
            g_in_packet_buf[6] == SET_SUCCESS){
            return true;
        }
    }
    return false;
}

/*
 *
 * @brief SEE3CAM_16CUGM::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_16CUGM::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
