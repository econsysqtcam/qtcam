#include "see3cam_cu135m_h01r1.h"
See3CAM_CU135M_H01R1::See3CAM_CU135M_H01R1()
{
}

See3CAM_CU135M_H01R1::~See3CAM_CU135M_H01R1()
{
}


void See3CAM_CU135M_H01R1::initializeBuffers()
{
    //Initialize input and output buffers
    memset(g_out_packet_buf, 0x00, sizeof(g_out_packet_buf));
    memset(g_in_packet_buf, 0x00, sizeof(g_in_packet_buf));
}

/**
 * @brief See3CAM_CU135M_H01R1::setStreamMode - Setting  Streaming mode
 * @param streamMode - mode selected in UI
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::setStreamMode(streamModes streamMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = SET_STREAM_MODE_CU1330M; /* set stream mode command */
    g_out_packet_buf[3] = streamMode; /* stream mode to set */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==SET_STREAM_MODE_CU1330M &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::getStreamMode - get Stream mode from camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getStreamMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = GET_STREAM_MODE_CU1330M; /* get stream mode command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==GET_STREAM_MODE_CU1330M &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit streamModeValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setFlashState - Setting  flash mode
 * @param flashMode - mode selected in UI
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::setFlashState(flashStateValues flashMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] =CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* set camera control code */
    g_out_packet_buf[2] = SET_FLASH_MODE_CU1330M ; /* set flash mode code */
    g_out_packet_buf[3] = flashMode; /* actual flash mode */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==SET_FLASH_MODE_CU1330M  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }

    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getFlashState - Getting  flash mode
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::getFlashState()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;  /* set camera control code */
   g_out_packet_buf[2] = GET_FLASH_MODE_CU1330M ; /* get flash mode code */

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
           g_in_packet_buf[1]==GET_FLASH_MODE_CU1330M  &&
           g_in_packet_buf[6]==GET_SUCCESS) {
           emit flashModeValue(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setGainMode - Setting orientation - set Normal/horizontal/vertical/Rotate180
 * @param - hFlip - To set horizondal mode
 * @param - vFlip - To set vertical mode
 * return true/false
*/
bool See3CAM_CU135M_H01R1::setFlipCtrlValue(bool hFlip ,bool vFlip)
{
    u_int8_t flipMode;

    if(hFlip && vFlip){
        flipMode = FLIP_BOTH; /* both flip enable */
    }else if(hFlip && !vFlip){
        flipMode = FLIP_HORIZONTAL; /* horizontal flip only mode */
    }else if(!hFlip && vFlip){
        flipMode = FLIP_VERTICAL; /* vertical flip only mode */
    }else{
        flipMode = FLIP_NORMAL;
    }
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* set camera control code */
    g_out_packet_buf[2] = SET_ORIENTATION_CU1330M ; /* set flip control code */
    g_out_packet_buf[3] = flipMode; /* set flipControl value */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==SET_ORIENTATION_CU1330M  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::getFlipCtrlValue - get flip control value from camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getFlipCtrlValue()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* set camera control code */
    g_out_packet_buf[2] = GET_ORIENTATION_CU1330M ; /* get flip control code */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==GET_ORIENTATION_CU1330M  &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit flipCtrlValue(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setROIAutoExposure - Set ROI auto exposure to camera
 * @param camROIAutoExposureMode - ROI mode
 * @param vidResolnWidth - preview resolution width
 * @param vidResolnHeight - preview resolution height
 * @param xCord - mouse xCordinate from preview
 * @param yCord - mouse yCordinate from preview
 * @param winSize - ROI window size
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::setROIAutoExposure(camROIAutoExpMode see3camAutoexpROIMode, uint vidResolnWidth, uint vidResolnHeight, uint xCord, uint yCord, QString winSize)
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

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = SET_EXP_ROI_MODE_CU1330M; /* set exposure ROI command */
    g_out_packet_buf[3] = see3camAutoexpROIMode; /* exposure ROI mode to set */

    if(see3camAutoexpROIMode == AutoExpManual){
        g_out_packet_buf[4] = outputXCord; // x cord
        g_out_packet_buf[5] = outputYCord; // y cord
        g_out_packet_buf[6] = winSize.toUInt(); // window size
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){

        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==SET_EXP_ROI_MODE_CU1330M &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::getAutoExpROIModeAndWindowSize - get ROI auto exposure mode and window size
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getAutoExpROIModeAndWindowSize(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = GET_EXP_ROI_MODE_CU1330M; /* get exposure ROI mode  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==GET_EXP_ROI_MODE_CU1330M &&
            g_in_packet_buf[6]==GET_SUCCESS) {
            emit roiAutoExpMode(g_in_packet_buf[2], g_in_packet_buf[3], g_in_packet_buf[4], g_in_packet_buf[5]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getGainMode - Get gain mode(Auto / Manual)
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::getGainMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();
    uint manualGain;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[2] = GET_AUTO_GAIN_MODE_SEE3CAM_CU135M_H01R1_H;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == GET_AUTO_GAIN_MODE_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == GET_SUCCESS) {

            emit gainModeReceived(g_in_packet_buf[2]);
            emit autoGainModeRecieved(g_in_packet_buf[3]);
            manualGain = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            emit manualGainModeRecieved(manualGain);

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setGainMode - Setting gain attributes
 * @param gainType   - Auto / Manual mode
 * @param autoModes  - Continous / Single shot in Auto gain mode
 * @param manualGain - Manual values set in UI
 * return true/false
*/
bool See3CAM_CU135M_H01R1::setGainMode(gainMode gainType,autoGain autoModes,uint manualGain)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[2] = SET_AUTO_GAIN_MODE_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[3] = gainType;
    if(g_out_packet_buf[3] == 0x00)
    {
        g_out_packet_buf[4] = autoModes;
    }
    else if(g_out_packet_buf[3] == 0x01)
    {
        g_out_packet_buf[5] = ((manualGain & 0xFF00) >> 8);
        g_out_packet_buf[6] = ((manualGain & 0x00FF) >> 0);
    }

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == SET_AUTO_GAIN_MODE_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getGainLimit - To get gain upper and lower limit value in camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getGainLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint lowerLimit, upperLimit;
    uint sliderMin, sliderMax;
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[2] = GET_AUTO_GAIN_LIMIT_SEE3CAM_CU135M_H01R1_H;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[10] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == GET_AUTO_GAIN_LIMIT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[10] == GET_SUCCESS) {

            sliderMin = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);
            sliderMax = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);

            lowerLimit = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            upperLimit = (g_in_packet_buf[8] << 8) | (g_in_packet_buf[9] << 0);

            emit sliderMinimumRecieved(sliderMin);
            emit sliderMaximumRecieved(sliderMax);

            emit lowerGainLimitRecieved(lowerLimit);
            emit upperGainLimitRecieved(upperLimit);

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setGainLimit - set gain limit value in camera
 * @param lowerLimit - set lower limit for gain
 * @param upperLimit - set upper limit for gain
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::setGainLimit(uint lowerLimit, uint upperLimit)
{
    if(lowerLimit > upperLimit)
    {
        emit indicateExposureValueRangeFailure("Failure", "Lower limit should be smaller than upper limit");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[2] = SET_AUTO_GAIN_LIMIT_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[3] = ((lowerLimit & 0xFF00) >> 8);
    g_out_packet_buf[4] = ((lowerLimit & 0x00FF) >> 0);
    g_out_packet_buf[5] = ((upperLimit & 0xFF00) >> 8);
    g_out_packet_buf[6] = ((upperLimit & 0x00FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            emit indicateCommandStatus("Failure","Limit value is failed to set");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == SET_AUTO_GAIN_LIMIT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            lowerLimit = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);
            upperLimit = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);

            emit indicateCommandStatus("Success","Limit value is set successfully!");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setBlackLevelAdjustment - set black level value in camera
 * @param blacklevel - set blacklevel to enable or disable
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::setBlackLevelAdjustment(blackLevelAdjustment blacklevel)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[2] = SET_BLACKLEVEL_CORRECTION_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[3] = blacklevel;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == SET_BLACKLEVEL_CORRECTION_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getBlackLevelAdjustment - get black level adjustment from the camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getBlackLevelAdjustment()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[2] = GET_BLACKLEVEL_CORRECTION_SEE3CAM_CU135M_H01R1_H;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == GET_BLACKLEVEL_CORRECTION_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit blackLevelAdjustmentChanged(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::setTargetBrightness - set brightness value in camera
 * @param targetBrightness - set brightness value to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1 ::setTargetBrightness(uint targetBrightness)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* set camera control code */
    g_out_packet_buf[2] = SET_TARGET_BRIGHTNESS_SEE3CAM_CU135M_H01R1_H ;
    g_out_packet_buf[3] = ((targetBrightness & 0xFF00) >> 8);
    g_out_packet_buf[4] = ((targetBrightness & 0x00FF) >> 0);

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAILURE) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==SET_TARGET_BRIGHTNESS_SEE3CAM_CU135M_H01R1_H  &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }

    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getTargetBrightness - get brightness from the camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getTargetBrightness()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   uint brightness;
   uint sliderMin,sliderMax;
   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;  /* set camera control code */
   g_out_packet_buf[2] = GET_TARGET_BRIGHTNESS_SEE3CAM_CU135M_H01R1_H;

   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[8]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
           g_in_packet_buf[1]==GET_TARGET_BRIGHTNESS_SEE3CAM_CU135M_H01R1_H  &&
           g_in_packet_buf[8]==GET_SUCCESS) {

           sliderMin = (g_in_packet_buf[2] << 8) | (g_in_packet_buf[3] << 0);
           sliderMax = (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
           brightness = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);

           emit brightnessMinSliderReceived(sliderMin);
           emit brightnessMaxSliderReceived(sliderMax);
           emit targetBrightnessReceived(brightness);

           return true;
       }
   }
   return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::setExposureMode - set exposure mode in the camera
 * @param exposureMode - set exposure mode to the camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::setExposureMode(exposureModes exposureMode)
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* set camera control code */
    g_out_packet_buf[2] = SET_EXPOSURE_MODE_SEE3CAM_CU135M_H01R1_H ;
    g_out_packet_buf[3] = exposureMode;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==GET_FAILURE) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]== SET_EXPOSURE_MODE_SEE3CAM_CU135M_H01R1_H  &&
            g_in_packet_buf[6]== SET_SUCCESS) {
            return true;
        }
    }

    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getExposureMode - get exposure mode from the camera
 * return true - success /false - failure
 */
bool See3CAM_CU135M_H01R1::getExposureMode()
{
   // hid validation
   if(uvccamera::hid_fd < 0)
   {
       return false;
   }

   //Initialize buffers
   initializeBuffers();

   // fill buffer values
   g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H;  /* set camera control code */
   g_out_packet_buf[2] = GET_EXPOSURE_MODE_SEE3CAM_CU135M_H01R1_H;
   // send request and get reply from camera
   if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
       if (g_in_packet_buf[6]==GET_FAILURE) {
           return false;
       } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
           g_in_packet_buf[1]== GET_EXPOSURE_MODE_SEE3CAM_CU135M_H01R1_H  &&
           g_in_packet_buf[6]== GET_SUCCESS) {
           emit exposureModeRecieved(g_in_packet_buf[2]);
           return true;
       }
   }
   return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::getAutoExposureLowerLimit  - get current lower limit exposure from camera
 * @return - true/false
 */
bool See3CAM_CU135M_H01R1::getAutoExposureLowerLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint expLowerLimit;

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = GET_AUTO_EXPOSURE_LOWER_LIMIT_SEE3CAM_CU135M_H01R1_H;

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == GET_AUTO_EXPOSURE_LOWER_LIMIT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            expLowerLimit = (g_in_packet_buf[2] << 24) | (g_in_packet_buf[3] << 16) | (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            exposureLowerLimit = expLowerLimit;
            emit lowerLimitExposure(expLowerLimit);

            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setAutoExposureLowerLimit - setting lower limit for exposure
 * @param lowerLimit - mode to set lower limit for exposure
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::setAutoExposureLowerLimit(uint lowerLimit){

    exposureLowerLimit = lowerLimit;

    //Validating the limit values -> Lower auto exposure limit <= Upper auto exposure limit
    if(exposureLowerLimit > exposureUpperLimit)
    {
      emit indicateExposureValueRangeFailure("Failure", "Exposure lower limit should be smaller than Exposure upper limit");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = SET_AUTO_EXPOSURE_LOWER_LIMIT_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[3] = ((exposureLowerLimit & 0xFF000000) >> 24);
    g_out_packet_buf[4] = ((exposureLowerLimit & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = ((exposureLowerLimit & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = ((exposureLowerLimit & 0x000000FF) >> 0);


    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure","Set lower limit value failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == SET_AUTO_EXPOSURE_LOWER_LIMIT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            emit indicateCommandStatus("Success","Lower limit value set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getAutoExposureUpperLimit  - get current upper limit exposure from camera
 * @return - true/false
 */
bool See3CAM_CU135M_H01R1::getAutoExposureUpperLimit()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    uint expUpperLimit;
    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = GET_AUTO_EXPOSURE_UPPER_LIMIT_SEE3CAM_CU135M_H01R1_H; /* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {

            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == GET_AUTO_EXPOSURE_UPPER_LIMIT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            expUpperLimit = (g_in_packet_buf[2] << 24) | (g_in_packet_buf[3] << 16) | (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            exposureUpperLimit = expUpperLimit;
            emit upperLimitExposure(expUpperLimit);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setAutoExposureUpperLimit - setting upper limit for exposure
 * @param upperLimit - mode to set upper limit for exposure
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::setAutoExposureUpperLimit(uint upperLimit){

    exposureUpperLimit = upperLimit;

    //Validating the limit values -> Lower auto exposure limit <= Upper auto exposure limit
    if(exposureLowerLimit > exposureUpperLimit)
    {
      emit indicateExposureValueRangeFailure("Failure", "Exposure Upper limit should be smaller than Exposure upper limit");
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
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = SET_AUTO_EXPOSURE_UPPER_LIMIT_SEE3CAM_CU135M_H01R1_H;
    g_out_packet_buf[3] = ((exposureUpperLimit & 0xFF000000) >> 24);
    g_out_packet_buf[4] = ((exposureUpperLimit & 0x00FF0000) >> 16);
    g_out_packet_buf[5] = ((exposureUpperLimit & 0x0000FF00) >> 8);
    g_out_packet_buf[6] = ((exposureUpperLimit & 0x000000FF) >> 0);
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            emit indicateCommandStatus("Failure","Set upper limit value failed");
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == SET_AUTO_EXPOSURE_UPPER_LIMIT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            emit indicateCommandStatus("Success","Upper limit value set successfully");
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::setAntiFlickerMode - setting anti flicker mode
 * @param antiFlickerMode - mode to set [value of enum  - auto/50hz/60hz]
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::setAntiFlickerMode(antiFlickerDetection antiFlickerMode){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU135M_H01R1_H; /* set anti flicker command  */
    g_out_packet_buf[3] = antiFlickerMode; /* anti flicker mode to set */
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == SET_ANTI_FLICKER_DETECTION_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == SET_SUCCESS) {
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getAntiFlickerMode  - get current anti flicker mode from camera
 * @return - true/false
 */
bool See3CAM_CU135M_H01R1::getAntiFlickerMode()
{
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU135M_H01R1_H; /* get anti flicker command  */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6] == GET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1] == GET_ANTI_FLICKER_DETECTION_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6] == GET_SUCCESS) {
            emit antiFlickerModeRecieved(g_in_packet_buf[2]);
            return true;
        }
    }
    return false;
}

/**
 * @brief See3CAM_CU135M_H01R1::getStatistics - To get Gain & Exposure statistics when respective modes are in auto mode
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::getStatistics(){
    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    uint gainStat, exposureStat;
    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = READ_STATISTICS_SEE3CAM_CU135M_H01R1_H;
    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[8]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==READ_STATISTICS_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[8]==SET_SUCCESS) {
            exposureStat = (g_in_packet_buf[2] << 24) | (g_in_packet_buf[3] << 16) | (g_in_packet_buf[4] << 8) | (g_in_packet_buf[5] << 0);
            gainStat = (g_in_packet_buf[6] << 8) | (g_in_packet_buf[7] << 0);
            emit gainStatisticsRecieved(gainStat);
            emit exposureStatisticsRecieved(exposureStat);
            return true;
        }
    }
    return false;
}


/**
 * @brief See3CAM_CU135M_H01R1::setToDefault - set all the values to default in camera
 * @return true/false
 */
bool See3CAM_CU135M_H01R1::setToDefault(){

    // hid validation
    if(uvccamera::hid_fd < 0)
    {
        return false;
    }

    //Initialize buffers
    initializeBuffers();

    // fill buffer values
    g_out_packet_buf[1] = CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H; /* camera id */
    g_out_packet_buf[2] = SET_TO_DEFAULT_SEE3CAM_CU135M_H01R1_H; /* set to default command */

    // send request and get reply from camera
    if(uvc.sendHidCmd(g_out_packet_buf, g_in_packet_buf, BUFFER_LENGTH)){
        if (g_in_packet_buf[6]==SET_FAIL) {
            return false;
        } else if(g_in_packet_buf[0] == CAMERA_CONTROL_CU1330M_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[1]==SET_TO_DEFAULT_SEE3CAM_CU135M_H01R1_H &&
            g_in_packet_buf[6]==SET_SUCCESS) {
            return true;
        }
    }
    return false;
}
