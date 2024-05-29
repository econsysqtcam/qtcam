/*
 * see3cam_cu200.cpp -- Handling special feature of See3CAM_CU200 camera
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

#include "see3cam_cu200.h"

#include <cmath>
#include <iostream>
#include <iomanip> //Header for std::setprecision

SEE3CAM_CU200::SEE3CAM_CU200()
{

}


SEE3CAM_CU200::~SEE3CAM_CU200()
{

}


/**
 *  @brief SEE3CAM_CU200::getGainMode - Retrieves the gain mode, along with the minimum, maximum, and step values, as well as features of both auto and manual gain modes from the camera.
 *  @return True if successful, false otherwise.
*/
bool SEE3CAM_CU200::getGainMode()
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
    g_out_packet_buf[3] = GET_GAIN_MODE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_GAIN_MODE &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            emit gainModeReceived(g_in_packet_buf[3], g_in_packet_buf[6], g_in_packet_buf[7], g_in_packet_buf[8], g_in_packet_buf[4], g_in_packet_buf[5]);

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::setGainMode - Setting the gain mode, auto gain features, and manual gain value on the camera.
 * @param gainMode   - Auto or Manual
 * @param autoGain   - Continious or Single Shot
 * @param manualGain - manual gain value
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setGainMode(GAIN_MODE gainMode, AUTO_GAIN_FEATURES autoGain, uint manualGain)
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
    g_out_packet_buf[3] = SET_GAIN_MODE;
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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_GAIN_MODE  &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_CU200::getRBGain - Retrieves RB Gain Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getRBGain()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float currentRGain, minRGain, maxRGain, RGainStepValue;
    float currentBGain, minBGain, maxBGain, BGainStepValue;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_RB_GAIN;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[35] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == GET_RB_GAIN &&
            g_in_packet_buf[35] == GET_SUCCESS) {

                fCurrentRGain.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
                fRGainMin.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
                fRGainMax.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
                fRGainStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

                fCurrentBGain.u   = (g_in_packet_buf[19] << 24) | (g_in_packet_buf[20] << 16) | (g_in_packet_buf[21] << 8) | (g_in_packet_buf[22] << 0);
                fBGainMin.u       = (g_in_packet_buf[23] << 24) | (g_in_packet_buf[24] << 16) | (g_in_packet_buf[25] << 8) | (g_in_packet_buf[26] << 0);
                fBGainMax.u       = (g_in_packet_buf[27] << 24) | (g_in_packet_buf[28] << 16) | (g_in_packet_buf[29] << 8) | (g_in_packet_buf[30] << 0);
                fBGainStepValue.u = (g_in_packet_buf[31] << 24) | (g_in_packet_buf[32] << 16) | (g_in_packet_buf[33] << 8) | (g_in_packet_buf[34] << 0);

                currentRGain   = fCurrentRGain.f;
                minRGain       = fRGainMin.f;
                maxRGain       = fRGainMax.f;
                RGainStepValue = fRGainStepValue.f;

                currentBGain   = fCurrentBGain.f;
                minBGain       = fBGainMin.f;
                maxBGain       = fBGainMax.f;
                BGainStepValue = fBGainStepValue.f;
                emit rGainPropertiesReceived(minRGain, maxRGain, RGainStepValue, currentRGain);
                emit bGainPropertiesReceived(minBGain, maxBGain, BGainStepValue, currentBGain);
          }

            return true;
        }
}


/**
 * @brief SEE3CAM_CU200::setRGBGain - Setting RBGain Value to the camera
 * RGBGain - RB Gain value to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setRBGain(float RGain, float BGain)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fCurrentRGain.f = RGain;
    fCurrentBGain.f = BGain;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_RB_GAIN;

    g_out_packet_buf[4] = ((fCurrentRGain.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentRGain.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentRGain.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentRGain.u & 0x000000FF) >> 0);

    g_out_packet_buf[8]  = ((fCurrentBGain.u & 0xFF000000) >> 24);
    g_out_packet_buf[9]  = ((fCurrentBGain.u  & 0x00FF0000) >> 16);
    g_out_packet_buf[10] = ((fCurrentBGain.u  & 0x0000FF00) >> 8);
    g_out_packet_buf[11] = ((fCurrentBGain.u  & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_RB_GAIN &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getAutoGainLimit - To get gain upper and lower limit value in camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU200::getAutoGainLimit()
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
    g_out_packet_buf[3] = GET_AUTO_GAIN_LIMIT;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[8] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == GET_AUTO_GAIN_LIMIT &&
            g_in_packet_buf[8]  == GET_SUCCESS) {

            emit currentAutoGainLimitValuesReceived(g_in_packet_buf[5],g_in_packet_buf[6],g_in_packet_buf[7],g_in_packet_buf[3],g_in_packet_buf[4]);

            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU200::setGainLimit - set gain limit value in camera
 * @param lowerLimit - Lower limit to be set
 * @param upperLimit - Upper limit to be set
 * return true - success /false - failure
 */
bool SEE3CAM_CU200::setAutoGainLimit(uint lowerLimit, uint upperLimit)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AUTO_GAIN_LIMIT;
    g_out_packet_buf[4] = lowerLimit;
    g_out_packet_buf[5] = upperLimit;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == SET_AUTO_GAIN_LIMIT &&
            g_in_packet_buf[6]  == GET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU200::getColorCorrectionMatrix - Getting Color correction matrix elements from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getColorCorrectionMatrix()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float currentRr, currentRg, currentRb;
    float currentGr, currentGg, currentGb;
    float currentBr, currentBg, currentBb;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_COLOR_CORRECTION_MATRIX;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[39] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == GET_COLOR_CORRECTION_MATRIX &&
            g_in_packet_buf[39] == GET_SUCCESS) {

            fCurrentRr.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fCurrentRg.u = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fCurrentRb.u = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);

            fCurrentGr.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);
            fCurrentGg.u = (g_in_packet_buf[19] << 24) | (g_in_packet_buf[20] << 16) | (g_in_packet_buf[21] << 8) | (g_in_packet_buf[22] << 0);
            fCurrentGb.u = (g_in_packet_buf[23] << 24) | (g_in_packet_buf[24] << 16) | (g_in_packet_buf[25] << 8) | (g_in_packet_buf[26] << 0);

            fCurrentBr.u  = (g_in_packet_buf[27] << 24) | (g_in_packet_buf[28] << 16) | (g_in_packet_buf[29] << 8) | (g_in_packet_buf[30] << 0);
            fCurrentBg.u  = (g_in_packet_buf[31] << 24) | (g_in_packet_buf[32] << 16) | (g_in_packet_buf[33] << 8) | (g_in_packet_buf[34] << 0);
            fCurrentBb.u  = (g_in_packet_buf[35] << 24) | (g_in_packet_buf[36] << 16) | (g_in_packet_buf[37] << 8) | (g_in_packet_buf[38] << 0);

            currentRr = fCurrentRr.f;
            currentRg = fCurrentRg.f;
            currentRb = fCurrentRb.f;

            currentGr = fCurrentGr.f;
            currentGg = fCurrentGg.f;
            currentGb = fCurrentGb.f;

            currentBr = fCurrentBr.f;
            currentBg = fCurrentBg.f;
            currentBb = fCurrentBb.f;


            emit currentRrValuesReceived(currentRr);
            emit currentRgValuesReceived(currentRg);
            emit currentRbValuesReceived(currentRb);

            emit currentGrValuesReceived(currentGr);
            emit currentGgValuesReceived(currentGg);
            emit currentGbValuesReceived(currentGb);

            emit currentBrValuesReceived(currentBr);
            emit currentBgValuesReceived(currentBg);
            emit currentBbValuesReceived(currentBb);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setColorCorrectionMatrix - Setting the values for Color correction elements to the camera
 * Rr, Rg, Rb, Gr, Gg, Gb, Br, Bg, Bb - Color Correction Matrix Elements to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setColorCorrectionMatrix(float Rr, float Rg, float Rb, float Gr, float Gg, float Gb, float Br, float Bg, float Bb)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fCurrentRr.f = Rr;
    fCurrentRg.f = Rg;
    fCurrentRb.f = Rb;

    fCurrentGr.f = Gr;
    fCurrentGg.f = Gg;
    fCurrentGb.f = Gb;

    fCurrentBr.f = Br;
    fCurrentBg.f = Bg;
    fCurrentBb.f = Bb;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_COLOR_CORRECTION_MATRIX;

    g_out_packet_buf[4] = ((fCurrentRr.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentRr.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentRr.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentRr.u & 0x000000FF) >> 0);

    g_out_packet_buf[8]  = ((fCurrentRg.u & 0xFF000000) >> 24);
    g_out_packet_buf[9]  = ((fCurrentRg.u & 0x00FF0000) >> 16);
    g_out_packet_buf[10] = ((fCurrentRg.u & 0x0000FF00) >> 8);
    g_out_packet_buf[11] = ((fCurrentRg.u & 0x000000FF) >> 0);

    g_out_packet_buf[12] = ((fCurrentRb.u & 0xFF000000) >> 24);
    g_out_packet_buf[13] = ((fCurrentRb.u & 0x00FF0000) >> 16);
    g_out_packet_buf[14] = ((fCurrentRb.u & 0x0000FF00) >> 8);
    g_out_packet_buf[15] = ((fCurrentRb.u & 0x000000FF) >> 0);



    g_out_packet_buf[16] = ((fCurrentGr.u & 0xFF000000) >> 24);
    g_out_packet_buf[17] = ((fCurrentGr.u & 0x00FF0000) >> 16);
    g_out_packet_buf[18] = ((fCurrentGr.u & 0x0000FF00) >> 8);
    g_out_packet_buf[19] = ((fCurrentGr.u & 0x000000FF) >> 0);

    g_out_packet_buf[20] = ((fCurrentGg.u & 0xFF000000) >> 24);
    g_out_packet_buf[21] = ((fCurrentGg.u & 0x00FF0000) >> 16);
    g_out_packet_buf[22] = ((fCurrentGg.u & 0x0000FF00) >> 8);
    g_out_packet_buf[23] = ((fCurrentGg.u & 0x000000FF) >> 0);

    g_out_packet_buf[24] = ((fCurrentGb.u & 0xFF000000) >> 24);
    g_out_packet_buf[25] = ((fCurrentGb.u & 0x00FF0000) >> 16);
    g_out_packet_buf[26] = ((fCurrentGb.u & 0x0000FF00) >> 8);
    g_out_packet_buf[27] = ((fCurrentGb.u & 0x000000FF) >> 0);



    g_out_packet_buf[28] = ((fCurrentBr.u & 0xFF000000) >> 24);
    g_out_packet_buf[29] = ((fCurrentBr.u & 0x00FF0000) >> 16);
    g_out_packet_buf[30] = ((fCurrentBr.u & 0x0000FF00) >> 8);
    g_out_packet_buf[31] = ((fCurrentBr.u & 0x000000FF) >> 0);

    g_out_packet_buf[32] = ((fCurrentBg.u & 0xFF000000) >> 24);
    g_out_packet_buf[33] = ((fCurrentBg.u & 0x00FF0000) >> 16);
    g_out_packet_buf[34] = ((fCurrentBg.u & 0x0000FF00) >> 8);
    g_out_packet_buf[35] = ((fCurrentBg.u & 0x000000FF) >> 0);

    g_out_packet_buf[36] = ((fCurrentBb.u & 0xFF000000) >> 24);
    g_out_packet_buf[37] = ((fCurrentBb.u & 0x00FF0000) >> 16);
    g_out_packet_buf[38] = ((fCurrentBb.u & 0x0000FF00) >> 8);
    g_out_packet_buf[39] = ((fCurrentBb.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[39] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == SET_COLOR_CORRECTION_MATRIX &&
            g_in_packet_buf[39] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getBlackLevel - Getting Black Level Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getBlackLevel()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint minValue, maxValue, current;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_BLACK_LEVEL;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_BLACK_LEVEL &&
            g_in_packet_buf[10] == GET_SUCCESS) {

            current  = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            minValue = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            maxValue = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

            emit minBlackLevelReceived(minValue);
            emit maxBlackLevelReceived(maxValue);
            emit currentBlackLevelReceived(current);
            emit blackLevelStepValueReceived(g_in_packet_buf[9]);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setBlackLevel - Setting Black Level Value to the camera
 * @param blackLevel - Black level to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setBlackLevel(uint blackLevel)
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
    g_out_packet_buf[3] = SET_BLACK_LEVEL;
    g_out_packet_buf[4] = ((blackLevel & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((blackLevel & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_BLACK_LEVEL &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getBrightness - Getting Brightness Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getBrightness()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float current, min, max, stepValue;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_BRIGHTNESS;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[19] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_BRIGHTNESS &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            fMinBrightness.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxBrightness.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fCurrentBrightness.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fBrightnessStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            current   = fCurrentBrightness.f;
            min       = fMinBrightness.f;
            max       = fMaxBrightness.f;
            stepValue = fBrightnessStepValue.f;

            emit brightnessPropertiesReceived(min, max, stepValue, current);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setBrightness - Setting Brightness Value to the camera
 * brightness - Brightness to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setBrightness(float brightness)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fCurrentBrightness.f = brightness;

    //Initializing buffers
    initializeBuffers();
    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_BRIGHTNESS;
    g_out_packet_buf[4] = ((fCurrentBrightness.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentBrightness.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentBrightness.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentBrightness.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_BRIGHTNESS &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setTargetBrightness - set brightness value in camera
 * @param targetBrightness - set brightness value to the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU200::setTargetBrightness(uint targetBrightness)
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
    g_out_packet_buf[3] = SET_TARGET_BRIGHTNESS;
    g_out_packet_buf[4] = ((targetBrightness & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((targetBrightness & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_TARGET_BRIGHTNESS &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getTargetBrightness - To get the targeted brightness from the camera
 * return true - success /false - failure
 */
bool SEE3CAM_CU200::getTargetBrightness()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint targetBrightness, min, max;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_TARGET_BRIGHTNESS;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == GET_TARGET_BRIGHTNESS &&
            g_in_packet_buf[10] == GET_SUCCESS) {
            targetBrightness = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);
            min = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            max = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);

            emit targetBrightnessPropertiesReceived(min, max, g_in_packet_buf[9], targetBrightness);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getContrast - Getting Contrast Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getContrast()
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
    g_out_packet_buf[3] = GET_CONTRAST;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_CONTRAST &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            emit minContrastReceived(g_in_packet_buf[4]);
            emit maxContrastReceived(g_in_packet_buf[5]);
            emit currentContrastReceived(g_in_packet_buf[3]);
            emit contrastStepValueReceived(g_in_packet_buf[6]);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setContrast - Setting contrast Value to the camera
 * contrast - Contrast to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setContrast(uint contrast)
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
    g_out_packet_buf[3] = SET_CONTRAST;
    g_out_packet_buf[4] = contrast;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_CONTRAST &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getSaturation - Getting Saturation Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getSaturation()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float current, min, max, stepValue;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_SATURATION;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[19] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_SATURATION &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            fCurrentSaturation.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fMinSaturation.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxSaturation.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fSaturationStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            current   = fCurrentSaturation.f;
            min       = fMinSaturation.f;
            max       = fMaxSaturation.f;
            stepValue = fSaturationStepValue.f;

            emit saturationPropertiesReceived(min, max, stepValue, current);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setSaturation - Setting saturation Value to the camera
 * saturation - Saturation to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setSaturation(float saturation)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fCurrentSaturation.f = saturation;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_SATURATION;
    g_out_packet_buf[4] = ((fCurrentSaturation.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentSaturation.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentSaturation.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentSaturation.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_SATURATION &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getColorTemperature - Getting Color Temperature Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getColorTemperature()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint colorTemp, min, max, step;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_COLOR_TEMPERATURE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_COLOR_TEMPERATURE &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            colorTemp = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);

            min  = (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            max  = (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            step = (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            emit colorTemperatureReceived(min, max, step, colorTemp);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setColorTemperature - Setting color temperature to the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setColorTemperature(uint colorTemp)
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
    g_out_packet_buf[3] = SET_COLOR_TEMPERATURE;
    g_out_packet_buf[4] = ((colorTemp & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((colorTemp & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_COLOR_TEMPERATURE &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getGammaCorrection - Getting Gamma Value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getGammaCorrection()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float current, min, max, stepValue;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_GAMMA;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[19] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == GET_GAMMA &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            fCurrentGamma.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fMinGamma.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxGamma.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fGammaStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            current   = fCurrentGamma.f;
            min       = fMinGamma.f;
            max       = fMaxGamma.f;
            stepValue = fGammaStepValue.f;

            emit gammaPropertiesReceived(min, max, stepValue, current);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setGammaCorrection - Setting Gamma Value to the camera
 * gammaCorrection - Gamma value to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setGammaCorrection(float gammaCorrection)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fCurrentGamma.f = gammaCorrection;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_GAMMA;
    g_out_packet_buf[4] = ((fCurrentGamma.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentGamma.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentGamma.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentGamma.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_GAMMA &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getExposure - Getting exposure value from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getExposure()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint manualExposure;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_EXPOSURE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[9] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_EXPOSURE &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            manualExposure  = (g_in_packet_buf[5] << 24) | (g_in_packet_buf[6] << 16) | (g_in_packet_buf[7] << 8) | (g_in_packet_buf[8] << 0);
            emit exposurePropertiesReceived(g_in_packet_buf[3], g_in_packet_buf[4], manualExposure);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setExposure - Setting Exposure to the camera
 * exposure - exposure value from UI
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setExposure(EXPOSURE_MODE expMode, AUTO_EXPOSURE_FEATURE autoExpFeature, uint32_t manualExposure)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_EXPOSURE;
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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_EXPOSURE &&
            g_in_packet_buf[9] == GET_SUCCESS) {

            emit indicateCommandStatus("Success", "Exposure set successfully");
            return true;
        }
    }
}

/**
 * @brief SEE3CAM_CU200::getAutoExposureLimit - To retrieve auto exposure limit from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getAutoExposureLimit()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint lowerLimit, upperLimit;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = GET_AUTO_EXPOSURE_LIMIT;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if((g_in_packet_buf[0]  == CAMERA_CONTROL_ID_1) &&
                  (g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2) &&
                  (g_in_packet_buf[2]  == GET_AUTO_EXPOSURE_LIMIT) &&
                  (g_in_packet_buf[11] == GET_SUCCESS)) {

            lowerLimit  = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            upperLimit  = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);

            emit currentAutoExposureLimitReceived(lowerLimit, upperLimit);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setAutoExposureLimit - Setting auto exposure limit values to the camera
 * @param lowerLimit - Lower limit to be set
 * @param upperLimit - Upper limit to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setAutoExposureLimit(uint lowerLimit, uint upperLimit)
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AUTO_EXPOSURE_LIMIT;

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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == SET_AUTO_EXPOSURE_LIMIT &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            emit indicateCommandStatus("Success", "Set Exposure Limit Success");

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool SEE3CAM_CU200::setROIAutoExposure(autoExpRoiModes see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = SET_AUTO_EXPOSURE_ROI;
    g_out_packet_buf[4] = see3camAutoexpROIMode; /* ROI mode which is need to set */

    g_out_packet_buf[5] = outputXCord;
    g_out_packet_buf[6] = outputYCord;
    g_out_packet_buf[7] = winSize.toUInt();

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_AUTO_EXPOSURE_ROI &&
            g_in_packet_buf[7] == SET_SUCCESS) {
            return true;
        }
    }

    return false;
}



/**
 * @brief SEE3CAM_CU200::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */

bool SEE3CAM_CU200::getAutoExpROIModeAndWindowSize(){
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
    g_out_packet_buf[3] = GET_AUTO_EXPOSURE_ROI;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_AUTO_EXPOSURE_ROI &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            emit roiAutoExpModeReceived(g_in_packet_buf[3], g_in_packet_buf[6]);
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU200::getCameraMode - Getting Camera mode from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getCameraMode()
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
    g_out_packet_buf[3] = GET_CAMERA_MODE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_CAMERA_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit cameraModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setCameraMode - Setting Camera Mode to the camera
 * camMode - Type of Camera Mode from UI
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setCameraMode(CAMERA_MODE camMode)
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
    g_out_packet_buf[3] = SET_CAMERA_MODE;
    g_out_packet_buf[4] = camMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_CAMERA_MODE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getStrobeMode - Getting strobe mode from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getStrobeMode()
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
    g_out_packet_buf[3] = GET_STROBE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_STROBE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setStrobeMode - Setting Strobe Mode to the camera
 * @param strobeMode - Strobe mode to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setStrobeMode(STROBE_MODE strobeMode)
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
    g_out_packet_buf[3] = SET_STROBE;
    g_out_packet_buf[4] = strobeMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_STROBE &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getOrientation - getting flip mode from the camera
 * return true - success /false - failure
*/
bool SEE3CAM_CU200::getOrientation()
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
 * @brief SEE3CAM_CU200::setOrientation - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - horizontal flip selection
 * @param - vertical flip selection
 * return true/false
*/
bool SEE3CAM_CU200::setOrientation(bool horzModeSel, bool vertiModeSel)
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
 * @brief SEE3CAM_CU200::getWhiteBalanceMode - Getting white balance mode as well as the featues of Auto & Manual white balance from the camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::getWhiteBalanceMode()
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
    g_out_packet_buf[3] = GET_WHITE_BALANCE;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_WHITE_BALANCE &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            whiteBalancePropertiesReceived(g_in_packet_buf[3], g_in_packet_buf[4], g_in_packet_buf[5]);

            //Initialize buffers
            initializeBuffers();

            g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
            g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
            g_out_packet_buf[3] = GET_WHITE_BALANCE_PRESET;

            // send request and get reply from camera
            if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
                if (g_in_packet_buf[6] == GET_FAIL) {
                    return false;
                } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
                    g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
                    g_in_packet_buf[2] == GET_WHITE_BALANCE_PRESET &&
                    g_in_packet_buf[6] == GET_SUCCESS) {
                    emit whiteBalancePresetModeReceived(g_in_packet_buf[3]);

                    return true;
                }
            }

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setWhiteBalanceMode - Setting white balance mode as well as the featues of Auto & Manual white balance to the camera
 * @param mode - Auto or Manual
 * @param autoFeatures - Continious or Single Shot
 * @param manualFeature - Preset, Color Temperature or Pro mode
 * @param presetMode - Preset mode to be set
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setWhiteBalanceMode(WHITE_BALANCE_MODE mode, AUTO_WB_FEATURES autoFeature, MANUAL_WB_MODES manualMode, WB_PRESET presetMode)
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
    g_out_packet_buf[3] = SET_WHITE_BALANCE;
    g_out_packet_buf[4] = mode;
    g_out_packet_buf[5] = autoFeature;
    g_out_packet_buf[6] = manualMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_WHITE_BALANCE &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            if(g_in_packet_buf[3] == MANUAL_WB)
            {
                if(g_in_packet_buf[5] == PRESET) {
                    //Initializing buffers
                    initializeBuffers();

                    //Filling the buffer values
                    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
                    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
                    g_out_packet_buf[3] = SET_WHITE_BALANCE_PRESET;
                    g_out_packet_buf[4] = presetMode;

                    // send request and get reply from camera
                    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
                        if (g_in_packet_buf[6] == GET_FAIL) {
                            return false;
                        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
                            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
                            g_in_packet_buf[2] == SET_WHITE_BALANCE_PRESET &&
                            g_in_packet_buf[6] == GET_SUCCESS) {
                            return true;
                        }
                    }
                }
            }
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setAntiFlickerMode - setting Anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - 50hz/60hz/Disable]
 * @return true/false
 */
bool SEE3CAM_CU200::setAntiFlickerMode(ANTI_FLICKER_DETECTION antiFlickerMode){
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
    g_out_packet_buf[3] = SET_ANTI_FLICKER_DETECTION;
    g_out_packet_buf[4] = antiFlickerMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_ANTI_FLICKER_DETECTION &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU200::getAntiFlickerMode - get current anti flicker mode from camera
 * @return - true/false
 */
bool SEE3CAM_CU200::getAntiFlickerMode()
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
    g_out_packet_buf[3] = GET_ANTI_FLICKER_DETECTION;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == GET_ANTI_FLICKER_DETECTION &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
    return false;
}




/**
 * @brief SEE3CAM_CU200::setPropertiesForCrossStill - To set still properties while taking still in cross resolution
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setPropertiesForCrossStill(bool isEnable){

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
    g_out_packet_buf[3] = SET_CROSS_STILL_PROPERTIES;
    g_out_packet_buf[4] = isEnable;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
                  g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_CROSS_STILL_PROPERTIES &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_CU200::readStatistics - To retrieve the current exposure, gain, and color correction matrix values during its auto mode from the camera.
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::readStatistics(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint currentExposure;

    float currentRGain, currentBGain;

    float currentRr, currentRg, currentRb;
    float currentGr, currentGg, currentGb;
    float currentBr, currentBg, currentBb;

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2;
    g_out_packet_buf[3] = READ_STATISTICS;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[52] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
                  g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2]  == READ_STATISTICS &&
            g_in_packet_buf[52] == SET_SUCCESS) {
            currentExposure  = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            fCurrentRGain.u   = (g_in_packet_buf[8] << 24) | (g_in_packet_buf[9] << 16) | (g_in_packet_buf[10] << 8) | (g_in_packet_buf[11] << 0);
            fCurrentBGain.u   = (g_in_packet_buf[12] << 24) | (g_in_packet_buf[13] << 16) | (g_in_packet_buf[14] << 8) | (g_in_packet_buf[15] << 0);

            currentRGain = fCurrentRGain.f;
            currentBGain = fCurrentBGain.f;

            fCurrentRr.u = (g_in_packet_buf[16] << 24) | (g_in_packet_buf[17] << 16) | (g_in_packet_buf[18] << 8) | (g_in_packet_buf[19] << 0);
            fCurrentRg.u = (g_in_packet_buf[20] << 24) | (g_in_packet_buf[21] << 16) | (g_in_packet_buf[22] << 8) | (g_in_packet_buf[23] << 0);
            fCurrentRb.u = (g_in_packet_buf[24] << 24) | (g_in_packet_buf[25] << 16) | (g_in_packet_buf[26] << 8) | (g_in_packet_buf[27] << 0);

            fCurrentGr.u = (g_in_packet_buf[28] << 24) | (g_in_packet_buf[29] << 16) | (g_in_packet_buf[30] << 8) | (g_in_packet_buf[31] << 0);
            fCurrentGg.u = (g_in_packet_buf[32] << 24) | (g_in_packet_buf[33] << 16) | (g_in_packet_buf[34] << 8) | (g_in_packet_buf[35] << 0);
            fCurrentGb.u = (g_in_packet_buf[36] << 24) | (g_in_packet_buf[37] << 16) | (g_in_packet_buf[38] << 8) | (g_in_packet_buf[39] << 0);

            fCurrentBr.u  = (g_in_packet_buf[40] << 24) | (g_in_packet_buf[41] << 16) | (g_in_packet_buf[42] << 8) | (g_in_packet_buf[43] << 0);
            fCurrentBg.u  = (g_in_packet_buf[44] << 24) | (g_in_packet_buf[45] << 16) | (g_in_packet_buf[46] << 8) | (g_in_packet_buf[47] << 0);
            fCurrentBb.u  = (g_in_packet_buf[48] << 24) | (g_in_packet_buf[49] << 16) | (g_in_packet_buf[50] << 8) | (g_in_packet_buf[51] << 0);

            currentRr = fCurrentRr.f;
            currentRg = fCurrentRg.f;
            currentRb = fCurrentRb.f;

            currentGr = fCurrentGr.f;
            currentGg = fCurrentGg.f;
            currentGb = fCurrentGb.f;

            currentBr = fCurrentBr.f;
            currentBg = fCurrentBg.f;
            currentBb = fCurrentBb.f;

            emit currentAutoExposure(currentExposure);

            emit currentGainProperties(g_in_packet_buf[7], currentRGain, currentBGain);

            emit currentRMatrixElements(currentRr, currentRg, currentRb);
            emit currentGMatrixElements(currentGr, currentGg, currentGb);
            emit currentBMatrixElements(currentBr, currentBg, currentBb);

            return true;
        }
    }
    return false;
}




/**
 * @brief SEE3CAM_CU200::saveConfiguration
 * @param configMode - To set default / User defined configuration
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::saveConfiguration(SAVECONFIGURATION configMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION; /* camera id */
    g_out_packet_buf[2] = configMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION  &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations Saved Successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief SEE3CAM_CU200::resetDevice - To Reset the device
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::resetDevice()
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
 * @brief SEE3CAM_CU200::readISPFirmwareVersion - To read the firmware version of ISP
 * @return true/false
 */
bool SEE3CAM_CU200::readISPFirmwareVersion()
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1; 	/* control_id_1 */
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2; 	/* control_id_2 */
    g_out_packet_buf[3] = READ_ISP_FIRMWARE_VERSION; /* Id to get ISP version */

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
            if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
               g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
               g_in_packet_buf[2] == READ_ISP_FIRMWARE_VERSION) {

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
 * @brief SEE3CAM_CU200::setToDefault - set all the values to default in camera
 * @return True if successful, false otherwise.
 */
bool SEE3CAM_CU200::setToDefault(){
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
    g_out_packet_buf[3] = SET_TO_DEFAULT; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2 &&
            g_in_packet_buf[2] == SET_TO_DEFAULT &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU200::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_CU200::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
