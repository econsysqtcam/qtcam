/*
 * see3cam_50cug_m.cpp -- Handling special feature of nilecam30_usb camera
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
 * @brief SEE3CAM_50CUGM::getCameraMode - To get status of camera mode from the camera
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
 * @brief SEE3CAM_50CUGM::setGainMode - setting Auto/manual Gain mode
 * @param - gainMode - To switch between Auto and Manual mode
 * @param - manualValues - To set the gain manually
 * return true/false
*/
bool SEE3CAM_50CUGM::setGainMode(GAIN_MODE gainMode, AUTO_GAIN_FEATURES autoGain, uint manualGain)
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
    g_out_packet_buf[2] = SET_GAIN_MODE_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = gainMode;
    g_out_packet_buf[4] = autoGain;
    g_out_packet_buf[5] = ((manualGain & 0xFF00) >> 8);
    g_out_packet_buf[6] = ((manualGain & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11]== SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_GAIN_MODE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[11] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getGainMode - To get status of gain mode from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getGainMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint current, min, max;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_GAIN_MODE_SEE3CAM_50CUG_M; /* get gain mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1]  == GET_GAIN_MODE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            current = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            min     = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            max     = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);

            emit gainModeReceived(g_in_packet_buf[2]);
            emit autoGainFeatureReceived(g_in_packet_buf[3]);
            emit manualGainValuesReceived(current, min, max, g_in_packet_buf[10]);

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
 * @brief SEE3CAM_50CUGM::setExposure - To set the exposure in the camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::setExposure(EXPOSURE_MODE expMode, AUTO_EXPOSURE_FEATURES autoExpFeature, uint seconds, uint milliSeconds, uint microSeconds)
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
    g_out_packet_buf[2] = SET_EXPOSURE_SEE3CAM_50CUG_M; /* set exposure command */
    g_out_packet_buf[3] = expMode; /* Exposure mode to set */

    if(g_out_packet_buf[3] == 0X00)
    {
        g_out_packet_buf[4] = autoExpFeature;
    }
    else if(g_out_packet_buf[3] == 0X01)
    {
        g_out_packet_buf[5]  = seconds;

        g_out_packet_buf[6] = ((milliSeconds & 0xFF00) >> 8);
        g_out_packet_buf[7] = ((milliSeconds & 0x00FF) >> 0);

        g_out_packet_buf[8] = ((microSeconds & 0xFF00) >> 8);
        g_out_packet_buf[9] = ((microSeconds & 0x00FF) >> 0);
    }
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_EXPOSURE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[9] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getExposure - To get exposure from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getExposure()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_EXPOSURE_SEE3CAM_50CUG_M; /* get exposure commands  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_EXPOSURE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            microSeconds = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

            emit exposureModeReceived(g_in_packet_buf[2]);
            emit autoExposureFeaturesReceived(g_in_packet_buf[3]);
            emit manualExposureValuesReceived(g_in_packet_buf[4], milliSeconds, microSeconds);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setTargetBrightness - To set the brightness in the camera
 * @param brightness - brightness value to be set
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::setTargetBrightness(uint brightness){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SET_TARGET_BRIGHTNESS_SEE3CAM_50CUG_M; /* set brightness command */

    /* Brightness value to set */
    g_out_packet_buf[7] = ((brightness & 0xFF00) >> 8);
    g_out_packet_buf[8] = ((brightness & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_TARGET_BRIGHTNESS_SEE3CAM_50CUG_M &&
            g_in_packet_buf[9] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::getTargetBrightness - get the brightness values from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::getTargetBrightness()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint min, max, current;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_TARGET_BRIGHTNESS_SEE3CAM_50CUG_M; /* get target brightness */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_TARGET_BRIGHTNESS_SEE3CAM_50CUG_M &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            min     = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);
            max     = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            current = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);

            emit minBrightnessReceived(min);
            emit maxBrightnessReceived(max);
            emit currentBrightnessReceived(current);
            emit brightnessStepValueReceived(g_in_packet_buf[8]);

            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_50CUGM::getAutoGainLimit() - To get gain upper and lower limit value in camera
 * return true - success / false - failure
 */
bool SEE3CAM_50CUGM::getAutoGainLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint lowerLimit, upperLimit;
    uint min, max;

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = GET_AUTO_GAIN_LIMIT_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1]  == GET_AUTO_GAIN_LIMIT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            min = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);
            max = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);

            lowerLimit = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            upperLimit = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);

            emit sliderMinimumReceived(min);
            emit sliderMaximumReceived(max);

            emit lowerGainLimitReceived(lowerLimit);
            emit upperGainLimitReceived(upperLimit);

            emit gainLimitStepValueReceived(g_in_packet_buf[10]);

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::setGainLimit - set gain limit value in camera
 * @param lowerLimit - set lower limit for gain
 * @param upperLimit - set upper limit for gain
 * return true - success /false - failure
 */
bool SEE3CAM_50CUGM::setAutoGainLimit(uint lowerLimit, uint upperLimit)
{
    if(lowerLimit == upperLimit)
    {

    }
    else if(lowerLimit > upperLimit)
    {
        emit indicateGainValueRangeFailure();
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
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_AUTO_GAIN_LIMIT_SEE3CAM_50CUG_M;
    g_out_packet_buf[7] = ((lowerLimit & 0xFF00) >> 8);
    g_out_packet_buf[8] = ((lowerLimit & 0x00FF) >> 0);
    g_out_packet_buf[9] = ((upperLimit & 0xFF00) >> 8);
    g_out_packet_buf[10] = ((upperLimit & 0x00FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_AUTO_GAIN_LIMIT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[11] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::getAutoExposureLowerLimit  - get current lower limit exposure from camera
 * @return - true/false
 */
bool SEE3CAM_50CUGM::getAutoExposureLowerLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_AUTOEXP_LOWER_LIMIT_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_AUTOEXP_LOWER_LIMIT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            microSeconds = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            emit lowerLimitExposure(g_in_packet_buf[2], milliSeconds, microSeconds);

            lowerLimitSec      = g_in_packet_buf[2];
            lowerLimitMilliSec = milliSeconds;
            lowerLimitMicroSec = microSeconds;

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setAutoExposureLowerLimit - setting lower limit for exposure
 * @param lowerLimit - mode to set lower limit for exposure
 * @return true/false
 */
bool SEE3CAM_50CUGM::setAutoExposureLowerLimit(uint seconds, uint milliSeconds, uint microSeconds){

    lowerLimitSec      = seconds;
    lowerLimitMilliSec = milliSeconds;
    lowerLimitMicroSec = microSeconds;

    if(lowerLimitSec > upperLimitSec)
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec > upperLimitMilliSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if (((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec == upperLimitMilliSec)) && (lowerLimitMicroSec > upperLimitMicroSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_AUTOEXP_LOWER_LIMIT_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = seconds;
    g_out_packet_buf[4] = ((milliSeconds & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((milliSeconds & 0x00FF) >> 0);
    g_out_packet_buf[6] = ((microSeconds & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((microSeconds & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_AUTOEXP_LOWER_LIMIT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[7] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_50CUGM::getAutoExposureUpperLimit  - get current upper limit exposure from camera
 * @return - true/false
 */
bool SEE3CAM_50CUGM::getAutoExposureUpperLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_AUTOEXP_UPPER_LIMIT_SEE3CAM_50CUG_M; /* get exposure upper limit command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_AUTOEXP_UPPER_LIMIT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            microSeconds = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            emit upperLimitExposure(g_in_packet_buf[2], milliSeconds, microSeconds);

            upperLimitSec      = g_in_packet_buf[2];
            upperLimitMilliSec = milliSeconds;
            upperLimitMicroSec = microSeconds;

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::setAutoExposureUpperLimit - setting upper limit for exposure
 * @param upperLimit - mode to set upper limit for exposure
 * @return true/false
 */
bool SEE3CAM_50CUGM::setAutoExposureUpperLimit(uint seconds, uint milliSeconds, uint microSeconds){

    upperLimitSec      = seconds;
    upperLimitMilliSec = milliSeconds;
    upperLimitMicroSec = microSeconds;

    if(lowerLimitSec > upperLimitSec)
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec > upperLimitMilliSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
        return false;
    }
    else if (((lowerLimitSec == upperLimitSec) && (lowerLimitMilliSec == upperLimitMilliSec)) && (lowerLimitMicroSec > upperLimitMicroSec))
    {
        emit indicateExposureLimitsInvalidInput("Invalid Input", "Lower auto exposure limit <= Upper auto exposure limit");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_AUTOEXP_UPPER_LIMIT_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = seconds;
    g_out_packet_buf[4] = ((milliSeconds & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((milliSeconds & 0x00FF) >> 0);
    g_out_packet_buf[6] = ((microSeconds & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((microSeconds & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            emit indicateCommandStatus("Failure","Set upper limit value failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_AUTOEXP_UPPER_LIMIT_SEE3CAM_50CUG_M &&
            g_in_packet_buf[7] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_50CUGM::getAntiFlickerMode  - get current anti flicker mode from camera
 * @return - true/false
 */
bool SEE3CAM_50CUGM::getAntiFlickerMode()
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
    g_out_packet_buf[2] = GET_ANTI_FLICKER_DETECTION_SEE3CAM_50CUG_M; /* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_ANTI_FLICKER_DETECTION_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setAntiFlickerMode - To set anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - auto/50hz/60hz]
 * @return true/false
 */
bool SEE3CAM_50CUGM::setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = SET_ANTI_FLICKER_DETECTION_SEE3CAM_50CUG_M; /* set anti flicker command  */
    g_out_packet_buf[3] = antiFlickerMode; /* anti flicker mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_ANTI_FLICKER_DETECTION_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_50CUGM::getStatistics - To get Gain & Exposure statistics when respective modes are in auto mode
 * @return true/false
 */
bool SEE3CAM_50CUGM::readStatistics(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds, microSeconds, gainStatistics;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = READ_STATISTICS_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == READ_STATISTICS_SEE3CAM_50CUG_M &&
            g_in_packet_buf[9] == SET_SUCCESS) {

            milliSeconds = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            microSeconds = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            gainStatistics = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

            emit exposureStatisticsReceived(g_in_packet_buf[2], milliSeconds, microSeconds);
            emit gainStatisticsReceived(gainStatistics);

            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_50CUGM::readTeperature() - To read the temperature in the camera
 * @return true/false
 */
bool SEE3CAM_50CUGM::readTemperature(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = READ_TEMPERATURE_SEE3CAM_50CUG_M;
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == READ_TEMPERATURE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            emit temperatureReceived(g_in_packet_buf[2]);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::getMultiExposureTrigger() - To get multi exposure values
 * @return true/false
 */
bool SEE3CAM_50CUGM::getMultiExposureTrigger(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint exposureCount;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M; /* camera id */
    g_out_packet_buf[2] = GET_MULTI_EXP_TRIGGER_SEE3CAM_50CUG_M;
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_MULTI_EXP_TRIGGER_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            exposureCount = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);

            emit multiExposureModeReceived(g_in_packet_buf[2]);
            emit multiExposureValueReceived(exposureCount);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setMultiExposureTrigger - To Enable/Disable Multi Exposure Trigger count option
 * @param metMode - Enable/Disable Multi Exposure Mode
 * @param exposureCount - Number of times of exposure
 * @return true/false
 */
bool SEE3CAM_50CUGM::setMultiExposureTrigger(MET_MODE mode, uint triggerCount)
{
    if((triggerCount > MULTI_EXPOSURE_TRIGGER_MAX) || (triggerCount < MULTI_EXPOSURE_TRIGGER_MIN))
    {
        emit indicateMultipleExposureTriggerFailure("Invalid Input","Value should be within the range of 2 to 4095.");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_MULTI_EXP_TRIGGER_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = mode;

    //If MET mode is enabled
    if(mode == 0x01)
    {
        g_out_packet_buf[4] = ((triggerCount & 0xFF00) >> 8);
        g_out_packet_buf[5] = ((triggerCount & 0x00FF) >> 0);
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_MULTI_EXP_TRIGGER_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::getMultiFrameSet -
 * @return true/false
 */
bool SEE3CAM_50CUGM::getMultiFrameSet(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint milliSeconds1, microSeconds1;
    uint milliSeconds2, microSeconds2;
    uint milliSeconds3, microSeconds3;
    uint milliSeconds4, microSeconds4;

    uint gain1, gain2, gain3, gain4;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = GET_MULTI_FRAMESET_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[31] == SET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_MULTI_FRAMESET_SEE3CAM_50CUG_M &&
            g_in_packet_buf[31] == SET_SUCCESS) {

            gain1 = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            gain2 = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            gain3 = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            gain4 = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            milliSeconds1 = (g_in_packet_buf[12] << 8) | (g_in_packet_buf[13] << 0);
            microSeconds1 = (g_in_packet_buf[14] << 8) | (g_in_packet_buf[15] << 0);

            milliSeconds2 = (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);
            microSeconds2 = (g_in_packet_buf[19] << 8) | (g_in_packet_buf[20] << 0);

            milliSeconds3 = (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);
            microSeconds3 = (g_in_packet_buf[24] << 8) | (g_in_packet_buf[25] << 0);

            milliSeconds4 = (g_in_packet_buf[27] << 8) | (g_in_packet_buf[28] << 0);
            microSeconds4 = (g_in_packet_buf[29] << 8) | (g_in_packet_buf[30] << 0);

            emit multiFrameSetModeReceived(g_in_packet_buf[2]);

            emit frameSet1Received(gain1, g_in_packet_buf[11], milliSeconds1, microSeconds1);
            emit frameSet2Received(gain2, g_in_packet_buf[16], milliSeconds2, microSeconds2);
            emit frameSet3Received(gain3, g_in_packet_buf[21], milliSeconds3, microSeconds3);
            emit frameSet4Received(gain4, g_in_packet_buf[26], milliSeconds4, microSeconds4);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setAutoExposureUpperLimit - setting upper limit for exposure
 * @param upperLimit - mode to set upper limit for exposure
 * @return true/false
 */
bool SEE3CAM_50CUGM::setMultiFrameSet(MULTIPLE_FRAME_SET mode, uint gain1, uint second1, uint milliSecond1, uint microSecond1, uint gain2, uint second2, uint milliSecond2, uint microSecond2,  uint gain3,uint second3, uint milliSecond3, uint microSecond3, uint gain4, uint second4, uint milliSecond4, uint microSecond4){

    if(((gain1 > GAIN_MAX) || (gain2 > GAIN_MAX) || (gain3 > GAIN_MAX) || (gain4 > GAIN_MAX)) || ((gain1 < GAIN_MIN) || (gain2 < GAIN_MIN) || (gain3 < GAIN_MIN) || (gain4 < GAIN_MIN)) )
    {
        emit indicateMultiFrameSetInvalidInput("Invalid Input","Value should be within the range of 0 to 48");
        return false;
    }

    if(((second1 > SECONDS_MAX) || (second2 > SECONDS_MAX) || (second3 > SECONDS_MAX) || (second4 > SECONDS_MAX)) || ((second1 < SECONDS_MIN) || (second2 < SECONDS_MIN) || (second3 < SECONDS_MIN) || (second4 < SECONDS_MIN)) )
    {
        emit indicateMultiFrameSetInvalidInput("Invalid Input","Value should be within the range of 0 to 14");
        return false;
    }

    if(((milliSecond1 > MILLISECONDS_MAX) || (milliSecond2 > MILLISECONDS_MAX) || (milliSecond3 > MILLISECONDS_MAX) || (milliSecond4 > MILLISECONDS_MAX)) || ((milliSecond1 < MILLISECONDS_MIN) || (milliSecond2 < MILLISECONDS_MIN) || (milliSecond3 < MILLISECONDS_MIN) || (milliSecond4 < MILLISECONDS_MIN)) )
    {
        emit indicateMultiFrameSetInvalidInput("Invalid Input","Value should be within the range of 0 to 999");
        return false;
    }

    if(((microSecond1 > MICROSECONDS_MAX) || (microSecond2 > MICROSECONDS_MAX) || (microSecond3 > MICROSECONDS_MAX) || (microSecond4 > MICROSECONDS_MAX)) || ((microSecond1 < MILLISECONDS_MIN) || (microSecond2 < MICROSECONDS_MIN) || (microSecond3 < MICROSECONDS_MIN) || (microSecond4 < MICROSECONDS_MIN)) )
    {
        emit indicateMultiFrameSetInvalidInput("Invalid Input","Value should be within the range of 0 to 999");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_MULTI_FRAMESET_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = mode;

    g_out_packet_buf[4] = ((gain1 & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((gain1 & 0x00FF) >> 0);

    g_out_packet_buf[6] = ((gain2 & 0xFF00) >> 8);
    g_out_packet_buf[7] = ((gain2 & 0x00FF) >> 0);

    g_out_packet_buf[8] = ((gain3 & 0xFF00) >> 8);
    g_out_packet_buf[9] = ((gain3 & 0x00FF) >> 0);

    g_out_packet_buf[10] = ((gain4 & 0xFF00) >> 8);
    g_out_packet_buf[11] = ((gain4 & 0x00FF) >> 0);

    g_out_packet_buf[12] = second1;
    g_out_packet_buf[13] = ((milliSecond1 & 0xFF00) >> 8);
    g_out_packet_buf[14] = ((milliSecond1 & 0x00FF) >> 0);
    g_out_packet_buf[15] = ((microSecond1 & 0xFF00) >> 8);
    g_out_packet_buf[16] = ((microSecond1 & 0x00FF) >> 0);

    g_out_packet_buf[17] = second2;
    g_out_packet_buf[18] = ((milliSecond2 & 0xFF00) >> 8);
    g_out_packet_buf[19] = ((milliSecond2 & 0x00FF) >> 0);
    g_out_packet_buf[20] = ((microSecond2 & 0xFF00) >> 8);
    g_out_packet_buf[21] = ((microSecond2 & 0x00FF) >> 0);

    g_out_packet_buf[22] = second3;
    g_out_packet_buf[23] = ((milliSecond3 & 0xFF00) >> 8);
    g_out_packet_buf[24] = ((milliSecond3 & 0x00FF) >> 0);
    g_out_packet_buf[25] = ((microSecond3 & 0xFF00) >> 8);
    g_out_packet_buf[26] = ((microSecond3 & 0x00FF) >> 0);

    g_out_packet_buf[27] = second4;
    g_out_packet_buf[28] = ((milliSecond4 & 0xFF00) >> 8);
    g_out_packet_buf[29] = ((milliSecond4 & 0x00FF) >> 0);
    g_out_packet_buf[30] = ((microSecond4 & 0xFF00) >> 8);
    g_out_packet_buf[31] = ((microSecond4 & 0x00FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[31] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_MULTI_FRAMESET_SEE3CAM_50CUG_M &&
            g_in_packet_buf[31] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::getUltraShortExposure() - To get Ultra Short Exposure Values
 * @return true/false
 */
bool SEE3CAM_50CUGM::getUltraShortExposure(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = GET_ULTRA_SHORT_EXPOSURE_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_ULTRA_SHORT_EXPOSURE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            emit ultraShortExposureModeReceived(g_in_packet_buf[2]);

            emit minUltraShortExposureReceived(g_in_packet_buf[4]);
            emit maxUltraShortExposureReceived(g_in_packet_buf[5]);
            emit currentUltraShortExpValueReceived(g_in_packet_buf[3]);

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::setUltraShortExposure() - To Enable/Disable Ultra Short Exposure mode
 * @param expMode - Enable/Disable Ultra Short Exposure
 * @param ultraShortExposure - exposure value to set
 * @return true/false
 */
bool SEE3CAM_50CUGM::setUltraShortExposure(ULTRA_SHORT_EXPOSURE mode, uint ultraShortExposure)
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
    g_out_packet_buf[2] = SET_ULTRA_SHORT_EXPOSURE_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = mode;
    g_out_packet_buf[4] = ultraShortExposure;

    //If Ultra Short Exposure mode is enabled
    if(mode == 0x01)
    {
        g_out_packet_buf[4] = ultraShortExposure;
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_ULTRA_SHORT_EXPOSURE_SEE3CAM_50CUG_M &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUGM::getSoftwareTrigger()  - To get Software Trigger from the camera
 * @return - true/false
 */
bool SEE3CAM_50CUGM::getSoftwareTrigger()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint min, max, current;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = GET_SOFTWARE_TRIGGER_SEE3CAM_50CUG_M;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == GET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == GET_SOFTWARE_TRIGGER_SEE3CAM_50CUG_M &&
            g_in_packet_buf[8] == GET_SUCCESS) {

            min     = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            max     = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            current = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);

            triggerMin = min;
            triggerMax = max;

            emit currentTriggerValueReceived(current);

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_50CUGM::setSoftwareTrigger() - To set software trigger value to the camera
 * @param triggerValue - Trigger value to be set
 * @return true/false
 */
bool SEE3CAM_50CUGM::setSoftwareTrigger(uint triggerValue){

    if((triggerValue > triggerMax) || (triggerValue < triggerMin))
    {
        emit indicateSoftwareTriggerFailure("Invalid Input","Software trigger value should be within the range of 0 to 255.");
        return false;
    }

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint current;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_SEE3CAM_50CUG_M;
    g_out_packet_buf[2] = SET_SOFTWARE_TRIGGER_SEE3CAM_50CUG_M;
    g_out_packet_buf[3] = ((triggerValue & 0xFF00) >> 8);
    g_out_packet_buf[4] = ((triggerValue & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_SEE3CAM_50CUG_M &&
            g_in_packet_buf[1] == SET_SOFTWARE_TRIGGER_SEE3CAM_50CUG_M &&
            g_in_packet_buf[8] == SET_SUCCESS) {

            current = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);

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
