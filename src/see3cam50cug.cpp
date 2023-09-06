/*
 * see3cam50cug.cpp -- Handling special feature of SEE3CAM_50CUG camera
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
#include "see3cam50cug.h"
#include <cmath>
#include <iostream>
#include <iomanip> // Include the header for std::setprecision

SEE3CAM_50CUG::SEE3CAM_50CUG()
{

}


SEE3CAM_50CUG::~SEE3CAM_50CUG()
{

}

/**
 * @brief SEE3CAM_50CUG::getGainMode - Getting gain mode to the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getGainMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_GAIN_MODE_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_GAIN_MODE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit gainValueReceived(g_in_packet_buf[4]);
            return true;
        }
    }
}



/**
 * @brief SEE3CAM_50CUG::setGainMode - Setting gain mode to the camera
 * @param gainMode - mode selected in UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setGainMode(GAIN_MODE gainMode, uint gainValue)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_GAIN_MODE_SEE3CAM_50CUG ; /* set gain mode code */
    g_out_packet_buf[4] = gainMode; /* actual gain mode */
    g_out_packet_buf[5] = gainValue; /* Gain Slider Value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_GAIN_MODE_SEE3CAM_50CUG  &&
            g_in_packet_buf[6] == SET_SUCCESS) {

            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUG::getRGBGain - Getting RGB Gain Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getRBGain()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float RGain;
    float BGain;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_RGB_GAIN_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_RGB_GAIN_SEE3CAM_50CUG &&
            g_in_packet_buf[11] == GET_SUCCESS) {

            frgain.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            RGain = frgain.f;

            fbgain.u = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            BGain = fbgain.f;

            emit gainRReceived(RGain);
            emit gainBReceived(BGain);
          }
            return true;
        }
}


/**
 * @brief SEE3CAM_50CUG::setRGBGain - Setting RGBGain Value to the camera
 * RGBGain - RGBGain value from the UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setRBGain(float RGain, float BGain)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    frgain.f = RGain;
    fbgain.f = BGain;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_RGB_GAIN_SEE3CAM_50CUG;

    g_out_packet_buf[4] = ((frgain.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((frgain.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((frgain.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((frgain.u & 0x000000FF) >> 0);

    g_out_packet_buf[8]  = ((fbgain.u & 0xFF000000) >> 24);
    g_out_packet_buf[9]  = ((fbgain.u  & 0x00FF0000) >> 16);
    g_out_packet_buf[10] = ((fbgain.u  & 0x0000FF00) >> 8);
    g_out_packet_buf[11] = ((fbgain.u  & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[11] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_RGB_GAIN_SEE3CAM_50CUG &&
            g_in_packet_buf[11] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::getColorCorrectionMatrix - Getting Color correction matrix elements from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getColorCorrectionMatrix()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float Rr, Rg, Rb;
    float Gr, Gg, Gb;
    float Br, Bg, Bb;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_CC_MATRIX_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[39] == GET_FAIL) {
            return false;
        }else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_CC_MATRIX_SEE3CAM_50CUG &&
            g_in_packet_buf[39] == GET_SUCCESS) {

            fRr.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            Rr = fRr.f;

            fRg.u = (g_in_packet_buf[7] << 24) | (g_in_packet_buf[8] << 16) | (g_in_packet_buf[9] << 8) | (g_in_packet_buf[10] << 0);
            Rg = fRg.f;

            fRb.u = (g_in_packet_buf[11] << 24) | (g_in_packet_buf[12] << 16) | (g_in_packet_buf[13] << 8) | (g_in_packet_buf[14] << 0);
            Rb = fRb.f;


            fGr.u = (g_in_packet_buf[15] << 24) | (g_in_packet_buf[16] << 16) | (g_in_packet_buf[17] << 8) | (g_in_packet_buf[18] << 0);
            Gr = fGr.f;

            fGg.u = (g_in_packet_buf[19] << 24) | (g_in_packet_buf[20] << 16) | (g_in_packet_buf[21] << 8) | (g_in_packet_buf[22] << 0);
            Gg = fGg.f;

            fGb.u = (g_in_packet_buf[23] << 24) | (g_in_packet_buf[24] << 16) | (g_in_packet_buf[25] << 8) | (g_in_packet_buf[26] << 0);
            Gb = fGb.f;


            fBr.u = (g_in_packet_buf[27] << 24) | (g_in_packet_buf[28] << 16) | (g_in_packet_buf[29] << 8) | (g_in_packet_buf[30] << 0);
            Br = fBr.f;

            fBg.u = (g_in_packet_buf[31] << 24) | (g_in_packet_buf[32] << 16) | (g_in_packet_buf[33] << 8) | (g_in_packet_buf[34] << 0);
            Bg = fBg.f;

            fBb.u = (g_in_packet_buf[35] << 24) | (g_in_packet_buf[36] << 16) | (g_in_packet_buf[37] << 8) | (g_in_packet_buf[38] << 0);
            Bb = fBb.f;

            emit valueRReceived(Rr, Rg, Rb);
            emit valueGReceived(Gr, Gg, Gb);
            emit valueBReceived(Br, Bg, Bb);

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setColorCorrectionMatrix - Setting the values for Color correction elements to the camera
 * Rr, Rg, Rb, Gr, Gg, Gb, Br, Bg, Bb - Color Correction Matrix Elements
 * @return true/false
 */
bool SEE3CAM_50CUG::setColorCorrectionMatrix(float Rr, float Rg, float Rb, float Gr, float Gg, float Gb, float Br, float Bg, float Bb)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fRr.f = Rr;
    fRg.f = Rg;
    fRb.f = Rb;

    fGr.f = Gr;
    fGg.f = Gg;
    fGb.f = Gb;

    fBr.f = Br;
    fBg.f = Bg;
    fBb.f = Bb;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_CC_MATRIX_SEE3CAM_50CUG;


    g_out_packet_buf[4] = ((fRr.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fRr.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fRr.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fRr.u & 0x000000FF) >> 0);

    g_out_packet_buf[8]  = ((fRg.u & 0xFF000000) >> 24);
    g_out_packet_buf[9]  = ((fRg.u & 0x00FF0000) >> 16);
    g_out_packet_buf[10] = ((fRg.u & 0x0000FF00) >> 8);
    g_out_packet_buf[11] = ((fRg.u & 0x000000FF) >> 0);

    g_out_packet_buf[12] = ((fRb.u & 0xFF000000) >> 24);
    g_out_packet_buf[13] = ((fRb.u & 0x00FF0000) >> 16);
    g_out_packet_buf[14] = ((fRb.u & 0x0000FF00) >> 8);
    g_out_packet_buf[15] = ((fRb.u & 0x000000FF) >> 0);



    g_out_packet_buf[16] = ((fGr.u & 0xFF000000) >> 24);
    g_out_packet_buf[17] = ((fGr.u & 0x00FF0000) >> 16);
    g_out_packet_buf[18] = ((fGr.u & 0x0000FF00) >> 8);
    g_out_packet_buf[19] = ((fGr.u & 0x000000FF) >> 0);

    g_out_packet_buf[20] = ((fGg.u & 0xFF000000) >> 24);
    g_out_packet_buf[21] = ((fGg.u & 0x00FF0000) >> 16);
    g_out_packet_buf[22] = ((fGg.u & 0x0000FF00) >> 8);
    g_out_packet_buf[23] = ((fGg.u & 0x000000FF) >> 0);

    g_out_packet_buf[24] = ((fGb.u & 0xFF000000) >> 24);
    g_out_packet_buf[25] = ((fGb.u & 0x00FF0000) >> 16);
    g_out_packet_buf[26] = ((fGb.u & 0x0000FF00) >> 8);
    g_out_packet_buf[27] = ((fGb.u & 0x000000FF) >> 0);



    g_out_packet_buf[28] = ((fBr.u & 0xFF000000) >> 24);
    g_out_packet_buf[29] = ((fBr.u & 0x00FF0000) >> 16);
    g_out_packet_buf[30] = ((fBr.u & 0x0000FF00) >> 8);
    g_out_packet_buf[31] = ((fBr.u & 0x000000FF) >> 0);

    g_out_packet_buf[32] = ((fBg.u & 0xFF000000) >> 24);
    g_out_packet_buf[33] = ((fBg.u & 0x00FF0000) >> 16);
    g_out_packet_buf[34] = ((fBg.u & 0x0000FF00) >> 8);
    g_out_packet_buf[35] = ((fBg.u & 0x000000FF) >> 0);

    g_out_packet_buf[36] = ((fBb.u & 0xFF000000) >> 24);
    g_out_packet_buf[37] = ((fBb.u & 0x00FF0000) >> 16);
    g_out_packet_buf[38] = ((fBb.u & 0x0000FF00) >> 8);
    g_out_packet_buf[39] = ((fBb.u & 0x000000FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[39] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1]  == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2]  == SET_CC_MATRIX_SEE3CAM_50CUG &&
            g_in_packet_buf[39] == GET_SUCCESS) {

            return true;
        }
    }
}




/**
 * @brief SEE3CAM_50CUG::getBlackLevel - Getting Black Level Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getBlackLevel()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint blackLevel;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_BLACK_LEVEL_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_BLACK_LEVEL_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            blackLevel = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);

            emit blackLevelReceived(blackLevel);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setBlackLevel - Setting Black Level Value to the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::setBlackLevel(uint blackLevel)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_BLACK_LEVEL_SEE3CAM_50CUG;
    g_out_packet_buf[4] = ((blackLevel & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((blackLevel & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_BLACK_LEVEL_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}

/**
 * @brief SEE3CAM_50CUG::getBrightness - Getting Brightness Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getBrightness()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float brightness;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_BRIGHTNESS_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_BRIGHTNESS_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            fBrightness.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            brightness = fBrightness.f;
            emit brightnessReceived(brightness);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setBrightness - Setting Brightness Value to the camera
 * brightness - Value from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setBrightness(float brightness)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fBrightness.f = brightness;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_BRIGHTNESS_SEE3CAM_50CUG;
    g_out_packet_buf[4] = ((fBrightness.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fBrightness.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fBrightness.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fBrightness.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_BRIGHTNESS_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}

/**
 * @brief SEE3CAM_50CUG::getContrast - Getting Contrast Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getContrast()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_CONTRAST_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_CONTRAST_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit contrastReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setContrast - Setting contrast Value to the camera
 * contrast - Contrast value from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setContrast(uint contrast)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_CONTRAST_SEE3CAM_50CUG;
    g_out_packet_buf[4] = contrast;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_CONTRAST_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}



/**
 * @brief SEE3CAM_50CUG::getSaturation - Getting Saturation Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getSaturation()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float saturation;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_SATURATION_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_SATURATION_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            fSaturation.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            saturation = fSaturation.f;

            emit saturationReceived(saturation);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setSaturation - Setting saturation Value to the camera
 * saturation - Saturation value from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setSaturation(float saturation)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fSaturation.f = saturation;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_SATURATION_SEE3CAM_50CUG;
    g_out_packet_buf[4] = ((fSaturation.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fSaturation.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fSaturation.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fSaturation.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_SATURATION_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::getColorTemperature - Getting Color Temperature Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getColorTemperature()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint colorTemp;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_COLOR_TEMPERATURE_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_COLOR_TEMPERATURE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            colorTemp = (g_in_packet_buf[3] << 8) | (g_in_packet_buf[4] << 0);

            emit colorTemperatureReceived(colorTemp);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setColorTemperature - Setting Black Level Value to the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::setColorTemperature(uint colorTemp)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_COLOR_TEMPERATURE_SEE3CAM_50CUG;
    g_out_packet_buf[4] = ((colorTemp & 0xFF00) >> 8);
    g_out_packet_buf[5] = ((colorTemp & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_COLOR_TEMPERATURE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            return true;
        }
    }
}



/**
 * @brief SEE3CAM_50CUG::getGammaCorrection - Getting Gamma Value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getGammaCorrection()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    float gammaCorrection;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_GAMMA_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_GAMMA_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            fgamma.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            gammaCorrection = fgamma.f;

            emit gammaCorrectionReceived(gammaCorrection);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setGammaCorrection - Setting Gamma Value to the camera
 * gammaCorrection - Gsmma value from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setGammaCorrection(float gammaCorrection)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    fgamma.f = gammaCorrection;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_GAMMA_SEE3CAM_50CUG;
    g_out_packet_buf[4] = ((fgamma.u & 0xFF000000) >> 24);
    g_out_packet_buf[5] = ((fgamma.u & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((fgamma.u & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((fgamma.u & 0x000000FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_GAMMA_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            fgamma.u = (g_in_packet_buf[3] << 24) | (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);
            gammaCorrection = fgamma.f;

            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::getCameraMode - Getting Camera mode from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getCameraMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_CAMERA_MODE_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_CAMERA_MODE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit cameraModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setCameraMode - Setting Camera Mode to the camera
 * camMode - Type of Camera Mode from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setCameraMode(CAMERA_MODE camMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_CAMERA_MODE_SEE3CAM_50CUG;
    g_out_packet_buf[4] = camMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_CAMERA_MODE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::getStrobeMode - Getting strobe mode from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getStrobeMode()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_STROBE_MODE_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_STROBE_MODE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit strobeModeReceived(g_in_packet_buf[3]);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setStrobeMode - Setting Strobe Mode to the camera
 * strobeMode - Type of strobe Mode from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setStrobeMode(STROBE_MODE strobeMode)
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_STROBE_MODE_SEE3CAM_50CUG;
    g_out_packet_buf[4] = strobeMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_STROBE_MODE_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            return true;
        }
    }
}




/**
 * @brief SEE3CAM_50CUG::getExposure - Getting exposure value from the camera
 * @return true/false
 */
bool SEE3CAM_50CUG::getExposure()
{
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint exposure;

    //Initializing buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = GET_EXPOSURE_SEE3CAM_50CUG;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[7] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == GET_EXPOSURE_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            exposure = (g_in_packet_buf[4] << 16) | (g_in_packet_buf[5] << 8) | (g_in_packet_buf[6] << 0);

            emit exposureValueReceived(exposure);
            return true;
        }
    }
}


/**
 * @brief SEE3CAM_50CUG::setExposure - Setting Exposure to the camera
 * exposure - exposure value from UI
 * @return true/false
 */
bool SEE3CAM_50CUG::setExposure(EXPOSURE_MODE expMode, uint exposure)
{
    if(EXPOSURE_MIN > exposure || EXPOSURE_MAX < exposure){
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
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_EXPOSURE_SEE3CAM_50CUG;
    g_out_packet_buf[4] = expMode;
    g_out_packet_buf[5] = ((exposure & 0x00FF0000) >> 16);
    g_out_packet_buf[6] = ((exposure & 0x0000FF00) >> 8);
    g_out_packet_buf[7] = ((exposure & 0x000000FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[7] == GET_FAIL) {
            emit indicateExposureValueRangeFailure("Failure", "Exposure Set Failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_EXPOSURE_SEE3CAM_50CUG &&
            g_in_packet_buf[7] == GET_SUCCESS) {

            emit indicateCommandStatus("Success", "Exposure is set successfully");
            return true;
        }
    }
}



/**
 * @brief SEE3CAM_50CUG::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool SEE3CAM_50CUG::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Filling the buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_ID_1_SEE3CAM_50CUG;
    g_out_packet_buf[2] = CAMERA_CONTROL_ID_2_SEE3CAM_50CUG;
    g_out_packet_buf[3] = SET_TO_DEFAULT_SEE3CAM_50CUG; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_ID_1_SEE3CAM_50CUG &&
            g_in_packet_buf[1] == CAMERA_CONTROL_ID_2_SEE3CAM_50CUG &&
            g_in_packet_buf[2] == SET_TO_DEFAULT_SEE3CAM_50CUG &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief SEE3CAM_50CUG::saveConfiguration
 * @return true/false
 */
bool SEE3CAM_50CUG::saveConfiguration()
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
void SEE3CAM_50CUG::initializeBuffers(){
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}
