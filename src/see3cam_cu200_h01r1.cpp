/*
 * see3cam_cu200_h01r1.cpp -- Handling special feature of See3CAM_CU200M_H01R1 camera
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

#include "see3cam_cu200_h01r1.h"

See3CAM_CU200M_H01R1::See3CAM_CU200M_H01R1()
{

}


See3CAM_CU200M_H01R1::~See3CAM_CU200M_H01R1()
{

}



/**
 *  @brief See3CAM_CU200M_H01R1::getGainMode - Retrieves the gain mode, along with the minimum, maximum, and step values, as well as features of both auto and manual gain modes from the camera.
 *  @return True if successful, false otherwise.
*/
bool See3CAM_CU200M_H01R1::getGainMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_GAIN_MODE_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_GAIN_MODE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            emit gainModeReceived(g_in_packet_buf[3], g_in_packet_buf[6], g_in_packet_buf[7], g_in_packet_buf[8], g_in_packet_buf[4], g_in_packet_buf[5]);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setGainMode - Setting the gain mode, auto gain features, and manual gain value on the camera.
 * @param gainMode   - Auto or Manual
 * @param autoGain   - Continious or Single Shot
 * @param manualGain - manual gain value
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setGainMode(GAIN_MODE gainMode, AUTO_GAIN_FEATURES autoGain, uint manualGain)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_GAIN_MODE_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = gainMode;

    if(gainMode == AUTO_GAIN){
        g_out_packet_buf[5] = autoGain;
    } else if(gainMode == MANUAL_GAIN) {
        g_out_packet_buf[6] = manualGain;
    }
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_GAIN_MODE_SEE3CAM_CU200_H01R1  &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getAutoGainLimit - To get gain upper and lower limit value in camera
 * return true - success /false - failure
 */
bool See3CAM_CU200M_H01R1::getAutoGainLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_AUTO_GAIN_LIMIT_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[8] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2]  == GET_AUTO_GAIN_LIMIT_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[8]  == GET_SUCCESS) {

            emit currentAutoGainLimitValuesReceived(g_in_packet_buf[5],g_in_packet_buf[6],g_in_packet_buf[7],g_in_packet_buf[3],g_in_packet_buf[4]);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setGainLimit - set gain limit value in camera
 * @param lowerLimit - Lower limit to be set
 * @param upperLimit - Upper limit to be set
 * return true - success /false - failure
 */
bool See3CAM_CU200M_H01R1::setAutoGainLimit(uint lowerLimit, uint upperLimit)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_AUTO_GAIN_LIMIT_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = lowerLimit;
    g_out_packet_buf[5] = upperLimit;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2]  == SET_AUTO_GAIN_LIMIT_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6]  == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getExposure - Getting exposure value from the camera
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::getExposure()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint manualExposure;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_EXPOSURE_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_EXPOSURE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            manualExposure  = (g_in_packet_buf[5] << 24) | (g_in_packet_buf[6] << 16) | (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            emit exposurePropertiesReceived(g_in_packet_buf[3], g_in_packet_buf[4], manualExposure);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setExposure - Setting Exposure to the camera
 * exposure - exposure value from UI
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setExposure(EXPOSURE_MODE expMode, AUTO_EXPOSURE_FEATURE autoExpFeature, uint32_t manualExposure)
{
    if(EXPOSURE_MIN > manualExposure || EXPOSURE_MAX < manualExposure){
        emit indicateExposureValueRangeFailure("Failure", "Given exposure value is invalid");
        return false;
    }

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_EXPOSURE_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = expMode;
    g_out_packet_buf[5] = autoExpFeature;
    g_out_packet_buf[6] = ((manualExposure & 0xFF000000) >> 24);
    g_out_packet_buf[7] = ((manualExposure & 0x00FF0000) >> 16);
    g_out_packet_buf[8] = ((manualExposure & 0x0000FF00) >> 8);
    g_out_packet_buf[9] = ((manualExposure & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            emit indicateExposureValueRangeFailure("Failure", "Exposure Set Failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_EXPOSURE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            emit indicateCommandStatus("Success", "Exposure set successfully");
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getAutoExposureLimit - To retrieve auto exposure limit from the camera
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::getAutoExposureLimit()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint lowerLimit, upperLimit;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_AUTO_EXPOSURE_LIMIT_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if((g_in_packet_buf[0]  == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1) &&
                  (g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1) &&
                  (g_in_packet_buf[2]  == GET_AUTO_EXPOSURE_LIMIT_SEE3CAM_CU200_H01R1) &&
                  (g_in_packet_buf[11] == GET_SUCCESS)) {

            lowerLimit  = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            upperLimit  = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            emit currentAutoExposureLimitReceived(lowerLimit, upperLimit);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setAutoExposureLimit - Setting auto exposure limit values to the camera
 * @param lowerLimit - Lower limit to be set
 * @param upperLimit - Upper limit to be set
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setAutoExposureLimit(uint lowerLimit, uint upperLimit)
{
    exposureLowerLimit = lowerLimit;
    exposureUpperLimit = upperLimit;

    if((exposureLowerLimit < EXPOSURE_MIN) || (exposureUpperLimit > EXPOSURE_MAX))
    {
        emit indicateExposureValueRangeFailure("Failure", "Invalid Lower Limit Value");
        return false;
    }

    //Validating the limit values -> Lower auto exposure limit <= Upper auto exposure limit
    if(exposureLowerLimit > exposureUpperLimit)
    {
      emit indicateExposureValueRangeFailure("Failure", "Exposure upper limit should be greater than Exposure lower limit");
      return false;
    }

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_AUTO_EXPOSURE_LIMIT_SEE3CAM_CU200_H01R1;

    g_out_packet_buf[4] = ((lowerLimit & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((lowerLimit & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((lowerLimit & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((lowerLimit & 0x000000FF) >> 0);

    g_out_packet_buf[8]  = ((upperLimit & 0xFF000000) >> 24);
    g_out_packet_buf[9]  = ((upperLimit & 0x00FF0000) >> 16);
    g_out_packet_buf[10] = ((upperLimit & 0x0000FF00) >> 8);
    g_out_packet_buf[11] = ((upperLimit & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2]  == SET_AUTO_EXPOSURE_LIMIT_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            emit indicateCommandStatus("Success", "Set Exposure Limit Success");

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getTargetBrightness - To get the targeted brightness from the camera
 * return true - success /false - failure
 */
bool See3CAM_CU200M_H01R1::getTargetBrightness()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint targetBrightness, min, max;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_TARGET_BRIGHTNESS_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2]  == GET_TARGET_BRIGHTNESS_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[10] == GET_SUCCESS) {
            targetBrightness = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            min = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            max = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

            emit targetBrightnessPropertiesReceived(min, max, g_in_packet_buf[9], targetBrightness);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setTargetBrightness - set brightness value in camera
 * @param targetBrightness - set brightness value to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU200M_H01R1::setTargetBrightness(uint targetBrightness)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_TARGET_BRIGHTNESS_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = ((targetBrightness & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((targetBrightness & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_TARGET_BRIGHTNESS_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */

bool See3CAM_CU200M_H01R1::getAutoExpROIModeAndWindowSize(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_AUTO_EXP_ROI_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_AUTO_EXP_ROI_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit roiAutoExpModeReceived(g_in_packet_buf[3], g_in_packet_buf[6]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_CU200M_H01R1::setROIAutoExposure(autoExpRoiModes see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

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

    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_AUTO_EXP_ROI_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = see3camAutoexpROIMode; /* ROI mode which is need to set */

    g_out_packet_buf[5] = outputXCord;
    g_out_packet_buf[6] = outputYCord;
    g_out_packet_buf[7] = winSize.toUInt();

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_AUTO_EXP_ROI_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getAntiFlickerMode - get current anti flicker mode from camera
 * @return - true/false
 */
bool See3CAM_CU200M_H01R1::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setAntiFlickerMode - setting Anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - 50hz/60hz/Disable]
 * @return true/false
 */
bool See3CAM_CU200M_H01R1::setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = antiFlickerMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}




/**
 * @brief See3CAM_CU200M_H01R1::getBlackLevel - Getting Black Level Value from the camera
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::getBlackLevel()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint min, max, current;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_BLACK_LEVEL_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_BLACK_LEVEL_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[10] == GET_SUCCESS) {

            current  = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            min = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            max = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

            emit blacklevelPropertiesReceived(min, max, g_in_packet_buf[9], current);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setBlackLevel - Setting Black Level Value to the camera
 * @param blackLevel - Black level to be set
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setBlackLevel(uint blackLevel)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_BLACK_LEVEL_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = ((blackLevel & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((blackLevel & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_BLACK_LEVEL_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getCameraMode - Getting Camera mode from the camera
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::getCameraMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_CAMERA_MODE_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_CAMERA_MODE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit cameraModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setCameraMode - Setting Camera Mode to the camera
 * camMode - Type of Camera Mode from UI
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setCameraMode(CAMERA_MODE camMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_CAMERA_MODE_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = camMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_CAMERA_MODE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getStrobeMode - Getting strobe mode from the camera
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::getStrobeMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = GET_STROBE_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_STROBE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setStrobeMode - Setting Strobe Mode to the camera
 * @param strobeMode - Strobe mode to be set
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setStrobeMode(STROBE_MODE strobeMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_STROBE_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = strobeMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_STROBE_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
*/
bool See3CAM_CU200M_H01R1::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1; /* camera id_2 */
    g_out_packet_buf[3] = GET_ORIENTATION_SEE3CAM_CU200_H01R1; /* get orientation command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == GET_ORIENTATION_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit flipMirrorModeChanged(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool See3CAM_CU200M_H01R1::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1; /* camera id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1; /* camera id_2 */
    g_out_packet_buf[3] = SET_ORIENTATION_SEE3CAM_CU200_H01R1; /* set orientation command  */

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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_ORIENTATION_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::readStatistics - To retrieve the current auto mode values of exposure and gain from the camera.
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::readStatistics(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint currentExposure;

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = READ_STATISTICS_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
                  g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2]  == READ_STATISTICS_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[8] == SET_SUCCESS) {

            currentExposure  = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            emit exposureStatisticsReceived(currentExposure);
            emit gainStatisticsReceived(g_in_packet_buf[7]);

            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setPropertiesForCrossStill - To set still properties while taking still in cross resolution
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setStillProperties(bool isEnable){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_STILL_PROPERTIES_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[4] = isEnable;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
                  g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_STILL_PROPERTIES_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::readISPFirmwareVersion - To read the firmware version of ISP
 * @return true/false
 */
bool See3CAM_CU200M_H01R1::readISPFirmwareVersion()
{
    _title = tr("ISP Version");

    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    bool timeout = true;
    int ret = 0;
    unsigned int start, end = 0;

    //Initialize the buffer
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1; 	/* control_id_2 */
    g_out_packet_buf[3] = READ_ISP_FIRMWARE_VERSION_SEE3CAM_CU200_H01R1; /* Id to get ISP version */

    /* Send a Report to the Device */
    ret = write(uvccamera::hid_fd, g_out_packet_buf, BUFFER_LENGTH);
    if (ret < 0) {
        _text = tr("Device not available");
        return false;
    }

    /* Read the Firmware Version from the device */
    start = uvc.getTickCount();

    while(timeout)
    {
        /* Get a report from the device */
        ret = read(uvccamera::hid_fd, g_in_packet_buf, BUFFER_LENGTH);
        if (ret < 0) {
        } else {
            if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
               g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
               g_in_packet_buf[2] == READ_ISP_FIRMWARE_VERSION_SEE3CAM_CU200_H01R1) {

                timeout = false;
            }
            timeout = false;
        }
        end = uvc.getTickCount();
        if(end - start > TIMEOUT)
        {
            timeout = false;
            return false;
        }
    }

    _text.clear();
    _text.append("Version: ");
    _text.append(QString::number(g_in_packet_buf[3]).append(".").append(QString::number(g_in_packet_buf[4])).append(".").append(QString::number(g_in_packet_buf[5])).append(".").append(QString::number(g_in_packet_buf[6])));
    emit titleTextChanged(_title,_text);
    return true;
}




/**
 * @brief See3CAM_CU200M_H01R1::saveConfiguration
 * @param configMode - To set default / User defined configuration
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::saveConfiguration(SAVECONFIGURATION configMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION_SEE3CAM_CU200_H01R1; /* camera id */
    g_out_packet_buf[2] = configMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION_SEE3CAM_CU200_H01R1  &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations Saved Successfully");
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::resetDevice - To Reset the device
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::resetDevice()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = RESET_COMMAND_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = RESET_COMMAND_2_SEE3CAM_CU200_H01R1;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == RESET_COMMAND_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == RESET_COMMAND_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::setToDefault - set all the values to default in camera
 * @return True if successful, false otherwise.
 */
bool See3CAM_CU200M_H01R1::setToDefault(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1;
    g_out_packet_buf[3] = SET_TO_DEFAULT_SEE3CAM_CU200_H01R1; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[2] == SET_TO_DEFAULT_SEE3CAM_CU200_H01R1 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief See3CAM_CU200M_H01R1::initializeBuffers - Initialize input and output buffers
 */
void See3CAM_CU200M_H01R1::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

