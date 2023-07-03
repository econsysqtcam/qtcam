/*
 * see3cam_cu210.cpp -- Handling special feature of See3CAM_CU210 camera
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

#include "see3cam_cu210.h"

See3CAM_CU210::See3CAM_CU210()
{

}


/**
 * @brief See3CAM_CU210::setAwbMode() - setting AWB mode to the camera
 * @param awbMode - Type of AWB mode
 * return true - success /false - failure
 */
bool See3CAM_CU210::setAwbMode(AWSMODE awbMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_AWB_MODE_SEE3CAM_CU210; /* set AWB mode */
    g_out_packet_buf[4] = awbMode; /* set AWB mode*/

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_AWB_MODE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getAwbMode() - get AWB mode from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getAwbMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_AWB_MODE_SEE3CAM_CU210; /* get AWB mode */
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_AWB_MODE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit awbModeRecieved(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU210::setAwbLockStatus - setting AWB mode to the camera
 * @param awbMode - Type of AWB mode
 * return true - success /false - failure
 */
bool See3CAM_CU210::setAwbLockStatus(AWB_LOCK_STATUS lockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_AWB_LOCK_STATUS_SEE3CAM_CU210;
    g_out_packet_buf[4] = lockStatus;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_AWB_LOCK_STATUS_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getAwbLockStatus - get AWB lock status from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getAwbLockStatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_AWB_LOCK_STATUS_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_AWB_LOCK_STATUS_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit awbLockStatusReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU210::setExposureMeteringMode - setting metering mode to the camera
 * @param meteringMode - Type of metering mode
 * return true - success /false - failure
 */
bool See3CAM_CU210::setExposureMeteringMode(EXPOSURE_METERING_MODE meteringMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_EXPOSURE_METERING_MODE_SEE3CAM_CU210;
    g_out_packet_buf[4] = meteringMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_EXPOSURE_METERING_MODE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getExposureMeteringMode - get metering mode from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getExposureMeteringMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_EXPOSURE_METERING_MODE_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_EXPOSURE_METERING_MODE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit awbLockStatusReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU210::setAeLockStatus - setting AE lock status to the camera
 * @param aeLockStatus - Status of AE lock
 * return true - success /false - failure
 */
bool See3CAM_CU210::setAeLockStatus(AE_LOCK_STATUS aeLockStatus)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_AE_LOCK_STATUS_SEE3CAM_CU210;
    g_out_packet_buf[4] = aeLockStatus;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_AE_LOCK_STATUS_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getAeLockStatus - get AE Lock Status from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getAeLockStatus()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_AE_LOCK_STATUS_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_AE_LOCK_STATUS_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit aeLockStatusReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU210::setBurstLength - setting burst length to the camera
 * @param burstLength - To set the burst length to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::setBurstLength(uint burstLength)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_BURST_LENGTH_SEE3CAM_CU210;
    g_out_packet_buf[4] = burstLength;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_BURST_LENGTH_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_BURST_LENGTH_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_BURST_LENGTH_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit burstLengthReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU210::setQFactorValue - setting QFactor value to the camera
 * @param QFactor - To set the QFactor value to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::setQFactorValue(uint QFactor)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_Q_FACTOR_VALUE_SEE3CAM_CU210;
    g_out_packet_buf[4] = QFactor;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_Q_FACTOR_VALUE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getQFactorValue - get QFactor value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getQFactorValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_Q_FACTOR_VALUE_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_Q_FACTOR_VALUE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit qFactorValueReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU210::setFlickerMode - setting flicker mode to the camera
 * @param flickerMode - To set the flicker to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::setFlickerMode(FLICKER flicker)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_FLICKER_MODE_SEE3CAM_CU210;
    g_out_packet_buf[4] = flicker;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_FLICKER_MODE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getFlickerMode - get flicker mode from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_FLICKER_MODE_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_FLICKER_MODE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flickerModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU210::setDenoiseMode - setting denoise value to the camera
 * @param denoise - To set the denoise value to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::setDenoiseMode(DENOISE denoise)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control code */
    g_out_packet_buf[3] = SET_DENOISE_SEE3CAM_CU210;
    g_out_packet_buf[4] = denoise;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_DENOISE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU210::getDenoiseMode - get denoise value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU210::getDenoiseMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = GET_DENOISE_SEE3CAM_CU210;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == GET_DENOISE_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit denoiseModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU210::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_CU210::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID1_SEE3CAM_CU210; /* set camera control id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID2_SEE3CAM_CU210; /* set camera control id_2 */
    g_out_packet_buf[3] = SET_DEFAULT_SEE3CAM_CU210; /* Restore default */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID1_SEE3CAM_CU210 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID2_SEE3CAM_CU210 &&
            g_in_packet_buf[2] == SET_DEFAULT_SEE3CAM_CU210 &&
            g_in_packet_buf[6] == SET_SUCCESS){
            return true;
        }
    }
    return false;
}

/*
 *
 * @brief See3CAM_CU210::initializeBuffers - Initialize input and output buffers
 */
void See3CAM_CU210::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
