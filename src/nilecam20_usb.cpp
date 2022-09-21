/*
 * nilecam20_usb.cpp -- Handling special feature of nilecam30_usb camera
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

#include<nilecam20_usb.h>

NILECAM20_USB::NILECAM20_USB()
{
}

/**
 * @brief NILECAM20_USB::setSensorMode - setting sensor mode
 * @param sensorMode - standard/dsr hlo mode
 * @return true/false
 */
bool NILECAM20_USB::setSensorMode(sensorModes sensorMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_SENSOR_MODE_NILECAM20_USB; /* set sensor command  */
    g_out_packet_buf[3] = sensorMode; /* pass sensor mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_SENSOR_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getSensorMode - setting sensor mode
 * @return true/false
 */
bool NILECAM20_USB::getSensorMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_SENSOR_MODE_NILECAM20_USB; /* get sensor command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_SENSOR_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sensorModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setCameraMode - setting camera mode
 * @param cameraMode - master/slave
 * @return true/false
 */
bool NILECAM20_USB::setCameraMode(cameraModes cameraMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_CAMERA_MODE_NILECAM20_USB; /* set camera mode command  */
    g_out_packet_buf[3] = cameraMode; /* pass camera mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == SET_CAMERA_MODE_NILECAM20_USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getCameraMode - getting camera mode
 * @return true/false
 */
bool NILECAM20_USB::getCameraMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_CAMERA_MODE_NILECAM20_USB; /* get camera command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_CAMERA_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit cameraModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getSpecialMode - get effect mode set in camera
 * return true - success /false - failure
 */
bool NILECAM20_USB::getSpecialMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_SPECIAL_MODE_NILECAM20_USB; /* get special effect code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_SPECIAL_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS)
        {
            emit specialEffectMode(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setSpecialMode - setting special effects to the camera
 * return true - success /false - failure
 */
bool NILECAM20_USB::setSpecialMode(specialModes specialEffect)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* set camera control code */
    g_out_packet_buf[2] = SET_SPECIAL_MODE_NILECAM20_USB; /* set special effect Mode */
    g_out_packet_buf[3] = specialEffect; /* set special effect */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_SPECIAL_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/*
 * @brief NILECAM20_USB::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool NILECAM20_USB::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_ORIENTATION_NILECAM20_USB; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = Rotate180; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = HorizontalMirror; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = VerticalFlip; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = Normal; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == SET_ORIENTATION_NILECAM20_USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
 */
bool NILECAM20_USB::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_ORIENTATION_NILECAM20_USB; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == GET_ORIENTATION_NILECAM20_USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


bool NILECAM20_USB::setStrobeMode(strobeValues strobeMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_STROBE_MODE_NILECAM20_USB; /* set strobe mode command id */
    g_out_packet_buf[3] = strobeMode; /* pass strobe mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == SET_STROBE_MODE_NILECAM20_USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getStrobeMode - get Strobe mode
 * @return true/false
 */
bool NILECAM20_USB::getStrobeMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_STROBE_MODE_NILECAM20_USB; /* get strobe mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == GET_STROBE_MODE_NILECAM20_USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setDenoiseCtrlMode - setting denoise value to the camera
 * @param deNoiseVal - denoise value
 * return true - success /false - failure
 */
bool NILECAM20_USB::setDenoiseCtrlMode(int deNoiseVal)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* set camera control code */
    g_out_packet_buf[2] = SET_DENOISE_NILECAM20_USB; /* set denoise control code */
    g_out_packet_buf[3] = deNoiseVal; /* set denoise value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_DENOISE_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getDenoiseCtrlMode - get denoise value from camera
 * return true - success /false - failure
 */
bool NILECAM20_USB::getDenoiseCtrlMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_DENOISE_NILECAM20_USB; /* get denoise code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_DENOISE_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit sendDenoiseValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setExposureCompensation - setting exposure compensation
 * @param exposureCompValue - exposure compensation value
 * @return true/false
 */
bool NILECAM20_USB::setExposureCompensation(unsigned int exposureCompValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_EXPOSURE_COMPENSATION_NILECAM20_USB; /* set exposure compensation command */
    g_out_packet_buf[3] = exposureCompValue;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_EXPOSURE_COMPENSATION_NILECAM20_USB && g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getExposureCompensation - getting exposure compensation
 * @return true/false
 */
bool NILECAM20_USB::getExposureCompensation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* set camera control code */
    g_out_packet_buf[2] = GET_EXPOSURE_COMPENSATION_NILECAM20_USB; /* get exposure compensation code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]== GET_EXPOSURE_COMPENSATION_NILECAM20_USB && g_in_packet_buf[6]==GET_SUCCESS) {
            emit exposureCompValueReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setBurstLength - set burst length in camera
 * @param burstLength - burst length - no of images to be taken.
 * return true - success /false - failure
 */
bool NILECAM20_USB::setBurstLength(int burstLength)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_IMAGE_BURST_NILECAM20_USB; /* set burst length command */
    g_out_packet_buf[3] = burstLength; /* burst length value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_IMAGE_BURST_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getBurstLength - get burst length from camera
 * return true - success /false - failure
 */
bool NILECAM20_USB::getBurstLength()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_IMAGE_BURST_NILECAM20_USB; /* get burst length command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_IMAGE_BURST_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit burstLengthValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setColourKill - set Colour Kill value
 * @param colourKillValue - value to set
 * @return true/false
 */
bool NILECAM20_USB::setColourKill(uint colourKillValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_COLOR_KILL_MODE_NILECAM20_USB; /* set color kill status command  */
    g_out_packet_buf[3] = colourKillValue; /* pass colour kill status value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == SET_COLOR_KILL_MODE_NILECAM20_USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getColourKill - get color kill status value
 * @return true/false
 */
bool NILECAM20_USB::getColourKill()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_COLOR_KILL_MODE_NILECAM20_USB; /* get colour kill command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == GET_COLOR_KILL_MODE_NILECAM20_USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit colorKillValueChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool NILECAM20_USB::setROIAutoExposure(ROIAutoExpMode nileCam20USBAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera control id for camera NILECAM20_USB */
    g_out_packet_buf[2] = SET_AUTO_EXPOSURE_ROI_MODE_NILECAM20_USB; /* set Auto exposure ROI mode command  */
    g_out_packet_buf[3] = nileCam20USBAutoexpROIMode; /* ROI mode which is need to set */

    if(nileCam20USBAutoexpROIMode == AUTO_EXP_MANNUAL){
        g_out_packet_buf[4] = outputXCord;
        g_out_packet_buf[5] = outputYCord;
        g_out_packet_buf[6] = winSize.toUInt();
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_AUTO_EXPOSURE_ROI_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getROIAutoExposure - get ROI auto exposure mode
 * return true - success /false - failure
 */
bool NILECAM20_USB::getROIAutoExposure()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_AUTO_EXPOSURE_ROI_MODE_NILECAM20_USB; /* get Auto exposure ROI mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_AUTO_EXPOSURE_ROI_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS) {
                emit roiAutoExpModeValue(g_in_packet_buf[2], g_in_packet_buf[5]);
                return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setLSCMode - set LSC mode
 * @param LSC mode to set
 * @return true/false
 */
bool NILECAM20_USB::setLSCMode(lscModes lscMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_LSC_MODES_NILECAM20_USB; /* set Lsc mode command  */
    g_out_packet_buf[3] = lscMode; /* LSC Mode value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == SET_LSC_MODES_NILECAM20_USB &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::getLSCMode - get current LSC mode from camera
 * @return true/false
 */
bool NILECAM20_USB::getLSCMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_LSC_MODES_NILECAM20_USB; /* get LSC mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1] == GET_LSC_MODES_NILECAM20_USB &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit lscModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setAntiFlickerMode - setting the flicker control.
 * @param flickerMode - mode we set for camera
 * @return true/false
 */

bool NILECAM20_USB::setAntiFlickerMode(FlickerMode flickerMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_FLICKER_MODE_NILECAM20_USB; /* set flicker detection command */
    g_out_packet_buf[3] = flickerMode; /* flicker detection mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_FLICKER_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


 /*
  * @brief NILECAM20_USB::getAntiFlickerMode - getting the flicker control we set,from the camera.
  * @return true/false
 */

bool NILECAM20_USB::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = GET_FLICKER_MODE_NILECAM20_USB; /* get flicker detection command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==GET_FLICKER_MODE_NILECAM20_USB &&
            g_in_packet_buf[6]==GET_SUCCESS){
                emit flickerDetectionMode(g_in_packet_buf[2]);
                return true;
        }
    }
    return false;
}


/**
 * @brief NILECAM20_USB::setToDefaultValues - set all the values to default in camera
 * @return true/false
 */
bool NILECAM20_USB::setToDefaultValues()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_NILECAM20_USB; /* camera id */
    g_out_packet_buf[2] = SET_DEFAULT_NILECAM20_USB; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6]==SET_FAIL)
        {
            return false;
        }
        else if(g_in_packet_buf[0] == CAMERA_CONTROL_NILECAM20_USB &&
            g_in_packet_buf[1]==SET_DEFAULT_NILECAM20_USB &&
            g_in_packet_buf[6]==SET_SUCCESS){
            return true;
        }
    }
    return false;
}


/*
 *
 * @brief NILECAM20_USB::initializeBuffers - Initialize input and output buffers
 */
void NILECAM20_USB::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

