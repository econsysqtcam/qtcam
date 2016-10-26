/*
 * ascella.cpp -- Handling special features of ascella camera
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
#include "ascella.h"
#include <stdio.h>


void ASCELLA::setLEDStatusMode(camLedMode ledMode, QString brightnessVal){

    int bytesSent;
    u_int8_t brightnessIntVal;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setLEDStatusMode: Handle is Null");
        return void();
    }

    if(ledMode == LedAuto || ledMode == LedManual || ledMode == LedOff){
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

        g_out_packet_buf[1] = 0x01;
        if(ledMode == LedOff){
            g_out_packet_buf[2] = 0x00;
        }else if(ledMode == LedAuto){
            g_out_packet_buf[2] = 0x01;
        }else if(ledMode == LedManual){
            g_out_packet_buf[2] = 0x02;
        }
        brightnessIntVal = brightnessVal.toInt();
        g_out_packet_buf[3] = (unsigned char)(brightnessIntVal & 0xFF);

        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);
        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setLEDStatusMode: libusb_control_transfer set command failed");            
            return void();
        }
    }
    else{        
        emit logHandle(QtCriticalMsg, "setLEDStatusMode: led mode is not in range");
        return void();
    }

}

void ASCELLA::setAutoFocusMode(camAfMode afMode){

    int bytesSent;

    if(uvccamera::handle == NULL)
    {
        emit logHandle(QtCriticalMsg, "setAutoFocusMode: Handle is Null");
        return void();
    }

    if(afMode == Continuous || afMode == OneShot)
    {
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

        g_out_packet_buf[1] = 0x04;
        if(afMode == Continuous)
            g_out_packet_buf[2] = 0x03;
        else if(afMode == OneShot)
            g_out_packet_buf[2] = 0x00;

        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);
        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setAutoFocusMode: set command failed");
            return void();
        }
    }
    else{
        emit logHandle(QtCriticalMsg, "setAutoFocusMode: auto focus mode is not in range");
        return void();
    }    

}

void ASCELLA::setExposureCompensation(QString exposureVal){

    int bytesSent;
    u_int8_t exposureIntVal;

    if(uvccamera::handle == NULL)
    {
        emit logHandle(QtCriticalMsg, "setExposureCompensation: Handle is Null");
        return void();
    }

    memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

    g_out_packet_buf[1] = 0x03;
    exposureIntVal = exposureVal.toInt();
    g_out_packet_buf[2] = (unsigned char)(exposureIntVal & 0xFF);

    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0x21,
                                        0x09,
                                        0x200,
                                        0x2,
                                        g_out_packet_buf,
                                        ASCELLA_BUFLEN,
                                        ASCELLA_TIMEOUT);
    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "setExposureCompensation: libusb_control_transfer set command failed");
        return void();
    }    
}


void ASCELLA::setSceneMode(camSceneMode sceneMode){

    int bytesSent;

    if(uvccamera::handle == NULL)
    {
        emit logHandle(QtCriticalMsg, "setSceneMode: Handle is Null");
        return void();
    }
    if(sceneMode == SceneNormal || sceneMode == SceneDocScan){
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

        g_out_packet_buf[1] = 0x09;
        if(sceneMode == 0x01)
            g_out_packet_buf[2] = 0x00;
        else if(sceneMode == 0x02)
            g_out_packet_buf[2] = 0x20;

        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);

        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setSceneMode: libusb_control_transfer set command failed");
            return void();
        }
    }
    else{
        emit logHandle(QtCriticalMsg, "setSceneMode: scene mode is not in range");
        return void();
    }    

}


void ASCELLA::setNoiseReduceMode(camNoiseReduceMode NoiseReduceMode, QString NoiseReduceFixVal){

    int bytesSent;
    u_int8_t noiseReduceFixIntVal;    

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setNoiseReduceMode: Handle is Null");
        return void();
    }

    if(NoiseReduceMode == NoiseReduceNormal || NoiseReduceMode == NoiseReduceFix){
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

        g_out_packet_buf[1] = 8;
        if(NoiseReduceMode == NoiseReduceNormal)
            g_out_packet_buf[2] = 0x00;
        else if(NoiseReduceMode == NoiseReduceFix){
            noiseReduceFixIntVal = NoiseReduceFixVal.toInt();
            noiseReduceFixIntVal |= 0x80;
            g_out_packet_buf[2] = (unsigned char)(noiseReduceFixIntVal & 0xFF);
       }

        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);

        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setNoiseReduceMode: libusb_control_transfer set command failed");
            return void();
        }
    }
    else{
        emit logHandle(QtCriticalMsg, "setNoiseReduceMode: NoiseReduceMode is not in range");
        return void();
    }    

}

void ASCELLA::setLimitMaxFrameRateMode(camLimitMaxFRMode LimitMaxFRMode, QString maxFrameRateVal){

    int bytesSent;
    u_int8_t maxFRIntVal;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setLimitMaxFrameRate: Handle is Null");
        return void();
    }
    if(LimitMaxFRMode == Disable || LimitMaxFRMode == ApplyMaxFrameRate){
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

        g_out_packet_buf[1] = 10;
        if(LimitMaxFRMode == Disable)
            g_out_packet_buf[2] = 0x00;
        else if(LimitMaxFRMode == ApplyMaxFrameRate){
            maxFRIntVal = maxFrameRateVal.toInt();
            g_out_packet_buf[2] = (unsigned char)(maxFRIntVal & 0xFF);
        }
        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);

        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setLimitMaxFrameRateMode: libusb_control_transfer set command failed");
            return void();
        }
    }
    else{
        emit logHandle(QtCriticalMsg, "setLimitMaxFrameRateMode: LimitMaxFRMode is not in range");
        return void();
    }
}

void ASCELLA::setColorMode(camColorMode colorMode, QString blackwhiteThreshold){

    int bytesSent;
    u_int8_t bwThresholdIntVal;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setColorMode: Handle is Null");
        return void();
    }

    if(colorMode == ColorModeNormal || colorMode == ColorModeMono || colorMode == ColorModeNegative || colorMode == ColorModeBlackWhite){
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

        g_out_packet_buf[1] = 0x06;
        if(colorMode == ColorModeNormal)
            g_out_packet_buf[2] = 0x00;
        else if(colorMode == ColorModeMono)
            g_out_packet_buf[2] = 0x01;
        else if(colorMode == ColorModeNegative)
            g_out_packet_buf[2] = 0x03;
        else if(colorMode == ColorModeBlackWhite){
            bwThresholdIntVal = blackwhiteThreshold.toInt();
            g_out_packet_buf[2] = 0x0A;
            g_out_packet_buf[3] = bwThresholdIntVal;
        }

        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);

        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setColorMode: libusb_control_transfer set command failed");
            return void();
        }
    }
    else{
        emit logHandle(QtCriticalMsg, "setColorMode: colorMode is not in range");
        return void();
    }    

}


void ASCELLA::setCenterWeightedAutoFocus(){

    int bytesSent;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setCenterWeightedAutoFocus: Handle is Null");
        return void();
    }
    memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

    g_out_packet_buf[1] = 0xFF;
    g_out_packet_buf[2] = 0x01;

    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0x21,
                                        0x09,
                                        0x200,
                                        0x2,
                                        g_out_packet_buf,
                                        ASCELLA_BUFLEN,
                                        ASCELLA_TIMEOUT);

    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "setCenterWeightedAutoFocus: libusb_control_transfer set command failed");
        return void();
    }

}

bool ASCELLA::setCustomAreaAutoFocus(QString afHoriStart, QString afHoriEnd, QString afVertiStart, QString afVertiEnd){

    int bytesSent;
    int afHoriStartIntVal;
    int afHoriEndIntVal;
    int afVertiStartIntVal;
    int afVertiEndIntVal;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setCustomWeightedAutoFocus: Handle is Null");
        return false;
    }

    afHoriStartIntVal = afHoriStart.toInt();
    afHoriEndIntVal  = afHoriEnd.toInt();
    afVertiStartIntVal = afVertiStart.toInt();
    afVertiEndIntVal = afVertiEnd.toInt();

    if((afHoriStartIntVal > afHoriEndIntVal) || (afVertiStartIntVal > afVertiEndIntVal)){
        emit deviceStatus("Error", "Horizontal/Vertical AF window start position must be less than or equal to Horizontal/Vertical AF window end postion");
        return false;
    }

    memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

    g_out_packet_buf[1] = 0xFF;
    g_out_packet_buf[2] = 0x02;
    g_out_packet_buf[3] = (afHoriStartIntVal >> 8);
    g_out_packet_buf[4] = (afHoriStartIntVal >> 0);
    g_out_packet_buf[5] = (afHoriEndIntVal >> 8);
    g_out_packet_buf[6] = (afHoriEndIntVal >> 0);
    g_out_packet_buf[7] = (afVertiStartIntVal >> 8);
    g_out_packet_buf[8] = (afVertiStartIntVal >> 0);
    g_out_packet_buf[9] = (afVertiEndIntVal >> 8);
    g_out_packet_buf[10] = (afVertiEndIntVal >> 0);

    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0x21,
                                        0x09,
                                        0x200,
                                        0x2,
                                        g_out_packet_buf,
                                        ASCELLA_BUFLEN,
                                        ASCELLA_TIMEOUT);
    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "setCenterWeightedAutoFocus: libusb_control_transfer set command failed");
        return false;
    }
    return true;
}


void ASCELLA::setBinnedResizedMode(camBinnResizeMode mode){

    int bytesSent;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "setBinnedResizedMode: Handle is Null");
        return void();
    }

    if(mode == Binned || mode == Resized){
        memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);
        g_out_packet_buf[1] = 0xEE;
        g_out_packet_buf[2] = mode;

        bytesSent = libusb_control_transfer(uvccamera::handle,
                                            0x21,
                                            0x09,
                                            0x200,
                                            0x2,
                                            g_out_packet_buf,
                                            ASCELLA_BUFLEN,
                                            ASCELLA_TIMEOUT);
        if(0 > bytesSent){
            emit logHandle(QtCriticalMsg, "setBinnedResizedMode: libusb_control_transfer set command failed");
            return void();
        }
    }
    else{
        emit logHandle(QtCriticalMsg, "setBinnedResizedMode: binned/Resized mode is not in range");
        return void();
    }    

}

#if 0
void ASCELLA::setRollValue(QString rollValue){
    int bytesSent;
    u_int8_t rollIntVal;
    if(uvccamera::handle == NULL)
    {
        emit logHandle(QtCriticalMsg, "setRollValue: Handle is Null");
        return void();
    }

    memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);

    g_out_packet_buf[1] = 0x00;
    rollIntVal = rollValue.toInt();
    g_out_packet_buf[2] = (unsigned char)(rollIntVal & 0xFF);
    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0x21,
                                        0x09,
                                        0x200,
                                        0x2,
                                        g_out_packet_buf,
                                        ASCELLA_BUFLEN,
                                        10000);
    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "setRollValue: libusb_control_transfer set command failed");
        return void();
    }
}
#endif

void ASCELLA::setDefaultValues(){

    // For setting default value, it will take some time to set values in the camera. So for each request , 50ms delay is added.
    u_int8_t defaultValue[30];

    memset(defaultValue, 0x00, sizeof(defaultValue));

    /* get default values */
    getDefaultValues(defaultValue);

    /* Enabling led off */
    QString brightness = QString::number(defaultValue[1]);
    emit ledOffEnable(brightness);
    setLEDStatusMode(LedOff, brightness);

    usleep(50000);

    /* Enabling exposure compensation if auto exposure is selected in Image Quality Settings menu */
    QString exposureValue = QString::number(defaultValue[2]);
    emit autoExposureEnable(exposureValue);
    usleep(50000);

    /* Enabling normal color mode */
    emit normalColorModeEnable();
    usleep(50000);

    /* Enabling black and white color mode auto and setting manual default slider value */
    QString bwThresholdValue = QString::number(defaultValue[18]);
    emit bwColorModeAutoEnable(bwThresholdValue);
    setColorMode(ColorModeNormal, bwThresholdValue);
    usleep(50000);

    /* Enabling auto focus mode control if auto focus is checked in Image Quality Settings menu*/
    emit afContinuousEnable();
    usleep(50000);

    /* Enabling noise reduction auto mode */
    emit noiseReductionAutoEnable();
    usleep(50000);

    /* Enabling normal scene mode */
    emit normalSceneModeEnable();
    usleep(50000);

    /* Enabling limit max frame rate disable mode and setting max frame rate value*/
    QString frameRateValue = QString::number(ASCELLA_DEFAULT_MAXFRAMERATE);
    emit limitMaxFRDisableMode(frameRateValue);
    usleep(50000);

    /* Enabling binned/resized mode */
    QString binResizeMode = QString::number(defaultValue[19]);
    emit setBinnResizeEnableDisable(binResizeMode);
    usleep(50000);

    /* Select binn or resize mode */
    QString binResizeSelect = QString::number(defaultValue[20]);
    emit setBinnResizeSelect(binResizeSelect);
    usleep(50000);

    /* Enabling auto focus area center mode */
    emit setAfAreaCenterMode();
    usleep(50000);

}

void ASCELLA::getDefaultValues(u_int8_t *pDefaultValue){

    int bytesSent;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "getDefaultValues: Handle is Null");
        return void();
    }

    memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);
    memset(g_in_packet_buf, 0x00, ASCELLA_BUFLEN);

    g_out_packet_buf[1] = 0xCC;
    g_out_packet_buf[2] = 1;

    // Sending the request command to get default values - output buffer
    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0x21,
                                        0x09,
                                        0x200,
                                        0x2,
                                        g_out_packet_buf,
                                        ASCELLA_BUFLEN,
                                        ASCELLA_TIMEOUT);

    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "getDefaultValues: libusb_control_transfer set command failed");
        return void();
    }

    // Getting the response - in buffer
    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0xA1,
                                        0x01,
                                        0x100,
                                        0x2,
                                        g_in_packet_buf,
                                        ASCELLA_BUFLEN,
                                        ASCELLA_TIMEOUT);
    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "getDefaultValues: libusb_control_transfer get command failed");
        return void();
    }

    pDefaultValue[0] = 0x00; //0 - 0x00 //Torch Status: 0 for OFF, 1 for Auto ON, 2 for Manual ON
    pDefaultValue[1] = 0x01; //1 - 0x01 //Torch level: 0-100 supported
    pDefaultValue[2] = 0x06; //2 - 0x06 //Exposure Compensation level: 0-12 supported
    pDefaultValue[3] = 0x03; //3 - 0x03 //AF mode: 0 for one-shot AF, 3 for continuous
    pDefaultValue[4] = 0x00; //4 - 0x00 //Color Mode: 0 for Normal, 1 for Mono, 3 for Negative, 10 for B&W
    pDefaultValue[5] = 0x00; //5 - 0x00 //B&W Threshold: 0 for Auto, 1-255 for Fix Threshold value
    pDefaultValue[6] = 0x00; //6 - 0x00 //bit 7: 0 for Auto, 1 for Fix level; bit 0-6: 1-10 supported
    pDefaultValue[7] = 0x00; //7 - 0x00 //Scene mode: 00 for normal, 0x20 for Document Scanner
    pDefaultValue[8] = 0x00; //8 - 0x00 //Max Frame Rate Control: 0 for Disabling MAx Frame Rate Control, 3-119 for Fixing the frame rate
    pDefaultValue[9] = 0x01;  //9 - 0x01 //AF mode: 1 for center weighted, 2 for custom weighted
    for(int i = 10;i < 18;i++)
        pDefaultValue[i] = 0x00; // 10 to 17 - 0x00 // Auto focus area horizontal and vertical position
    pDefaultValue[18] = 0x01; //18 - 0x01 //B&W: Previous B&W Fix Threshold Value
    pDefaultValue[19] = g_in_packet_buf[20]; //19 - Enable binning/resize; 0 for disable, 1 for enable
    pDefaultValue[20] = g_in_packet_buf[21]; //20 - 1 for binning, 2 for resize
}


void ASCELLA::getCurrentValues(u_int8_t *pCurrentValue){
    int bytesSent;

    if(uvccamera::handle == NULL){
        emit logHandle(QtCriticalMsg, "getCurrentValues: Handle is Null");
        return void();
    }

    memset(g_out_packet_buf, 0x00, ASCELLA_BUFLEN);
    memset(g_in_packet_buf, 0x00, ASCELLA_BUFLEN);

    g_out_packet_buf[1] = 0xCC;
    g_out_packet_buf[2] = 2;

    // Sending the request command to get current values - output buffer
    bytesSent = libusb_control_transfer(uvccamera::handle,
                                        0x21,
                                        0x09,
                                        0x200,
                                        0x2,
                                        g_out_packet_buf,
                                        ASCELLA_BUFLEN,
                                        ASCELLA_TIMEOUT);
    if(0 > bytesSent){
        emit logHandle(QtCriticalMsg, "getCurrentValues: libusb_control_transfer set command failed");
        return void();
    }

    usleep(100000); // sleep for 100ms
    bytesSent = libusb_control_transfer(uvccamera::handle,
                                               0xA1, // req type
                                               0x01, // request
                                               0x100, // value
                                               0x2, // index
                                               g_in_packet_buf,
                                               ASCELLA_BUFLEN,
                                               ASCELLA_TIMEOUT);
    if(0 > bytesSent){       
       emit logHandle(QtCriticalMsg, "getCurrentValues: libusb_control_transfer get command failed");
       return void();
    }

    int i=0;
    /* pCurrentValue[0] - led status mode */
    switch(g_in_packet_buf[1]){
        case 0x00:
            pCurrentValue[i] = 0x01; break;
        case 0x01:
            pCurrentValue[i] = 0x02; break;
        case 0x02:
            pCurrentValue[i] = 0x03; break;
    }

    i++;
    /* pCurrentValue[1] - Led Brightness value */
    pCurrentValue[i] = g_in_packet_buf[2];
    i++;

    /* pCurrentValue[2] - Exposure */
    pCurrentValue[i] = g_in_packet_buf[3];    
    i++;

    /* pCurrentValue[3] - Auto focus mode */
    switch(g_in_packet_buf[4]){
        case 0x03:
            pCurrentValue[i] = 0x01; break;
        case 0x00:
            pCurrentValue[i] = 0x02; break;
    }
    i++;

    /* pCurrentValue[4] - color mode */
    switch(g_in_packet_buf[5]){
        case 0x00:
            pCurrentValue[i] = 0x01; break;
        case 0x01:
            pCurrentValue[i] = 0x02; break;
        case 0x03:
            pCurrentValue[i] = 0x03; break;
        case 0x0A:
            pCurrentValue[i] = 0x04; break;
    }
    i++;
    /* pCurrentValue[5] - Black and white mode */
    if(g_in_packet_buf[6] == 0x00)
        pCurrentValue[i] = 0x00;      //Black&White Auto
    else if(g_in_packet_buf[6] >= 0x01)
        pCurrentValue[i] = g_in_packet_buf[6]; //black and white thresold

    i++;    

    /* pCurrentValue[6] - noise reduction mode */
    pCurrentValue[i++] = g_in_packet_buf[7];

    /* pCurrentValue[7] - scene mode */
    switch(g_in_packet_buf[8]){
        case 0x00:
            pCurrentValue[i] = 0x01; break;
        case 0x20:
            pCurrentValue[i] = 0x02; break;
    }
    i++;

    /* pCurrentValue[8] - limit max frame rate */
    if(g_in_packet_buf[9] >= 0x03){            // Max frame rate
        pCurrentValue[i] = g_in_packet_buf[9];
    }else if(g_in_packet_buf[9] == 0x00)       // Disable
        pCurrentValue[i] = 0x01;

    i++;


    // pCurrentValue[9] - Auto Focus area either  center or custom
    pCurrentValue[i++] = g_in_packet_buf[10];

    // pCurrentValue[10 - 17] - Auto focus area Horizontal, vertical position
    // pCurrentValue[18] - black and white threshold
    // pCurrentValue[19] - enable binning/resizing
    // pCurrentValue[20] - select binn or resize mode
    // pCurrentValue[21 - 27] -  firmware version
    for(int j = 0;j < 18 ; j++){
        pCurrentValue[i++] = g_in_packet_buf[11 + j];
    }
}


void ASCELLA::setCurrentValues(QString vidResolution){
    u_int8_t currentValue[30], noiseValue;
    u_int16_t position = 0;
    QString curPosition, ledCurBrightness;
    int vidWidth, vidHeight;

    memset(currentValue, 0x00, sizeof(currentValue));
    getCurrentValues(currentValue);

    /* Set Led mode and led brightness */
    if(currentValue[1] < 1)
        ledCurBrightness = QString::number(10);
    else
        ledCurBrightness = QString::number(currentValue[1]);

    if(currentValue[0] == 0x01)
        emit setCurrentLedValue(LedOff, ledCurBrightness);
    else if(currentValue[0] == 0x02)
        emit setCurrentLedValue(LedAuto, ledCurBrightness);
    else if(currentValue[0] == 0x03)
        emit setCurrentLedValue(LedManual, ledCurBrightness);

    /* Set auto exposure mode */
    QString curExposure = QString::number(currentValue[2]);
    emit setCurAutoExposureEnable(curExposure);

    /* Set Auto focus mode */
    QString afMode = QString::number(currentValue[3]);
    emit setCurrentAfMode(afMode);

    /* Set current color mode */
    QString curColorMode = QString::number(currentValue[4]);
    emit setCurrentColorMode(curColorMode);

    /* Set Black and white mode */
    QString curBWMode = QString::number(currentValue[5]);
    QString CurBWthreshold = QString::number(currentValue[18]);

    emit setCurrentBwMode(curBWMode, CurBWthreshold);

    /* Set noise reduction mode */
    noiseValue = currentValue[6];
    QString curNoiseValue = QString::number(noiseValue);

    //Modified by Nithyesh
    /*
     * Condition is always true as currentValue[6] is of uint type.
     * Previous check was if(currentValue[6] >= 0x00 && currentValue[6] <= 0x0A)
     */
    if(currentValue[6] <= 0x0A){
        emit setCurNoiseReductionMode(curNoiseValue, NoiseReduceNormal);
    }else if(currentValue[6] >= 0x80){
        noiseValue ^= 0x80;
        QString curNoiseValue = QString::number(noiseValue);
        emit setCurNoiseReductionMode(curNoiseValue, NoiseReduceFix);
    }

    /* scene mode */
    QString curSceneMode = QString::number(currentValue[7]);
    emit setCurSceneMode(curSceneMode);

    /* set current limit frame rate mode */
    QString curFRLimit = QString::number(currentValue[8]);
    QString maxFrameRate = QString::number(ASCELLA_DEFAULT_MAXFRAMERATE);
    if(currentValue[8] == 0x01){
        emit setCurFRMode(Disable, maxFrameRate);
    }else if(currentValue[8] >= 0x03){
        emit setCurFRMode(ApplyMaxFrameRate, curFRLimit);
    }

    vidWidth = vidResolution.split('x')[0].toInt();
    vidHeight = vidResolution.split('x')[1].toInt();
    if(currentValue[9] == 0x01){ /* set auto focus area center mode */
        emit setCurrentAfAreaCenterMode();
    }else if(currentValue[9] == 0x02){ /* set auto focus area custom mode */
        position = 256 * currentValue[10] + currentValue[11];
        curPosition = QString::number(position);
        if(position > 0){
            if(position <= vidWidth){
                emit setCurrentAfAreaCustomMode(curPosition, "0x01");   /* set auto focus area - Hori start position */
            }
        }

        position = 256 * currentValue[12] + currentValue[13];
        curPosition = QString::number(position);
        if(position > 0){
            if(position <= vidWidth){
                emit setCurrentAfAreaCustomMode(curPosition, "0x02");   /* set auto focus area - Hori end position */
            }
        }

        position = 256 * currentValue[14] + currentValue[15];
        curPosition = QString::number(position);
        if(position > 0){
            if(position <= vidHeight){
                emit setCurrentAfAreaCustomMode(curPosition, "0x03");   /* set auto focus area - Verti start position */
            }
        }

        position = 256 * currentValue[16] + currentValue[17];
        curPosition = QString::number(position);
        if(position > 0){
            if(position <= vidHeight){
                emit setCurrentAfAreaCustomMode(curPosition, "0x04");   /* set auto focus area - Verti end position */
            }
        }
    }
    /* Enable/Disable binn resize modes */
    QString binResizeMode = QString::number(currentValue[19]);
    emit setCurrbinnResizeEnableDisable(binResizeMode);

    /* Select binn or resize mode */
    QString binResizeSelect = QString::number(currentValue[20]);
    emit setCurbinnResizeSelect(binResizeSelect);

}


void ASCELLA::getFirmwareVersion(){
    u_int8_t currentValue[30];

    emit logHandle(QtDebugMsg,"Firmware version:");
    _title = tr("Firmware Version");

    getCurrentValues(currentValue);
    //currentValue[21]: CX3 Firmware Version-Major
    //currentValue[22]: CX3 Firmware Version-Minor
    //currentValue[23]: CX3 Build Date- Year
    //currentValue[24]: CX3 Build Date- Month
    //currentValue[25]: CX3 Build Date- Date
    //currentValue[26]: THP7312 Firmware Version-Major
    //currentValue[27]: THP7312 Firmware Version-Minor
    _text.clear();
    _text.append("Thine ISP Firmware Version: ");
    _text.append(QString::number(currentValue[26]).append(".").append(QString::number(currentValue[27])));
    _text.append("\nCypress CX3 Firmware Version: ");
    _text.append(QString::number(currentValue[21]).append(".").append(QString::number(currentValue[22])));
    _text.append("\nBuild Date: ");
    _text.append(QString::number(currentValue[25]).append("/").append(QString::number(currentValue[24]).append("/").append(QString::number(currentValue[23]))));
    emit titleTextChanged(_title, _text);
}

