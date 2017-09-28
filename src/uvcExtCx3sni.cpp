/*
 * uvcExtCx3sni.cpp -- preview the camera buffer
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

#include "uvcExtCx3sni.h"



UVCExtCx3SNI::UVCExtCx3SNI()
{
  gvobj = NULL;
}


UVCExtCx3SNI::~UVCExtCx3SNI()
{

}

/**
 * @brief UVCExtCx3SNI::initUVCExtensionUnit
 * @param vobj - pass video stream class object
 */
void UVCExtCx3SNI::initUVCExtensionUnit(Videostreaming *vobj){   
   if(!vobj){       
   	qDebug()<<"video streaming class object - NULL";
   	return;
   }
   gvobj = vobj;
}

void UVCExtCx3SNI::deInitUVCExtensionUnit(){
    gvobj = NULL;
}

/**
 * @brief UVCExtCx3SNI::setLedBrightness - setting brightness value
 * @param brightnessValue - brightness value to set
 * @return true/false
 */
bool UVCExtCx3SNI::setLedBrightness(uint brightnessValue){
    __u8 brightness[1];

    brightness[0] = brightnessValue;
    return setCurrentValueCmd(CX3_UVC_XU_VC_LED_FLASH_CONTROL, brightness);
}


/**
 * @brief UVCExtCx3SNI::getLedBrightnessValue
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
 * @return true/false
 */
bool UVCExtCx3SNI::getLedBrightness(uint queryType){
    __u8 outputBrightnessVal;

    if(getValueCmd(CX3_UVC_XU_VC_LED_FLASH_CONTROL, queryType, 1, &outputBrightnessVal)){        
        emit ledBrightnessValueReceived(queryType, outputBrightnessVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setAWBPresetControl - Setting Auto whitebalance preset control
 * @param awbPresetMode - auto white balance mode 
 * @return true/false
 */
bool UVCExtCx3SNI::setAWBPresetControl(uint awbPresetMode){
   __u8 awbPreset[1];

   awbPreset[0] = awbPresetMode;
   return setCurrentValueCmd(CX3_UVC_XU_VC_AWB_PRESET_CONTROL, awbPreset);
}

/**
 * @brief UVCExtCx3SNI::getAWBPresetMode - Getting AWB preset mode value
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * @return true/false
 */
bool UVCExtCx3SNI::getAWBPresetMode(uint queryType){
    __u8 outputAWBModeVal;

    if(getValueCmd(CX3_UVC_XU_VC_AWB_PRESET_CONTROL, queryType, 1, &outputAWBModeVal)){
        emit awbPresetsModeReceived(queryType, outputAWBModeVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setAWBLock - Setting Auto whitebalance lock - If it is true, we cant change auto white balance effect
 * @param locked - true/false 
 * @return true/false
 */
bool UVCExtCx3SNI::setAWBLock(bool locked){
    __u8 awbLock[1];
    awbLock[0] = __u8(locked);
    return setCurrentValueCmd(CX3_UVC_XU_VC_AWB_LOCK_CONTROL, awbLock);
}

/**
 * @brief UVCExtCx3SNI::getAWBLock - Getting current Auto whitebalance lock value
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * @return true/false
 */
bool UVCExtCx3SNI::getAWBLock(uint queryType){
    __u8 outputAWBLockVal;

    if(getValueCmd(CX3_UVC_XU_VC_AWB_LOCK_CONTROL, queryType, 1, &outputAWBLockVal)){
        emit awbLockValueReceived(queryType, outputAWBLockVal);
        return true;
    }
    else
        return false;
}


/**
 * @brief UVCExtCx3SNI::setAEMeterMode - Setting Auto Exposure meter mode control
 * @param aeMeterMode - auto exposure meter mode 
 * @return true/false
 */
bool UVCExtCx3SNI::setAEMeterMode(uint aeMeterMode){
   __u8 aeMeter[1];

   aeMeter[0] = aeMeterMode;
   return setCurrentValueCmd(CX3_UVC_XU_VC_AE_METERING_MODE_CONTROL, aeMeter);
}


/**
 * @brief UVCExtCx3SNI::getAEMeterMode - Getting AE metering mode value from camera
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * return true/false
 */
bool UVCExtCx3SNI::getAEMeterMode(uint queryType){
    __u8 outputAEModeVal;

    if(getValueCmd(CX3_UVC_XU_VC_AE_METERING_MODE_CONTROL, queryType, 1, &outputAEModeVal)){
        emit autoExpModeReceived(queryType, outputAEModeVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setCAFScanRangeMode - Setting scan Range mode control
 * @param scanRangeMode - scan Range mode 
 * @return true/false
 */
bool UVCExtCx3SNI::setCAFScanRangeMode(uint cafScanRangeMode){
   __u8 cafScanRange[1];

   cafScanRange[0] = cafScanRangeMode;
   return setCurrentValueCmd(CX3_UVC_XU_VC_CAF_SCAN_RANGE_CONTROL, cafScanRange);
}


/**
 * @brief UVCExtCx3SNI::getCAFScanRangeMode - Getting Scan Range mode value from camera
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * @return true/false
 */
bool UVCExtCx3SNI::getCAFScanRangeMode(uint queryType){
    __u8 outputScanRangeModeVal;

    if(getValueCmd(CX3_UVC_XU_VC_CAF_SCAN_RANGE_CONTROL, queryType, 1, &outputScanRangeModeVal)){
        emit cafScanRangeModeReceived(queryType, outputScanRangeModeVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setAWBLock - Setting Auto exposure lock - If it is true, we cant change auto exposure effect
 * @param locked - true/false 
 * @return true/false
 */
bool UVCExtCx3SNI::setAELock(bool locked){
    __u8 aeLock[1];

    aeLock[0] = __u8(locked);
    return setCurrentValueCmd(CX3_UVC_XU_VC_AE_LOCK_CONTROL, aeLock);
}

/**
 * @brief UVCExtCx3SNI::getAELock - Getting current Auto exposure lock value
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * @return true/false
 */
bool UVCExtCx3SNI::getAELock(uint queryType){
    __u8 outputAELockVal;

    if(getValueCmd(CX3_UVC_XU_VC_AE_LOCK_CONTROL, queryType, 1, &outputAELockVal)){
        emit autoExpLockValueReceived(queryType, outputAELockVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setFlickerMode - setting flicker mode
 * @param flickerMode - input flicker mode to set 
 * @return true/false
 */
bool UVCExtCx3SNI::setFlickerMode(uint flickerMode){
   __u8 flicker[1];

   flicker[0] = flickerMode;
   return setCurrentValueCmd(CX3_UVC_XU_VC_FLICKER_CONTROL, flicker);
}

/**
 * @brief UVCExtCx3SNI::getFlickerMode - Getting flicker mode value from camera
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * return true/false
 */
bool UVCExtCx3SNI::getFlickerMode(uint queryType){
    __u8 flickerModeVal;

    if(getValueCmd(CX3_UVC_XU_VC_FLICKER_CONTROL, queryType, 1, &flickerModeVal)){
        emit flickerModeReceived(queryType, flickerModeVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setJpegQfactor - setting JPEG Q value
 * @param jpegQValue - Jpeg Q value 
 * @return true/false
 */
bool UVCExtCx3SNI::setJpegQfactor(uint jpegQValue){
    __u8 jpegQ[1];

    jpegQ[0] = jpegQValue;
    return setCurrentValueCmd(CX3_UVC_XU_VC_JPEG_Q_CONTROL, jpegQ);
}

/**
 * @brief UVCExtCx3SNI::getJpegQfactor - Getting Led jpeg Q value from camera
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN / UVC_GET_MAX/ UVC_GET_RES
 * return true/false
 */
bool UVCExtCx3SNI::getJpegQfactor(uint queryType){
    __u8 outputJpegQVal = 0;

    if(getValueCmd(CX3_UVC_XU_VC_JPEG_Q_CONTROL, queryType, 1, &outputJpegQVal)){
         emit jpegQValueReceived(queryType, outputJpegQVal);
        return true;
    }
    else
        return false;
}


/**
 * @brief UVCExtCx3SNI::setManualLensPositionMode - Setting manual lens position mode
 * @param lensPosMode - lens position mode 
 * @return true/false
 */
bool UVCExtCx3SNI::setManualLensPositionMode(uint lensPosMode){
    __u8 lensPos[1];

    lensPos[0] = lensPosMode;
    return setCurrentValueCmd(CX3_UVC_XU_VC_FOCUS_LENS_POSITION_CONTROL, lensPos);
}


/**
 * @brief UVCExtCx3SNI::getManualLensPositionMode - Getting manual lens position mode value from camera
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * return true/false
 */
bool UVCExtCx3SNI::getManualLensPositionMode(uint queryType){
    __u8 outputManualLensPosModeVal;

    if(getValueCmd(CX3_UVC_XU_VC_FOCUS_LENS_POSITION_CONTROL, queryType, 1, &outputManualLensPosModeVal)){
        emit manualLensPositionModeReceived(queryType, outputManualLensPosModeVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setAutoFocusMode - setting auto Focus mode
 * @param autoFocusMode - set/unset autofocus value 
 * @return true/false
 */
bool UVCExtCx3SNI::setAutoFocusMode(uint autoFocusMode){
    __u8 autoFocus[1];    

    autoFocus[0] = autoFocusMode;
    return setCurrentValueCmd(CX3_UVC_XU_VC_AF_SINGLE_CONTROL, autoFocus);
}

/**
 * @brief UVCExtCx3SNI::getAutoFocusMode - Getting auto focus mode
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF 
 * @return true/false
 */
bool UVCExtCx3SNI::getAutoFocusMode(uint queryType){
    __u8 autoFocusModeVal;

    if(getValueCmd(CX3_UVC_XU_VC_AF_SINGLE_CONTROL, queryType, 1,  &autoFocusModeVal)){        
        emit autoFocusModeReceived(queryType, autoFocusModeVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setCenteredAutoFocusMode - Setting centered auto focus mode 
 * @return true/false
 */
bool UVCExtCx3SNI::setCenteredAutoFocusMode(){
    __u8 centerAreaParamters[ROI_DATA_SIZE];    

    if(getValueCmd(CX3_UVC_XU_ROI_CONTROL, UVC_GET_CUR, ROI_DATA_SIZE, centerAreaParamters)){
        centerAreaParamters[8] = 0x08;
        centerAreaParamters[9] = 0x00;
        return setCurrentValueCmd(CX3_UVC_XU_ROI_CONTROL, centerAreaParamters);
    }else
        return false;
}


/**
 * @brief UVCExtCx3SNI::setCustomAreaAutoFocus - Setting custom area auto focus area
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord  - X co-ordinate
 * @param yCord  - Y co-ordinate
 * @param winSize - window size 
 * @return true/false
 */
bool UVCExtCx3SNI::setCustomAreaAutoFocus(int vidResolnWidth, int vidResolnHeight, uint xCord, uint yCord, QString winSize){

    __u8 customAreaParamters[ROI_DATA_SIZE];

    memset(customAreaParamters, 0x00, ROI_DATA_SIZE);

    unsigned int calculatedWinWidth =  (vidResolnWidth * winSize.toUInt()) / MAX_WINDOW_SIZE;
    unsigned int calculatedWinHeight =  (vidResolnHeight * winSize.toUInt()) / MAX_WINDOW_SIZE;

    /* calculate focus window coordinates based on mouseclick point and window size */
    int focusWinXmin = xCord - calculatedWinWidth/2;
    int focusWinYmin = yCord - calculatedWinHeight/2;
    int focusWinXmax = xCord + calculatedWinWidth/2;
    int focusWinYmax = yCord + calculatedWinHeight/2;

    //  if resultant calculated focus window co-ordinates values lie less than zero , set to zero
    if(0 > focusWinXmin){ focusWinXmin = 0; }
    if(0 > focusWinYmin){ focusWinYmin = 0; }

    if(0 > focusWinXmax){ focusWinXmax = 0; }
    if(0 > focusWinYmax){ focusWinYmax = 0; }

    //  if resultant calculated focus window co-ordinates x,y values lie  greater than preview width, height respectively ,
    //  set the x,y corrdinate values to width, height respectively
    if(vidResolnWidth < focusWinXmax)   {  focusWinXmax = vidResolnWidth; }
    if(vidResolnHeight < focusWinYmax)  {  focusWinYmax = vidResolnHeight; }

    if(vidResolnWidth < focusWinXmin)   {  focusWinXmin = vidResolnWidth; }
    if(vidResolnHeight < focusWinYmin)  {  focusWinYmin = vidResolnHeight; }

    customAreaParamters[0] = focusWinYmin;
    customAreaParamters[1] = focusWinYmin >> 8;
    customAreaParamters[2] = focusWinXmin;
    customAreaParamters[3] = focusWinXmin >> 8;
    customAreaParamters[4] = focusWinYmax;
    customAreaParamters[5] = focusWinYmax >> 8;
    customAreaParamters[6] = focusWinXmax;
    customAreaParamters[7] = focusWinXmax >> 8;
    customAreaParamters[8] = 0x08;
    customAreaParamters[9] = 0x00;

    return setCurrentValueCmd(CX3_UVC_XU_ROI_CONTROL, customAreaParamters);
}

/**
 * @brief UVCExtCx3SNI::getAutoFocusROIMode - get Auto Focus ROI mode
 * @param vobj
 * @return true/false
 */
bool UVCExtCx3SNI::getAutoFocusROIMode(uint queryType){
    __u8 outputValues[ROI_DATA_SIZE];

    // To do: need to clarify ROI control mode when getting current auto focus ROI mode
    if(getValueCmd(CX3_UVC_XU_ROI_CONTROL, queryType, ROI_DATA_SIZE, outputValues)){  
        emit autoFocusRoiModeReceived(queryType, outputValues[GET_FOCUS_ROI_MODE_INDEX]);
       return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setAFLock - set Auto focus lock mode
 * @param locked - select/unselect[boolean] in UI 
 * @return true/false
 */
bool UVCExtCx3SNI::setAFLock(bool locked){  
    __u8 afLock[1];

    if(locked){
        afLock[0] = 0x00;
    }
    else{
        afLock[0] = 0x01;
    }
    return setCurrentValueCmd(CX3_UVC_XU_VC_CAF_LOCK_CONTROL, afLock);
}

/**
 * @brief UVCExtCx3SNI::getAFLock - Get Auto Focus lock mode 
 * @return true/false
 */
bool UVCExtCx3SNI::getAFLock(uint queryType){
    __u8 outputAFLockVal;

    if(getValueCmd(CX3_UVC_XU_VC_CAF_LOCK_CONTROL, queryType, 1, &outputAFLockVal)){
        emit afLockValueReceived(queryType, outputAFLockVal);
        return true;
    }
    else
        return false;
}

/**
 * @brief UVCExtCx3SNI::setCurrentValueCmd - set current value from camera
 * @param controlId - control id
 * @param setVal[] - value(s) to set
 * @return true/false
 */
bool UVCExtCx3SNI::setCurrentValueCmd(__u8 controlId, __u8 setVal[]){
    if(!gvobj){
        return false;
    }

    struct uvc_xu_control_query xquery;
    __u16 size = 0;

    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = (__u8 *)&size;

    if(!gvobj->setUvcExtControlValue(xquery)){        
        return false;
    }

    // To set the current value to the camera
    xquery.query = UVC_SET_CUR;
    xquery.size = size;
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = setVal;

    if(!gvobj->setUvcExtControlValue(xquery)){
        return false;
    }    

    return true;
}

/**
 * @brief UVCExtCx3SNI::getValueCmd - get Current value command multiple values
 * @param controlId - control id
 * @param queryType - UVC_GET_CUR / UVC_GET_DEF / UVC_GET_MIN/ UVC_GET_MAX/ UVC_GET_RES
 * @param numberOfValues - No of values to receive
 * @param outputValues - pointer to hold outputValues 
 * @return true/false
 */
bool UVCExtCx3SNI::getValueCmd(__u8 controlId,  __u8 queryType, uint numberOfValues, __u8 *outputValues){
    if(!gvobj){
        return false;
    }

    if(!outputValues){
        return false;
    }

    struct uvc_xu_control_query xquery;
    __u16 size = 0;

    memset(&xquery, 0, sizeof(xquery));

    // To allocate a sufficiently large buffer and set the buffer size to the correct value
    xquery.query = UVC_GET_LEN;
    xquery.size = 2; /* size seems to always be 2 for the LEN query, but
                       doesn't seem to be documented. Use result for size
                                  in all future queries of the same control number */
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = (__u8 *)&size;

    if(!gvobj->setUvcExtControlValue(xquery)){        
        return false;
    }

    memset(&xquery, 0, sizeof(xquery));

    // get value from camera
    xquery.query = queryType;
    xquery.size = size;
    xquery.selector = controlId;
    xquery.unit = EXTENSION_UNIT_ID;
    xquery.data = outputValues;

    if(!gvobj->setUvcExtControlValue(xquery)){        
        return false;
    }

    __u8 *val = xquery.data;

    for(uint i=0; i<numberOfValues; i++){
        outputValues[i] = val[i];        
    }

    return true;
}

/**
 * @brief UVCExtCx3SNI::getFirmwareVersion - getting ISP and CX3 firmware version 
 * @return true/false
 */
bool UVCExtCx3SNI::getFirmwareVersion(){
    __u8 outputValues[FW_DATA_SIZE];
    _title = "Firmware Version";

    if(!getValueCmd(CX3_UVC_XU_FW_VERSION, UVC_GET_CUR, FW_DATA_SIZE, outputValues)){
        return false;
    }

    // outputValues[27], outputValues[28]: ISP Firmware Version
    // outputValues[4] - outputValues[14]: ISP FW Build Date in the format  "Month Date Year"
    // outputValues[32], outputValues[33]: CX3 Firmware Version
    // outputValues[33] - outputValues[36]: CX3 Firmware build  Month
    // outputValues[38] - CX3 Firmware build date
    // outputValues[39],  outputValues[40] - CX3 Firmware build year
    _text.clear();
    _text.append("ISP Firmware Version: ");
    _text.append(QString::number(outputValues[27]).append(".").append(QString::number(outputValues[28])));

    // traverse buffer index starts from 4 to 14
    _text.append("\nISP Firmware Build Date: ");
    for(int i=4; i<15; i++){
        _text.append(outputValues[i]);
    }

    _text.append("\nCX3 Firmware Version: ");
    _text.append(QString::number(outputValues[32]).append(".").append(QString::number(outputValues[33])));

    // traverse buffer index starts  from 34 to 36
    _text.append("\nCX3 Firmware Build Date: ");
    for(int i=34; i<37; i++){
        _text.append(outputValues[i]); // month
    }
    // buffer index 37 is null character
    _text.append(" ");
    _text.append(QString::number(outputValues[38])).append(" ");
    _text.append(QString::number(outputValues[39]));
    _text.append(QString::number(outputValues[40]));

    emit titleTextChanged(_title, _text);
    return true;
}
