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
 * @brief SEE3CAM_CU200::getGainMode - Getting gain mode to the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_GAIN_MODE_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_GAIN_MODE_SEE3CAM_CU200 &&
            g_in_packet_buf[8] == GET_SUCCESS) {

            emit minGainReceived(g_in_packet_buf[5]);
            emit maxGainReceived(g_in_packet_buf[6]);
            emit currentGainReceived(g_in_packet_buf[4]);
            emit gainStepValueReceived(g_in_packet_buf[7]);

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::setGainMode - Setting gain mode to the camera
 * @param gainMode - mode selected in UI
 * @return true/false
 */
bool SEE3CAM_CU200::setGainMode(GAIN_MODE gainMode, uint gainValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_GAIN_MODE_SEE3CAM_CU200 ; /* set gain mode code */
    g_out_packet_buf[4] = gainMode; /* actual gain mode */
    g_out_packet_buf[5] = gainValue; /* Gain Slider Value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_GAIN_MODE_SEE3CAM_CU200  &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}



/**
 * @brief SEE3CAM_CU200::getRGBGain - Getting RGB Gain Value from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_RB_GAIN_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[35] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2]  == GET_RB_GAIN_SEE3CAM_CU200 &&
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

            emit minRGainReceived(minRGain);
            emit maxRGainReceived(maxRGain);
            emit currentRGainReceived(currentRGain);
            emit rGainStepValueReceived(RGainStepValue);

            emit minBGainReceived(minBGain);
            emit maxBGainReceived(maxBGain);
            emit currentBGainReceived(currentBGain);
            emit bGainStepValueReceived(BGainStepValue);
          }

            return true;
        }
}


/**
 * @brief SEE3CAM_CU200::setRGBGain - Setting RGBGain Value to the camera
 * RGBGain - RGBGain value from the UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_RB_GAIN_SEE3CAM_CU200;

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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_RB_GAIN_SEE3CAM_CU200 &&
            g_in_packet_buf[11] == GET_SUCCESS) {
            return true;
        }
    }
}




/**
 * @brief SEE3CAM_CU200::getColorCorrectionMatrix - Getting Color correction matrix elements from the camera
 * @return true/false
 */
bool SEE3CAM_CU200::getColorCorrectionMatrix()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float currentRr, minRr, maxRr, stepRr;
    float currentRg, minRg, maxRg, stepRg;
    float currentRb, minRb, maxRb, stepRb;

    float currentGr, minGr, maxGr, stepGr;
    float currentGg, minGg, maxGg, stepGg;
    float currentGb, minGb, maxGb, stepGb;

    float currentBr, minBr, maxBr, stepBr;
    float currentBg, minBg, maxBg, stepBg;
    float currentBb, minBb, maxBb, stepBb;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_CC_MATRIX_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[147] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2]  == GET_CC_MATRIX_SEE3CAM_CU200 &&
            g_in_packet_buf[147] == GET_SUCCESS) {

            fCurrentRr.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fMinRr.u     = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxRr.u     = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fStepRr.u    = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            currentRr = fCurrentRr.f;
            minRr     = fMinRr.f;
            maxRr     = fMaxRr.f;
            stepRr    = fStepRr.f;


            fCurrentRg.u = (g_in_packet_buf[19] << 24) | (g_in_packet_buf[20] << 16) | (g_in_packet_buf[21] << 8) | (g_in_packet_buf[22] << 0);
            fMinRg.u     = (g_in_packet_buf[23] << 24) | (g_in_packet_buf[24] << 16) | (g_in_packet_buf[25] << 8) | (g_in_packet_buf[26] << 0);
            fMaxRg.u     = (g_in_packet_buf[27] << 24) | (g_in_packet_buf[28] << 16) | (g_in_packet_buf[29] << 8) | (g_in_packet_buf[30] << 0);
            fStepRg.u    = (g_in_packet_buf[31] << 24) | (g_in_packet_buf[32] << 16) | (g_in_packet_buf[33] << 8) | (g_in_packet_buf[34] << 0);

            currentRg = fCurrentRg.f;
            minRg     = fMinRg.f;
            maxRg     = fMaxRg.f;
            stepRg    = fStepRg.f;


            fCurrentRb.u = (g_in_packet_buf[35] << 24) | (g_in_packet_buf[36] << 16) | (g_in_packet_buf[37] << 8) | (g_in_packet_buf[38] << 0);
            fMinRb.u     = (g_in_packet_buf[39] << 24) | (g_in_packet_buf[40] << 16) | (g_in_packet_buf[41] << 8) | (g_in_packet_buf[42] << 0);
            fMaxRb.u     = (g_in_packet_buf[43] << 24) | (g_in_packet_buf[44] << 16) | (g_in_packet_buf[45] << 8) | (g_in_packet_buf[46] << 0);
            fStepRb.u    = (g_in_packet_buf[47] << 24) | (g_in_packet_buf[48] << 16) | (g_in_packet_buf[49] << 8) | (g_in_packet_buf[50] << 0);

            currentRb = fCurrentRb.f;
            minRb     = fMinRb.f;
            maxRb     = fMaxRb.f;
            stepRb    = fStepRb.f;

            //-------------------------------------------------------


            fCurrentGr.u = (g_in_packet_buf[51] << 24) | (g_in_packet_buf[52] << 16) | (g_in_packet_buf[53] << 8) | (g_in_packet_buf[54] << 0);
            fMinGr.u     = (g_in_packet_buf[55] << 24) | (g_in_packet_buf[56] << 16) | (g_in_packet_buf[57] << 8) | (g_in_packet_buf[58] << 0);
            fMaxGr.u     = (g_in_packet_buf[59] << 24) | (g_in_packet_buf[60] << 16) | (g_in_packet_buf[61] << 8) | (g_in_packet_buf[62] << 0);
            fStepGr.u    = (g_in_packet_buf[63] << 24) | (g_in_packet_buf[64] << 16) | (g_in_packet_buf[65] << 8) | (g_in_packet_buf[66] << 0);

            currentGr = fCurrentGr.f;
            minGr     = fMinGr.f;
            maxGr     = fMaxGr.f;
            stepGr    = fStepGr.f;


            fCurrentGg.u = (g_in_packet_buf[67] << 24) | (g_in_packet_buf[68] << 16) | (g_in_packet_buf[69] << 8) | (g_in_packet_buf[70] << 0);
            fMinGg.u     = (g_in_packet_buf[71] << 24) | (g_in_packet_buf[72] << 16) | (g_in_packet_buf[73] << 8) | (g_in_packet_buf[74] << 0);
            fMaxGg.u     = (g_in_packet_buf[75] << 24) | (g_in_packet_buf[76] << 16) | (g_in_packet_buf[77] << 8) | (g_in_packet_buf[78] << 0);
            fStepGg.u    = (g_in_packet_buf[79] << 24) | (g_in_packet_buf[80] << 16) | (g_in_packet_buf[81] << 8) | (g_in_packet_buf[82] << 0);

            currentGg = fCurrentGg.f;
            minGg     = fMinGg.f;
            maxGg     = fMaxGg.f;
            stepGg    = fStepGg.f;


            fCurrentGb.u = (g_in_packet_buf[83] << 24) | (g_in_packet_buf[84] << 16) | (g_in_packet_buf[85] << 8) | (g_in_packet_buf[86] << 0);
            fMinGb.u     = (g_in_packet_buf[87] << 24) | (g_in_packet_buf[88] << 16) | (g_in_packet_buf[89] << 8) | (g_in_packet_buf[90] << 0);
            fMaxGb.u     = (g_in_packet_buf[91] << 24) | (g_in_packet_buf[92] << 16) | (g_in_packet_buf[93] << 8) | (g_in_packet_buf[94] << 0);
            fStepGb.u    = (g_in_packet_buf[95] << 24) | (g_in_packet_buf[96] << 16) | (g_in_packet_buf[97] << 8) | (g_in_packet_buf[98] << 0);

            currentGb = fCurrentGb.f;
            minGb     = fMinGb.f;
            maxGb     = fMaxGb.f;
            stepGb    = fStepGb.f;


            //-------------------------------------------


            fCurrentBr.u = (g_in_packet_buf[99] << 24) | (g_in_packet_buf[100] << 16) | (g_in_packet_buf[101] << 8) | (g_in_packet_buf[102] << 0);
            fMinBr.u     = (g_in_packet_buf[103] << 24) | (g_in_packet_buf[104] << 16) | (g_in_packet_buf[105] << 8) | (g_in_packet_buf[106] << 0);
            fMaxBr.u     = (g_in_packet_buf[107] << 24) | (g_in_packet_buf[108] << 16) | (g_in_packet_buf[109] << 8) | (g_in_packet_buf[110] << 0);
            fStepBr.u     = (g_in_packet_buf[111] << 24) | (g_in_packet_buf[112] << 16) | (g_in_packet_buf[113] << 8) | (g_in_packet_buf[114] << 0);

            currentBr = fCurrentBr.f;
            minBr     = fMinBr.f;
            maxBr     = fMaxBr.f;
            stepBr    = fStepBr.f;


            fCurrentBg.u = (g_in_packet_buf[115] << 24) | (g_in_packet_buf[116] << 16) | (g_in_packet_buf[117] << 8) | (g_in_packet_buf[118] << 0);
            fMinBg.u     = (g_in_packet_buf[119] << 24) | (g_in_packet_buf[120] << 16) | (g_in_packet_buf[121] << 8) | (g_in_packet_buf[122] << 0);
            fMaxBg.u     = (g_in_packet_buf[123] << 24) | (g_in_packet_buf[124] << 16) | (g_in_packet_buf[125] << 8) | (g_in_packet_buf[126] << 0);
            fStepBg.u     = (g_in_packet_buf[127] << 24) | (g_in_packet_buf[128] << 16) | (g_in_packet_buf[129] << 8) | (g_in_packet_buf[130] << 0);

            currentBg = fCurrentBg.f;
            minBg     = fMinBg.f;
            maxBg     = fMaxBg.f;
            stepBg    = fStepBg.f;


            fCurrentBb.u = (g_in_packet_buf[131] << 24) | (g_in_packet_buf[132] << 16) | (g_in_packet_buf[133] << 8) | (g_in_packet_buf[134] << 0);
            fMinBb.u     = (g_in_packet_buf[134] << 24) | (g_in_packet_buf[136] << 16) | (g_in_packet_buf[137] << 8) | (g_in_packet_buf[138] << 0);
            fMaxBb.u     = (g_in_packet_buf[139] << 24) | (g_in_packet_buf[140] << 16) | (g_in_packet_buf[141] << 8) | (g_in_packet_buf[142] << 0);
            fStepBb.u     = (g_in_packet_buf[143] << 24) | (g_in_packet_buf[144] << 16) | (g_in_packet_buf[145] << 8) | (g_in_packet_buf[146] << 0);

            currentBb = fCurrentBb.f;
            minBb     = fMinBb.f;
            maxBb     = fMaxBb.f;
            stepBb    = fStepBb.f;

            emit currentRrValuesReceived(currentRr, minRr, maxRr, stepRr);
            emit currentRgValuesReceived(currentRg, minRg, maxRg, stepRg);
            emit currentRbValuesReceived(currentRb, minRb, maxRb, stepRb);

            emit currentGrValuesReceived(currentGr, minGr, maxGr, stepGr);
            emit currentGgValuesReceived(currentGg, minGg, maxGg, stepGg);
            emit currentGbValuesReceived(currentGb, minGb, maxGb, stepGb);

            emit currentBrValuesReceived(currentBr, minBr, maxBr, stepBr);
            emit currentBgValuesReceived(currentBg, minBg, maxBg, stepBg);
            emit currentBbValuesReceived(currentBb, minBb, maxBb, stepBb);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setColorCorrectionMatrix - Setting the values for Color correction elements to the camera
 * Rr, Rg, Rb, Gr, Gg, Gb, Br, Bg, Bb - Color Correction Matrix Elements
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_CC_MATRIX_SEE3CAM_CU200;


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
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2]  == SET_CC_MATRIX_SEE3CAM_CU200 &&
            g_in_packet_buf[39] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getBlackLevel - Getting Black Level Value from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_BLACK_LEVEL_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_BLACK_LEVEL_SEE3CAM_CU200 &&
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
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_BLACK_LEVEL_SEE3CAM_CU200;
    g_out_packet_buf[4] = ((blackLevel & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((blackLevel & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_BLACK_LEVEL_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getBrightness - Getting Brightness Value from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_BRIGHTNESS_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[19] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_BRIGHTNESS_SEE3CAM_CU200 &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            fMinBrightness.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxBrightness.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fCurrentBrightness.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fBrightnessStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            current   = fCurrentBrightness.f;
            min       = fMinBrightness.f;
            max       = fMaxBrightness.f;
            stepValue = fBrightnessStepValue.f;

            emit minBrightnessReceived(min);
            emit maxBrightnessReceived(max);
            emit currentBrightnessReceived(current);
            emit brightnessStepValueReceived(stepValue);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setBrightness - Setting Brightness Value to the camera
 * brightness - Value from UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_BRIGHTNESS_SEE3CAM_CU200;
    g_out_packet_buf[4] = ((fCurrentBrightness.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentBrightness.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentBrightness.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentBrightness.u & 0x000000FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_BRIGHTNESS_SEE3CAM_CU200 &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getContrast - Getting Contrast Value from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_CONTRAST_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_CONTRAST_SEE3CAM_CU200 &&
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
 * contrast - Contrast value from UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_CONTRAST_SEE3CAM_CU200;
    g_out_packet_buf[4] = contrast;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_CONTRAST_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getSaturation - Getting Saturation Value from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_SATURATION_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[19] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_SATURATION_SEE3CAM_CU200 &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            fCurrentSaturation.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fMinSaturation.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxSaturation.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fSaturationStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            current   = fCurrentSaturation.f;
            min       = fMinSaturation.f;
            max       = fMaxSaturation.f;
            stepValue = fSaturationStepValue.f;

            emit minSaturationReceived(min);
            emit maxSaturationReceived(max);
            emit currentSaturationReceived(current);
            emit saturationStepValueReceived(stepValue);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setSaturation - Setting saturation Value to the camera
 * saturation - Saturation value from UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_SATURATION_SEE3CAM_CU200;
    g_out_packet_buf[4] = ((fCurrentSaturation.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentSaturation.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentSaturation.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentSaturation.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_SATURATION_SEE3CAM_CU200 &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getColorTemperature - Getting Color Temperature Value from the camera
 * @return true/false
 */
bool SEE3CAM_CU200::getColorTemperature()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint colorTemp;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_COLOR_TEMPERATURE_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_COLOR_TEMPERATURE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            colorTemp = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);

            emit colorTemperatureReceived(colorTemp);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setColorTemperature - Setting Black Level Value to the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_COLOR_TEMPERATURE_SEE3CAM_CU200;
    g_out_packet_buf[4] = ((colorTemp & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((colorTemp & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_COLOR_TEMPERATURE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getGammaCorrection - Getting Gamma Value from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_GAMMA_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[19] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2]  == GET_GAMMA_SEE3CAM_CU200 &&
            g_in_packet_buf[19] == GET_SUCCESS) {

            fCurrentGamma.u   = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            fMinGamma.u       = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            fMaxGamma.u       = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            fGammaStepValue.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);

            current   = fCurrentGamma.f;
            min       = fMinGamma.f;
            max       = fMaxGamma.f;
            stepValue = fGammaStepValue.f;

            emit minGammaCorrectionReceived(min);
            emit maxGammaCorrectionReceived(max);
            emit currentGammaCorrectionReceived(current);
            emit gammaCorrectionStepValueReceived(stepValue);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setGammaCorrection - Setting Gamma Value to the camera
 * gammaCorrection - Gsmma value from UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_GAMMA_SEE3CAM_CU200;
    g_out_packet_buf[4] = ((fCurrentGamma.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fCurrentGamma.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fCurrentGamma.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fCurrentGamma.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_GAMMA_SEE3CAM_CU200 &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            return true;
        }
    }
}




/**
 * @brief SEE3CAM_CU200::getExposure - Getting exposure value from the camera
 * @return true/false
 */
bool SEE3CAM_CU200::getExposure()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

//    uint exposureMode;
    uint manualExposure;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_EXPOSURE_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[8] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_EXPOSURE_SEE3CAM_CU200 &&
            g_in_packet_buf[8] == GET_SUCCESS) {

//            exposureMode = g_in_packet_buf[3]; //As of now Auto exposure is not supported
            manualExposure  = (g_in_packet_buf[4] << 24) | (g_in_packet_buf[5] << 16) | (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);

            emit exposureValueReceived(manualExposure);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setExposure - Setting Exposure to the camera
 * exposure - exposure value from UI
 * @return true/false
 */
bool SEE3CAM_CU200::setExposure(EXPOSURE_MODE expMode, uint32_t manualExposure)
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

    uint manualExposureVal;

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_EXPOSURE_SEE3CAM_CU200;
    g_out_packet_buf[4] = expMode;
    g_out_packet_buf[5] = ((manualExposure & 0xFF000000) >> 24);
    g_out_packet_buf[6] = ((manualExposure & 0x00FF0000) >> 16);
    g_out_packet_buf[7] = ((manualExposure & 0x0000FF00) >> 8);
    g_out_packet_buf[8] = ((manualExposure & 0x000000FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8] == GET_FAIL) {
            emit indicateExposureValueRangeFailure("Failure", "Exposure Set Failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_EXPOSURE_SEE3CAM_CU200 &&
            g_in_packet_buf[8] == GET_SUCCESS) {

            emit indicateCommandStatus("Success", "Exposure set successfully");

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_CU200::getCameraMode - Getting Camera mode from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_CAMERA_MODE_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_CAMERA_MODE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit cameraModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setCameraMode - Setting Camera Mode to the camera
 * camMode - Type of Camera Mode from UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_CAMERA_MODE_SEE3CAM_CU200;
    g_out_packet_buf[4] = camMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_CAMERA_MODE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::getStrobeMode - Getting strobe mode from the camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = GET_STROBE_MODE_SEE3CAM_CU200;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == GET_STROBE_MODE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit strobeModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setStrobeMode - Setting Strobe Mode to the camera
 * strobeMode - Type of strobe Mode from UI
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_STROBE_MODE_SEE3CAM_CU200;
    g_out_packet_buf[4] = strobeMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_STROBE_MODE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_CU200::setToDefault - set all the values to default in camera
 * @return true/false
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_CU200;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_CU200;
    g_out_packet_buf[3] = SET_TO_DEFAULT_SEE3CAM_CU200; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_CU200 &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_CU200 &&
            g_in_packet_buf[2] == SET_TO_DEFAULT_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_CU200::saveConfiguration
 * @return true/false
 */
bool SEE3CAM_CU200::saveConfiguration()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = SAVE_CONFIGURATION_SEE3CAM_CU200; /* camera id */
    g_out_packet_buf[2] = SAVE_SEE3CAM_CU200; /* set to default command */


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH))
    {
        if (g_in_packet_buf[6] == SET_FAIL)
        {
            emit indicateCommandStatus("Failure", "Saving Configurations Failed");
            return false;
        }
        else if(g_in_packet_buf[0] == SAVE_CONFIGURATION_SEE3CAM_CU200  &&
            g_in_packet_buf[1] == SAVE_SEE3CAM_CU200 &&
            g_in_packet_buf[6] == SET_SUCCESS){
            emit indicateCommandStatus("Success", "Configurations saved successfully");
            return true;
        }
    }
    return false;
}


/*
 * @brief SEE3CAM_CU200::initializeBuffers - Initialize input and output buffers
 */
void SEE3CAM_CU200::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}



