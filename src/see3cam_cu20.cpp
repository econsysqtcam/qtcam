
/*
 * seecam_cu20.cpp -- Handling special feature of seecamcu20 camera
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

#include "see3cam_cu20.h"


/**
 * @brief See3CAM_CU20::setSensorMode - setting sensor mode
 * @param sensorMode - standard/dsr hlo mode
 * @return true/false
 */
bool See3CAM_CU20::setSensorMode(sensorModes  sensorMode)
{    
    // hid validation
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_SENSOR_MODE_CU20; /* set sensor command  */
    g_out_packet_buf[3] = sensorMode; /* pass camera mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){        
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1]==SET_SENSOR_MODE_CU20 &&            
            g_in_packet_buf[6]==SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setSensorMode - setting sensor mode 
 * @return true/false
 */
bool See3CAM_CU20::getSensorMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_SENSOR_MODE_CU20; /* get sensor command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){	
        if (g_in_packet_buf[6]==GET_FAIL) {	    
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1]==GET_SENSOR_MODE_CU20 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit sensorModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setCameraMode - setting camera mode
 * @param cameraMode - master/slave
 * @return true/false
 */
bool See3CAM_CU20::setCameraMode(cameraModes  cameraMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_CAMERA_MODE_CU20; /* set camera mode command  */
    g_out_packet_buf[3] = cameraMode; /* pass camera mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {	    
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_CAMERA_MODE_CU20 &&            
            g_in_packet_buf[6] == SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setCameraMode - getting camera mode
 * @return true/false
 */
bool See3CAM_CU20::getCameraMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_CAMERA_MODE_CU20; /* get camera command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1]==GET_CAMERA_MODE_CU20 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit cameraModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setSpecialMode - Setting special mode in the camera
 * @param specialMode - What special mode need to set
 * @return true/false
 */
bool See3CAM_CU20::setSpecialMode(specialModes  specialMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_SPECIAL_MODE_CU20; /* set special mode command  */
    g_out_packet_buf[3] = specialMode; /* pass special mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_SPECIAL_MODE_CU20 &&            
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getSpecialMode
 * @return true/false
 */
bool See3CAM_CU20::getSpecialMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_SPECIAL_MODE_CU20; /* get special mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1]==GET_SPECIAL_MODE_CU20 &&
            g_in_packet_buf[6]==GET_SUCCESS) {\
            emit specialModeReceived(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setOrientation
 * @param horzModeSel - select/deselect horizontal flip mode
 * @param vertiModeSel - select/deselect vertical flip mode
 * @return true/false
 */
bool See3CAM_CU20::setOrientation(bool horzModeSel, bool vertiModeSel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_ORIENTATION_CU20; /* set orientation command  */

    if(horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SetBothFlipEnable; /* both flip enable */
    }else if(horzModeSel && !vertiModeSel){
        g_out_packet_buf[3] = SetHorzFlip; /* horizontal flip only mode */
    }else if(!horzModeSel && vertiModeSel){
        g_out_packet_buf[3] = SetVertiFlip; /* vertical flip only mode */
    }else
        g_out_packet_buf[3] = SetBothFlipDisable; /* both flip disable */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_ORIENTATION_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {\
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getOrientation - get current orientation set in camera
 * @return true/false
 */
bool See3CAM_CU20::getOrientation()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_ORIENTATION_CU20; /* get orientation command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_ORIENTATION_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {\
            emit flipMirrorModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU20::setStrobeMode - Setting strobe mode in camera
 * @param strobeMode - mode which is need to set
 * @return true/false
 */
bool See3CAM_CU20::setStrobeMode(strobeValues strobeMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_STROBEMODE_CU20; /* set strobe mode command  */
    g_out_packet_buf[3] = strobeMode; /* pass strobe mode value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){        
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_STROBEMODE_CU20 &&            
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getStrobeMode - get Strobe mode
 * @return true/false
 */
bool See3CAM_CU20::getStrobeMode(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_STROBEMODE_CU20; /* get strobe mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){  
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_STROBEMODE_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {\
            emit strobeModeChanged(g_in_packet_buf[2]);            
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_CU20::setROIAutoExposure(uint see3camAeROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize){

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

    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_AE_ROI_MODE_CU20; /* set auto exposure ROI mode command */
    g_out_packet_buf[3] = see3camAeROIMode; /* auto exposure ROI mode value */

    if(see3camAeROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt(); // window size
    }

    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_AE_ROI_MODE_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }

    return false;
}

/**
 * @brief See3CAM_CU20::getAutoExpROIModeAndWindowSize
 * @return true/false
 */
bool See3CAM_CU20::getAutoExpROIModeAndWindowSize(){
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    //Set the Report Number
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_AE_ROI_MODE_CU20; /* get exposure ROI mode  */

    uint roiMode, windowSize = 0;
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {	    
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_AE_ROI_MODE_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            roiMode = g_in_packet_buf[2];
            windowSize = g_in_packet_buf[5];         
            emit roiAutoExpModeChanged(roiMode, windowSize);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setColourKill - set Colour Kill value
 * @param colourKillValue - value to set
 * @return true/false
 */
bool See3CAM_CU20::setColourKill(uint colourKillValue){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_COLORKILLVAL_CU20; /* set color kill status command  */
    g_out_packet_buf[3] = colourKillValue; /* pass colour kill status value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_COLORKILLVAL_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getColourKill - get color kill status value
 * @return true/false
 */
bool See3CAM_CU20::getColourKill(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_COLORKILLVAL_CU20; /* get colour kill command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_COLORKILLVAL_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit colorKillValueChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setBurstLength - Setting burst length value
 * @param burstLength - number of shots to take
 * @return true/false
 */
bool See3CAM_CU20::setBurstLength(uint burstLength){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_BURSTLENGTH_CU20; /* set burst length command  */
    g_out_packet_buf[3] = burstLength; /* set burst length value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_BURSTLENGTH_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getBurstLength - get current value of burst length value
 * @return true/false
 */
bool See3CAM_CU20::getBurstLength(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_BURSTLENGTH_CU20; /* get burst length command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_BURSTLENGTH_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit burstLengthChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setAntiFlickerMode - setting anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - auto/50hz/60hz]
 * @return true/false
 */
bool See3CAM_CU20::setAntiFlickerMode(camAntiFlickerMode antiFlickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_ANTIFLICKER_CU20; /* set anti flicker command  */
    g_out_packet_buf[3] = antiFlickerMode; /* anti flicker mode to set */    
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_ANTIFLICKER_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {            
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getAntiFlickerMode  - get current anti flicker mode from camera
 * @return - true/false
 */
bool See3CAM_CU20::getAntiFlickerMode(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_ANTIFLICKER_CU20; /* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_ANTIFLICKER_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {                        
            emit antiFlickerModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setToDefaultValues - set the extension control values to default
 * @return  true/false
 */
bool See3CAM_CU20::setToDefaultValues(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_CU20; /* set default command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit setdefaultValueFailed();
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_TO_DEFAULT_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setDenoiseCtrlMode - set denoise mode into the camera
 * @param deNoiseMode - denoise mode value to set
 * @return true/false
 */
bool See3CAM_CU20::setDenoiseCtrlMode(denoiseModes  deNoiseMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {        
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_DENOISE_MODE_CU20; /* set special mode command  */
    g_out_packet_buf[3] = deNoiseMode; /* pass denoise mode value */   

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){        
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_DENOISE_MODE_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {            
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getDenoiseCtrlMode - get denoise control mode
 * @return true/false
 */
bool See3CAM_CU20::getDenoiseCtrlMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_DENOISE_MODE_CU20; /* get special mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_DENOISE_MODE_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit denoiseModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::setLSCMode - set LSC mode
 * @param LSC mode to set
 * @return true/false
 */
bool See3CAM_CU20::setLSCMode(uint lscMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = SET_LSCMODE_CU20; /* set Lsc mode command  */
    g_out_packet_buf[3] = lscMode; /* LSC Mode value to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){        
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == SET_LSCMODE_CU20 &&
            g_in_packet_buf[6] == SET_SUCCESS) {           
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::getLSCMode - get current LSC mode from camera
 * @return true/false
 */
bool See3CAM_CU20::getLSCMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU20; /* camera id */
    g_out_packet_buf[2] = GET_LSCMODE_CU20; /* get LSC mode command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU20 &&
            g_in_packet_buf[1] == GET_LSCMODE_CU20 &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit lscModeChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU20::initializeBuffers - Initialize input and output buffers
 */
void See3CAM_CU20::initializeBuffers(){

    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

